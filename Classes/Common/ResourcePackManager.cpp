#include "ResourcePackManager.h"
#include "crypto/CCCrypto.h"
#include "CommonFunction.h"
#include "pystring.h"

//对整个流程的进度, 80%为下载, 10%为验证, 10%为解压
#define FlowProgressDownloadPercentage      80
#define FlowProgressVerifyPercentage        10
#define FlowProgressUncompressPercentage    10

//通用资源包下载和恢复列表的key
#define CommonResourcePackResumeDownloadKey     "CommonResourcePackResumeDownloadKey"

static ResourcePackManager *__instance = NULL;

ResourcePackManager::ResourcePackManager()
    :m_delegate(NULL)
    ,m_isFlowDone(true)
    ,m_currentIndex(0)
{
    m_assetsManager = TNAssetsManager::create();
    m_assetsManager->retain();
    m_assetsManager->setDelegate(this);
    m_assetsManager->setConnectionTimeout(60);

    m_resourcePacks = CCArray::create();
    m_resourcePacks->retain();

}

ResourcePackManager::~ResourcePackManager() {
    m_delegate = NULL;
    CC_SAFE_RELEASE_NULL(m_assetsManager);
    CC_SAFE_RELEASE_NULL(m_resourcePacks);
}

ResourcePackManager *ResourcePackManager::sharedInstance() {
    if (NULL == __instance) {
        __instance = new ResourcePackManager;
        __instance->init();
    }
    return __instance;
}

void ResourcePackManager::purgeInstance() {
    if (NULL != __instance) {
        __instance->cleanup();
    }
    CC_SAFE_DELETE(__instance);
}

bool ResourcePackManager::init() {
    restoreResourcePack();
    return true;
}

void ResourcePackManager::createSaveDirectory(const char *path) {
    if (NULL != path) {
        CommonFunction::createDirectory(path);
    }
}

void ResourcePackManager::addResourcePack(ResourcePack *pack) {
    if (NULL != pack) {
        for (int i = 0; i < m_resourcePacks->count(); i++) {
            ResourcePack* item = (ResourcePack*)m_resourcePacks->objectAtIndex(i);
            if (pack->getName() == item->getName() && pack->getVersion() == item->getVersion()) {
                return;
            }
        }
        m_resourcePacks->addObject(pack);
        saveResourcePack(pack);

        //回调进度
        handleProgressCallback(pack, 0);
    }
}

bool ResourcePackManager::isResourcePackDownloaded(ResourcePack *pack) {
    if (NULL != pack) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
        std::string saveVersion =  userDefault->getStringForKey(getSaveDownloadedResourcePackRecordKey(pack).c_str());
        return saveVersion == pack->getVersion();
    }
    return false;
}

bool ResourcePackManager::isResourceDownloaded(const char* gameName)
{
	if (NULL != gameName) {
		CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
		__String* package = __String::createWithFormat("%sPackageVersion",gameName);
		std::string saveVersion =  userDefault->getStringForKey(package->getCString());
		if (saveVersion.length() > 0)
		{
			return true;
		}
	}
	return false;
}

bool ResourcePackManager::isResourcePackDownloading(ResourcePack *pack) {
    if (NULL != pack) {
        ResourcePack *curPack = getCurrentDownloadingResourcePack();
        if (NULL != curPack && curPack->getName() == pack->getName() && curPack->getVersion() == pack->getVersion()) {
            return true;
        }
    }
    return false;
}

bool ResourcePackManager::isInDownPackList(const char* packName)
{
		if(m_resourcePacks->count() >0) 
		{
			for (int i=0; i< m_resourcePacks->count();i++)
			{
				ResourcePack *pack = (ResourcePack*)m_resourcePacks->objectAtIndex(i);
				if (strcmp(pack->getName().c_str(),packName) == 0)
				{
					return true;
				}
			}
		}

		return false;
}

CCArray *ResourcePackManager::getResourcePackList() {
    return m_resourcePacks;
}

ResourcePack *ResourcePackManager::getCurrentDownloadingResourcePack() {
    if (m_currentIndex >= 0 && m_currentIndex < m_resourcePacks->count()) {
        ResourcePack *pack = (ResourcePack*)m_resourcePacks->objectAtIndex(m_currentIndex);
        return pack;
    }
    return NULL;
}

