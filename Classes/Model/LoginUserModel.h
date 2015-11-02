/**
 * @file LoginUserModel
 * @brief  登陆用户model
 * @author tjl
 * @date 15-5-26
 */
#ifndef __LoginUserModel_H_
#define __LoginUserModel_H_

#include "cocos2d.h"

USING_NS_CC;

#define login_user_id_saving_key "login_user_id_saving_key"
#define login_user_account_type_saving_key "login_user_account_type_saving_key"
#define login_user_account_saving_key "login_user_account_saving_key"
#define login_user_password_saving_key "login_user_password_saving_key"
#define login_user_assignday_saving_key "login_user_assignday_saving_key" //用户签到天数
#define login_user_MsgRead_saving_key "login_user_MsgRead_saving_key" //用户已读邮件

#define qucik_login_account_saving_key  "qucik_login_account_saving_key"
#define  qucik_login_pwd_saving_key        "qucik_login_pwd_saving_key"

#define  login_user_firstLogin_saving_key        "login_user_firstLogin_saving_key"
enum  accountType
{
	accountNormal = 0,//帐号登录
	accountMachineID,//游客身份
	accountQQ,//QQ登录
	accountMax
};

class LoginUserModel : public Ref {
public:
    LoginUserModel();
    virtual ~LoginUserModel();

    static LoginUserModel *create();
    bool init();
	
public:

    // 持久化数据, 只保存到CCUserDefault, CCUserDefault的持久化需要外部调用
    // 暂时只保存m_userId, AccountType 快速登录帐号 和密码
	// 保存注册账号和密码
    void flush();

    // 获取保存在磁盘的数据
    void loadDataFromDisk();

    //用户id
    void setUserId(std::string userId);
    std::string getUserId()const;

    // 用户如果AE账号登录的情况下，需要区别该账号是否已经通过邮件激活过了
    // true -> 没有激活， false -> 激活了
    bool getIsTemp()const;
    void setIsTemp(bool temp);

    // 账号名
    void setAccount(std::string account);
    std::string getAccount()const;

    //密码
	void setPassword(std::string pwd);
	std::string getPassword()const;

    // token
    void setToken(std::string token);
    std::string getToken()const;

    // 不知道啥用
	// 现在知道了。账号绑定用的
    void setOldId(int oldId);
    int getOldId()const;

	void setOldId2(int oldId);
    int getOldId2()const;

    // 昵称
    void setNickname(std::string name);
    std::string getNickname()const;

    // 账号类型
    void setAccountType(accountType type);
    accountType getAccountType()const;
    
	//银行密码
	void setInsurePassword(std::string pwd);
	std::string getInsurePassword()const;
    // 首次bind后设置为true
    CC_SYNTHESIZE(bool, m_firstBind, FirstBind);

	std::string  getAssignDate() const {return m_assigngDate;}
	void setAssignDate(std::string date) {m_assigngDate = date;}

	//已读邮件 内容
	std::string  getMsgData() const {return m_msgData;}
	void setMsgData(std::string data) {m_msgData = data;}

	 CC_SYNTHESIZE(std::string, m_qucikAccount, QucikAccount);
	 CC_SYNTHESIZE(std::string, m_qucikPwd, QucikPwd);

	 CC_SYNTHESIZE(bool, m_firstLogin, FirstLogin);
private:
    std::string     m_userId;
    bool            m_isTemp;
    std::string     m_account;
	std::string     m_password;
    std::string     m_token;
    int             m_oldId;
    int             m_oldId2;
    std::string     m_nickname;
    accountType     m_accountType;
	std::string     m_insurePwd;
	std::string     m_assigngDate;

	std::string     m_msgData;

};

#endif //__LoginUserModel_H_
