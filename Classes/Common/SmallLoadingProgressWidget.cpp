#include "SmallLoadingProgressWidget.h"

SmallLoadingProgressWidget* SmallLoadingProgressWidget::create(bool thinMode, const char* backgroundImage, const char* frontImage) {
    SmallLoadingProgressWidget* s = new SmallLoadingProgressWidget();
    s->init(thinMode, backgroundImage, frontImage);
    s->autorelease();
    return s;
}


bool SmallLoadingProgressWidget::init(bool thinMode, const char* backgroundImage, const char* frontImage) {
    CCNode::init();
    
    if (!thinMode) {
        std::string bg = (NULL == backgroundImage ? "common/loading_circle_background.png" : backgroundImage);
        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(bg.c_str());
        if (NULL != texture) {
            m_backgroundSprite = Sprite::create(bg.c_str());
        } else {
            m_backgroundSprite = Sprite::createWithSpriteFrameName(bg.c_str());
        }
    } else {
        m_backgroundSprite = Sprite::create("common/loading_circle_background_long.png");
    }
    
    addChild(m_backgroundSprite);

    std::string front = (NULL == frontImage ? "common/loading_circle_forground.png" : frontImage);
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(front.c_str());
    if (NULL != texture) {
        m_forgroundSprite = Sprite::create(front.c_str());
    } else {
        m_forgroundSprite = Sprite::createWithSpriteFrameName(front.c_str());
    }
    m_progressTimer = CCProgressTimer::create(m_forgroundSprite);
    m_progressTimer->setPercentage(100);
    m_progressTimer->setReverseProgress(true);
    if (thinMode) {
        m_progressTimer->setPosition(ccp(128, 202));
        m_progressTimer->setAnchorPoint(ccp(0.5, 0.5));
    } else {
        m_progressTimer->setAnchorPoint(ccp(0, 0));
    }
   
    m_backgroundSprite->addChild(m_progressTimer);
    

	//m_progressText = Text::create();
	//m_progressText->setText(__String::createWithFormat("%d%%",0)->getCString());
	//m_progressText->setFontSize(20);
	//m_progressText->setPosition(Vec2(m_backgroundSprite->getContentSize().width/2,m_backgroundSprite->getContentSize().height/2));
	//m_progressText->setAnchorPoint(Vec2(0.5, 0.5));

	m_progressLabel = LabelAtlas::create("0:","common/loading_number.png",30,24,'0');
	m_progressLabel->setPosition(Vec2(m_backgroundSprite->getContentSize().width/2,m_backgroundSprite->getContentSize().height/2));
	m_progressLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_backgroundSprite->addChild(m_progressLabel);

	setProgress(0);
    return true;
}

void SmallLoadingProgressWidget::startLoading() {
    setVisible(true);
}

void SmallLoadingProgressWidget::stopLoading() {
    setVisible(false);
}

void SmallLoadingProgressWidget::setProgress(float progress) {
    if (progress == 0) {
        progress = 0.1;
    }
    m_progress = progress;
    m_progressTimer->setPercentage(100 - m_progress);
	m_progressLabel->setString(__String::createWithFormat("%d:",(int)progress)->getCString());
}