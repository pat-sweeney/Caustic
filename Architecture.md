# Caustic — Architecture Guide

> Personal graphics library for building DirectX 11 rendering applications.
> Windows-only · C++20 modules · Visual Studio 2022 (v143) · MIT License

---

## Repository Layout

```
Caustic/
├── Apps/                  # End-user applications
├── Libraries/             # Core engine libraries (the heart of Caustic)
│   ├── Audio/             #   Audio playback
│   ├── Base/              #   Core utilities & math
│   ├── Cameras/           #   Camera input sources
│   ├── Geometry/          #   Mesh, importers, data structures
│   ├── Imaging/           #   Image processing, filters, video
│   ├── Interop/           #   .NET/C++-CLI wrappers
│   ├── Interpreter/       #   Scheme interpreter (WIP)
│   ├── Parsers/           #   JSON, lexer, phonemes
│   └── Rendering/         #   Renderer, scene graph, shaders, ECS
├── Samples/               # Starter templates
├── Tools/                 # Utilities (shader compiler, doc tools, etc.)
├── External/              # Third-party deps (ImGui, ImPlot, NDI, etc.)
├── docs/                  # NaturalDocs generated documentation
├── ModelData/             # Test model data
├── UnitTestData/          # Test data files
├── Caustic.sln            # Main Visual Studio solution
├── Caustic.props          # Shared MSBuild property sheet (paths, libs)
└── README.md              # Build instructions & dependency setup
```

---

## Build System

- **Solution:** `Caustic.sln` — Visual Studio 2022 / MSBuild
- **Toolset:** v143, Windows SDK 10.0, C++ Language Standard: `stdcpplatest`
- **C++20 Modules:** Enabled (`EnableModules=true`) for all core C++ projects. Module interface files use `.ixx` extension.
- **Shaders:** HLSL `.vs`/`.ps`/`.cs` files compiled via FXC. `ParseShader.exe` reflects shaders and generates `.shi` metadata XML. Compiled shader objects are `.cso` files.
- **Shared props:** `Caustic.props` defines paths (`GithubRoot`, `CausticRoot`, `CausticFXCPath`) and external library references.
- **No CI/CD** pipeline is configured.

### Key Environment Variables

| Variable | Purpose |
|----------|---------|
| `CausticRoot` | Root of the Caustic source tree |
| `CausticFXCPath` | Path to FXC shader compiler |
| `AzureKinectSDKDir` | Azure Kinect SDK path |
| `UseNDI` | Enable NDI support (`true`/`false`) |
| `UseOpenCV` | Enable OpenCV support (`true`/`false`) |

### External Dependencies

| Dependency | Usage |
|------------|-------|
| **ImGui** (docking branch) | All GUI applications |
| **ImPlot** | Plotting in tools |
| **DirectX 11** | Core rendering |
| **OpenCV 4.7** | Face detection/landmarks (optional) |
| **Azure Kinect SDK** | Depth camera support (optional) |
| **NDI SDK** | Network video streaming (optional) |
| **WPFDXInterop** | WPF ↔ DirectX interop |
| **NaturalDocs** | Documentation generation |

---

## Module System & Naming Conventions

All library code uses C++20 modules. The module name mirrors the folder path:

```
Libraries/{Category}/{Library}/{File}.ixx
  → export module {Category}.{Library}.{Name};
```

**Examples:**
- `Libraries/Base/Core/Error.ixx` → `export module Base.Core.Error;`
- `Libraries/Rendering/Caustic/Renderer.ixx` → `export module Rendering.Caustic.Renderer;`
- `Libraries/Geometry/Mesh/IMesh.ixx` → `export module Geometry.Mesh.IMesh;`

**Naming conventions:**
- `I*.ixx` — Interface/contract (pure virtual classes): `ICamera`, `IMesh`, `IRenderer`
- Non-`I` `.ixx` — Implementation: `Camera`, `Mesh`, `Renderer`
- `UnitTest.ixx` / `UnitTests.ixx` — Test modules
- `.cpp` files contain method implementations for the corresponding `.ixx` module

