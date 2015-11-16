#include "CommonLoadingWidget.h"
#include "CommonFunction.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "Message.h"

CommonLoadingWidget::CommonLoadingWidget() {
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(netWorkValidMsg, CC_CALLBACK_1(CommonLoadingWidget::netWorkIsValid, this)), this);
}

CommonLoadingWidget::~CommonLoadingWidget() {
	stopAllActions();
	_eventDispatcher->removeEventListenersForTarget(this);
}

CommonLoadingWidget* CommonLoadingWidget::create(Size size)
{
	CommonLoadingWidget *w = new CommonLoadingWidget;
	if (NULL != w && w->init(size)) {
		w->autorelease();
		return w;
	}
	CC_SAFE_DELETE(w);
	return NULL;
}

bool CommonLoadingWidget::init(Size size) {
    if (!Layout::init()) {
        return false;
    }

	setTag(COMMON_LOADING_TAG);
	setSize(size);

	setAnchorPoint(Vec2(0, 0));
	setBackGroundColorType(LAYOUT_COLOR_SOLID);
	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorOpacity(100);
	setTouchEnabled(true);

	ImageView* bg = ImageView::create();
	bg->loadTexture("common/commonloading/loading_logo_bg.png");
	bg->setPosition(Vec2(getSize().width/2, getSize().height/2));
	addChild(bg);

	//loading icon
	Sprite* icon =  Sprite::create();
	std::string imageName = "";
	std::vector<std::string> images;
	for (int i = 0 ; i < 8 ; i++)
	{
		imageName = __String::createWithFormat("common/commonloading/loading_logo_%d.png",i+1)->getCString();
		images.push_back(imageName);
	}

	Animate* pAction = CommonFunction::createFrameAnimate(images,0.14f);
	icon->runAction(RepeatForever::create(pAction));
	icon->setPosition(Vec2(bg->getContentSize().width/2 + 23, bg->getContentSize().height/2 +15));
	bg->addChild(icon);

	ImageView* qipao = ImageView::create();
	qipao->loadTexture("charge_qipao1.png");
	qipao->setPosition(Vec2(bg->getSize().width/2, bg->getSize().height/2));
	qipao->setScale(0.83f);
	bg->addChild(qipao);

	// loading icon
	/*ImageView* icon = ImageView::create();
	icon->loadTexture("common_loading_icon.png");
	icon->setPosition(Vec2(getSize().width/2, getSize().height/2));
	addChild(icon);
	m_icon = icon;

	icon->getVirtualRenderer()->runAction(RepeatForever::create(RotateBy::create(0.2f, 30)));*/

	// lable
	m_tipsLable = Text::create();
	m_tipsLable->setText(CommonFunction::GBKToUTF8("请稍候..."));
	m_tipsLable->setFontSize(30);
	m_tipsLable->setAnchorPoint(Vec2(0.5, 0.5));


	m_tipsLable->setPosition(Vec2(m_tipsLable->getContentSize().width/2,-90));
	icon->addChild(m_tipsLable);


	//超时时间
	DelayTime* dt = DelayTime::create(35.f);
	CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(CommonLoadingWidget::TimeOut));

	runAction(Sequence::createWithTwoActions(dt,callback));

    return true;
}

void CommonLoadingWidget::TimeOut( Node* node )
{
	node->removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("连接超时,请查看您的网络!")));
	
}

void CommonLoadingWidget::netWorkIsValid(EventCustom* evt)
{
	removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("连接失败，请查看您的网络!")));
}

void CommonLoadingWidget::setTips(std::string tips)
{
	m_tipsLable->setText(CommonFunction::GBKToUTF8(tips.c_str()));
    
    if (tips == "Bad networking..." || tips == "Finding a seat..." ) {
        m_tipsLable->setFontSize(12);
        this->setAnchorPoint(Vec2(0.5, 0));
        m_tipsLable->setPosition(Vec2(100, getSize().height / 2));
        //m_icon->setScale(0.5);
       // m_icon->setPosition(Vec2(20, m_icon->getPosition().y));
    }
}