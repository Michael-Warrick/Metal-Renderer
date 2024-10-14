//
//  application_delegate.cpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//

#include "application_delegate.hpp"

ApplicationDelegate::~ApplicationDelegate()
{
    m_MTKView->release();
    m_Window->release();
    m_Device->release();
    
    delete m_ViewDelegate;
}

NS::Menu *ApplicationDelegate::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;
    
    NS::Menu *mainMenu = NS::Menu::alloc()->init();
    NS::Menu *applicationMenu = NS::Menu::alloc()->init(NS::String::string("Appname", UTF8StringEncoding));
    NS::MenuItem *applicationMenuItem = NS::MenuItem::alloc()->init();
    NS::String *applicationName = NS::RunningApplication::currentApplication()->localizedName();
    
    // Quit
    NS::String *quitItemName = NS::String::string("Quit ", UTF8StringEncoding)->stringByAppendingString(applicationName);
    
    SEL quitCallback = NS::MenuItem::registerActionCallback("appQuit", [](void *, SEL, const NS::Object *pSender){
        auto *application = NS::Application::sharedApplication();
        application->terminate(pSender);
    });
    
    NS::MenuItem *applicationQuitItem = applicationMenu->addItem(quitItemName, quitCallback, NS::String::string("q", UTF8StringEncoding));
    applicationQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    applicationMenuItem->setSubmenu(applicationMenu);
    
    // Close Window
    NS::Menu *windowMenu = NS::Menu::alloc()->init(NS::String::string("Window", UTF8StringEncoding));
    NS::MenuItem *windowMenuItem = NS::MenuItem::alloc()->init();
    
    SEL closeWindowCallback = NS::MenuItem::registerActionCallback("windowClose", [](void *, SEL, const NS::Object *){
        auto application = NS::Application::sharedApplication();
        application->windows()->object<NS::Window>(0)->close();
    });
    
    NS::MenuItem *closeWindowItem = windowMenu->addItem(NS::String::string("Close Window", UTF8StringEncoding), closeWindowCallback, NS::String::string("w", UTF8StringEncoding));
    closeWindowItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    windowMenuItem->setSubmenu(windowMenu);
    
    mainMenu->addItem(applicationMenuItem);
    mainMenu->addItem(windowMenuItem);
    
    applicationMenuItem->release();
    windowMenuItem->release();
    applicationMenu->release();
    windowMenu->release();
    
    return mainMenu->autorelease();
}

void ApplicationDelegate::applicationWillFinishLaunching(NS::Notification *notification)
{
    NS::Menu *menu = createMenuBar();
    
    NS::Application *application = reinterpret_cast<NS::Application *>(notification->object());
    application->setMainMenu(menu);
    application->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void ApplicationDelegate::applicationDidFinishLaunching(NS::Notification *notification)
{
    CGRect frame = (CGRect){{100.0, 100.0}, {640.0, 480.0}};
    
    m_Window = NS::Window::alloc()->init(frame, NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled, NS::BackingStoreBuffered, false);
    m_Device = MTL::CreateSystemDefaultDevice();
    
    m_MTKView = MTK::View::alloc()->init(frame, m_Device);
    m_MTKView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    m_MTKView->setClearColor(MTL::ClearColor::Make(0.02, 0.02, 0.02, 1.0));
    
    m_ViewDelegate = new MTKViewDelegate(m_Device);
    m_MTKView->setDelegate(m_ViewDelegate);
    
    m_Window->setContentView(m_MTKView);
    m_Window->setTitle(NS::String::string("Metal Renderer", NS::StringEncoding::UTF8StringEncoding));
    m_Window->makeKeyAndOrderFront(nullptr);
    
    NS::Application *application = reinterpret_cast<NS::Application *>(notification->object());
    application->activateIgnoringOtherApps(true);
}

bool ApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application *sender)
{
    return true;
}
