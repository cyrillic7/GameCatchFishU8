/**
 * @file   ModalViewManager
 * @brief  模态层管理类
 * @author tjl
 * @date $date$
 */

#ifndef __ModalViewManager_H_
#define __ModalViewManager_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

//不可以被移除的标志
#define WIDGET_CANNOT_REMVOVE_FLAG     "cannot_remove_true"

class ModalViewManager : public Layout
{
	ModalViewManager();
	~ModalViewManager();

	bool init();

public:
	static ModalViewManager* sharedInstance();
	static void purgeInstance();
    void onExit();

	//显示
	//返回widget对应的tag
    // canBeRemove 是不是可以在removeAll的时候被移除
	int showWidget(Widget *w, bool canBeRemove = true);

	//获取指定tag的widget
	Widget* getWidgetWithTag(int tag);

	//移除指定tag的widget
	void removeWidgetWithTag(int tag);

	//移除退出游戏的Widget
	void removeExitWidget();
	//移除指定的widget
	void removeWidget(Widget *w);

	//移除所有的widget
    //forceRemove 是不是强制移除所有的
	void removeAllWidgets(bool forceRemove = false);

    //移除除了最后一个的其他所有widget
    void removeWidgetsToTop();

private:
	void update(float dt);

	Layout		*m_touchGroup;

	int		m_widgetTag;
};

#endif	//__ModalViewManager_H_