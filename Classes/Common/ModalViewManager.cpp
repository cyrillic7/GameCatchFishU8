#include "ModalViewManager.h"

static ModalViewManager *__instance = NULL;

ModalViewManager::ModalViewManager()
{
	m_widgetTag = 0;
}

ModalViewManager::~ModalViewManager()
{
}

bool ModalViewManager::init()
{
	if (!Layout::init())
	{
		return false;
	}
	m_touchGroup = Layout::create();
	addChild(m_touchGroup);

	//setTouchPriority(-129);

	return true;
}

ModalViewManager* ModalViewManager::sharedInstance()
{
	if (NULL == __instance)
	{
		__instance = new ModalViewManager;
		__instance->init();
	}
	return __instance;
}

void ModalViewManager::purgeInstance()
{
	CC_SAFE_DELETE(__instance);
}

int ModalViewManager::showWidget( Widget *w, bool canBeRemove)
{
	if (NULL != w && !m_touchGroup->getChildren().contains(w)) {
		m_touchGroup->addChild(w);
		m_widgetTag ++;
		w->setTag(m_widgetTag);
        if (!canBeRemove) { //不可以被移除
            w->setName(WIDGET_CANNOT_REMVOVE_FLAG);
        }
       
		if (NULL == getParent()) {
			Scene *scene = Director::getInstance()->getRunningScene();
			scene->addChild(this, 10000);
			this->scheduleUpdate();
		}
		return m_widgetTag;
	}
	return 0;
}

void ModalViewManager::removeWidget( Widget *w )
{
	if (NULL != w && NULL != w->getParent())
	{
		w->removeFromParentAndCleanup(true);
	}
}

void ModalViewManager::removeAllWidgets(bool forceRemove)
{
    if (forceRemove) {
        m_touchGroup->removeAllChildrenWithCleanup(true);
    } else {
        Vector<Node*> children = m_touchGroup->getChildren();
        for (int i = m_touchGroup->getChildrenCount() - 1; i >= 0; i--) {
            Widget *w = (Widget*)children.at(i);
            if (0 != strcmp(w->getName().c_str(), WIDGET_CANNOT_REMVOVE_FLAG)) {
                w->removeFromParent();
            }
        }
    }
}

void ModalViewManager::update( float dt ) {
    // 每一帧检查一下，当前还有没有模态弹出层，如果没有就把自己从当前scene移除，当前scenen就可以恢复触摸事件了.
	if (m_touchGroup->getChildrenCount() == 0 && NULL != getParent()) {
		m_widgetTag = 0;
		this->unscheduleUpdate();
		this->removeFromParentAndCleanup(true);
        
	}
}

Widget* ModalViewManager::getWidgetWithTag( int tag )
{
	Widget* w = (Widget*)m_touchGroup->getChildByTag(tag);
	return w;
}

void ModalViewManager::removeWidgetWithTag( int tag )
{
	Widget *w = getWidgetWithTag(tag);
	removeWidget(w);
}

void ModalViewManager::removeExitWidget()
{
	Vector<Node*> children = m_touchGroup->getChildren();
	for (int i = m_touchGroup->getChildrenCount() - 1; i >= 0; i--) {
		Widget *w = (Widget*)children.at(i);
		if (0 == strcmp(w->getName().c_str(), WIDGET_CANNOT_REMVOVE_FLAG)) {
			w->removeFromParent();
		}
	}
}

void ModalViewManager::onExit() {
    this->unscheduleUpdate();
    Layout::onExit();
}

void ModalViewManager::removeWidgetsToTop() {
    for (int i = 0; i < m_touchGroup->getChildrenCount() - 1; ++i) {
        Widget *w = (Widget*)m_touchGroup->getChildren().at(i);
        w->removeFromParentAndCleanup(true);
    }
}
