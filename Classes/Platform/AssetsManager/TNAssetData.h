/**
 * @file TNAssetData.h
 * @brief  需要下载的资源的相关信息数据
 * @author wudb
 * @date 14/12/9
 */
#ifndef __TNAssetData_H_
#define __TNAssetData_H_

#include "cocos2d.h"

USING_NS_CC;

class TNAssetData : public Ref {
public:
    TNAssetData();
    ~TNAssetData();

    bool init(){return true;}
    CREATE_FUNC(TNAssetData);

    //名字
    CC_SYNTHESIZE(std::string, m_name, Name);

    //下载链接
    CC_SYNTHESIZE(std::string, m_downloadUrl, DownloadUrl);

    //版本信息
    CC_SYNTHESIZE(std::string, m_version, Version);

    //md5验证码
    CC_SYNTHESIZE(std::string, m_verifyCode, VerifyCode);

    //资源下载后存储的路径
    CC_SYNTHESIZE(std::string, m_storagePath, StoragePath);

	//资源包大小 签证包是否下载完
	 CC_SYNTHESIZE(int, m_packageSize, PackageSize);
};


#endif //__TNAssetData_H_
