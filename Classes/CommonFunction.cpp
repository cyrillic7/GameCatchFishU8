#include "CommonFunction.h"
#include "SessionManager.h"
#include "cocostudio/CCSGUIReader.h"
#include "Common/pystring.h"
#include "Message.h"

#define JNI_PACKAGE "com/game/GameCatchFish/GameCatchFish"


#if  CC_PLATFORM_IOS == CC_TARGET_PLATFORM
#import "FXKeychain.h"
#import <Foundation/Foundation.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>
#endif


#if (defined(WIN32) && defined(_WINDOWS))
#include <winsock2.h>
#include <iphlpapi.h>
#include <direct.h>
#include <regex>
#pragma comment(lib, "IPHLPAPI.lib")
#endif


CommonFunction::CommonFunction()
{

}

CommonFunction::~CommonFunction()
{

}

float CommonFunction::CalcDistance(float x1, float y1, float x2, float y2) {
	return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float CommonFunction::CalcAngle(float x1, float y1, float x2, float y2) {
	float distance = CalcDistance(x1, y1, x2, y2);
	if (distance == 0.f) return 0.f;
	float sin_value = (y1 - y2) / distance;
	float angle = std::acos(sin_value);
	
	if (x1 < x2) angle= M_PI*2-angle;

	return angle;
}

#define FISH_NAME(fish_kind, name) case fish_kind: return name;
const char* fish_name(FishKind fish_kind) {
	switch (fish_kind) {
		FISH_NAME_MAP(FISH_NAME)
	default: {
		assert(false);
		return "fish invalid";
		}
	}
}
#undef FISH_NAME

const char* CommonFunction::fish_name_with_status(FishKind fish_kind, const char* status) {
	//static char fish_name_status[128];
	String* nameStr = String::createWithFormat("%s_%s", fish_name(fish_kind), status);
	return nameStr->getCString();
}

Animate *CommonFunction::createFrameAnimate(const std::vector<std::string> &imageNames, const float duration) {
	if (imageNames.size() > 0) {
		Vector<SpriteFrame*> arr;
		for (int i = 0; i < imageNames.size(); i ++) {
			SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(imageNames[i].c_str());
			if (NULL == frame) {
				CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(imageNames[i].c_str());
				if (NULL != texture) {
					frame = SpriteFrame::createWithTexture(texture, CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
				}
			}
			if (NULL != frame) {
				arr.pushBack(frame);
			}
		}
		Animation *animation = Animation::createWithSpriteFrames(arr, duration);
		return Animate::create(animation);
	}
	return NULL;
}

Animate* CommonFunction::createFrameAnimate(const std::string path, const float duration,int col ,int row, int frameW,int frameH)
{
	Vector<SpriteFrame*> frames;
	bool isReload = false;
	Texture2D* pTexture =  Director::sharedDirector()->getTextureCache()->getTextureForKey(path);
	if(pTexture)
		isReload = true;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			SpriteFrame* frame = nullptr;
			if(isReload)
				frame= SpriteFrame::createWithTexture(pTexture,CCRectMake(j*frameW,i*frameH,frameW,frameH));
			else
				frame = SpriteFrame::create(path,CCRectMake(j*frameW,i*frameH,frameW,frameH));
			
			if (NULL != frame) {
				frames.pushBack(frame);
			}
		}
	}
	Animation* anima = Animation::createWithSpriteFrames(frames,duration);
	return Animate::create(anima);

}

Widget *CommonFunction::getWidgetFromCocostudioFile(const char *fileName) {
	char name[100] = {0};
	sprintf(name, "%s.json", fileName);
	if (!CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(name))) {
		sprintf(name, "%s.csb", fileName);
		return cocostudio::GUIReader::shareReader()->widgetFromBinaryFile(name);
	} else {
		return cocostudio::GUIReader::shareReader()->widgetFromJsonFile(name);
	}
}

