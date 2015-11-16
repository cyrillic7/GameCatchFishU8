#include "GameHallBankWidget.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "SessionManager.h"

#define  ImagePopBgTag 18
#define  BankBtnCloseTag 42
#define  BankCheckBoxGetOutTag 124
#define  BankCheckBoxInTag   126
#define  PanelGetOutTag		 128
#define  PanelInTag          242

#define  BtnGetOutOrInTag    55
#define  LabelGetOutOrInTag  56

#define  ImageInputBgTag    140
#define  InputTextGetOutTag 293

#define  ImageInputInBgTag    246
#define  InputTextInTag 294

#define  ImageAllOutTag 207
#define  Image1bwOutTag 209
#define  Image5bwOutTag 211
#define  Image1kwOutTag 213
#define  Image5kwOutTag 217
#define  Image1yOutTag  221

#define  LableAllOutTag 208
#define  Lable1bwOutTag 210
#define  Lable5bwOutTag 212
#define  Lable1kwOutTag 214
#define  Lable5kwOutTag 218
#define  Lable1yOutTag  222

#define  ImageAllInTag 249
#define  Image1bwInTag 251
#define  Image5bwInTag 253
#define  Image1kwInTag 255
#define  Image5kwInTag 257
#define  Image1yInTag  259

#define  LableAllInTag 250
#define  Lable1bwInTag 252
#define  Lable5bwInTag 254
#define  Lable1kwInTag 256
#define  Lable5kwInTag 258
#define  Lable1yInTag  260


#define  TextTakeOutTag 132
#define  TextSaveInTag  244

#define  ImageTakeOutBitBg 141
#define  ImageTakeOutBitBg2 143

#define  ImageSaveInBitBg 247
#define  ImageSaveInBitBg2 248

#define  imageTabTag 5942
#define  imageTabItemTag 5943
#define  imageTabItem2Tag 6209
#define  imageTabCommonTag 6210

#define  bitWidth 90

GameHallBankWidget::GameHallBankWidget()
{
	mPutInPanel = nullptr;
	mGetOutPanel = nullptr;
	m_isInGetOut = true;
	m_editFlag = -1;
	mTakeScoreText = nullptr;
	mSaveScoreText = nullptr;
	mTakeSelectIndex = 0;
	mSaveSelectIndex = 0;
	mUserInsure = "";
	mUserScore  = "";
	mTabTake = nullptr;
	mTabSave = nullptr;
	mHasMov = false;
	mCanSaveIndex = 0;
	mCanTakeIndex = 0;
}

GameHallBankWidget::~GameHallBankWidget()
{

}

GameHallBankWidget* GameHallBankWidget::create(bool isShowBackBtn)
{
	GameHallBankWidget* ret = new GameHallBankWidget();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = NULL;
		return ret;
	}
}

void GameHallBankWidget::onExit()
{
	TNWidget::onExit();
}

void GameHallBankWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(queryInsureInfoMsg, CC_CALLBACK_1(GameHallBankWidget::refreshInsureData, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(insureSuccessMsg, CC_CALLBACK_1(GameHallBankWidget::receiveInsureSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(insureFailureMsg, CC_CALLBACK_1(GameHallBankWidget::receiveInsureFailureMsg, this)), this);
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendQueryBankRequest();
	showLoading();
}

bool GameHallBankWidget::init()
{
	if ( false == TNWidget::init())
	{
		return false;
	}
	return true;
}

std::string GameHallBankWidget::getWidgetJsonName()
{
	return "myBank";
}

void GameHallBankWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关事件
    popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBgTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BankBtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::onClose, this));

	//
	ImageView* tab = static_cast<ImageView*>(popBg->getChildByTag(imageTabTag));
	

	mTabTake = static_cast<ImageView*>(popBg->getChildByTag(imageTabItemTag));
	mTabTake->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::onClickTake, this));

	mTabSave = static_cast<ImageView*>(popBg->getChildByTag(imageTabItem2Tag));
	mTabSave->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::onClickSave, this));

	mMovItem =  static_cast<ImageView*>(popBg->getChildByTag(imageTabCommonTag));
	mMovItem->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::onClickCommon,this));
		 
	mGetOutPanel = static_cast<Layout*>(popBg->getChildByTag(PanelGetOutTag));
	mPutInPanel  = static_cast<Layout*>(popBg->getChildByTag(PanelInTag));
	mPutInPanel->setTouchEnabled(false);

	Button*  operta_btn = static_cast<Button*>(popBg->getChildByTag(BtnGetOutOrInTag));
	operta_btn->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::onBankGetOutOrInPro, this));

	ImageView* outFiledBg = static_cast<ImageView*>(mGetOutPanel->getChildByTag(ImageInputBgTag));
	mTextFieldOut = static_cast<TextField*>(outFiledBg->getChildByTag(InputTextGetOutTag));
	
	mOutEdit= ui::EditBox::create(mTextFieldOut->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mOutEdit->setPosition(Vec2(mTextFieldOut->getContentSize().width/2,mTextFieldOut->getContentSize().height/2));
	mOutEdit->setFontSize(mTextFieldOut->getFontSize());
	mOutEdit->setFontColor(mTextFieldOut->getColor());//设置文字颜色

	mOutEdit->setPlaceHolder(mTextFieldOut->getPlaceHolder().c_str());//点位符
	mOutEdit->setPlaceholderFontSize(mTextFieldOut->getFontSize());
	mOutEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mOutEdit->setText(mTextFieldOut->getStringValue().c_str());


	mTextFieldOut->addChild(mOutEdit, 10);


	ImageView* inFiledBg = static_cast<ImageView*>(mPutInPanel->getChildByTag(ImageInputInBgTag));
	mTextFieldIn = static_cast<TextField*>(inFiledBg->getChildByTag(InputTextInTag));

	mInEdit= ui::EditBox::create(mTextFieldIn->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mInEdit->setPosition(Vec2(mTextFieldIn->getContentSize().width/2,mTextFieldIn->getContentSize().height/2));
	mInEdit->setFontSize(mTextFieldIn->getFontSize());
	mInEdit->setFontColor(mTextFieldIn->getColor());//设置文字颜色

	mInEdit->setPlaceHolder(mTextFieldIn->getPlaceHolder().c_str());//点位符
	mInEdit->setPlaceholderFontSize(mTextFieldIn->getFontSize());
	mInEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mInEdit->setText(mTextFieldIn->getStringValue().c_str());

	mTextFieldIn->addChild(mInEdit, 10);

	mTakeScoreText = static_cast<Text*>(mGetOutPanel->getChildByTag(TextTakeOutTag));
	mSaveScoreText = static_cast<Text*>(mPutInPanel->getChildByTag(TextSaveInTag));


	ImageView* takeBitBg = static_cast<ImageView*>(mGetOutPanel->getChildByTag(ImageTakeOutBitBg));
	takeBitBg->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::modifyScore, this));
	ImageView* takeBitBg2 = static_cast<ImageView*>(mGetOutPanel->getChildByTag(ImageTakeOutBitBg2));
	takeBitBg2->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::modifyScore, this));
	ImageView* saveBitBg = static_cast<ImageView*>(mPutInPanel->getChildByTag(ImageSaveInBitBg));
	saveBitBg->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::modifyScore, this));
	ImageView* saveBitBg2 = static_cast<ImageView*>(mPutInPanel->getChildByTag(ImageSaveInBitBg2));
	saveBitBg2->addTouchEventListener(CC_CALLBACK_2(GameHallBankWidget::modifyScore, this));
}

void GameHallBankWidget::onClose(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
	}
}

