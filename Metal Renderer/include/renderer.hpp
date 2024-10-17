//
//  renderer.hpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include <simd/simd.h>

class Renderer
{
public:
    Renderer(MTL::Device *device);
    ~Renderer();
    
    void Draw(MTK::View *view);

private:
    void buildShaders();
    void buildBuffers();
    
    MTL::Device *m_Device;
    MTL::CommandQueue *m_CommandQueue;
    MTL::RenderPipelineState *m_RenderPipelineState;
    MTL::Library *m_ShaderLibrary;
    MTL::Buffer *m_ArgumentBuffer;
    MTL::Buffer *m_VertexPositionsBuffer;
    MTL::Buffer *m_VertexColorsBuffer;
};