std::string CommonFunction::correctUrl(const char *url) {
	if (NULL != url) {
		std::string tempUrl = url;
		do {
			std::string::size_type pos = tempUrl.find("\\");
			if (pos == std::string::npos) {
				break;
			}
			tempUrl.replace(pos, 1, "/");
		} while (true);

		return pystring::rstrip(tempUrl);
	}
	return "";
}

bool CommonFunction::isFileExist(const char *pFilePath) {
	return FileUtils::sharedFileUtils()->isFileExist(pFilePath);
}

bool CommonFunction::writeFile(const char *path, const char *address, int len) {
	bool ret = false;
	FILE* fp = fopen(path, "wb");
	if (fp != NULL) {
		fwrite(address, 1, len, fp);
		fclose(fp);
		ret = true;
	}
	return ret;
}
std::string CommonFunction::getWritablePath() {
	return CCFileUtils::sharedFileUtils()->getWritablePath();
}


//随机浮点数
float CommonFunction::fgRangeRand( float min, float max )  
{  
	float rnd = ((float)rand() / (float)RAND_MAX);  
	return rnd * (max - min) + min; 
} 

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	#include "..\cocos2d\external\win32-specific\icon\include\iconv.h"
#else
	#include "..\libiconv\include\iconv.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#include <iconv.h>
#endif


static char g_GBKConvUTF8Buf[5000] = { 0 };


std::string  CommonFunction::GBKToUTF8(const char * strChar){
	std::string result = "";
	if (strcmp(strChar, "") == 0)
	{
		return result;
	}
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	iconvH = iconv_open("utf-8", "gbk");
	//iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength << 2;
	size_t copyLength = outLength;

	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#endif

	//memcpy(g_GBKConvUTF8Buf, pBuff, copyLength);
	result = pBuff;
	free(pBuff);
	iconv_close(iconvH);

	return result;
}

std::string CommonFunction::UTF8TOGBK(const char * strChar){
	std::string result = "";
	if (strcmp(strChar,"")==0)
	{
		return result;
	}
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	//iconvH = iconv_open("utf-8", "gb2312");
	iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength << 2;
	size_t copyLength = outLength;

	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#endif

	result = pBuff;
	free(pBuff);
	iconv_close(iconvH);
	return result;
}


std::string CommonFunction::getPlatformImageURL(const char* url)
{
	std::string imageUrl = url;
	size_t pos = imageUrl.find(".png");
	if (pos != std::string::npos)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (imageUrl.find("_ios") == std::string::npos)
		{
			imageUrl.insert(pos, "_ios");
		}		
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
	}

	return imageUrl;
}

std::string CommonFunction::truncationString(std::string str, int maxLength) {

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSString* iOSString = [NSString stringWithUTF8String:str.c_str()];
	if ([iOSString length] > maxLength) {
		NSRange stringRange = {0, static_cast<NSUInteger>(MIN([iOSString length] * MAX(0.4, [iOSString length] / str.length()), maxLength))};
		stringRange = [iOSString rangeOfComposedCharacterSequencesForRange:stringRange];
		NSString *shortString = [iOSString substringWithRange:stringRange];
		std::string newString([shortString cStringUsingEncoding:NSUTF8StringEncoding]);
		return newString + "...";
	} else {
		return str;
	}
#endif
	if (str.length() > maxLength) {
		unsigned char c = str.c_str()[maxLength-1];
		if (c >> 7 != 0 && maxLength > 1) {
			do {
				maxLength--;
				c = str.c_str()[maxLength-1];
				if (c >> 7 == 0) {
					break;
				}
				if (c >> 6 != 2) {
					if (maxLength > 0) {
						maxLength -= 1;
					}
					break;
				}
			} while (maxLength > 1);
		}
		return str.substr(0, maxLength) + "...";
	}
	return str;
}



void CommonFunction::addGray(Node* sp) {
	//TODO upgrade cocos(test)
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename("example_greyScale.fsh");
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
	sp->setShaderProgram(glprogram);
}

void CommonFunction::removeGray(Node* sp) {
	//TODO upgrade cocos(test)
	GLProgram* pProgram = GLProgramCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
	sp->setShaderProgram(pProgram);
}

#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

