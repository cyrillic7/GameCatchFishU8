#include "GameHallAssignWidget.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "GameHallAssignItem.h"
#include "Model/LoginUserModel.h"
#include "SessionManager.h"

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  ImageAssginPathTag 1898
#define  BtnAssignTag 55

#define  ImageBag1Tag 1904
#define  ImageBag2Tag 1909
#define  ImageBag3Tag 1919
#define  ImageBag4Tag 1927

#define  LabelTip1Tag  1930
#define  ImageAwardBg1 1936 
#define  LabelAward1_1 1938
#define  LabelAward1_2 1939
#define  ImageAwardBg2 1948
#define  LabelAward2_1 1950
#define  LabelAward2_2 1951
#define  ImageAwardBg3 1960
#define  LabelAward3_1 1962
#define  LabelAward3_2 1963
#define  ImageAwardBg4 1972
#define  LabelAward4_1 1974

#define ImagePathBgTag 5416

#define  EffectNodeTag 6018

GameHallAssginWidget::GameHallAssginWidget()
{
	mAssignDay = 0;
	mbCanAssign = true;
	mAssignPath = nullptr;
	mPathBag  =nullptr;
	memset(&mSignInfo,0,sizeof(CMD_GP_GetSignInTaskInfo));
	mItemArray = __Array::create();
	mItemArray->retain();
}

GameHallAssginWidget::~GameHallAssginWidget()
{
	CC_SAFE_RELEASE_NULL(mItemArray);
}

std::string GameHallAssginWidget::getWidgetJsonName()
{
	return "assignWidget";
}

void GameHallAssginWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallAssginWidget::onEnter()
{
	TNWidget::onEnter();

	//绑定签到返消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getAssignListMsg, CC_CALLBACK_1(GameHallAssginWidget::refreshData, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(userAssignMsg, CC_CALLBACK_1(GameHallAssginWidget::receiveAssinRsp, this)), this);
	//发送获取签到
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendGetAssignList();
	showLoading();
}

bool GameHallAssginWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}



	return true;
}
void GameHallAssginWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关事件

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	//Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	//btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallAssginWidget::onBack, this));

	Button* btn_assign = static_cast<Button*>(popBg->getChildByTag(BtnAssignTag));
	btn_assign->addTouchEventListener(CC_CALLBACK_2(GameHallAssginWidget::onAssign, this));

	mAssignPath = static_cast<ImageView*>(popBg->getChildByTag(ImageAssginPathTag));

	mPathBag = static_cast<ImageView*>(popBg->getChildByTag(ImagePathBgTag));
}

