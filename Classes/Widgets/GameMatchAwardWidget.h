#ifndef __GameMatchAwardWidget_H_H_
#define __GameMatchAwardWidget_H_H_

/**
 * @file GameMatchAwardWidget
 * @brief  游戏比赛结算界面
 * @author tjl
 * @date 15-10-24
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/AlertWidget.h"
#include "Model/MatchModel.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameMatchAwardWidget : public TNWidget
{
public:
	GameMatchAwardWidget();
	~GameMatchAwardWidget();
	virtual bool init(CMD_GR_MatchResult *pResult);
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	static GameMatchAwardWidget* create(CMD_GR_MatchResult *pResult);
private:
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);//返回
	void onContinue(Ref* pSender,ui::Widget::TouchEventType type);//继续比赛
	void ExitSelf(float dt);
protected:
	CMD_GR_MatchResult m_awardInfo;
};


class GameMatchNextTryWidget : public TNWidget
{
public:
	GameMatchNextTryWidget();
	~GameMatchNextTryWidget();
	virtual bool init(GameMatchModel *pModel);
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	static GameMatchNextTryWidget* create(GameMatchModel *pModel);
private:
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);//返回
	void onTryAgain(Ref* pSender,ui::Widget::TouchEventType type);//继续比赛
	void updateRankMsg(EventCustom* evt);
protected:
	GameMatchModel* mModeInfo;
	TextAtlas* mRank ;
	TextAtlas* mHighScore;
	TextAtlas* mCurScore;
};

#endif//__GameMatchAwardWidget_H_H_