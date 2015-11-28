#include "LoginUserModel.h"

LoginUserModel::LoginUserModel()
    : m_userId("")
    , m_accountType(accountNormal)
	, m_account("")
    , m_oldId(-1)
    , m_oldId2(-1)
    , m_firstBind(false)
	, m_insurePwd("")
	, m_assigngDate("")
	, m_msgData("")
	, m_qucikAccount("")
	, m_qucikPwd("")
	, m_firstLogin(false)
	, m_loginUrlIndex(0)
{
    m_password = "";
}

LoginUserModel::~LoginUserModel() {

}

bool LoginUserModel::init() {
    return true;
}

void LoginUserModel::flush() {

	CCUserDefault::sharedUserDefault()->setIntegerForKey(cur_loing_url_index_key, m_loginUrlIndex);

	if (m_accountType == accountType::accountQQ)
	{
		return;
	}
    CCUserDefault::sharedUserDefault()->setStringForKey(login_user_id_saving_key, m_userId.c_str());
   // CCUserDefault::sharedUserDefault()->setIntegerForKey(login_user_account_type_saving_key, m_accountType);
	 CCUserDefault::sharedUserDefault()->setBoolForKey(login_user_firstLogin_saving_key, m_firstLogin);
    // oc 这边有额外的保存操作
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	if (m_account != "") {
		CCUserDefault::sharedUserDefault()->setStringForKey(login_user_account_saving_key, m_account.c_str());
	}
	if (m_password != "") {
		CCUserDefault::sharedUserDefault()->setStringForKey(login_user_password_saving_key, m_password.c_str());
	}
	if(m_assigngDate !="")
		CCUserDefault::sharedUserDefault()->setStringForKey(login_user_assignday_saving_key, m_assigngDate);
	if (m_msgData!="")
		CCUserDefault::sharedUserDefault()->setStringForKey(login_user_MsgRead_saving_key, m_msgData);
	if (m_qucikAccount !="")
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(qucik_login_account_saving_key, m_qucikAccount);
	}
	if(m_qucikPwd != "")
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(qucik_login_pwd_saving_key, m_qucikPwd);
	}
	
#endif
}

void LoginUserModel::loadDataFromDisk() {
    m_userId = CCUserDefault::sharedUserDefault()->getStringForKey(login_user_id_saving_key);
    //m_accountType = (accountType)CCUserDefault::sharedUserDefault()->getIntegerForKey(login_user_account_type_saving_key);
	m_account = CCUserDefault::sharedUserDefault()->getStringForKey(login_user_account_saving_key);
	m_password = CCUserDefault::sharedUserDefault()->getStringForKey(login_user_password_saving_key);
	m_assigngDate =  CCUserDefault::sharedUserDefault()->getStringForKey(login_user_assignday_saving_key);
	m_msgData       = CCUserDefault::sharedUserDefault()->getStringForKey(login_user_MsgRead_saving_key);
	m_qucikAccount =  CCUserDefault::sharedUserDefault()->getStringForKey(qucik_login_account_saving_key);
	m_qucikPwd  =   CCUserDefault::sharedUserDefault()->getStringForKey(qucik_login_pwd_saving_key);
	m_firstLogin  =   CCUserDefault::sharedUserDefault()->getBoolForKey(login_user_firstLogin_saving_key);
	m_loginUrlIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(cur_loing_url_index_key);
}

LoginUserModel *LoginUserModel::create() {
    LoginUserModel *m = new LoginUserModel;
    if (NULL != m && m->init()) {
        m->autorelease();
        return m;
    }
    CC_SAFE_DELETE(m);
    return NULL;
}

void LoginUserModel::setUserId(std::string userId) {
    m_userId = userId;
}

std::string LoginUserModel::getUserId() const {
    return m_userId;
}

bool LoginUserModel::getIsTemp() const {
    return m_isTemp;
}

void LoginUserModel::setIsTemp(bool temp) {
    m_isTemp = temp;
}

void LoginUserModel::setAccount(std::string account) {
    m_account = account;
}

std::string LoginUserModel::getAccount() const {
	if (m_accountType == accountType::accountMachineID)
	{
		return m_qucikAccount;
	}
	else
	{
		return m_account;
	}
}

void LoginUserModel::setPassword(std::string pwd) {
	m_password = pwd;
}

std::string LoginUserModel::getPassword() const {
	if (m_accountType == accountType::accountMachineID)
	{
		return m_qucikPwd;
	}
	else
	{
		return m_password;
	}
}


void LoginUserModel::setToken(std::string token) {
    m_token = token;
}

std::string LoginUserModel::getToken() const {
    return m_token;
}

void LoginUserModel::setOldId(int oldId) {
    m_oldId = oldId;
}

int LoginUserModel::getOldId() const {
    return m_oldId;
}

void LoginUserModel::setOldId2(int oldId) {
    m_oldId2 = oldId;
}

int LoginUserModel::getOldId2() const {
    return m_oldId2;
}

void LoginUserModel::setNickname(std::string name) {
    m_nickname = name;
}

std::string LoginUserModel::getNickname() const {
    return m_nickname;
}

void LoginUserModel::setAccountType(accountType type) {
    m_accountType = type;
}

accountType LoginUserModel::getAccountType() const {
    return m_accountType;
}

void LoginUserModel::setInsurePassword(std::string pwd) {
	m_insurePwd = pwd;
}

std::string LoginUserModel::getInsurePassword() const {
	return m_insurePwd;
}