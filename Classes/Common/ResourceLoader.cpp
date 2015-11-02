#include "ResourceLoader.h"
#include "Common/pystring.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
#include <thread>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#else
#include "CCPThreadWinRT.h"
#include <ppl.h>
#include <ppltasks.h>
using namespace concurrency;
#endif

//TODO upgrate cocos
static std::mutex		s_ImageInfoMutex;
//static pthread_mutex_t      s_ImageInfoMutex;

USING_NS_CC_EXT;


typedef struct __ResourceInfo
{
    ResourceDefinition *res;
    Image        *image;
    Image::Format    imageType;
} ResourceInfo;

static std::queue<ResourceInfo*>*   s_pResourceQueue = NULL;

Image::Format ResourceLoader::computeImageFormat(std::string filename) {
    Image::Format ret = Image::Format::UNKOWN;

    if ((std::string::npos != filename.find(".jpg")) || (std::string::npos != filename.find(".jpeg")))
    {
        ret = Image::Format::JPG;
    }
    else if ((std::string::npos != filename.find(".png")) || (std::string::npos != filename.find(".PNG")))
    {
        ret = Image::Format::PNG;
    }
    else if ((std::string::npos != filename.find(".tiff")) || (std::string::npos != filename.find(".TIFF")))
    {
        ret = Image::Format::TIFF;
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
    else if ((std::string::npos != filename.find(".webp")) || (std::string::npos != filename.find(".WEBP")))
    {
        ret = Image::Format::WEBP;
    }
#endif

    return ret;
}

ResourceLoader::ResourceLoader()
: m_delegate(NULL),
  m_resourceNotLoadedCount(0),
  m_resources(NULL),
  m_concurrentCount(1),
  m_currentLoadIndex(0),
  m_loadedCount(0)
{

}

ResourceLoader::~ResourceLoader() {
    m_delegate = NULL;
    CC_SAFE_RELEASE_NULL(m_resources);
}

ResourceLoader *ResourceLoader::create(ResourceLoaderDelegate *delegate) {
    ResourceLoader* l = new ResourceLoader();
    if (NULL == l || !l->init(delegate)) {
        CC_SAFE_RELEASE_NULL(l);
    }
    return l;
}

bool ResourceLoader::init(ResourceLoaderDelegate *d) {
    m_delegate = d;

    return true;
}

void ResourceLoader::purge() {
    m_delegate = NULL;
}

void ResourceLoader::loadResourceAsync(__Array *resources) {
    if (m_resourceNotLoadedCount <= 0 && NULL != resources) {
        m_resourceNotLoadedCount = resources->count();
        CC_SAFE_RELEASE(m_resources);
        m_resources = resources;
        m_resources->retain();
        if (m_resourceNotLoadedCount > 0) {
            if (m_concurrentCount > 1) {
                loadMoreResourcesAsync();
            } else {
                loadOneResourceAsync();
            }
        }
    }
}

void ResourceLoader::loadOneResourceAsync() {
    if (m_resources != NULL && m_resourceNotLoadedCount > 0) {
        ResourceDefinition* d = (ResourceDefinition*)m_resources->getObjectAtIndex(m_resources->count() - m_resourceNotLoadedCount);
        ResourceType type = d->getType();
        if (type == ResourceTypeImage) {
            CCTextureCache::sharedTextureCache()->addImageAsync(d->getPath().c_str(),
                    CC_CALLBACK_1(ResourceLoader::loadTextureCallback, this));
        } else if (type == ResourceTypePlist) {
            std::string s = d->getPath();
            std::string textureName = pystring::replace(s, ".plist", ".png", 1);
            CCTextureCache::sharedTextureCache()->addImageAsync(textureName.c_str(),
                    CC_CALLBACK_1(ResourceLoader::loadTextureCallback, this));

        } else if (type == ResourceTypeArmature) {
            ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(d->getPath().c_str(),
                    this, schedule_selector(ResourceLoader::loadArmatureCallback));

        } else if (d->getType() == ResourceTypeSound) {
            // do nothing, 声音请用loadResourceEveryUpdate
        }
    }
}

void ResourceLoader::loadResourceEveryUpdate(__Array *resources) {
}

void ResourceLoader::loadTextureCallback(Texture2D* obj) {
    ResourceDefinition* d = getResourceNeedLoad();

    if (d->getType() == ResourceTypePlist) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(d->getPath().c_str());
    }

    loadResourceCommonFunc(d);
}

void ResourceLoader::loadArmatureCallback(float dt) {
    ResourceDefinition* d = getResourceNeedLoad();

    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(d->getPath().c_str());

   loadResourceCommonFunc(d);
}

