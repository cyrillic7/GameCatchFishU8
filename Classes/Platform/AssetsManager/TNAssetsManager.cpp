#include "TNAssetsManager.h"
#include "crypto/CCCrypto.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "TNAssetsManagerIOS.h"
#endif

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <curl/curl.h>
#include <curl/easy.h>

#include <stdio.h>
#include <vector>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#include "unzip/unzip.h"
#include "base/CCScriptSupport.h"

using namespace std;

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

// Message type
#define ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED                0
#define ASSETSMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define ASSETSMANAGER_MESSAGE_PROGRESS                      2
#define ASSETSMANAGER_MESSAGE_ERROR                         3

// Some data struct for sending messages

struct ActionMessage
{
    int index; //当前成功或者失败的资源的索引
    TNAssetsManagerActionCode code; // 失败或者成功的码
    TNAssetsManager* manager;
};

struct ProgressMessage
{
    int percent;
    TNAssetsManagerActionCode code;
    TNAssetsManager* manager;
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
static NSMutableArray *m_assetManagerIOSArray = [[NSMutableArray alloc] init];

static TNAssetsManagerIOS* getAssetManagerInstance(TNAssetsManagerDelegateProtocol *delegate) {
    if (NULL != delegate) {
        for (int i = 0; i < [m_assetManagerIOSArray count]; i++) {
            TNAssetsManagerIOS *instance = [m_assetManagerIOSArray objectAtIndex:i];
            if ([instance getAssetsManagerDelegateProtocol] == delegate) {
                return instance;
            }
        }
    }
    return NULL;
}
#endif

// Implementation of AssetsManager

TNAssetsManager::TNAssetsManager()
:_curl(NULL)
, _threadIsRun(false)
, _connectionTimeout(0)
, _delegate(NULL)
, _scriptHandler(0)
, m_downloadIndex(0)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS
    _schedule = new Helper();

	m_assetDatas = CCArray::create();
    m_assetDatas->retain();
#endif
}

TNAssetsManager::~TNAssetsManager()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS
    unregisterScriptHandler();
	CC_SAFE_RELEASE_NULL(m_assetDatas);
#endif
}

void TNAssetsManager::cleanup() {
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS
    if (_schedule) {
        _schedule->release();
    }
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
#endif
}

void* downloadInThread(void *data) {
    TNAssetsManager* self = (TNAssetsManager*)data;

    bool isSuccess = true;
    for (int i = 0; i < self->m_assetDatas->count(); ++i) {
        TNAssetData *assetData = (TNAssetData*)self->m_assetDatas->objectAtIndex(i);
        if (assetData->getDownloadUrl().length() > 0 && assetData->getStoragePath().length() > 0 && assetData->getVersion().length() > 0) {
            //检查包体是否存在并且正确
            self->m_downloadIndex = i;
            if (!self->checkAssetPackageExist(assetData)) {
                if (! self->downLoad(assetData)) {
                    isSuccess = false;
                    self->sendErrorMessage(kDownload, i);
                    break;
                }
            }else {
                self->sendProgressMessage(kDownload, ((i + 1) * 100 / self->m_assetDatas->count()) );
            }
            self->sendSuccessMessage(kDownload, i);
            log("[ download sucess ] => [ downloadUrl: %s, version: %s ]", assetData->getDownloadUrl().c_str(), assetData->getVersion().c_str());
        } else {
            self->sendErrorMessage(kInvalidAssetData, i);
            log("[ download fail ] => [ downloadUrl: %s, version: %s ]", assetData->getDownloadUrl().c_str(), assetData->getVersion().c_str());
        }
    }

    if (isSuccess) { //全部下载成功
        self->sendSuccessMessage(kDownload, kAllAssetsFinishIndex);
    }

    if (self->_threadIsRun) {
        self->_threadIsRun = false;
    }
    return NULL;
}

