#include "WebImageView.h"

#define FADE_OUT_IN_DURATION        0.3f

WebImageView::WebImageView() {
    m_needAnimate = false;
	m_needLoading = false;
    m_customSize = Size::ZERO;
	m_keepRatio = false;
    m_url = "";
	m_loadingSprite = NULL;
	m_loadingAnimate = NULL;

	m_loadingImage = nullptr;
}

WebImageView::~WebImageView() {
}

WebImageView *WebImageView::create() {
    WebImageView *v = new WebImageView;
    if (v && v->init()) {
        v->autorelease();
        return v;
    }
    CC_SAFE_DELETE(v);
    return NULL;
}

void WebImageView::setAnimate(bool animate) {
	m_needAnimate = animate;
}

void WebImageView::setLoadingAnimation(bool loading) {
	m_needLoading = loading;
}

void WebImageView::imageWithUrl(const char *url, const char *placeholder) {
    if (NULL != url) {
        m_url = url;
        
		/* if (NULL != placeholder) 
		{
		std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(placeholder);
		CCTexture2D *texture2D = CCTextureCache::sharedTextureCache()->addImage(fullPath.c_str());
		this->updateTexture(texture2D);
		}*/
		
		if (m_needLoading)
		{
			addLoadingWidget();
		}

        startDownloadImage(url);
    }
}

bool WebImageView::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

void WebImageView::startDownloadImage(const char *url) {
    WebImageCache::sharedInstance()->imageWithUrl(url, this, webimagecache_selector(WebImageView::downloadFinished));
}

void WebImageView::downloadFinished(const char* savingPath, const char *tag, bool fromDisk) {
    if (NULL != savingPath) {
        // 如果从磁盘过来就不要做渐隐的动画了
        if (m_needAnimate && !fromDisk) {
            CCTexture2D *texture2D = CCTextureCache::sharedTextureCache()->addImage(savingPath);
            if (NULL != texture2D) {
                CCFadeOut *fadeOut = CCFadeOut::create(FADE_OUT_IN_DURATION);
                CCCallFuncO *callFuncO = CCCallFuncO::create(this, callfuncO_selector(WebImageView::fadeOutDidFinished), __String::create(savingPath));
                this->runAction(CCSequence::create(fadeOut, callFuncO, NULL));
            }
        } else {
            CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(savingPath);
            
            if (NULL != texture) {
                this->updateTexture(texture);
            }

            if (m_loadingImage != NULL) {
                m_loadingImage->setVisible(false);
            }
        }
    }
}

void WebImageView::fadeOutDidFinished(Ref* object) {
    if (NULL != object) {
        __String* savingPath = (__String*)object;
        CCTexture2D *texture2D = CCTextureCache::sharedTextureCache()->addImage(savingPath->getCString());
        if (NULL != texture2D) {
            this->updateTexture(texture2D);
            CCFadeIn *fadeIn = CCFadeIn::create(FADE_OUT_IN_DURATION);
            this->runAction(fadeIn);
        }
	}
    
    if (m_loadingImage != NULL) {
        m_loadingImage->setVisible(false);
    }
}

void WebImageView::onExit() {
    this->cleanup();
    Sprite::onExit();
}

void WebImageView::cleanup() {
    WebImageCache::sharedInstance()->clearAllRequestsForTarget(this);
}

void WebImageView::setCustomSize(Vec2 size) {
	m_customSize = size;
}

void WebImageView::setCustomSize(CCSize size) {
    m_customSize = size;
}

void WebImageView::setKeepRatio(bool ratio) {
	m_keepRatio = ratio;
}

void WebImageView::updateTexture(CCTexture2D *texture) {
    if (NULL != texture) {
        this->setTexture(texture);
        setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
        updateSize();
    }
}

void WebImageView::updateSize() {
    if (!m_customSize.equals(Size::ZERO)) {
        CCTexture2D *texture = getTexture();
        if (m_keepRatio) {
            float _scaleX = m_customSize.width / getContentSize().width;//texture->getContentSize().width;
            float _scaleY = m_customSize.height / getContentSize().height;//texture->getContentSize().height;
            float _scale = _scaleX > _scaleY ? _scaleY : _scaleX;
            setScale(_scale);
        } else {
//            setScaleX(m_customSize.width / texture->getContentSize().width);
//            setScaleY(m_customSize.height / texture->getContentSize().height);
            setScaleX(m_customSize.width / getContentSize().width);
            setScaleY(m_customSize.height / getContentSize().height);
        }
    }

	/*if (m_loadingSprite != NULL) {
	m_loadingSprite->setPosition(m_customSize/2);
	m_loadingSprite->setScale(m_loadingSprite->getScale() / getScale());
	}*/

	if (m_loadingImage != nullptr)
	{
		m_loadingImage->setScale(getScale());
	}
}

void WebImageView::addLoadingWidget()
{
	// loading 指示器
	if (NULL == m_loadingImage) {

		m_loadingImage = ImageView::create();
		m_loadingImage->loadTexture("down_default.png",ui::Widget::TextureResType::LOCAL);
		m_loadingImage->setAnchorPoint(Vec2(0.5,0.5));
		addChild(m_loadingImage, 1);

		m_loadingSprite = Sprite::create();
		m_loadingSprite->setAnchorPoint(ccp(0.5, 0.5));
		m_loadingSprite->setPosition(Vec2(m_loadingImage->getContentSize().width/2,m_loadingImage->getContentSize().height/2));
		m_loadingImage->addChild(m_loadingSprite);

		if (m_loadingAnimate) {
			m_loadingSprite->runAction(CCRepeatForever::create(m_loadingAnimate));
		} else {
			//CCArray* array = CCArray::createWithCapacity(19);
			//char name[100] = {0};

			//for (int i=0; i<19; i++) {
			//	sprintf(name, "Common/small_loading/loading_activity_%d.png", i);
			//	CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage(name);
			//	SpriteFrame* f = SpriteFrame::createWithTexture(t, CCRectMake(0, 0, t->getContentSize().width, t->getContentSize().height));
			//	array->addObject(f);
			//}

			//Animation *animation = Animation::createWithSpriteFrames(array, 0.05);
			//m_loadingSprite->runAction(CCRepeatForever::create(Animate::create(animation)));
			CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("image_down_loading.png");
			SpriteFrame* f = SpriteFrame::createWithTexture(t, CCRectMake(0, 0, 39, 39));
			m_loadingSprite->setDisplayFrame(f);
			m_loadingSprite->runAction(CCRepeatForever::create(CCRotateBy::create(0.2f, 30)));

			if (!m_customSize.equals(Size::ZERO))
			{
				float _scaleX = m_customSize.width / 62;
				float _scaleY = m_customSize.height / 62;
				float _scale = _scaleX > _scaleY ? _scaleY : _scaleX;
				if (_scale > 1) _scale = 1;
				m_loadingSprite->setScale(_scale);
			}
		}
	}
	m_loadingSprite->setVisible(true);
}

void WebImageView::setLoadingAnimate(Animate* animate)
{
	m_loadingAnimate = animate;
}