void ResourceLoader::loadResourceCommonFunc(ResourceDefinition* d) {
    m_resourceNotLoadedCount--;

    if (NULL != m_delegate) {
        m_delegate->oneResourceLoaded(d, m_resourceNotLoadedCount);
    }

    // 加载下一个资源
    loadOneResourceAsync();
}

ResourceDefinition *ResourceLoader::getResourceNeedLoad() {
    if (NULL != m_resources && m_resourceNotLoadedCount > 0) {
        return (ResourceDefinition*)m_resources->getObjectAtIndex(m_resources->count() - m_resourceNotLoadedCount);
    }
    return NULL;
}

void ResourceLoader::loadMoreResourcesAsync() {
    if (NULL == s_pResourceQueue) {
        s_pResourceQueue = new std::queue<ResourceInfo*>();
        Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(ResourceLoader::loadMoreResourcesAsyncCallback), this, 0, false);
    }
    if (NULL != m_resources && m_resources->count() > 0) {
        int leftCount = m_resources->count() - m_currentLoadIndex;
        if (leftCount > 0) {
            int loadCount = leftCount > m_concurrentCount ? m_concurrentCount : leftCount;
            m_loadedCount += loadCount;
            for (int i = m_currentLoadIndex; i < m_currentLoadIndex + loadCount; ++i) {
                createLoadThread(i);
            }
        }
    }
}

static void* loadImageData(void *data) {
    ResourceDefinition *r = (ResourceDefinition*)data;
    if (NULL != r) {
        ResourceInfo *pImageInfo = new ResourceInfo();
        pImageInfo->res = r;

        std::string path = r->getPath();
        if (r->getType() == ResourceTypePlist) {
            path = pystring::replace(path, ".plist", ".png", 1);
        }
        
        // compute image type
        Image::Format imageType = ResourceLoader::computeImageFormat(path);

        // generate image
        Image *pImage = new (std::nothrow) Image();
        if (imageType == Image::Format::UNKOWN || !pImage->initWithImageFileThreadSafe(path.c_str()))
        {
            CC_SAFE_RELEASE(pImage);
            pImageInfo->image = NULL;
        } else {
            // generate image info
            pImageInfo->image = pImage;
            pImageInfo->imageType = imageType;
        }
        // put the image info into the queue
        s_ImageInfoMutex.lock();
        s_pResourceQueue->push(pImageInfo);
        s_ImageInfoMutex.unlock();
    }
    return NULL;
}

void ResourceLoader::createLoadThread(int index) {
    if (index >= 0 && index < m_resources->count()) {
        ResourceDefinition *r = (ResourceDefinition*)m_resources->getObjectAtIndex(index);
        //pthread_mutex_init(&s_ImageInfoMutex, NULL);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
		std::thread _tid = std::thread(&loadImageData, r);
		_tid.detach();
//#endif
    }
}

void ResourceLoader::loadMoreResourcesAsyncCallback(float dt) {
	s_ImageInfoMutex.lock();
    if (s_pResourceQueue->empty())
	{
		s_ImageInfoMutex.unlock();
    }
    else
    {
        m_currentLoadIndex ++;
        ResourceInfo *pImageInfo = s_pResourceQueue->front();
		s_pResourceQueue->pop();
		s_ImageInfoMutex.unlock();

        CCImage *pImage = pImageInfo->image;

        // generate texture in render thread
        ResourceDefinition *res = pImageInfo->res;
        if (NULL != res) {
            std::string path = pystring::replace(res->getPath(), ".plist", ".png", 1);
            CCTextureCache::sharedTextureCache()->addUIImage(pImage, path.c_str());
        }
        if (res->getType() == ResourceTypePlist) {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(res->getPath().c_str());
        }

        pImage->release();
        delete pImageInfo;
        pImageInfo = NULL;

        if (m_currentLoadIndex < m_resources->count()) {
            if (m_currentLoadIndex == m_loadedCount) {
                loadMoreResourcesAsync();
            }
        }

        //全部加载完
        if (m_currentLoadIndex == m_resources->count()) {
            Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(ResourceLoader::loadMoreResourcesAsyncCallback), this);

            //pthread_mutex_destroy(&s_ImageInfoMutex);
            delete s_pResourceQueue;
            s_pResourceQueue = NULL;
        }
		//进度条显示
		if (NULL != m_delegate) {
			m_delegate->oneResourceLoaded(NULL, m_resources->count() - m_currentLoadIndex);
		}
    }
}
