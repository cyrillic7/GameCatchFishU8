/**
 * @file GameHallBankWidget 
 * @brief  主界面的银行弹出小菜单
 * @author tjl
 * @date 15-5-26
 */
#ifndef __GameHallBankWidget_H_
#define __GameHallBankWidget_H_

#include "Common/TNWidget.h"
#include "GameGlobalSetting.h"

class GameHallBankWidget:public TNWidget
{
public:
	GameHallBankWidget();
	virtual ~GameHallBankWidget();
	
	static GameHallBankWidget* create(bool isShowBackBtn = false);
	virtual bool init();

	void onEnter();
	void onExit();

	virtual void loadUI();

	virtual std::string getWidgetJsonName();

private:

	void onClose(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickTake(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickSave(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickCommon(Ref* pSender,ui::Widget::TouchEventType eventtype);

	void onBankGetOutOrInPro(Ref* pSender,ui::Widget::TouchEventType eventtype);//银行存入/取出操作处理
	void onInputGoldNumber(Ref *sender, TextFiledEventType type);

	void refreshInsureData(EventCustom* evt);
	void receiveInsureSuccessMsg(EventCustom* evt);
	void receiveInsureFailureMsg(EventCustom* evt);
	void showLoading();
	void removeLoading();
	void modifyScore(Ref* pSender,ui::Widget::TouchEventType type);
	void setSelect(int index,bool isShow,bool isTakeOut = true);
	std::string getBitValue(int index,bool isTakeOut);
	void switchToSavePanel(Node* node);
	void switchToTakePanel(Node* node);

	void checkCanClick();

private:
	Layout* mGetOutPanel;
	Layout* mPutInPanel;
	bool    m_isInGetOut;//默认在取出界面
	ImageView* popBg ;
	TextField* mTextFieldOut;
	TextField* mTextFieldIn;
	int m_editFlag;
	Text*  mTakeScoreText;
	Text*  mSaveScoreText;
	int    mTakeSelectIndex;
	int    mSaveSelectIndex;
	std::string  mUserInsure ;//用户银行存款
	std::string  mUserScore;//用户身上存款
	ImageView*   mTabTake;
	ImageView*   mTabSave;
	ImageView*   mMovItem;
	ui::EditBox* mOutEdit;
	ui::EditBox* mInEdit;
	Vec2   mStartPos;
	bool   mHasMov;
	int    mCanTakeIndex;//能够取出的最大索引
	int    mCanSaveIndex;//能够存入的最大索引
};

#endif