//
//  GameSprite.h
//  AiirHockey
//
//  Created by 叶孙松 on 13-9-11.
//
//

#ifndef __AiirHockey__GameSprite__
#define __AiirHockey__GameSprite__

#include "cocos2d.h"

class  GameSprite:public cocos2d::CCSprite{
public:
    CC_SYNTHESIZE(cocos2d::CCPoint, _nextPosition, NextPosition);
    CC_SYNTHESIZE(cocos2d::CCPoint, _vector, Vector);
    CC_SYNTHESIZE(cocos2d::CCTouch*, _touch, Touch);
    
    GameSprite();
    virtual ~GameSprite();
    static GameSprite* gameSpriteWithFile(const char* pszFileName);
    virtual void setPosition(const cocos2d::CCPoint& pos);
    float radius();
private:
};

#endif /* defined(__AiirHockey__GameSprite__) */
