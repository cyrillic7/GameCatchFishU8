/**
 * @file CommonFunction
 * @brief 公共函数
 * @author tjl
 * @date
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"

USING_NS_CC;

#define  WEB_QQ_LOGIN 200 //QQ登录
#define  WEB_ACTIVITY 201 //活动

using namespace ui;

//convert int, float, long, double to string
template <typename T>
static std::string numberToString(T number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}



class CommonFunction
{
public:
	CommonFunction();
	~CommonFunction();
	static float CalcDistance(float x1, float y1, float x2, float y2);
	static float CalcAngle(float x1, float y1, float x2, float y2);
	static const char* fish_name_with_status(FishKind fish_kind, const char* status);
	static Animate *createFrameAnimate(const std::vector<std::string> &imageNames, const float duration);
	static Animate *createFrameAnimate(const std::string path, const float duration,int col ,int row, int frameW,int frameH);
	static Widget *getWidgetFromCocostudioFile(const char *fileName);
	static std::string correctUrl(const char *url);
	static bool isFileExist(const char *pFilePath);
	static bool writeFile(const char *path, const char *address, int len);
	static float fgRangeRand( float min, float max ) ;
	static std::string  GBKToUTF8(const char * strChar);
	static std::string  UTF8TOGBK(const char * strChar);
	static std::string getPlatformImageURL(const char* url);
	//字符串截断, 超过maxLength的字符串截断后加...
	static std::string truncationString(std::string str, int maxLength);
	static void addGray(Node* sp);
	static void removeGray(Node* sp);
	// 获取可写的路径
	static std::string getWritablePath();
	//存取已登录帐号
	//static void writeAccountFile();
	//static __Array* readAccountFile();

	//专门调用JAVA 借口
	static void callJni(const char* data,int webType);
	static void callPay(const char* orderNo);//打开支付宝界面
	static void callMakdir(const char* path,int luanyong);
	static void updateGame(const char* url);
	static void callWx(int flag);
	static std::string getUUID ();
	static std::string getNetWorkType();
	static std::string getVersion();
	static std::string getCurOrderNo();
	static void  moreGamePro(const char* packageName,const char* startAcivityName,const char* url);
	//for qq登录
	static void loginQQ(const char* account, const char* pwd);

	//是否含有中文
	static bool isAllChniese(std::string string);

	//是否是纯数字
	static bool isDigit2(std::string str) ;

	//是否是纯字母
	static bool isEnglishWord(std::string str);

	static std::string getEffectName(std::string name);

	static std::string getResDir();

	// 判断文件夹是否存在，不存在时创建
	static void createDirectory(const char *pFolderPath);

	//转换成真正的椅子ID
	static WORD SwitchViewChairID(WORD chair_id);

	static std::string covertTimeSpanStr(LONGLONG span);

	static std::string replace_all(std::string& str, const   std::string&  old_value, const   std::string& new_value);

	static std::string getUUIDInIOS();
	static std::string getUUIDInWin32();
	static std::string getUUIDInAndroid();
};

#endif

