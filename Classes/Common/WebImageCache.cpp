#include "WebImageCache.h"
//#include <boost/thread/mutex.hpp>
#include "CommonFunction.h"
#include "crypto/CCCrypto.h"

//#include "MD5/MD5.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  #import "AFNetworking.h"

static AFHTTPRequestOperationManager* __manager = nil;

#endif

//默认并发数
#define DEFAULT_CONCURRENT_COUNT        3

//默认重试次数
#define DEFAULT_RETRY_COUNT             3

std::mutex    webImageCacheMutex;

static WebImageCache *__instance = NULL;

WebImageCache::WebImageCache()
: m_concurrentCount(DEFAULT_CONCURRENT_COUNT) {
    m_requestQueue = CCArray::create();
    m_requestQueue->retain();

    m_waitQueue = CCArray::create();
    m_waitQueue->retain();
}

WebImageCache::~WebImageCache() {
    CC_SAFE_RELEASE_NULL(m_requestQueue);
    CC_SAFE_RELEASE_NULL(m_waitQueue);
}

WebImageCache *WebImageCache::sharedInstance() {
    if (NULL == __instance) {
        __instance = new WebImageCache;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // AFNetworking 的 manager
        __manager = [AFHTTPRequestOperationManager manager];
        AFHTTPResponseSerializer *s = [AFHTTPResponseSerializer serializer];
        [__manager setResponseSerializer:s];
        __manager.shouldUseCredentialStorage = NO;
#endif
    }
    return __instance;
}

void WebImageCache::purgeInstance() {
    CC_SAFE_DELETE(__instance);
}

void WebImageCache::setConcurrentCount(int count) {
    if (count > 0) {
        m_concurrentCount = count;
    }
}

int WebImageCache::getConcurrentCount() {
    return m_concurrentCount;
}

void WebImageCache::clearAllRequests() {
    for (int i = 0; i < m_requestQueue->count(); i ++) {
        WebImageRequestObject *obj = (WebImageRequestObject*)m_requestQueue->objectAtIndex(i);
        if (NULL != obj->getHttpRequest()) {
           // obj->getHttpRequest()->cancel();
			//add by tjl
			obj->target = NULL;
			obj->callback = NULL;
        }
    }
    m_requestQueue->removeAllObjects();
    m_waitQueue->removeAllObjects();
}

void WebImageCache::clearAllRequestsForTarget( Ref *target ) {
	if (NULL == target) {
		return;
	}
	for (int i = m_requestQueue->count() - 1; i >=0 ; i --) {
		WebImageRequestObject *obj = (WebImageRequestObject*)m_requestQueue->objectAtIndex(i);
		if (obj->target == target && NULL != obj->getHttpRequest()) {
			//obj->getHttpRequest()->cancel();
			//add by tjl
			obj->target = NULL;
			obj->callback = NULL;
			m_requestQueue->fastRemoveObject(obj);
		}
	}
	for (int i = m_waitQueue->count() - 1; i >= 0; i--) {
		WebImageRequestObject *obj = (WebImageRequestObject*)m_waitQueue->objectAtIndex(i);
		if (obj->target == target) {
			m_waitQueue->fastRemoveObject(obj);
		}
	}
}

void WebImageCache::imageWithUrl(const char* url, Ref *target, WebImageCacheCallback callback, int listener, const char* tag) {
    if (NULL != url) {
        std::string correctUrl = CommonFunction::correctUrl(url);
        std::string path = getSavePath(correctUrl.c_str());
        //std::string path = correctUrl.c_str();
        // 如果图片，已经在本地，直接返回
        if (CommonFunction::isFileExist(path.c_str())) {
            CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());
            
            if (NULL != texture) {
                if (NULL != target) {
                    callCppCallback(target, callback, path.c_str(), tag, true);
                } else {
                    callLuaCallback(listener, path.c_str(), tag, true);
                }              
                
                return;
            }
        }
        
        // 保存请求
        WebImageRequestObject *obj = WebImageRequestObject::create();
        obj->imageUrl = correctUrl;
        if (NULL != tag) {
            obj->tag = tag;
        }
        if (NULL != target) {
            obj->target = target;
            obj->callback = callback;
            obj->forLua = false;
        } else {
            obj->listener = listener;
            obj->forLua = true;
        }
        m_waitQueue->addObject(obj);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString* urlString = [NSString stringWithUTF8String:correctUrl.c_str()];
        NSURL *iosURL = [NSURL URLWithString:[urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:iosURL];
        
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
//        operation.outputStream = [NSOutputStream outputStreamToFileAtPath:[NSString stringWithUTF8String:path.c_str()] append:NO];
        
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            NSData* data = (NSData*)responseObject;
            if (nil != data) {
                [data writeToFile:[NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding] atomically:YES];
                WebImageCache::sharedInstance()->doCallback(correctUrl.c_str(), path.c_str(), false);
                
                //printHttpDataLog("[ WebImageCache ], [ download image succeed ], url: %s", correctUrl.c_str());
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              // ???? 是否应该删掉
//            WebImageCache::sharedInstance()->handleRequestFail(correctUrl.c_str());
            //printHttpDataLog("[WebImageCache], [request failed, url: %s], [error mesage : %s]", correctUrl.c_str(), [[error description] cStringUsingEncoding:NSUTF8StringEncoding]);
        }];

        [__manager.operationQueue addOperation:operation];
      
        //printHttpDataLog("[ WebImageCache ], [ download image.. ], url: %s", correctUrl.c_str());
        
#else
        sendRequest();
#endif
    }
}

