/**
 * @file ResourcePackManager.h
 * @brief  资源下载逻辑管理
 * @author wudb
 * @date 15/1/28
 */
#ifndef __ResourcePackManager_H_
#define __ResourcePackManager_H_

#include "cocos2d.h"
#include "Platform/AssetsManager/TNAssetsManager.h"
#include "ResourcePack.h"

USING_NS_CC;

//资源下载管理的协议
class ResourcePackProtocol {
public:
    //全部流程成功时回调, 如果是单个资源串行模式, 每个资源完成整个流程后也会回调
    virtual void onSuccess(ResourcePack *pack){};

    //失败回调
    virtual void onError(ResourcePack *pack, TNAssetsManagerActionCode code){};

    //当前资源包下载的进度回调, progress(0-100)
    virtual void onProgress(ResourcePack *pack, int progress){};
};


class ResourcePackManager : public Ref, public TNAssetsManagerDelegateProtocol {
public:
    ResourcePackManager();
    virtual ~ResourcePackManager();

    static ResourcePackManager *sharedInstance();
    static void purgeInstance();

    virtual bool init();

    //代理
    CC_SYNTHESIZE(ResourcePackProtocol*, m_delegate, Delegate);

    //创建存储的文件夹
    void createSaveDirectory(const char* path);

    //添加待下载的资源包
    void addResourcePack(ResourcePack *pack);

    //判断一个包是否已经下载了, true表示已经下载
    bool isResourcePackDownloaded(ResourcePack *pack);

	//add by tjl
	bool isResourceDownloaded(const char* gameName);

    //判断一个包是否正在下载, true表示正在下载
    bool isResourcePackDownloading(ResourcePack *pack);

	//判断一个任务是否在在下载列表中
	bool isInDownPackList(const char* packName);
    //获得下载列表
    CCArray *getResourcePackList();

    //获得当前正在下载的资源包
    ResourcePack* getCurrentDownloadingResourcePack();

    //开始下载
    void start();

    //清除, 停掉计时器, 停止下载
    void cleanup();

public: //TNAssetsManagerDelegateProtocol的回调
    //失败回调
    virtual void onError(TNAssetsManagerActionCode code, int index);

    //进度回调
    //percent   0-100
    virtual void onProgress(TNAssetsManagerActionCode code, int percent);

    //成功回调, 只有当一组资源的相应事件成功才会回调此方法
    virtual void onSuccess(TNAssetsManagerActionCode code, int index);

protected:
    void update(float dt);

    //----------保存和恢复下载相关-------------
    //保存与恢复, 用作退出, crash后继续下载用
    void restoreResourcePack();
    void saveResourcePack(ResourcePack *pack);
    //从存储记录中删除当前下载完的资源包
    void removeResourcePackFromDisk(ResourcePack *pack);
    //获得保存和恢复下载的资源列表的key
    virtual std::string getSaveResourcePackKey();

    //----------保存下载过的资源包的记录相关-------------
    //记录下载过的资源包
    void saveDownloadedResourcePackRecord(ResourcePack *pack);
    //获得保存下载记录的key, 规则:name + "PackageVersion"
    std::string getSaveDownloadedResourcePackRecordKey(ResourcePack *pack);

    //----------处理回调相关-------------
    void handleSuccessCallback();
    void handleFailureCallback(TNAssetsManagerActionCode code);
    void handleProgressCallback(ResourcePack *pack, int progress);

    //----------流程执行相关-------------
    //执行具体事件
    void execute(TNAssetsManagerActionCode code);
    //执行一系列流程
    void executeFlows(std::vector<TNAssetsManagerActionCode>& flowVector, int index = 0);
    //从某个动作开始执行流程
    void executeFlow(TNAssetsManagerActionCode code);
    //添加未处理过相应事件的资源
    void addUnhandledAsset(TNAssetsManagerActionCode code);

    //----------流程执行过程中action的记录相关-------------
    //保存动作成功的记录
    void saveActionRecord(TNAssetsManagerActionCode code, TNAssetData *data);
    //清除所有下载, 验证, 解压动作记录
    void clearAllActionRecords();
    void clearActionRecord(TNAssetData *data);
    //检查是否有相同动作的记录
    bool checkActionRecord(TNAssetsManagerActionCode code, TNAssetData *data);
    //生成url + name + version md5后的字符串为一条记录名称
    std::string getActionRecordKey(TNAssetData *data);

protected:
    //资源下载管理器
    TNAssetsManager     *m_assetsManager;

    //所有添加的需下载的资源列表
    CCArray             *m_resourcePacks;

    //当前正在下载的资源包
    int                 m_currentIndex;

    //标志一个资源的所有流程是否都已成功或者失败
    bool                m_isFlowDone;
};

#endif //__ResourcePackManager_H_
