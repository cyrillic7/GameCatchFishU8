#include "GameHallMsgWidget.h"
#include "Message.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "GameHallMsgItem.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"

#define  TabSystemTag 570
#define  ImageSystemTag 572
#define  ImagePrivateTag  573
#define  TabPrivatelTag 571

#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  ListViewTag      574

#define NoMsgNodeTag    1018

GameHallMsgWidget::GameHallMsgWidget()
{
	mMsgList = nullptr;
	mTextSystem = nullptr;
	mTextPrivate = nullptr;
	mTabSystem  = nullptr;
	mTabPrivate  = nullptr;

	mPageIndex = 1;//默认为礼包页面
}

GameHallMsgWidget::~GameHallMsgWidget()
{

}

bool GameHallMsgWidget::init()
{
	if (!TNWidget::init())
		return false;




	return true;
}

std::string GameHallMsgWidget::getWidgetJsonName()
{
	return "msgWidget";
}

void GameHallMsgWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定相关事件

	mTabSystem = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabSystemTag));
	mTabSystem->addTouchEventListener(CC_CALLBACK_2(GameHallMsgWidget::onClickSystem, this));
	mTextSystem = static_cast<ImageView*>(mTabSystem->getChildByTag(ImageSystemTag));


	mTabPrivate = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabPrivatelTag));
	mTabPrivate->addTouchEventListener(CC_CALLBACK_2(GameHallMsgWidget::onClickPrivate, this));
	mTextPrivate = static_cast<ImageView*>(mTabPrivate->getChildByTag(ImagePrivateTag));

	//
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallMsgWidget::onBack, this));

	//Layout* panel = static_cast<Layout*>(popBg->getChildByTag(PanelTag));
	//mMsgList= ListView::create();
	//mMsgList->setBounceEnabled(true);
	//mMsgList->setDirection(ui::ScrollView::Direction::VERTICAL);
	//mMsgList->setAnchorPoint(ccp(0.5,0.5));
	//mMsgList->ignoreContentAdaptWithSize(false);
	//mMsgList->setContentSize(Size(panel->getContentSize().width,panel->getContentSize().height));
	//mMsgList->setPosition(ccp(panel->getContentSize().width/2,panel->getContentSize().height/2));
	//panel->addChild(mMsgList);
	mMsgList = static_cast<ListView*>(popBg->getChildByTag(ListViewTag));

}



void GameHallMsgWidget::onEnter()
{

	SessionManager::shareInstance()->setNewMsgTag(false);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);

	TNWidget::onEnter();
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GetShopListFromServerMessage, CC_CALLBACK_1(GameHallMsgWidget::refreshMsgList, this)), this);

	refreshData(1);
}

void GameHallMsgWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallMsgWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}



void GameHallMsgWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallMsgWidget::refreshMsgList(EventCustom* event)
{
	removeLoading();
	__Dictionary* infoDic = (__Dictionary*)event->getUserData();
	__Array* dataArray = (__Array*)infoDic->objectForKey("goods");
	int type = ((__Integer*)infoDic->objectForKey("type"))->getValue();

	if (dataArray->count() <=0)
		return;

	//mGiftList->refreashList(dataArray,type);
}

void GameHallMsgWidget::onClickPrivate(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex == 1)
		{
			mPageIndex = 2 ;
			Vec2 tab1pos = mTabSystem->getPosition();
			Vec2 tab2pos = mTabPrivate->getPosition();
			mTabPrivate->setPosition(Vec2(mTabPrivate->getPositionX(),tab1pos.y));
			mTabPrivate->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextPrivate->loadTexture("font_my_message.png",Widget::TextureResType::PLIST);
			mTabSystem->setPosition(Vec2(mTabSystem->getPositionX(),tab2pos.y));
			mTabSystem->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			mTextSystem->loadTexture("btn_sysmessage_on.png",Widget::TextureResType::PLIST);

			refreshData(mPageIndex);
		}
	}

}

void GameHallMsgWidget::onClickSystem(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex == 2)
		{
			mPageIndex = 1 ; 

			Vec2 tab1pos = mTabSystem->getPosition();
			Vec2 tab2pos = mTabPrivate->getPosition();
			mTabPrivate->setPosition(Vec2(mTabPrivate->getPositionX(),tab1pos.y));
			mTabPrivate->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			mTextPrivate->loadTexture("font_my_message_on.png",Widget::TextureResType::PLIST);
			mTabSystem->setPosition(Vec2(mTabSystem->getPositionX(),tab2pos.y));
			mTabSystem->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextSystem->loadTexture("btn_sysmessage.png",Widget::TextureResType::PLIST);

			refreshData(mPageIndex);
		}
	}
}


void GameHallMsgWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}
}