void GameHallBankWidget::modifyScore(Ref* pSender,ui::Widget::TouchEventType type)
{
	Vec2 touchPos;
	int index = 1;

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ImageView* pBit = (ImageView*)pSender;
		
		touchPos = pBit->getTouchEndPosition();
		Vec2 pos = pBit->getPosition();
		Vec2 wordPos = pBit->convertToNodeSpace(touchPos);
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
		
		
		if (pBit->getTag() == ImageTakeOutBitBg || pBit->getTag() == ImageTakeOutBitBg2 )
		{
			if ( pBit->getTag() == ImageTakeOutBitBg2 )
			{
				index +=3;
			}
			if (mTakeSelectIndex == index || index > mCanTakeIndex)
			{
				return;
			}
			if(mTakeSelectIndex > 0)
				setSelect(mTakeSelectIndex,false);
			setSelect(index,true);
			mTakeSelectIndex = index;
		}
		else
		{
			if ( pBit->getTag() == ImageSaveInBitBg2 )
			{
				index +=3;
			}
			if (mSaveSelectIndex == index || index > mCanSaveIndex)
			{
				return;
			}
			if(mSaveSelectIndex > 0)
				setSelect(mSaveSelectIndex,false,false);
			setSelect(index,true,false);
			mSaveSelectIndex = index;
		}

		
	}
}
void GameHallBankWidget::setSelect(int index,bool isShow,bool isTakeOut /* = true */)
{

	if (index <=0)
		return;

	int imageTag = 0;
	int textTag = 0;
	ImageView* imageBit = nullptr;
	Text*	   textBit = nullptr;
	Layout* select_panel = mGetOutPanel;
	if (!isTakeOut)
	{
		select_panel = mPutInPanel;
	}
	switch (index)
	{
	case 1:
		if (isTakeOut)
		{
			imageTag =  ImageAllOutTag;
			textTag  =  LableAllOutTag;
		}
		else
		{
			imageTag =  ImageAllInTag;
			textTag  =  LableAllInTag;
		}
		break;
	case 2:
		if (isTakeOut)
		{
			imageTag =  Image1bwOutTag;
			textTag  =  Lable1bwOutTag;
		}
		else
		{
			imageTag =  Image1bwInTag;
			textTag  =  Lable1bwInTag;
		}
		break;
	case 3:
		if (isTakeOut)
		{
			imageTag =  Image5bwOutTag;
			textTag  =  Lable5bwOutTag;
		}
		else
		{
			imageTag =  Image5bwInTag;
			textTag  =  Lable5bwInTag;
		}
		break;
	case 4:
		if (isTakeOut)
		{
			imageTag =  Image1kwOutTag;
			textTag  =  Lable1kwOutTag;
		}
		else
		{
			imageTag =  Image1kwInTag;
			textTag  =  Lable1kwInTag;
		}
		break;
	case 5:
		if (isTakeOut)
		{
			imageTag =  Image5kwOutTag;
			textTag  =  Lable5kwOutTag;
		}
		else
		{
			imageTag =  Image5kwInTag;
			textTag  =  Lable5kwInTag;
		}
		break;
	case 6:
		if (isTakeOut)
		{
			imageTag =  Image1yOutTag;
			textTag  =  Lable1yOutTag;
		}
		else
		{
			imageTag =  Image1yInTag;
			textTag  =  Lable1yInTag;
		}
		break;
	}


	imageBit = static_cast<ImageView*>(select_panel->getChildByTag(imageTag));
	imageBit->setVisible(isShow);
	textBit = static_cast<Text*>(select_panel->getChildByTag(textTag));
	if(!isShow)
		textBit->setColor(ccc3(96,34,0));
	else
		textBit->setColor(ccc3(255,255,255));
	textBit->setLocalZOrder(imageBit->getLocalZOrder()+1);

	if (!isTakeOut)
	{
		mInEdit->setText(getBitValue(index,isTakeOut).c_str());
	}
	else
	{
		mOutEdit->setText(getBitValue(index,isTakeOut).c_str());
	}
}

std::string GameHallBankWidget::getBitValue(int index,bool isTakeOut)
{
	switch (index)
	{
	case  1:
		if (!isTakeOut)
		{
			return mUserScore;
		}
		else
		{
			return mUserInsure;
		}
		break;
	case  2:
		return  numberToString(1000000);
		break;
	case  3:
		return numberToString(5000000);
		break;
	case  4:
		return numberToString(10000000);
		break;
	case  5:
		return numberToString(50000000);
		break;
	case  6:
		return numberToString(100000000);
		break;
	}
}