void CommonFunction::callJni(const char* data,int webType )
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"callJni","(Ljava/lang/String;I)V");//本人在此传递的参数是字符串
	if (!isHave) {
		log("jni->%s/callJni:此函数不存在", JNI_PACKAGE);
	}else{
		log("jni->%s/callJni:此函数存在", JNI_PACKAGE);
		jstring jdata = minfo.env->NewStringUTF(data);
		jint    jType = webType;
		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jdata,jType);
	}
	log("jni-java函数执行完毕");
#endif
}

void CommonFunction::callWx(int flag)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"callWx","(I)V");
	if(isHave)
	{
		jint    jFlag = flag;
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jFlag);
	}
#endif
}



void CommonFunction::callPay(const char* orderNo)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"callPay","(Ljava/lang/String;)V");
	if (!isHave) {
		log("jni->%s/callPay:is not exit", JNI_PACKAGE);
	}else{
		log("jni->%s/callPay:is exit", JNI_PACKAGE);
		jstring jdata = minfo.env->NewStringUTF(orderNo);
		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jdata);
	}
	log("jni-java excute finish");
#endif
}

void CommonFunction::callMakdir(const char* path,int luanyong)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"callMakeDownDir","(Ljava/lang/String;I)V");//本人在此传递的参数是字符串
	if (!isHave) {
		log("jni->%s/makdir:is not exit", JNI_PACKAGE);
	}else{
		log("jni->%s/makdir:is exit", JNI_PACKAGE);

		jstring jdata = minfo.env->NewStringUTF(path);
		jint    jType = luanyong;
		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jdata,jType);
	}
	log("jni-java excute finish");
#endif
}


void CommonFunction::updateGame(const char* url)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"setDownloadUrl","(Ljava/lang/String;)V");//本人在此传递的参数是字符串
	if (!isHave) {
		log("jni->%s/setDownloadUrl:is not exit", JNI_PACKAGE);
	}else{
		log("jni->%s/setDownloadUrl:is exit", JNI_PACKAGE);

		jstring jdata = minfo.env->NewStringUTF(url);

		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jdata);
	}
	log("jni-java excute finish");
#endif
}

void  CommonFunction::moreGamePro(const char* packageName,const char* startAcivityName,const char* url)
{

#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	log("moreGamePro");
	JniMethodInfo minfo;//定义Jni函数信息结构体
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"moreGamePro","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (!isHave) {
		log("jni->%s/moreGamePro:is not exit", JNI_PACKAGE);
	}else{
		log("jni->%s/moreGamePro:is exit", JNI_PACKAGE);

		jstring gameName = minfo.env->NewStringUTF(packageName);
		jstring activityName = minfo.env->NewStringUTF(startAcivityName);
		jstring downUrl = minfo.env->NewStringUTF(url);
		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,gameName,activityName,downUrl);
	}
	log("jni-java excute finish");
#endif
}

std::string CommonFunction::getUUID()
{
	std::string uuid = "";

#if  (CC_PLATFORM_ANDROID == CC_PLATFORM_IOS)
	uuid = getUUIDInIOS();
#endif

#if defined(ANDROID)
	uuid = getUUIDInAndroid();
#endif

#if (defined(WIN32) && defined(_WINDOWS))
	uuid = getUUIDInWin32();
#endif

	return uuid;
}


std::string CommonFunction::getUUIDInIOS() {

#if  CC_PLATFORM_IOS == CC_TARGET_PLATFORM

	FXKeychain* keychain = [FXKeychain defaultKeychain];
	NSString* saved_uuid = [keychain objectForKey:[NSString stringWithUTF8String:UUID_SAVING_KEY]];

	if (saved_uuid == nil) {
		CFUUIDRef uuid = CFUUIDCreate(NULL);
		CFStringRef uuidStr = CFUUIDCreateString(NULL, uuid);
		CFRelease(uuid);
		saved_uuid = [NSString stringWithCString:CFStringGetCStringPtr(uuidStr, kCFStringEncodingUTF8)
encoding:NSUTF8StringEncoding];
		CFRelease(uuidStr);
		[keychain setObject:saved_uuid forKey:[NSString stringWithUTF8String:UUID_SAVING_KEY]];

		printAppErrorMessage("[ App State ], [ get new uuid: %s ]", [saved_uuid cStringUsingEncoding:NSUTF8StringEncoding]);
	}

	return [saved_uuid cStringUsingEncoding:NSUTF8StringEncoding];
#endif

	return "";
}

