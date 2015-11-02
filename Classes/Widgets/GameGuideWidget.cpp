#include "GameGuideWidget.h"
#include "Common/TNVisibleRect.h"
#include "SessionManager.h"
#include "GameResourcesManager.h"
#include "CommonFunction.h"

#define  ImagePopTag 18
#define  ListViewTag 19
#define  ListItemTag 11953

#define  colTotalCnt 5


GameGuideItem::GameGuideItem()
{

}

GameGuideItem::~GameGuideItem()
{

}

GameGuideItem* GameGuideItem::create(GuideItemModel* info)
{
	GameGuideItem* pitem = new GameGuideItem();
	if (pitem&&pitem->init(info))
	{
		pitem->autorelease();
		return pitem;
	}
	delete pitem;
	pitem = nullptr;
	return pitem;
}

bool GameGuideItem::init(GuideItemModel* info)
{
	if (!Layout::init())
		return false;
	
	ImageView* itemBg = ImageView::create();
	itemBg->loadTexture("guide_item_bg.png",ui::Widget::TextureResType::PLIST);
	addChild(itemBg);

	/*ImageView* fish = ImageView::create();
	String* name = __String::createWithFormat("%s.png",info->getImageName().c_str());
	fish->loadTexture(name->getCString(),ui::Widget::TextureResType::PLIST);
	fish->setAnchorPoint(Vec2(0.5,0.5));
	fish->setPosition(Vec2(itemBg->getContentSize().width/2,itemBg->getContentSize().height/2));
	itemBg->addChild(fish);*/

	return true;
}

GameGuideWidget::GameGuideWidget()
{
}

GameGuideWidget::~GameGuideWidget()
{

}

std::string GameGuideWidget::getWidgetJsonName()
{
	return "guideWidget";
}

void GameGuideWidget::onExit()
{
	TNWidget::onExit();
}

void GameGuideWidget::onEnter()
{
	TNWidget::onEnter();
}


bool GameGuideWidget::init( )
{
	if (!TNWidget::init())
	{
		return false;
	}

	return true;
}
void GameGuideWidget::loadUI()
{

	TNWidget::loadUI();

	//绑定相关事件
	ImageView* bg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));
	
	ListView*   pList = static_cast<ListView*>(bg->getChildByTag(ListViewTag));

	Layout*    PLayerItem = static_cast<Layout*>(pList->getChildByTag(ListItemTag));
	pList->setItemModel(PLayerItem);
	pList->removeAllItems();
	pList->insertDefaultItem(0);

	int itemCount = 0;
	Layout* lineLayout = nullptr;
	__Array* guideInfos = ResourcesManager::getInstance()->getGuideInfo();
	while (itemCount < guideInfos->count())
	{
		if (itemCount < colTotalCnt)
		{
			lineLayout = (Layout*)((Vector<Widget*>)pList->getItems()).at(0);
		}
		else
		{
			if (itemCount % colTotalCnt == 0 )
			{
				pList->insertDefaultItem(pList->getItems().size());
				int cnt = pList->getItems().size();
				lineLayout = (Layout*)((Vector<Widget*>)pList->getItems()).at(cnt-1);
			}
		}
		for ( int i =0 ; i < colTotalCnt; i ++)
		{
			ImageView* itemBg = ImageView::create();
			itemBg->loadTexture("guide_item_bg.png",ui::Widget::TextureResType::PLIST);
			itemBg->setPosition(Vec2(itemBg->getContentSize().width/2+i*itemBg->getContentSize().width+(i+1)*12,lineLayout->getContentSize().height/2));
			lineLayout->addChild(itemBg);

			ImageView* fish = ImageView::create();
			GuideItemModel* info  = (GuideItemModel*)guideInfos->getObjectAtIndex(itemCount);
			String* name = __String::createWithFormat("%s.png",info->getImageName().c_str());
			fish->loadTexture(name->getCString(),ui::Widget::TextureResType::PLIST);
			fish->setAnchorPoint(Vec2(0.5,0.5));
			fish->setPosition(Vec2(itemBg->getContentSize().width/2,itemBg->getContentSize().height/2));
			itemBg->addChild(fish);

			Text*  itemTitle = Text::create();
			itemTitle->setText(info->getTitle().c_str());
			itemTitle->setFontSize(24);
			itemTitle->setTextColor(ccc4(255,255,255,255));
			itemTitle->setAnchorPoint(Vec2(0.5, 0.5));
			itemTitle->setPosition(Vec2(itemBg->getContentSize().width/2, itemBg->getContentSize().height - itemTitle->getContentSize().height/2 -5));
			itemBg->addChild(itemTitle);

			Text*  itemPower = Text::create();
			if(info->getPower() !=0)
				itemPower->setText(__String::createWithFormat("%d%s",info->getPower(),CommonFunction::GBKToUTF8("倍").c_str())->getCString());
			else
				itemPower->setText(info->getItemDesc().c_str());
			itemPower->setFontSize(30);
			itemPower->setTextColor(ccc4(255,255,255,255));
			itemPower->setAnchorPoint(Vec2(0.5, 0.5));
			itemPower->setPosition(Vec2(itemBg->getContentSize().width/2,  itemTitle->getContentSize().height/2 + 5));
			itemBg->addChild(itemPower);

			/*GameGuideItem* pitem = GameGuideItem::create((GuideItemModel*)guideInfos->getObjectAtIndex(itemCount));
			pitem->setContentSize(Size(107,160));
			pitem->setPosition(Vec2(pitem->getContentSize().width/2+i*pitem->getContentSize().width+(i+1)*12,lineLayout->getContentSize().height/2));
			lineLayout->addChild(pitem);*/

			itemCount++;
			//当前行，列数不够，退出
			if (itemCount >= guideInfos->count() )
					break;;
		}
		
	}

}