void GameHallBankWidget::onClickTake(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		
		if(!m_isInGetOut)
		{
			mMovItem->stopAllActions();

			if(mSaveSelectIndex > 0)
			{
				setSelect(mSaveSelectIndex,false,false);
				mSaveSelectIndex = -1;
				mInEdit->setText("");
			}
			Button*  operta_btn = static_cast<Button*>(popBg->getChildByTag(BtnGetOutOrInTag));
			
			m_isInGetOut = true;
			ImageView* _text =  static_cast<ImageView*>(operta_btn->getChildByTag(LabelGetOutOrInTag));
			_text->loadTexture("font_getgold.png",Widget::TextureResType::PLIST);

			MoveTo* mb = MoveTo::create(0.05f,mTabTake->getPosition());
			CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameHallBankWidget::switchToTakePanel));
			mMovItem->runAction(Sequence::createWithTwoActions(mb,callback));
		}
		
	}
}

void GameHallBankWidget::onClickSave(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{

		if(m_isInGetOut)
		{
			mMovItem->stopAllActions();

			if(mTakeSelectIndex > 0)
			{
				setSelect(mTakeSelectIndex,false);
				mTakeSelectIndex = -1;
				mOutEdit->setText("");
			}
			Button*  operta_btn = static_cast<Button*>(popBg->getChildByTag(BtnGetOutOrInTag));
			m_isInGetOut = false;
			ImageView* _text =  static_cast<ImageView*>(operta_btn->getChildByTag(LabelGetOutOrInTag));
			_text->loadTexture("font_cunru.png",Widget::TextureResType::PLIST);
			
			MoveTo* mb = MoveTo::create(0.05f,mTabSave->getPosition());
			CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameHallBankWidget::switchToSavePanel));
			mMovItem->runAction(Sequence::createWithTwoActions(mb,callback));
		}

		
	}
}

void GameHallBankWidget::onClickCommon(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (mHasMov)
	{
		return ;
	}
	Widget* pWidget = (Widget*)pSender;
	if(eventtype == ui::Widget::TouchEventType::BEGAN)
	{
		mStartPos = pWidget->getTouchBeganPosition();
	}
	else if (eventtype == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 curPos = pWidget->getTouchEndPosition();
		if (curPos.x - mStartPos.x <=0)//向左移动
		{
			mHasMov = true;
			Button*  operta_btn = static_cast<Button*>(popBg->getChildByTag(BtnGetOutOrInTag));

			m_isInGetOut = true;
			ImageView* _text =  static_cast<ImageView*>(operta_btn->getChildByTag(LabelGetOutOrInTag));
			_text->loadTexture("font_getgold.png",Widget::TextureResType::PLIST);

			MoveTo* mb = MoveTo::create(0.05f,mTabTake->getPosition());
			CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameHallBankWidget::switchToTakePanel));
			mMovItem->runAction(Sequence::createWithTwoActions(mb,callback));
		}
		else //向右移动
		{
			mHasMov = true;
			Button*  operta_btn = static_cast<Button*>(popBg->getChildByTag(BtnGetOutOrInTag));
			m_isInGetOut = false;
			ImageView* _text =  static_cast<ImageView*>(operta_btn->getChildByTag(LabelGetOutOrInTag));
			_text->loadTexture("font_cunru.png",Widget::TextureResType::PLIST);

			MoveTo* mb = MoveTo::create(0.05f,mTabSave->getPosition());
			CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameHallBankWidget::switchToSavePanel));
			mMovItem->runAction(Sequence::createWithTwoActions(mb,callback));
		}
	}
}


void GameHallBankWidget::switchToSavePanel(Node* node)
{
	ImageView* image = (ImageView*)node;
	image->loadTexture("bank_savescore.png",ui::Widget::TextureResType::PLIST);
	mGetOutPanel->setVisible(m_isInGetOut);
	mPutInPanel->setVisible(!m_isInGetOut);
	mHasMov = false;
	checkCanClick();
}

void GameHallBankWidget::switchToTakePanel(Node* node)
{
	ImageView* image = (ImageView*)node;
	image->loadTexture("bank_takescore.png",ui::Widget::TextureResType::PLIST);
	mGetOutPanel->setVisible(m_isInGetOut);
	mPutInPanel->setVisible(!m_isInGetOut);
	mHasMov = false;
	checkCanClick();
}