//获取签到列表
void GameHallAssginWidget::refreshData(EventCustom* evt)
{
	removeLoading();
	CMD_GP_GetSignInTaskInfo*  signInfo = (CMD_GP_GetSignInTaskInfo*)evt->getUserData();
	memcpy(&mSignInfo,signInfo,sizeof(CMD_GP_GetSignInTaskInfo));
	mAssignDay = signInfo->dwDay;
	for (int i = 0 ; i < 31; i++)
	{
		int tag = ImageAssginPathTag+i+1;
		ImageView* imageSign = nullptr;
		if (!isAwardBag(tag))
		{
			//已签到天数
			if(i < signInfo->dwDay)
			{
				imageSign = (ImageView*)mAssignPath->getChildByTag(tag);
				imageSign->loadTexture("has_assign.png",ui::Widget::TextureResType::PLIST);
			}
			
		}
		else
		{

			imageSign = (ImageView*)mAssignPath->getChildByTag(tag);
			Vec2 pos = imageSign->getPosition();
			imageSign->removeFromParent();
			
			ImageView* bag = ImageView::create();
			bag->loadTexture(getImageNameByTag(tag),ui::Widget::TextureResType::PLIST);
			bag->setPosition(Vec2(pos.x - 10,pos.y - 15));
			mPathBag->addChild(bag,0,tag - ImageAssginPathTag);
			
		}
	}

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));
	Text*      tipText = static_cast<Text*>(popBg->getChildByTag(LabelTip1Tag));
	CMD_GP_SignInTask signIn = signInfo->SignInTask[0];
	__String* tip = __String::createWithFormat("%s%lld%s",CommonFunction::GBKToUTF8("签到即可领取").c_str(),signIn.lScore,CommonFunction::GBKToUTF8("金币").c_str());
	tipText->setString(tip->getCString());


	GameHallAssignItem* pItem = GameHallAssignItem::create(signInfo->SignInTask[1]);
	pItem->setPosition(Vec2(popBg->getContentSize().width/2 + 180,popBg->getContentSize().height/2 + 150));
	pItem->setScale(0.75f);
	popBg->addChild(pItem);

	if (mAssignDay >= pItem->getAssignDay())
	{
		pItem->setSelect(true);
	}
	mItemArray->addObject(pItem);

	pItem = GameHallAssignItem::create(signInfo->SignInTask[2]);
	pItem->setPosition(Vec2(popBg->getContentSize().width/2 + 320,popBg->getContentSize().height/2 + 80));
	pItem->setScale(0.8f);
	popBg->addChild(pItem);

	if (mAssignDay >= pItem->getAssignDay())
	{
		pItem->setSelect(true);
	}
	mItemArray->addObject(pItem);

	pItem = GameHallAssignItem::create(signInfo->SignInTask[3]);
	pItem->setPosition(Vec2(popBg->getContentSize().width/2 + 180,popBg->getContentSize().height/2 - 40));
	pItem->setScale(0.8f);
	popBg->addChild(pItem);

	if (mAssignDay >= pItem->getAssignDay())
	{
		pItem->setSelect(true);
	}
	mItemArray->addObject(pItem);

	pItem = GameHallAssignItem::create(signInfo->SignInTask[4]);
	pItem->setPosition(Vec2(popBg->getContentSize().width/2 + 320 ,popBg->getContentSize().height/2 - 100));
	pItem->setScale(0.9f);
	popBg->addChild(pItem);

	if (mAssignDay >= pItem->getAssignDay())
	{
		pItem->setSelect(true);
	}
	mItemArray->addObject(pItem);
}
bool GameHallAssginWidget::isAwardBag(int tag)
{
	int day = tag - ImageAssginPathTag;
	if (day == mSignInfo.SignInTask[1].dwDay || day == mSignInfo.SignInTask[2].dwDay || day == mSignInfo.SignInTask[3].dwDay || day == mSignInfo.SignInTask[4].dwDay )
		return true;
	else
		return false;
}

std::string GameHallAssginWidget::getImageNameByTag(int tag)
{
	std::string imageName = "";
	int day = tag - ImageAssginPathTag;
	if(day ==  mSignInfo.SignInTask[1].dwDay)
	{
		if(mAssignDay >= day)
			imageName = "bag1_on.png";
		else
			imageName = "bag1.png";
	}else if(day ==  mSignInfo.SignInTask[2].dwDay)
	{
		if(mAssignDay >= day)
			imageName = "bag2_on.png";
		else
			imageName = "bag2.png";
	}
	else if(day ==  mSignInfo.SignInTask[3].dwDay)
	{
		if(mAssignDay >= day)
			imageName = "bag3_on.png";
		else
			imageName = "bag3.png";
	}
	else if(day ==  mSignInfo.SignInTask[4].dwDay)
	{
		if(mAssignDay >= day)
			imageName = "bag4_on.png";
		else
			imageName = "bag4.png";
	}
	return imageName;
}

