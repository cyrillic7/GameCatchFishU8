#include "GameHallVipPower.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "ui/UIScale9Sprite.h"
#include "GameHallShopWidget.h"
#include "Common/ModalViewManager.h"
#include "Common/AlertWidget.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"
#include "Widgets/GameHallChargeWidget.h"

#define  popBgTag 18
#define  BtnCloseTag 42
#define  BtnChargeTag        55

#define  ImageCurVipTag    1496
#define  ProgressBarTag      1497
#define  LableVipDesTag     1500
#define  ImageFontVip1Tag    1501
#define  ImageFontVip2Tag    1503

#define  PanelTabTag  1504
#define  ImageAwardBgTag 10398
#define  ListViewTag 10400

#define ImageVipTab1Tag 10403
#define LabelVipNum1Tag 10405
#define ImageVipTab2Tag 10404
#define LabelVipNum2Tag 10406
#define ImageVipTab3Tag 10407	
#define LabelVipNum3Tag 10417
#define ImageVipTab4Tag 10408
#define LabelVipNum4Tag 10413
#define ImageVipTab5Tag 10409
#define LabelVipNum5Tag 10414
#define ImageVipTab6Tag 10410
#define LabelVipNum6Tag 10415
#define ImageVipTab7Tag 10411
#define LabelVipNum7Tag 10416


#define AwardTypeGold 1
#define AwardTypeRed 2

const int  KVipTotalIngot[7] = {10,100,500,2000,5000,20000,50000};

VipItemWidget::VipItemWidget()
{
	mData = nullptr;
	mTextLable = nullptr;
	mBtnGet = nullptr;
	mType = ItemType::type_login;
	mImageTips = nullptr;
}

VipItemWidget::~VipItemWidget()
{
	CC_SAFE_RELEASE_NULL(mData);
}


VipItemWidget* VipItemWidget::create(int width,__Dictionary* info,ItemType type)
{
	VipItemWidget* pItem = new VipItemWidget();
	if (pItem && pItem->init(width,info,type))
	{
		pItem->autorelease();
		return pItem;
	}
	delete pItem;
	pItem = nullptr;
	return nullptr;
}

bool VipItemWidget::init(int width,__Dictionary* info,ItemType type)
{
	if(!TNWidget::init())
		return false;
	mData = info;
	mData->retain();

	mType = type;
	setContentSize(Size(width,242));

	addItem();
	return true;
}	



void VipItemWidget::onGetAward(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Integer::create(int(mType)),"type");
		if(mType == ItemType::type_login)
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ToSendGetAwardMsg,dic);
		else
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ToSendGetAwardMsg,dic);
	}
}

void VipItemWidget::onGotoShop(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(VipPowerToShopMsg);
	}
}

void VipItemWidget::onShopTips(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Integer::create(mType),"type");
		__Integer* vipLevel = (__Integer*)mData->objectForKey("vipLevel");
		dic->setObject(vipLevel,"vipLevel");
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(DealVipTipsMsg,dic);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	/*if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(VipPowerToChargeMsg);
	}*/
}

void VipItemWidget::showTips(Ref* pSender)
{
	if (mImageTips == nullptr)
	{
		Button* pButton = (Button*)pSender;
		Vec2 pos = pButton->getWorldPosition();
		mImageTips = ImageView::create();
		mImageTips->loadTexture("send_bg.png",ui::Widget::TextureResType::PLIST);
		mImageTips->setPosition(Vec2(pos.x - mImageTips->getContentSize().width/2 - 350 ,pos.y - mImageTips->getContentSize().height/2 - 50));
		getParent()->getParent()->getParent()->addChild(mImageTips);

		Text*  text_tip = Text::create();
		String* content = __String::createWithFormat("还需充值%d元宝领福利哦~",100);
		text_tip->setText(content->getCString());
		text_tip->setTextColor(ccc4(255,0,0,255));
		text_tip->setFontSize(15);
		text_tip->setPosition(Vec2(mImageTips->getContentSize().width/2,mImageTips->getContentSize().height/2));
		mImageTips->addChild(text_tip);
		
	}
}

void VipItemWidget::hideTips()
{
	if(mImageTips)
	{
		mImageTips->removeFromParent();
		mImageTips = nullptr;
	}
}

