#ifndef _CCMAIN_LAYER_H_H_
#define _CCMAIN_LAYER_H_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"



USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;



class CCMenuLayer : public cocos2d::Layer
{
public:
	enum
	{
		hortype = 0,
		vertype,
	};
	enum
	{
        initOpen=0,
	    initClose,
	};
	virtual ~CCMenuLayer();
	CREATE_FUNC(CCMenuLayer);
	virtual bool init();
	void onEnter();
	void onExit();
	void MenuHide( Node* node );
	void MenuShow( Node* node );
	void AppendItem(const char* normal,const char* select,int type,int index);
	void AddToggleItem(const char* img,const char* img2,int initType = 0);
	void SetTogPosition(Vec2 pt);
	void RegisterMenuHandler(Object* rec,ui::Widget::ccWidgetTouchCallback handler){
	    mRec = rec;
        mSelector = handler;
    }
	void setMenuHide(bool hide, float dur = 0.05f);

	void onClickToggle(Ref* pSender,ui::Widget::TouchEventType type);
	void playScaleAnimation(bool less, Ref* pSender);
	void showRedPoint(bool value,bool isTask);
private:
	//Menu*   menu; 
	int     mHorCount;
	int		mVecCount;
	ImageView* mToggle;
	int		mArriveCnt;
	bool	mCanClick;
	ui::Widget::ccWidgetTouchCallback  mSelector;
	Object* mRec;
	bool    mIsExpand ;
	Armature* mTaskRedPoint;
	Armature* mAuctionRedPoint;
};
#endif
