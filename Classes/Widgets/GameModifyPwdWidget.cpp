#include "GameModifyPwdWidget.h"
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
#include "GameMainScene.h"
#include "GameHallLoginWidget.h"

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnOkTag 398
#define  TextFieldOldPwdTag 400
#define  TextFieldNewPwdTag 409
#define  TextFieldPwdAgainTag 415

GameModifyPwdWidget::GameModifyPwdWidget()
{
	mPwdAgainText = nullptr;
	mNewPwdText = nullptr;
	mOldPwdText = nullptr;
}

GameModifyPwdWidget::~GameModifyPwdWidget()
{

}

std::string GameModifyPwdWidget::getWidgetJsonName()
{
	return "modifyMM";
}

void GameModifyPwdWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameModifyPwdWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameModifyPwdWidget::receiveOperateSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameModifyPwdWidget::receiveOperateFailureMsg, this)), this);
}

GameModifyPwdWidget* GameModifyPwdWidget::create(PwdType type)
{
	GameModifyPwdWidget* pWidget = new GameModifyPwdWidget();
	if (pWidget && pWidget->init(type))
	{
		pWidget->autorelease();
		return pWidget;
	}
	else
	{
		delete pWidget;
		pWidget = nullptr;
		return nullptr;
	}
}
bool GameModifyPwdWidget::init(PwdType type)
{
	if (!TNWidget::init())
	{
		return false;
	}
	mType = type;


	return true;
}
void GameModifyPwdWidget::loadUI()
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
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameModifyPwdWidget::onBack, this));

	Button* btn_Ok = static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	btn_Ok->addTouchEventListener(CC_CALLBACK_2(GameModifyPwdWidget::onOk, this));

	mOldPwdText = static_cast<TextField*>(popBg->getChildByTag(TextFieldOldPwdTag));

	mOldPwdEdit = ui::EditBox::create(mOldPwdText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mOldPwdEdit->setPosition(Vec2(mOldPwdText->getContentSize().width/2,mOldPwdText->getContentSize().height/2));
	mOldPwdEdit->setFontSize(mOldPwdText->getFontSize());
	mOldPwdEdit->setFontColor(mOldPwdText->getColor());//设置文字颜色

	mOldPwdEdit->setPlaceHolder(mOldPwdText->getPlaceHolder().c_str());//点位符
	mOldPwdEdit->setPlaceholderFontSize(mOldPwdText->getFontSize());
	mOldPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mOldPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mOldPwdEdit->setText(mOldPwdText->getStringValue().c_str());
	mOldPwdEdit->setTag(mOldPwdText->getTag());
	mOldPwdText->addChild(mOldPwdEdit, 10);

	mNewPwdText = static_cast<TextField*>(popBg->getChildByTag(TextFieldNewPwdTag));

	mNewPwdEdit = ui::EditBox::create(mNewPwdText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mNewPwdEdit->setPosition(Vec2(mNewPwdText->getContentSize().width/2,mNewPwdText->getContentSize().height/2));
	mNewPwdEdit->setFontSize(mNewPwdText->getFontSize());
	mNewPwdEdit->setFontColor(mNewPwdText->getColor());//设置文字颜色

	if(mType == PwdType_Login)
		mNewPwdEdit->setPlaceHolder(CommonFunction::GBKToUTF8("请输入新登录密码").c_str());//点位符
	else
		mNewPwdEdit->setPlaceHolder(CommonFunction::GBKToUTF8("请输入新保险柜密码").c_str());//点位符

	mNewPwdEdit->setPlaceholderFontSize(mNewPwdText->getFontSize());
	mNewPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mNewPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mNewPwdEdit->setText(mNewPwdText->getStringValue().c_str());
	mNewPwdEdit->setTag(mNewPwdText->getTag());
	mNewPwdText->addChild(mNewPwdEdit, 10);


	mPwdAgainText = static_cast<TextField*>(popBg->getChildByTag(TextFieldPwdAgainTag));

	mEnsureEdit = ui::EditBox::create(mPwdAgainText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mEnsureEdit->setPosition(Vec2(mPwdAgainText->getContentSize().width/2,mPwdAgainText->getContentSize().height/2));
	mEnsureEdit->setFontSize(mPwdAgainText->getFontSize());
	mEnsureEdit->setFontColor(mPwdAgainText->getColor());//设置文字颜色

	if(mType == PwdType_Login)
		mEnsureEdit->setPlaceHolder(CommonFunction::GBKToUTF8("请再次输入登录密码").c_str());//点位符
	else
		mEnsureEdit->setPlaceHolder(CommonFunction::GBKToUTF8("请再次输入保险柜密码").c_str());//点位符

	mEnsureEdit->setPlaceholderFontSize(mPwdAgainText->getFontSize());
	mEnsureEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mEnsureEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mEnsureEdit->setText(mPwdAgainText->getStringValue().c_str());
	mEnsureEdit->setTag(mPwdAgainText->getTag());
	mPwdAgainText->addChild(mEnsureEdit, 10);
}


void GameModifyPwdWidget::receiveOperateSuccessMsg(EventCustom* evt)
{
	removeLoading();
	if (mType == PwdType_Bank)
	{
		////保存银行密码
		//LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
		//loginModel->setInsurePassword(mNewPwdEdit->getText());
		////保存到本地
		//loginModel->flush();
		//置重设银行密码为TRUE
		SessionManager::shareInstance()->setBankPwdFlag(true);

		CMD_GP_OperateSuccess *success  = (CMD_GP_OperateSuccess*)evt->getUserData();
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(success->szDescribeString)));
	}
	else
	{
		//保存用户登录密码
		LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
		loginModel->setPassword(mNewPwdEdit->getText());
		//保存到本地
		loginModel->flush();
		//改变密码成功后，更新到帐号文件中去
		__Array* accouts = SessionManager::shareInstance()->getAccountList();
		int index = SessionManager::shareInstance()->getIndexByAccount(loginModel->getAccount());
		if (index >0)
		{
			accouts->removeObjectAtIndex(index);
			accouts->addObject(__String::createWithFormat("%s&%s",loginModel->getAccount().c_str(),mNewPwdEdit->getText()));
			//重新保存
			SessionManager::shareInstance()->writeAccountFile();
		}

		CMD_GP_OperateSuccess *success  = (CMD_GP_OperateSuccess*)evt->getUserData();
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(success->szDescribeString)));
		//removeFromParent();
	}
	mOldPwdEdit->setText("");
	mNewPwdEdit->setText("");
	mEnsureEdit->setText("");

	
}


void GameModifyPwdWidget::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	
	CMD_GP_OperateFailure* failureInfo = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(failureInfo->szDescribeString)));
}


void GameModifyPwdWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameModifyPwdWidget::onOk(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (strlen(mOldPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("旧密码不能为空!")));
			return;
		}
		if(strlen(mNewPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("新密码不能为空!")));
			return;
		}
		else
		{
			if(CommonFunction::isDigit2(mNewPwdEdit->getText()))
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("新密码不能全为数字!")));
				return;
			}
		}
		if (strcmp(mEnsureEdit->getText(),mNewPwdEdit->getText()) != 0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("新密码与确认密码不一致!")));
			return;
		}
		int type = 0;
		if (mType == PwdType_Bank)
		{
			type =1;
		}
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyPassword(mOldPwdEdit->getText(),mNewPwdEdit->getText(),type);
		showLoading();
	}
}

void GameModifyPwdWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameModifyPwdWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}