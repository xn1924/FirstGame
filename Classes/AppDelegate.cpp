#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GraphicsScene.h"
#include "TouchScene.h"
#include "MultiTouch.h"
#include "MouseScene.h"
#include "KeyboardScene.h"
#include "AnimateScene.hpp"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{

}
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Hello World");
        //glview->setFrameSize(640, 480);
        director->setOpenGLView(glview);
    }

    // create a scene. it's an autorelease object
    auto scene = AnimateScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {

}

