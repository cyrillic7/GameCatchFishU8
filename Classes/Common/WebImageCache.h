/**
 * @file WebImageCache
 * @brief  网络图片下载，缓存
 * @author tjl
 * @date 15-5-28
 */
#ifndef __WebImageCache_H_
#define __WebImageCache_H_

#include "cocos2d.h"
//#include "CCLuaEngine.h"

#include "network/HttpRequest.h"
#include "network/HttpClient.h"

//#include "netWork/cocos2dx_extra.h"
//#include "netWork/CCHTTPRequestDelegate.h"
//#include "netWork/CCHTTPRequest.h"
//USING_NS_CC_EXTRA;

USING_NS_CC;
using namespace network;



typedef void (Ref::*WebImageCacheCallback)(const char* savingPath, const char* tag, bool fromDisk);
#define webimagecache_selector(_SELECTOR) (WebImageCacheCallback)(&_SELECTOR)

class WebImageRequestObject : public Ref {
public:
    WebImageRequestObject() {
        imageUrl = "";
        retryCount = 0;
        tag = "";
        target = NULL;
        callback = NULL;
        listener = 0;
        forLua = false;
        m_httpRequest = NULL;
    };
    
    ~WebImageRequestObject() {
        target = NULL;
        CC_SAFE_RELEASE_NULL(m_httpRequest);
    };
    
    CREATE_FUNC(WebImageRequestObject);
    bool init(){return true;};

    std::string     imageUrl;
    int             retryCount;
    std::string     tag;
    bool            forLua;

    //for c++
    Ref        *target;
    WebImageCacheCallback   callback;

    //for lua
    int         listener;
    
	HttpRequest*   m_httpRequest;

	void setHttpRequest(HttpRequest* req)  { m_httpRequest = req;}
	HttpRequest* getHttpRequest() const {return m_httpRequest;}
    //CC_SYNTHESIZE_RETAIN(HttpRequest*, m_httpRequest, HttpRequest);
};

class WebImageCache : public Ref   //, public HTTPRequestDelegate

{
    WebImageCache();
    ~WebImageCache();

public:
    static WebImageCache* sharedInstance();
    static void purgeInstance();

    void imageWithUrl(const char* url, Ref *target, WebImageCacheCallback callback, const char* tag = NULL);
    //void imageWithUrl(const char* url, LUA_FUNCTION listener, const char* tag = NULL);
    
    // 内部功能函数
    void imageWithUrl(const char* url, Ref *target, WebImageCacheCallback callback, int listener, const char* tag = NULL);

    void requestFinished(HttpClient* client , HttpResponse* rsp);
    //virtual void requestFailed(HTTPRequest* request);
    
    //从队列中删除request
    void removeRequest(CCArray* array);

    //并发数, 默认为3
    void setConcurrentCount(int count);
    int getConcurrentCount();
    
    //清除所有请求
    void clearAllRequests();

	void clearAllRequestsForTarget(Ref *target);
    
    //获取对应图片保存的路径
    std::string getSavePath(const char *url);

private:
    //检查本地是否存在指定文件
    bool isExistFile(const char* filePath);
    
    //处理请求成功后数据
    void handleRequestData(void* data, int length, const char* url);

    //检查是否已经存在相同的请求, 根据url来判定是否为相同请求, 存在返回true
    bool isExistRequest(const char* url);

    //判断是否有相同的url请求已经在请求了
    bool isExistRequestSending(const char* url);

    //发送请求
    void sendRequest();

    //根据url来获取对应请求, 获取此url对应的所有请求数据
    CCArray *getRequestObject(const char* url);

    //执行回调函数
    void doCallback(const char* url, const char* savingPath, bool fromDisk);
    void callLuaCallback(int listener, const char* savingPath, const char* tag, bool fromDisk);
    void callCppCallback(Ref *target, WebImageCacheCallback callback, const char* savingPath, const char* tag, bool fromDisk);

    //请求失败的处理
    void handleRequestFail(const char* url);

    //根据url hash生成保存文件的名称
    std::string getSaveFileName(const char* url);
    
private:
    //正在请求的队列
    CCArray     *m_requestQueue;

    //等待请求的队列
    CCArray     *m_waitQueue;

    //并发数
    int         m_concurrentCount;
};

#endif //__WebImageCache_H_
