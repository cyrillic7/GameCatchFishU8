#ifndef __TNAssetsManager__
#define __TNAssetsManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "TNAssetData.h"
#include "TNAssetDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <string>
#include <curl/curl.h>
#include <thread>

class TNAssetsManagerDelegateProtocol;

//一组资源的下载, 验证, 解压等功能的管理类
class TNAssetsManager : public Ref
{
public:
    TNAssetsManager();
    virtual ~TNAssetsManager();

    CREATE_FUNC(TNAssetsManager);
    bool init(){return true;};

    //添加下载的资源
    void addAssetData(TNAssetData *data);
    void removeAllAssetDatas();

    //获取需要下载更新的资源列表的资源个数
    int assetCount();

    //根据index获取asset data
    TNAssetData* assetWithIndex(int index);

    //开始下载
    void download();

    //验证
    void verify();

    //解压
    void uncompress();

    //删除下载的包体
    bool removeAssetPackage(int index);
    void removeAllAssetPackages();

    //cleanup, you should call this function before delete this
    void cleanup();

    /** @brief Sets delegate, the delegate will receive messages
     */
    void setDelegate(TNAssetsManagerDelegateProtocol *delegate);
    
    /** @brief Register script handler, the hander will receive messages
     */
    void registerScriptHandler(int handler);
    void unregisterScriptHandler(void);

    /** @brief Sets connection time out in seconds
     */
    void setConnectionTimeout(unsigned int timeout);
    
    /** @brief Gets connection time out in secondes
     */
    unsigned int getConnectionTimeout();
    
    //add by wudb
    friend void* downloadInThread(void *);
    friend void* uncompressInThread(void *);
    friend void* verifyInThread(void*);

    friend int tnAssetsManagerProgressFunc(void *, double, double, double, double);

    //md5验证包体是否完整，正确, 未完成
    //true 表示包体一切ok
    //false 表示包体不可用
    bool verifyAssetPackage(TNAssetData *assetData);

    //检查对应url和版本的包体是否存在, 存在返回true, 否则返回false
    bool checkAssetPackageExist(TNAssetData *assetData);

    //把url和版本号连接起来, hash后作为Zip文件名
    std::string getZipFileName(TNAssetData *assetData);
    
protected:
    bool downLoad(TNAssetData *assetData);
    bool uncompress(TNAssetData *assetData);
    bool createDirectory(const char *path);
    void sendErrorMessage(TNAssetsManagerActionCode code, int index);
    void sendSuccessMessage(TNAssetsManagerActionCode code, int index);
    void sendProgressMessage(TNAssetsManagerActionCode code, int percent);

private:
    typedef struct _Message
    {
    public:
        _Message() : what(0), obj(NULL){}
        unsigned int what; // message type
        void* obj;
    } Message;
    
    class Helper : public Ref
    {
    public:
        Helper();
        ~Helper();
        
        virtual void update(float dt);
        void sendMessage(Message *msg);
        
    private:
        void handleUpdateSucceed(Message *msg);
        void handleUpdateError(Message *msg);
        void handleUpdateProgress(Message *msg);
        
        std::list<Message*> *_messageQueue;
        std::mutex _messageQueueMutex;
    };
    
private:
    CURL *_curl;
    Helper *_schedule;
    std::thread _tid;
	bool _threadIsRun;
    unsigned int _connectionTimeout;
    
    TNAssetsManagerDelegateProtocol *_delegate; // weak reference
    int _scriptHandler; // script handler

    std::mutex		m_assetDataMutex;
    CCArray         *m_assetDatas;

    //当前下载的资源索引
    int         m_downloadIndex;
};

#endif // CC_TARGET_PLATFORM != CC_PLATFORM_WINRT
//#endif /* defined(__TNAssetsManager__) */
