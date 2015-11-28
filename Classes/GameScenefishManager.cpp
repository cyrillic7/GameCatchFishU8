
#include "GameScenefishManager.h"
#include "GameFishManager.h"
#include "GameFishAction.h"
#include "SessionManager.h"

SceneFishManager::SceneFishManager()
{
	game_config_ = nullptr;
	
}

SceneFishManager::~SceneFishManager() {
}

SceneFishManager* SceneFishManager::create(Layer* pLayer,const ClientGameConfig* client_game_config)
{
	SceneFishManager* m = new SceneFishManager();
	if (m && m->init(pLayer,client_game_config))
	{
		m->autorelease();
		return m;
	}
	CC_SAFE_DELETE(m);
	m = nullptr;
	return nullptr;
}

bool SceneFishManager::init(Layer* pLayer ,const ClientGameConfig* client_game_config)
{
	game_config_ = client_game_config;
	fishLayer   = pLayer;
	return true;
}


void SceneFishManager::BuildSceneFish(SceneKind scene_kind, WORD me_chair_id) {

  int gameKind = SessionManager::shareInstance()->getGameKind();
  switch (scene_kind) {
    case SCENE_1: {
	  if(gameKind == GameKindDntg)
		 BuildSceneFish2(me_chair_id);
	  else if(GameKindLkpy == gameKind || GameKindJcpy == gameKind )
		 BuildSceneFish2ForLK(me_chair_id);
      break;
    }
    case SCENE_2: {
		if(gameKind == GameKindDntg)
			BuildSceneFish5(me_chair_id);
		else if(GameKindLkpy == gameKind || GameKindJcpy == gameKind )
			BuildSceneFish3ForLK(me_chair_id);
      break;
    }
    case SCENE_3: {
		if(gameKind == GameKindDntg)
			BuildSceneFish3(me_chair_id);
		else if(GameKindLkpy == gameKind || GameKindJcpy == gameKind )
			BuildSceneFish4ForLK(me_chair_id);
      break;
    }
    case SCENE_4: {
		if(gameKind == GameKindDntg)
			BuildSceneFish1(me_chair_id);
		else if(GameKindLkpy == gameKind || GameKindJcpy == gameKind )
			BuildSceneFish5ForLK(me_chair_id);
      break;
    }
    case SCENE_5: {
		if(gameKind == GameKindDntg)
			BuildSceneFish4(me_chair_id);
		else if(GameKindLkpy == gameKind || GameKindJcpy == gameKind )
			BuildSceneFish1ForLK(me_chair_id);
      break;
    }
    default: {
      assert(false);
    }
  }
}