void GameHallBankWidget::onBankGetOutOrInPro(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		//取出
		if (m_isInGetOut)
		{
			
			SCORE score = atoi(mOutEdit->getText());
			if(score <= 0)
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("取出金额不能为空!")));
				return;
			}
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserTakeScoreRequest(score);
		}
		else//存入
		{
			
			SCORE score = atoi(mInEdit->getText());
			if(score <= 0)
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("存入金额不能为空!")));
				return;
			}
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserSaveScoreRequest(score);
		}
		showLoading();
	}
}


void GameHallBankWidget::onInputGoldNumber(Ref *sender, TextFiledEventType type)
{
	if (TEXTFIELD_EVENT_DETACH_WITH_IME == type) {
		TextField* pTextField = (TextField*)sender;
		if (pTextField->getTag() == InputTextInTag)
		{
			mTextFieldIn->setText(pTextField->getStringValue());
		}
		else if(pTextField->getTag() == InputTextGetOutTag)
		{
			mTextFieldOut->setText(pTextField->getStringValue());
		}
	
	}
}

void GameHallBankWidget::refreshInsureData(EventCustom* evt)
{
	//银行资料
	removeLoading();
	CMD_GP_UserInsureInfo* inSureInfo = (CMD_GP_UserInsureInfo*)evt->getUserData();
	__String* insureScore = __String::createWithFormat("%lld",inSureInfo->lUserInsure) ;
	mTakeScoreText->setText(insureScore->getCString());
	mUserInsure = insureScore->getCString();
	mTextFieldOut->setOpacity(255);

	__String* useScore = __String::createWithFormat("%lld",inSureInfo->lUserScore) ;
	mSaveScoreText->setText( useScore->getCString());
	mUserScore = useScore->getCString();
	mTextFieldIn->setOpacity(255);

	checkCanClick();
}

void GameHallBankWidget::receiveInsureSuccessMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_UserInsureSuccess* info = (CMD_GP_UserInsureSuccess*)evt->getUserData();
	__String* insureScore = __String::createWithFormat("%lld",info->lUserInsure) ;
	mUserInsure = numberToString(insureScore->getCString());
	mTakeScoreText->setText(insureScore->getCString());
	__String* useScore = __String::createWithFormat("%lld",info->lUserScore) ;
	mSaveScoreText->setText( useScore->getCString());
	mUserScore = numberToString(useScore->getCString());

	//取出
	if (m_isInGetOut)
	{
		setSelect(mTakeSelectIndex,false);
		mOutEdit->setText("");
		mTakeSelectIndex = 0;
		
	}
	else//存入
	{
		setSelect(mSaveSelectIndex,false,false);
		mInEdit->setText("");
		mSaveSelectIndex = 0;
	}
	
	checkCanClick();

	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(info->szDescribeString)));

	SessionManager::shareInstance()->getUser()->setUserScore(info->lUserScore);

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateUserInfoMsg);	
}

void GameHallBankWidget::receiveInsureFailureMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_UserInsureFailure* info = (CMD_GP_UserInsureFailure*)evt->getUserData();

	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(info->szDescribeString)));
}

void GameHallBankWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallBankWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}


