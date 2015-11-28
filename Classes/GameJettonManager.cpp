#include "GameJettonManager.h"
#include "SessionManager.h"
#include "Message.h"

static GameJettonManager* __instance = nullptr;
// 筹码位置
//Position kStackPos[GAME_PLAYER] = {
//	{ 396.5f, 728.f }, { 827.5f, 728.f }, { 1258.5f, 728.f },
//	{ 969.5f, 26.f }, { 538.5f, 26.f }, { 107.5f, 26.f }
//};

Position kStackPos[GAME_PLAYER] = {
	{ 396.5f, 748.f }, { 827.5f, 748.f }, { 1258.5f, 748.f },
	{ 969.5f, 6.f }, { 538.5f, 6.f }, { 107.5f, 6.f }
};

const int kMaxShowJettonCount = 3;
const DWORD kJettonMoveInterval = 2.0;
const float kJettonMoveOffset = 1.0f ;//8.0f;


GameJettonManager::GameJettonManager()
{
	mJetton = Sprite::create("common/players/jetton.png");
	mJetton->retain();

	for (int i= 0 ; i < GAME_PLAYER; i++)
	{
		jettonArray[i] = __Array::createWithCapacity(4);
		jettonArray[i]->retain();
	}
}

GameJettonManager::~GameJettonManager()
{
	CC_SAFE_RELEASE_NULL(mJetton);
	for (int i= 0 ; i < GAME_PLAYER; i++)
	{
		CC_SAFE_RELEASE_NULL(jettonArray[i]);
	}
}

GameJettonManager* GameJettonManager::shareInstance()
{
	if (__instance == nullptr)
	{
		__instance = new GameJettonManager();
	}
	return __instance;
}

void GameJettonManager::purgeInstance()
{
	if (__instance)
	{
		delete __instance;
		__instance = nullptr;
	}
}

void GameJettonManager::Update(float delta_time)
{
	DWORD now_tick = SessionManager::getTimeStamp();
	Vec2 pos;
	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		if (jettonArray[i]->count() == 0)
			continue;
		//取最前面一个JETTON，判断是否该消失
		GameJettonWidget* pNode = (GameJettonWidget*)jettonArray[i]->getObjectAtIndex(0);
		//pNode->Render();
		int curTick = pNode->getTick() + kJettonMoveInterval;
		if (curTick < now_tick) {
			jettonArray[i]->removeObjectAtIndex(0,false);
			pNode->removeFromParent();
			if (jettonArray[i]->count() == 0)
				continue;
			pNode = (GameJettonWidget*)jettonArray[i]->getObjectAtIndex(0);
			pNode->addTick(now_tick);
			for (int j = 0 ; j < jettonArray[i]->count();j++)
			{
				pNode = (GameJettonWidget*)jettonArray[i]->getObjectAtIndex(j);
				pNode->setMovFlag(true);
			
			}
		}	
		size_t size = 0;
		Vec2  curPos;
		for(int k  = 0; k < jettonArray[i]->count(); k++) {
			pNode = (GameJettonWidget*)jettonArray[i]->getObjectAtIndex(k);
			pos = GetJettonPos(i, size++);
			curPos.y = pNode->getPositionY();
			if (pNode->getMovFlag()) {	
				if (i < 3) {
					curPos.x = pNode->getPositionX() - kJettonMoveOffset;
					if (curPos.x <= pos.x) {
						curPos.x = pos.x;
						pNode->setMovFlag(false);
					}
					pNode->setPosition(curPos);
				} else {
					curPos.x = pNode->getPositionX() + kJettonMoveOffset;
					if (curPos.x <= pos.x) {
						curPos.x = pos.x;
						pNode->setMovFlag(false);
					}
					pNode->setPosition(curPos);	
				}
			}
		}
	}
}

void GameJettonManager::Render()
{
	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		if (jettonArray[i]->count() == 0)
			continue;
		for(int k  = 0; k < jettonArray[i]->count(); k++) {
			GameJettonWidget* pNode = (GameJettonWidget*)jettonArray[i]->getObjectAtIndex(k);
			pNode->Render();
		}
	}
}

void GameJettonManager::addJetton(int view_chair_id,SCORE score)
{
	if (view_chair_id >= GAME_PLAYER)
		return;
	if (score <= 0) return;

	Vec2 jettonPos = GetJettonPos(view_chair_id,jettonArray[view_chair_id]->count());
	int index = 0;
	int cnt = jettonArray[view_chair_id]->count();
	if (cnt  == 0)
	{
		index = 0;
	}
	else
	{
		//获取数组中最后一个元素的索引
		GameJettonWidget* pWidget = (GameJettonWidget*)jettonArray[view_chair_id]->getObjectAtIndex(cnt-1);
		index = (pWidget->getIndex() + 1)%2;
	}

	GameJettonWidget* pNode = GameJettonWidget::create(score,index);
	if (cnt == 0)
	{
		pNode->addTick(SessionManager::shareInstance()->getTimeStamp());
	}
	pNode->setPosition(jettonPos);

	if (view_chair_id < 3)
	{
		pNode->setRotation(180);
	}

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(DealJettonEffectMsg,pNode);
	//通知到游戏场景

	jettonArray[view_chair_id]->addObject(pNode);
	if (jettonArray[view_chair_id]->count() > kMaxShowJettonCount) {
		pNode = (GameJettonWidget*)jettonArray[view_chair_id]->getObjectAtIndex(0);
		jettonArray[view_chair_id]->removeObjectAtIndex(0,false);
		pNode->removeFromParent();
		//取出第一个元素
		pNode = (GameJettonWidget*)jettonArray[view_chair_id]->getObjectAtIndex(0);
		pNode->addTick(SessionManager::shareInstance()->getTimeStamp());
		for (int i = 0 ; i < jettonArray[view_chair_id]->count();i++)
		{
			pNode = (GameJettonWidget*)jettonArray[view_chair_id]->getObjectAtIndex(i);
			pNode->setMovFlag(true);	
		}
	}
	

}

Vec2 GameJettonManager::GetJettonPos(WORD chair_id, size_t size) {
	Vec2 jetton_pos;
	if (chair_id >= GAME_PLAYER)
		return jetton_pos;

	if (chair_id <= 2) {
		jetton_pos.x = kStackPos[chair_id].x + mJetton->getContentSize().width* size;
		jetton_pos.y = kStackPos[chair_id].y;
	} else {
		jetton_pos.x = kStackPos[chair_id].x - mJetton->getContentSize().width * size;
		jetton_pos.y = kStackPos[chair_id].y;
	}

	return jetton_pos;
}