void VipItemWidget::addItem()
{
	SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("image_vippower_bg.png");
	ui::Scale9Sprite* itembg = ui::Scale9Sprite::createWithSpriteFrame(frame);
	itembg->setContentSize(Size(180,frame->getRect().size.height));
	itembg->setPosition(Vec2(180/2,itembg->getContentSize().height/2));
	addChild(itembg);

	std::string  title = "";
	std::string  strDesc ="";
	std::string  btnText ="领取奖励";
	std::string  iconName ="";
	switch (mType)
	{
	case VipItemWidget::type_login:
		{
			__Integer* vipLevel = (__Integer*)mData->objectForKey("vipLevel");
			iconName = __String::createWithFormat("u_vip_reward_goods%d.png",vipLevel->getValue())->getCString();
			title = "每日登录返还";
			__String* gold = (__String*)mData->objectForKey("loginScore");
			strDesc = __String::createWithFormat("%s%s",gold->getCString(),CommonFunction::GBKToUTF8("金币").c_str())->getCString();

		}
		break;
	case VipItemWidget::type_dayAward:
		{

			title = "每日赠送";
			__Integer* red = (__Integer*)mData->objectForKey("red");
			__Integer* redPiece = (__Integer*)mData->objectForKey("redPiece");
			if(red->getValue() > 0)
			{
				strDesc = __String::createWithFormat("%d%s",red->getValue(),CommonFunction::GBKToUTF8("红包").c_str())->getCString();
				iconName = __String::createWithFormat("u_vip_reward_goods%s.png","Red")->getCString();
			}
			else if(redPiece->getValue() > 0)
			{
				strDesc = __String::createWithFormat("%d%s",redPiece->getValue(),CommonFunction::GBKToUTF8("红包碎片").c_str())->getCString();
				iconName = __String::createWithFormat("u_vip_reward_goods%s.png","RedPieces")->getCString();
			}
		}
		break;
	case VipItemWidget::type_shopDiscount:
		{
			__Integer* vipLevel = (__Integer*)mData->objectForKey("vipLevel");
			iconName = __String::createWithFormat("u_vip_reward_discount%d.png",vipLevel->getValue())->getCString();
			title = "VIP商城折扣";
			btnText ="前往商城";

			__Integer* discount = (__Integer*)mData->objectForKey("shopDiscount");
			float f_discount = discount->getValue()/10.0 ;
			if (discount->getValue()%10 == 0)
			{
				strDesc = __String::createWithFormat("%s%.0f%s",CommonFunction::GBKToUTF8("享").c_str(),f_discount,CommonFunction::GBKToUTF8("折优惠").c_str())->getCString();
			}
			else
			{
				strDesc = __String::createWithFormat("%s%.1f%s",CommonFunction::GBKToUTF8("享").c_str(),f_discount,CommonFunction::GBKToUTF8("折优惠").c_str())->getCString();
			}

		}
		break;
	}

	ImageView* icon= ImageView::create();
	icon->loadTexture(iconName,ui::Widget::TextureResType::PLIST);
	icon->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height - 50));
	itembg->addChild(icon);

	Text* textLogin = Text::create();
	textLogin->setText(CommonFunction::GBKToUTF8(title.c_str()));
	textLogin->setTextColor(ccc4(96,34,0,255));
	textLogin->setAnchorPoint(Vec2(0.5,0.5));
	textLogin->setFontSize(25);
	textLogin->setPosition(Vec2(itembg->getContentSize().width/2  ,itembg->getContentSize().height/2 +10 )); 
	itembg->addChild(textLogin);

	Text* textScore = Text::create();
	textScore->setText(strDesc.c_str());
	textScore->setTextColor(ccc4(255,0,0,255));
	textScore->setAnchorPoint(Vec2(0.5,0.5));
	textScore->setFontSize(29);
	textScore->setPosition(Vec2(itembg->getContentSize().width/2,itembg->getContentSize().height/2 - textLogin->getContentSize().height/2-15)); 
	itembg->addChild(textScore);


	Button* btn_get = Button::create();
	btn_get->loadTextureNormal("btn_4.png",ui::Widget::TextureResType::PLIST);
	btn_get->loadTexturePressed("btn_4.png",ui::Widget::TextureResType::PLIST);
	btn_get->setTitleFontSize(25);
	btn_get->setPosition(Vec2(itembg->getContentSize().width/2,btn_get->getContentSize().height/2 + 10));
	itembg->addChild(btn_get);

	if(mType  == ItemType::type_shopDiscount)
	{
		btn_get->setTitleText(CommonFunction::GBKToUTF8(btnText.c_str()));
		btn_get->addTouchEventListener(CC_CALLBACK_2(VipItemWidget::onGotoShop, this));
	}
	else
	{
		__Integer* vipLevel = (__Integer*)mData->objectForKey("vipLevel");
		OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
		if (vipLevel->getValue() != pUser->getVipLevel() )
		{
			btn_get->loadTextureNormal("btn_vip_acitive.png",ui::Widget::TextureResType::LOCAL);
			btn_get->loadTexturePressed("btn_vip_acitive.png",ui::Widget::TextureResType::LOCAL);
			btn_get->addTouchEventListener(CC_CALLBACK_2(VipItemWidget::onShopTips, this));
			btn_get->setPosition(Vec2(itembg->getContentSize().width/2,btn_get->getContentSize().height/2 - 20));
			Text* textActive = Text::create();
			textActive->setText(CommonFunction::GBKToUTF8("激活奖励"));
			textActive->setTextColor(ccc4(255,255,255,255));
			textActive->setAnchorPoint(Vec2(0.5,0.5));
			textActive->setFontSize(25);
			textActive->setPosition(Vec2(btn_get->getContentSize().width/2,btn_get->getContentSize().height/2 +10)); 
			btn_get->addChild(textActive);
		}
		else
		{
			btn_get->setTitleText(CommonFunction::GBKToUTF8(btnText.c_str()));
			//判断是否不可领状态
			if (mType == ItemType::type_login && SessionManager::shareInstance()->getLoginScoreStatus() != 1)
			{
				btn_get->setTouchEnabled(false);
				btn_get->setColor(ccc3(100,100,100));
			}
			else if (mType == ItemType::type_dayAward && SessionManager::shareInstance()->getRedStatus() != 1)
			{
				btn_get->setTouchEnabled(false);
				btn_get->setColor(ccc3(100,100,100));	
			}
			else
			{
				btn_get->addTouchEventListener(CC_CALLBACK_2(VipItemWidget::onGetAward, this));
			}
		}
	}
	
	mBtnGet = btn_get;
}

