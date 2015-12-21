#include "GameHallLuckSpinWidget.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_GameServer.h"
#include "Common/ModalViewManager.h"


#define  popBgTag  14813
#define  BtnCloseTag 14814
#define  imageSpinTag 14815
#define  BtnSpinTag 14816

GameHallLuckSpinWidget::GameHallLuckSpinWidget()
: eDialState(DIAL_FREE)
, dialIndex(0)
{
	mBtnGo = nullptr;
	mSpinArrow = nullptr;
	//加载动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("AnimationDial.ExportJson");
}

GameHallLuckSpinWidget::~GameHallLuckSpinWidget()
{

}

bool GameHallLuckSpinWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}
	return true;
}


void GameHallLuckSpinWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(luckSpinRspMsg, CC_CALLBACK_1(GameHallLuckSpinWidget::receiveSpinRspMsg, this)), this);
}
void GameHallLuckSpinWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallLuckSpinWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0, 0, 0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0, 0));
	addChild(shade);

	TNWidget::loadUI();
	//添加相关事件
	bg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));

	Button* btnClose = static_cast<Button*>(bg->getChildByTag(BtnCloseTag));
	btnClose->addTouchEventListener(CC_CALLBACK_2(GameHallLuckSpinWidget::onClose, this));

	mBtnGo = static_cast<Button*>(bg->getChildByTag(BtnSpinTag));
	mBtnGo->addTouchEventListener(CC_CALLBACK_2(GameHallLuckSpinWidget::onClickSpin, this));

	mSpinArrow = static_cast<ImageView*>(bg->getChildByTag(imageSpinTag));
	pAEnd = Armature::create("AnimationDial");
	pAEnd->setPosition(Vec2(mSpinArrow->getContentSize().width / 2,0));
	mSpinArrow->addChild(pAEnd,999);
	pAEnd->getAnimation()->play("AnimationEnd");
	pAEnd->setVisible(false);


	CCArmature *pABg = CCArmature::create("AnimationDial");
	pABg->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
	bg->addChild(pABg);
	pABg->getAnimation()->play("AnimationBg");

	pAStartButton = CCArmature::create("AnimationDial");
	pAStartButton->setPosition(Vec2(mBtnGo->getContentSize().width / 2, mBtnGo->getContentSize().height / 2));
	mBtnGo->addChild(pAStartButton);
	pAStartButton->getAnimation()->play("AnimationButton");

}

std::string GameHallLuckSpinWidget::getWidgetJsonName()
{
	return "luckSpinWidget";
}

void GameHallLuckSpinWidget::onClickSpin(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		switch (eDialState)
		{
		case GameHallLuckSpinWidget::DIAL_FREE:
		{
			pAEnd->setVisible(false);
			setDialState(DIAL_ING);
		    //发送转盘请求
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendLuckSpinRequest();
		}
		break;
		}
	}
}

void GameHallLuckSpinWidget::onClose(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

//转盘返回处理
void GameHallLuckSpinWidget::receiveSpinRspMsg(EventCustom* evt)
{
	CMD_GP_DialLog *signInfo = (CMD_GP_DialLog*)(evt->getUserData());
	if (signInfo->dwRet == 0)
	{
		dialIndex = signInfo->dwIndex;
		float endAnlg = 360 * 15 + 360.0 / 12.0 * dialIndex;
		EaseExponentialIn  *in = EaseExponentialIn::create(CCRotateTo::create(8.0, 360 * 15));
		EaseExponentialOut  *out = EaseExponentialOut::create(CCRotateTo::create(8.0, endAnlg));
		CallFuncN *call = CallFuncN::create(this, callfuncN_selector(GameHallLuckSpinWidget::spinAnimationFinsh));
		Sequence *seq =Sequence::create(in, out, call, NULL);
		mSpinArrow->runAction(seq);
	}
	else if (signInfo->dwRet == 2)
	{
		setDialState(DIAL_FREE);
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this, "", CommonFunction::GBKToUTF8(signInfo->szDescribeString), true));
	}
	else{
		setDialState(DIAL_FREE);
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", CommonFunction::GBKToUTF8(signInfo->szDescribeString)));
	}
}
//动画完成
void GameHallLuckSpinWidget::spinAnimationFinsh(Node* pSender){
	mBtnGo->loadTextureNormal("vipgo.png", UI_TEX_TYPE_PLIST);
	mBtnGo->loadTexturePressed("vipgo_press.png", UI_TEX_TYPE_PLIST);
	pAStartButton->getAnimation()->play("AnimationButton1");
	pAEnd->setVisible(true);

	scheduleOnce(schedule_selector(GameHallLuckSpinWidget::showReward), 0.5);
}

void GameHallLuckSpinWidget::okCallback()
{
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(showVipPowerMsg);
	removeFromParent();
}
//显示奖励
void GameHallLuckSpinWidget::showReward(float dt)
{
	Armature *pABg = Armature::create("AnimationDial");
	pABg->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
	bg->addChild(pABg, 100);
	pABg->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(GameHallLuckSpinWidget::onAnimationEventOver));//动画播完回调用
	pABg->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(GameHallLuckSpinWidget::onAnimationEventFrame));
	pABg->getAnimation()->play(String::createWithFormat("AnimationGetProp%d", dialIndex)->getCString());
}

void GameHallLuckSpinWidget::onAnimationEventOver(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case MovementEventType::COMPLETE:
	case MovementEventType::LOOP_COMPLETE:
	{
		for (int i = 0; i < 12; i++)
		{
			String *aName = String::createWithFormat("AnimationGetProp%d", i);
			if (strcmp(movementID.c_str(), aName->getCString()) == 0)
			{
				armature->removeFromParentAndCleanup(true);
				setDialState(DIAL_FREE);
				return;
			}
		}
	}
		break;
	default:
		break;
	}
}

void GameHallLuckSpinWidget::onAnimationEventFrame(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{

}