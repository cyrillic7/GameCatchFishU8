#ifndef __casino__SmallLoadingProgressWidget__
#define __casino__SmallLoadingProgressWidget__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;

class SmallLoadingProgressWidget : public CCNode {
public:
    static SmallLoadingProgressWidget* create(bool thinMode = false, const char* backgroundImage = NULL, const char* frontImage = NULL);
    bool init(bool thinMode, const char* backgroundImage = NULL, const char* frontImage = NULL);

    // 百分比,0-100
    CC_SYNTHESIZE_READONLY(float, m_progress, Progress);
    void setProgress(float progress);

    // start 之前会隐藏的，start 才显示
    void startLoading();
    
    // 会隐藏，progress到了100也会
    void stopLoading();
    
private:
    // 背景图
    Sprite* m_backgroundSprite;
    // 前面那个圈
    Sprite* m_forgroundSprite;
    CCProgressTimer* m_progressTimer;

	ui::Text*    m_progressText;
	LabelAtlas*  m_progressLabel;
};

#endif /* defined(__casino__SmallLoadingProgressWidget__) */