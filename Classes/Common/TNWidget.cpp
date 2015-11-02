#include "TNWidget.h"
#include "CommonFunction.h"
#include "TNVisibleRect.h"
#include "Message.h"
#include "Common/AlertWidget.h"
#include "ModalViewManager.h"
#include "Common/CommonLoadingWidget.h"

TNWidget::TNWidget()
    : m_inputParam(NULL)
    , m_mainWidget(NULL)
    , m_isInit(true)
{
    m_inputParam = NULL;
    m_loadingLayout = NULL;
    m_loadingWidget = NULL;
}

TNWidget::~TNWidget() {
    CC_SAFE_RELEASE_NULL(m_inputParam);
    CC_SAFE_RELEASE_NULL(m_loadingLayout);
}

void TNWidget::onEnter() {
    Widget::onEnter();

    if (m_isInit) {
        m_isInit = false;
        loadUI();
    }
}

void TNWidget::onExit() {
    Widget::onExit();
}

std::string TNWidget::getWidgetJsonName() {
    return "";
}

void TNWidget::loadUI() {
    m_mainWidget = CommonFunction::getWidgetFromCocostudioFile(getWidgetJsonName().c_str());
    if (NULL != m_mainWidget) {
        Layout *visiblePanel = dynamic_cast<Layout*>(m_mainWidget->getChildByName("VisiblePanel"));
        if (NULL != visiblePanel)
        {
            if (fixHeight()) {
                visiblePanel->setSize(CCSizeMake(TNVisibleRect::getVisibleRect().size.width, visiblePanel->getSize().height));
            } else if(fixWidth()) {
                visiblePanel->setSize(CCSizeMake(visiblePanel->getSize().width, TNVisibleRect::getVisibleRect().size.height));
            } else {
                visiblePanel->setSize(TNVisibleRect::getVisibleRect().size);
            }
            visiblePanel->setAnchorPoint(ccp(0.5, 0.5));
            visiblePanel->setPosition(TNVisibleRect::center());
        }
        this->addChild(m_mainWidget);
    }
}

void TNWidget::runKeyboardAction(TextFiledEventType type, float offsetY) {
    if (type == TEXTFIELD_EVENT_ATTACH_WITH_IME) {
        float moveY = offsetY > 0 ? offsetY : TNVisibleRect::getVisibleRect().size.height / 2;
        CCMoveTo *moveTo = CCMoveTo::create(0.1f, ccp(m_mainWidget->getPositionX(), moveY));
        m_mainWidget->runAction(moveTo);
    }else if (type == TEXTFIELD_EVENT_DETACH_WITH_IME){
        CCMoveTo *moveTo = CCMoveTo::create(0.1f, ccp(m_mainWidget->getPositionX(), 0));
        m_mainWidget->runAction(moveTo);
    }
}

Widget *TNWidget::getMainWidget() {
    return m_mainWidget;
}

void TNWidget::setInputParam(Ref *inputParam) {
    if (m_inputParam != inputParam) {
        CC_SAFE_RELEASE_NULL(m_inputParam);
        m_inputParam = inputParam;
        m_inputParam->retain();
    }
}

Ref *TNWidget::getInputParam() {
    return m_inputParam;
}

bool TNWidget::fixHeight() {
    return false;
}

bool TNWidget::fixWidth() {
    return false;
}

void TNWidget::showLoading(bool canTouch) {
    // 模态背景框，不能触摸
    if (NULL == m_loadingLayout) {
        m_loadingLayout = Layout::create();
        m_loadingLayout->retain();
        m_loadingLayout->setAnchorPoint(ccp(0.5, 0.5));
        m_loadingLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
        m_loadingLayout->setBackGroundColor(Color3B::BLACK);
        m_loadingLayout->setBackGroundColorOpacity(125);
    }
    if (NULL == m_loadingLayout->getParent()) {
        addChild(m_loadingLayout);
    }
    m_loadingLayout->setTouchEnabled(true);
    m_loadingLayout->setVisible(true);

    // loading 指示器
    if (NULL == m_loadingWidget) {
        m_loadingWidget = ImageView::create();
		m_loadingWidget->loadTexture("Common/Common_loading_icon.png", UI_TEX_TYPE_LOCAL);
		m_loadingWidget->getVirtualRenderer()->runAction(CCRepeatForever::create(CCRotateBy::create(0.2f, 30)));
		
        m_loadingWidget->setAnchorPoint(ccp(0.5, 0.5));
        if (NULL != m_mainWidget) {
            m_loadingWidget->setPosition(m_mainWidget->getSize() / 2);
        } else {
            m_loadingWidget->setPosition(TNVisibleRect::center());
        }
        addChild(m_loadingWidget);
    }
    m_loadingWidget->setVisible(true);
}

void TNWidget::hideLoading() {
    if (NULL != m_loadingLayout) {
        m_loadingLayout->setVisible(false);
        m_loadingLayout->removeFromParent();
        setTouchEnabled(false);
    }

    if (NULL != m_loadingWidget) {
        m_loadingWidget->setVisible(false);
    }
}