void* uncompressInThread(void *data) {
    TNAssetsManager* self = (TNAssetsManager*)data;

    bool  isSuccess = true;
    for (int i = 0; i < self->m_assetDatas->count(); ++i) {
        TNAssetData *assetData = (TNAssetData*)self->m_assetDatas->objectAtIndex(i);
        // Uncompress zip file.
        if (! self->uncompress(assetData)) {
            isSuccess = false;
            self->sendErrorMessage(kUncompress, i);
            break;
        }
        self->sendProgressMessage(kUncompress, ((i + 1) * 100 / self->m_assetDatas->count()));
        self->sendSuccessMessage(kUncompress, i);
        log("[ uncompress success ] => [ url: %s, version: %s ]", assetData->getDownloadUrl().c_str(), assetData->getVersion().c_str());
    }

    if (isSuccess) { //表示解压缩成功了
        self->sendSuccessMessage(kUncompress, kAllAssetsFinishIndex);
	}

	if (self->_threadIsRun) {
		self->_threadIsRun = false;
	}
    return NULL;
}

void *verifyInThread(void *data) {
    TNAssetsManager* self = (TNAssetsManager*)data;

    bool isSuccess = true;
    for (int i = 0; i < self->m_assetDatas->count(); ++i) {
        TNAssetData *assetData = (TNAssetData*)self->m_assetDatas->objectAtIndex(i);
        if (! self->verifyAssetPackage(assetData)) {
            isSuccess = false;
            self->sendErrorMessage(kVerifyPackage, i);
            break;
        }
        self->sendProgressMessage(kVerifyPackage, ((i + 1) * 100 / self->m_assetDatas->count()));
        self->sendSuccessMessage(kVerifyPackage, i);
        log("[ verify success ] => [ url: %s, version: %s ]", assetData->getDownloadUrl().c_str(), assetData->getVersion().c_str());
    }

    if (isSuccess) { //表示验证成功了
        self->sendSuccessMessage(kVerifyPackage, kAllAssetsFinishIndex);
    }

	if (self->_threadIsRun) {
		self->_threadIsRun = false;
	}

    return NULL;
}

void TNAssetsManager::addAssetData(TNAssetData *data) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] addAssetData:data];
    [getAssetManagerInstance(_delegate) addAssetData:data];
#else
    if (NULL != data) {
        //记得加锁
        m_assetDataMutex.lock();
        m_assetDatas->addObject(data);

        log("[ add asset data ] => [ downloadUrl: %s, version: %s ]", data->getDownloadUrl().c_str(), data->getVersion().c_str());

        m_assetDataMutex.unlock();
    }
#endif
}

void TNAssetsManager::removeAllAssetDatas() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] removeAllAssets];
    [getAssetManagerInstance(_delegate) removeAllAssets];
#else
    if (NULL != m_assetDatas) {
        m_assetDatas->removeAllObjects();
    }
#endif
}

int TNAssetsManager::assetCount() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    return [[TNAssetsManagerIOS defaultInstance] assetCount];
    return [getAssetManagerInstance(_delegate) assetCount];
#else
    return m_assetDatas->count();
#endif
}

TNAssetData* TNAssetsManager::assetWithIndex(int index) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    return [[TNAssetsManagerIOS defaultInstance] assetWithIndex:index];
    return [getAssetManagerInstance(_delegate) assetWithIndex:index];
#else
    if (index >= 0 && index < m_assetDatas->count()) {
        return (TNAssetData*)m_assetDatas->objectAtIndex(index);
    }
    return NULL;
#endif
}

std::string TNAssetsManager::getZipFileName(TNAssetData *assetData) {
    if (NULL != assetData) {
        std::string fileName = assetData->getDownloadUrl() + assetData->getVersion();
        fileName = Crypto::MD5String((void*)(fileName.c_str()), (int)fileName.length()) + ".zip";
        return fileName;
    }
    return "";
}