**Notable exceptions:**
- `Rendering/Caustic/Caustic.ixx` exports `Caustic.Base` (aggregates all Caustic rendering exports)
- `Cameras/VirtualCam/*.ixx` exports `Cameras.VirtualCamera.*`

---

## Core Patterns

### Reference Counting (COM-style)

All engine objects use manual reference counting with an RAII smart pointer:

```cpp
// Interface — Base.Core.IRefCount
struct IRefCount {
    virtual uint32_t AddRef() = 0;
    virtual uint32_t Release() = 0;  // deletes at zero
};

// Base class — Base.Core.RefCount
class CRefCount {
    ULONG m_RefCnt;  // thread-safe via InterlockedIncrement/Decrement
};

// Smart pointer — CRefObj<T> (similar to CComPtr)
CRefObj<IRenderer> spRenderer = CreateRenderer(...);
// AddRef on copy, Release on destruction
```

### Error Handling

HRESULT-based with exception wrapping:

```cpp
// Base.Core.Error
class CausticException : public std::exception {
    HRESULT m_hr;
    std::string m_fn;   // source file
    int m_line;          // line number
};

CT(hr);          // throws CausticException if FAILED(hr)
CTWIN32(func);   // throws on Win32 failure using GetLastError()
```

### Interface/Implementation Split

Every subsystem follows the pattern:
1. `I{Name}.ixx` — defines the abstract interface
2. `{Name}.ixx` + `{Name}.cpp` — concrete implementation (typically prefixed `C`)
3. Free-standing `Create{Name}()` factory function exported from the interface module

```cpp
// IMesh.ixx
struct IMesh : public IRefCount { ... };
CRefObj<IMesh> CreateEmptyMesh();

// Mesh.ixx
class CMesh : public IMesh, public CRefCount { ... };
```

---

## Library Dependency Graph

```
Base/Core  ←── Base/Math
   ↑              ↑
   │         Imaging/Color
   │              ↑
   │       Geometry/GeomDS, Geometry/Rast
   │              ↑
   │       Imaging/Image ──→ Imaging/Video
   │              ↑
   │       Geometry/Mesh ──→ Geometry/MeshImport
   │              ↑
   │       Cameras/* (AzureKinect, WebCamera, NDI, VirtualCam)
   │              ↑
   ├── Parsers/* (Lex, JSon, Phonemes)
   │              ↑
   └── Rendering/Caustic (core renderer)
              ↑
       ┌──────┼──────────────┐
       │      │              │
  SceneGraph  RenderGraph  RendererMarshaller
       │      │              │
       └──────┼──────────────┘
              ↑
        RenderWindow
              ↑
         Apps / Tools
```

---

## Library Reference

### Base (`Libraries/Base/`)

| Module | Key Types | Purpose |
|--------|-----------|---------|
| `Base.Core.Core` | — | Foundational includes/macros |
| `Base.Core.IRefCount` | `IRefCount`, `CRefObj<T>` | Reference counting interface + smart pointer |
| `Base.Core.RefCount` | `CRefCount` | Thread-safe ref count base class |
| `Base.Core.Error` | `CausticException`, `CT()`, `CTWIN32()` | Exception/error handling |
| `Base.Core.CritSec` | `CCritSec` | Critical section wrapper |
| `Base.Core.Event` | `CEvent` | Win32 event wrapper |
| `Base.Core.Timer` | `CTimer` | High-resolution timer |
| `Base.Core.ConvertStr` | `str2wstr()`, `wstr2str()` | String conversion helpers |
| `Base.Core.BlockAllocator` | `CBlockAllocator` | Pool allocator |
| `Base.Core.ISerialize` | `ISerialize` | Serialization interface |
| `Base.Core.Property` | `CProperty` | Property system |
| `Base.Math.Vector` | `Vector2`, `Vector3`, `Vector4` | Vector math |
| `Base.Math.Matrix` | `Matrix4x4`, `Matrix4x3`, `Matrix3x3` | Matrix math |
| `Base.Math.BBox` | `BBox3` | Axis-aligned bounding box |
| `Base.Math.Ray` | `Ray3` | Ray representation |
| `Base.Math.Quaternion` | `Quaternion` | Quaternion math |

