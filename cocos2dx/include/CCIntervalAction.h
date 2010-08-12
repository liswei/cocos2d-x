/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __ACTION_CCINTERVAL_ACTION_H__
#define __ACTION_CCINTERVAL_ACTION_H__

#include "CCNode.h"
#include "CCAction.h"
#include "CCProtocols.h"
#include "CCSpriteFrame.h"

// #include <sys/time.h>

/** An interval action is an action that takes place within a certain period of time.
It has an start time, and a finish time. The finish time is the parameter
duration plus the start time.

These CCIntervalAction actions have some interesting properties, like:
 - They can run normally (default)
 - They can run reversed with the reverse method
 - They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

For example, you can simulate a Ping Pong effect running the action normally and
then running it again in Reverse mode.

Example:
 
	CCAction * pingPongAction = [CCSequence actions: action, [action reverse], nil];
*/

namespace cocos2d {

class CCX_DLL CCIntervalAction : public CCFiniteTimeAction
{
public:
	/** how many seconds had elapsed since the actions started to run. */
	inline ccTime getElapsed(void) { return m_elapsed; }

	/** initializes the action */
	bool initWithDuration(ccTime d);

	/** returns YES if the action has finished */
	virtual bool isDone(void);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void step(ccTime dt);
	virtual void startWithTarget(NSObject *pTarget);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCIntervalAction* actionWithDuration(ccTime d);

protected:
	ccTime m_elapsed;
	bool   m_bFirstTick;
};

/** Runs actions sequentially, one after another
 */
class CCX_DLL CCSequence : public CCIntervalAction
{
public:
	~CCSequence(void);

	/** initializes the action */
    bool initOneTwo(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void stop(void);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** helper contructor to create an array of sequenceable actions */
	static CCFiniteTimeAction* actions(CCFiniteTimeAction *pAction1, ...);

	/** creates the action */
	static CCSequence* actionOneTwo(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo);
protected:
	CCFiniteTimeAction *m_pActions[2];
	ccTime m_split;
	int m_last;
};

/** Repeats an action a number of times.
 * To repeat an action forever use the CCRepeatForever action.
 */
class CCX_DLL CCRepeat : public CCIntervalAction
{
public:
	~CCRepeat(void);

	/** initializes a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
	bool initWithAction(CCFiniteTimeAction *pAction, unsigned int times);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void stop(void);
	virtual void update(ccTime time);
	virtual bool isDone(void);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
	static CCRepeat* actionWithAction(CCFiniteTimeAction *pAction, unsigned int times);

protected:
	unsigned int m_uTimes;
	unsigned int m_uTotal;
	CCFiniteTimeAction *m_pOther;
};

/** Spawn a new action immediately
 */
class CCX_DLL CCSpawn : public CCIntervalAction
{
public:
	~CCSpawn(void);

