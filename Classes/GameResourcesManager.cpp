#include"GameResourcesManager.h"
#include "CommonFunction.h"
#include "SessionManager.h"
#include "Model/GuideItemModel.h"

static ResourcesManager* __instance = nullptr;


AnimationInfo::AnimationInfo()
	:mFishKind(FISH_WONIUYU),mFps(1.0f),mWalk(false),mLoop(false),mFrames(0),mImageName(""),mResId(0)
{

}

AnimationInfo::~AnimationInfo()
{

}


AnimationInfo* AnimationInfo::create()
{
	AnimationInfo* info = new AnimationInfo;
	if(info && info->init())
	{
		info->autorelease();
		return info;
	}
	CC_SAFE_DELETE(info);
	return nullptr;
}

bool AnimationInfo::init()
{
	return true;
}

ResourcesManager::ResourcesManager()
{
	mAnimationInfos = __Array::create();
	CC_SAFE_RETAIN(mAnimationInfos);

	mGuideInfos = __Array::create();
	CC_SAFE_RETAIN(mGuideInfos);
}

ResourcesManager::~ResourcesManager()
{
	CC_SAFE_RELEASE_NULL(mAnimationInfos);
	CC_SAFE_RELEASE_NULL(mGuideInfos);
}

ResourcesManager* ResourcesManager::getInstance()
{
	if(__instance==nullptr)
		__instance = new ResourcesManager;
	return __instance;
}

void ResourcesManager::purgeInstance()
{
	CC_SAFE_DELETE(__instance);
}

bool ResourcesManager::loadResourceFile(int gameId)
{
	//加载游走动画
	std::string anima_file = "";
	std::string dead_file = "";
	switch (gameId)
	{
	case GameKindDntg:
		anima_file= "animation.plist";
		dead_file = "animation_dead.plist";
		break;
	case GameKindLkpy:
		anima_file= "animation_lk.plist";
		dead_file = "animation_dead_lk.plist";
		break;
	case GameKindJcpy:
		anima_file= "animation_jc.plist";
		dead_file = "animation_dead_jc.plist";
		break;
	}
	
	ValueMap plist = FileUtils::getInstance()->getValueMapFromFile(anima_file);
	log(" ResourcesManager::loadResourceFile 111 plist.size() = %d",plist.size());
	for (int i=0;i<plist.size();i++)
	{
		ValueMap& fishdic = plist[numberToString(i)].asValueMap();
		AnimationInfo* pInfo = AnimationInfo::create();
		if(gameId == GameKindLkpy)
			pInfo->setKind((FishKind)(fishdic["fishKind"].asInt()+FISH_KIND_1));
		else if(gameId == GameKindJcpy)
			pInfo->setKind((FishKind)(fishdic["fishKind"].asInt()+FISH_KIND_JC1));
		else
			pInfo->setKind((FishKind)fishdic["fishKind"].asInt());

		pInfo->setFps( fishdic["fps"].asFloat());
		pInfo->setFrames(fishdic["frames"].asInt());
		pInfo->setLoop(fishdic["isLoop"].asBool());
		pInfo->setWalk(fishdic["isWalk"].asBool());
		pInfo->setFrameWidth(fishdic["width"].asInt());
		pInfo->setFrameHeight(fishdic["height"].asInt());
		pInfo->setImageName(fishdic["name"].asString());
		mAnimationInfos->addObject(pInfo);
	}
	//加载死亡动画
	ValueMap deadlist = FileUtils::getInstance()->getValueMapFromFile(dead_file);
	log(" ResourcesManager::loadResourceFile 222 deadlist.size() = %d",deadlist.size());
	for (int i=0;i<deadlist.size();i++)
	{
		ValueMap& fishdic = deadlist[numberToString(i)].asValueMap();
		AnimationInfo* pInfo = AnimationInfo::create();

		if(gameId == GameKindLkpy)
			pInfo->setKind((FishKind)(fishdic["fishKind"].asInt()+FISH_KIND_1));
		else  if(gameId == GameKindJcpy)
			pInfo->setKind((FishKind)(fishdic["fishKind"].asInt()+FISH_KIND_JC1));
		else
			pInfo->setKind((FishKind)fishdic["fishKind"].asInt());

		pInfo->setFps( fishdic["fps"].asFloat());
		pInfo->setFrames(fishdic["frames"].asInt());
		pInfo->setLoop(fishdic["isLoop"].asBool());
		pInfo->setWalk(fishdic["isWalk"].asBool());
		pInfo->setFrameWidth(fishdic["width"].asInt());
		pInfo->setFrameHeight(fishdic["height"].asInt());
		pInfo->setImageName(fishdic["name"].asString());
		mAnimationInfos->addObject(pInfo);
	}

	
	return true;
}

