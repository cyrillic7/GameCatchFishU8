/**
 * @file ResourcePack.h
 * @brief  一个资源包
 * @author wudb
 * @date 15/1/28
 */
#ifndef __ResourcePack_H_
#define __ResourcePack_H_

#include "cocos2d.h"

USING_NS_CC;

class ResourcePack : public Ref {
public:
    ResourcePack();
    virtual ~ResourcePack();

    CREATE_FUNC(ResourcePack);
    virtual bool init();

    //名称
    CC_SYNTHESIZE(std::string, m_name, Name);

    //tag
    CC_SYNTHESIZE(std::string, m_tag, Tag);

    //版本
    CC_SYNTHESIZE(std::string, m_version, Version);

    //下载路径
    CC_SYNTHESIZE(std::string, m_downloadPath, DownloadPath);

    //子包数量
    CC_SYNTHESIZE(int, m_count, Count);

	//子包大小
	CC_SYNTHESIZE(int, m_pakcageSize, PakcageSize);


    //验证码
    CC_SYNTHESIZE(std::string, m_verifyCode, VerifyCode);

    //存储路径
    CC_SYNTHESIZE(std::string, m_storagePath, StoragePath);

    //序列化
    void save();

    //读取存储来恢复数据
    bool restore(const char* key);

    //清除自己的本地缓存
    void clearCache();

    //获得保存的key
    virtual std::string getSaveKey();

    //解包, 解开成指定数量的TNAssetData的子包
    CCArray* unpack();
};

#endif //__ResourcePack_H_
