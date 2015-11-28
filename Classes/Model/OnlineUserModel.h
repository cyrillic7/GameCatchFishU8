/**
 * @file OnlineUserModel
 * @brief  在线用户model
 * @author tjl
 * @date 15-6-26
 */
#ifndef __OnlineUserModel_H_
#define __OnlineUserModel_H_

#include "cocos2d.h"

USING_NS_CC;

class OnlineUserModel : public Ref {
public:
    OnlineUserModel();
    ~OnlineUserModel();

    static OnlineUserModel* create();
    bool init();

    //用户id
    void setUserId(int userId);
    int getUserId() const;

    //昵称
    std::string getNickname()const ;
    void setNickname(std::string name);

    //头像
	int getFaceId();
	void setFaceId(int id);

    //vip级别, 0:none, 1:vip, 2:super vip, 3:royal vip
    int getVipLevel()const;
    void setVipLevel(int level);


    //用户金币数
    long long getUserScore()const;
    void setUserScore(long long number);

	//用户元宝数
	long long getUserIngot()const;
	void setUserIngot(long long number);

	//用户银行存款数
	long long getUserInsure()const;
	void setUserInsure(long long number);

	//用户奖券数
	long long getUserJiangJuan()const;
	void setUserJiangJuan(long long number);

	//获得经过format之后的筹码字符串,  million(百万), billion(10亿)
	// format方式: 大于1000的改成以k为单位, 10000=>10k,小于1000的不变
    //大于百万的改成以m为单位
    //大于10亿的改成以b为单位
	//std::string getFormatChips();

	//是否重设
    bool getIsReset();
    void setIsReset(bool flag);

	//性别
	void setUserSex(int flag);
	int  getUserSex();

	//手机号
	std::string getPhone();
	void setPhone(const char* phone);

	//身份证号
	std::string getPassPort();
	void setPassPort(const char* port);
    
	int  getGameId();
	void setGameId(int id);

	//是否进行过首次充值
	bool  isFirstCharge() {return mBFirstCharge;};
	void  setFirstCharge(bool value) {mBFirstCharge = value;}
private:
    int				m_userId;
	int             m_gameId;
    std::string     m_nickname;
    int             m_faceId;//头像ID
    int             m_vipLevel;//vip 等级
    long long       m_userIngot;//元宝
    long long       m_userScore;//用户金币
    long long		m_userInsure;//用户银行
	long long		m_userJianjuan;//用户奖券
	int             m_sex;//用户性别
	bool            m_bReset;//是否重设银行密码
	std::string     m_phone;//绑定手机号码
	std::string     m_passPortID;//绑定身份证号码
	bool           mBFirstCharge;
};

#endif //__OnlineUserModel_H_
