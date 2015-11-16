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
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(netWorkValidMsg, CC_CALLBACK_1(TNWidget::netWorkIsValid, this)), this);
    if (m_isInit) {
        m_isInit = false;
        loadUI();
    }
}

void TNWidget::onExit() {
	_eventDispatcher->removeEventListenersForTarget(this);
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

void TNWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5, 0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void TNWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}
void TNWidget::netWorkIsValid(EventCustom* evt)
{
	removeLoading();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", "连接失败，请查看您的网络!"));
}