void VipItemWidget::setBtnValid()
{
	mBtnGet->setTouchEnabled(false);
	mBtnGet->setColor(ccc3(100,100,100));	
}

void VipItemWidget::refreshData(__Dictionary* info)
{
	if (info !=mData)
	{
		mData->release();
		mData = info;
		mData->retain();

		removeAllChildren();

		addItem();
	}

}

void VipItemWidget::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}







//////////////////////////////////////////////////////////////////////////
GameHallVipPowerWidget::GameHallVipPowerWidget()
{
	mCurVipLabel = nullptr;
	mNexVipLable = nullptr;
	mVipDesc   = nullptr;

	mVipPowerInfos = __Array::create();
	mVipPowerInfos->retain();

	mGetType = 0;
}


GameHallVipPowerWidget::~GameHallVipPowerWidget()
{
	CC_SAFE_RELEASE_NULL(mVipPowerInfos);
}

void GameHallVipPowerWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getVipPowerInfo, CC_CALLBACK_1(GameHallVipPowerWidget::refreshData, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(vipAwardResultMsg, CC_CALLBACK_1(GameHallVipPowerWidget::recevieGetAwardResultMsg, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ToSendGetAwardMsg, CC_CALLBACK_1(GameHallVipPowerWidget::recevieToSendGetAwardMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(VipPowerToShopMsg, CC_CALLBACK_1(GameHallVipPowerWidget::recevieGotoShopMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(VipPowerToChargeMsg, CC_CALLBACK_1(GameHallVipPowerWidget::receiveGotoChargeMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(DealVipTipsMsg, CC_CALLBACK_1(GameHallVipPowerWidget::receiveDealTipsMsg, this)), this);
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVipPowerRequest();
	showLoading();
}

void GameHallVipPowerWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}
bool GameHallVipPowerWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}

	return true;
}
std::string GameHallVipPowerWidget::getWidgetJsonName()
{
	return "vipPowerWidget";
}

void GameHallVipPowerWidget::loadUI()
{

	//初始化界面相关数据，及事件绑定
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定相关事件

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallVipPowerWidget::onBack, this));


	mCurVip  = static_cast<ImageView*>(popBg->getChildByTag(ImageCurVipTag));

	mCurFontVip = static_cast<ImageView*>(popBg->getChildByTag(ImageFontVip1Tag));

	mNextFontVip = static_cast<ImageView*>(popBg->getChildByTag(ImageFontVip2Tag));

	mVipDesc = static_cast<Text*>(popBg->getChildByTag(LableVipDesTag));

	ProgressTimer* pProgerss = static_cast<ProgressTimer*>(popBg->getChildByTag(ProgressBarTag));
	pProgerss->setVisible(false);


	Sprite* progress = Sprite::createWithSpriteFrameName("vip_progress_v.png");
	mVipProgress = ProgressTimer::create(progress);
	mVipProgress->setType(ProgressTimer::Type::BAR);
	mVipProgress->setMidpoint(ccp(0, 0.5));    
	mVipProgress->setBarChangeRate(ccp(1, 0));
	mVipProgress->setPosition(pProgerss->getPosition());
	mVipProgress->setPercentage(0);  
	
    popBg->addChild(mVipProgress);

	Button* btn_charge = static_cast<Button*>(popBg->getChildByTag(BtnChargeTag));
	btn_charge->addTouchEventListener(CC_CALLBACK_2(GameHallVipPowerWidget::onCharge, this));


	mTabPanel = static_cast<Layout*>(popBg->getChildByTag(PanelTabTag));

	ImageView* image_tab = nullptr;
	for (int i = 1 ; i <= 7 ; i++)
	{
		image_tab = static_cast<ImageView*>(mTabPanel->getChildByTag(PanelTabTag+i));
		image_tab->addTouchEventListener(CC_CALLBACK_2(GameHallVipPowerWidget::switchVipGrade,this));
	}

	ImageView* awardBg = static_cast<ImageView*>(popBg->getChildByTag(ImageAwardBgTag));

	mItemListView = static_cast<ListView*>(awardBg->getChildByTag(ListViewTag));

	//提示窗口
	mTipsWindow = ImageView::create();
	mTipsWindow->loadTexture("image_viip_tipbg.png",ui::Widget::TextureResType::LOCAL);
	mTipsWindow->setVisible(false);
	popBg->addChild(mTipsWindow,1);

	mTipContent = Text::create();
	String* content = __String::createWithFormat("累积充值%d元宝领福利哦~",10);
	mTipContent->setText(CommonFunction::GBKToUTF8(content->getCString()));
	mTipContent->setTextColor(ccc4(255,0,4,255));
	mTipContent->setFontSize(20);
	mTipContent->setPosition(Vec2(mTipsWindow->getContentSize().width/2+10,mTipsWindow->getContentSize().height/2));
	mTipsWindow->addChild(mTipContent);
}

void GameHallVipPowerWidget::switchVipGrade(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ImageView* image_tab = (ImageView*)pSender;
		if (image_tab->getTag() == mSelectTab->getTag())
		{
			return;
		}
		else
		{
			if (mTipsWindow->isVisible())
			{
				mTipsWindow->stopAllActions();
				mTipsWindow->setVisible(false);
			}
			mSelectTab->loadTexture("btn_vippower.png",ui::Widget::TextureResType::PLIST);
			image_tab->loadTexture("btn_vippower_on.png",ui::Widget::TextureResType::PLIST);
			int index = image_tab->getTag()-PanelTabTag-1;
			__Dictionary* info = (__Dictionary*)mVipPowerInfos->getObjectAtIndex(index);

			Vector<Node*> _children = mItemListView->getChildren();
			int i = 0;
			for( ; i < _children.size(); i++ )
			{
				VipItemWidget* node = (VipItemWidget*)_children.at(i);
				node->refreshData(info);
			}

			mSelectTab = image_tab;
		}
	}
}

