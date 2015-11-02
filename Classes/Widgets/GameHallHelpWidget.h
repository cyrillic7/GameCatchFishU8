/**
 * @file GameHallHelpWidget
 * @brief  游戏大厅帮助界面
 * @author tjl
 * @date 15-5-31
 */

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameHallHelpWidget :public TNWidget
{
public:
	GameHallHelpWidget();
	~GameHallHelpWidget();
	virtual bool init();
	CREATE_FUNC(GameHallHelpWidget);
	virtual std::string getWidgetJsonName();

	void onEnter();
	void onExit();

	virtual void loadUI();
protected:
	
	void onClickLastPage(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickNextPage(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
private:
	ImageView*  mHelpImage;
	int		  mPageIndex;//当前页面索引
};
