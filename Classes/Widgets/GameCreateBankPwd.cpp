#include "GameCreateBankPwd.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "Model/LoginUserModel.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "CommonFunction.h"
#include "GameHallBankWidget.h"


#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnCreatTag 398
#define  TextFieldPwdTag 400
#define  TextFieldPwdAgainTag 415

GameBankCreatePwdWidget::GameBankCreatePwdWidget()
{
	mPwdAgainText = nullptr;
	mPwdText = nullptr;
}

GameBankCreatePwdWidget::~GameBankCreatePwdWidget()
{

}

std::string GameBankCreatePwdWidget::getWidgetJsonName()
{
	return "modifyBankMM";
}

void GameBankCreatePwdWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameBankCreatePwdWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameBankCreatePwdWidget::receiveOperateSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameBankCreatePwdWidget::receiveOperateFailureMsg, this)), this);
}

bool GameBankCreatePwdWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}



	return true;
}
void GameBankCreatePwdWidget::loadUI()
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
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameBankCreatePwdWidget::onBack, this));

	Button* btn_create = static_cast<Button*>(popBg->getChildByTag(BtnCreatTag));
	btn_create->addTouchEventListener(CC_CALLBACK_2(GameBankCreatePwdWidget::onCreate, this));

	mPwdText = static_cast<TextField*>(popBg->getChildByTag(TextFieldPwdTag));

	mPwdEdit = ui::EditBox::create(mPwdText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mPwdEdit->setPosition(Vec2(mPwdText->getContentSize().width/2,mPwdText->getContentSize().height/2));
	mPwdEdit->setFontSize(mPwdText->getFontSize());
	mPwdEdit->setFontColor(mPwdText->getColor());//设置文字颜色

	mPwdEdit->setPlaceHolder(mPwdText->getPlaceHolder().c_str());//点位符
	mPwdEdit->setPlaceholderFontSize(mPwdText->getFontSize());
	mPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mPwdEdit->setText(mPwdText->getStringValue().c_str());
	mPwdEdit->setTag(mPwdText->getTag());

	mPwdText->addChild(mPwdEdit, 10);

	mPwdAgainText = static_cast<TextField*>(popBg->getChildByTag(TextFieldPwdAgainTag));
	mEnsurePwdEdit = ui::EditBox::create(mPwdText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mEnsurePwdEdit->setPosition(Vec2(mPwdText->getContentSize().width/2,mPwdText->getContentSize().height/2));
	mEnsurePwdEdit->setFontSize(mPwdText->getFontSize());
	mEnsurePwdEdit->setFontColor(mPwdText->getColor());

	mEnsurePwdEdit->setPlaceHolder(mPwdText->getPlaceHolder().c_str());
	mEnsurePwdEdit->setPlaceholderFontSize(mPwdText->getFontSize());
	mEnsurePwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mEnsurePwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mEnsurePwdEdit->setText(mPwdText->getStringValue().c_str());
	mEnsurePwdEdit->setTag(mPwdText->getTag());

	mPwdAgainText->addChild(mEnsurePwdEdit, 10);
}


void GameBankCreatePwdWidget::receiveOperateSuccessMsg(EventCustom* evt)
{
	removeLoading();
	//保存银行密码
	//LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	//loginModel->setInsurePassword(mPwdText->getString());
	//保存到本地
	//loginModel->flush();
	SessionManager::shareInstance()->setBankPwdFlag(true);
	//保存到缓存
	SessionManager::shareInstance()->setInsurePass(mPwdEdit->getText());
	removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(GameHallBankWidget::create());
}


void GameBankCreatePwdWidget::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	
	CMD_GP_OperateFailure* failureInfo = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(failureInfo->szDescribeString)));
}


void GameBankCreatePwdWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameBankCreatePwdWidget::onCreate(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if(strlen(mPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能为空!")));
			return;
		}

		if (strcmp(mPwdEdit->getText(),mEnsurePwdEdit->getText()) != 0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码与确认密码不一致!")));
			return;
		}
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyInsurePass(mPwdEdit->getText());
		showLoading();
	}
}

void GameBankCreatePwdWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameBankCreatePwdWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}