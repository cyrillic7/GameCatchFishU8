#include "GameHallLoudspeakerWidget.h"
#include "Message.h"
#include "Common/ModalViewManager.h"
#include "GameServiceClientManager.h"


#define  popBgTag			14315
#define  panelHornTag		14316
#define  imageInputBgTag	14317
#define  textInputTag		14318
#define  btnSendTag			14319

GameHallLoudSpeakerWidget::GameHallLoudSpeakerWidget()
{
	mHistoryPanel = nullptr;
	bg = nullptr;
}

GameHallLoudSpeakerWidget::~GameHallLoudSpeakerWidget()
{

}


bool GameHallLoudSpeakerWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}


	return true;
}

void GameHallLoudSpeakerWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateLastHornMsg, CC_CALLBACK_1(GameHallLoudSpeakerWidget::receiveUpdateHornMsg, this)), this);
	setScrollViewData();
}

void GameHallLoudSpeakerWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

std::string GameHallLoudSpeakerWidget::getWidgetJsonName()
{
	return "loudSpeakerWidget";
}

void GameHallLoudSpeakerWidget::loadUI()
{
	/*LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0, 0, 0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0, 0));
	addChild(shade);*/

	TNWidget::loadUI();
	m_mainWidget->setTouchEnabled(true);
	m_mainWidget->addTouchEventListener(CC_CALLBACK_2(GameHallLoudSpeakerWidget::onPanelTouch, this));
	//绑定相关事件
	bg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));

	mHistoryPanel = static_cast<ui::ScrollView*>(bg->getChildByTag(panelHornTag));

	ImageView* inputBg = static_cast<ImageView*>(bg->getChildByTag(imageInputBgTag));

	TextField* inputText = static_cast<TextField*>(inputBg->getChildByTag(textInputTag));
	inputText->setText("");

	mContentEdit = ui::EditBox::create(inputText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mContentEdit->setPosition(Vec2(inputText->getContentSize().width/2,inputText->getContentSize().height/2));
	mContentEdit->setFontSize(inputText->getFontSize());
	mContentEdit->setFontColor(inputText->getColor());//设置文字颜色
	mContentEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	inputText->addChild(mContentEdit,10);

	Button* btnSend = static_cast<Button*>(bg->getChildByTag(btnSendTag));
	btnSend->addTouchEventListener(CC_CALLBACK_2(GameHallLoudSpeakerWidget::onClickSend, this));

	//延时0.2秒弹出消息框
	CallFuncN *call = CallFuncN::create(this, callfuncN_selector(GameHallLoudSpeakerWidget::showHornContent));
	Sequence *seq = Sequence::create(DelayTime::create(0.2F), call, NULL);
	bg->runAction(seq);
}

void GameHallLoudSpeakerWidget::okCallback()
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	c->sendhornRequset(mContentEdit->getText());
	hideHornContent();
}

///没有点在消息区域 隐藏消息框架
void GameHallLoudSpeakerWidget::onPanelTouch(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		hideHornContent();
	}
}

void GameHallLoudSpeakerWidget::hideHornContent()
{
	EaseExponentialIn  *out = EaseExponentialIn::create(MoveTo::create(0.2, ccp(kScreenWidth / 2, kScreenHeight + bg->getContentSize().height / 2)));
	CallFuncN *call = CallFuncN::create(this, callfuncN_selector(GameHallLoudSpeakerWidget::removSelf));
	Sequence *seq = Sequence::createWithTwoActions(out, call);
	bg->runAction(seq);
}
//
void GameHallLoudSpeakerWidget::showHornContent(Node* pSender)
{
	bg->stopAllActions();
	EaseExponentialOut  *out = EaseExponentialOut::create(MoveTo::create(0.2, Vec2(kScreenWidth / 2, kScreenHeight - 200 - bg->getContentSize().height / 2)));
	Sequence *seq = Sequence::create(out,NULL);
	bg->runAction(seq);
}

void GameHallLoudSpeakerWidget::removSelf(Node* pSender)
{
	removeFromParent();
	//通知主界面显示喇叭
	//Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(showHornNoticeMsg);
}

void GameHallLoudSpeakerWidget::onClickSend(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true, pSender);
	}
	else
	{
		playScaleAnimation(false, pSender);
	}

	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		if (strlen(mContentEdit->getText()) == 0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(NULL, "", CommonFunction::GBKToUTF8("内容不能为空")));
		}
		else
		{
			OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
			//喇叭数大于0
			if (pUser->getLoudSpeakerNum() <= 0)
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this, "", CommonFunction::GBKToUTF8("您可发送的喇叭数为0，继续发送将会从保险柜中扣除100万币，是否继续发送？"), true));
			}
			else
			{
				GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
				c->sendhornRequset(mContentEdit->getText());
				hideHornContent();
			}
		}
	}
}

