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
}

Renderer::~Renderer()
{
    m_CommandQueue->release();
    m_Device->release();
}

void Renderer::Draw(MTK::View *view)
{
    NS::AutoreleasePool *autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    MTL::CommandBuffer *commandBuffer = m_CommandQueue->commandBuffer();
    MTL::RenderPassDescriptor *renderPassDescriptor = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder *renderCommandEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    renderCommandEncoder->endEncoding();
    
    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();
    
    autoreleasePool->release();
}
