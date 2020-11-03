//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Imaging\Image\Image.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"

namespace Caustic
{
    struct IGPUPipeline;

    //**********************************************************************
    // Interface: IGPUPipelineNode
    // Defines the base class for each node in a GPU pipeline
    //**********************************************************************
    struct IGPUPipelineNode : public IRefCount
    {
        virtual uint32 GetNumberInputs() = 0;
        virtual void SetShader(IShader *pShader) = 0;
        virtual CRefObj<IShader> GetShader() = 0;
        virtual CRefObj<IGPUPipelineNode> GetInput(uint32 index) = 0;
        virtual void SetInput(uint32 index, IGPUPipelineNode*pNode) = 0;
        virtual void SetOutputSize(uint32 width, uint32 height) = 0;
        virtual uint32 GetOutputWidth() = 0;
        virtual uint32 GetOutputHeight() = 0;
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline *pPipeline) = 0;
        virtual void Process(IGPUPipeline *pPipeline) = 0;
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
        //**********************************************************************
        virtual void Process() = 0;

        //**********************************************************************
        // Method: CreateSourceNode
        // Returns a source node. The source node will feed the pipeline
        // with the specified image.
        //
        // Parameters:
        // pImage - image to feed through pipeline
        //**********************************************************************
        virtual CRefObj<IGPUPipelineSourceNode> CreateSourceNode(IImage *pImage) = 0;

        //**********************************************************************
        // Method: CreateSinkNode
        // Returns a sink node.
        //
        // Parameters:
        // numInputs - number of inputs into the sink node
        // pShader - shader to run on inputs
        //**********************************************************************
        virtual CRefObj<IGPUPipelineSinkNode> CreateSinkNode(uint32 numInputs, IShader *pShader) = 0;

        //**********************************************************************
        // Method: CreateNode
        // Returns a GPU node that makes up part of the pipeline.
        //
        // Parameters:
        // numInputs - number of inputs into the node
        // pShader - shader to run on inputs
        //**********************************************************************
        virtual CRefObj<IGPUPipelineNode> CreateNode(uint32 numInputs, IShader *pShader) = 0;

        //**********************************************************************
        // Method: CreatePredefinedNode
        // Returns a GPU node that has a predefined name (e.g. "GaussianBlur")
        //
        // Parameters:
        // pNodeName - name of the node
        //**********************************************************************
        virtual CRefObj<IGPUPipelineNode> CreatePredefinedNode(wchar_t *pNodeName) = 0;
    };
    CAUSTICAPI CRefObj<IGPUPipeline> CreateGPUPipeline(IRenderer *pRenderer);
}