void GameHallVipPowerWidget::refreshData(EventCustom* evt)
{
	removeLoading();

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));
	CMD_GP_VipPower* info = (CMD_GP_VipPower*)evt->getUserData();

	SessionManager::shareInstance()->setLoginScoreStatus(info->dwLoginScoreStatus);
	SessionManager::shareInstance()->setRedStatus(info->dwRedPaperStatus);
	if (info->dwVipID == 0 )
	{
		mCurFontVip->loadTexture("font_no_vip.png",ui::Widget::TextureResType::PLIST);
		LabelAtlas* nextVipLable = LabelAtlas::create("0","number_vip.png",14,19,'0');
		nextVipLable->setPosition(Vec2(mNextFontVip->getPositionX()+ mNextFontVip->getContentSize().width/2 + 5,mNextFontVip->getPositionY()));
		nextVipLable->setAnchorPoint(Vec2(0.5,0.5));
		nextVipLable->setString(__String::createWithFormat("%d",1)->getCString());
		popBg->addChild(nextVipLable);
	}
	else
	{
		if(!mCurFontVip->isVisible())
		{
			mCurFontVip->loadTexture("font_vip_title.png",ui::Widget::TextureResType::PLIST);
			mCurFontVip->setVisible(true);
		}
		mCurVip->loadTexture("icon_vip_on.png",ui::Widget::TextureResType::PLIST);
		LabelAtlas* curVipLable = LabelAtlas::create("0","number_vip.png",14,19,'0');
		curVipLable->setPosition(Vec2(mCurFontVip->getPositionX() + mCurFontVip->getContentSize().width/2 + 10,mCurFontVip->getPositionY()));
		curVipLable->setAnchorPoint(Vec2(0.5,0.5));
		curVipLable->setString(__String::createWithFormat("%d",info->dwVipID)->getCString());
		popBg->addChild(curVipLable);


		LabelAtlas* nextVipLable = LabelAtlas::create("0","number_vip.png",14,19,'0');
		nextVipLable->setPosition(Vec2(mNextFontVip->getPositionX()+ mNextFontVip->getContentSize().width/2 + 5,mNextFontVip->getPositionY()));
		nextVipLable->setAnchorPoint(Vec2(0.5,0.5));
		nextVipLable->setString(__String::createWithFormat("%d",info->dwVipID+1)->getCString());
		popBg->addChild(nextVipLable);
	}

	if (info->dwAllIngot == 0 && info->dwIngot == 0)
	{
		mVipProgress->setPercentage(10);
	}
	else
	{
		float persent =(info->dwAllIngot -  info->dwIngot)*100/info->dwAllIngot ;
		mVipProgress->setPercentage(persent);
	}

	std::string desc;
	std::string split_str1  = CommonFunction::GBKToUTF8("还需冲值");
	std::string split_str2 = CommonFunction::GBKToUTF8("元宝即可成为");
	std::string split_str3 = CommonFunction::GBKToUTF8("福利翻倍哦。");
	desc = __String::createWithFormat("%s%d%sVIP%d,%s",split_str1.c_str(),info->dwIngot,split_str2.c_str(),info->dwVipID+1,split_str3.c_str())->getCString();
	mVipDesc->setFontSize(20);
	mVipDesc->setText(desc.c_str());
	mVipDesc->setTextColor(ccc4(96,34,0,255));


	CMD_GP_VipPowerInfo* vipPowerInfo = info->VipPowerInfo;
	for (int i = 0 ; i < 7 ; i ++)
	{
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__String::createWithFormat("%lld",vipPowerInfo->lLoginScore),"loginScore");
		dic->setObject(__Integer::create(vipPowerInfo->dwVipID),"vipLevel");
		dic->setObject(__Integer::create(vipPowerInfo->dwRedPaper),"red");
		dic->setObject(__Integer::create(vipPowerInfo->dwRedPieces),"redPiece");
		dic->setObject(__Integer::create(vipPowerInfo->dwShopping),"shopDiscount");
		mVipPowerInfos->addObject(dic);
		vipPowerInfo++;
	}

	__Dictionary* curVipInfo = nullptr;
	if (info->dwVipID <=0)
	{
		curVipInfo = (__Dictionary*)mVipPowerInfos->getObjectAtIndex(0);
	}
	else
	{
		curVipInfo = (__Dictionary*)mVipPowerInfos->getObjectAtIndex(info->dwVipID - 1);
	}

	VipItemWidget* pItem = nullptr;
	//登录返还项
	pItem = VipItemWidget::create(185,curVipInfo,VipItemWidget::type_login);
	mItemListView->pushBackCustomItem(pItem);
	//每日赠送
	pItem = VipItemWidget::create(185,curVipInfo,VipItemWidget::type_dayAward);
	mItemListView->pushBackCustomItem(pItem);
	//商品折扣
	pItem = VipItemWidget::create(185,curVipInfo,VipItemWidget::type_shopDiscount);
	mItemListView->pushBackCustomItem(pItem);


	Vector<Node*> _children = mTabPanel->getChildren();
	int i = 0;
	for( ; i < _children.size(); i++ )
	{
		ImageView* node = (ImageView*)_children.at(i);
		if (info->dwVipID == 0 && i == 0)
		{
			mSelectTab = node;
			node->loadTexture("btn_vippower_on.png",ui::Widget::TextureResType::PLIST);
		}
		else if(info->dwVipID - 1 == i)
		{
			mSelectTab = node;
			node->loadTexture("btn_vippower_on.png",ui::Widget::TextureResType::PLIST);
		}
	}
}


