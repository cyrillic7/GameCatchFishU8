/**
 * @file CommonLoadingWidget
 * @brief wait loading页面
 * @author tjl
 * @date
 */

#ifndef __CommonLoadingWidget_H_
#define __CommonLoadingWidget_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

#define COMMON_LOADING_TAG		12345

class CommonLoadingWidget : public Layout {
public:
    CommonLoadingWidget();
    virtual ~CommonLoadingWidget();
	static CommonLoadingWidget* create(Size size);
	bool init(Size size);

	void setTips(std::string tips);
	void netWorkIsValid(EventCustom* evt);
protected:
	void TimeOut( Node* node );
private:
    ImageView* m_icon;
	Text*		m_tipsLable;
};

#endif //__CommonLoadingWidget_H_