void SceneFishManager::BuildSceneFish1(WORD me_chair_id) {
  const float kFishSpeed = 60.0f;
  const Vec2 kBenchmarkPos(520.0f, kScreenHeight / 2.f);
  const Vec2 center1(-kBenchmarkPos.x, kBenchmarkPos.y), center2(kScreenWidth + kBenchmarkPos.x, kBenchmarkPos.y);
  int fish_id = 0;
  FishKind fish_kind;
  ActionFishMove* action = NULL;
  Vec2 start, end;
  // 中心2条大鱼 银龙和金龙
  fish_id = 0;
  fish_kind = FISH_SWK;
  start.x = -kBenchmarkPos.x;
  start.y = kBenchmarkPos.y;
  end.x = kScreenWidth + kBenchmarkPos.x;
  end.y = kBenchmarkPos.y;
  SwitchViewPosition(me_chair_id, &start, &end);
  action = ActionFishMoveLinear::create(kFishSpeed, start, end);
  GameFish* fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);

  fish_id = 1;
  fish_kind = FISH_YUWANGDADI;
  start.x = kScreenWidth + kBenchmarkPos.x;
  start.y = kBenchmarkPos.y;
  end.x = -kBenchmarkPos.x;
  end.y = kBenchmarkPos.y;
  SwitchViewPosition(me_chair_id, &start, &end);
  action = ActionFishMoveLinear::create(kFishSpeed, start, end);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);

  // 第1环左边
  fish_id = 2;
  fish_kind = FISH_XIAOCHOUYU;
  float radius = 185.f;
  float cell_radian = 2 * M_PI / 25;
  for (int i = 0; i < 25; ++i) {
    start.x = center1.x + radius * std::cos(i * cell_radian);
    start.y = center1.y + radius * std::sin(i * cell_radian);
    end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }

  // 第1环右边
  fish_id += 25;
  fish_kind = FISH_HUANGBIANYU;
  radius = 195.0f;
  for (int i = 0; i < 25; ++i) {
    start.x = center2.x + radius * std::cos(i * cell_radian);
    start.y = center2.y + radius * std::sin(i * cell_radian);
    end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }

  // 第2环左边
  fish_id += 25;
  fish_kind = FISH_DAYANYU;
  radius = 287.f;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    start.x = center1.x + radius * std::cos(i * cell_radian);
    start.y = center1.y + radius * std::sin(i * cell_radian);
    end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }

  // 第2环右边
  fish_id += 30;
  fish_kind = FISH_HUANGCAOYU;
  radius = 296.f;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    start.x = center2.x + radius * std::cos(i * cell_radian);
    start.y = center2.y + radius * std::sin(i * cell_radian);
    end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }

  // 第3环左边
  fish_id += 30;
  fish_kind = FISH_LVCAOYU;
  radius = 363.f;
  cell_radian = 2 * M_PI / 35;
  for (int i = 0; i < 35; ++i) {
    start.x = center1.x + radius * std::cos(i * cell_radian);
    start.y = center1.y + radius * std::sin(i * cell_radian);
    end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }

  // 第3环右边
  fish_id += 35;
  fish_kind = FISH_HUANGBIANYU;
  radius = 370.f;
  cell_radian = 2 * M_PI / 35;
  for (int i = 0; i < 35; ++i) {
    start.x = center2.x + radius * std::cos(i * cell_radian);
    start.y = center2.y + radius * std::sin(i * cell_radian);
    end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
}