void GameHallMsgWidget::refreshData(int type)
{
	__Array* dataList = SessionManager::shareInstance()->getMsgArray();
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	if (dataList->count() == 0  || calMsgCountByType(dataList,type) == 0)//发送请求
	{
		//添加无消息动画效果
		mMsgList->removeAllItems();
		
		if (!popBg->getChildByTag(NoMsgNodeTag))
		{
			Node* noMsgNode = Node::create();
			noMsgNode->setPosition(Vec2(popBg->getContentSize().width/2,popBg->getContentSize().height/2 + 100 ));
			noMsgNode->setRotation(-3);
			popBg->addChild(noMsgNode,0,NoMsgNodeTag);

			ImageView* title = ImageView::create();
			title->loadTexture("nulltitle.png",ui::Widget::TextureResType::LOCAL);
			
			noMsgNode->addChild(title);


			ImageView* nomsg = ImageView::create();
			nomsg->loadTexture("nullbg.png",ui::Widget::TextureResType::LOCAL);
			nomsg->setAnchorPoint(Vec2(0.5,1.0f));
			nomsg->setScale(1.5f);
			//nomsg->setPosition(Vec2(0,nomsg->getContentSize().height/2));
			nomsg->setLocalZOrder(title->getLocalZOrder()-1);
			noMsgNode->addChild(nomsg);

			//title->setPosition(Vec2(0,nomsg->getContentSize().height/2));
			Vector<FiniteTimeAction*> actions;
			RotateTo* rb = RotateTo::create(1.0f,6);
			actions.pushBack(rb);
			RotateTo* rb2 = RotateTo::create(1.0f,-6);
			actions.pushBack(rb2);
			noMsgNode->runAction(RepeatForever::create(Sequence::create(actions)));

			//提示文字
			Text* tipLabel = Text::create();
			tipLabel->setAnchorPoint(ccp(0.5, 0.5));
			tipLabel->setFontSize(25);
			tipLabel->setFontName("Arail Regular");
			tipLabel->setTextColor(ccc4(96,33,0,255));
			tipLabel->setText(CommonFunction::GBKToUTF8("您的消息为空!"));
			tipLabel->setPosition(Vec2(0,-140));
			noMsgNode->addChild(tipLabel);

		}
	}
	else
	{
		if (popBg->getChildByTag(NoMsgNodeTag))
		{
			popBg->removeChildByTag(NoMsgNodeTag);
		}

		//刷新msg列表
		mMsgList->removeAllItems();
		for (int i = 0 ; i < dataList->count();i++)
		{
			__Dictionary* dic = (__Dictionary*)dataList->getObjectAtIndex(i);
			int userId = ((__Integer*)dic->objectForKey("userId"))->getValue();
			if (type == 1 && userId ==0)//系统消息页
			{
				//添加消息项
				GameHallMsgItem* pItem = GameHallMsgItem::create(dic,mMsgList->getContentSize().width - 50);
				pItem->addTouchEventListener(CC_CALLBACK_2(GameHallMsgWidget::onClickItem,this));
				mMsgList->pushBackCustomItem(pItem);
			}
			else if(type == 2 && userId > 0 )//个人消息业
			{
				//添加消息项
				GameHallMsgItem* pItem = GameHallMsgItem::create(dic,mMsgList->getContentSize().width - 50);
				pItem->addTouchEventListener(CC_CALLBACK_2(GameHallMsgWidget::onClickItem,this));
				mMsgList->pushBackCustomItem(pItem);
			}
		}
	}
	//每进一次消息界面保存一下数据
	SessionManager::shareInstance()->SaveReadMsgInfo();
}

int GameHallMsgWidget::calMsgCountByType(__Array* pArray,int type)
{
	int count = 0;
	for (int i = 0 ; i < pArray->count(); i++)
	{
		__Dictionary* dic = (__Dictionary*)pArray->getObjectAtIndex(i);
		int userId = ((__Integer*)dic->objectForKey("userId"))->getValue();
		if (type == 1)//系统消息
		{
			if (userId == 0 )
			{
				count++;
			}
		}else if (type == 2)//个人消息
		{
			if (userId > 0 )
			{
				count++;
			}
		}
	}

	return count;
}

void GameHallMsgWidget::onClickItem(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		GameHallMsgItem* pItem = (GameHallMsgItem*)pSender;
		__Dictionary* infodic = (__Dictionary*)pItem->getData();
		__String* content = (__String*)infodic->objectForKey("msgContent");
		//__String* content = __String::create("字花样百出甘划框框一基本原理 基本原理 基本原理巧夺天工玩世不恭大型要基本原理基本原理 在一革夺爱爱爱君子坦荡荡么多");
		ModalViewManager::sharedInstance()->showWidget(GameHallMsgDescWidget::create(content->getCString()));
		removeFromParent();
	}
}