void WebImageCache::imageWithUrl(const char *url, Ref *target, WebImageCacheCallback callback, const char *tag) {
    imageWithUrl(url, target, callback, NULL, tag);
}

//void WebImageCache::imageWithUrl(const char *url, LUA_FUNCTION listener, const char *tag) {
//    imageWithUrl(url, NULL, NULL, listener, tag);
//}

bool WebImageCache::isExistRequest(const char* url) {
    if (NULL != url) {
        bool exist = false;
        for (int i = 0; i < m_waitQueue->count(); i ++) {
            WebImageRequestObject *obj = (WebImageRequestObject*)m_waitQueue->objectAtIndex(i);
            if (obj->imageUrl == std::string(url)) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            for (int i = 0; i < m_requestQueue->count(); i ++) {
                WebImageRequestObject *obj = (WebImageRequestObject*)m_requestQueue->objectAtIndex(i);
                if (obj->imageUrl == std::string(url)) {
                    exist = true;
                    break;
                }
            }
        }
        return exist;
    }
    return false;
}

bool WebImageCache::isExistRequestSending(const char* url) {
    if (NULL != url) {
        bool exist = false;
        for (int i = 0; i < m_requestQueue->count(); i ++) {
            WebImageRequestObject *obj = (WebImageRequestObject*)m_requestQueue->objectAtIndex(i);
            if (obj->imageUrl == std::string(url)) {
                exist = true;
                break;
            }
        }
        return exist;
    }
    return false;
}

bool WebImageCache::isExistFile(const char *filePath) {
    if (NULL != filePath) {
        return FileUtils::sharedFileUtils()->isFileExist(filePath);
    }
    return false;
}

void WebImageCache::sendRequest() {
    if (m_requestQueue->count() < m_concurrentCount && m_waitQueue->count() > 0) {
        for (int i = 0; i < m_waitQueue->count(); ++i) {
            WebImageRequestObject *obj = (WebImageRequestObject*)m_waitQueue->objectAtIndex(i);

            //判断是否存在相同url正在请求, 并且请求队列没有满
            if (!isExistRequestSending(obj->imageUrl.c_str()) && m_requestQueue->count() < m_concurrentCount) {
                m_requestQueue->addObject(obj);

				/* HTTPRequest *request = Network::createHTTPRequest(this, obj->imageUrl.c_str());

				request->setRequestUrl(obj->imageUrl.c_str());
				if (obj->tag.length() > 0) {
				request->setTag(obj->tag);
				}
				obj->setHttpRequest(request);*/

				HttpRequest* request = new HttpRequest();
				request->setUrl(obj->imageUrl.c_str());
			    request->setRequestType(HttpRequest::Type::GET);
				request->setResponseCallback(this, SEL_HttpResponse(&WebImageCache::requestFinished));
				request->setTag("GET test1");
				HttpClient::getInstance()->send(request);
				obj->setHttpRequest(request);

				//request->release(); 

            }
        }
        m_waitQueue->removeObjectsInArray(m_requestQueue);
    }
}

CCArray *WebImageCache::getRequestObject(const char *url) {
    if (NULL != url) {
        CCArray *tempArray = CCArray::create();
        for (int i = 0; i < m_requestQueue->count(); i ++) {
            WebImageRequestObject *obj = (WebImageRequestObject*)m_requestQueue->objectAtIndex(i);
            if (obj->imageUrl == std::string(url)) {
                tempArray->addObject(obj);
            }
        }
        for (int i = 0; i < m_waitQueue->count(); i ++) {
            WebImageRequestObject *obj = (WebImageRequestObject*)m_waitQueue->objectAtIndex(i);
            if (obj->imageUrl == std::string(url)) {
                tempArray->addObject(obj);
            }
        }
        return tempArray;
    }
    return NULL;
}

void WebImageCache::removeRequest(CCArray* array) {
    if (NULL != array) {
        m_requestQueue->removeObjectsInArray(array);
        m_waitQueue->removeObjectsInArray(array);
    }
}