void GameHallLoudSpeakerWidget::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 1.1f : 1.2f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}


void GameHallLoudSpeakerWidget::receiveUpdateHornMsg(EventCustom* evt)
{
	setScrollViewData();
}

void GameHallLoudSpeakerWidget::editBoxEditingDidBegin(ui::EditBox* editBox)
{
	
}

void GameHallLoudSpeakerWidget::editBoxEditingDidEnd(ui::EditBox* editBox)
{
	
}

void GameHallLoudSpeakerWidget::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
}

void GameHallLoudSpeakerWidget::editBoxReturn(ui::EditBox* editBox)
{

}

//设置滚动容器数据
void GameHallLoudSpeakerWidget::setScrollViewData()
{
	mHistoryPanel->removeAllChildren();
	int height = callUpHight();

	std::list <std::string> ::reverse_iterator iter;
	for (iter = SessionManager::shareInstance()->listHornMsg.rbegin(); iter != SessionManager::shareInstance()->listHornMsg.rend(); iter++)
	{
		Node* pNode = Node::create();
		pNode->setAnchorPoint(Vec2(0,0));
		Label *pLContent = createMsgContent(iter->c_str());
		height += pLContent->getContentSize().height;
		pLContent->setAnchorPoint(Vec2(0, 1));
		pLContent->setPosition(Vec2(0, height));
		pNode->addChild(pLContent);

		mHistoryPanel->setInnerContainerSize(Size(mHistoryPanel->getContentSize().width, height));

		ImageView *pIVHornLine = ImageView::create();
		pIVHornLine->loadTexture("u_line-between.png");
		height += pIVHornLine->getContentSize().height ;
		pIVHornLine->setPosition(Vec2(mHistoryPanel->getContentSize().width / 2, pLContent->getPositionY() - pLContent->getContentSize().height - 2.5));
		pIVHornLine->setScaleX(mHistoryPanel->getContentSize().width);
		pNode->addChild(pIVHornLine);
		mHistoryPanel->addChild(pNode);
		
	}
	mHistoryPanel->scrollToBottom(0.2, false);
}

int GameHallLoudSpeakerWidget::callUpHight()
{
	int height = 0;
	std::list <std::string> ::iterator iter;
	for (iter = SessionManager::shareInstance()->listHornMsg.begin(); iter != SessionManager::shareInstance()->listHornMsg.end(); iter++)
	{
		cocos2d::Label *pLMsgContent = cocos2d::Label::create();
		//Label *pLMsgContent = Label::createWithSystemFont(iter->c_str(), "Arail", 25, Size(mHistoryPanel->getContentSize().width, 0));
		int width = mHistoryPanel->getContentSize().width;
		pLMsgContent->setSystemFontSize(25);
		pLMsgContent->setDimensions(width, 0);
		pLMsgContent->setString(iter->c_str());
		height += pLMsgContent->getContentSize().height;
	}
	if (height >= mHistoryPanel->getContentSize().height)
	{
		height = 0;
	}
	else
	{
		height = mHistoryPanel->getContentSize().height - height;
	}
	return height;
}
//创建消息内容
cocos2d::Label *GameHallLoudSpeakerWidget::createMsgContent(std::string content){
	cocos2d::Label *pLMsgContent = cocos2d::Label::create();
	//Label *pLMsgContent = Label::createWithSystemFont(content.c_str(), "Arial", 25, Size(mHistoryPanel->getContentSize().width, 0));
	pLMsgContent->setSystemFontSize(25);
	int width = mHistoryPanel->getContentSize().width;
	pLMsgContent->setDimensions(width, 0);
	pLMsgContent->setColor(ccc3(230,254,255));
	pLMsgContent->setString(content.c_str());
	return pLMsgContent;
}

Text* GameHallLoudSpeakerWidget::createMsgText(std::string content)
{
	Text* pText = Text::create();
	pText->setFontSize(30);
	pText->setText(content.c_str());
	pText->setTextColor(ccc4(230, 254, 255, 255));
	pText->ignoreContentAdaptWithSize(true);
	pText->setTextAreaSize(Size(mHistoryPanel->getContentSize().width, 0));

	return pText;
}