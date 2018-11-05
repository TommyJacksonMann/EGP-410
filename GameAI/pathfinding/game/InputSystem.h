#pragma once
#include <Trackable.h>
#include "KeyboardInputs.h"
#include <Vector2D.h>

typedef unsigned char BYTE;

class InputSystem : public Trackable
{
public:
	InputSystem();
	~InputSystem();
	void init();
	void cleanup();
	void update();
	void mouseInputUpdate();
	void keyInputUpdate();

	bool getHasByte(const BYTE value, const BYTE comparison)const;
	
	Vector2D getLastPosition()const { return mLastPos; }
	
private:
	BYTE mBitwiseKeyStates[static_cast<int>(KeyCode::NUM_SCANCODES)];
	BYTE mLeftMouse, mRightMouse;
	Vector2D mMouseLocation;
	Vector2D mLastPos;

};
struct StateBitValues
{
	static const BYTE JUST_PRESSED = 0x1;         //0001
	static const BYTE CURRENTLY_PRESSED = 0x2;    //0010
	static const BYTE JUST_RELEASED = 0x4;        //0100
};