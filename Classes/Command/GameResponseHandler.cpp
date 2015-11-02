#include"GameResponseHandler.h"
#include "SessionManager.h"
#include "CommonFunction.h"

bool GameResponseHandler::init()
{
	//游戏内的消息
	registerResponseHandler(SUB_S_EXCHANGE_FISHSCORE,responsehandler_selector(GameResponseHandler::OnSubExchangeFishScore));
	registerResponseHandler(SUB_S_USER_FIRE,responsehandler_selector(GameResponseHandler::OnSubUserFire));
	registerResponseHandler(SUB_S_DISTRIBUTE_FISH,responsehandler_selector(GameResponseHandler::OnSubDistributeFish));
	registerResponseHandler(SUB_S_CATCH_FISH_GROUP,responsehandler_selector(GameResponseHandler::OnSubCatchFishGroup));
	registerResponseHandler(SUB_S_BULLET_DOUBLE_TIMEOUT,responsehandler_selector(GameResponseHandler::OnSubBulletDoubleTimeout));
	registerResponseHandler(SUB_S_SWITCH_SCENE,responsehandler_selector(GameResponseHandler::OnSubSwitchScene));
	registerResponseHandler(SUB_S_CATCH_CHAIN,responsehandler_selector(GameResponseHandler::OnSubCatchChain));
	registerResponseHandler(SUB_S_SCENE_FISH,responsehandler_selector(GameResponseHandler::OnSubSceneFish));
	registerResponseHandler(SUB_S_SCENE_BULLETS,responsehandler_selector(GameResponseHandler::OnSubSceneBullets));
	registerResponseHandler(SUB_S_FORCE_TIMER_SYNC,responsehandler_selector(GameResponseHandler::OnSubForceTimerSync));
	registerResponseHandler(SUB_S_TIMER_SYNC,responsehandler_selector(GameResponseHandler::OnSubTimerSync));
	registerResponseHandler(SUB_S_STOCK_OPERATE_RESULT,responsehandler_selector(GameResponseHandler::OnSubStockOperateResult));
	registerResponseHandler(SUB_S_ADMIN_CONTROL,responsehandler_selector(GameResponseHandler::OnSubAdminControl));
	registerResponseHandler(SUB_S_HIT_FISH_LK,responsehandler_selector(GameResponseHandler::OnSubModifyLkMulriple));

	//registerResponseHandler(SUB_GF_GAME_SCENE,responsehandler_selector(GameResponseHandler::OnReceiveGameSceneMsg));
	registerResponseHandler(SUB_GF_GAME_STATUS,responsehandler_selector(GameResponseHandler::OnReceiveGameStatusMsg));
	registerResponseHandler(SUB_GF_SYSTEM_MESSAGE,responsehandler_selector(GameResponseHandler::OnReceiveSystemMsg));
	registerResponseHandler(SUB_GF_LOOKON_STATUS,responsehandler_selector(GameResponseHandler::OnReceivePlayerToSperator));
	registerResponseHandler(SUB_C_EXCHANGE_FISHSCORE,responsehandler_selector(GameResponseHandler::OnReceiveExchangeScoreMsg));
	
	return true;
}

void GameResponseHandler::OnSubExchangeFishScore(void*data,WORD data_size)
{
	if (data_size != sizeof(CMD_S_ExchangeFishScore))
	{
		CCLOG("ExchangeFishScore data is error");
		return ;
	}
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ExchangeFishScoreMsg,data);

}


void GameResponseHandler::OnSubUserFire(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_UserFire))
	{
		CCLOG("UserFire data is error");
		return ;
	}
	CMD_S_UserFire* fireData = (CMD_S_UserFire*)data;
	__Dictionary * dic = __Dictionary::create();
	dic->setObject(__Integer::create(fireData->tick_count),"tick");
	dic->setObject(__Integer::create(fireData->chair_id),"chairId");
	dic->setObject(__Integer::create(fireData->bullet_id),"bulletId");
	dic->setObject(__Float::create(fireData->angle),"angle");
	dic->setObject(__Bool::create(fireData->bullet_double),"isDouble");
	dic->setObject(__Integer::create(fireData->bullet_mulriple),"mulriple");
	dic->setObject(__Integer::create(fireData->lock_fish_id),"targetId");


	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(UserFireMsg,dic);
}

