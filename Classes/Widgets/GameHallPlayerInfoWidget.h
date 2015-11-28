/**
 * @file GameHallPlayerInfoWidget
 * @brief  游戏大厅个人消息界面
 * @author tjl
 * @date 15-5-31
 */

#include "cocos2d.h"
#include "Common/TNWidget.h"

USING_NS_CC;

class GameHallPlayerInfoWidget :public TNWidget
{
public:
	GameHallPlayerInfoWidget();
	~GameHallPlayerInfoWidget();
	virtual bool init();
	static GameHallPlayerInfoWidget* create();
	virtual std::string getWidgetJsonName();
	void onEnter();
	void onExit();
	virtual void loadUI();
protected:
	void onClickBindPhone(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickConvert(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onSelectMan(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onSelectWoman(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void playScaleAnimation(bool less, Ref* pSender);
	void gotoCharge(Ref* pSender,ui::Widget::TouchEventType type);
	void receiveSuccessResult(EventCustom* evt);
	void receiveFailureResult(EventCustom* evt);
	void refreshTreasureData(EventCustom* evt);
private:
	Text* mTextFieldNickName;//昵称
	ImageView* mCheckBoxMan;
	ImageView* mCheckBoxWoman;
	bool	   mIsWoman;
	Text*      mUserIdText;
	Text*      mScoreText;
	Text*      mIngotText;
	Text*      mJiangJuanText;
	ImageView* mHeadIcon;
};