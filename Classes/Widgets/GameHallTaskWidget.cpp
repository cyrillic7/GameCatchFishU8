#include "GameHallTaskWidget.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "CommonFunction.h"
#include "GameHallShopItem.h"
#include "Common/WebImageView.h"
#include "Common/ModalViewManager.h"
#include "Common/AlertWidget.h"
#include "GameServiceClientManager.h"
#include "Common/pystring.h"

static const char jiangjuang[] = { 0xE5,0xA5,0x96,0xE5,0x88,0xB8,0x00 };

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  ListViewGoldTag 1874
//#define  ListViewTimeTag 1876

#define  NoMsgNodeTag      10188

#define  SendGetTaskAwardRequestMsg "SendGetTaskAwardRequestMsg"

GameHallTaskItem::GameHallTaskItem()
{
	mdata = nullptr;
	
}

GameHallTaskItem::~GameHallTaskItem()
{
	CC_SAFE_RELEASE_NULL(mdata);
}

GameHallTaskItem* GameHallTaskItem::create(__Dictionary* info)
{
	GameHallTaskItem* pItem = new GameHallTaskItem();
	if (pItem && pItem->init(info))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		delete pItem;
		pItem = nullptr;
		return nullptr;
	}
}

bool GameHallTaskItem::init(__Dictionary* info)
{
	if(!Layout::init())
		return false;

	mdata  = info;
	mdata->retain();

	int curProgrss = ((__Integer*)info->objectForKey("curProgress"))->getValue();
	int totalProgrss = ((__Integer*)info->objectForKey("totalProgress"))->getValue();

	__Integer* awardValue = (__Integer*)info->objectForKey("awardValue1");
	
	std::string imageBg = "task_bg.png";
	bool isFinish = false;
	if (curProgrss >= totalProgrss && curProgrss > 0 && (awardValue && awardValue->getValue() > 0) )
	{
		isFinish = true;
	}
	SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageBg);
	ui::Scale9Sprite* itembg = ui::Scale9Sprite::createWithSpriteFrame(frame);
	itembg->setContentSize(Size(262,412));
	itembg->setPosition(Vec2(268/2,412/2));
	addChild(itembg);
	
	bool showJidu = true;
	bool showPersent = false;
	bool isNOJinDu = false;
	bool isTimeTask = false;
	std::string title_name = "";
	__String* titleStr = (__String*)mdata->objectForKey("title");
	if (strcmp(titleStr->getCString(),"捕捉特定鱼") == 0)
	{
		title_name = "task_tiltle2.png";
		
	}else if(strcmp(titleStr->getCString(),"累计赢金币") == 0)
	{
		title_name = "task_tiltle1.png";		
		showPersent = true;
	}
	else//累计时间
	{
		title_name = "task_tiltle3.png";
		isTimeTask = true;
	}

	ImageView* imageTitle = ImageView::create();
	imageTitle->loadTexture(title_name,ui::Widget::TextureResType::LOCAL);
	imageTitle->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height - 30));
	itembg->addChild(imageTitle);

	ImageView* shine = ImageView::create("shop_item_shine.png",Widget::TextureResType::PLIST);
	shine->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height - 150));
	itembg->addChild(shine);

	__String* desc_str = (__String*)info->objectForKey("awardDesc1");
	std::string name =((__String*)info->objectForKey("imageName"))->getCString();
	
	WebImageView* imageIcon = WebImageView::create();
	imageIcon->imageWithUrl(__String::createWithFormat(TaskURLFormat,name.c_str())->getCString());
	imageIcon->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height - 140));
	itembg->addChild(imageIcon);

	int awardItemCount= calItemCount(info);
	int awardItemWidth;
	if (awardItemCount == 1)
	{
		awardItemWidth = 211;
	}
	else if(awardItemCount == 2)
	{
		awardItemWidth = 220;
	}
	else if(awardItemCount = 3)
	{
		awardItemWidth  =240;
	}
	

	//award bg
    frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("task_award_bg.png");
	ui::Scale9Sprite* awardBg = ui::Scale9Sprite::createWithSpriteFrame(frame,Rect(10,15,7,11));
	awardBg->setContentSize(Size(awardItemWidth,55));
	awardBg->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height - 250));
	itembg->addChild(awardBg,1);

	//箭头
	ImageView* imageMirrow = ImageView::create();
	imageMirrow->loadTexture("task_award_mirrow.png",ui::Widget::TextureResType::PLIST);
	imageMirrow->setPosition(Vec2(itembg->getContentSize().width/2,awardBg->getPositionY()+awardBg->getContentSize().height/2+imageMirrow->getContentSize().height/2-2));
	itembg->addChild(imageMirrow,0);

	std::string award_content = "";
	int award_v = ((__Integer*)info->objectForKey(__String::createWithFormat("awardValue%d",1)->getCString()))->getValue() ;
	desc_str = (__String*)info->objectForKey(__String::createWithFormat("awardDesc%d",1)->getCString());
	if (award_v > 0)
	{
		award_content += __String::createWithFormat("%d%s+",award_v,CommonFunction::GBKToUTF8(desc_str->getCString()).c_str())->getCString();
	}
	award_v = ((__Integer*)info->objectForKey(__String::createWithFormat("awardValue%d",2)->getCString()))->getValue() ;
	desc_str = (__String*)info->objectForKey(__String::createWithFormat("awardDesc%d",2)->getCString());
	if (award_v > 0)
	{
		award_content += __String::createWithFormat("%d%s+",award_v,CommonFunction::GBKToUTF8(desc_str->getCString()).c_str())->getCString();
	}

	award_v = ((__Integer*)info->objectForKey(__String::createWithFormat("awardValue%d",3)->getCString()))->getValue() ;
	desc_str = (__String*)info->objectForKey(__String::createWithFormat("awardDesc%d",3)->getCString());
	if (award_v > 0)
	{
		award_content += __String::createWithFormat("%d%s+",award_v,CommonFunction::GBKToUTF8(desc_str->getCString()).c_str())->getCString();
	}

	award_content = pystring::slice(award_content,0,award_content.length() -1);

	Text* awardText = Text::create();
	//__String* str_ = __String::createWithFormat("%s*%d",CommonFunction::GBKToUTF8(desc_str->getCString()).c_str(),award_v);
	awardText->setTextColor(ccc4(255,255,255,255));
	awardText->setFontSize(20);
	awardText->setText(award_content);
	awardText->setPosition(Vec2(awardBg->getContentSize().width/2,awardBg->getContentSize().height/2));
	awardBg->addChild(awardText);

	Text* text_remark = Text::create();
	text_remark->setText(CommonFunction::GBKToUTF8(((__String*)info->objectForKey("remark"))->getCString()));
	text_remark->setTextColor(ccc4(96,33,0,255));
	text_remark->setAnchorPoint(Vec2(0.5,0.5));
	text_remark->setFontSize(25);
	text_remark->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height/2  - 110)); 
	itembg->addChild(text_remark);

		if (curProgrss >0 && curProgrss < totalProgrss)
		{
			if(!isNOJinDu)
			{

			}
			else
			{
				ImageView* nowing = ImageView::create();
				nowing->loadTexture("image_nowing.png",ui::Widget::TextureResType::PLIST);
				nowing->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height/2  - 160 ));
				itembg->addChild(nowing);
			}
			
		}
		else if(curProgrss >= totalProgrss && curProgrss > 0 )
		{
			if (awardValue && awardValue->getValue() > 0)
			{
				/*	Button* btn_get = Button::create();
				btn_get->loadTextureNormal("task_btn_get.png",ui::Widget::TextureResType::LOCAL);
				btn_get->setPosition(Vec2(itembg->getContentSize().width/2 ,itembg->getContentSize().height/2  - 160));
				itembg->addChild(btn_get);*/

				Button* btn_get = Button::create();
				btn_get->loadTextureNormal("btn_3.png",ui::Widget::TextureResType::PLIST);
				btn_get->loadTexturePressed("btn_3.png",ui::Widget::TextureResType::PLIST);
				btn_get->setTitleText(CommonFunction::GBKToUTF8("领取奖励"));
				btn_get->setTitleFontSize(25);
				btn_get->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height/2  - 160));
				itembg->addChild(btn_get);

				btn_get->addTouchEventListener(CC_CALLBACK_2(GameHallTaskItem::onGetAward, this));
			}
			else
			{
				ImageView* hasGet = ImageView::create();
				hasGet->loadTexture("image_yiding.png",ui::Widget::TextureResType::PLIST);
				hasGet->setPosition(Vec2(itembg->getContentSize().width /2 ,itembg->getContentSize().height/2  - 160 ));
				itembg->addChild(hasGet);
			}

		} else if ( isNOJinDu && curProgrss ==0 )
		{
			ImageView* nowing = ImageView::create();
			nowing->loadTexture("image_nowing.png",ui::Widget::TextureResType::PLIST);
			nowing->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height/2  - 160 ));
			itembg->addChild(nowing);
		}



	if (curProgrss >= totalProgrss && curProgrss > 0)
	{
		showJidu = false;
	}

	if (showJidu)
	{
		frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("progressbg.png");
		ui::Scale9Sprite* progressBg = ui::Scale9Sprite::createWithSpriteFrame(frame);
		progressBg->setContentSize(Size(160,22));
		progressBg->setPosition(Vec2(itembg->getContentSize().width/2 ,itembg->getContentSize().height/2  - 160));
		itembg->addChild(progressBg);

		int p = curProgrss*100/totalProgrss;
		if ((curProgrss > 0 && p >=1 && !isTimeTask ) || (isTimeTask && curProgrss/60 >=1))
		{
			frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("progress.png");
			ui::Scale9Sprite* progressV = ui::Scale9Sprite::createWithSpriteFrame(frame);

			int v_width = curProgrss* progressBg->getContentSize().width / totalProgrss;
			if (p <20 )
				v_width = progressV->getContentSize().width/2 + 5 + v_width;
			
			progressV->setContentSize(Size(v_width,22));
			progressV->setPosition(Vec2(progressV->getContentSize().width/2,progressV->getContentSize().height/2));
			progressBg->addChild(progressV);
		}

		//进度值
		Text* persent = Text::create();
		if (showPersent )
		{
			int p = curProgrss*100/totalProgrss;
			persent->setText(__String::createWithFormat("%d%%",p)->getCString());
		}
		else
		{
			//float f_p = curProgrss*100.0/totalProgrss;
			if (isTimeTask)
			{
				curProgrss = curProgrss/60;
				totalProgrss = totalProgrss/60;
			}
			persent->setString(__String::createWithFormat("%d/%d",curProgrss,totalProgrss)->getCString());
		}
		persent->setTextColor(ccc4(96,33,0,255));
		persent->setAnchorPoint(Vec2(0.5,0.5));
		persent->setFontSize(20);
		persent->setPosition(Vec2(progressBg->getContentSize().width/2 ,progressBg->getContentSize().height/2)); 
		progressBg->addChild(persent);
	}

	return true;
}	