bool TNAssetsManager::verifyAssetPackage(TNAssetData *assetData) {
    if (NULL != assetData && assetData->getVerifyCode().length() > 0 && checkAssetPackageExist(assetData)) {
        std::string path = assetData->getStoragePath() + getZipFileName(assetData);
        
        unsigned char output[Crypto::MD5_BUFFER_LENGTH] = {0};
        Crypto::MD5File(path.c_str(), output);
        std::string result; //= cocos2d::extra::CCCrypto::MD5String(output, cocos2d::extra::CCCrypto::MD5_BUFFER_LENGTH);
            
        char buf[32] = {0};
        for (int i = 0; i < Crypto::MD5_BUFFER_LENGTH; i++) {
            sprintf((char *)buf, "%02x", output[i]);
            result.append(buf);
        }
        
        log("[ verify ], [ url: %s, generateCode: %s, originCode: %s ]", assetData->getDownloadUrl().c_str(), result.c_str(), assetData->getVerifyCode().c_str());
            
        return result == assetData->getVerifyCode();
    }
    return false;
}

bool TNAssetsManager::checkAssetPackageExist(TNAssetData *assetData) {
	bool ret = false;
    if (NULL != assetData) {
        std::string path = assetData->getStoragePath() + getZipFileName(assetData);
		//存在且文件大小等于整包大小 for 断点续传
		/*	ssize_t outLength = 0;
		int _initPackSize = 0;
		unsigned char* pbuffer = FileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &outLength);
		if (CCFileUtils::sharedFileUtils()->isFileExist(path.c_str()) &&  outLength == assetData->getPackageSize())
		{
		ret =true;
		}
		delete []pbuffer;*/
       return CCFileUtils::sharedFileUtils()->isFileExist(path.c_str());
    }
    return ret;
}

void TNAssetsManager::download()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] download];
    [getAssetManagerInstance(_delegate) download];
#else
    if (_threadIsRun) return;
    
    // 1. Urls of package and version should be valid;
    if (m_assetDatas->count() == 0) {
        log("no asset file download");
        return;
    }

	_threadIsRun = true;
	_tid = std::thread(&downloadInThread, this);
	_tid.detach();
#endif
}

void TNAssetsManager::verify() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] verify];
    [getAssetManagerInstance(_delegate) verify];
#else
    if (_threadIsRun) return;

    // 1. Urls of package and version should be valid;
    if (m_assetDatas->count() == 0) {
        log("no asset file download");
        return;
    }

	_threadIsRun = true;
	_tid = std::thread(&verifyInThread, this);
	_tid.detach();
#endif
}

void TNAssetsManager::uncompress() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] uncompress];
    [getAssetManagerInstance(_delegate) uncompress];
#else
    if (_threadIsRun) return;

    // 1. Urls of package and version should be valid;
    if (m_assetDatas->count() == 0) {
        log("no asset file download");
        return;
    }

	_threadIsRun = true;
	_tid = std::thread(&uncompressInThread, this);
	_tid.detach();
#endif
}

bool TNAssetsManager::removeAssetPackage(int index) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    BOOL success = [[TNAssetsManagerIOS defaultInstance] removeAssetPackage:index];
    BOOL success = [getAssetManagerInstance(_delegate) removeAssetPackage:index];
    return success ? true : false;
#else
    if (index >= 0 && index < m_assetDatas->count()) {
        TNAssetData *data = (TNAssetData*)m_assetDatas->objectAtIndex(index);
        std::string filePath = data->getStoragePath() + getZipFileName(data);
        return (remove(filePath.c_str()) == 0 ? true : false);
    }
    return false;
#endif
}

void TNAssetsManager::removeAllAssetPackages() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] removeAllAssetPackages];
    [getAssetManagerInstance(_delegate) removeAllAssetPackages];
#else
    for (int i = 0; i < m_assetDatas->count(); ++i) {
        removeAssetPackage(i);
    }
#endif
}

