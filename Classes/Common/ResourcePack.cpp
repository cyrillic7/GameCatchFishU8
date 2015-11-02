#include "ResourcePack.h"
#include "pystring.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "Platform/AssetsManager/TNAssetsManager.h"
#include "CommonFunction.h"

using namespace rapidjson;
USING_NS_CC_EXT;

//保存的key的前缀
#define ResourcePackSaveKeyPrefix   "ResourcePackSaveKeyPrefix_"

ResourcePack::ResourcePack() {
    m_name = "";
    m_version = "";
    m_tag = "";
    m_verifyCode = "";
    m_count = 0;
    m_downloadPath = "";
    m_storagePath = "";
	m_pakcageSize = 0;
}

ResourcePack::~ResourcePack() {

}

bool ResourcePack::init() {
    return true;
}

void ResourcePack::save() {
    Document doc;
    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value name(kStringType);
    name.SetString(m_name.c_str(), m_name.length());
    doc.AddMember("name", name, allocator);

    rapidjson::Value version(kStringType);
    version.SetString(m_version.c_str(), m_version.length());
    doc.AddMember("version", version, allocator);

    rapidjson::Value url(kStringType);
    url.SetString(m_downloadPath.c_str(), m_downloadPath.length());
    doc.AddMember("downloadPath", url, allocator);

    rapidjson::Value count(kNumberType);
    count.SetInt(m_count);
    doc.AddMember("count", count, allocator);

    rapidjson::Value verifyCode(kStringType);
    verifyCode.SetString(m_verifyCode.c_str(), m_verifyCode.length());
    doc.AddMember("verifyCode", verifyCode, allocator);
    
    rapidjson::Value storagePath(kStringType);
    storagePath.SetString(m_storagePath.c_str(), m_storagePath.length());
    doc.AddMember("storagePath", storagePath, allocator);

	rapidjson::Value packageSize(kNumberType);
	packageSize.SetInt(m_pakcageSize);
	doc.AddMember("packageSize", packageSize, allocator);

    rapidjson::StringBuffer  buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setStringForKey(getSaveKey().c_str(), buffer.GetString());
    userDefault->flush();
}

bool ResourcePack::restore(const char *key) {
    if (NULL != key) {
        CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

        std::string buffer = userDefault->getStringForKey(key);

        Document doc;
        doc.Parse<kParseDefaultFlags>(buffer.c_str());

        if (!doc.HasParseError()) {
            m_name = doc["name"].GetString();
            m_version = doc["version"].GetString();
            m_downloadPath = doc["downloadPath"].GetString();
            m_count = doc["count"].GetInt();
            m_verifyCode = doc["verifyCode"].GetString();
            m_storagePath = doc["storagePath"].GetString();
			m_pakcageSize = doc["packageSize"].GetInt();
            return true;
        }
    }
    return false;
}

void ResourcePack::clearCache() {
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setStringForKey(getSaveKey().c_str(), "");
    userDefault->flush();
}

std::string ResourcePack::getSaveKey() {
    return ResourcePackSaveKeyPrefix + m_name + m_version;
}

CCArray* ResourcePack::unpack() {
    if (m_name.length() > 0 && m_downloadPath.length() > 0 && m_count > 0 && m_verifyCode.length() > 0) {
        std::vector<std::string> v;
        pystring::split(m_verifyCode, v, ",");
        if (v.size() == m_count) {
            CCArray *assets = CCArray::createWithCapacity(m_count);
            std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_storagePath;
            for (int i = 1; i <= m_count; ++i) {
                TNAssetData *asset = TNAssetData::create();
                asset->setName(m_name);
                asset->setDownloadUrl(m_downloadPath);
                asset->setVersion(m_version);
                asset->setVerifyCode(v[i-1]);
                asset->setStoragePath(path);
				asset->setPackageSize(m_pakcageSize);
                assets->addObject(asset);
            }

//            printHttpDataLog("[ unpack ], [ name: %s, version: %s, url: %s ]", m_name.c_str(), m_version.c_str(), m_downloadPath.c_str());

            return assets;
        }
    }
    return NULL;
}