void GameHallTaskItem::onGetAward(Ref* pSender,ui::Widget::TouchEventType type)
{
	if(type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(SendGetTaskAwardRequestMsg,mdata);
	}
}

int GameHallTaskItem::calItemCount(__Dictionary* info)
{
	int col  =0;//列数
	 __Integer* pValue = (__Integer*)info->objectForKey("awardValue1");
	 if (pValue->getValue() >0)
	 {
		 col++;
	 }
	 pValue = (__Integer*)info->objectForKey("awardValue2");
	 if (pValue->getValue() >0)
	 {
		 col++;
	 }
	 pValue = (__Integer*)info->objectForKey("awardValue3");
	 if (pValue->getValue() >0)
	 {
		 col++;
	 }

	 return col;
}


//////////////////////////////////////////////////////////////////////////

GameHallTaskWidget::GameHallTaskWidget()
{
	mClientName = "taskSorket";
	mRequestType = 0;
	mBGetFail = false;
	SessionManager::shareInstance()->setNewTaskTag(false);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);
}

GameHallTaskWidget::~GameHallTaskWidget()
{

}

void GameHallTaskWidget::onEnter()
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	if(!c->isConnect())
	{
		mBGetFail = true;
	}
	TNWidget::onEnter();
	//绑定相关事件
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getTaskListMsg, CC_CALLBACK_1(GameHallTaskWidget::refreshData, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getTaskAwardMsg, CC_CALLBACK_1(GameHallTaskWidget::receiveAwardResultMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(SendGetTaskAwardRequestMsg,CC_CALLBACK_1(GameHallTaskWidget::sendAwardRequest,this)),this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(TaskSorketCloseMsg,CC_CALLBACK_1(GameHallTaskWidget::taskSorketConnetFail,this)),this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginTaskServerSuccessMsg,CC_CALLBACK_1(GameHallTaskWidget::receiveLoginSuccessMsg,this)),this);
	if(!c->isConnect())
	{
		c->loginTaskServer();
	}
	else
	{
		c->sendGetTaskList();
		showLoading();
	}
}

void GameHallTaskWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

std::string GameHallTaskWidget::getWidgetJsonName()
{
	return "taskWidget";
}

bool GameHallTaskWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}

	////初始化SOCKET
	//GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	//m->registerServiceClient(mClientName.c_str(), false,TASK_SORKET_INDEX);
	//GameServiceClient* c = m->serviceClientForName(mClientName.c_str());
	//
	//
	//bool ret = c->Connect(SessionManager::shareInstance()->getTaskServer().c_str(),SessionManager::shareInstance()->getTaskPort());
	//if (ret)
	//{
	//	c->loginTaskServer();
	//}

	return true;
}

void GameHallTaskWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallTaskWidget::onBack, this));

	mGoldTaskList = static_cast<ListView*>(popBg->getChildByTag(ListViewGoldTag));
	mGoldTaskList->setBounceEnabled(true);

	if (mBGetFail)
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
		tipLabel->setText(CommonFunction::GBKToUTF8("获取失败，请重新获取!"));
		tipLabel->setPosition(Vec2(0,-140));
		noMsgNode->addChild(tipLabel);
	}

}


void GameHallTaskWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallTaskWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallTaskWidget::playScaleAnimation(bool less, Ref* pSender) 
{
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}

void GameHallTaskWidget::refreshData(EventCustom* evt)
{
	removeLoading();
	CMD_GL_TaskInfo* taskInfo = (CMD_GL_TaskInfo*)evt->getUserData();
	int cnt = SessionManager::shareInstance()->getTaskCnt();

	if (mBGetFail)
	{
		ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));
		Node*   pNode =    (Node* )popBg->getChildByTag(NoMsgNodeTag) ;   
		if (pNode)
		{
			pNode->removeFromParent();
		}
	}
	mGoldTaskList->removeAllItems();
	for (int i = 0; i < cnt; i++)
	{
		
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__String::create(taskInfo->szTitle),"title");
		dic->setObject(__Integer::create(taskInfo->dwTaskID),"taskId");
		dic->setObject(__String::create(taskInfo->szImgName),"imageName");
		dic->setObject(__Integer::create(taskInfo->dwCurProgress),"curProgress");
		dic->setObject(__Integer::create(taskInfo->dwProgress),"totalProgress");
		dic->setObject(__String::create(taskInfo->szRemarks),"remark");

		CMD_GL_TaskAward award = taskInfo->Award;

		dic->setObject(__Integer::create(award.dwAward1),"awardValue1");
		dic->setObject(__String::create(award.szAward1),"awardDesc1");

		dic->setObject(__Integer::create(award.dwAward2),"awardValue2");
		dic->setObject(__String::create(award.szAward2),"awardDesc2");

		dic->setObject(__Integer::create(award.dwAward3),"awardValue3");
		dic->setObject(__String::create(award.szAward3),"awardDesc3");

		GameHallTaskItem* pItem  = GameHallTaskItem::create(dic);
		if (pItem)
		{
			pItem->setContentSize(Size(268,412));
			pItem->setAnchorPoint(Vec2(0.5f,0.5f));
			pItem->setScale(0.95f);
			mGoldTaskList->pushBackCustomItem(pItem);
		}

		
		taskInfo++;
	}

}


void GameHallTaskWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if(type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallTaskWidget::onGet(Ref* pSender,ui::Widget::TouchEventType type)
{
	
}


void GameHallTaskWidget::receiveAwardResultMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GL_TaskIDLog* log = (CMD_GL_TaskIDLog*)evt->getUserData();
	if (log->lResultCode == 0)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("奖励领取成功!")));
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(log->szDescribeString)));
	}
	
	//刷新列表
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	if(!c->isConnect())
	{
		mRequestType = 0;
		c->loginTaskServer();
		showLoading();
	}
	else
	{
		c->sendGetTaskList();
		showLoading();
	}	
	
}

void GameHallTaskWidget::sendAwardRequest(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	int taskId =((__Integer*)info->objectForKey("taskId"))->getValue();
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	if (!c->isConnect())
	{
		mRequestType = 1;
		mTaskId = taskId;
		c->loginTaskServer();
		showLoading();
	}
	else
	{
		c->sendTaskAwardRequest(taskId);
		showLoading();
	}
	
}

void GameHallTaskWidget::taskSorketConnetFail(EventCustom* evt)
{
	removeLoading();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("网络异常,请重试!")));
}

void GameHallTaskWidget::receiveLoginSuccessMsg(EventCustom* evt)
{
	removeLoading();
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	if (mRequestType== 0)
	{
		c->sendGetTaskList();
		showLoading();
	}
	else
	{
		c->sendTaskAwardRequest(mTaskId);
		showLoading();
	}
}