#include "GameHallPaimaiItem.h"
#include "CommonFunction.h"
#include "GameServiceClientManager.h"
#include "Common/WebImageView.h"
#include "GameHallBuyAlertWidget.h"
#include "Common/ModalViewManager.h"
#include "SessionManager.h"

#define  ITEM_HEIGHT 80
#define  ITEM_POSY   (ITEM_HEIGHT)/2 - 5

#define  SELL_ITEM_HEIGHT 50

GameHallPaimaiInfoItem::GameHallPaimaiInfoItem()
{
	mData = nullptr;
}

GameHallPaimaiInfoItem::~GameHallPaimaiInfoItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallPaimaiInfoItem* GameHallPaimaiInfoItem::create(__Dictionary* infodic,int width)
{
	GameHallPaimaiInfoItem* pItem = new GameHallPaimaiInfoItem();
	if (pItem && pItem->init(infodic,width))
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

bool GameHallPaimaiInfoItem::init(__Dictionary* infodic,int width)
{
	if (!Layout::init())
	{
		return false;
	}
	mData = infodic;
	mData->retain();

	Vec2 pos = Vec2(0,0);
	setContentSize(Size(width,ITEM_HEIGHT));
	int itemWidth = width / 5;


	ImageView* line = ImageView::create("image_line.png",ui::Widget::TextureResType::PLIST);
	line->setScaleX(width*2/line->getSize().width);
	line->setPosition(Vec2(itemWidth/2 -20, getContentSize().height-5));
	addChild(line);

	//物品名称
	__String* name = (__String*)infodic->objectForKey("auctionName");
	Text* nameLable = Text::create();
	nameLable->setAnchorPoint(ccp(0.5, 0.5));
	nameLable->setFontSize(25);
	nameLable->setTextColor(ccc4(96,33,0,255));
	nameLable->setText(CommonFunction::GBKToUTF8(name->getCString()));
	int posx = itemWidth/2 - 20;
	if (itemWidth/2 - 20 <= nameLable->getContentSize().width/2)
	{
		posx = nameLable->getContentSize().width/2;
	}
	nameLable->setPosition(Vec2(posx, ITEM_POSY));
	addChild(nameLable);


	//物品个数
	__Integer* count = (__Integer*)infodic->objectForKey("propNum");
	Text* countLable = Text::create();
	countLable->setAnchorPoint(ccp(0.5, 0.5));
	countLable->setFontSize(25);
	countLable->setTextColor(ccc4(96,33,0,255));
	countLable->setText(numberToString(count->getValue()));
	countLable->setPosition(Vec2(3*itemWidth/2 -20, ITEM_POSY));
	addChild(countLable);


	//拍卖者昵称
	__String* sailerName = (__String*)infodic->objectForKey("nickName");
	Text* nickLable = Text::create();
	nickLable->setAnchorPoint(ccp(0.5, 0.5));
	nickLable->setFontSize(25);
	nickLable->setTextColor(ccc4(96,33,0,255));
	std::string trun_str = CommonFunction::truncationString(CommonFunction::GBKToUTF8(sailerName->getCString()),10);
	nickLable->setText(trun_str);
	nickLable->setPosition(Vec2( 5*itemWidth/2 - 45 , ITEM_POSY));
	addChild(nickLable);


	//拍卖者ID
	__Integer* sailerID = (__Integer*)infodic->objectForKey("id");
	Text* idLabel = Text::create();
	idLabel->setAnchorPoint(ccp(0.5, 0.5));
	idLabel->setFontSize(25);
	idLabel->setTextColor(ccc4(96,33,0,255));
	idLabel->setText(numberToString(sailerID->getValue()));
	idLabel->setPosition(Vec2( 5*itemWidth/2 + 85 , ITEM_POSY));
	addChild(idLabel);

	//拍卖品单价
	__Integer* cost = (__Integer*)infodic->objectForKey("cost");
	Text* costLabel = Text::create();
	costLabel->setAnchorPoint(ccp(0.5, 0.5));
	costLabel->setFontSize(25);
	costLabel->setTextColor(ccc4(96,33,0,255));
	costLabel->setText(numberToString(cost->getValue()));
	costLabel->setPosition(Vec2( 7*itemWidth/2 + 45, ITEM_POSY));
	addChild(costLabel);


	Button* _btnBuy = Button::create();
	_btnBuy->loadTextureNormal("btn_3.png",ui::Widget::TextureResType::PLIST);
	_btnBuy->loadTexturePressed("btn_3.png",ui::Widget::TextureResType::PLIST);
	_btnBuy->setPosition(Vec2(  9*itemWidth/2 +10, ITEM_POSY));
	_btnBuy->setTitleText(CommonFunction::GBKToUTF8("购 买"));
	_btnBuy->setScale(0.8f);
	_btnBuy->setTitleFontSize(30);
	addChild(_btnBuy);

	OnlineUserModel* model = SessionManager::shareInstance()->getUser();
	if(model->getGameId() == sailerID->getValue())//自已拍卖的
	{
		//_btnBuy->setColor(ccc3(100,100,100));
		_btnBuy->setTitleText(CommonFunction::GBKToUTF8("撤消拍买"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiInfoItem::onClickCancelBuy, this));
	}
	else
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiInfoItem::onClickBuy, this));


	return true;

}

