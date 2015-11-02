/**
 * @file ResourceLoader
 * @brief 异步资源加载工具
 * @author tjl
 * @date
 */

#ifndef __ResourceLoader_H_
#define __ResourceLoader_H_

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    ResourceTypeUnknown = -1,
    ResourceTypeImage, // 图片
    ResourceTypePlist, // 打包图片
    ResourceTypeArmature, // 动画资源
    ResourceTypeSound, // 声音
    ResourceTypeMAx,
} ResourceType;

class ResourceDefinition;

class ResourceLoaderDelegate {
public:
    // 一个资源被加载成功后，调用该函数
    // resourceLeftToLoad，剩余等待加载的资源数
    virtual void oneResourceLoaded(ResourceDefinition* d, int resourceLeftToLoad) = 0;
};

class ResourceDefinition : public Ref {
public:
    CREATE_FUNC(ResourceDefinition);
    bool init() {return true;};
    static ResourceDefinition* create(const char* path, ResourceType type) {
        ResourceDefinition* d = ResourceDefinition::create();
        d->setPath(path);
        d->setType(type);
        return d;
    }

    // 资源路径
//    const char* m_path;
    std::string m_path;
    void setPath(const char* p) {
        m_path = (NULL == p ? "" : FileUtils::sharedFileUtils()->fullPathForFilename(p));
    }
    std::string getPath() {return m_path;}

    // 资源类型
    CC_SYNTHESIZE(ResourceType, m_type, Type);
};

class ResourceLoader : public Ref {
public:
    ResourceLoader();
    virtual ~ResourceLoader();

    // 代理类必须实现
    static ResourceLoader* create(ResourceLoaderDelegate* delegate);
    bool init(ResourceLoaderDelegate* d);

    // loader的拥有者，必须在自身清空前，调用函数
    void purge();

    // 异步加载资源，数组是ResourceDefinition
    // 如果是image，则加入CCTextureCache, 如果是plist则加入SpriteFrameCache
    // 资源的完整路径不能有重复的
    // 如果是CCArmature则configFileName和plist，image三个文件除了后缀，必须同名
    void loadResourceAsync(__Array* resources);

    // 每一帧加载一个资源   未实现
    void loadResourceEveryUpdate(__Array* resources);

    // 保存还未加载成功的资源数量
    // 用这个值判断当前是否有还未加载完的资源, 如果有，就不要加载新的资源
    CC_SYNTHESIZE(int, m_resourceNotLoadedCount, ResourceNotLoadedCount);

    // 需要加载的资源
    CC_SYNTHESIZE(__Array*, m_resources, Resources);

    //设置并发数, 默认为1
    CC_SYNTHESIZE(int, m_concurrentCount, ConcurrentCount);
    
    static Image::Format computeImageFormat(std::string filename);

private:
    // 异步加载一个加载资源
    void loadOneResourceAsync();

    //加载多个资源
    void loadMoreResourcesAsync();
    void loadMoreResourcesAsyncCallback(float dt);
    void createLoadThread(int index);

    // 异步加载纹理的回调函数
    void loadTextureCallback(Texture2D* texture);

    // 异步加载动画的回调函数
    void loadArmatureCallback(float dt);

    // 资源加载共同处理
    void loadResourceCommonFunc(ResourceDefinition*);

    // 获取等待加载的的资源
    ResourceDefinition* getResourceNeedLoad();

    // 代理
    ResourceLoaderDelegate* m_delegate;

    //当前加载到了那个资源
    int     m_currentLoadIndex;
    //当前已经加载了多少个资源
    int     m_loadedCount;
};

#endif //__ResourceLoader_H_
