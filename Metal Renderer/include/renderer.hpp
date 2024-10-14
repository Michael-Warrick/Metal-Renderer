//
//  renderer.hpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class Renderer
{
public:
    Renderer(MTL::Device *device);
    ~Renderer();
    
    void Draw(MTK::View *view);

private:
    MTL::Device *m_Device;
    MTL::CommandQueue *m_CommandQueue;
};
