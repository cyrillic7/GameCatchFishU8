#include "TNVisibleRect.h"

cocos2d::Rect TNVisibleRect::s_visibleRect;

void TNVisibleRect::lazyInit()
{
    //if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
		
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	CCDirector *director = Director::getInstance();
//	s_visibleRect.origin = director->getVisibleOrigin();
//	s_visibleRect.size.width = director->getVisibleSize().height;
//	s_visibleRect.size.height = director->getVisibleSize().width;
//#else
	CCEGLView* pEGLView = Director::getInstance()->getOpenGLView();
	s_visibleRect.origin = pEGLView->getVisibleOrigin();
    s_visibleRect.size = pEGLView->getVisibleSize();
//#endif
        
    }
}

cocos2d::Rect TNVisibleRect::getVisibleRect()
{
    lazyInit();
    return CCRectMake(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

Vec2 TNVisibleRect::left()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 TNVisibleRect::right()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 TNVisibleRect::top()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 TNVisibleRect::bottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Vec2 TNVisibleRect::center()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 TNVisibleRect::leftTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 TNVisibleRect::rightTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 TNVisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

Vec2 TNVisibleRect::rightBottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}
