#include "AppDelegate.h"
#include "GameMainScene.h"
#include "GameGlobalSetting.h"
#include "SimpleAudioEngine.h"
#include "SessionManager.h"
#include "LoginScene.h"
#include "SplashScene.h"
#include "audio/include/AudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::experimental;
USING_NS_CC;



AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{ 
	SimpleAudioEngine::end();
	AudioEngine::end();

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {

	// 设置资源搜索目录
	setSearchPath();

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
		//glview->setFrameSize(kScreenWidth,640); 
		glview->setFrameSize(kScreenWidth,kScreenHeight); 
        director->setOpenGLView(glview);
    }
	FileUtils::sharedFileUtils()->setPopupNotify(false);
	setDesignResolutionPolicy();

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	////加载本地资源
	//ResourcesManager::getInstance()->loadResourceFile("animation.xml");

    // create a scene. it's an autorelease object
    auto scene = SplashScene::create();
	//auto scene = MainScene::create();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
	 SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	 SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

	if (SessionManager::shareInstance()->getEffectOn()) {
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}

	if (SessionManager::shareInstance()->getMusicOn()) {
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void AppDelegate::setDesignResolutionPolicy() {
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();
	glView->setDesignResolutionSize(kScreenWidth, kScreenHeight, ResolutionPolicy::EXACT_FIT);

}


void AppDelegate::setSearchPath() 
{
	// 下载目录，这里的优先级别，需要比下面的高
	FileUtils::getInstance()->addSearchPath((FileUtils::getInstance()->getWritablePath()).c_str());
}

