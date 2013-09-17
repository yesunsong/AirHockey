//
//  GameLayer.h
//  AiirHockey
//
//  Created by 叶孙松 on 13-9-11.
//
//

#ifndef __AiirHockey__GameLayer__
#define __AiirHockey__GameLayer__

#include <iostream>
#define GOAL_WIDTH 400

#include "cocos2d.h"
#include "GameSprite.h"

using namespace cocos2d;

class GameLayer:public cocos2d::CCLayer{
    GameSprite* _player1;
    GameSprite* _player2;
    GameSprite* _ball;
    
    CCArray* _players;
    CCLabelTTF* _player1ScoreLabel;
    CCLabelTTF* _player2ScoreLabel;
  
    CCSize _screenSize;
    
    int _player1Score;
    int _player2Score;
    
public:
    ~GameLayer();
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameLayer);
    
    virtual void ccTouchesBegan(CCSet* pTouches,CCEvent* event);
    virtual void ccTouchesMoved(CCSet* pTouches,CCEvent* event);
    virtual void ccTouchesEnded(CCSet* pTouches,CCEvent* event);
    
    void update(float dt);
    void playerScore(int player);
};

#endif /* defined(__AiirHockey__GameLayer__) */