void  ResourcesManager::loadGuideFile(int gameId)
{
	//鱼百科文件名
	std::string guide_file = "";
	switch (gameId)
	{
	case GameKindDntg:
		guide_file= "guide_skw.plist";
		break;
	case GameKindLkpy:
		guide_file= "guide_lk.plist";
		break;
	case GameKindJcpy:
		guide_file= "guide_jc.plist";
		break;
	}

	__Dictionary* dic = __Dictionary::createWithContentsOfFile(guide_file.c_str());
	for (int i=0;i<dic->count();i++)
	{
		__Dictionary* infoDic = (__Dictionary* )dic->objectForKey(numberToString(i));
		GuideItemModel* pInfo = GuideItemModel::create();
		pInfo->setTitle(((__String*)infoDic->objectForKey("title"))->getCString());
		pInfo->setImageName(((__String*)infoDic->objectForKey("Imagename"))->getCString());
		pInfo->setPower(((__String*)infoDic->objectForKey("power"))->intValue());
		pInfo->setItemDesc(((__String*)infoDic->objectForKey("desc"))->getCString());
		mGuideInfos->addObject(pInfo);
	}
}


bool ResourcesManager::ContainsKey(FishKind kind)
{
	for(int i=0;i<mAnimationInfos->count();i++)
	{
		AnimationInfo* info = (AnimationInfo*)mAnimationInfos->getObjectAtIndex(i);
		if(info->getKind() == kind)
		{
			return true;
		}
	}
	return false;
}

AnimationInfo* ResourcesManager::getAnimationInfoByKind(FishKind kind,bool isWalk)
{
	if(kind >= 0 && kind < FISH_KIND_COUNT )
	{
		for(int i=0;i<mAnimationInfos->count();i++)
		{
			AnimationInfo* info = (AnimationInfo*)mAnimationInfos->getObjectAtIndex(i);
			if(info->getKind() == kind && info->isWalk() == isWalk)
			{
				return info;
			}
		}
	}
	return nullptr;
}

//针对不是鱼的效果，如意金箍棒，无敌风火轮,镇妖金刚塔，等文字效果
AnimationInfo* ResourcesManager::getAnimationInfoByName(const char* name )
{
	if(strlen(name) > 0)
	{
		for(int i=0;i<mAnimationInfos->count();i++)
		{
			AnimationInfo* info = (AnimationInfo*)mAnimationInfos->getObjectAtIndex(i);
			if(strcmp(info->getImageName().c_str(),name)==0)
			{
				return info;
			}
		}
	}
	return nullptr;
}

void ResourcesManager::clearAllResource()
{
	mAnimationInfos->removeAllObjects();
	mGuideInfos->removeAllObjects();
}


Vec2  ResourcesManager::stringToPoint(std::string str){
	std::string strArray[2];
	char s[1024];
	strcpy(s, str.c_str());
	const char *d = " ,";
	char *p;
	p = strtok(s, d);
	int i =0;
	while (p){
		strArray[i++] = p;
		p = strtok(NULL, d);
	}
	Vec2 centerPt = Vec2(atoi(strArray[0].c_str()),atoi(strArray[1].c_str()));
	return centerPt;
}

