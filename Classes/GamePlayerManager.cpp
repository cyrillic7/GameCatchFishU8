#include "GamePlayerManager.h"
#include "GameScene.h"
#include "SessionManager.h"

GamePlayer::GamePlayer()
{
	mShooter = nullptr;
	mChairId = -1;
	mBulletKind = BULLET_2_NORMAL;
	mTurrentAngle = 0;
	mBulletMulriple = 1;//默认最小为1倍
	m_bInitUI = false;
}

GamePlayer::~GamePlayer()
{
	CC_SAFE_RELEASE_NULL(mShooter);
}

GamePlayer* GamePlayer::create()
{
	GamePlayer* player = new GamePlayer();
	if(player && player->init())
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool GamePlayer::init(){
	return true;
}

void GamePlayer::setShooter(GameShooter* pShooter)
{
	mShooter = pShooter;
	mShooter->retain();
}

void GamePlayer::setAngle(float angle)
{
	mTurrentAngle = angle;
	mShooter->setAngle(CC_RADIANS_TO_DEGREES(angle));
}

float GamePlayer::getAngle()
{
	return mTurrentAngle;
}

void GamePlayer::fire()
{
	if(mShooter)
		mShooter->fire();
}

void GamePlayer::exchangeScore(SCORE score)
{
	if(mShooter)
		mShooter->setScore(score);
}

void GamePlayer::setBulletMulriple(int power )
{
	mBulletMulriple = power;
	mShooter->setBulletMulriple(power);
}

int GamePlayer::getBulletMulriple()
{
	return mBulletMulriple;
}

void GamePlayer::AddScore(SCORE score,bool coin_effect,bool bingo_effect)
{
	mShooter->addScore(score,coin_effect,bingo_effect);
}

void GamePlayer::switchGun(BulletKind kind)
{
	mShooter->switchGun(kind);
}
//////////////////////////////////////////////////////////////////////////

GamePlayerManager::GamePlayerManager()
{
	mGameScene = nullptr;
	mPlayers = __Array::create();
	CC_SAFE_RETAIN(mPlayers);
}

GamePlayerManager::~GamePlayerManager()
{
	CC_SAFE_RELEASE_NULL(mPlayers);
}

GamePlayerManager* GamePlayerManager::create(GameScene* pScene)
{
	GamePlayerManager* pManager = new GamePlayerManager();
	if (pManager && pManager->init(pScene))
	{
		pManager->autorelease();
		return pManager;
	}
	CC_SAFE_DELETE(pManager);
	return nullptr;
}

bool GamePlayerManager::init(GameScene* pScene)
{
	mGameScene = pScene;
	return true;
}

void GamePlayerManager::addPlayer(int chairId)
{
	if (!playerIsExist(chairId))
	{
		GamePlayer* player = GamePlayer::create();
		player->setChairID(chairId);
		mPlayers->addObject(player); 

		if (SessionManager::shareInstance()->getChairId() != -1)
		{
			player->setInitUIFalg(true);
			//初始化界面UI
			mGameScene->initPlayer(player);
		}
		
	}
}

bool GamePlayerManager::playerIsExist(int chairId)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getChairID() == chairId)
		{
			return true;
		}
	}
	return false;
}

void GamePlayerManager::removePlayer(int chairId)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getChairID() == chairId)
		{
			mGameScene->removePlayer(player);
			//从玩家数组中移除
			mPlayers->removeObjectAtIndex(i);	
		}
	}
}

void GamePlayerManager::fire(int chairId)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getChairID() == chairId)
		{
			player->fire();
		}
	}
}

GamePlayer* GamePlayerManager::getPlayerByChairId(int chairId)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getChairID() == chairId)
		{
			return player;
		}
	}
	return nullptr;
}

void GamePlayerManager::exchangeScore(int chairId ,SCORE score)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getChairID() == chairId)
		{
			player->exchangeScore(score);
		}
	}
}

void GamePlayerManager::AddScore(int chairId, SCORE score, bool coin_effect, bool bingo_effect)
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		
		if (player->getChairID() == chairId && player->getShooter())
		{
			player->AddScore(score,coin_effect,bingo_effect);
		}
	}
}

void GamePlayerManager::isAllPalyersIsOnSit()
{
	for (int i = 0; i < mPlayers->count(); i++)
	{
		GamePlayer* player = (GamePlayer*)mPlayers->getObjectAtIndex(i);
		if (player->getInitUIFalg() == false)
		{
			log("isAllPalyersIsOnSit chair id = %d",player->getChairID());
			//初始化界面UI
			mGameScene->initPlayer(player);
			player->setInitUIFalg(true);
		}
	}
}