bool TNAssetsManager::uncompress(TNAssetData *assetData)
{
    if (NULL == assetData) {
       return false;
    }
	// Open the zip file
    std::string outFileName = assetData->getStoragePath() + getZipFileName(assetData);
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        std::string fullPath = assetData->getStoragePath() + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
			// get all dir
			std::string fileNameStr = string(fileName);
            size_t position = 0;
			while((position=fileNameStr.find_first_of("/",position))!=string::npos)
			{
				string dirPath = assetData->getStoragePath() + fileNameStr.substr(0, position);
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
				if (!createDirectory(dirPath.c_str()))
            {
					CCLOG("can not create directory %s", dirPath.c_str());
					//unzClose(zipfile);
					//return false;
            }
				position++;
        }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");

	//原版少一句导致无法删除
	unzClose(zipfile);
    return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool TNAssetsManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	return FileUtils::getInstance()->createDirectory(path);
#elif (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int tnAssetsManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	// by mb
	if (totalToDownload == 0) totalToDownload = 1;

    TNAssetsManager* manager = (TNAssetsManager*)ptr;
//    int percent = manager->m_downloadIndex * (1 * 100 / manager->m_assetDatas->count()) + (int)(nowDownloaded/totalToDownload*100);
        int percent = manager->m_downloadIndex * (1 * 100 / manager->m_assetDatas->count()) + (int)(nowDownloaded/totalToDownload * (1 * 100 / manager->m_assetDatas->count()));
    manager->sendProgressMessage(kDownload, percent);

    return 0;
}

bool TNAssetsManager::downLoad(TNAssetData *assetData)
{
    if (NULL == assetData) {
        return false;
    }
	// Create a file to save package.
    int index = m_assetDatas->indexOfObject(assetData);
    std::string outFileName = assetData->getStoragePath() + getZipFileName(assetData);
    FILE *fp = fopen(outFileName.c_str(), "wb");
    if (! fp)
    {
        sendErrorMessage(kCreateFileError, index);
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
	//断点续传
	/*ssize_t outLength = 0;
	int _initPackSize = 0;
	unsigned char* pbuffer = FileUtils::sharedFileUtils()->getFileData(outFileName.c_str(), "rb", &outLength);
	delete []pbuffer;
	CCLOG("downLoad -- locate file = %s, %lu", outFileName.c_str(), outLength);
	curl_slist *http_headers = NULL;
	if(outLength > 0)
	{
			http_headers = curl_slist_append(http_headers, __String::createWithFormat("Range: bytes=%lu-", outLength)->getCString());
			_initPackSize = outLength;
	}*/

    // Download pacakge
	CURLcode res;
	_curl = curl_easy_init();
	//curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, http_headers);
	curl_easy_setopt(_curl, CURLOPT_URL, assetData->getDownloadUrl().c_str());
	curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, tnAssetsManagerProgressFunc);
    //curl_easy_setopt(_curl, CURLOPT_PROXY, "10.122.83.73:8888");
    //curl_easy_setopt(_curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
	if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (res != 0)
    {
        sendErrorMessage(kNetworkError, index);
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
//    CCLOG("succeed downloading package %s", assetData->getDownloadUrl().c_str());
    
    fclose(fp);
    return true;
}

void TNAssetsManager::setDelegate(TNAssetsManagerDelegateProtocol *delegate)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[TNAssetsManagerIOS defaultInstance] setAssetsManagerDelegateProtocol:delegate];
    TNAssetsManagerIOS *instance = [TNAssetsManagerIOS instance];
    [instance setAssetsManagerDelegateProtocol:delegate];
    [m_assetManagerIOSArray addObject:instance];
#endif
    _delegate = delegate;
}

void TNAssetsManager::registerScriptHandler(int handler)
{
    unregisterScriptHandler();
    _scriptHandler = handler;
}

void TNAssetsManager::unregisterScriptHandler(void)
{
    ScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(_scriptHandler);
    _scriptHandler = 0;
}

