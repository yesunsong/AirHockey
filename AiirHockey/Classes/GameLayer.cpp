//
//  GameLayer.cpp
//  AiirHockey
//
//  Created by 叶孙松 on 13-9-11.
//
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

GameLayer::~GameLayer(){
    CC_SAFE_RELEASE(_players);
}

CCScene* GameLayer::scene(){
    CCScene* sce=CCScene::create();
    GameLayer* layer=GameLayer::create();
    sce->addChild(layer);
    return sce;
}

bool GameLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    _player1Score=0;
    _player2Score=0;
    _screenSize=CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* court=CCSprite::create("court.png");
    court->setPosition(ccp(_screenSize.width*0.5, _screenSize.height*0.5));
    this->addChild(court);
    
    _player1=GameSprite::gameSpriteWithFile("mallet.png");
    _player1->setPosition(ccp(_screenSize.width*0.5, _player1->radius()*2));
    this->addChild(_player1);
    
    _player2=GameSprite::gameSpriteWithFile("mallet.png");
    _player2->setPosition(ccp(_screenSize.width*0.5, _screenSize.height-_player2->radius()*2));
    this->addChild(_player2);
    
    _ball=GameSprite::gameSpriteWithFile("puck.png");
    _ball->setPosition(ccp(_screenSize.width*0.5, _screenSize.height*0.5-2*_ball->radius()));
    this->addChild(_ball);
    
    _players=CCArray::create(_player1,_player2,NULL);
    _players->retain();
    
    _player1ScoreLabel=CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width-60, _screenSize.height*0.5-80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel=CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width-60, _screenSize.height*0.5+80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::update(float dt){
    CCPoint ballNextPosition=_ball->getNextPosition();
    CCPoint ballVector=_ball->getVector();
    ballVector=ccpMult(ballVector, 0.98f);
    ballNextPosition.x+=ballVector.x;
    ballNextPosition.y+=ballVector.y;
    
    CCPoint playerNextPosition;
    CCPoint playerVector;
    
    float squared_radii=pow(_player1->radius()+_ball->radius(), 2);
    GameSprite* player;
    for (int p=0; p<_players->count(); p++) {
        player=(GameSprite*)_players->objectAtIndex(p);
        playerNextPosition=player->getNextPosition();
        playerVector=player->getVector();
        
        float diffx=ballNextPosition.x-player->getPositionX();
        float diffy=ballNextPosition.y-player->getPositionY();
        
        float distance1=pow(diffx, 2)+pow(diffy, 2);
        float distance2=pow(_ball->getPositionX()-playerNextPosition.x, 2)+pow(_ball->getPositionY()-playerNextPosition.y, 2);
        
        if (distance1<=squared_radii||distance2<=squared_radii) {
            float mag_ball=pow(ballVector.x, 2)+pow(ballVector.y, 2);
            float mag_player=pow(playerVector.x,2)+pow(playerVector.y, 2);
            float force=sqrtf(mag_ball+mag_player);
            float angle=atan2(diffy, diffx);
            
            ballVector.x=force*cos(angle);
            ballVector.y=force*sin(angle);
            
            ballNextPosition.x=playerNextPosition.x+(player->radius()+_ball->radius()+force)*cos(angle);
            ballNextPosition.y=playerNextPosition.y+(player->radius()+_ball->radius()+force)*sin(angle);
            
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
        
        if (ballNextPosition.x<_ball->radius()) {
            ballNextPosition.x=_ball->radius();
            ballVector.x*=-0.8f;
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
        
        if (ballNextPosition.x>_screenSize.width-_ball->radius()) {
            ballNextPosition.x=_screenSize.width-_ball->radius();
            ballVector.x*=-0.8f;
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
        
        if (ballNextPosition.y>_screenSize.height-_ball->radius()) {
            if (_ball->getPosition().x<_screenSize.width*0.5f-GOAL_WIDTH*0.5f||_ball->getPosition().x>_screenSize.width*0.5f+GOAL_WIDTH*0.5f) {
                ballNextPosition.y=_screenSize.height-_ball->radius();
                ballVector.y*=-0.8f;
                SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
            }
        }
        
        if (ballNextPosition.y<_ball->radius()) {
            if (_ball->getPosition().x<_screenSize.width*0.5f-GOAL_WIDTH*0.5f||_ball->getPosition().x-_screenSize.width*0.5f+GOAL_WIDTH*0.5f) {
                ballNextPosition.y=_ball->radius();
                ballVector.y*=-0.8f;
                SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
            }
        }
        
        _ball->setVector(ballVector);
        _ball->setNextPosition(ballNextPosition);
        
        if (ballNextPosition.y<-_ball->radius()*2) {
            this->playerScore(2);
        }
        
        if (ballNextPosition.y>_screenSize.height+_ball->radius()*2) {
            this->playerScore(1);
        }
        
        _player1->setPosition(_player1->getNextPosition());
        _player2->setPosition(_player2->getNextPosition());
        _ball->setPosition(_ball->getNextPosition());
    }
}

void GameLayer::playerScore(int player){
    SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
    _ball->setVector(ccp(0, 0));
    
    char score_buffer[10];
    if (player==1) {
        _player1Score++;
        sprintf(score_buffer, "%i",_player1Score);
        _player1ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width*0.5, _screenSize.height*0.5+2*_ball->radius()));
    }else{
        _player2Score++;
        sprintf(score_buffer, "%i",_player2Score);
        _player2ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width*0.5, _screenSize.height*0.5-2*_ball->radius()));
    }
    
    _player1->setPosition(ccp(_screenSize.width*0.5,_player1->radius()*2));
    _player2->setPosition(ccp(_screenSize.width*0.5,_screenSize.height- _player1->radius()*2));
    _player1->setTouch(NULL);
    _player2->setTouch(NULL);
}


