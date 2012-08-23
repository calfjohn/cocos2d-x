//
//  Chapter.cpp
//  cocos2dx
//
//  Created by Cocos2d on 12-7-18.
//  Copyright (c) 2012年 厦门雅基软件有限公司. All rights reserved.
//

#include "CCTouchPageTurn.h"

NS_CC_EXT_BEGIN

// on "init" you need to initialize your instance
bool CCTouchPageTurn::init()
{
    CCLayer::init();

    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    int x,y;
    if( s.width > s.height)
    {
        x=16;
        y=12;
    }
    else
    {
        x=12;
        y=16;
    }
    
    m_action =  CCPageTurn3D::create(ccg(x,y), 1);
    
    this->setTouchEnabled(true);

    return true;
}

void CCTouchPageTurn::onEnter()
{
    CCLayer::onEnter();
}

void CCTouchPageTurn::onExit()
{
    CCLayer::onExit();
}

bool CCTouchPageTurn::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    
    CCPoint touchPoint = touch->locationInView();
    
    m_ptTouchBegan = m_ptTouchEnd = CCDirector::sharedDirector()->convertToGL(touchPoint);
      
    return false;
}

void CCTouchPageTurn::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
 
    CCPoint touchPoint = touch->locationInView();
    m_ptTouchEnd = CCDirector::sharedDirector()->convertToGL(touchPoint);   
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    m_action->update((m_ptTouchEnd.x - m_ptTouchBegan.x) / size.width / 2);
}

void CCTouchPageTurn::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
}

void CCTouchPageTurn::ccTouchCancelled(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
}

NS_CC_EXT_END