std::string CommonFunction::getUUIDInWin32() {
	std::string macOUT = "";

#if (defined(WIN32) && defined(_WINDOWS))
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
		return false;
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
			return false;
	}

	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			/// 确保是以太网
			if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(pAdapter->AddressLength != 6)
				continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int (pAdapter->Address[0]),
				int (pAdapter->Address[1]),
				int (pAdapter->Address[2]),
				int (pAdapter->Address[3]),
				int (pAdapter->Address[4]),
				int (pAdapter->Address[5]));
			macOUT = acMAC;
			break;
		}
	}

	free(pAdapterInfo);

#endif

	return macOUT;
}

std::string CommonFunction::getUUIDInAndroid() 
{
	std::string uuid = "";

#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"getUUid","()Ljava/lang/String;");
	if (!isHave) {
		log("jni->%s/getUUid:not exit", JNI_PACKAGE);
		return "";
	}else{
		log("jni->%s/getUUid:is exit", JNI_PACKAGE);
		//调用此函数
		jstring jdata = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

		uuid = JniHelper::jstring2string(jdata);
		
		// release
		minfo.env->DeleteLocalRef(jdata);
	}
#endif
	return uuid;
}

std::string CommonFunction::getVersion()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"getCurVersion","()Ljava/lang/String;");
	if (!isHave) {
		log("jni->%s/getCurVersion:not exit", JNI_PACKAGE);
		return "";
	}else{
		log("jni->%s/getCurVersion:is exit", JNI_PACKAGE);
		//调用此函数
		jstring jdata = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		return  minfo.env->GetStringUTFChars(jdata, NULL);
	}
	log("jni-java函数执行完毕");
#endif
	return std::string("1.5.0");
}

std::string CommonFunction::getNetWorkType()
{
	std::string netType = "null";
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"getCurNetWorkType","()Ljava/lang/String;");
	if(isHave)
	{
		//调用此函数
		jstring jdata = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

	    netType = JniHelper::jstring2string(jdata);

		// release
		minfo.env->DeleteLocalRef(jdata);
	}
#endif
	return netType;
}

std::string CommonFunction::getCurOrderNo()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo minfo;//定义Jni函数信息结构体
	bool isHave = JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"getCurTradeNo","()Ljava/lang/String;");
	if (!isHave) {
		log("jni->%s/getCurTradeNo:not exit", JNI_PACKAGE);
		return "";
	}else{
		log("jni->%s/getCurTradeNo:is exit", JNI_PACKAGE);
		//调用此函数
		jstring jdata = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		return  minfo.env->GetStringUTFChars(jdata, NULL);
	}
	log("jni-java函数执行完毕");
#endif
	return std::string("20101201102322159111239");
}



#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
extern "C"
{
	//方法名与java类中的包名+方法名，以下划线连接
	void Java_com_game_GameCatchFish_GameCatchFish_JniQQLogin(JNIEnv*  env, jobject obj, jint a,jstring account,jstring pwd)
	{
		//这里java调用nativePaySuccess(1);，这里就会被调用。jint a = 1
		log("Java_com_xw_GameCatchFish_GameCatchFish_JniQQLogin account = %s",env->GetStringUTFChars(account, NULL));
		log("Java_com_xw_GameCatchFish_GameCatchFish_JniQQLogin pwd = %s",env->GetStringUTFChars(pwd, NULL));
		CommonFunction::loginQQ(env->GetStringUTFChars(account, NULL),env->GetStringUTFChars(pwd, NULL));
	}

	void Java_com_game_GameCatchFish_GameCatchFish_JniCallPay(JNIEnv*  env, jobject obj)
	{
		log("Java_com_xw_GameCatchFish_AppActivity_JniCallPay ");
	}

}
#endif