void GameHallPaimaiInfoItem::onClickBuy(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallBuyAlertWidget::createWithDic(proType_Auction,mData));
	}
}

void GameHallPaimaiInfoItem::onClickCancelBuy(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendCancelAuctionRequest(mData);
	}
}

//////////////////////////////////////////////////////////////////////////
GameHallMyPaiamiItem::GameHallMyPaiamiItem()
{
	mData = nullptr;
}

GameHallMyPaiamiItem::~GameHallMyPaiamiItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallMyPaiamiItem* GameHallMyPaiamiItem::create(__Dictionary* infodic,int width)
{
	GameHallMyPaiamiItem* pItem = new GameHallMyPaiamiItem();
	if (pItem && pItem->init(infodic,width))
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

bool GameHallMyPaiamiItem::init(__Dictionary* infodic,int width)
{
	if (!Layout::init())
	{
		return false;
	}
	mData = infodic;
	mData->retain();

	Vec2 pos = Vec2(0,0);
	setContentSize(Size(width,ITEM_HEIGHT));
	int itemWidth = width / 4;

	ImageView* line = ImageView::create("image_line.png",ui::Widget::TextureResType::PLIST);
	line->setScaleX(width*2/line->getSize().width);
	line->setPosition(Vec2(itemWidth/2 -20, getContentSize().height-5));
	addChild(line);

	//物品名称
	__String* name = (__String*)infodic->objectForKey("auctionName");
	Text* nameLable = Text::create();
	nameLable->setAnchorPoint(ccp(0.5, 0.5));
	nameLable->setFontSize(25);
	nameLable->setTextColor(ccc4(96,33,0,255));
	nameLable->setText(CommonFunction::GBKToUTF8(name->getCString()));
	nameLable->setPosition(Vec2(itemWidth/2 -20, ITEM_POSY));
	addChild(nameLable);


	//物品个数
	__Integer* count = (__Integer*)infodic->objectForKey("propNum");
	Text* countLable = Text::create();
	countLable->setAnchorPoint(ccp(0.5, 0.5));
	countLable->setFontSize(25);
	countLable->setTextColor(ccc4(96,33,0,255));
	countLable->setText(numberToString(count->getValue()));
	countLable->setPosition(Vec2(3*itemWidth/2,ITEM_POSY));
	addChild(countLable);


	

	//拍卖品单价
	__Integer* cost = (__Integer*)infodic->objectForKey("cost");
	Text* costLabel = Text::create();
	costLabel->setAnchorPoint(ccp(0.5, 0.5));
	costLabel->setFontSize(25);
	costLabel->setTextColor(ccc4(96,33,0,255));
	costLabel->setText(numberToString(cost->getValue()));
	costLabel->setPosition(Vec2( 5*itemWidth/2, ITEM_POSY));
	addChild(costLabel);


	Button* _btnCancel = Button::create();
	_btnCancel->loadTextureNormal("btn_3.png",ui::Widget::TextureResType::PLIST);
	_btnCancel->setPosition(Vec2(  7*itemWidth/2, ITEM_POSY));
	_btnCancel->setTitleText(CommonFunction::GBKToUTF8("撤销拍卖"));
	_btnCancel->setScale(0.8f);
	_btnCancel->setTitleFontSize(30);
	addChild(_btnCancel);
	_btnCancel->addTouchEventListener(CC_CALLBACK_2(GameHallMyPaiamiItem::onClickCancel, this));


	return true;

}

void GameHallMyPaiamiItem::onClickCancel(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendCancelAuctionRequest(mData);

	}
}

//////////////////////////////////////////////////////////////////////////
GameHallPaimaiHistoryItem::GameHallPaimaiHistoryItem()
{
}

