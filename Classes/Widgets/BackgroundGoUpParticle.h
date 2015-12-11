
#ifndef __casino__BackgroundGoUpParticle__
#define __casino__BackgroundGoUpParticle__

#include "cocos2d.h"
USING_NS_CC;

class BackgroundGoUpParticle : public CCParticleSystemQuad {
public:
    BackgroundGoUpParticle(){}
    virtual ~BackgroundGoUpParticle(){}
//    bool init(){ return initWithTotalParticles(700); }
    virtual bool initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles);
    
    static BackgroundGoUpParticle* create(const char* particleImage);
    static BackgroundGoUpParticle* createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles = 120);
};

class HaloParticle : public CCParticleSystemQuad {
public:
    HaloParticle(){}
    virtual ~HaloParticle(){}
    virtual bool initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles);
    
    static HaloParticle* create(const char* particleImage);
    static HaloParticle* createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles = 60);
};

class BackgroundGoDownParticle : public CCParticleSystemQuad {
public:
    BackgroundGoDownParticle(){}
    virtual ~BackgroundGoDownParticle(){}
    //    bool init(){ return initWithTotalParticles(700); }
    virtual bool initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles);
    
    static BackgroundGoDownParticle* create(const char* particleImage);
    static BackgroundGoDownParticle* createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles = 120);
};


class ChainParticle : public CCParticleSystemQuad {
public:
	ChainParticle(){}
	virtual ~ChainParticle(){}
	//    bool init(){ return initWithTotalParticles(700); }
	virtual bool initWithTotalParticles(const char* particleImage, unsigned int numberOfParticles);

	static ChainParticle* create(const char* particleImage);
	static ChainParticle* createWithTotalParticles(const char* particleImage, unsigned int numberOfParticles = 120);
};


class CommonParticle : public CCParticleSystemQuad {
public:
	CommonParticle(){}
	virtual ~CommonParticle(){}

	static CommonParticle* create(const char* particleFile);

	static CommonParticle* create(ValueMap& dictionary, const std::string& dirname);
};

#endif