#include "GameHallAssignItem.h"
#include "CommonFunction.h"

GameHallAssignItem::GameHallAssignItem()
{
	memset(&mInfo,0,sizeof(mInfo));
	mSelectImage = nullptr;
}

GameHallAssignItem::~GameHallAssignItem()
{

}

GameHallAssignItem* GameHallAssignItem::create(CMD_GP_SignInTask info)
{
	GameHallAssignItem* pItem = new GameHallAssignItem();
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

bool GameHallAssignItem::init(CMD_GP_SignInTask info)
{
	if (!Layout::init())
	{
		return false;
	}
	mInfo = info;

	std::string imageName = "";
	if (info.dwDay == 3)
	{
		imageName = "image_charge_2.png";
	}
	else if (info.dwDay == 8)
	{
		imageName = "image_charge_3.png";
	}
	else if (info.dwDay == 15)
	{
		imageName = "image_charge_4.png";
	}
	else
	{
		imageName = "maxGold.png";
	}
	gold = ImageView::create();
	gold->loadTexture(imageName,ui::Widget::TextureResType::PLIST);
	gold->setScale(0.8f);
	addChild(gold);

	Sprite* paopao = Sprite::create("assign_qipao1.png");

	std::vector<std::string> imageNames;
	std::string path  = "";
	for (int i = 0 ; i < 4;i++ )
	{
		if(i < 3)
			path= "assign_qipao" + numberToString(i+1) + ".png";
		else
			path= "assign_qipao" + numberToString(2-i%3) + ".png";
		imageNames.push_back(path);
	}


	Animate* pAction = CommonFunction::createFrameAnimate(imageNames,0.2f);
	paopao->runAction(RepeatForever::create(pAction));
	paopao->setPosition(Vec2(0,-5));
	addChild(paopao);

	LabelAtlas* awardLabel = LabelAtlas::create("0","award_number.png",20,27,'0');
	awardLabel->setString(__String::createWithFormat("%lld",info.lScore)->getCString());
	awardLabel->setPosition(Vec2(paopao->getContentSize().width/2,awardLabel->getContentSize().height/2 +5));
	awardLabel->setAnchorPoint(Vec2(0.5,0.5));
	paopao->addChild(awardLabel);

	mSelectImage = ImageView::create();
	mSelectImage->loadTexture(__String::createWithFormat("assign_%d.png",info.dwDay)->getCString(),ui::Widget::TextureResType::PLIST);
	mSelectImage->setPosition(Vec2(0,paopao->getContentSize().height/2 - mSelectImage->getContentSize().height/2 + 10));
	addChild(mSelectImage);

	mHasGetImage = ImageView::create();
	mHasGetImage->loadTexture("image_yiding.png",ui::Widget::TextureResType::PLIST);
	//mHasGetImage->setPosition(Vec2(gold->getContentSize().width/2,gold->getContentSize().height/2));
	mHasGetImage->setVisible(false);
	addChild(mHasGetImage);
	return true;
}

void GameHallAssignItem::setSelect(bool value)
{
	if (value)
	{
		mSelectImage->loadTexture(__String::createWithFormat("assign_%d_on.png",mInfo.dwDay)->getCString(),ui::Widget::TextureResType::PLIST);
		if(!mHasGetImage->isVisible())
		{
			gold->setOpacity(120);
			mHasGetImage->setVisible(true);
		}
	}
	else
	{
		mSelectImage->loadTexture(__String::createWithFormat("assign_%d.png",mInfo.dwDay)->getCString(),ui::Widget::TextureResType::PLIST);
		
	}
}