GameHallPaimaiHistoryItem::~GameHallPaimaiHistoryItem()
{

}

GameHallPaimaiHistoryItem* GameHallPaimaiHistoryItem::create(__Dictionary* infodic,int width)
{
	GameHallPaimaiHistoryItem* pItem = new GameHallPaimaiHistoryItem();
	if (pItem && pItem->init(infodic,width))
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

bool GameHallPaimaiHistoryItem::init(__Dictionary* infodic,int width)
{
	if (!Layout::init())
	{
		return false;
	}

	Vec2 pos = Vec2(0,0);
	setContentSize(Size(width,ITEM_HEIGHT));
	int itemWidth = width / 6;

	ImageView* line = ImageView::create("image_line.png",ui::Widget::TextureResType::PLIST);
	line->setScaleX(width*2/line->getSize().width);
	line->setPosition(Vec2(itemWidth/2 -20, getContentSize().height-5));
	addChild(line);

	//物品名称
	__String* name = (__String*)infodic->objectForKey("auctionName");
	Text* nameLable = Text::create();
	nameLable->setAnchorPoint(ccp(0.5, 0.5));
	nameLable->setFontSize(25);
	nameLable->setTextColor(ccc4(96,33,0,255));
	nameLable->setText(CommonFunction::GBKToUTF8(name->getCString()));
	int posx = itemWidth/2 - 20;
	if (itemWidth/2 - 20 <= nameLable->getContentSize().width/2)
	{
		posx = nameLable->getContentSize().width/2;
	}
	nameLable->setPosition(Vec2(posx,ITEM_POSY));
	addChild(nameLable);

	//拍卖类型
	__Integer* state = (__Integer*)infodic->objectForKey("type");
	Text* stateLable = Text::create();
	stateLable->setAnchorPoint(ccp(0.5, 0.5));
	stateLable->setFontSize(25);
	stateLable->setTextColor(ccc4(184,43,34,255));
	if(state->getValue() == 1)
		stateLable->setText(CommonFunction::GBKToUTF8("买 入"));
	else if(state->getValue() == 2)
		stateLable->setText(CommonFunction::GBKToUTF8("卖 出"));

	stateLable->setPosition(Vec2(15+ 3*itemWidth/2,ITEM_POSY));
	addChild(stateLable);


	//对象拍卖者的昵称
	__String*  nick= (__String*)infodic->objectForKey("nickName");
	std::string trun_str = CommonFunction::truncationString(CommonFunction::GBKToUTF8(nick->getCString()),12);
	Text* nickLable = Text::create();
	nickLable->setAnchorPoint(ccp(0.5, 0.5));
	nickLable->setFontSize(25);
	nickLable->setTextColor(ccc4(96,33,0,255));
	nickLable->setText(trun_str);
	nickLable->setPosition(Vec2(10+ 5*itemWidth/2, ITEM_POSY));
	addChild(nickLable);

	//时间
	__String*  date= (__String*)infodic->objectForKey("date");
	Text* dateLable = Text::create();
	dateLable->setAnchorPoint(ccp(0.5, 0.5));
	dateLable->setFontSize(25);
	dateLable->setTextColor(ccc4(96,33,0,255));
	dateLable->setText(date->getCString());
	dateLable->setPosition(Vec2(30+7*itemWidth/2,ITEM_POSY +dateLable->getContentSize().height/2));
	addChild(dateLable);

	//时分秒
	__String*  time= (__String*)infodic->objectForKey("time");
	Text* timeLable = Text::create();
	timeLable->setAnchorPoint(ccp(0.5, 0.5));
	timeLable->setFontSize(25);
	timeLable->setTextColor(ccc4(96,33,0,255));
	timeLable->setText(time->getCString());
	timeLable->setPosition(Vec2(30+7*itemWidth/2, ITEM_POSY- timeLable->getContentSize().height/2));
	addChild(timeLable);

	

	//拍卖数量
	__Integer* count = (__Integer*)infodic->objectForKey("propNum");
	Text* countLable = Text::create();
	countLable->setAnchorPoint(ccp(0.5, 0.5));
	countLable->setFontSize(25);
	countLable->setTextColor(ccc4(96,33,0,255));
	countLable->setText(numberToString(count->getValue()));
	countLable->setPosition(Vec2(20+ 9*itemWidth/2, ITEM_POSY));
	addChild(countLable);

	//交易额
	__Integer* cost = (__Integer*)infodic->objectForKey("cost");
	Text* costLable = Text::create();
	costLable->setAnchorPoint(ccp(0.5, 0.5));
	costLable->setFontSize(25);
	costLable->setTextColor(ccc4(96,33,0,255));
	costLable->setText(numberToString(cost->getValue()));
	costLable->setPosition(Vec2( 20+11*itemWidth/2, ITEM_POSY));
	addChild(costLable);
	return true;

}
//////////////////////////////////////////////////////////////////////////
//sellitem

GameHallPaimaiSellItem::GameHallPaimaiSellItem()
{
	mData = nullptr;
	mbSelect = false;
}


GameHallPaimaiSellItem::~GameHallPaimaiSellItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallPaimaiSellItem* GameHallPaimaiSellItem::create(__Dictionary* infodic,int width)
{
	GameHallPaimaiSellItem* pItem = new GameHallPaimaiSellItem();
	if (pItem && pItem->init(infodic,width))
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

bool GameHallPaimaiSellItem::init(__Dictionary* infodic,int width)
{
	if (!Layout::init())
	{
		return false;
	}
	mData = infodic;
	mData->retain();
	setTouchEnabled(true);
	setContentSize(Size(width,SELL_ITEM_HEIGHT));
	//图标
	WebImageView* icon = WebImageView::create();
	icon->setPosition(Vec2(34/2 + 5,getContentSize().height/2));
	__String* url = (__String*)mData->objectForKey("propUrl");
	icon->setLoadingAnimation(true);
	icon->imageWithUrl(__String::createWithFormat(ImageURLFormat,url->getCString())->getCString());
	icon->setScale(0.5f);
	addChild(icon);
	
	mSelectLayer = LayerColor::create(ccc4(55,9,0,125));
	mSelectLayer->setContentSize(getContentSize());
	mSelectLayer->setVisible(false);
	addChild(mSelectLayer);

	

	//数量
	__String* numberStr = __String::createWithFormat("x%d",((__Integer*)mData->objectForKey("propCnt"))->getValue());

	mCountLable = Text::create();
	mCountLable->setAnchorPoint(ccp(0, 0.5));
	mCountLable->setFontSize(20);
	mCountLable->setTextColor(ccc4(96,33,0,255));
	int count = ((__Integer*)mData->objectForKey("propCnt"))->getValue();
	if (count < 99)
	{
		mCountLable->setText(numberStr->getCString());
	}
	else
	{
		mCountLable->setText(__String::create("x99+")->getCString());
	}
	
	mCountLable->setPosition(Vec2(icon->getPositionX()+20, getContentSize().height/2));
	addChild(mCountLable);

	//物品名称
	__String* nameStr = (__String*)mData->objectForKey("propName");
	mNameLable = Text::create();
	mNameLable->setAnchorPoint(ccp(0.5, 0.5));
	mNameLable->setFontSize(20);
	mNameLable->setTextColor(ccc4(96,33,0,255));
	mNameLable->setText(CommonFunction::GBKToUTF8(nameStr->getCString()));
	mNameLable->setPosition(Vec2(getContentSize().width - mNameLable->getContentSize().width/2 - 10, getContentSize().height/2));
	addChild(mNameLable);


	ImageView* line = ImageView::create("image_line.png",ui::Widget::TextureResType::PLIST);
	line->setScaleX(width*2/line->getSize().width);
	line->setPosition(Vec2(width/2, 0));
	addChild(line);

	return true;
}

void GameHallPaimaiSellItem::setSelect()
{
	mbSelect = true;
	mNameLable->setTextColor(ccc4(255,255,255,255));
	mCountLable->setTextColor(ccc4(255,255,255,255));
	if(!mSelectLayer->isVisible())
		mSelectLayer->setVisible(true);
}

void GameHallPaimaiSellItem::unSelect()
{
	mbSelect = false;
	mNameLable->setTextColor(ccc4(96,33,0,255));
	mCountLable->setTextColor(ccc4(96,33,0,255));
	if(mSelectLayer->isVisible())
		mSelectLayer->setVisible(false);
}

void GameHallPaimaiSellItem::refreshData(int cnt)
{
	if(cnt < 99)
	{
		__String* numberStr = __String::createWithFormat("x%d",cnt);
		mCountLable->setText(numberStr->getCString());
	}
	else
	{
		__String* numberStr = __String::createWithFormat("x%d+",99);
		mCountLable->setText(numberStr->getCString());
	}
	
	mData->setObject(__Integer::create(cnt),"propCnt");

}

