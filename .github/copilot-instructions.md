# Caustic — Copilot Instructions

## Build

- **IDE:** Visual Studio 2022 (v143 toolset), MSBuild
- **Solution:** `Caustic.sln`
- **Shared props:** `Caustic.props` defines all paths (`GithubRoot`, `CausticRoot`, `CausticFXCPath`) and external library references. Edit this to configure your environment.
- **C++ standard:** `/std:c++latest` with C++20 modules enabled (`EnableModules=true`)
- **Module files:** `.ixx` extension for module interface units; `.cpp` for implementations
- **Shaders:** HLSL files (`.vs`, `.ps`, `.cs`) compiled by FXC → `.cso`. `ParseShader.exe` reflects shaders into `.shi` metadata XML loaded at runtime.
- **No CI/CD pipeline** is configured.

### Environment variables

| Variable | Purpose |
|----------|---------|
| `CausticRoot` | Root of the Caustic source tree |
| `CausticFXCPath` | Path to FXC shader compiler |
| `AzureKinectSDKDir` | Azure Kinect SDK path (optional) |
| `UseNDI` | Enable NDI support (optional) |
| `UseOpenCV` | Enable OpenCV support (optional) |

## Testing

No external test framework. Tests are built into library modules and run via `Apps/UnitTester`:

```
UnitTester.exe -all              # Run all tests
UnitTester.exe -mesh             # Geometry/Mesh tests only
UnitTester.exe -imaging          # Image processing tests only
UnitTester.exe -math             # Math library tests only
UnitTester.exe -renderwindow     # Render window tests
UnitTester.exe -lex              # Lexer tests
UnitTester.exe -sceneimport      # Scene import tests
UnitTester.exe -audio            # Audio playback tests
UnitTester.exe -camera           # Camera tests
```

Each test suite is a class derived from `UnitTestSuite` defined in `UnitTest.ixx` alongside the library code. Tests are static `bool` functions registered in a `UnitTestEntry[]` array with a `RunUnitTests()` driver.

## Architecture

Caustic is a personal DirectX 11 graphics library. Everything lives under `Libraries/`, which contains the core engine, with `Apps/` and `Tools/` consuming it.

### Module naming

All library code uses C++20 modules. Module names mirror the folder path:

```
Libraries/{Category}/{Library}/{File}.ixx
  → export module {Category}.{Library}.{Name};
```

Example: `Libraries/Rendering/Caustic/IRenderer.ixx` → `export module Rendering.Caustic.IRenderer;`

### Library dependency hierarchy (bottom-up)

```
Base/Core → Base/Math → Imaging/Color → Geometry/* → Imaging/Image
  → Cameras/* → Parsers/* → Rendering/Caustic (core renderer)
    → SceneGraph, RenderGraph, RendererMarshaller → RenderWindow → Apps/Tools
```

### Key subsystems

- **Rendering/Caustic** — Core D3D11 renderer, shaders, textures, materials, lights
- **Rendering/SceneGraph** — Scene tree with typed element nodes (mesh, material, light, group, etc.)
- **Rendering/RenderGraph** — Node-based GPU pipeline composition
- **Rendering/RendererMarshaller** — Thread marshalling; all D3D calls go through a single render thread
- **Rendering/ECS** — Entity-Component-System (work-in-progress, alongside mature scene graph)
- **Geometry/Mesh + MeshImport** — Mesh data and importers (OBJ, PLY, glTF)
- **Imaging** — Image processing filters, video playback
- **Cameras** — Camera sources (webcam, Azure Kinect, NDI, virtual)

## Conventions

### Interface/implementation split

Every subsystem follows this pattern:

1. `I{Name}.ixx` — abstract interface (pure virtual `struct`, inherits `IRefCount`)
2. `{Name}.ixx` + `{Name}.cpp` — concrete implementation (class prefixed `C`)
3. Free-standing `Create{Name}()` factory function exported from the interface module

```cpp
// IMesh.ixx
struct IMesh : public IRefCount { ... };
CRefObj<IMesh> CreateEmptyMesh();

// Mesh.ixx / Mesh.cpp
class CMesh : public IMesh, public CRefCount { ... };
```

### Reference counting — CRefObj\<T\>

All engine objects use COM-style manual reference counting, not `std::shared_ptr`:

- `IRefCount` — base interface with `AddRef()`/`Release()`
- `CRefCount` — thread-safe base class (InterlockedIncrement/Decrement), deletes on zero
- `CRefObj<T>` — RAII smart pointer (like `CComPtr`). Use for all Caustic objects.
- `CComPtr<T>` — used only for actual COM/DirectX objects

### Naming conventions

| Kind | Convention | Example |
|------|-----------|---------|
| Interfaces | `I` prefix, PascalCase | `IRenderer`, `IMesh`, `ICamera` |
| Classes | `C` prefix, PascalCase | `CRenderer`, `CMesh`, `CRefCount` |
| Member variables | `m_` prefix | `m_spDevice`, `m_width` |
| Constants | `c_` prefix | `c_MeshComponent` |
| Enums | `E` prefix | `ESceneElemType`, `EImageType` |
| Namespaces | `Caustic` (engine), `CausticTestSuite` (tests) | |

### Error handling

HRESULT-based with exception wrapping:

- `CT(hr)` — throws `CausticException` if `FAILED(hr)`, captures file and line
- `CTWIN32(func)` — throws on Win32 failure using `GetLastError()`
- Both are defined in `Base.Core.Error`

### Documentation comments

Use NaturalDocs-style block comments with labeled sections:

```cpp
//**********************************************************************
// Interface: IRenderer
// Defines the main rendering interface.
//
// Method: BeginRender
// Begins a new render frame.
//
// Parameters:
// pCtx - render context
//
// Returns:
// S_OK on success
//**********************************************************************
```

### Shader conventions

- Entry points are named `VS()` and `PS()` (or `CS()` for compute)
- Structures follow: `VSInput` → `VSOutput`
- Common semantic suffixes: `posOS` (object space), `posWS` (world space), `posPS` (projection space), `normOS`, `uvs`
- Shaders `#include "defs.h"` for shared definitions

### Creating a new Caustic application

Follow the pattern in `Samples/CausticTemplate/`:

```cpp
Caustic::SystemStartup();
auto spCausticFactory = Caustic::CreateCausticFactory();
auto spRenderWindow = Caustic::CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
    renderCallback, prePresentCallback);
auto spSceneFactory = Caustic::CreateSceneFactory();
// Build scene graph, load content, run message loop
```

### Factory pattern

`ICausticFactory` and `ISceneFactory` are the primary object creation APIs. Always use factory methods (`Create*()`) rather than constructing objects directly.
