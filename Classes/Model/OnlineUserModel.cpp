#include "OnlineUserModel.h"
#include "CommonFunction.h"
#include "SessionManager.h"




OnlineUserModel::OnlineUserModel()
    : m_userId(0)
    , m_nickname("")
    , m_vipLevel(0)
    , m_sex(0)
	, m_faceId(0)
    , m_bReset(false)
	, m_userIngot(0)
	, m_userJianjuan(0)
	, m_userScore(0)
	, m_userInsure(0)
	, m_phone("")
	, m_passPortID("")
	, mBFirstCharge(false)
	, m_loudSpeakerNum(0)
	, m_luckSpinNum(0)
{
  
}

OnlineUserModel::~OnlineUserModel() {

}

bool OnlineUserModel::init() {
    return true;
}




OnlineUserModel *OnlineUserModel::create() {
    OnlineUserModel *m = new OnlineUserModel;
    if (NULL != m && m->init()) {
        m->autorelease();
        return m;
    }
    CC_SAFE_DELETE(m);
    return NULL;
}

void OnlineUserModel::setUserId(int userId) {
    m_userId = userId;
}

int OnlineUserModel::getUserId() const {
    return m_userId;
}

std::string OnlineUserModel::getNickname() const {
    return m_nickname;
}

void OnlineUserModel::setNickname(std::string name) {
    m_nickname = name;
}



int OnlineUserModel::getVipLevel() const {
    return m_vipLevel;
}

void OnlineUserModel::setVipLevel(int type) {
    m_vipLevel = type;
}



long long OnlineUserModel::getUserIngot() const {
    return m_userIngot;
}

void OnlineUserModel::setUserIngot(long long number) {


    m_userIngot = number;
}

long long OnlineUserModel::getUserScore() const {
	return m_userScore;
}

void OnlineUserModel::setUserScore(long long number) {


	m_userScore = number;
}

long long OnlineUserModel::getUserJiangJuan() const {
	return m_userJianjuan;
}

void OnlineUserModel::setUserJiangJuan(long long number) {


	m_userJianjuan = number;
}

long long OnlineUserModel::getUserInsure() const {
	return m_userInsure;
}

void OnlineUserModel::setUserInsure(long long number) {


	m_userInsure = number;
}

int OnlineUserModel::getFaceId()
{
	//for test
	if (m_faceId == 0)
	{
		return 1;
	}
	return m_faceId;
}

void OnlineUserModel::setFaceId(int id)
{
	m_faceId = id;
}


int OnlineUserModel::getUserSex()
{
	return m_sex;
}

void OnlineUserModel::setUserSex(int flag)
{
	m_sex = flag;
}

bool OnlineUserModel::getIsReset()
{
	return m_bReset;
}

void OnlineUserModel::setIsReset(bool flag)
{
	m_bReset = flag;
}


//手机号
std::string OnlineUserModel::getPhone()
{
	return m_phone;
}
void OnlineUserModel::setPhone(const char* phone)
{
	m_phone = phone;
}

//身份证号
std::string OnlineUserModel::getPassPort()
{
	return m_passPortID;
}
void OnlineUserModel::setPassPort(const char* port)
{
	m_passPortID = port;
}

void OnlineUserModel::setGameId(int id)
{
	m_gameId = id;
}

int OnlineUserModel::getGameId()
{
	return m_gameId;
}