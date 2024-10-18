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
    struct FrameData {
        float angle;
    };
    
    Renderer(MTL::Device *device);
    ~Renderer();
    
    void Draw(MTK::View *view);

private:
    std::string loadShaderFile(const std::string& filePath);
    void buildShaders();
    void buildBuffers();
    void buildFrameData();
    
    MTL::Device *m_Device;
    MTL::CommandQueue *m_CommandQueue;
    MTL::RenderPipelineState *m_RenderPipelineState;
    MTL::Library *m_ShaderLibrary;
    MTL::Buffer *m_ArgumentBuffer;
    MTL::Buffer *m_VertexPositionsBuffer;
    MTL::Buffer *m_VertexColorsBuffer;
    MTL::Buffer *m_FrameData[3];
    
    float m_Angle;
    int m_Frame;
    dispatch_semaphore_t m_Semaphore;
    static const int MAX_FRAMES_IN_FLIGHT = 3;
};