void GameHallVipPowerWidget::onGetGold(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVipAwardRequest(AwardTypeGold);
		showLoading();
	}

}


void GameHallVipPowerWidget::onGetRed(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVipAwardRequest(AwardTypeRed);
		showLoading();
	}

}

void GameHallVipPowerWidget::onGotoShop(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallShopWidget::create());
	}

}

void GameHallVipPowerWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}

}
void GameHallVipPowerWidget::onCharge(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		//跳转充值元宝界面
		ModalViewManager::sharedInstance()->showWidget(GameHallChargeWidget::create(tab_ingot));
	}
}



void GameHallVipPowerWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallVipPowerWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}


void GameHallVipPowerWidget::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}

void GameHallVipPowerWidget::recevieGetAwardResultMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_VipAwardLog* logInfo = (CMD_GP_VipAwardLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(logInfo->szDescribeString)));

	if (logInfo->dwRet == 0)
	{
		Vector<Node*> _children = mItemListView->getChildren();
		int i = 0;
		for( ; i < _children.size(); i++ )
		{
			VipItemWidget* node = (VipItemWidget*)_children.at(i);
			if (node->getItemType() == mGetType)
			{
				node->setBtnValid();
				mGetType = 0;
				if (mGetType == VipItemWidget::ItemType::type_login)
				{
					SessionManager::shareInstance()->setLoginScoreStatus(2);
				}else if (mGetType == VipItemWidget::ItemType::type_dayAward)
				{
					SessionManager::shareInstance()->setRedStatus(2);
				}
				break;
			}
		}	
	}
}