void CommonFunction::loginQQ(const char* account, const char* pwd)
{
	__Dictionary*  dic = __Dictionary::create();
	dic->setObject(__String::create(account),"account");
	dic->setObject(__String::create(pwd),"password");
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(qqLoginMsg,dic);
}


////是否中文
bool CommonFunction::isAllChniese(std::string string){
	if (string.size() < 4)
	{
		return false;
	}
	for (std::string::iterator iter = string.begin(); iter != string.end(); ++iter)
	{
		if (*iter <= 0 || *iter >= 127)
		{
			iter += 2;
			return true;
		}
		else
		{

		}
	}
	return false;
}
//是否全是数字
bool CommonFunction::isDigit2(std::string str) {
	for (int i = 0; i < str.length(); i++)
	{
		if ((str.at(i) > '9') || (str.at(i) < '0'))
		{
			return   false;
		}
	}
	return  true;
}
//全是字母
bool CommonFunction::isEnglishWord(std::string str) {

	for (int i = 0; i < str.length(); i++)
	{
		if ((str.at(i) > 'a' && str.at(i)< 'z') || (str.at(i) > 'A' && str.at(i) <'Z' ))
		{
			
		}
		else
		{
			return false;
		}
	}
	return  true;
}

std::string CommonFunction::getEffectName(std::string name)
{
	std::string path ="";
	if (SessionManager::shareInstance()->getGameKind() == GameKindDntg)
	{
		path = __String::createWithFormat("audios/%s",name.c_str())->getCString();
	}else if(SessionManager::shareInstance()->getGameKind() == GameKindLkpy )
	{
		path = __String::createWithFormat("GameResources/audios_lk/%s",name.c_str())->getCString();
	}
	else if(SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
	{
		path = __String::createWithFormat("GameResources/audios_jc/%s",name.c_str())->getCString();
	}
	return path;
}

std::string CommonFunction::getResDir()
{
	std::string path ="";
	if (SessionManager::shareInstance()->getGameKind() == GameKindDntg)
	{
		path ="dntg2/images";
	}else if(SessionManager::shareInstance()->getGameKind() == GameKindLkpy)
	{
		path = "GameResources/lkpy";
	}else
	{
		path = "GameResources/jcpy";
	}
	return path;
}

void CommonFunction::createDirectory(const char *pFolderPath) 
{
	log("CommonFunction::createDirectory");
	if(!CCFileUtils::sharedFileUtils()->isDirectoryExist(pFolderPath)) 
	{

		log("CommonFunction::createDirectory2222");
		//TODO 路径问题需要检验
		std::string path = pystring::os::path::normpath_nt(pFolderPath);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		path = "md " + path;
		system((path).c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		
		CommonFunction::callMakdir(pFolderPath,1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		NSString* nsPath = [NSString stringWithFormat:@"%s", pFolderPath];
		NSFileManager* fm = [NSFileManager defaultManager];
		bool res = [fm createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:NULL error:NULL];
#endif
	}
}

WORD CommonFunction::SwitchViewChairID(WORD chair_id) {
	WORD me_chair_id = SessionManager::shareInstance()->getChairId();
	if (me_chair_id >= GAME_PLAYER)
		return chair_id;
	if (me_chair_id < 3) {
		return (chair_id + 3) % GAME_PLAYER;
	}
	return chair_id;
}

std::string CommonFunction::covertTimeSpanStr(LONGLONG span)
{
	char buffer[32] = {0};
	int day = span/(24*3600);
	int hour = (span - day*24*3600)/3600 ;
	int min  =  (span - day*24*3600 - hour*3600)/60;
	int second = (span - day*24*3600 - hour*3600 - min*60)%60 ;

	if (day !=0)
	{
		 sprintf(buffer, "%02d:%02d:%02d:%02d", day,hour,min,second);
	}
	else
	{
		 sprintf(buffer, "%02d:%02d:%02d",hour,min,second);
	}
	 std::string result(buffer);
	 return result;
}