void GameResponseHandler::OnSubDistributeFish(void* data, WORD data_size)
{
	assert(data_size % sizeof(CMD_S_DistributeFish) == 0);
	if (data_size % sizeof(CMD_S_DistributeFish) != 0)
	{
		CCLOG("DistributeFish data is error");
		return ;
	}
	__Array* fishArray = __Array::create();
	CMD_S_DistributeFish* distribute_fish = static_cast<CMD_S_DistributeFish*>(data);
	WORD distribute_fish_count = data_size / sizeof(CMD_S_DistributeFish);
	for (int i=0;i<distribute_fish_count;i++)
	{
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Integer::create(distribute_fish->fish_kind),"fishKind");
		dic->setObject(__Integer::create(distribute_fish->fish_id),"fishId");
		dic->setObject(__Integer::create(distribute_fish->tag),"tag");
		dic->setObject(__Integer::create(distribute_fish->tick_count),"tick");
		dic->setObject(__Integer::create(distribute_fish->position_count),"ptCnt");

		PointArray* ptArray = PointArray::create(distribute_fish->position_count);
		for (int j=0;j<distribute_fish->position_count;j++)
		{
			if(SessionManager::shareInstance()->getChairId() < 3)
			{
				ptArray->addControlPoint(Vec2(kScreenWidth - distribute_fish->position[j].x,distribute_fish->position[j].y));
			}
			else
			{
				ptArray->addControlPoint(Vec2(distribute_fish->position[j].x  ,kScreenHeight - distribute_fish->position[j].y));
			}
		}
		dic->setObject(ptArray,"ptArray");
		fishArray->addObject(dic);
		distribute_fish++;
	}

	if(fishArray->count()>0)
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(CreateNewFishMsg,fishArray);


}

void GameResponseHandler::OnSubCatchFishGroup(void* data, WORD data_size)
{
	if (data_size % sizeof(CMD_S_CatchFishGroup) != 0)
	{
		log("catchfish group data is error");
		return ;
	}
	CMD_S_CatchFishGroup* catch_fish_group = static_cast<CMD_S_CatchFishGroup*>(data);
	int total_fish_count = data_size/sizeof(CMD_S_CatchFishGroup);
	__Array* catchFishArray = __Array::create();
	for (int i =0;i< total_fish_count;i++)
	{
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Integer::create(catch_fish_group->bullet_id),"bulletId");
		dic->setObject(__Integer::create(catch_fish_group->chair_id),"chairId");
		dic->setObject(__Integer::create(catch_fish_group->tick_count),"tick");
		int _fishCnt = catch_fish_group->fish_count;
		dic->setObject(__Integer::create(_fishCnt),"fishCnt");
		if (_fishCnt > 0)
		{
			__Array* fishArray = __Array::create();
			CatchFish* catchFish = catch_fish_group->catch_fish;
			for (int k =0; k< _fishCnt;k++)
			{
				__Dictionary* fishDic = __Dictionary::create();
				fishDic->setObject(__Integer::create(catchFish->fish_id),"fishId");
				fishDic->setObject(__Integer::create(catchFish->fish_kind),"fishKind");
				log("111 %I64d,2222 = %lld",catchFish->fish_score,catchFish->fish_score);
				fishDic->setObject(__String::createWithFormat("%lld",catchFish->fish_score),"fishScore");
				fishDic->setObject(__Integer::create(catchFish->link_fish_id),"linkFishId");
				fishDic->setObject(__Bool::create(catchFish->bullet_double),"isDouble");
				catchFish++;
				fishArray->addObject(fishDic);
			}
			dic->setObject(fishArray,"catchFish");
		}
		catch_fish_group++;
		catchFishArray->addObject(dic);
	}

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(CatchFishGroupMsg,catchFishArray);
}

void GameResponseHandler::OnSubBulletDoubleTimeout(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_BulletDoubleTimeout))
	{
		CCLOG("double time out data is error");
		return ;
	}
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(BulletDoubleTimeoutMsg,data);
}

void GameResponseHandler::OnSubSwitchScene(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_SwitchScene))\
	{
		log("switch scene data is error");
		return ;
	}

	//发送消息
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ChangeSceneMsg,data);

}
void GameResponseHandler::OnSubCatchChain(void* data, WORD data_size)
{
	if (data_size % sizeof(CMD_S_CatchChain) != 0)
	{
		CCLOG("fish chain  data is erro");
		return;
	}
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(CatchChainMsg,data);

}

void GameResponseHandler::OnSubSceneFish(void* data, WORD data_size)
{
	if (data_size % sizeof(CMD_S_SceneFish) != 0)
	{
		CCLOG("scene fish data is erro");
		return;
	}
	__Array* fishArray = __Array::create();
	CMD_S_SceneFish* scene_fish = static_cast<CMD_S_SceneFish*>(data);
	WORD fish_count = data_size / sizeof(CMD_S_SceneFish);

	for (int i=0; i < fish_count;i++)
	{
		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Integer::create(scene_fish->fish_kind),"fishKind");
		dic->setObject(__Integer::create(scene_fish->fish_id),"fishId");
		dic->setObject(__Integer::create(scene_fish->tag),"tag");
		dic->setObject(__Integer::create(scene_fish->tick_count),"tick");
		dic->setObject(__Integer::create(scene_fish->elapsed),"elapsed");
		dic->setObject(__Integer::create(scene_fish->position_count),"ptCnt");

		PointArray* ptArray = PointArray::create(scene_fish->position_count);
		for (int j=0;j<scene_fish->position_count;j++)
		{
			if(SessionManager::shareInstance()->getChairId() < 3)
			{
				ptArray->addControlPoint(Vec2(kScreenWidth - scene_fish->position[j].x,scene_fish->position[j].y));
			}
			else
			{
				ptArray->addControlPoint(Vec2(scene_fish->position[j].x  ,kScreenHeight - scene_fish->position[j].y));
			}
		}
		dic->setObject(ptArray,"ptArray");
		fishArray->addObject(dic);

		scene_fish++;
	}


	if (fishArray->count() > 0)
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(SubSceneFishMsg,fishArray);
	
	

}
void GameResponseHandler::OnSubSceneBullets(void* data, WORD data_size)
{

}
void GameResponseHandler::OnSubForceTimerSync(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_ForceTimerSync))
	{
		CCLOG("force time sync data is error");
		return;
	}
	CMD_S_ForceTimerSync* timer_sync = static_cast<CMD_S_ForceTimerSync*>(data);
	if (SessionManager::shareInstance()->getChairId() != timer_sync->chair_id)
		return ;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ForceTimerSyncMsg,data);

}
void GameResponseHandler::OnSubTimerSync(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_TimerSync))
	{
		CCLOG("timesync data is error");
		return ;
	}
	CMD_S_TimerSync* timer_sync = static_cast<CMD_S_TimerSync*>(data);
	//只处理自己的时间同步消息
	if (SessionManager::shareInstance()->getChairId() == timer_sync->chair_id)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(TimerSyncMsg,data);
	}
}

