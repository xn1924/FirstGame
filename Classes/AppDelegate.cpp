#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GraphicsScene.h"
#include "TouchScene.h"
#include "MultiTouch.h"
#include "MouseScene.h"
#include "KeyboardScene.h"
#include "CombatScene.h"
#include "MapScene.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

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
        glview = GLViewImpl::create("梦幻西游");

		//屏幕和设计分辨率不匹配，会导致UI交互事件Touch坐标异常
        glview->setFrameSize(800, 600);//屏幕分辨率
		glview->setDesignResolutionSize(800, 600, ResolutionPolicy::EXACT_FIT);//设计分辨率，缺省为960*640

        director->setOpenGLView(glview);
	}

    // create a scene. it's an autorelease object
    auto scene = MapScene::createScene();
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

