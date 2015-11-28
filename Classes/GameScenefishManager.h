
#ifndef SCENE_FISH_MANAGER_H_
#define SCENE_FISH_MANAGER_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
USING_NS_CC;

class SceneFishManager : public Ref {
 public:
  static SceneFishManager* create(Layer* pLayer,const ClientGameConfig* client_game_config);
  SceneFishManager();
  bool init(Layer* pLayer,const ClientGameConfig* client_game_config);
  ~SceneFishManager();

  void BuildSceneFish(SceneKind scene_kind, WORD me_chair_id);

 private:
  void BuildSceneFish1(WORD me_chair_id);
  void BuildSceneFish2(WORD me_chair_id);
  void BuildSceneFish3(WORD me_chair_id);
  void BuildSceneFish4(WORD me_chair_id);
  void BuildSceneFish5(WORD me_chair_id);
  void SwitchViewPosition(WORD me_chair_id, Vec2* start, Vec2* end);


  void BuildSceneFish1ForLK(WORD me_chair_id);
  void BuildSceneFish2ForLK(WORD me_chair_id);
  void BuildSceneFish3ForLK(WORD me_chair_id);
  void BuildSceneFish4ForLK(WORD me_chair_id);
  void BuildSceneFish5ForLK(WORD me_chair_id);

 private:
  const ClientGameConfig* game_config_;
  Layer* fishLayer;
};

#endif  // SCENE_FISH_MANAGER_H_