void ResourcePackManager::start() {
    if (m_resourcePacks->count() > 0 && m_isFlowDone) {
        m_isFlowDone = false;

        //正常流程:下载=>验证=>解压=>删除
        std::vector<TNAssetsManagerActionCode> v;
        v.push_back(kDownload);
        v.push_back(kVerifyPackage);
        v.push_back(kUncompress);

        executeFlows(v);

        ResourcePack *pack = getCurrentDownloadingResourcePack();
        if (NULL != pack) {
            log("[ start ] => [ name: %s, downloadPath: %s ]", pack->getName().c_str(), pack->getDownloadPath().c_str());
        }
    }
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(ResourcePackManager::update), this, 1.0, false);
}

void ResourcePackManager::cleanup() {
    m_assetsManager->cleanup();
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

void ResourcePackManager::onError(TNAssetsManagerActionCode code, int index) {
    if (index >= 0 && index < m_assetsManager->assetCount()) {
        TNAssetData *data = m_assetsManager->assetWithIndex(index);
        log("[ error ], [ action: %d name: %s url: %s ]", code, data->getName().c_str(), data->getDownloadUrl().c_str());

        handleFailureCallback(code);

        if (code == kDownload || code == kVerifyPackage || code == kUncompress || code == kInvalidAssetData) { //如果验证出错, 删除此包
            m_assetsManager->removeAssetPackage(index);
            clearActionRecord(data);

            if (m_resourcePacks->count() > 0 && m_resourcePacks->count() > m_currentIndex && m_currentIndex >= 0) {
                m_resourcePacks->removeObjectAtIndex(m_currentIndex);
            }

            m_currentIndex--;
        }
    }

    m_currentIndex ++; //错误也继续下一个
    if (m_currentIndex >= m_resourcePacks->count()) {
        m_currentIndex = 0;
    }
    m_isFlowDone = true;
}

void ResourcePackManager::onProgress(TNAssetsManagerActionCode code, int percent) {
    //对整个流程的进度, 80%为下载, 10%为验证, 10%为解压
    int flowPercent = 0;
    if (code == kDownload) {
        flowPercent = percent * FlowProgressDownloadPercentage / 100;
    } else if(code == kVerifyPackage) {
        flowPercent = FlowProgressDownloadPercentage + percent * FlowProgressVerifyPercentage / 100;
    } else if(code == kUncompress) {
        flowPercent = FlowProgressDownloadPercentage + FlowProgressVerifyPercentage + percent * FlowProgressUncompressPercentage / 100;
    }
    handleProgressCallback(getCurrentDownloadingResourcePack(), flowPercent);
}

void ResourcePackManager::onSuccess(TNAssetsManagerActionCode code, int index) {
    if (index != kAllAssetsFinishIndex) {
        TNAssetData *data = m_assetsManager->assetWithIndex(index);
        saveActionRecord(code, data); //保存此资源相应记录
    }
    switch (code) {
        case kDownload:  {
            if (index == kAllAssetsFinishIndex) {
                ResourcePack *pack = getCurrentDownloadingResourcePack();
                if (NULL != pack) {
                    log("[ all download success ] => [ name: %s, downloadPath: %s ]", pack->getName().c_str(), pack->getDownloadPath().c_str());
                }
                executeFlow(kVerifyPackage);
            }
        }
            break;
        case kVerifyPackage:  {
            if (index == kAllAssetsFinishIndex) {
                ResourcePack *pack = getCurrentDownloadingResourcePack();
                if (NULL != pack) {
                    log("[ all verfify success ] => [ name: %s, downloadPath: %s ]", pack->getName().c_str(), pack->getDownloadPath().c_str());
                }
                executeFlow(kUncompress);
            }
        }
            break;
        case kUncompress: {
            if (index == kAllAssetsFinishIndex) {
                ResourcePack *pack = getCurrentDownloadingResourcePack();
                if (NULL != pack) {
                    log("[ all uncompress success ] => [ name: %s, downloadPath: %s ]", pack->getName().c_str(), pack->getDownloadPath().c_str());
                }

                //保存一条记录, 记录当前皮肤包已经更新成功了
                saveDownloadedResourcePackRecord(getCurrentDownloadingResourcePack());

                //解压完就当做整个流程成功了, 所以此处回调成功消息
                handleSuccessCallback();

                //清除动作相关的所有记录
                clearAllActionRecords();

                //清掉这个皮肤的数据缓存
                removeResourcePackFromDisk(getCurrentDownloadingResourcePack());

                m_resourcePacks->removeObjectAtIndex(m_currentIndex);
                m_isFlowDone = true; //标志整个流程完成
            } else {
                //解压成功后, 删除包
                m_assetsManager->removeAssetPackage(index);
            }
        }
            break;
    }
}

void ResourcePackManager::update(float dt) {
    if ( m_isFlowDone && m_currentIndex >= 0 && m_currentIndex < m_resourcePacks->count()) {
        start();
    }
}

void ResourcePackManager::restoreResourcePack() {
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

    std::string nameString = userDefault->getStringForKey(getSaveResourcePackKey().c_str(), "");
    std::vector<std::string> v;
    pystring::split(nameString, v, "|");

    m_resourcePacks->removeAllObjects();
    for (int i = 0; i < v.size(); ++i) {
        if (v[i].length() > 0) {
            ResourcePack *pack = ResourcePack::create();
            if(pack->restore(v[i].c_str()) ) {
                m_resourcePacks->addObject(pack);
            }
        }
    }
}

void ResourcePackManager::saveResourcePack(ResourcePack *pack) {
    if (NULL != pack && pack->getName().length() > 0) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

        pack->save();

        std::string nameString = userDefault->getStringForKey(getSaveResourcePackKey().c_str(), "");
        std::vector<std::string> v;
        pystring::split(nameString, v, "|");

        //是否有相同的皮肤, 没有找到就存储
        std::vector<std::string>::iterator it = std::find(v.begin(), v.end(), pack->getSaveKey());
        if (it == v.end()) {
            v.push_back(pack->getSaveKey());
            nameString = pystring::join("|", v);
            userDefault->setStringForKey(getSaveResourcePackKey().c_str(), nameString);
        }
        userDefault->flush();
    }
}

