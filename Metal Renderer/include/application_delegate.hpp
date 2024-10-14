//
//  application_delegate.hpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#pragma once
#include <iostream>

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "mtk_view_delegate.hpp"

class ApplicationDelegate : public NS::ApplicationDelegate
{
public:
    ~ApplicationDelegate();
    
    virtual void applicationWillFinishLaunching(NS::Notification *notification) override;
    virtual void applicationDidFinishLaunching(NS::Notification *notification) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application *sender) override;
    
private:
    NS::Menu *createMenuBar();
    
    NS::Window *m_Window;
    MTK::View *m_MTKView;
    MTL::Device *m_Device;
    MTKViewDelegate *m_ViewDelegate = nullptr;
};
