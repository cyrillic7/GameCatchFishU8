#include "MenuLayer.h"
#include "GameGlobalSetting.h"
#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "SessionManager.h"

#define  interval 52

#define  kMenuScaleValue 1.2f

CCMenuLayer::~CCMenuLayer()
{
}

bool CCMenuLayer::init()
{
	if(!Layer::init())
		return false;
	mHorCount =0;
	mVecCount =0;
	mArriveCnt = 0;
	mCanClick = true;
	//menu = Menu::create();
	//menu->setPosition(Vec2(40,30));
	//this->addChild(menu);

	mToggle = nullptr;
	mIsExpand = true;//默认是展开的
	return true;
}

void CCMenuLayer::onExit()
{
	Layer::onExit();
}

void CCMenuLayer::onEnter()
{
	Layer::onEnter();
}

void CCMenuLayer::AddToggleItem(const char* img,const char* img2 ,int initType)
{

	mToggle = ImageView::create("menu_on.png");
	mToggle->setScale(kMenuScaleValue);
	mToggle->setPosition(Vec2(35,30));
	mToggle->setTouchSize(CCSizeMake(113,122));
	mToggle->setTag(0);
	addChild(mToggle);
	mToggle->setTouchEnabled(true);
	mToggle->addTouchEventListener(CC_CALLBACK_2(CCMenuLayer::onClickToggle, this));

	Sprite* qipaoSp = Sprite::create();
	qipaoSp->setPosition(Vec2(38,35));
	mToggle->addChild(qipaoSp);
	std::vector<std::string> imageNames;
	std::string path = "";
	for (int i = 0 ; i < 6;i++)
	{
		if (i < 4)
			path = "menu"+numberToString(i+1)+".png";
		else
			path = "menu"+numberToString(4-i%3)+".png";
	
		imageNames.push_back(path);
	}

	Animate* pAnima = CommonFunction::createFrameAnimate(imageNames,0.2);
	qipaoSp->runAction(RepeatForever::create(pAnima));
	

}

void CCMenuLayer::onClickToggle(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}
	mToggle->setTouchEnabled(false);
	if (mIsExpand)
	{
		mToggle->loadTexture("menu.png");
		mIsExpand = false;
	}
	else
	{
		mToggle->loadTexture("menu_on.png");
		mIsExpand = true;
	}
	int i = 0;
	Vector<Node*> childrens = getChildren();
	if (mIsExpand)//展开
	{
		for(i = 0 ; i < childrens.size();i++)
		{
			ImageView* item = (ImageView*)childrens.at(i);
			item->setVisible(true);
			Vec2 pt = item->getPosition();
			int tag = item->getTag();
			if (tag == 0)
				continue;
			if (tag%2 == 0)//纵向
			{
			
				MoveTo* movto = MoveTo::create(/*(tag-1)*0.05f*/0.14f,Vec2(40,mToggle->getContentSize().height/2 + tag/2*(item->getContentSize().height+interval)));
				CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(CCMenuLayer::MenuShow));
				item->runAction(Sequence::createWithTwoActions(movto,pCall));
			}
			else//横向
			{
				MoveTo* movto = MoveTo::create(/*(tag/2 + 1)*0.05f*/0.14f,Vec2(mToggle->getContentSize().width/2 + (tag/2 + 1)*(item->getContentSize().width+interval),30));
				CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(CCMenuLayer::MenuShow));
				item->runAction(Sequence::createWithTwoActions(movto,pCall));
			}
		}
	}
	else
	{
		for(i = 0 ; i < childrens.size();i++)
		{
			ImageView* item = (ImageView*)childrens.at(i);
			Vec2 pt = item->getPosition();
			int tag = item->getTag();
			if (tag == 0)
				continue;
			if (tag%2 == 0)//纵向
			{
				MoveTo* movto = MoveTo::create(/*(tag-1)*0.05f*/0.14f,Vec2(40,30));
				CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(CCMenuLayer::MenuHide));
				item->runAction(Sequence::createWithTwoActions(movto,pCall));
			}
			else//横向
			{
				MoveTo* movto = MoveTo::create(/*(tag/2 + 1)*0.05f*/0.14f,Vec2(40,30));
				CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(CCMenuLayer::MenuHide));
				item->runAction(Sequence::createWithTwoActions(movto,pCall));
			}

		}
	}

}

void CCMenuLayer::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 1.1f : 1.2f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}



void CCMenuLayer::SetTogPosition( Vec2 pt )
{
	mToggle->setPosition(pt);
}



