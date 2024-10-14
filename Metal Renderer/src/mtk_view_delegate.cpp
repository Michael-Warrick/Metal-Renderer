//
//  mtk_view_delegate.cpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#include "mtk_view_delegate.hpp"

MTKViewDelegate::MTKViewDelegate(MTL::Device *device) : MTK::ViewDelegate()
{
    m_Renderer = new Renderer(device);
}

MTKViewDelegate::~MTKViewDelegate()
{
    delete m_Renderer;
}

void MTKViewDelegate::drawInMTKView(MTK::View *view)
{
    m_Renderer->Draw(view);
}