void GameResponseHandler::OnSubStockOperateResult(void* data, WORD data_size)
{

}

void GameResponseHandler::OnSubAdminControl(void* data, WORD data_size)
{

}


void GameResponseHandler::OnReceiveGameSceneMsg(void* data, WORD data_size)//游戏场景
{
#if 0
	CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(data);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GameSceneMsg,data);
#else
	log("GameResponseHandler::OnReceiveGameSceneMsg data_size = %d",data_size);
	// 计算鱼种类数量
	int nSubLen = data_size-sizeof(bool)-sizeof(float)-sizeof(unsigned int)-sizeof(SCORE)*GAME_PLAYER*2-sizeof(int)*5-sizeof(float)*BULLET_KIND_COUNT*2;
	if (nSubLen%(3*sizeof(float)) != 0 || nSubLen/(3*sizeof(float)) < 1)
	{
		CCLOG("GameScene data is error");
		return;
	}
	int nFishCount = nSubLen/(3*sizeof(float));
	CMD_S_GameStatus* pOrgGamestatus = static_cast<CMD_S_GameStatus*>(data);
	CMD_S_GameStatus gamestatus;
	memset(&gamestatus, 0, sizeof(CMD_S_GameStatus));
	memcpy(&gamestatus, data, data_size);
	memcpy(gamestatus.game_config.fish_bounding_radius, pOrgGamestatus->game_config.fish_speed+nFishCount, nSubLen/3);
	memcpy(gamestatus.game_config.fish_bounding_count, pOrgGamestatus->game_config.fish_speed+nFishCount*2, nSubLen/3);
	memcpy(gamestatus.game_config.bullet_speed, pOrgGamestatus->game_config.fish_speed+nFishCount*3, sizeof(float)*BULLET_KIND_COUNT*2);
	memcpy(gamestatus.fish_score, pOrgGamestatus->game_config.fish_speed+nFishCount*3+BULLET_KIND_COUNT*2, sizeof(SCORE)*GAME_PLAYER*2);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GameSceneMsg,&gamestatus);
#endif
	return;
}

void GameResponseHandler::OnReceiveGameStatusMsg(void* data, WORD data_size)//
{
	assert(data_size == sizeof(CMD_GF_GameStatus));
	if (data_size != sizeof(CMD_GF_GameStatus))
	{
		CCLOG("GameStatus data is error");
		return ;
	}
	CMD_GF_GameStatus* game_status = (CMD_GF_GameStatus*)data;
	BYTE status = game_status->cbGameStatus;
	int isLookOn = !game_status->cbAllowLookon ? false : true;

	__Dictionary* dic = __Dictionary::create();
	dic->setObject(__Integer::create(status),"gameStatus");
	dic->setObject(__Bool::create(isLookOn),"isLookOn");

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GameStatusMsg,dic);

}

void GameResponseHandler::OnReceiveSystemMsg(void* data, WORD data_size)//收到系统消息
{
	CMD_CM_SystemMessage* system_msg = (CMD_CM_SystemMessage*)data;
	WORD head_size = sizeof(CMD_CM_SystemMessage) - sizeof(system_msg->szString);
	if (data_size < head_size || data_size != head_size + system_msg->wLength * sizeof(TCHAR))
	{
		log("system data is error");
		return ;
	}

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(SystemMessage,data);	
}

void GameResponseHandler::OnReceivePlayerToSperator(void* data, WORD data_size)//转为旁观者
{

}

void GameResponseHandler::OnReceiveExchangeScoreMsg(void* data,WORD data_size)
{
	CMD_S_ExchangeFishScore* info = (CMD_S_ExchangeFishScore*)data;
}

void GameResponseHandler::OnSubModifyLkMulriple(void* data,WORD data_size)
{
	if(sizeof(CMD_S_HitFishLK) != data_size)
	{
		return;
	}
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ModifyLkMulriple,data);	
}
