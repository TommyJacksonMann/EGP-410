#include "InputSystem.h"
#include "SDL.h"
#include "GameMessage.h"
#include "PlayerMoveToMessage.h"
#include "EscapeMessage.h"
#include "AddUnitMessage.h"
#include "DeleteUnitMessage.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"

InputSystem::InputSystem()
{

	init();
}
InputSystem::~InputSystem()
{
	cleanup();
}

void InputSystem::init()
{
	mLeftMouse = 0x0;
	mRightMouse = 0x0;
	for (int i = 0; i < static_cast<int>(KeyCode::NUM_SCANCODES); i++)
	{
		mBitwiseKeyStates[i] = 0x0;
	}
}
void InputSystem::cleanup()
{

}
void InputSystem::update()
{
	keyInputUpdate();
	mouseInputUpdate();
	for (int i = 0; i < static_cast<int>(KeyCode::NUM_SCANCODES); i++)
	{
		if (mBitwiseKeyStates[i] || 0x0)
		{
			if (mBitwiseKeyStates[KeyCode::SCANCODE_ESCAPE] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new EscapeMessage();
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_RETURN] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new AddUnitMessage();
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_D] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new DeleteUnitMessage();
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
		}
	}

	if (getHasByte(mLeftMouse, StateBitValues::CURRENTLY_PRESSED))
	{
		GameMessage* pMessage = new PlayerMoveToMessage(mMouseLocation);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
}
void InputSystem::mouseInputUpdate()
{
	mLeftMouse &= ~StateBitValues::JUST_PRESSED;
	mLeftMouse &= ~StateBitValues::JUST_RELEASED;
	mRightMouse &= ~StateBitValues::JUST_PRESSED;
	mRightMouse &= ~StateBitValues::JUST_RELEASED;

	//Loop through all recorded mouse events
	int x, y;
	SDL_GetMouseState(&x, &y);
	mMouseLocation = Vector2D(x, y);

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		BYTE* whichButton = &mLeftMouse;
		mLeftMouse |= StateBitValues::JUST_PRESSED;
		mLeftMouse |= StateBitValues::CURRENTLY_PRESSED;

	}
	else if (!SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		BYTE* whichButton = &mLeftMouse;
		mLeftMouse &= ~StateBitValues::CURRENTLY_PRESSED;
		mLeftMouse |= StateBitValues::JUST_RELEASED;
	}


	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		BYTE* whichButton = &mRightMouse;
		mRightMouse |= StateBitValues::JUST_PRESSED;
		mRightMouse |= StateBitValues::CURRENTLY_PRESSED;
	}
	else if (!SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		BYTE* whichButton = &mRightMouse;
		mRightMouse &= ~StateBitValues::CURRENTLY_PRESSED;
		mRightMouse |= StateBitValues::JUST_RELEASED;
	}


}
void InputSystem::keyInputUpdate()
{
	SDL_Event ev;
	for (int i = 0; i < static_cast<int>(KeyCode::NUM_SCANCODES); i++)
	{
		mBitwiseKeyStates[i] &= (~StateBitValues::JUST_PRESSED & ~StateBitValues::JUST_RELEASED);
	}
	while (SDL_PollEvent(&ev))
	{

		if (ev.key.keysym.scancode >= static_cast<int>(KeyCode::NUM_SCANCODES))
			continue;
		if (ev.type == SDL_KEYDOWN)
		{
			mBitwiseKeyStates[ev.key.keysym.scancode] |= StateBitValues::JUST_PRESSED;
			mBitwiseKeyStates[ev.key.keysym.scancode] |= StateBitValues::CURRENTLY_PRESSED;

		}
		else if (ev.type == SDL_KEYUP)
		{
			mBitwiseKeyStates[ev.key.keysym.scancode] &= ~StateBitValues::CURRENTLY_PRESSED;
			mBitwiseKeyStates[ev.key.keysym.scancode] |= StateBitValues::JUST_RELEASED;
		}
	}
}

bool InputSystem::getHasByte(const BYTE value, const BYTE comparison)const
{
	return (value & comparison);
}