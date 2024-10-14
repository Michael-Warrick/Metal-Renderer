//
//  mtk_view_delegate.hpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "renderer.hpp"

class MTKViewDelegate : public MTK::ViewDelegate
{
public:
    MTKViewDelegate(MTL::Device *device);
    virtual ~MTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View *view) override;
    
private:
    Renderer *m_Renderer;
};
