//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Imaging\Image\Image.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"

// The IGPUPipeline object is used to create a graph of GPU shaders that will process
// images.
// A pipeline is built out of a series of IGPUImageNode's. Each node is conceptually
// just a pixel shader to be run on its input image data.
// The general structure of a GPU graph looks something like:
//
//   IGPUImageSourceNode         IGPUImageNode              IGPUImageSinkNode
//  +------------+              +--------------+           +------------------+
//  |            |----------->  |              |---------> |                  |
//  +------------+              +--------------+           +------------------+
//
// There can be multiple IGPUImageSourceNodes and IGPUImageNodes. IGPUImageNodes can
// have multiple input sources but always generate a single output.
// The graph uses a pull model. Each node will call GetOutputTexture() on its source
// nodes, which in turn will run Process on that node.
namespace Caustic
{
    struct IGPUPipeline;
    struct IGPUImageNode : public IRefCount
    {
        virtual uint32 GetNumberInputs() = 0;
        virtual void SetShader(IShader *pShader) = 0;
        virtual void GetShader(IShader **ppShader) = 0;
        virtual void GetInput(uint32 index, IGPUImageNode **ppNode) = 0;
        virtual void SetInput(uint32 index, IGPUImageNode *pNode) = 0;
        virtual void SetOutputSize(uint32 width, uint32 height) = 0;
        virtual uint32 GetOutputWidth() = 0;
        virtual uint32 GetOutputHeight() = 0;
        virtual void GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture) = 0;
        virtual void Process(IGPUPipeline *pPipeline) = 0;
    };

    struct IGPUImageSourceNode : public IGPUImageNode
    {
        virtual void SetSource(IGPUPipeline *pPipeline, IImage *pSource) = 0;
    };

    struct IGPUImageSinkNode : public IGPUImageNode
    {
        virtual void GetOutput(IGPUPipeline *pPipeline, IImage **ppSource) = 0;
    };

    // Define list of predefined filter names
    static const wchar_t *g_GaussianVerticalFilter = L"GaussianVertical"; // Name of GPU node that performs a gaussian blur in the vertical direction
    static const wchar_t *g_GaussianHorizontalFilter = L"GaussianHorizontal"; // Name of GPU node that performs a gaussian blur in the horizontal direction
    static const wchar_t *g_RawCopyFilter = L"RawCopy"; // Name of GPU node that performs a raw copy of the data

    struct IGPUPipeline : public IRefCount
    {
        virtual void RenderQuad(IShader *pShader) = 0;
        virtual void Process() = 0;
        virtual void CreateSourceNode(IImage *pImage, IGPUImageSourceNode **ppNewNode) = 0;
        virtual void CreateSinkNode(uint32 numInputs, IShader *pShader, IGPUImageSinkNode **ppNewNode) = 0;
        virtual void CreateNode(uint32 numInputs, IShader *pShader, IGPUImageNode **ppNewNode) = 0;
        virtual void CreatePredefinedNode(wchar_t *pShaderName, IGPUImageNode **ppNewNode) = 0;
        virtual void CreateShader(BYTE *pShaderCode, uint32 shaderCodeSize, ShaderParam *pShaderParams, uint32 shaderParamSize, IShader **ppShader) = 0;
    };
    extern void CreateGPUPipeline(IRenderer *pRenderer, IGPUPipeline **ppPipeline);
}
