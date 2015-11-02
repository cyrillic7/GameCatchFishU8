/**
 * @file GameJettonWidget
 * @brief  ÓÎÏ·ÖÐµÄJetton
 * @author tjl
 * @date 15-6-3
 */
#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GameJettonWidget : public Node
{
public:
	GameJettonWidget();
	~GameJettonWidget();
	static GameJettonWidget* create(int score,int index);
	bool init(int score,int index);
	Rect calJettonRect();
	int  getIndex() const {return mIndex;}
	void addTick(int tick) {mTickCount = tick;}
	int  getTick() const {return mTickCount;}
	void setMovFlag(bool value) { m_isMov = value;}
	bool getMovFlag() const {return m_isMov;}
	void Render();
	int  getMovDistance() const {return mJettonSp->getContentSize().width;}
	Vec2 position() const {return mCurPos;}
	void setCurPositon(Vec2 pos) {mCurPos = pos;}
protected:
	void RendScoreLable();
private:
	Sprite* mJettonSp;
	int mScore;
	int mIndex;
	int mTickCount;
	bool m_isMov;
	int mCurLayCnt;
	int mMaxLayCnt;
	Vec2 mCurPos;

};





