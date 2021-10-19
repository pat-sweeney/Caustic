//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Imaging.Image.IImage;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include <varargs.h>

namespace Caustic
{
    struct IGPUPipeline;

    //**********************************************************************
    // Interface: IGPUPipelineNode
    // Defines the base class for each node in a GPU pipeline
    //**********************************************************************
    struct IGPUPipelineNode : public IRefCount
    {
        //**********************************************************************
        // Method: SetName
        // pName - Sets the debug name for a GPU node
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) = 0;

        //**********************************************************************
        // Method: IsEnabled
        // Returns true if node is enabled (i.e. will run) in the pipeline
        //**********************************************************************
        virtual bool IsEnabled() = 0;

        //**********************************************************************
        // Method: Enable
        // Turns on a GPU node. If enabled when GPUPipeline::Process() is called
        // this node will run (along with all its proceeding pipeline nodes).
        // Otherwise, the node is not executed, and its Output texture is undefined.
        //**********************************************************************
        virtual void Enable() = 0;

        //**********************************************************************
        // Method: Disable
        // Turns off the GPU node. This is the oppside of <IGPUPipelineNode::Enable>
        //**********************************************************************
        virtual void Disable() = 0;

        //**********************************************************************
        // Method: SetShader
        // Assigns a pixel shader to the node. This shader will be run on an
        // image when the node is processed.
        //
        // Parameters:
        // pShader - pixel shader
        //**********************************************************************
        virtual void SetShader(IShader *pShader) = 0;

        //**********************************************************************
        // Method: GetShader
        // Returns the shader currently assigned to the node
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() = 0;

