//
//  main.cpp
//  Metal-Renderer
//
//  Created by Michael Warrick on 12/10/2024.
//
#include <AppKit/AppKit.hpp>

#include "application_delegate.hpp"

int main()
{
    NS::AutoreleasePool *autoreleasePool = NS::AutoreleasePool::alloc()->init();
    ApplicationDelegate applicationDelegate;
        
    NS::Application *sharedApplication = NS::Application::sharedApplication();
    sharedApplication->setDelegate(&applicationDelegate);
    sharedApplication->run();
    
    autoreleasePool->release();
    
    return 0;
}
