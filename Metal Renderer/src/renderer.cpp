//
//  renderer.cpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#include "renderer.hpp"

Renderer::Renderer(MTL::Device *device)
{
    m_Device = device->retain();
    m_CommandQueue = device->newCommandQueue();
    
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    m_ShaderLibrary->release();
    m_ArgumentBuffer->release();
    m_VertexPositionsBuffer->release();
    m_VertexColorsBuffer->release();
    
    m_RenderPipelineState->release();
    
    m_CommandQueue->release();
    m_Device->release();
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;
    
    const char* shaderSourceCode = R"(
        #include <metal_stdlib>
        using namespace metal;
    
        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };
    
        struct VertexData
        {
            device float3* positions [[id(0)]];
            device float3* colors [[id(1)]];
        };
    
        v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]], uint vertexId [[vertex_id]] )
        {
            v2f o;
            o.position = float4( vertexData->positions[ vertexId ], 1.0 );
            o.color = half3(vertexData->colors[ vertexId ]);
            return o;
        }
    
        half4 fragment fragmentMain( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";
    
    NS::Error *error = nullptr;
    MTL::Library *library = m_Device->newLibrary(NS::String::string(shaderSourceCode, UTF8StringEncoding), nullptr, &error);
    if (!library) {
        throw std::runtime_error(error->localizedDescription()->utf8String());
    }
    
    MTL::Function *vertexFunction = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function *fragmentFunction = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));
    
    MTL::RenderPipelineDescriptor *renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDescriptor->setVertexFunction(vertexFunction);
    renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    
    m_RenderPipelineState = m_Device->newRenderPipelineState(renderPipelineDescriptor, &error);
    if (!m_RenderPipelineState) {
        throw std::runtime_error(error->localizedDescription()->utf8String());
    }
    
    vertexFunction->release();
    fragmentFunction->release();
    renderPipelineDescriptor->release();
    
    m_ShaderLibrary = library;
}

void Renderer::buildBuffers()
{
    using NS::StringEncoding::UTF8StringEncoding;
    
    const size_t numVertices = 3;
    
    simd::float3 positions[numVertices]
    {
        {-0.8f, -0.8f, 0.0f},
        {0.8f, -0.8f, 0.0f},
        {0.0f, 0.8f, 0.0f}
    };
    
    simd::float3 colors[numVertices] =
    {
        {1.0f, 0.3f, 0.2f},
        {0.8f, 1.0, 0.0f},
        {0.8f, 0.0f, 1.0f}
    };
    
    const size_t positionsDataSize = numVertices * sizeof(simd::float3);
    const size_t colorsDataSize = numVertices * sizeof(simd::float3);
    
    MTL::Buffer *vertexPositionsBuffer = m_Device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer *vertexColorsBuffer = m_Device->newBuffer(colorsDataSize, MTL::ResourceStorageModeManaged);
    
    m_VertexPositionsBuffer =  vertexPositionsBuffer;
    m_VertexColorsBuffer =  vertexColorsBuffer;
    
    memcpy(m_VertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(m_VertexColorsBuffer->contents(), colors, colorsDataSize);
    
    m_VertexPositionsBuffer->didModifyRange(NS::Range::Make(0, m_VertexPositionsBuffer->length()));
    m_VertexColorsBuffer->didModifyRange(NS::Range::Make(0, m_VertexColorsBuffer->length()));
    
    if (!m_ShaderLibrary) {
        throw std::runtime_error("Error: ShaderLibrary either incomplete or corrupted!");
    }
    
    MTL::Function *vertexFunction = m_ShaderLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::ArgumentEncoder *argumentEncoder = vertexFunction->newArgumentEncoder(0);
    
    MTL::Buffer *argumentBuffer = m_Device->newBuffer(argumentEncoder->encodedLength(), MTL::ResourceStorageModeManaged);
    m_ArgumentBuffer = argumentBuffer;
    
    argumentEncoder->setArgumentBuffer(m_ArgumentBuffer, 0);
    
    argumentEncoder->setBuffer(m_VertexPositionsBuffer, 0, 0);
    argumentEncoder->setBuffer(m_VertexColorsBuffer, 0, 1);
    
    m_ArgumentBuffer->didModifyRange(NS::Range::Make(0, m_ArgumentBuffer->length()));
    
    vertexFunction->release();
    argumentEncoder->release();
}

void Renderer::Draw(MTK::View *view)
{
    NS::AutoreleasePool *autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    MTL::CommandBuffer *commandBuffer = m_CommandQueue->commandBuffer();
    MTL::RenderPassDescriptor *renderPassDescriptor = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder *renderCommandEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    renderCommandEncoder->setRenderPipelineState(m_RenderPipelineState);
    renderCommandEncoder->setVertexBuffer(m_ArgumentBuffer, 0, 0);
    renderCommandEncoder->useResource(m_VertexPositionsBuffer, MTL::ResourceUsageRead);
    renderCommandEncoder->useResource(m_VertexColorsBuffer, MTL::ResourceUsageRead);
    
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    
    renderCommandEncoder->endEncoding();
    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();
    
    autoreleasePool->release();
}
