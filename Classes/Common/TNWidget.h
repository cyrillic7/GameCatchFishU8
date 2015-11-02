/**
 * @file TNWidget
 * @brief  widget基础类
 * @author tjl
 * @date 15-5-25
 */
#ifndef __TNWidget_H_
#define __TNWidget_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class TNWidget : public Widget {
public:
    TNWidget();
    virtual ~TNWidget();

    virtual void onEnter();
    virtual void onExit();

    //获取界面对应的json文件
    virtual std::string getWidgetJsonName();

    //加载cocostudio界面
    virtual void loadUI();

    //键盘输入场景移动动画, 默认为0, 即默认移动屏幕中心
    void runKeyboardAction(TextFiledEventType type, float offsetY = 0);

    //加载视图获得widget
    Widget* getMainWidget();

    //界面传参, 请在onEnter里面调用
    void setInputParam(Ref *inputParam);
    Ref *getInputParam();
    
    //widget布局使用, fix某一边时, 这一边的visiblePanel尺寸不会随分辨率更改
    //固定高
    virtual bool fixHeight();
    //固定宽
    virtual bool fixWidth();

    //加载指示
    void showLoading(bool canTouch = false);
    void hideLoading();
    ImageView* m_loadingWidget;
    Layout* m_loadingLayout;
protected:
    //是不是初始化
    bool        m_isInit;

    Widget      *m_mainWidget;

    Ref    *m_inputParam;
};


#endif //__TNWidget_H_