void WebImageCache::handleRequestFail(const char *url) {
    if (NULL != url) {
        CCArray *objs = getRequestObject(url);
        if (NULL != objs) {
            for (int i = 0; i < objs->count(); ++i) {
                WebImageRequestObject *obj = (WebImageRequestObject*)objs->objectAtIndex(i);
                if (obj->retryCount < DEFAULT_RETRY_COUNT) {
                    obj->retryCount ++;
                    obj->setHttpRequest(NULL);
                    m_waitQueue->addObject(obj);
                }
            }
            m_requestQueue->removeObjectsInArray(objs);
            m_waitQueue->removeObjectsInArray(objs);
            
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
            sendRequest();
#endif
        }
    }
}

void WebImageCache::callLuaCallback(int listener, const char* savingPath, const char *tag, bool fromDisk) {
	/*if (listener != 0) {
	LuaStack *stack = LuaEngine::defaultEngine()->getLuaStack();
	stack->clean();
	stack->pushString(savingPath);
	if (NULL != tag) {
	stack->pushString(tag);
	} else  {
	stack->pushNil();
	}
	stack->executeFunctionByHandler(listener, 2);
	}*/
}

void WebImageCache::callCppCallback(Ref *target, WebImageCacheCallback callback, const char* savingPath, const char *tag, bool fromDisk) {
    if (NULL != target && NULL != callback) {
        (target->*callback)(savingPath, tag, fromDisk);
    }
}

void WebImageCache::doCallback(const char *url, const char* savingPath, bool fromDisk) {
    if (NULL != url) {
        CCArray *objList = getRequestObject(url);
        if (NULL != objList) {
            for (int i = 0; i < objList->count(); ++i) {
                WebImageRequestObject *obj = (WebImageRequestObject*)objList->objectAtIndex(i);
                if (obj->forLua) {
                    callLuaCallback(obj->listener, savingPath, obj->tag.c_str(), fromDisk);
                } else {
                    callCppCallback(obj->target, obj->callback, savingPath, obj->tag.c_str(), fromDisk);
                }
            }
            m_requestQueue->removeObjectsInArray(objList);
            m_waitQueue->removeObjectsInArray(objList);
            
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
            sendRequest();
#endif
        }
    }
}

void WebImageCache::requestFinished(HttpClient* client , HttpResponse* rsp) {
    std::vector<char>* responseData = rsp->getResponseData();
	std::string bufffff(responseData->begin(), responseData->end());

	int length = responseData->size();
	char* url = (char*)rsp->getHttpRequest()->getUrl();
	CCImage *tempImage = new CCImage;
	//TODO upgrade cocos
	tempImage->initWithImageData((unsigned char*)bufffff.c_str(),length);

	if (NULL != tempImage->getData() && tempImage->getDataLen() > 0) {
		
		CommonFunction::writeFile(getSavePath(url).c_str(), (char*)bufffff.c_str(), length);
		doCallback(url, getSavePath(url).c_str(), false);


	}
}

void WebImageCache::handleRequestData(void* responseData, int length, const char* url) {
    if ( NULL != responseData) {
        
        
        //boost::mutex::scoped_lock lck(webImageCacheMutex);
		std::lock_guard<std::mutex> lock(webImageCacheMutex);  
        
        CCImage *tempImage = new CCImage;
		//TODO upgrade cocos
        tempImage->initWithImageData((unsigned char*)responseData, length);
        
        if (NULL != tempImage->getData() && tempImage->getDataLen() > 0) {
//            CCTexture2D *texture = new CCTexture2D();
//            texture->autorelease();
//            texture->initWithImage(tempImage);
            CommonFunction::writeFile(getSavePath(url).c_str(), (char*)responseData, length);
            doCallback(url, getSavePath(url).c_str(), false);
        } else {
            
            
//            handleRequestFail(url);
        }
        
        if (NULL != tempImage) {
            delete  tempImage;
            tempImage = NULL;
        }
        
        free(responseData);
        responseData = NULL;
        
    } else {
        
        //printHttpDataLog("[ WebImageCache ], [ download image finished but response data is null ], url: %s", url);
        
        handleRequestFail(url);
    }
}

//void WebImageCache::requestFailed(HTTPRequest *request) {
//   // printHttpDataLog("[WebImageCache], [ request failed ] : error message: %s, error code: %d", request->getErrorMessage().c_str(), request->getErrorCode());
//    
//    handleRequestFail(request->getRequestUrl().c_str());
//}

std::string WebImageCache::getSaveFileName(const char *url) {
    if (NULL != url) {
        return Crypto::MD5String((void*)url, (int)strlen(url));
		/*MD5 m;
		m.ComputMd5(url,(int)strlen(url));
		return m.GetMd5();*/
    }
    return "";
}

std::string WebImageCache::getSavePath(const char *url) {
    std::string writablePath = FileUtils::sharedFileUtils()->getWritablePath();
	writablePath.append(getSaveFileName(url));
	
    return writablePath;
}