### Geometry (`Libraries/Geometry/`)

| Module | Key Types | Purpose |
|--------|-----------|---------|
| `Geometry.Mesh.IMesh` | `IMesh`, `ISubMesh` | Mesh interface (vertices, faces, submeshes) |
| `Geometry.Mesh.IMeshConstructor` | `IMeshConstructor` | Procedural mesh building |
| `Geometry.Mesh.IMaterialAttrib` | `IMaterialAttrib` | Material properties (colors, textures, scalars) |
| `Geometry.Mesh.RenderTypes` | `CGeomVertex` | Vertex format for rendering |
| `Geometry.Mesh.MeshFuncs` | `CreateSphere()`, etc. | Common shape generators |
| `Geometry.MeshImport` | `MeshImport::LoadObj()`, `LoadPLY()`, `LoadglTF()` | File format importers (OBJ, PLY, Lightwave, glTF) |
| `Geometry.GeomDS.*` | `IKDTree`, `IPath2`, `IPolygon` | Geometric data structures |
| `Geometry.Rast.Bresenham` | `Bresenham` | Line/circle rasterization |

### Imaging (`Libraries/Imaging/`)

| Module | Key Types | Purpose |
|--------|-----------|---------|
| `Imaging.Color` | `FRGBColor`, `FRGBAColor` | Color types |
| `Imaging.Image.IImage` | `IImage`, `EImageType` | Image interface (RGBA, grayscale, depth, etc.) |
| `Imaging.Image.ImageFilter.*` | Various filters | BoxFilter, GaussianBlur, Sobel, Median, Scale, Rotate, etc. |
| `Imaging.Image.IGPUPipeline` | `IGPUPipeline` | GPU-accelerated image processing pipeline |
| `Imaging.Video.IVideo` | `IVideo` | Video file playback |

### Cameras (`Libraries/Cameras/`)

| Module | Purpose |
|--------|---------|
| `Cameras.CameraBase.ICamera` | Abstract camera interface |
| `Cameras.WebCamera.*` | Webcam capture via Media Foundation |
| `Cameras.AzureKinect.*` | Azure Kinect depth/color/skeleton |
| `Cameras.NDIStream.*` | NDI network video input |
| `Cameras.VirtualCamera.*` | Synthetic/virtual camera source |

### Rendering (`Libraries/Rendering/`)

| Module | Key Types | Purpose |
|--------|-----------|---------|
| `Rendering.Caustic.IRenderer` | `IRenderer` | Core D3D11 renderer interface |
| `Rendering.Caustic.ICausticFactory` | `ICausticFactory` | Central factory for all rendering objects |
| `Rendering.Caustic.IShader` | `IShader` | Shader wrapper (VS/PS/CS) |
| `Rendering.Caustic.IShaderMgr` | `IShaderMgr` | Shader registry and lookup |
| `Rendering.Caustic.ITexture` | `ITexture` | GPU texture wrapper |
| `Rendering.Caustic.ISampler` | `ISampler` | Texture sampler state |
| `Rendering.Caustic.IRenderMaterial` | `IRenderMaterial` | GPU-bound material (shader + textures) |
| `Rendering.Caustic.IRenderMesh` | `IRenderMesh` | GPU vertex/index buffers |
| `Rendering.Caustic.IRenderable` | `IRenderable` | Drawable object (mesh + material + transform) |
| `Rendering.Caustic.ICamera` | `ICamera` | Render camera (view/projection) |
| `Rendering.Caustic.ITrackball` | `ITrackball` | Mouse-driven camera orbit control |
| `Rendering.Caustic.ILight` | `ILight`, `IPointLight`, `ISpotLight`, `IDirectionalLight` | Light types |
| `Rendering.SceneGraph.*` | `ISceneGraph`, `ISceneElem`, `ISceneFactory` | Scene graph tree with typed element nodes |
| `Rendering.RenderGraph.*` | `IRenderGraph`, `IRenderGraphNode` | Node-based render pipeline |
| `Rendering.RenderWindow.*` | `IRenderWindow` | Window management + input + render loop |
| `Rendering.RendererMarshaller.*` | `IRendererMarshaller` | Thread marshalling for render calls |
| `Rendering.ECS.*` | `IECS`, `ECSManager` | Entity-Component-System (work in progress) |
| `Rendering.GuiControls.Common` | — | ImGui helper controls |