void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *event){
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    GameSprite* player;
    //
    for (i=pTouches->begin(); i!=pTouches->end(); i++) {
        touch=(CCTouch*)(*i);
        if (touch) {
            tap=touch->getLocation();
            for (int p=0; p<2; p++) {
                player=(GameSprite*)_players->objectAtIndex(p);
                if (player->boundingBox().containsPoint(tap)) {
                    player->setTouch(touch);
                }
            }
        }
    }
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *event){
    CCSetIterator i;
    GameSprite* player;
    CCTouch* touch;
    CCPoint tap;
    
    for (i=pTouches->begin(); i!=pTouches->end(); i++) {
        touch=(CCTouch*)(*i);
        if (touch) {
            tap=touch->getLocation();
            for (int p=0; p<_players->count(); p++) {
                player=(GameSprite*)_players->objectAtIndex(p);
                if (player->getTouch()!=NULL&&player->getTouch()==touch) {
                    CCPoint nextPosition=tap;
                    if (nextPosition.x<player->radius()) {
                        nextPosition.x=player->radius();
                    }
                    if (nextPosition.x>_screenSize.width-player->radius()) {
                        nextPosition.x=_screenSize.width-player->radius();
                    }
                    if (nextPosition.y<player->radius()) {
                        nextPosition.y=player->radius();
                    }
                    if (nextPosition.y>_screenSize.height-player->radius()) {
                        nextPosition.y=_screenSize.height-player->radius();
                    }
                    if (player->getPositionY()<_screenSize.height*0.5f) {
                        if (nextPosition.y>_screenSize.height*0.5-player->radius()) {
                            nextPosition.y=_screenSize.height*0.5-player->radius();
                        }
                    }else{
                        if (nextPosition.y>_screenSize.height*0.5+player->radius()) {
                            nextPosition.y=_screenSize.height*0.5+player->radius();
                        }
                    }
                    player->setNextPosition(nextPosition);
                    player->setVector(ccp(tap.x-player->getPositionX(),tap.y-player->getPositionY()));
                }
            }
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *event){
    CCSetIterator i;
    GameSprite* player;
    CCTouch* touch;
    CCPoint tap;
    
    for (i=pTouches->begin(); i!=pTouches->end(); i++) {
        touch=(CCTouch*)(*i);
        if (touch) {
            for (int p=0; p<_players->count(); p++) {
                player=(GameSprite*)_players->objectAtIndex(p);
                if (player->getTouch()!=NULL&&player->getTouch()==touch) {
                    player->setTouch(NULL);
                    player->setVector(ccp(0, 0));
                }
            }
        }
    }
}