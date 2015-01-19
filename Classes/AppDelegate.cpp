#include "AppDelegate.h"
#include "MainScence\GameWorld.h"
#include "MainScence\LoginScene.h"
#include "MainScence\SongSelectionScene.h"
#include "consts\ResolutionConst.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
		glview->setFrameSize(480, 800);
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_WIDTH);

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("fonts/hanzi.plist");


	cache->addSpriteFramesWithFile("mainscence/count.plist");
	cache->addSpriteFramesWithFile("mainscence/explode.plist");
	cache->addSpriteFramesWithFile("rolemove/rolemove.plist");
	cache->addSpriteFramesWithFile("mainscence/energy.plist");
//    auto scene = GameWorld::create();
    auto scene = LoginScene::create();
//    auto scene = SongSelectionScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