### Parsers (`Libraries/Parsers/`)

| Module | Purpose |
|--------|---------|
| `Parsers.Lex.*` | Lexical analyzer / tokenizer |
| `Parsers.JSon.*` | JSON parser, tree, and GUI inspector |
| `Parsers.Phonemes.*` | Phoneme timing data parser |

---

## Rendering Pipeline

### Initialization Flow

```
1. Caustic::SystemStartup()
2. CreateCausticFactory()           → ICausticFactory singleton
3. CreateRenderWindow(hwnd, ...)    → IRenderWindow
   ├── Creates IRendererMarshaller  (owns the render thread)
   ├── Creates IRenderer            (D3D11 device, context, swap chain)
   ├── Creates ISceneGraph          (root group node)
   ├── Creates ICamera + ITrackball (default camera)
   └── Loads shaders from folder    (.shi metadata + .cso compiled shaders)
```

### Frame Rendering

```
RenderLoop() → RenderFrame() each frame:
  1. Clear render target + depth buffer
  2. RenderScene()
     ├── Pass: Shadow    — render shadow maps for shadow-casting lights
     ├── Pass: Opaque    — render opaque objects
     ├── Pass: Transparent — render transparent objects (sorted)
     └── Pass: Environment — environment/skybox
  3. Client render callback (custom drawing)
  4. Client pre-present callback (ImGui UI)
  5. Present swap chain
```

### Scene Graph Traversal

```
ISceneGraph::Render(IRenderer, callback)
  └── Root ISceneGroupElem::Render(SceneCtx)
       ├── Push transform (multiply parent × local)
       ├── For each child:
       │   ├── ISceneMaterialElem  → pushes shader + material into SceneCtx
       │   ├── ISceneLightCollectionElem → registers lights
       │   ├── ISceneMeshElem     → converts IMesh → IRenderMesh, draws
       │   ├── ISceneCubeElem     → draws unit cube
       │   ├── ISceneSphereElem   → draws sphere
       │   ├── ISceneCylinderElem → draws cylinder
       │   ├── ISceneLineElem     → draws line
       │   ├── ISceneOverlay2DElem → draws 2D overlay
       │   ├── ISceneComputeShaderElem → dispatches compute shader
       │   ├── ISceneMarchingCubesElem → marching cubes isosurface
       │   ├── ISceneLevelOfDetailElem → selects child by camera distance
       │   ├── ISceneCustomRenderElem → user callback
       │   └── ISceneGroupElem   → recurse
       └── Pop transform
```

### Mesh Loading Path (OBJ example)

```
MeshImport::LoadObj(filename)
  1. Read entire file into memory
  2. CObjParser::ParseAscii()
     a. Parse "mtllib" → load .mtl file → ParseMaterial() → m_matmap
     b. Parse "v"  → vertex positions
     c. Parse "vn" → vertex normals
     d. Parse "vt" → texture coordinates
     e. Parse "f"  → faces (triangles/quads) via ParseFace()
     f. Parse "usemtl" / "g" / "s" → flush submesh, start new one
  3. If no normals specified → ComputeNormals()
  4. Return IMesh with submeshes + materials
```