void ResourcePackManager::removeResourcePackFromDisk(ResourcePack *pack) {
    if (NULL != pack) {
        //1.先清掉皮肤数据
        pack->clearCache();

        //2.再清掉名字
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
        std::string nameString = userDefault->getStringForKey(getSaveResourcePackKey().c_str(), "");
        std::vector<std::string> v;
        pystring::split(nameString, v, ",");
        std::vector<std::string>::iterator it = std::find(v.begin(), v.end(), pack->getName());
        if (it != v.end()) { //找到了
            v.erase(it);
            nameString = pystring::join(",", v);
            userDefault->setStringForKey(getSaveResourcePackKey().c_str(), nameString);
        }
        userDefault->flush();
    }
}

std::string ResourcePackManager::getSaveResourcePackKey() {
    return CommonResourcePackResumeDownloadKey;
}

void ResourcePackManager::saveDownloadedResourcePackRecord(ResourcePack *pack) {
    if (NULL != pack) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
        userDefault->setStringForKey(getSaveDownloadedResourcePackRecordKey(pack).c_str(), pack->getVersion());
        userDefault->flush();
    }
}

std::string ResourcePackManager::getSaveDownloadedResourcePackRecordKey(ResourcePack *pack) {
    if (NULL != pack) {
        return pack->getName() + "PackageVersion";
    }
    return "";
}

void ResourcePackManager::handleSuccessCallback() {
    if (NULL != m_delegate) {
        m_delegate->onSuccess(getCurrentDownloadingResourcePack());
    }
}

