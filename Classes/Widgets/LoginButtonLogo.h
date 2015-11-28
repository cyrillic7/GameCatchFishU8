
#ifndef __LOGIN_BUTTON_LOGO_H__
#define __LOGIN_BUTTON_LOGO_H__

#include "cocos2d.h"

USING_NS_CC;


class LoginButtonLogo : public Node {
public:
	typedef enum {
		LOGIN_BUTTON_TYPE_ACCOUNT,
		LOGIN_BUTTON_TYPE_FAST,
		LOGIN_BUTTON_TYPE_QQ
	} LoginButtonType;

public:
	static LoginButtonLogo* create(LoginButtonType type);

private:
	virtual bool init(LoginButtonType type);
};


#endif