void GameHallVipPowerWidget::recevieToSendGetAwardMsg(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	int type = ((__Integer*)info->objectForKey("type"))->getValue();
	if (type == VipItemWidget::ItemType::type_login)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVipAwardRequest(AwardTypeGold);
		mGetType = type;
		showLoading();
	}
	else if(type == VipItemWidget::ItemType::type_dayAward)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVipAwardRequest(AwardTypeRed);
		mGetType = type;
		showLoading();
	}	
}

void GameHallVipPowerWidget::recevieGotoShopMsg(EventCustom* evt)
{
	removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(GameHallShopWidget::create());
}

void GameHallVipPowerWidget::receiveGotoChargeMsg(EventCustom* evt)
{
	removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(GameHallChargeWidget::create(tabType::tab_ingot));
}

void GameHallVipPowerWidget::receiveDealTipsMsg(EventCustom* evt)
{
	mTipsWindow->stopAllActions();
	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	int type = ((__Integer*)dic->objectForKey("type"))->getValue();
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));
	Vec2 pos;
	if (type == 0)//login
	{
		pos.x = 350;
		pos.y = 150;
	}
	else if(type == 1)
	{
		pos.x = 530;
		pos.y = 150;
	}
	int vipLevel = ((__Integer*)dic->objectForKey("vipLevel"))->getValue();
	String* desc = __String::createWithFormat("%s%d%s~",CommonFunction::GBKToUTF8("累积充值").c_str(),KVipTotalIngot[vipLevel-1],CommonFunction::GBKToUTF8("元宝领福利哦").c_str());
	mTipContent->setString(desc->getCString());
	mTipsWindow->setPosition(pos);
	mTipsWindow->setVisible(true);
	mTipsWindow->setScale(0);


	Vector<FiniteTimeAction*> actions;
	ScaleTo* sc = ScaleTo::create(0.3f,1);
	actions.pushBack(sc);
	DelayTime* dt = DelayTime::create(2.5f);
	actions.pushBack(dt);
	CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(GameHallVipPowerWidget::hideTips));
	actions.pushBack(pCall);

	mTipsWindow->runAction(Sequence::create(actions));

}

void GameHallVipPowerWidget::hideTips(Node* pSender)
{
	mTipsWindow->setVisible(false);
}