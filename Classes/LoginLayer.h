
#ifndef __LOGIN_LAYER_H__
#define __LOGIN_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class LoginLayer : public Layer {
public:
	typedef enum {
		FISH_TYPE_1 = 1,
		FISH_TYPE_2,
		FISH_TYPE_3,
		FISH_TYPE_4
	} FishType;

public:
	CREATE_FUNC(LoginLayer);

private:
	virtual bool init(void);

	void runSpriteFrameAnimation(Sprite* sprite, FishType type);

	void onClickAccountLogin(Ref* pRef,ui::Widget::TouchEventType type);
	void onClickFastLogin(Ref* pRef,ui::Widget::TouchEventType type);
	void onClickQQLogin(Ref* pRef,ui::Widget::TouchEventType type);
	void onClickWXLogin(Ref*pRef, ui::Widget::TouchEventType type);
	void playScaleAnimation(bool less, Ref* pSender);
};


#endif