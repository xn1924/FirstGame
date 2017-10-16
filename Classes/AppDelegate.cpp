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

		//ÆÁÄ»ºÍÉè¼Æ·Ö±æÂÊ²»Æ¥Åä£¬»áµ¼ÖÂUI½»»¥ÊÂ¼þTouch×ø±êÒì³£
        glview->setFrameSize(800, 600);//ÆÁÄ»·Ö±æÂ
		glview->setDesignResolutionSize(800, 600, ResolutionPolicy::EXACT_FIT);//Éè¼Æ·Ö±æÂÊ£¬È±Ê¡Îª960*640
		glview->setCursorVisible(false);

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