void TNAssetsManager::setConnectionTimeout(unsigned int timeout)
{
    _connectionTimeout = timeout;
}

unsigned int TNAssetsManager::getConnectionTimeout()
{
    return _connectionTimeout;
}

void TNAssetsManager::sendErrorMessage(TNAssetsManagerActionCode code, int index)
{
    Message *msg = new Message();
    msg->what = ASSETSMANAGER_MESSAGE_ERROR;
    
    ActionMessage *errorMessage = new ActionMessage();
    errorMessage->code = code;
    errorMessage->index = index;
    errorMessage->manager = this;
    msg->obj = errorMessage;
    
    _schedule->sendMessage(msg);
}

void TNAssetsManager::sendSuccessMessage(TNAssetsManagerActionCode code, int index) {
    Message *msg = new Message();
    msg->what = ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED;

    ActionMessage *successMessage = new ActionMessage();
    successMessage->code = code;
    successMessage->manager = this;
    successMessage->index = index;
    msg->obj = successMessage;

    _schedule->sendMessage(msg);
}

void TNAssetsManager::sendProgressMessage(TNAssetsManagerActionCode code, int percent) {
    TNAssetsManager::Message *msg = new TNAssetsManager::Message();
    msg->what = ASSETSMANAGER_MESSAGE_PROGRESS;

    ProgressMessage *progressData = new ProgressMessage();
    progressData->percent = percent;
    progressData->manager = this;
    progressData->code = code;
    msg->obj = progressData;

    _schedule->sendMessage(msg);
}

// Implementation of AssetsManagerHelper

TNAssetsManager::Helper::Helper()
{
    _messageQueue = new std::list<Message*>();
    Director::getInstance()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

TNAssetsManager::Helper::~Helper()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    delete _messageQueue;
}

void TNAssetsManager::Helper::sendMessage(Message *msg)
{
    _messageQueueMutex.lock();
    _messageQueue->push_back(msg);
    _messageQueueMutex.unlock();
}

void TNAssetsManager::Helper::update(float dt)
{
    Message *msg = NULL;
    
	// Returns quickly if no message
	_messageQueueMutex.lock();
    if (0 == _messageQueue->size())
	{
		_messageQueueMutex.unlock();
        return;
    }
    
    // Gets message
    msg = *(_messageQueue->begin());
	_messageQueue->pop_front();
	_messageQueueMutex.unlock();
    
    switch (msg->what) {
        case ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED:
            handleUpdateSucceed(msg);
            break;
        case ASSETSMANAGER_MESSAGE_PROGRESS:
            handleUpdateProgress(msg);
            break;
        case ASSETSMANAGER_MESSAGE_ERROR:
            handleUpdateError(msg);
            break;
        default:
            break;
    }
    
    delete msg;
}

void TNAssetsManager::Helper::handleUpdateSucceed(Message *msg)
{
    ActionMessage *message = (ActionMessage*)msg->obj;

    if (message->manager) {
        if (message->manager->_delegate) {
            message->manager->_delegate->onSuccess(message->code, message->index);
		}
    }
    delete message;
}

void TNAssetsManager::Helper::handleUpdateError(TNAssetsManager::Message *msg) {
    // error call back
    ActionMessage *message = (ActionMessage*)msg->obj;
    if (message->manager->_delegate) {
        message->manager->_delegate->onError(message->code, message->index);
    }
   
    delete message;
}

void TNAssetsManager::Helper::handleUpdateProgress(TNAssetsManager::Message *msg) {
    ProgressMessage *message = (ProgressMessage*)msg->obj;
    if (message->manager && message->manager->_delegate) {
        message->manager->_delegate->onProgress(message->code, message->percent);
    }
    delete message;
}

//#endif // CC_PLATFORM_WINRTTNAssetData* TNAssetsManager::assetWithIndex(int index){