	/** initializes the Spawn action with the 2 actions to spawn */
	bool initOneTwo(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void stop(void);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** helper constructor to create an array of spawned actions */
	static CCFiniteTimeAction* actions(CCFiniteTimeAction *pAction1, ...);

	/** creates the Spawn action */
	static CCSpawn* actionOneTwo(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2);

protected:
	CCFiniteTimeAction *m_pOne;
	CCFiniteTimeAction *m_pTwo;
};

/**  Rotates a CCNode object to a certain angle by modifying it's
 rotation attribute.
 The direction will be decided by the shortest angle.
*/ 
class CCX_DLL CCRotateTo : public CCIntervalAction
{
public:
	/** initializes the action */
	bool initWithDuration(ccTime duration, float fDeltaAngle);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);

public:
	/** creates the action */
	static CCRotateTo* actionWithDuration(ccTime duration, float fDeltaAngle);

protected:
	float m_fDstAngle;
	float m_fStartAngle;
	float m_fDiffAngle;
};

/** Rotates a CCNode object clockwise a number of degrees by modiying it's rotation attribute.
*/
class CCX_DLL CCRotateBy : public CCIntervalAction
{
public:
	/** initializes the action */
    bool initWithDuration(ccTime duration, float fDeltaAngle);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCRotateBy* actionWithDuration(ccTime duration, float fDeltaAngle);

protected:
	float m_fAngle;
	float m_fStartAngle;
};

/** Moves a CCNode object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
*/
class CCX_DLL CCMoveTo : public CCIntervalAction
{
public:
	/** initializes the action */
	bool initWithDuration(ccTime duration, CGPoint position);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);

public:
	/** creates the action */
	static CCMoveTo* actionWithDuration(ccTime duration, CGPoint position);

protected:
	CGPoint m_endPosition;
	CGPoint m_startPosition;
	CGPoint m_delta;
};

/**  Moves a CCNode object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Duration is is seconds.
*/ 
class CCX_DLL CCMoveBy : public CCMoveTo
{
public:
	/** initializes the action */
	bool initWithDuration(ccTime duration, CGPoint position);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCMoveBy* actionWithDuration(ccTime duration, CGPoint position);
};

/** Moves a CCNode object simulating a parabolic jump movement by modifying it's position attribute.
*/
class CCX_DLL CCJumpBy : public CCIntervalAction
{
public:
	/** initializes the action */
	bool initWithDuration(ccTime duration, CGPoint position, ccTime height, int jumps);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCJumpBy* actionWithDuration(ccTime duration, CGPoint position, ccTime height, int jumps);

protected:
	CGPoint m_startPosition;
	CGPoint m_delta;
	ccTime  m_height;
	int     m_nJumps;
};

/** Moves a CCNode object to a parabolic position simulating a jump movement by modifying it's position attribute.
*/ 
class CCX_DLL CCJumpTo : public CCJumpBy
{
public:
    virtual void startWithTarget(NSObject *pTarget);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
	static CCJumpTo* actionWithDuration(ccTime duration, CGPoint position, ccTime height, int jumps);
};

/** bezier configuration structure
 */
typedef struct _ccBezierConfig {
	//! end position of the bezier
	CGPoint endPosition;
	//! Bezier control point 1
	CGPoint controlPoint_1;
	//! Bezier control point 2
	CGPoint controlPoint_2;
} ccBezierConfig;

/** An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class CCX_DLL CCBezierBy : public CCIntervalAction
{
public:
	/** initializes the action with a duration and a bezier configuration */
	bool initWithDuration(ccTime t, ccBezierConfig c);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action with a duration and a bezier configuration */
	static CCBezierBy* actionWithDuration(ccTime t, ccBezierConfig c);

protected:
	ccBezierConfig m_sConfig;
	CGPoint m_startPosition;
};

/** An action that moves the target with a cubic Bezier curve to a destination point.
 @since v0.8.2
 */
class CCX_DLL CCBezierTo : public CCBezierBy
{
public:
    virtual void startWithTarget(NSObject *pTarget);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
    static CCBezierTo* actionWithDuration(ccTime t, ccBezierConfig c);
};

/** Scales a CCNode object to a zoom factor by modifying it's scale attribute.
 @warning This action doesn't support "reverse"
 */
class CCX_DLL CCScaleTo : public CCIntervalAction
{
public:
	/** initializes the action with the same scale factor for X and Y */
	bool initWithDuration(ccTime duration, float s);

	/** initializes the action with and X factor and a Y factor */
	bool initWithDuration(ccTime duration, float sx, float sy);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);

public:
	/** creates the action with the same scale factor for X and Y */
	static CCScaleTo* actionWithDuration(ccTime duration, float s);

	/** creates the action with and X factor and a Y factor */
	static CCScaleTo* actionWithDuration(ccTime duration, float sx, float sy);
protected:
	float m_fScaleX;
	float m_fScaleY;
	float m_fStartScaleX;
  	float m_fStartScaleY;
    float m_fEndScaleX;
	float m_fEndScaleY;
	float m_fDeltaX;
	float m_fDeltaY;
};

/** Scales a CCNode object a zoom factor by modifying it's scale attribute.
*/
class CCX_DLL CCScaleBy : public CCScaleTo
{
public:
    virtual void startWithTarget(NSObject *pTarget);
	virtual CCIntervalAction* reverse(void);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
	/** creates the action with the same scale factor for X and Y */
	static CCScaleBy* actionWithDuration(ccTime duration, float s);

	/** creates the action with and X factor and a Y factor */
	static CCScaleBy* actionWithDuration(ccTime duration, float sx, float sy);
};

/** Blinks a CCNode object by modifying it's visible attribute
*/
class CCX_DLL CCBlink : public CCIntervalAction
{
public:
	/** initilizes the action */
	bool initWithDuration(ccTime duration, unsigned int uBlinks);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCBlink* actionWithDuration(ccTime duration, unsigned int uBlinks);
protected:
	int m_nTimes;
};

/** Fades In an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 */
class CCX_DLL CCFadeIn : public CCIntervalAction
{
public:
    virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
	static CCFadeIn* actionWithDuration(ccTime d);
};

