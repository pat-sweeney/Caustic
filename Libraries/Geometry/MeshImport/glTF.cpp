//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//
// glTF 2.0 importer for Caustic. Supports:
// - .gltf (JSON) + .bin (binary buffer) format
// - .glb (single binary container) format
// - Triangle meshes with positions, normals, UVs, indices
// - PBR metallic-roughness material model
// - External texture image files
//
// Limitations (can be extended later):
// - No skeleton/animation support
// - No embedded base64 image data
//**********************************************************************
module;
#include <Windows.h>
#include <ShlWapi.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <memory>
#include <any>

module Geometry.MeshImport.glTF;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.ConvertStr;
import Base.Math.Vector;
import Geometry.Mesh.MaterialAttrib;
import Geometry.Mesh.IMaterialAttrib;
import Geometry.Mesh.RenderTypes;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.Mesh;
import Geometry.Mesh.MeshFuncs;
import Imaging.Color;
import Imaging.Image.IImage;
import Parsers.JSon.IJSonParser;
import Parsers.JSon.JSonParser;

namespace Caustic
{
    //**********************************************************************
    // Helper: Get a property from a JSON object map by key name
    //**********************************************************************
    static CRefObj<IJSonObj> GetProperty(IJSonObj* pObj, const std::string& key)
    {
        if (pObj == nullptr || pObj->GetType() != CJSonType::Object)
            return CRefObj<IJSonObj>(nullptr);
        auto pMap = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
        auto it = pMap->find(key);
        if (it != pMap->end())
            return it->second;
        return CRefObj<IJSonObj>(nullptr);
    }

    //**********************************************************************
    // Helper: Get array elements from a JSON array object
    //**********************************************************************
    static std::vector<CRefObj<IJSonObj>> GetArray(IJSonObj* pObj)
    {
        if (pObj == nullptr || pObj->GetType() != CJSonType::Array)
            return {};
        auto pVec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(pObj->GetValue());
        return *pVec;
    }

    //**********************************************************************
    // Helper: Get float value from a JSON object (handles int and float)
    //**********************************************************************
    static float GetFloat(IJSonObj* pObj, float defaultVal = 0.0f)
    {
        if (pObj == nullptr) return defaultVal;
        if (pObj->GetType() == CJSonType::Number)
            return std::any_cast<float>(pObj->GetValue());
        if (pObj->GetType() == CJSonType::Integer)
            return (float)std::any_cast<int>(pObj->GetValue());
        return defaultVal;
    }

    //**********************************************************************
    // Helper: Get int value from a JSON object
    //**********************************************************************
    static int GetInt(IJSonObj* pObj, int defaultVal = 0)
    {
        if (pObj == nullptr) return defaultVal;
        if (pObj->GetType() == CJSonType::Integer)
            return std::any_cast<int>(pObj->GetValue());
        if (pObj->GetType() == CJSonType::Number)
            return (int)std::any_cast<float>(pObj->GetValue());
        return defaultVal;
    }

    //**********************************************************************
    // Helper: Get string value from a JSON object
    //**********************************************************************
    static std::string GetString(IJSonObj* pObj, const std::string& defaultVal = "")
    {
        if (pObj == nullptr || pObj->GetType() != CJSonType::String)
            return defaultVal;
        return std::any_cast<std::string>(pObj->GetValue());
    }

