#ifndef __GameGuideWidget_H_H_
#define __GameGuideWidget_H_H_

/**
 * @file GameGuideWidget
 * @brief  游戏向导界面
 * @author tjl
 * @date 15-9-17
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Model/GuideItemModel.h"
using namespace cocos2d::ui;
USING_NS_CC;

class GameGuideItem : public Layout
{
public:
	GameGuideItem();
	~GameGuideItem();
	static GameGuideItem* create(GuideItemModel* info);
	virtual bool init(GuideItemModel* info);
};

class GameGuideWidget : public TNWidget
{
public:
	GameGuideWidget();
	~GameGuideWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameGuideWidget);
protected:
};


#endif//__GameGuideWidget_H_H_