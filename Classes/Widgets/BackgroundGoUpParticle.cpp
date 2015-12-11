#include "BackgroundGoUpParticle.h"
#include "Common/TNVisibleRect.h"

BackgroundGoUpParticle* BackgroundGoUpParticle::create(const char* particleImage)
{
    BackgroundGoUpParticle* pRet = new BackgroundGoUpParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, 700))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

BackgroundGoUpParticle* BackgroundGoUpParticle::createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    BackgroundGoUpParticle* pRet = new BackgroundGoUpParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, numberOfParticles)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BackgroundGoUpParticle::initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    if(NULL != particleImage && CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
    {
        // duration
        _duration = kCCParticleDurationInfinity;
        
        // set gravity mode.
        setEmitterMode(kCCParticleModeGravity);
        
        setSpeed(60);
        setSpeedVar(30);
        
        // Gravity Mode: gravity
        setGravity(ccp(14, 5));
        
        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);
        
        // Gravity mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);
        
        // emitter position
        setPosition(ccp(TNVisibleRect::center().x, TNVisibleRect::top().y / 3));
        setPosVar(TNVisibleRect::rightTop() / 2);
        
        // angle
        setAngle(75);
        setAngleVar(27);
        
        // life of particles
        _life = 3;
        _lifeVar = 0;
        
        // size, in pixels
        _startSize = 40.0f;
        _startSizeVar = 0.0f;
        _endSize = 0.0f;
        _endSizeVar = 0.0f;
        
        //rotation
        setStartSpin(14);
        setStartSpinVar(0);
        setEndSpin(0);
        setEndSpinVar(453);
        
        // emits per second
        _emissionRate = 20;
        
        // color of particles
        _startColor.r = 1.0f;
        _startColor.g = 1.0f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;
        
        _blendFunc.src = GL_ONE;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(particleImage);
        if (NULL != texture) {
            setTexture(texture);
        }

        return true;
    }
    return false;
}

HaloParticle* HaloParticle::create(const char* particleImage)
{
    HaloParticle* pRet = new HaloParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, 50)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

HaloParticle* HaloParticle::createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    HaloParticle* pRet = new HaloParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, numberOfParticles)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool HaloParticle::initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    if(NULL != particleImage && CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
    {
        // duration
        _duration = kCCParticleDurationInfinity;
        
        // set gravity mode.
        setEmitterMode(kCCParticleModeGravity);
        
		setDuration(2.0f);

        setSpeed(59.21);
        setSpeedVar(10);
        
        // Gravity Mode: gravity
        setGravity(ccp(0, -20));
        
        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(12);
        
        // Gravity mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);
        
        // emitter position
//        setPosition(ccp(TNVisibleRect::center().x, TNVisibleRect::top().y / 3));
        setPosVar(ccp(0, 5));//设置初始化位置偏差
        
        // angle
        setAngle(90);
        setAngleVar(20);
        
        // life of particles
        _life = 2;
        _lifeVar = 2;
        
        // size, in pixels
        _startSize = 0.0f;
        _startSizeVar = 0.0f;
        _endSize = 23.0f;
        _endSizeVar =2.0f;
        
        //rotation
        setStartSpin(0);
        setStartSpinVar(0);
        setEndSpin(0);
        setEndSpinVar(0);
        
        // emits per second
        _emissionRate = 5;
        
        // color of particles
        _startColor.r = 1.0f;
        _startColor.g = 1.0f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;
        
        _blendFunc.src = GL_ONE;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        
        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(particleImage);
        if (NULL != texture) {
            setTexture(texture);
        }
        
        return true;
    }
    
    return false;
}

BackgroundGoDownParticle* BackgroundGoDownParticle::create(const char* particleImage)
{
    BackgroundGoDownParticle* pRet = new BackgroundGoDownParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, 700))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

BackgroundGoDownParticle* BackgroundGoDownParticle::createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    BackgroundGoDownParticle* pRet = new BackgroundGoDownParticle();
    if (pRet && pRet->initWithTotalParticles(particleImage, numberOfParticles)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BackgroundGoDownParticle::initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
    if(NULL != particleImage && CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
    {
        // duration
        _duration = kCCParticleDurationInfinity;
        
        // set gravity mode.
        setEmitterMode(kCCParticleModeGravity);
        
        setSpeed(60);
        setSpeedVar(30);
        
        // Gravity Mode: gravity
        setGravity(ccp(14, -5));
        
        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);
        
        // Gravity mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);
        
        // emitter position
        setPosition(ccp(TNVisibleRect::center().x, TNVisibleRect::top().y));
        setPosVar(TNVisibleRect::rightTop() / 2);
        
        // angle
        setAngle(-75);
        setAngleVar(27);
        
        // life of particles
        _life = 3;
        _lifeVar = 0;
        
        // size, in pixels
        _startSize = 40.0f;
        _startSizeVar = 0.0f;
        _endSize = 0.0f;
        _endSizeVar = 0.0f;
        
        //rotation
        setStartSpin(14);
        setStartSpinVar(0);
        setEndSpin(0);
        setEndSpinVar(453);
        
        // emits per second
        _emissionRate = 20;
        
        // color of particles
        _startColor.r = 1.0f;
        _startColor.g = 1.0f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;
        
        _blendFunc.src = GL_ONE;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        
        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(particleImage);
        if (NULL != texture) {
            setTexture(texture);
        }
        
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
ChainParticle* ChainParticle::create(const char* particleImage)
{
	ChainParticle* pRet = new ChainParticle();
	if (pRet && pRet->initWithTotalParticles(particleImage, 700))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

ChainParticle* ChainParticle::createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
	ChainParticle* pRet = new ChainParticle();
	if (pRet && pRet->initWithTotalParticles(particleImage, numberOfParticles)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool ChainParticle::initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles)
{
	if(NULL != particleImage && CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
	{
		// duration
		_duration =2.0f;

		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0,0));

		// Gravity Mode: speed of particles
		setSpeed(170);
		setSpeedVar(40);

		// Gravity Mode: radial
		setRadialAccel(-120);
		setRadialAccelVar(30);

		// Gravity Mode: tangential
		setTangentialAccel(0.0f);
		setTangentialAccelVar(0.0f);

		// angle
		_angle = 90;
		_angleVar = 360;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width/2, winSize.height/2);
		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 5.0f;
		_lifeVar = 2;

		// size, in pixels
		_startSize =0.0f;
		_startSizeVar = 100.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = 50;

		//rotation
		setStartSpin(0);
		setStartSpinVar(0);
		setEndSpin(0);
		setEndSpinVar(360);

		// color of particles
		_startColor.r = 1.0f;
		_startColor.g = 1.0f;
		_startColor.b = 1.0f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.0f;
		_endColor.g = 1.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(particleImage);
		if (NULL != texture) {
			setTexture(texture);
		}
		

		// additive
		this->setBlendAdditive(false);
		return true;
	}
	return false;
}


CommonParticle* CommonParticle::create(const char* particleFile)
{
	CommonParticle* pRet = new CommonParticle();
	if (pRet && pRet->initWithFile(particleFile)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

CommonParticle* CommonParticle::create(ValueMap& dictionary, const std::string& dirname)
{
	CommonParticle* pRet = new CommonParticle();
	if (pRet && pRet->initWithDictionary(dictionary,dirname)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}