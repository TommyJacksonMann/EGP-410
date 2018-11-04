#include "InputSystem.h"
#include "SDL.h"
#include "GameMessage.h"
#include "PathToMessage.h"
#include "EscapeMessage.h"
#include "DijkstraMessage.h"
#include "AStarMessage.h"
#include "DepthFirstSearchMessage.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"
#include "GameApp.h"
#include ".\SteeringFiles\PlayerMoveToMessage.h"

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
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_A] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new AStarMessage();
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_D] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new DijkstraMessage();
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_F] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new DepthFirstSearchMessage();
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 0);
			}
		}
	}

	if (getHasByte(mLeftMouse, StateBitValues::CURRENTLY_PRESSED))
	{
		
		static Vector2D lastPos(0.0f, 0.0f);
		if (lastPos.getX() != mMouseLocation.getX() || lastPos.getY() != mMouseLocation.getY())
		{
			GameMessage* pMessage = new PathToMessage(lastPos, mMouseLocation);
			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
			pGame->getMessageManager()->addMessage(pMessage, 0);
			pMessage = new PlayerMoveToMessage(mMouseLocation);
			pGame->getMessageManager()->addMessage(pMessage, 0);
			lastPos = mMouseLocation;
		}/**/
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