    //**********************************************************************
    // Helper: Load binary buffer from file
    //**********************************************************************
    static std::vector<uint8_t> LoadBinaryBuffer(const std::wstring& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            return {};
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char*>(buffer.data()), size);
        return buffer;
    }

    //**********************************************************************
    // Helper: Extract folder path from a full file path
    //**********************************************************************
    static std::wstring GetFolderPath(const wchar_t* pFilename)
    {
        std::wstring path(pFilename);
        size_t lastSlash = path.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos)
            return path.substr(0, lastSlash + 1);
        return L"";
    }

    //**********************************************************************
    // Struct: glTFAccessor
    // Holds parsed accessor data from glTF
    //**********************************************************************
    struct glTFAccessor
    {
        int bufferView;
        int byteOffset;
        int componentType; // 5120=byte, 5121=ubyte, 5122=short, 5123=ushort, 5125=uint, 5126=float
        int count;
        std::string type; // "SCALAR", "VEC2", "VEC3", "VEC4", "MAT4"
    };

    //**********************************************************************
    // Struct: glTFBufferView
    //**********************************************************************
    struct glTFBufferView
    {
        int buffer;
        int byteOffset;
        int byteLength;
        int byteStride;
    };

    //**********************************************************************
    // Helper: Get number of components for a glTF accessor type
    //**********************************************************************
    static int TypeComponentCount(const std::string& type)
    {
        if (type == "SCALAR") return 1;
        if (type == "VEC2") return 2;
        if (type == "VEC3") return 3;
        if (type == "VEC4") return 4;
        if (type == "MAT4") return 16;
        return 0;
    }

    //**********************************************************************
    // Helper: Get byte size of a glTF component type
    //**********************************************************************
    static int ComponentTypeSize(int componentType)
    {
        switch (componentType)
        {
        case 5120: return 1; // byte
        case 5121: return 1; // unsigned byte
        case 5122: return 2; // short
        case 5123: return 2; // unsigned short
        case 5125: return 4; // unsigned int
        case 5126: return 4; // float
        default: return 0;
        }
    }

    //**********************************************************************
    // Helper: Read float data from a buffer using accessor/bufferView
    //**********************************************************************
    static std::vector<float> ReadFloatAccessor(
        const glTFAccessor& accessor,
        const std::vector<glTFBufferView>& bufferViews,
        const std::vector<std::vector<uint8_t>>& buffers)
    {
        const auto& bv = bufferViews[accessor.bufferView];
        const auto& buf = buffers[bv.buffer];
        int components = TypeComponentCount(accessor.type);
        int compSize = ComponentTypeSize(accessor.componentType);
        int stride = bv.byteStride > 0 ? bv.byteStride : components * compSize;
        int offset = bv.byteOffset + accessor.byteOffset;

        std::vector<float> result;
        result.reserve(accessor.count * components);

        for (int i = 0; i < accessor.count; i++)
        {
            const uint8_t* ptr = buf.data() + offset + i * stride;
            for (int c = 0; c < components; c++)
            {
                if (accessor.componentType == 5126) // float
                {
                    float val;
                    memcpy(&val, ptr + c * sizeof(float), sizeof(float));
                    result.push_back(val);
                }
                else if (accessor.componentType == 5121) // unsigned byte (normalize to 0-1)
                {
                    result.push_back(ptr[c] / 255.0f);
                }
                else if (accessor.componentType == 5123) // unsigned short (normalize)
                {
                    uint16_t val;
                    memcpy(&val, ptr + c * sizeof(uint16_t), sizeof(uint16_t));
                    result.push_back(val / 65535.0f);
                }
            }
        }
        return result;
    }

    //**********************************************************************
    // Helper: Read unsigned int index data from a buffer
    //**********************************************************************
    static std::vector<uint32_t> ReadIndexAccessor(
        const glTFAccessor& accessor,
        const std::vector<glTFBufferView>& bufferViews,
        const std::vector<std::vector<uint8_t>>& buffers)
    {
        const auto& bv = bufferViews[accessor.bufferView];
        const auto& buf = buffers[bv.buffer];
        int compSize = ComponentTypeSize(accessor.componentType);
        int stride = bv.byteStride > 0 ? bv.byteStride : compSize;
        int offset = bv.byteOffset + accessor.byteOffset;

        std::vector<uint32_t> result;
        result.reserve(accessor.count);

        for (int i = 0; i < accessor.count; i++)
        {
            const uint8_t* ptr = buf.data() + offset + i * stride;
            uint32_t val = 0;
            switch (accessor.componentType)
            {
            case 5121: val = *ptr; break;
            case 5123: { uint16_t v; memcpy(&v, ptr, 2); val = v; break; }
            case 5125: memcpy(&val, ptr, 4); break;
            }
            result.push_back(val);
        }
        return result;
    }

    //**********************************************************************
    // Helper: Check if filename has .glb extension (case-insensitive)
    //**********************************************************************
    static bool IsGLBFile(const wchar_t* pFilename)
    {
        std::wstring fn(pFilename);
        if (fn.size() < 4)
            return false;
        std::wstring ext = fn.substr(fn.size() - 4);
        for (auto& c : ext) c = towlower(c);
        return ext == L".glb";
    }

    //**********************************************************************
    // GLB constants
    //**********************************************************************
    static const uint32_t c_GLBMagic = 0x46546C67;   // "glTF"
    static const uint32_t c_GLBVersion = 2;
    static const uint32_t c_ChunkTypeJSON = 0x4E4F534A; // "JSON"
    static const uint32_t c_ChunkTypeBIN  = 0x004E4942; // "BIN\0"

    //**********************************************************************
    // Function: LoadglTF
    // See MeshImportglTF.ixx for description
    //**********************************************************************
    CRefObj<IMesh> MeshImport::LoadglTF(const wchar_t* pFilename)
    {
        std::wstring folder = GetFolderPath(pFilename);

        CRefObj<IJSonParser> spParser = CreateJSonParser();
        CRefObj<IJSonObj> spRoot;

        // GLB embedded binary chunk (empty for .gltf files)
        std::vector<uint8_t> glbBinChunk;
        bool isGLB = IsGLBFile(pFilename);

        if (isGLB)
        {
            // Load entire .glb file
            std::vector<uint8_t> fileData = LoadBinaryBuffer(std::wstring(pFilename));
            if (fileData.size() < 12)
                CT(E_FAIL);

            // Parse GLB header
            uint32_t magic, version, totalLength;
            memcpy(&magic, fileData.data(), 4);
            memcpy(&version, fileData.data() + 4, 4);
            memcpy(&totalLength, fileData.data() + 8, 4);
            if (magic != c_GLBMagic || version != c_GLBVersion)
                CT(E_FAIL);
            if (totalLength > fileData.size())
                CT(E_FAIL);

            // Parse chunks
            size_t offset = 12;
            bool foundJSON = false;
            while (offset + 8 <= totalLength)
            {
                uint32_t chunkLength, chunkType;
                memcpy(&chunkLength, fileData.data() + offset, 4);
                memcpy(&chunkType, fileData.data() + offset + 4, 4);
                offset += 8;

                if (offset + chunkLength > totalLength)
                    CT(E_FAIL);

                if (chunkType == c_ChunkTypeJSON && !foundJSON)
                {
                    // Null-terminate the JSON for the parser
                    std::string jsonStr(reinterpret_cast<const char*>(fileData.data() + offset), chunkLength);
                    spRoot = spParser->ReadDOM(jsonStr.c_str());
                    foundJSON = true;
                }
                else if (chunkType == c_ChunkTypeBIN && glbBinChunk.empty())
                {
                    glbBinChunk.assign(fileData.data() + offset, fileData.data() + offset + chunkLength);
                }

                offset += chunkLength;
            }

            if (!foundJSON || spRoot == nullptr)
                CT(E_FAIL);
        }
        else
        {
            // Standard .gltf JSON file
            std::wstring fn(pFilename);
            spRoot = spParser->LoadDOM(fn);
            if (spRoot == nullptr)
                CT(E_FAIL);
        }

        // Parse buffers — for GLB, buffers without a URI use the embedded BIN chunk
        std::vector<std::vector<uint8_t>> buffers;
        auto spBuffersArr = GetProperty(spRoot, "buffers");
        if (spBuffersArr != nullptr)
        {
            for (auto& spBuf : GetArray(spBuffersArr))
            {
                auto spUri = GetProperty(spBuf, "uri");
                std::string uri = GetString(spUri);
                if (uri.empty())
                {
                    // No URI — use embedded GLB binary chunk
                    buffers.push_back(glbBinChunk);
                }
                else
                {
                    std::wstring bufPath = folder + str2wstr(uri);
                    buffers.push_back(LoadBinaryBuffer(bufPath));
                }
            }
        }

        // Parse bufferViews
        std::vector<glTFBufferView> bufferViews;
        auto spBVArr = GetProperty(spRoot, "bufferViews");
        if (spBVArr != nullptr)
        {
            for (auto& spBV : GetArray(spBVArr))
            {
                glTFBufferView bv;
                bv.buffer = GetInt(GetProperty(spBV, "buffer"));
                bv.byteOffset = GetInt(GetProperty(spBV, "byteOffset"));
                bv.byteLength = GetInt(GetProperty(spBV, "byteLength"));
                bv.byteStride = GetInt(GetProperty(spBV, "byteStride"));
                bufferViews.push_back(bv);
            }
        }

        // Parse accessors
        std::vector<glTFAccessor> accessors;
        auto spAccArr = GetProperty(spRoot, "accessors");
        if (spAccArr != nullptr)
        {
            for (auto& spAcc : GetArray(spAccArr))
            {
                glTFAccessor acc;
                acc.bufferView = GetInt(GetProperty(spAcc, "bufferView"));
                acc.byteOffset = GetInt(GetProperty(spAcc, "byteOffset"));
                acc.componentType = GetInt(GetProperty(spAcc, "componentType"));
                acc.count = GetInt(GetProperty(spAcc, "count"));
                acc.type = GetString(GetProperty(spAcc, "type"));
                accessors.push_back(acc);
            }
        }

        // Parse images
        std::vector<std::wstring> imagePaths;
        auto spImagesArr = GetProperty(spRoot, "images");
        if (spImagesArr != nullptr)
        {
            for (auto& spImg : GetArray(spImagesArr))
            {
                auto spUri = GetProperty(spImg, "uri");
                std::string uri = GetString(spUri);
                if (!uri.empty())
                {
                    imagePaths.push_back(folder + str2wstr(uri));
                }
                else
                {
                    // Embedded GLB image: extract from bufferView and write to temp file
                    int bvIdx = GetInt(GetProperty(spImg, "bufferView"), -1);
                    std::string mimeType = GetString(GetProperty(spImg, "mimeType"));
                    if (bvIdx >= 0 && bvIdx < (int)bufferViews.size())
                    {
                        auto& bv = bufferViews[bvIdx];
                        if (bv.buffer < (int)buffers.size())
                        {
                            const uint8_t* pData = buffers[bv.buffer].data() + bv.byteOffset;
                            int dataLen = bv.byteLength;

                            // Determine file extension from MIME type
                            std::wstring ext = L".bin";
                            if (mimeType == "image/png") ext = L".png";
                            else if (mimeType == "image/jpeg") ext = L".jpg";

                            // Write to temp file
                            wchar_t tempPath[MAX_PATH];
                            wchar_t tempFile[MAX_PATH];
                            GetTempPathW(MAX_PATH, tempPath);
                            GetTempFileNameW(tempPath, L"glb", 0, tempFile);

                            // Rename with proper extension
                            std::wstring finalPath = std::wstring(tempFile) + ext;
                            DeleteFileW(finalPath.c_str());
                            MoveFileW(tempFile, finalPath.c_str());

                            // Write image data
                            HANDLE hFile = CreateFileW(finalPath.c_str(), GENERIC_WRITE, 0, nullptr,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
                            if (hFile != INVALID_HANDLE_VALUE)
                            {
                                DWORD bytesWritten;
                                WriteFile(hFile, pData, (DWORD)dataLen, &bytesWritten, nullptr);
                                CloseHandle(hFile);
                                imagePaths.push_back(finalPath);
                            }
                            else
                            {
                                imagePaths.push_back(L"");
                            }
                        }
                        else
                        {
                            imagePaths.push_back(L"");
                        }
                    }
                    else
                    {
                        imagePaths.push_back(L"");
                    }
                }
            }
        }

        // Parse textures (texture → sampler + image source)
        std::vector<int> textureImageIndex;
        auto spTexturesArr = GetProperty(spRoot, "textures");
        if (spTexturesArr != nullptr)
        {
            for (auto& spTex : GetArray(spTexturesArr))
            {
                textureImageIndex.push_back(GetInt(GetProperty(spTex, "source"), -1));
            }
        }

        // Parse materials
        std::vector<CRefObj<IMaterialAttrib>> materials;
        auto spMatsArr = GetProperty(spRoot, "materials");
        if (spMatsArr != nullptr)
        {
            uint32_t materialID = 0;
            for (auto& spMat : GetArray(spMatsArr))
            {
                FRGBColor albedo(0.8f, 0.8f, 0.8f);
                float metallic = 0.0f;
                float roughness = 0.5f;

                // PBR metallic-roughness
                auto spPBR = GetProperty(spMat, "pbrMetallicRoughness");
                if (spPBR != nullptr)
                {
                    auto spBaseColor = GetProperty(spPBR, "baseColorFactor");
                    if (spBaseColor != nullptr)
                    {
                        auto arr = GetArray(spBaseColor);
                        if (arr.size() >= 3)
                            albedo = FRGBColor(GetFloat(arr[0]), GetFloat(arr[1]), GetFloat(arr[2]));
                    }
                    metallic = GetFloat(GetProperty(spPBR, "metallicFactor"), 1.0f);
                    roughness = GetFloat(GetProperty(spPBR, "roughnessFactor"), 1.0f);
                }

                CRefObj<IMaterialAttrib> spMaterial = CreatePBRMaterialAttrib(albedo, metallic, roughness, 1.0f);
                spMaterial->SetMaterialID(materialID);

                // Set material name
                std::string name = GetString(GetProperty(spMat, "name"), "material");
                spMaterial->SetName(name.c_str());

                // Load textures
                if (spPBR != nullptr)
                {
                    auto spBaseColorTex = GetProperty(spPBR, "baseColorTexture");
                    if (spBaseColorTex != nullptr)
                    {
                        int texIdx = GetInt(GetProperty(spBaseColorTex, "index"), -1);
                        if (texIdx >= 0 && texIdx < (int)textureImageIndex.size())
                        {
                            int imgIdx = textureImageIndex[texIdx];
                            if (imgIdx >= 0 && imgIdx < (int)imagePaths.size() && !imagePaths[imgIdx].empty())
                                spMaterial->SetTextureViaFilename(L"albedoTexture", imagePaths[imgIdx], EShaderAccess::PixelShader);
                        }
                    }

                    auto spMRTex = GetProperty(spPBR, "metallicRoughnessTexture");
                    if (spMRTex != nullptr)
                    {
                        int texIdx = GetInt(GetProperty(spMRTex, "index"), -1);
                        if (texIdx >= 0 && texIdx < (int)textureImageIndex.size())
                        {
                            int imgIdx = textureImageIndex[texIdx];
                            if (imgIdx >= 0 && imgIdx < (int)imagePaths.size() && !imagePaths[imgIdx].empty())
                                spMaterial->SetTextureViaFilename(L"metallicRoughnessTexture", imagePaths[imgIdx], EShaderAccess::PixelShader);
                        }
                    }
                }

                // Normal map
                auto spNormalTex = GetProperty(spMat, "normalTexture");
                if (spNormalTex != nullptr)
                {
                    int texIdx = GetInt(GetProperty(spNormalTex, "index"), -1);
                    if (texIdx >= 0 && texIdx < (int)textureImageIndex.size())
                    {
                        int imgIdx = textureImageIndex[texIdx];
                        if (imgIdx >= 0 && imgIdx < (int)imagePaths.size() && !imagePaths[imgIdx].empty())
                            spMaterial->SetTextureViaFilename(L"normalTexture", imagePaths[imgIdx], EShaderAccess::PixelShader);
                    }
                }

                // AO texture
                auto spOcclusionTex = GetProperty(spMat, "occlusionTexture");
                if (spOcclusionTex != nullptr)
                {
                    int texIdx = GetInt(GetProperty(spOcclusionTex, "index"), -1);
                    if (texIdx >= 0 && texIdx < (int)textureImageIndex.size())
                    {
                        int imgIdx = textureImageIndex[texIdx];
                        if (imgIdx >= 0 && imgIdx < (int)imagePaths.size() && !imagePaths[imgIdx].empty())
                            spMaterial->SetTextureViaFilename(L"aoTexture", imagePaths[imgIdx], EShaderAccess::PixelShader);
                    }
                }

                materials.push_back(spMaterial);
                materialID++;
            }
        }

        // Create the mesh
        CRefObj<IMesh> spMesh = CreateEmptyMesh();

        // Parse meshes
        auto spMeshesArr = GetProperty(spRoot, "meshes");
        if (spMeshesArr == nullptr)
            return spMesh;

        for (auto& spGltfMesh : GetArray(spMeshesArr))
        {
            auto spPrimitives = GetProperty(spGltfMesh, "primitives");
            if (spPrimitives == nullptr)
                continue;

            for (auto& spPrim : GetArray(spPrimitives))
            {
                auto spAttributes = GetProperty(spPrim, "attributes");
                if (spAttributes == nullptr)
                    continue;

                // Read vertex attributes
                std::vector<float> positions, normals, uvs;

                auto spPosIdx = GetProperty(spAttributes, "POSITION");
                if (spPosIdx != nullptr)
                {
                    int idx = GetInt(spPosIdx);
                    positions = ReadFloatAccessor(accessors[idx], bufferViews, buffers);
                }

                auto spNormIdx = GetProperty(spAttributes, "NORMAL");
                if (spNormIdx != nullptr)
                {
                    int idx = GetInt(spNormIdx);
                    normals = ReadFloatAccessor(accessors[idx], bufferViews, buffers);
                }

                auto spUVIdx = GetProperty(spAttributes, "TEXCOORD_0");
                if (spUVIdx != nullptr)
                {
                    int idx = GetInt(spUVIdx);
                    uvs = ReadFloatAccessor(accessors[idx], bufferViews, buffers);
                }

                // Read indices
                std::vector<uint32_t> indices;
                auto spIndicesIdx = GetProperty(spPrim, "indices");
                if (spIndicesIdx != nullptr)
                {
                    int idx = GetInt(spIndicesIdx);
                    indices = ReadIndexAccessor(accessors[idx], bufferViews, buffers);
                }

                // Build CGeomVertex array
                int numVerts = (int)(positions.size() / 3);
                std::vector<CGeomVertex> verts(numVerts);
                for (int i = 0; i < numVerts; i++)
                {
                    verts[i].pos = Vector3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
                    if (i * 3 + 2 < (int)normals.size())
                        verts[i].norm = Vector3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
                    else
                        verts[i].norm = Vector3(0.0f, 1.0f, 0.0f);
                    if (i * 2 + 1 < (int)uvs.size())
                        verts[i].uvs[0] = Vector2(uvs[i * 2], uvs[i * 2 + 1]);
                    verts[i].index = i;
                }

                // Build flat face index array (every 3 ints = one triangle)
                std::vector<int> faceIndices;
                if (!indices.empty())
                {
                    faceIndices.reserve(indices.size());
                    for (auto idx : indices)
                        faceIndices.push_back((int)idx);
                }
                else
                {
                    // Non-indexed: sequential vertex indices
                    faceIndices.reserve(numVerts);
                    for (int i = 0; i < numVerts; i++)
                        faceIndices.push_back(i);
                }

                // Determine material ID for this primitive
                int matIdx = GetInt(GetProperty(spPrim, "material"), -1);
                uint32_t materialID = (matIdx >= 0 && matIdx < (int)materials.size()) ? (uint32_t)matIdx : 0;

                CRefObj<ISubMesh> spSubMesh = CreateEmptySubMesh();
                spSubMesh->SetFromIndexedData(verts, faceIndices, materialID);

                // Parse morph targets (blend shapes) if present
                auto spTargetsArr = GetProperty(spPrim, "targets");
                if (spTargetsArr != nullptr)
                {
                    std::vector<std::vector<MorphTargetDelta>> morphTargets;
                    for (auto& spTarget : GetArray(spTargetsArr))
                    {
                        std::vector<float> posDelta, normDelta;

                        auto spPosIdx = GetProperty(spTarget, "POSITION");
                        if (spPosIdx != nullptr)
                            posDelta = ReadFloatAccessor(accessors[GetInt(spPosIdx)], bufferViews, buffers);

                        auto spNormIdx = GetProperty(spTarget, "NORMAL");
                        if (spNormIdx != nullptr)
                            normDelta = ReadFloatAccessor(accessors[GetInt(spNormIdx)], bufferViews, buffers);

                        std::vector<MorphTargetDelta> deltas(numVerts);
                        for (int v = 0; v < numVerts; v++)
                        {
                            if (v * 3 + 2 < (int)posDelta.size())
                                deltas[v].positionDelta = Vector3(posDelta[v * 3], posDelta[v * 3 + 1], posDelta[v * 3 + 2]);
                            else
                                deltas[v].positionDelta = Vector3(0, 0, 0);

                            if (v * 3 + 2 < (int)normDelta.size())
                                deltas[v].normalDelta = Vector3(normDelta[v * 3], normDelta[v * 3 + 1], normDelta[v * 3 + 2]);
                            else
                                deltas[v].normalDelta = Vector3(0, 0, 0);
                        }
                        morphTargets.push_back(std::move(deltas));
                    }

                    if (!morphTargets.empty())
                        spSubMesh->SetMorphTargets(morphTargets);
                }

                // Parse default morph weights from the mesh object
                auto spWeightsArr = GetProperty(spGltfMesh, "weights");
                if (spWeightsArr != nullptr)
                {
                    std::vector<float> defaultWeights;
                    for (auto& spW : GetArray(spWeightsArr))
                        defaultWeights.push_back(GetFloat(spW, 0.0f));
                    if (!defaultWeights.empty())
                        spSubMesh->SetMorphWeights(defaultWeights);
                }

                spMesh->AddSubMesh(spSubMesh);
            }
        }

        // Assign materials to the mesh
        if (!materials.empty())
            spMesh->SetMaterials(materials);

        return spMesh;
    }
}