### Mesh Loading Path (glTF example)

```
MeshImport::LoadglTF(filename)
  1. Parse .gltf JSON via JSon parser → DOM tree
  2. Load binary .bin buffer(s) from URI references
  3. Parse bufferViews and accessors (stride, offset, component type)
  4. Parse materials:
     a. pbrMetallicRoughness → baseColorFactor, metallicFactor, roughnessFactor
     b. Texture references → albedo, metallicRoughness, normal, occlusion maps
     c. CreatePBRMaterialAttrib() → sets pbrModel=1.0 marker scalar
  5. Parse mesh primitives:
     a. Read POSITION, NORMAL, TEXCOORD_0 via accessors → CGeomVertex array
     b. Read indices → flat face index array
     c. CreateSubMesh() with materialID linkage
  6. SetMaterials() on mesh → return IMesh
```

### PBR Material System

Materials in Caustic use a property-bag model (`IMaterialAttrib`) with named colors, scalars, and textures. PBR extends this without changing the interface:

- **`CreatePBRMaterialAttrib()`** — factory that sets `albedo` (color), `metallic`, `roughness`, `ao` (scalars), and `pbrModel=1.0` (marker scalar)
- **Shader selection** — `CRenderSubMesh::Render()` checks the `pbrModel` scalar: if > 0 → selects the `PBR` shader, otherwise falls through to Default/Textured logic
- **PBR shader** (`PBR.ps`) — Cook-Torrance BRDF with:
  - GGX normal distribution function
  - Schlick-GGX geometry function (Smith method)
  - Fresnel-Schlick approximation
  - Cotangent-frame normal mapping (using screen-space derivatives — no per-vertex tangents needed)
  - CSM shadow support
  - Reinhard tone mapping + gamma correction
- **Texture slots**: albedoTexture (t0), shadowMapTexture (t1), metallicRoughnessTexture (t2), normalTexture (t3), aoTexture (t4)
- **glTF convention**: metallic-roughness texture packs roughness in G channel, metallic in B channel

---

## Shader System

Shaders are HLSL files compiled by FXC:

| Extension | Type |
|-----------|------|
| `.vs` | Vertex shader |
| `.ps` | Pixel shader |
| `.cs` | Compute shader |
| `.xml` | Shader metadata (parsed by `ParseShader.exe` → `.shi`) |
| `.shi` | Compiled shader info (loaded at runtime by `ShaderMgr`) |
| `.cso` | Compiled shader object (loaded at runtime) |

**Key shaders in `Libraries/Rendering/Caustic/`:**
- `Default` — basic rendering
- `Textured` / `TexturedWithShadow` — textured surfaces
- `ObjShader` — OBJ file material rendering
- `PBR` — physically based rendering (Cook-Torrance BRDF, metallic-roughness workflow)
- `ShadowMap` — shadow map generation
- `Line` — line rendering
- `PointCloud` — point cloud rendering
- `DrawNormal` — normal visualization
- `Overlay2D` — screen-space overlays
- `InfinitePlane` — ground plane
- `ScreenQuad` / `FullQuad` — full-screen passes

---

## Applications

| App | Description |
|-----|-------------|
| **MeshViewer** | Full scene graph editor/viewer. Loads OBJ/PLY meshes, inspects scene nodes, materials, lights. ImGui UI. |
| **GPUPipelineView** | Azure Kinect GPU pipeline viewer with depth processing, skeleton rendering, and ImGui controls. |
| **Ditto** | Lip-sync / face-warp video tool using phoneme timing and face landmarks. |
| **Fomit** | Azure Kinect depth-to-point-cloud demo using render graph. |
| **WebcamToNDI** | Webcam-to-NDI broadcaster with ImGui docking UI. |
| **CameraOverlay** | Always-on-top webcam overlay window (plain Win32). |
| **SimpleImageViewer** | Webcam / Azure Kinect image viewer with filter toggles. |
| **UnitTester** | Command-line test runner for library unit tests (`-mesh`, `-imaging`, `-math`, etc.). |
| **SlideShow** | Python/Tkinter photo slideshow from Synology NAS (unrelated to Caustic engine). |