void CCMenuLayer::AppendItem(const char* normal,const char* select,int type ,int index)
{
//	Sprite* normalsprite = Sprite::create(normal);
//	Sprite* selectsprite = Sprite::create(select);
//	MenuItemSprite* item = MenuItemSprite::create(normalsprite,selectsprite,mRec,mSelector);
//
//	Sprite* sp = (Sprite*)item->getNormalImage();
//	Vec2 pt = menu->getPosition();
//	if (type == hortype)
//	{
//		item->setTag(mHorCount*2+1);
//		mHorCount++;
//		item->setPosition(Vec2((sp->getContentSize().width + interval) *mHorCount,0));
//
//	}
//	else if(type == vertype)
//	{
//		item->setTag((mVecCount+1)*2);
//		mVecCount++;
//		item->setPosition(Vec2(0,mVecCount*(sp->getContentSize().height+interval)));
//	}
//	//item->setAnchorPoint(Vec2(0.5,0.5));
//	item->setUserData( (void*)index);
//	int scale = TNVisibleRect::getVisibleRect().size.height / 640 ;
//	item->setContentSize(CCSizeMake(item->getContentSize().width,item->getContentSize().height + scale*20));
//	menu->addChild(item,mToggle->getLocalZOrder()-1);

	ImageView* item = ImageView::create(normal);
	item->setScale(kMenuScaleValue);
	if (type == hortype)
	{
		item->setTag(mHorCount*2+1);
		mHorCount++;
		item->setPosition(Vec2(mToggle->getContentSize().width/2 + (item->getContentSize().width + interval) *mHorCount,30));

	}
	else if(type == vertype)
	{
		item->setTag((mVecCount+1)*2);
		mVecCount++;
		item->setPosition(Vec2(40,mToggle->getContentSize().height/2 + mVecCount*(item->getContentSize().height+interval)));
	}

	item->setTouchEnabled(true);
	item->addTouchEventListener(mSelector);
	item->setUserData((void*)index);
	addChild(item,mToggle->getLocalZOrder()-1);

	//font
	ImageView* fontImage = ImageView::create(select);
	fontImage->setPosition(Vec2(35,20-(item->getContentSize().height - fontImage->getContentSize().height)/2));
	item->addChild(fontImage,2);

	//任务红点
	if (index ==3 )
	{

		ArmatureDataManager::getInstance()->addArmatureFileInfo("AnimationTip.ExportJson");  
		mTaskRedPoint = Armature::create("AnimationTip");
		mTaskRedPoint->setAnchorPoint(ccp(0.5, 0.5));
		mTaskRedPoint->setPosition(Vec2(item->getContentSize().width /2,item->getContentSize().height /2 -2));
		mTaskRedPoint->setVisible(SessionManager::shareInstance()->getNewTaskTag());
		if(mTaskRedPoint->isVisible())
			mTaskRedPoint->getAnimation()->play("Animation1",1,1);
		item->addChild(mTaskRedPoint,1);
	}
	else if(index == 6 )//拍卖行红点
	{

		ArmatureDataManager::getInstance()->addArmatureFileInfo("AnimationTip.ExportJson");  
		mAuctionRedPoint = Armature::create("AnimationTip");
		mAuctionRedPoint->setAnchorPoint(ccp(0.5, 0.5));
		mAuctionRedPoint->setPosition(Vec2(item->getContentSize().width /2,item->getContentSize().height /2-2));
		mAuctionRedPoint->setVisible(SessionManager::shareInstance()->getNewAuctionTag());
		if(mAuctionRedPoint->isVisible())
			mAuctionRedPoint->getAnimation()->play("Animation2",1,1);
		item->addChild(mAuctionRedPoint,1);
	}

	Sprite* qipaoSp = Sprite::create();
	//qipaoSp->setPosition(menu->getPosition());
	qipaoSp->setPosition(Vec2(35,35));
	item->addChild(qipaoSp);
	std::vector<std::string> imageNames;
	std::string path = "";
	for (int i = 0 ; i < 6;i++)
	{
		if (i < 4)
			path = "qipao"+numberToString(i+1)+".png";
		else
			path = "qipao"+numberToString(4-i%3)+".png";

		imageNames.push_back(path);
	}

	Animate* pAnima = CommonFunction::createFrameAnimate(imageNames,0.2);
	qipaoSp->runAction(RepeatForever::create(pAnima));
}

void CCMenuLayer::showRedPoint(bool value,bool isTask)
{
	if (isTask)
	{
		mTaskRedPoint->setVisible(value);
		if (mTaskRedPoint->isVisible())
		{
			mTaskRedPoint->getAnimation()->play("Animation1",1,1);
		}
	}
	else
	{
		mAuctionRedPoint->setVisible(value);
		if (mAuctionRedPoint->isVisible())
		{
			mAuctionRedPoint->getAnimation()->play("Animation2",1,1);
		}
		
	}
}

void CCMenuLayer::MenuHide( CCNode* node )
{
	ImageView* item = (ImageView*)node;
	item->setVisible(false);
	item->setTouchEnabled(false);
	mArriveCnt++;
	if (mArriveCnt == getChildrenCount()-1 )
	{
		mCanClick = true;
		mArriveCnt =0;
		mToggle->setTouchEnabled(true);
	}
}

void CCMenuLayer::MenuShow( CCNode* node )
{
	mArriveCnt++;
	ImageView* item = (ImageView*)node;
	item->setTouchEnabled(true);
	if (mArriveCnt == getChildrenCount() - 1)
	{
		mCanClick = true;
		mArriveCnt =0;
		mToggle->setTouchEnabled(true);
	}
}




