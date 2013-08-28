//
//  AiirHockeyAppDelegate.cpp
//  AiirHockey
//
//  Created by 叶孙松 on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    
    CCEGLView* pEGLView=CCEGLView::sharedOpenGLView();    
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(pEGLView);

    //自适应
    CCSize screenSize=pEGLView->getFrameSize();
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(768, 1024, kResolutionExactFit);
    if (screenSize.width>768) {
//        CCFileUtils::sharedFileUtils()->setSearchPaths(<#const std::vector<std::string> &searchPaths#>);
        pDirector->setContentScaleFactor(2);
    }else{
//        ccfileutils::sharedFileUtils->setSearchPaths();
        pDirector->setContentScaleFactor(1);
    }
    
    
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