## Tools

| Tool | Description |
|------|-------------|
| **ParseShader** | Reflects HLSL shaders → XML metadata for runtime loading. |
| **CompileShader** | Build helper for shader compilation. |
| **ModuleBrowser** | Prints C++20 module dependency tree from `.module.json` files. |
| **PostProcDoc** | Post-processes NaturalDocs HTML for Caustic-specific link tags. |
| **FMMTest** | ImGui/ImPlot demo with JSON tree view and plotting. |
| **SplitVideo** | Video/phoneme processing and face-warp data extraction. |
| **TestSamplers** | Generates filter/sampler comparison images. |

---

## Testing

There is no external test framework. Tests are built into library modules and run via `Apps/UnitTester`:

```
UnitTester.exe -all              # Run all tests
UnitTester.exe -mesh             # Geometry/Mesh tests
UnitTester.exe -imaging          # Image processing tests
UnitTester.exe -math             # Math library tests
UnitTester.exe -renderwindow     # Render window tests
UnitTester.exe -lex              # Lexer tests
UnitTester.exe -sceneimport      # Scene import tests
UnitTester.exe -audio            # Audio playback tests
UnitTester.exe -camera           # Camera tests
```

Test modules follow the pattern `{Library}.UnitTests` in `UnitTest.ixx` files alongside the library code.

---

## Creating a New Caustic Application

Standard pattern (see `Samples/CausticTemplate/`):

```cpp
import Caustic.Base;
import Base.Core.Core;
import Rendering.Caustic.ICausticFactory;
import Rendering.RenderWindow.IRenderWindow;
import Rendering.SceneGraph.ISceneFactory;

// 1. Initialize
Caustic::SystemStartup();
auto spCausticFactory = Caustic::CreateCausticFactory();

// 2. Create render window (sets up D3D, scene graph, camera)
auto spRenderWindow = Caustic::CreateImguiRenderWindow(
    hwnd, viewport, shaderFolder,
    [](IRenderer* pRenderer, IRenderCtx* pCtx, int pass) {
        // render callback — called each frame per pass
    },
    [](IRenderer* pRenderer) {
        // pre-present callback — draw ImGui UI here
    }
);

// 3. Build scene graph
auto spSceneFactory = Caustic::CreateSceneFactory();
auto spMeshElem = spSceneFactory->CreateMeshElem();
auto spMaterialElem = spSceneFactory->CreateMaterialElem();
auto spLightElem = spSceneFactory->CreateLightCollectionElem();

// 4. Load content
auto spMesh = Caustic::MeshImport::LoadObj(filename, nullptr);
spMeshElem->SetMesh(spMesh);

// 5. Assemble scene tree
spMaterialElem->AddChild(spMeshElem);
spRenderWindow->GetSceneGraph()->AddChild(spMaterialElem);

// 6. Run message loop (rendering happens on marshaller thread)
```

---

## Key Design Decisions

- **COM-style ref counting** over `std::shared_ptr` — matches DirectX/Windows patterns
- **C++20 modules** over headers — faster builds, explicit dependency graph
- **Single render thread** — all D3D calls marshalled via `RendererMarshaller`
- **Scene graph** for high-level scene composition; **Render graph** for GPU pipeline composition
- **Factory pattern** — `ICausticFactory` and `ISceneFactory` are the primary object creation APIs
- **ECS** exists but is work-in-progress alongside the mature scene graph
