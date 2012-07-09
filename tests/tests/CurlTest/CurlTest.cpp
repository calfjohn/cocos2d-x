#include "CurlTest.h"
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"

#include "Network/CCNetwork.h"
#include "Network/CCMessagequeue.h"

CurlTest::CurlTest()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = CCLabelTTF::create("Curl Test", "Arial", 28);
    addChild(label, 0);
    label->setPosition( ccp(s.width/2, s.height-50) );

    setTouchEnabled(true);

    // create a label to display the tip string
    m_pLabel = CCLabelTTF::create("Touch the screen to connect", "Arial", 22);
    m_pLabel->setPosition(ccp(s.width / 2, s.height / 2));
    addChild(m_pLabel, 0);
    
    m_pLabel->retain();
    
    CCMessagequeue::sharedMessagequeue();
}

// the test code is
// http://curl.haxx.se/mail/lib-2009-12/0071.html
void CurlTest::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    //=============================================
//    network.Post("weibo.com", "i'm a request", strResponse);
//    network.Post("192.168.0.116", "i'm a request", strResponse);
//    network.Get("192.168.0.116", strResponse);
//    network.Posts(192.168.0.116", "i'm a request", strResponse);
//    CCNetwork::sharedNetwork()->Get("192.168.0.116", strResponse);
//    CCNetwork::sharedNetwork()->Get("weibo.com", strResponse);
    //CCLog("response1:%s", strResponse.c_str());
    string strResponse;
    CCMessagequeue::sharedMessagequeue()->pushRequest("weibo.com", "i'm a request", this, callfuncND_selector(CurlTest::callback3));
    return;
    //=============================================
    
    CURL *curl;
    CURLcode res;
    char buffer[10];

    curl = curl_easy_init();
    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, "google.com");
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        if (res == 0)
        {
            m_pLabel->setString("0 response");
        }
        else
        {
            sprintf(buffer,"code: %i",res);
            m_pLabel->setString(buffer);
        }
    } 
    else 
    {
        m_pLabel->setString("no curl");
    } 
}

void CurlTest::callback3(CCNode* sender, void* data)
{
    RequestInfo *pRequestInfo = (RequestInfo *)data;
    
    char szName[100] = {0};
    sprintf(szName, "response %d success back", pRequestInfo->requestId);
    
//   m_pLabel->setString(szName);
    
//    CCSize winsize  = CCDirector::sharedDirector()->getWinSize();
//    CCAction *spawnAction = CCSpawn::actions(
//                                    CCMoveBy::actionWithDuration(3.0, ccp(0, 30)),
//                                    CCFadeOut::actionWithDuration(3.0),
//                                    NULL);
//    
//    CCAction *action = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height/2)), CCShow::action(), spawnAction, NULL);
//    
//    m_pLabel->setString(szName);
//    m_pLabel->runAction(action);
}

CurlTest::~CurlTest()
{
    m_pLabel->release();
}

void CurlTestScene::runThisTest()
{
    CCLayer* pLayer = new CurlTest();
    addChild(pLayer);

    CCDirector::sharedDirector()->replaceScene(this);
    pLayer->release();
}