//签到返回
void GameHallAssginWidget::receiveAssinRsp(EventCustom* evt)
{
	removeLoading();
	CMD_GP_SignInTaskLog* signLog = (CMD_GP_SignInTaskLog*)evt->getUserData();
	if (signLog->dwRet == 0 )//签到成功
	{
		mAssignDay = signLog->dwDay;
		int tag = ImageAssginPathTag + mAssignDay;
		ImageView* imageSign = nullptr;
		if (!isAwardBag(tag))
		{
			imageSign = (ImageView*)mAssignPath->getChildByTag(tag);
			imageSign->loadTexture("has_assign.png",ui::Widget::TextureResType::PLIST);
		}
		else
		{
			imageSign = (ImageView*)mPathBag->getChildByTag(mAssignDay);
			imageSign->loadTexture(getImageNameByTag(tag),ui::Widget::TextureResType::PLIST);
			for (int i = 0 ; i < mItemArray->count();i++)
			{
				GameHallAssignItem* pItem = (GameHallAssignItem*)mItemArray->getObjectAtIndex(i);
				if (pItem->getAssignDay() == mAssignDay)
				{
					pItem->setSelect(true);
				}
			}
		}

		dealScoreEffect(signLog->lAllScore + signLog->lScore);
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8(signLog->szDescribeString)));
	}

	//换手机登录已签到帐号也得处理
	//LoginUserModel* model = SessionManager::shareInstance()->getLoginModel();
	//model->setAssignDate(SessionManager::shareInstance()->getDateTime());
	//model->flush();
	SessionManager::shareInstance()->saveAssignInfo();
}


void GameHallAssginWidget::dealScoreEffect(SCORE value)
{
	mbCanAssign = false;
	Node* effectNode = Node::create();
	effectNode->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
	addChild(effectNode,0,EffectNodeTag);

	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
	effectNode->addChild(shade);

	Sprite* effect = Sprite::create();
	
	effectNode->addChild(effect);
	std::vector<std::string> imageNames;
	std::string path = "";
	for (int i = 0 ; i < 6;i++)
	{

		path = "award_effect_"+numberToString(i+1)+".png";
		imageNames.push_back(path);
	}

	Animate* pAnima = CommonFunction::createFrameAnimate(imageNames,0.14);
	CallFuncN* callBack = CallFuncN::create(this,callfuncN_selector(GameHallAssginWidget::removeScoreEffect));
	effect->runAction(Sequence::createWithTwoActions(pAnima,callBack));

	ImageView* image_plus = ImageView::create();
	image_plus->loadTexture("image_plus.png",ui::Widget::TextureResType::PLIST);
	image_plus->setPosition(Vec2((-image_plus->getContentSize().width)/2 -30,effect->getContentSize().height/2 +image_plus->getContentSize().height/2));
	effectNode->addChild(image_plus);

	LabelAtlas* awardLabel = LabelAtlas::create("1:","award_effect_number.png",36,35,'0');
	awardLabel->setString(__String::createWithFormat("%lld",value)->getCString());
	awardLabel->setPosition(Vec2((awardLabel->getContentSize().width)/2 -30,effect->getContentSize().height/2 +image_plus->getContentSize().height/2));
	awardLabel->setAnchorPoint(Vec2(0.5,0.5));
	effectNode->addChild(awardLabel);


}
void GameHallAssginWidget::removeScoreEffect(Node* node)
{
	if (getChildByTag(EffectNodeTag))
	{
		FadeOut* fo = FadeOut::create(1.0f);
		//RemoveSelf* rs   = RemoveSelf::create();
		CallFuncN* call = CallFuncN::create(this, callfuncN_selector(GameHallAssginWidget::returnBack));
		getChildByTag(EffectNodeTag)->runAction(Sequence::createWithTwoActions(fo, call));
	}
}

void GameHallAssginWidget::returnBack(Node* pSender)
{
	removeFromParent();
}

void GameHallAssginWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallAssginWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallAssginWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallAssginWidget::onAssign(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (mbCanAssign)
		{
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendAssignMsg();
			showLoading();
		}
		
	}
}

void GameHallAssginWidget::okCallback()
{
	removeFromParent();
}