void ResourcePackManager::handleFailureCallback(TNAssetsManagerActionCode code) {
    if (NULL != m_delegate) {
		m_delegate->onError(getCurrentDownloadingResourcePack(), code);
        /*switch (code) {
            case kCreateFileError:
                m_delegate->onError(getCurrentDownloadingResourcePack(), "create file fail");
                break;
            case kInvalidAssetData:
                m_delegate->onError(getCurrentDownloadingResourcePack(), "invalid asset data");
                break;
            case kDownload:
            {
                m_delegate->onError(getCurrentDownloadingResourcePack(), "download fail");
            }
                break;
            case kVerifyPackage:
            {
                m_delegate->onError(getCurrentDownloadingResourcePack(), "verify fail");
            }
                break;
            case kUncompress:
            {
                m_delegate->onError(getCurrentDownloadingResourcePack(), "uncompress fail");
            }
                break;
        }*/
    }
}

void ResourcePackManager::handleProgressCallback(ResourcePack *pack, int progress) {
    if (NULL != m_delegate && NULL != pack) {
        m_delegate->onProgress(pack, progress);
    }
}

void ResourcePackManager::execute(TNAssetsManagerActionCode code) {
    switch (code) {
        case kDownload:
        {
            m_assetsManager->download();
        }
            break;
        case kVerifyPackage:
        {
            m_assetsManager->verify();
        }
            break;
        case kUncompress:
        {
            m_assetsManager->uncompress();
        }
            break;
        default:
            break;
    }
}

void ResourcePackManager::executeFlows(std::vector<TNAssetsManagerActionCode> &flowVector, int index) {
    if (index >= 0 && index < flowVector.size()) {
        addUnhandledAsset(flowVector[index]);
        if (m_assetsManager->assetCount() > 0) {
            execute(flowVector[index]);
        } else {
            ++index;
            executeFlows(flowVector, index);
        }
    } else {
        m_isFlowDone = true;
    }
}

void ResourcePackManager::executeFlow(TNAssetsManagerActionCode code) {
    std::vector<TNAssetsManagerActionCode> v;
    v.push_back(code);
    executeFlows(v);
}

void ResourcePackManager::addUnhandledAsset(TNAssetsManagerActionCode code) {
    m_assetsManager->removeAllAssetDatas();
    if (m_currentIndex >= 0 && m_currentIndex < m_resourcePacks->count()) {
        //取出当前皮肤包的所有子包
        ResourcePack *pack = (ResourcePack*)m_resourcePacks->objectAtIndex(m_currentIndex);
        CCArray *assets = pack->unpack();
        if (NULL != assets) {
            for (int i = 0; i < assets->count(); ++i) {
                TNAssetData *data = (TNAssetData*)assets->objectAtIndex(i);
                if (!checkActionRecord(code, data)) {
                    m_assetsManager->addAssetData(data);
                }
            }
        }
    }
}

void ResourcePackManager::saveActionRecord(TNAssetsManagerActionCode code, TNAssetData *data) {
    if (NULL != data) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

        std::string hashKey = getActionRecordKey(data);
        userDefault->setIntegerForKey(hashKey.c_str(), code);

        userDefault->flush();
    }
}

void ResourcePackManager::clearAllActionRecords() {
    ResourcePack *pack = (ResourcePack*)m_resourcePacks->objectAtIndex(m_currentIndex);
    CCArray *assets = pack->unpack();
    if (NULL != assets) {
        for (int i = 0; i < assets->count(); ++i) {
            TNAssetData *data = (TNAssetData*)assets->objectAtIndex(i);
            clearActionRecord(data);
        }
    }
}

void ResourcePackManager::clearActionRecord(TNAssetData *data) {
    if (NULL != data) {
        std::string name = getActionRecordKey(data);
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
        userDefault->setIntegerForKey(name.c_str(), -1);
        userDefault->flush();
    }
}

bool ResourcePackManager::checkActionRecord(TNAssetsManagerActionCode code, TNAssetData *data) {
    if (NULL != data) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

        std::string name = getActionRecordKey(data);
        int actionCode = userDefault->getIntegerForKey(name.c_str());
        return actionCode >= code;
    }
    return false;
}

std::string ResourcePackManager::getActionRecordKey(TNAssetData *data) {
    if (NULL != data) {
        std::string encryptString = data->getDownloadUrl() + data->getName() + data->getVersion();
        return Crypto::MD5String((void*)encryptString.c_str(), encryptString.length());
    }
    return "";
}