void SceneFishManager::BuildSceneFish2(WORD me_chair_id) {
  const float kFishSpeed = 60.0f;
  int fish_id = 0;
  FishKind fish_kind;
  ActionFishMove* action = NULL;
  Vec2 start, end;
  const float kOffset = 100.f;
  float sub_offset;
  GameFish* fish = nullptr;
  // 绿草鱼 16 * 4 // 200
  sub_offset = 168.f;
  fish_kind = FISH_LVCAOYU;
  for (int i = 0; i < 16; ++i) {
    end.y = start.y = 150 + 12;
    end.x = kScreenWidth + 60.f;
    start.x = -kOffset - sub_offset - i * 54;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind],game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 16;
  for (int i = 0; i < 16; ++i) {
    end.y = start.y = 150 + 12 + 100;
    end.x = kScreenWidth + 60.f;
    start.x = -kOffset - sub_offset - i * 54;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 16;
  for (int i = 0; i < 16; ++i) {
    end.y = start.y = kScreenHeight - 150 - 100;
    end.x = kScreenWidth + 60.f;
    start.x = -kOffset - sub_offset - i * 54;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 16;
  for (int i = 0; i < 16; ++i) {
    end.y = start.y = kScreenHeight - 150;
    end.x = kScreenWidth + 60.f;
    start.x = -kOffset - sub_offset - i * 54;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 16;

  // 小刺鱼 10 + 10 + 3 + 3
  fish_kind = FISH_XIAOCIYU;
  for (int i = 0; i < 10; ++i) {
    end.y = start.y = 150 + 12 + 50;
    end.x = kScreenWidth + 150.f;
    start.x = -kOffset - i * 120;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 10;
  for (int i = 0; i < 10; ++i) {
    end.y = start.y = kScreenHeight - 150 - 50;
    end.x = kScreenWidth + 150.f;
    start.x = -kOffset - i * 120;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 10;
  for (int i = 0; i < 3; ++i) {
    end.y = start.y = 150.f + 12 + 50 + (i + 1) * 100;
    end.x = kScreenWidth + 150.f;
    start.x = -kOffset;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 3;
  for (int i = 0; i < 3; ++i) {
    end.y = start.y = 150.f + 12 + 50 + (i + 1) * 100;
    end.x = kScreenWidth + 150.f;
    start.x = -kOffset - 9 * 120;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 3;

  // 大闹天宫 绿草鱼和小刺鱼
  fish_kind = FISH_DNTG;
  end.y = start.y = 150 + 12;
  end.x = kScreenWidth + 150.f;
  start.x = -kOffset - 256;
  SwitchViewPosition(me_chair_id, &start, &end);
  action = ActionFishMoveLinear::create(kFishSpeed, start, end);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, FISH_LVCAOYU, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish); 
  ++fish_id;
  end.y = start.y = 150 + 40;
  end.x = kScreenWidth + 150.f;
  start.x = -kOffset - 128;
  SwitchViewPosition(me_chair_id, &start, &end);
  action = ActionFishMoveLinear::create(kFishSpeed, start, end);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, FISH_XIAOCIYU, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);
  ++fish_id;

  // 大眼鱼 4 + 4
  fish_kind = FISH_DAYANYU;
  Vec2 center(-kOffset - sub_offset - 2 * 54, 150.f + 12 + 50 + 165);
  float radius = 50.f;
  float angle = 0.f;
  for (int i = 0; i < 4; ++i) {
    start.x = center.x + radius * std::cos(angle);
    start.y = center.y + radius * std::sin(angle);
    end.x =  kScreenWidth + 150.f;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
    fishLayer->addChild(fish);
	angle += M_PI_2;
  }
  fish_id += 4;
  center.x = -kOffset - sub_offset - 13 * 54;
  angle = 0.f;
  for (int i = 0; i < 4; ++i) {
    start.x = center.x + radius * std::cos(angle);
    start.y = center.y + radius * std::sin(angle);
    end.x =  kScreenWidth + 150.f;
    end.y = start.y;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
    fishLayer->addChild(fish);
	angle += M_PI_2;
  }
  fish_id += 4;

  // 悟空
  fish_kind = FISH_SWK;
  end.y = start.y = kScreenHeight - 150 - 100;
  end.x = kScreenWidth + 380.f;
  start.x = -kOffset - 620;
  SwitchViewPosition(me_chair_id, &start, &end);
  action = ActionFishMoveLinear::create(kFishSpeed, start, end);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);
  ++fish_id;
}

void SceneFishManager::BuildSceneFish3(WORD me_chair_id) {
  const float kFishSpeed = 150.f;
  int fish_id = 0;
  FishKind fish_kind;
  ActionFishMove* action = NULL;
  const Vec2 center(kScreenWidth / 2.f, kScreenHeight / 2.f);
  float radius;
  float cell_radian;
  float angle;
  GameFish * fish =nullptr;
  // 玉皇大帝
  fish_kind = FISH_YUWANGDADI;
  action = ActionScene3FishMove::create(center, 0, 28.f, me_chair_id < 3 ? M_PI : 0, 4 * M_PI + M_PI_2, 5.f, kFishSpeed);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);
  ++fish_id;

  // 小丑鱼
  fish_kind = FISH_XIAOCHOUYU;
  radius = 150.f;
  cell_radian = 2 * M_PI / 10;
  for (int i = 0; i < 10; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene3FishMove::create(center, radius, 27.f, angle, 4 * M_PI, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 10;

  // 大眼鱼
  fish_kind = FISH_DAYANYU;
  radius = 150.f + 52.f + 42.f;
  cell_radian = 2 * M_PI / 18;
  for (int i = 0; i < 18; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene3FishMove::create(center, radius, 26.f, angle, 4 * M_PI - M_PI_2, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 18;

  // 黄草鱼
  fish_kind = FISH_HUANGCAOYU;
  radius = 150.f + 52.f + 42.f * 2 + 30;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene3FishMove::create(center, radius, 25.f, angle, 4 * M_PI - M_PI_2 * 2, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 30;

  // 蜗牛鱼
  fish_kind = FISH_WONIUYU;
  radius = 150.f + 52.f + 42.f * 2 + 30 * 2 + 35;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene3FishMove::create(center, radius, 24.f, angle, 4 * M_PI - M_PI_2 * 3, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 30;
}

void SceneFishManager::BuildSceneFish4(WORD me_chair_id) {
  const float kFishSpeed = 150.f;
  int fish_id = 0;
  FishKind fish_kind;
  ActionFishMove* action = NULL;
  const Vec2 center(kScreenWidth / 2.f, kScreenHeight / 2.f);
  float radius;
  float cell_radian;
  float angle;
  GameFish* fish = nullptr;
  // 
  fish_kind = FISH_SWK;
  action = ActionScene4FishMove::create(center, 0, 28.f, me_chair_id < 3 ? M_PI : 0, 4 * M_PI + M_PI_2, 5.f, kFishSpeed);
  fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
  fishLayer->addChild(fish);
  ++fish_id;

  // 小丑鱼
  fish_kind = FISH_XIAOCHOUYU;
  radius = 150.f;
  cell_radian = 2 * M_PI / 10;
  for (int i = 0; i < 10; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene4FishMove::create(center, radius, 27.f, angle, 4 * M_PI, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 10;

  // 大眼鱼
  fish_kind = FISH_DAYANYU;
  radius = 150.f + 52.f + 42.f;
  cell_radian = 2 * M_PI / 18;
  for (int i = 0; i < 18; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene4FishMove::create(center, radius, 26.f, angle, 4 * M_PI - M_PI_2, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 18;

  // 黄草鱼
  fish_kind = FISH_HUANGCAOYU;
  radius = 150.f + 52.f + 42.f * 2 + 30;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene4FishMove::create(center, radius, 25.f, angle, 4 * M_PI - M_PI_2 * 2, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 30;

  // 蜗牛鱼
  fish_kind = FISH_WONIUYU;
  radius = 150.f + 52.f + 42.f * 2 + 30 * 2 + 35;
  cell_radian = 2 * M_PI / 30;
  for (int i = 0; i < 30; ++i) {
    angle = i * cell_radian;
    if (me_chair_id < 3)
      angle += M_PI;
    action = ActionScene4FishMove::create(center, radius, 24.f, angle, 4 * M_PI - M_PI_2 * 3, 5.f, kFishSpeed);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 30;
}

void SceneFishManager::BuildSceneFish5(WORD me_chair_id) {
  float kFishSpeed = 50.0f;
  int fish_id = 0;
  FishKind fish_kind;
  ActionFishMove* action = NULL;
  Vec2 start, end;
  GameFish* fish =nullptr;

  // 蜗牛鱼 上50 下50
  float hinterval = kScreenWidth / 13.f;
  float vinterval = kScreenHeight / 6.f;
  fish_kind = FISH_WONIUYU;
  for (int i = 0; i < 50; ++i) {
    start.x = hinterval + (hinterval + (hinterval / 5.f)) * (i % 10);
    start.y = -100.f - (i / 10) * vinterval - (i % 3) * vinterval / 5.f;
    end.x = start.x;
    end.y = kScreenHeight + 100.f;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 50;
  for (int i = 0; i < 50; ++i) {
    start.x = hinterval + (hinterval + (hinterval / 5.f)) * (i % 10) + hinterval / 2;
    start.y = kScreenHeight + 100.f + 4 * vinterval - (i / 10) * vinterval + (i % 3) * vinterval / 5.f;
    end.x = start.x;
    end.y = -100.f;
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 40;

  // 蝙蝠鱼 银鲨 金鲨
  kFishSpeed = 100.0f;
  fish_kind = FISH_BIANFUYU;
  const Vec2 kFishStart1[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
  const Vec2 kFishEnd1[5] = { Vec2(-200, kScreenHeight / 2.f), Vec2(-200, kScreenHeight / 2.f - 100), Vec2(-200, kScreenHeight / 2.f + 100), Vec2(-200, kScreenHeight / 2.f + 60), Vec2(-200, kScreenHeight / 2.f - 60) };
  for (int i = 0; i < 5; ++i) {
    start = kFishStart1[i];
    end = kFishEnd1[i];
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 5;
  fish_kind = FISH_YINSHA;
  const Vec2 kFishStart2[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
  const Vec2 kFishEnd2[5] = { Vec2(-200.f, kScreenHeight - 100.f), Vec2(-300.f, kScreenHeight + 1.f), Vec2(-400.f, kScreenHeight + 100.f), Vec2(-500.f, kScreenHeight + 60.f), Vec2(-600, kScreenHeight - 60.f) };
  for (int i = 0; i < 5; ++i) {
    start = kFishStart2[i];
    end = kFishEnd2[i];
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 5;
  fish_kind = FISH_JINSHA;
  const Vec2 kFishStart3[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
  const Vec2 kFishEnd3[5] = { Vec2(-200, 100.f), Vec2(-300, 0), Vec2(-400, -100), Vec2(-500, -60), Vec2(-600, 60) };
  for (int i = 0; i < 5; ++i) {
    start = kFishStart3[i];
    end = kFishEnd3[i];
    SwitchViewPosition(me_chair_id, &start, &end);
    action = ActionFishMoveLinear::create(kFishSpeed, start, end);
    fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
  }
  fish_id += 5;
}

//////////////////////////////////////////////////////////////////////////
//李逵特殊场景的鱼

void SceneFishManager::BuildSceneFish2ForLK(WORD me_chair_id)
{
	const float kFishSpeed = 60.0f;
	const Vec2 kBenchmarkPos(520.0f, kScreenHeight / 2.f);
	Vec2 center(-kBenchmarkPos.x, kScreenHeight/2),center1(-kBenchmarkPos.x, kScreenHeight/2);
	int fish_id = 0;
	FishKind fish_kind;
	ActionFishMove* action = NULL;
	Vec2 start, end;
	GameFish* fish = nullptr;
	//最外环
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	float radius = 273.f;
	float cell_radian = 2 * M_PI / 100;
	for (int i = 0; i < 100; ++i) {
		start.x = center.x + radius * std::cos(i * cell_radian);
		start.y = center.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 100;

	//左上角
	radius = 273/2-20;
	center1.x = center.x - std::cos(M_PI_4)*radius;
	center1.y = center.y - std::sin(M_PI_4)*radius;
	fish_kind = (FishKind)(FISH_KIND_3 - FISH_KIND_1);
	cell_radian = 2 * M_PI / 20;
	for (int i = 0; i < 20; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 20;

	//右上角
	center1.x = center.x + std::cos(M_PI_4)*radius;
	center1.y = center.y - std::sin(M_PI_4)*radius;
	fish_kind = (FishKind)(FISH_KIND_5 - FISH_KIND_1);
	cell_radian = 2 * M_PI / 20;
	for (int i = 0; i < 20; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 20;

	//左下角
	center1.x = center.x - std::cos(M_PI_4)*radius;
	center1.y = center.y + std::sin(M_PI_4)*radius;
	fish_kind = (FishKind)(FISH_KIND_2 - FISH_KIND_1);
	cell_radian = 2 * M_PI / 30;
	for (int i = 0; i < 30; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 30;

	//右下角
	center1.x = center.x + std::cos(M_PI_4)*radius;
	center1.y = center.y + std::sin(M_PI_4)*radius;
	fish_kind = (FishKind)(FISH_KIND_4 - FISH_KIND_1);
	cell_radian = 2 * M_PI / 30;
	for (int i = 0; i < 30; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 30;

	//中间
	fish_kind = (FishKind)(FISH_KIND_6 - FISH_KIND_1);
	radius = 273/3;
	cell_radian = 2 * M_PI / 15;
	for (int i = 0; i < 15; ++i) {
		start.x = center.x + radius * std::cos(i * cell_radian);
		start.y = center.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed,start,end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 15;

	if(SessionManager::shareInstance()->getGameKind() == GameKindLkpy)
		fish_kind = (FishKind)(FISH_KIND_20 - FISH_KIND_1);
	else if(SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
		fish_kind = (FishKind)(FISH_KIND_JC22 - FISH_KIND_JC1);

	start.x = center.x;
	start.y = center.y;
	end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
	end.y = start.y;
	SwitchViewPosition(me_chair_id, &start, &end);
	action = ActionFishMoveLinear::create(kFishSpeed,start,end);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
}


void SceneFishManager::BuildSceneFish3ForLK(WORD me_chair_id)
{
	float kFishSpeed = 50.0f;
	int fish_id = 0;
	FishKind fish_kind;
	ActionFishMove* action = NULL;
	Vec2 start, end;
	GameFish* fish = nullptr;

	// 蜗牛鱼 上50 下50
	float hinterval = kScreenWidth / 13.f;
	float vinterval = kScreenHeight / 6.f;
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	for (int i = 0; i < 50; ++i) {
		start.x = hinterval + (hinterval + (hinterval / 5.f)) * (i % 10);
		start.y = -100.f - (i / 10) * vinterval - (i % 3) * vinterval / 5.f;
		end.x = start.x;
		end.y = kScreenHeight + 100.f;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 50;
	for (int i = 0; i < 50; ++i) {
		start.x = hinterval + (hinterval + (hinterval / 5.f)) * (i % 10) + hinterval / 2;
		start.y = kScreenHeight + 100.f + 4 * vinterval - (i / 10) * vinterval + (i % 3) * vinterval / 5.f;
		end.x = start.x;
		end.y = -100.f;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 50;

	// 蝙蝠鱼 银鲨 金鲨
	kFishSpeed = 100.0f;
	fish_kind = (FishKind)(FISH_KIND_15 - FISH_KIND_1);
	const Vec2 kFishStart1[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
	const Vec2 kFishEnd1[5] = { Vec2(-200, kScreenHeight / 2.f), Vec2(-200, kScreenHeight / 2.f - 100), Vec2(-200, kScreenHeight / 2.f + 100), Vec2(-200, kScreenHeight / 2.f + 60), Vec2(-200, kScreenHeight / 2.f - 60) };
	for (int i = 0; i < 5; ++i) {
		start = kFishStart1[i];
		end = kFishEnd1[i];
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 5;
	fish_kind = (FishKind)(FISH_KIND_16 - FISH_KIND_1);
	const Vec2 kFishStart2[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
	const Vec2 kFishEnd2[5] = { Vec2(-200.f, kScreenHeight - 100.f), Vec2(-300.f, kScreenHeight + 1.f), Vec2(-400.f, kScreenHeight + 100.f), Vec2(-500.f, kScreenHeight + 60.f), Vec2(-600, kScreenHeight - 60.f) };
	for (int i = 0; i < 5; ++i) {
		start = kFishStart2[i];
		end = kFishEnd2[i];
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 5;
	fish_kind = (FishKind)(FISH_KIND_17 - FISH_KIND_1);
	const Vec2 kFishStart3[5] = { Vec2(kScreenWidth + 200, kScreenHeight / 2.f), Vec2(kScreenWidth + 500, kScreenHeight / 2.f - 50), Vec2(kScreenWidth + 800, kScreenHeight / 2.f + 60), Vec2(kScreenWidth + 1100, kScreenHeight / 2.f - 60), Vec2(kScreenWidth + 1400, kScreenHeight / 2.f - 60) };
	const Vec2 kFishEnd3[5] = { Vec2(-200, 100.f), Vec2(-300, 0), Vec2(-400, -100), Vec2(-500, -60), Vec2(-600, 60) };
	for (int i = 0; i < 5; ++i) {
		start = kFishStart3[i];
		end = kFishEnd3[i];
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id + i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 5;
}


void SceneFishManager::BuildSceneFish4ForLK(WORD me_chair_id)
{
	const float kFishSpeed = 150.f;
	int fish_id = 0;
	FishKind fish_kind;
	ActionFishMove* action = NULL;
	Vec2 center(kScreenWidth/4*3, kScreenHeight / 2.f);
	GameFish* fish = nullptr;
	float radius;
	float cell_radian;
	float angle;
	//右边第一环
	fish_kind = (FishKind)(FISH_KIND_7 - FISH_KIND_1);
	radius = 135.f;
	cell_radian = 2 * M_PI / 13;
	for (int i = 0; i < 13; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 27.f, angle, 4 * M_PI, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 13;

	//右边第二环
	fish_kind = (FishKind)(FISH_KIND_6 - FISH_KIND_1);
	radius = 205;
	cell_radian = 2 * M_PI / 24;
	for (int i = 0; i < 24; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 26.f, angle, 4 * M_PI - M_PI_2, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 24;

	//右边第三环
	fish_kind = (FishKind)(FISH_KIND_5 - FISH_KIND_1);
	radius = 255;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 25.f, angle, 4 * M_PI - M_PI_2 * 2, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 40;

	//右边第四环
	fish_kind = (FishKind)(FISH_KIND_2 - FISH_KIND_1);
	radius = 290;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 24.f, angle, 4 * M_PI - M_PI_2 * 3, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 40;

	// 右边中间
	fish_kind = (FishKind)(FISH_KIND_17 - FISH_KIND_1);
	action = ActionScene3FishMove::create(center, 0, 28.f, me_chair_id < 3 ? M_PI : 0, 4 * M_PI + M_PI_2, 15.f, kFishSpeed);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
	++fish_id;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//左边第一环
	center.x = kScreenWidth/4;
	fish_kind = (FishKind)(FISH_KIND_6 - FISH_KIND_1);
	radius = 135.f;
	cell_radian = 2 * M_PI / 13;
	for (int i = 0; i < 13; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 27.f, angle, 4 * M_PI, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 13;

	//左边第二环
	fish_kind = (FishKind)(FISH_KIND_4 - FISH_KIND_1);
	radius = 205;
	cell_radian = 2 * M_PI / 24;
	for (int i = 0; i < 24; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action =  ActionScene3FishMove::create(center, radius, 26.f, angle, 4 * M_PI - M_PI_2, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 24;

	//左边第三环
	fish_kind = (FishKind)(FISH_KIND_3 - FISH_KIND_1);
	radius = 255;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene3FishMove::create(center, radius, 25.f, angle, 4 * M_PI - M_PI_2 * 2, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 40;

	//左边第四环
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	radius = 290;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action =  ActionScene3FishMove::create(center, radius, 24.f, angle, 4 * M_PI - M_PI_2 * 3, 15.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 40;

	//左边中间
	fish_kind = (FishKind)(FISH_KIND_18 - FISH_KIND_1);
	action = ActionScene3FishMove::create(center, 0, 28.f, me_chair_id < 3 ? M_PI : 0, 4 * M_PI + M_PI_2, 15.f, kFishSpeed);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
	++fish_id;
}


void SceneFishManager::BuildSceneFish5ForLK(WORD me_chair_id)
{
	const float kFishSpeed = 60.0f;
	const Vec2 kBenchmarkPos(520.0f, kScreenHeight / 2.f);
	const Vec2 center1(-kBenchmarkPos.x, kBenchmarkPos.y), center2(kScreenWidth + kBenchmarkPos.x, kBenchmarkPos.y);
	int fish_id = 0;
	FishKind fish_kind;
	ActionFishMove* action = NULL;
	Vec2 start, end;
	GameFish* fish = nullptr;
	// 第3环右边
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	float radius = 273.f;
	float cell_radian = 2 * M_PI / 50;
	for (int i = 0; i < 50; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 50;

	// 第3环左边
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	radius = 273.f;
	cell_radian = 2 * M_PI / 50;
	for (int i = 0; i < 50; ++i) {
		start.x = center2.x + radius * std::cos(i * cell_radian);
		start.y = center2.y + radius * std::sin(i * cell_radian);
		end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	// 第2环右边
	fish_id += 50;
	fish_kind = (FishKind)(FISH_KIND_3 - FISH_KIND_1);
	radius = 207.f;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}

	// 第2环左边
	fish_id += 40;
	fish_kind = (FishKind)(FISH_KIND_2 - FISH_KIND_1);
	radius = 207.f;
	cell_radian = 2 * M_PI / 40;
	for (int i = 0; i < 40; ++i) {
		start.x = center2.x + radius * std::cos(i * cell_radian);
		start.y = center2.y + radius * std::sin(i * cell_radian);
		end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action = ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}

	// 第1环右边
	fish_id += 40;
	fish_kind = (FishKind)(FISH_KIND_4 - FISH_KIND_1);
	radius = 165.f;
	cell_radian = 2 * M_PI / 30;
	for (int i = 0; i < 30; ++i) {
		start.x = center1.x + radius * std::cos(i * cell_radian);
		start.y = center1.y + radius * std::sin(i * cell_radian);
		end.x = start.x + kScreenWidth + kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action =  ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}

	// 第1环左边
	fish_id += 30;
	fish_kind = (FishKind)(FISH_KIND_5 - FISH_KIND_1);
	radius = 165.0f;
	for (int i = 0; i < 30; ++i) {
		start.x = center2.x + radius * std::cos(i * cell_radian);
		start.y = center2.y + radius * std::sin(i * cell_radian);
		end.x =  start.x - kScreenWidth - kBenchmarkPos.x * 2;
		end.y = start.y;
		SwitchViewPosition(me_chair_id, &start, &end);
		action =  ActionFishMoveLinear::create(kFishSpeed, start, end);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}


	// 中心2条大鱼 银龙和金龙
	fish_id += 30;
	fish_kind = (FishKind)(FISH_KIND_16 - FISH_KIND_1);
	start.x = -kBenchmarkPos.x;
	start.y = kBenchmarkPos.y;
	end.x = kScreenWidth + kBenchmarkPos.x;
	end.y = kBenchmarkPos.y;
	SwitchViewPosition(me_chair_id, &start, &end);
	action = ActionFishMoveLinear::create(kFishSpeed, start, end);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);

	fish_id++;
	fish_kind = (FishKind)(FISH_KIND_17 - FISH_KIND_1);
	start.x = kScreenWidth + kBenchmarkPos.x;
	start.y = kBenchmarkPos.y;
	end.x = -kBenchmarkPos.x;
	end.y = kBenchmarkPos.y;
	SwitchViewPosition(me_chair_id, &start, &end);
	action =  ActionFishMoveLinear::create(kFishSpeed, start, end);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
}


void SceneFishManager::BuildSceneFish1ForLK(WORD me_chair_id)
{
	const float kFishSpeed = 150.f;
	int fish_id = 0;
	FishKind fish_kind;
	ActionFishMove* action = NULL;
	const Vec2 center(kScreenWidth / 2.f, kScreenHeight / 2.f);
	float radius;
	float cell_radian;
	float angle;
	GameFish* fish;
	//
	fish_kind = (FishKind)(FISH_KIND_17 - FISH_KIND_1);
	action = ActionScene4FishMove::create(center, 0, 28.f, me_chair_id < 3 ? M_PI : 0, 4 * M_PI + M_PI_2, 5.f, kFishSpeed);
	fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
	fishLayer->addChild(fish);
	++fish_id;

	// 小丑鱼
	fish_kind = (FishKind)(FISH_KIND_1 - FISH_KIND_1);
	radius = 150.f;
	cell_radian = 2 * M_PI / 10;
	for (int i = 0; i < 10; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene4FishMove::create(center, radius, 27.f, angle, 4 * M_PI, 5.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 10;

	// 大眼鱼
	fish_kind = (FishKind)(FISH_KIND_3 - FISH_KIND_1);
	radius = 150.f + 52.f + 42.f;
	cell_radian = 2 * M_PI / 18;
	for (int i = 0; i < 18; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action =  ActionScene4FishMove::create(center, radius, 26.f, angle, 4 * M_PI - M_PI_2, 5.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 18;

	// 黄草鱼
	fish_kind = (FishKind)(FISH_KIND_3 - FISH_KIND_1);
	radius = 150.f + 52.f + 42.f * 2 + 30;
	cell_radian = 2 * M_PI / 30;
	for (int i = 0; i < 30; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action = ActionScene4FishMove::create(center, radius, 25.f, angle, 4 * M_PI - M_PI_2 * 2, 5.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 30;

	// 蜗牛鱼
	fish_kind = (FishKind)(FISH_KIND_5 - FISH_KIND_1);
	radius = 150.f + 52.f + 42.f * 2 + 30 * 2 + 35;
	cell_radian = 2 * M_PI / 30;
	for (int i = 0; i < 30; ++i) {
		angle = i * cell_radian;
		if (me_chair_id < 3)
			angle += M_PI;
		action =  ActionScene4FishMove::create(center, radius, 24.f, angle, 4 * M_PI - M_PI_2 * 3, 5.f, kFishSpeed);
		fish = GameFishManager::sharedInstance()->creatFish(fish_kind, fish_id+i, 0, game_config_->fish_bounding_radius[fish_kind], game_config_->fish_bounding_count[fish_kind], action);
		fishLayer->addChild(fish);
	}
	fish_id += 30;
}


void SceneFishManager::SwitchViewPosition(WORD me_chair_id, Vec2* start, Vec2* end) {
	/*if (me_chair_id >= 3)
		return;
	start->x = kScreenWidth - start->x;
	start->y = kScreenHeight - start->y;
	end->x = kScreenWidth - end->x;
	end->y = kScreenHeight - end->y;*/

	if (me_chair_id < 3)
	{
		start->x = kScreenWidth - start->x;
		end->x = kScreenWidth - end->x;
	}
	else
	{	
		start->y = kScreenHeight - start->y;
		end->y = kScreenHeight - end->y;
	}
}
