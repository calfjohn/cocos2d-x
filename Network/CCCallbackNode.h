/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef cocos2dx_CCCallbackNode_h
#define cocos2dx_CCCallbackNode_h

#include "cocos2d.h"
#include "CCMessageQueue.h"

USING_NS_CC;

NS_CC_NETWORK_BEGIN

class CC_DLL CCCallbackNode : public CCNode
{
public:
    
    /** allocates and initializes a node.
     The node will be created as "autorelease".
	 @deprecated: This interface will be deprecated sooner or later.
     */
    CC_DEPRECATED_ATTRIBUTE static CCCallbackNode * node(void);
    
	/** allocates and initializes a node.
     The node will be created as "autorelease".
     create schedule funtion and pause it
     */
    static CCCallbackNode * create(void);
    
    /* just resume schedule */
    void callback(void* data);

    /* invoked callback function and then pause schedule*/
    void performedCallback(float dt);
    
protected:
    RequestInfo *m_pRequestInfo;
};

NS_CC_NETWORK_END

#endif