        //**********************************************************************
        // Method: GetInput
        // Returns the node that attached to the specified node input parameter
        // (i.e. this is the prior node in the pipeline that provides this node
        // with the input specified by pName)
        //
        // Parameters:
        // pName - name of input parameter
        //**********************************************************************
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t *pName) = 0;

        //**********************************************************************
        // Method: SetInput
        // Links a node from earlier in the pipeline to the specified parameter.
        //
        // Parameters:
        // pName - name of input parameter
        // pSamplerName - name of sampler
        // pNode - previous node in pipeline that provides its output as input
        // for parameter pName.
        //**********************************************************************
        virtual void SetInput(const wchar_t* pName, const wchar_t* pSamplerName, IGPUPipelineNode*pNode) = 0;

        //**********************************************************************
        // Method: SetOutputSize
        // Sets the size of the output image from this node
        //
        // Parameters:
        // width - width of this image in pixels
        // height - height of this image in pixels
        //**********************************************************************
        virtual void SetOutputSize(uint32 width, uint32 height) = 0;

        //**********************************************************************
        // Method: GetOutputWidth
        // Returns the width of the output image in pixels
        //**********************************************************************
        virtual uint32 GetOutputWidth() = 0;

        //**********************************************************************
        // Method: GetOutputHeight
        // Returns the height of the output image in pixels
        //**********************************************************************
        virtual uint32 GetOutputHeight() = 0;

        //**********************************************************************
        // Method: GetOutputTexture
        // Returns the texture generated as output by this node
        //**********************************************************************
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline *pPipeline) = 0;

        //**********************************************************************
        // Method: Process
        // Runs this node
        //
        // Parameters:
        // pPipeline - pipeline to run this node on
        // pRenderer - current renderer
        // pRenderCtx - current render context
        //**********************************************************************
        virtual void Process(IGPUPipeline *pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx) = 0;
    };

    //**********************************************************************
    // Interface: IGPUPipelineSourceNode
    // Defines the a source node. This node is at the start of a pipeline.
    //**********************************************************************
    struct IGPUPipelineSourceNode : public IGPUPipelineNode
    {
        virtual void SetSource(IGPUPipeline *pPipeline, IImage *pSource) = 0;
    };

    //**********************************************************************
    // Interface: IGPUPipelineSinkNode
    // Defines the a sink node. This node is at the end of a pipeline.
    //**********************************************************************
    struct IGPUPipelineSinkNode : public IGPUPipelineNode
    {
        //**********************************************************************
        // Method: GetOutput
        // Returns the image that resulted from the pipeline processing
        //**********************************************************************
        virtual CRefObj<IImage> GetOutput(IGPUPipeline *pPipeline) = 0;
    };

    // Define list of predefined filter names
    static const wchar_t *g_GaussianVerticalFilter = L"GaussianVertical"; // Name of GPU node that performs a gaussian blur in the vertical direction
    static const wchar_t *g_GaussianHorizontalFilter = L"GaussianHorizontal"; // Name of GPU node that performs a gaussian blur in the horizontal direction
    static const wchar_t *g_RawCopyFilter = L"RawCopy"; // Name of GPU node that performs a raw copy of the data

    //**********************************************************************
    // Interface: IGPUPipeline
    // The IGPUPipeline object is used to create a graph of GPU shaders that will process
    // images.
    // A pipeline is built out of a series of <IGPUPipelineNode>'s. Each node is conceptually
    // just a pixel shader to be run on its input image data.
    // The general structure of a GPU graph looks something like:
    //
    //   IGPUPipelineSourceNode         IGPUPipelineNode              IGPUPipelineSinkNode
    //  +------------+              +--------------+           +------------------+
    //  |            |----------->  |              |---------> |                  |
    //  +------------+              +--------------+           +------------------+
    //
    // There can be multiple IGPUPipelineSourceNodes and IGPUPipelineNodes. IGPUPipelineNodes can
    // have multiple input sources but always generate a single output.
    // The graph uses a pull model. Each node will call GetOutputTexture() on its source
    // nodes, which in turn will run Process on that node.
    //**********************************************************************
    struct IGPUPipeline : public IRefCount
    {
        //**********************************************************************
        // Method: GetRenderer
        // Returns the Renderer associated with this pipeline
        //**********************************************************************
        virtual CRefObj<IRenderer> GetRenderer() = 0;

        //**********************************************************************
        // Method: RenderQuad
        // Draws the shader using a fullscreen quad
        //**********************************************************************
        virtual void RenderQuad(IShader* pShader) = 0;

        //**********************************************************************
        // Method: Process
        // Executes the pipeline
        //
        // Parameters:
        // pRenderer - renderer being used
        // pRenderCtx - current render context
        //**********************************************************************
        virtual void Process(IRenderer *pRenderer, IRenderCtx *pRenderCtx) = 0;

        //**********************************************************************
        // Method: CreateSourceNode
        // Returns a source node. The source node will feed the pipeline
        // with the specified image.
        //
        // Parameters:
        // pName - name for this node (used for debugging)
        // pImage - image to feed through pipeline
        // format - format of output texture
        //**********************************************************************
        virtual CRefObj<IGPUPipelineSourceNode> CreateSourceNode(const wchar_t *pName, IImage *pImage, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) = 0;

        //**********************************************************************
        // Method: CreateSinkNode
        // Returns a sink node.
        //
        // Parameters:
        // pName - name for this node (used for debugging)
        // pShader - shader to run on inputs
        // format - format of output texture
        //**********************************************************************
        virtual CRefObj<IGPUPipelineSinkNode> CreateSinkNode(const wchar_t* pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) = 0;

        //**********************************************************************
        // Method: CreateNode
        // Returns a GPU node that makes up part of the pipeline.
        //
        // Parameters:
        // pName - name for this node (used for debugging)
        // pShader - shader to run on inputs
        // format - format of output texture
        //**********************************************************************
        virtual CRefObj<IGPUPipelineNode> CreateNode(const wchar_t* pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) = 0;

        //**********************************************************************
        // Method: CreatePredefinedNode
        // Returns a GPU node that makes up part of the pipeline. This method is
        // for specific nodes that have a custom Process() step beyond just running
        // some shader.
        //
        // Parameters:
        // pName - name of prefined node
        // ... - list of parameters specific to the requested node
        //**********************************************************************
        virtual CRefObj<IGPUPipelineNode> CreatePredefinedNode(const wchar_t* pName, ...) = 0;

        //**********************************************************************
        // Method: IncrementCurrentEpoch
        // Increments the current Epoch. The current epoch is a value indicating
        // how many times the processing pipeline has been run. This is useful for
        // determining whether a node in the pipeline has been run during the current
        // frame.
        //**********************************************************************
        virtual void IncrementCurrentEpoch() = 0;

        //**********************************************************************
        // Method: GetCurrentEpoch
        // Returns the current Epoch
        //**********************************************************************
        virtual uint32 GetCurrentEpoch() = 0;
    };

    //**********************************************************************
    // Constant: c_CustomNode_DepthMeshNode
    // Defines name for a GPU node that creates a mesh from a depth map and
    // renders that depth data into a render target. This is used primarily
    // to map the depth to its corresponding color camera.
    //
    // Arguments:
    // <uint32> depthWidth - width in pixels of depth map
    // <uint32> depthHeight - height in pixels of depth map
    // <uint32> colorWidth - width in pixels of color map
    // <uint32> colorHeight - height in pixels of color map
    // <DXGI_FORMAT> colorFormat - pixel format for color image
    //**********************************************************************
    static const wchar_t* c_CustomNode_DepthMeshNode = L"DepthMeshNode";

    //**********************************************************************
    // Constant: c_CustomNode_Color2Depth
    // Defines name for a GPU node that creates a mesh from a depth map and
    // renders that the texture coordinates into a map. The map then contains
    // texture coordinates for mapping color texture coordinates to depth
    // texture coordinates.
    //
    // Arguments:
    // <uint32> depthWidth - width in pixels of depth map
    // <uint32> depthHeight - height in pixels of depth map
    // <uint32> colorWidth - width in pixels of color map
    // <uint32> colorHeight - height in pixels of color map
    //**********************************************************************
    static const wchar_t* c_CustomNode_Color2Depth = L"Color2DepthNode";

    CRefObj<IGPUPipeline> CreateGPUPipeline(IRenderer *pRenderer);
}