/** Fades Out an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
*/
class CCX_DLL CCFadeOut : public CCIntervalAction
{
public:
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
	static CCFadeOut* actionWithDuration(ccTime d);
};

/** Fades an object that implements the CCRGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 @warning This action doesn't support "reverse"
 */
class CCX_DLL CCFadeTo : public CCIntervalAction
{
public:
	/** initializes the action with duration and opacity */
	bool initWithDuration(ccTime duration, GLubyte opacity);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);

public:
	/** creates an action with duration and opactiy */
	static CCFadeTo* actionWithDuration(ccTime duration, GLubyte opacity);

protected:
	GLubyte m_toOpacity;
	GLubyte m_fromOpacity;
};

/** Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 @warning This action doesn't support "reverse"
 @since v0.7.2
*/
class CCX_DLL CCTintTo : public CCIntervalAction
{
public:
	/** initializes the action with duration and color */
	bool initWithDuration(ccTime duration, GLubyte red, GLubyte green, GLubyte blue);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);

public:
	/** creates an action with duration and color */
	static CCTintTo* actionWithDuration(ccTime duration, GLubyte red, GLubyte green, GLubyte blue);

protected:
	ccColor3B m_to;
	ccColor3B m_from;
};

/** Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 @since v0.7.2
 */
class CCX_DLL CCTintBy : public CCIntervalAction
{
public:
	/** initializes the action with duration and color */
	bool initWithDuration(ccTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates an action with duration and color */
	static CCTintBy* actionWithDuration(ccTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);

protected:
	GLshort m_deltaR;
	GLshort m_deltaG;
	GLshort m_deltaB;

	GLshort m_fromR;
	GLshort m_fromG;
	GLshort m_fromB;
};

/** Delays the action a certain amount of seconds
*/
class CCX_DLL CCDelayTime : public CCIntervalAction
{
public:
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);
	virtual NSObject* copyWithZone(NSZone* pZone);

public:
	static CCDelayTime* actionWithDuration(ccTime d);
};

/** Executes an action in reverse order, from time=duration to time=0
 
 @warning Use this action carefully. This action is not
 sequenceable. Use it as the default "reversed" method
 of your own actions, but using it outside the "reversed"
 scope is not recommended.
*/
class CCX_DLL CCReverseTime : public CCIntervalAction
{
public:
	~CCReverseTime(void);

	/** initializes the action */
    bool initWithAction(CCFiniteTimeAction *pAction);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void stop(void);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action */
	static CCReverseTime* actionWithAction(CCFiniteTimeAction *pAction);

protected:
	CCFiniteTimeAction *m_pOther;
};

class CCX_DLL CCTexture2D;
/** Animates a sprite given the name of an Animation */
class CCX_DLL CCAnimate : public CCIntervalAction
{
public:
	~CCAnimate(void);

	/** animation used for the animage, retain */
	inline CCAnimation* getAnimation(void) { return m_pAnimation; }
	inline void setAnimation(CCAnimation *pAnimation) 
	{
		m_pAnimation = pAnimation;
		pAnimation->retain();
	}

	/** initializes the action with an Animation and will restore the original frame when the animtion is over */
    bool initWithAnimation(CCAnimation *pAnimation);

	/** initializes the action with an Animation */
	bool initWithAnimation(CCAnimation *pAnimation, bool bRestoreOriginalFrame);

	/** initializes an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overrided by the duration parameter.
	 @since v0.99.0
	 */
	bool initWithDuration(ccTime duration, CCAnimation *pAnimation, bool bRestoreOriginalFrame);

	virtual NSObject* copyWithZone(NSZone* pZone);
	virtual void startWithTarget(NSObject *pTarget);
	virtual void stop(void);
	virtual void update(ccTime time);
	virtual CCIntervalAction* reverse(void);

public:
	/** creates the action with an Animation and will restore the original frame when the animation is over */
	static CCAnimate* actionWithAnimation(CCAnimation *pAnimation);

	/** creates the action with an Animation */
	static CCAnimate* actionWithAnimation(CCAnimation *pAnimation, bool bRestoreOriginalFrame);

	/** creates an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overrided by the duration parameter.
	 @since v0.99.0
	 */
	
     static CCAnimate* actionWithDuration(ccTime duration, CCAnimation *pAnimation, bool bRestoreOriginalFrame);
protected:
	CCAnimation *m_pAnimation;
	CCSpriteFrame *m_pOrigFrame;
    bool m_bRestoreOriginalFrame;
};

}

#endif //__ACTION_CCINTERVAL_ACTION_H__