#include "GameHallAboutWidget.h"
#include "CommonFunction.h"

#define  ImagePopBg 18
#define  BtnColseTag 42
#define  BtnSendGoodTag 55
#define  ImageTabTag 9756

#define  ImageTab1Tag 9758
#define  ImageTab2Tag 9760
#define  ImageTab3Tag 9762

#define bitWidth 115

GameHallAboutWidget::GameHallAboutWidget()
{
	mContentText = nullptr;
	mPageIndex =1;
	m_popBg = nullptr;
}

GameHallAboutWidget::~GameHallAboutWidget()
{

}


bool GameHallAboutWidget::init()
{
	if(!TNWidget::init())
		return false;

	return true;
}

std::string GameHallAboutWidget::getWidgetJsonName()
{
	return "aboutWidget";
}

void GameHallAboutWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	m_popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBg));

	Button* btn_close = static_cast<Button*>(m_popBg->getChildByTag(BtnColseTag));

	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallAboutWidget::onClose,this));



	//Button* btn_sendGood = static_cast<Button*>(m_popBg->getChildByTag(BtnSendGoodTag));

	//btn_sendGood->addTouchEventListener(CC_CALLBACK_2(GameHallAboutWidget::onSendGood,this));


	ImageView* image_tab = static_cast<ImageView*>(m_popBg->getChildByTag(ImageTabTag));
	image_tab->addTouchEventListener(CC_CALLBACK_2(GameHallAboutWidget::onSwitchPanel,this));

	m_tab1 = static_cast<ImageView*>(m_popBg->getChildByTag(ImageTab1Tag));

	m_tab2 = static_cast<ImageView*>(m_popBg->getChildByTag(ImageTab2Tag));
	m_tab2->setVisible(false);

	m_tab3 = static_cast<ImageView*>(m_popBg->getChildByTag(ImageTab3Tag));
	m_tab3->setVisible(false);

	/*mContentText =  LabelTTF::create(CommonFunction::GBKToUTF8("       七彩牛休闲游戏中心致力于为广大玩家带来高品质的网络游戏、完善的技术支持与售后服务。通过向用户提供包括大型多人的棋牌社区游戏、高级休闲游戏、网页游戏等多样化的网络游戏产品，满足各类游戏用户的普遍娱乐需求。\n\
															       以平台为中心，在自主开发的平台小游戏设计理念上体现了强烈的民族特色和深厚的中国文化底蕴，并提供极具地方特色的游戏，包括纯正的地方性玩法，地方性方言配音以及当地名胜命名的游戏房间，为全国各地网民提供一个最地道家乡味的网络休闲平台。"),
																"微软雅黑",25,Size(m_popBg->getContentSize().width - 150,m_popBg->getContentSize().height -170),TextHAlignment::LEFT);
	mContentText->setColor(ccc3(96,34,0));
	mContentText->setAnchorPoint(Vec2(0.5,0.5));
	mContentText->setPosition(Vec2(m_popBg->getContentSize().width/2,m_popBg->getContentSize().height/2));
	m_popBg->addChild(mContentText);
*/
	Label* content = Label::createWithSystemFont(CommonFunction::GBKToUTF8("	七彩牛休闲游戏中心致力于为广大玩家带来高品质的网络游戏、完善的技术支持与售后服务。通过向用户提供包括大型多人的棋牌社区游戏、高级休闲游戏、网页游戏等多样化的网络游戏产品，满足各类游戏用户的普遍娱乐需求。\n	以平台为中心，在自主开发的平台小游戏设计理念上体现了强烈的民族特色和深厚的中国文化底蕴，并提供极具地方特色的游戏，包括纯正的地方性玩法，地方性方言配音以及当地名胜命名的游戏房间，为全国各地网民提供一个最地道家乡味的网络休闲平台。"),
												"微软雅黑",25,	Size(m_popBg->getContentSize().width - 150,m_popBg->getContentSize().height -170),TextHAlignment::LEFT);
	content->setLineBreakWithoutSpace(true);
	content->setColor(ccc3(96,34,0));
	content->setAnchorPoint(Vec2(0.5,0.5));
	content->setPosition(Vec2(m_popBg->getContentSize().width/2,m_popBg->getContentSize().height/2));
	m_popBg->addChild(content);
}

void GameHallAboutWidget::onClose(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallAboutWidget::onSendGood(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallAboutWidget::onSwitchPanel(Ref* pSender,ui::Widget::TouchEventType type)
{

	Vec2 touchPos;
	int index =0;
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ImageView* pBit = (ImageView*)pSender;

		touchPos = pBit->getTouchEndPosition();
		Vec2 pos = pBit->getWorldPosition();
		Vec2 wordPos = convertToWorldSpace(touchPos);
		Vec2 delta ;
		Vec2::subtract(wordPos,pos,&delta);
		if ( (-1*bitWidth< delta.x &&delta.x < 0) || (delta.x <= bitWidth && delta.x > 0))
		{
			index = 2;
		}
		else if ( delta.x < -1*bitWidth)
		{
			index = 1;
		}
		else if (bitWidth< delta.x  )
		{
			index = 3;
		}

		if (mPageIndex != index)//切换文本内容
		{
			mPageIndex = index;
			changeText();
			
		}
	}
}

void GameHallAboutWidget::changeText()
{
	switch (mPageIndex)
	{
	case 1:
		{
			mContentText->setString(CommonFunction::GBKToUTF8("服务协议论苦东奔西走 基本原理基本原理基本原理巧夺天工要苯巧夺天工基本原理匡七"));
			m_tab1->setVisible(true);
			m_tab2->setVisible(false);
			m_tab3->setVisible(false);
		}
		break;
	case 2:
		{
			mContentText->setString(CommonFunction::GBKToUTF8("隐私策略论苦东奔西走 基本原理基本原理基本原理巧夺天工要苯巧夺天工基本原理匡七"));
			m_tab1->setVisible(false);
			m_tab2->setVisible(true);
			m_tab3->setVisible(false);
		}
		break;
	case 3:
		{
			mContentText->setString(CommonFunction::GBKToUTF8("金币声明论苦东奔西走 基本原理基本原理基本原理巧夺天工要苯巧夺天工基本原理匡七"));
			m_tab1->setVisible(false);
			m_tab2->setVisible(false);
			m_tab3->setVisible(true);
		}
		break;
	}
}