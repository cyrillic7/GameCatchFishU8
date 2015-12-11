/**
 * @file WebImageView.h
 * @brief  网络图片加载视图
 * @author tjl
 * @date 15-5-28
 */
#ifndef __WebImageView_H_
#define __WebImageView_H_

#include "cocos2d.h"
#include "WebImageCache.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


using namespace ui;
USING_NS_CC_EXT;

USING_NS_CC;

#define  image_default_name "down_default.png"

class WebImageView : public Sprite {
protected:
    WebImageView ();
	~WebImageView ();

	bool init();

    //开始下载
    void startDownloadImage(const char *url);

    //淡出完成
    void fadeOutDidFinished(Ref* object);

    //更新纹理
    void updateTexture(CCTexture2D *texture);

public:
    void onExit();

    static WebImageView *create();

    //下载完成后是不是需要以动画形式替换, 默认为true
	void setAnimate(bool animate);

	//下载完成前是不是需要loading框, 默认为false(只有默认图为空时才生效)
	void setLoadingAnimation(bool loading);

	//自定义loading动画
	void setLoadingAnimate(Animate* animation);

    //自定义尺寸
	void setCustomSize(Vec2 size);
	void setCustomSize(Size size);

	//是否保持比例
	void setKeepRatio(bool size);

    //开始下载
    //url   图片链接
    //placeholder 默认图片名称
    void imageWithUrl(const char* url, const char* placeholder = image_default_name);

    //下载回调方法
    void downloadFinished(const char* savingPath, const char* tag, bool fromDisk);
    
    //移除web image view 之前必须调用此方法, 否则将内存泄露
    void cleanup();

    //主动适应自定义的size
    void updateSize();

private:
	bool    m_needAnimate;
	bool    m_needLoading;
    CCSize  m_customSize;
	bool	m_keepRatio;
    std::string m_url;

	Sprite* m_loadingSprite;
	ImageView* m_loadingImage;
	void addLoadingWidget();

	Animate* m_loadingAnimate;
};

#endif //__WebImageView_H_