void GameHallBankWidget::checkCanClick()
{

	Layout* select_panel = mGetOutPanel;
	if (!m_isInGetOut)
	{
		select_panel = mPutInPanel;
	}
	ImageView* allImage = nullptr;
	ImageView* yBWImage = nullptr;
	ImageView* wBWImage = nullptr;
	ImageView* yKWImage = nullptr;
	ImageView* wKWImage = nullptr;
	ImageView* yYImage  = nullptr;

	if (m_isInGetOut)
	{
		allImage = static_cast<ImageView*>(select_panel->getChildByTag(ImageAllOutTag));
		yBWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1bwOutTag));
		wBWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image5bwOutTag));
		yKWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1kwOutTag));
		wKWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image5kwOutTag));
		yYImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1yOutTag));
	}
	else
	{
		allImage = static_cast<ImageView*>(select_panel->getChildByTag(ImageAllInTag));
		yBWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1bwInTag));
		wBWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image5bwInTag));
		yKWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1kwInTag));
		wKWImage = static_cast<ImageView*>(select_panel->getChildByTag(Image5kwInTag));
		yYImage = static_cast<ImageView*>(select_panel->getChildByTag(Image1yInTag));
	}
	
	//重置回去
	allImage->setVisible(false);
	yBWImage->setVisible(false);
	wBWImage->setVisible(false);
	yKWImage->setVisible(false);
	wKWImage->setVisible(false);
	yYImage->setVisible(false);

	CommonFunction::removeGray(allImage);
	CommonFunction::removeGray(yBWImage);
	CommonFunction::removeGray(wBWImage);
	CommonFunction::removeGray(yKWImage);
	CommonFunction::removeGray(wKWImage);
	CommonFunction::removeGray(yYImage);
	
	//取出
	if (m_isInGetOut)
	{
		int score = atoi(mUserInsure.c_str());
		if(score <= 0)
			mCanTakeIndex = 0;
		else if( score > 0 && score < 1000000)
			mCanTakeIndex  = 1;
		else if( score >= 1000000 && score < 5000000)
			mCanTakeIndex  = 2;
		else if( score >= 5000000 && score < 10000000)
			mCanTakeIndex  = 3;
		else if( score >= 10000000 && score < 50000000)
			mCanTakeIndex  = 4;
		else if( score >= 50000000 && score < 100000000)
			mCanTakeIndex  = 5;
		else if( score >= 100000000 )
			mCanTakeIndex  = 6;

		
	}
	else
	{
		int score = atoi(mUserScore.c_str());
		if(score <= 0)
			mCanSaveIndex = 0;
		else if( score > 0 && score < 1000000)
			mCanSaveIndex  = 1;
		else if( score >= 1000000 && score < 5000000)
			mCanSaveIndex  = 2;
		else if( score >= 5000000 && score < 10000000)
			mCanSaveIndex  = 3;
		else if( score >= 10000000 && score < 50000000)
			mCanSaveIndex  = 4;
		else if( score >= 50000000 && score < 100000000)
			mCanSaveIndex  = 5;
		else if( score >= 100000000 )
			mCanSaveIndex  = 6;

	}

	int index = mCanTakeIndex ;
	if (!m_isInGetOut)
	{
		index = mCanSaveIndex;
	}
	switch (index)
	{
	case 0:
		allImage->setVisible(true);
		yBWImage->setVisible(true);
		wBWImage->setVisible(true);
		yKWImage->setVisible(true);
		wKWImage->setVisible(true);
		yYImage->setVisible(true);

		CommonFunction::addGray(allImage);
		CommonFunction::addGray(yBWImage);
		CommonFunction::addGray(wBWImage);
		CommonFunction::addGray(yKWImage);
		CommonFunction::addGray(wKWImage);
		CommonFunction::addGray(yYImage);
		break;
	case 1:
		yBWImage->setVisible(true);
		wBWImage->setVisible(true);
		yKWImage->setVisible(true);
		wKWImage->setVisible(true);
		yYImage->setVisible(true);

		CommonFunction::addGray(yBWImage);
		CommonFunction::addGray(wBWImage);
		CommonFunction::addGray(yKWImage);
		CommonFunction::addGray(wKWImage);
		CommonFunction::addGray(yYImage);
		break;
	case 2:

		wBWImage->setVisible(true);
		yKWImage->setVisible(true);
		wKWImage->setVisible(true);
		yYImage->setVisible(true);

		CommonFunction::addGray(wBWImage);
		CommonFunction::addGray(yKWImage);
		CommonFunction::addGray(wKWImage);
		CommonFunction::addGray(yYImage);
		break;
	case 3:

		yKWImage->setVisible(true);
		wKWImage->setVisible(true);
		yYImage->setVisible(true);

		CommonFunction::addGray(yKWImage);
		CommonFunction::addGray(wKWImage);
		CommonFunction::addGray(yYImage);
		break;
	case 4:

		wKWImage->setVisible(true);
		yYImage->setVisible(true);


		CommonFunction::addGray(wKWImage);
		CommonFunction::addGray(yYImage);
		break;
	case 5:
		yYImage->setVisible(true);
		CommonFunction::addGray(yYImage);
		break;
	case 6:
		break;
	}
}