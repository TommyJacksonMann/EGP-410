#include "InputSystem.h"
#include "SDL.h"
#include "GameMessage.h"
#include "PathToMessage.h"
#include "EscapeMessage.h"
#include "DijkstraMessage.h"
#include "AStarMessage.h"
#include "DepthFirstSearchMessage.h"
#include "AddAndRemoveUnitsMessage.h"
#include "UnitToNewLocationMessage.h"
#include "PlayerMoveToMessage.h"
#include "FlowFieldMessage.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"
#include "GameApp.h"
#include "Node.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "FlowFieldPathFinder.h"
#include "SteeringFiles/SteeringComponent.h"
//#include ".\SteeringFiles\PlayerMoveToMessage.h"


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
			static KeyCode lastKeyPressed;
			if ((mBitwiseKeyStates[KeyCode::SCANCODE_UP] || mBitwiseKeyStates[KeyCode::SCANCODE_DOWN]
				|| mBitwiseKeyStates[KeyCode::SCANCODE_LEFT] || mBitwiseKeyStates[KeyCode::SCANCODE_RIGHT])
				&& getHasByte(mBitwiseKeyStates[i], StateBitValues::CURRENTLY_PRESSED)
				)
			{
				
				if (mBitwiseKeyStates[KeyCode::SCANCODE_UP])
				{
					lastKeyPressed = KeyCode::SCANCODE_UP;
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_DOWN])
				{
					lastKeyPressed = KeyCode::SCANCODE_DOWN;
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_LEFT])
				{
					lastKeyPressed = KeyCode::SCANCODE_LEFT;
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_RIGHT])
				{
					lastKeyPressed = KeyCode::SCANCODE_RIGHT;
				}
			}

			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
			Vector2D playerPos = pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
			if ((int)playerPos.getX() % 32 < 5 && (int)playerPos.getY() % 32 < 5)
			{
				Vector2D destination = pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
				destination += Vector2D(16, 16);
				Vector2D direction(0,0);
				bool hitWall = false;
				destination += direction;

				GridGraph* pGridGraph = pGame->getGridGraph();
				Grid* pGrid = pGame->getGrid();
				int toIndex = pGrid->getSquareIndexFromPixelXY(destination.getX(), destination.getY());
				int finalIndex = 0;
				Node* pToNode = pGridGraph->getNode(toIndex);
				Node* checkWallNode = pGridGraph->getNode(toIndex);


				while (hitWall == false)
				{
					finalIndex = toIndex;
					toIndex = pGrid->getSquareIndexFromPixelXY(destination.getX() + direction.getX()
						, destination.getY() + direction.getY());
					pToNode = checkWallNode;
					checkWallNode = pGridGraph->getNode(toIndex);
					hitWall = checkWallNode->getIsWall();
					if (!hitWall)
					{
						if (lastKeyPressed == SCANCODE_UP)
						{
							direction += Vector2D(0, -32);
						}
						else if (lastKeyPressed == SCANCODE_DOWN)
						{
							direction += Vector2D(0, 32);
						}
						else if (lastKeyPressed == SCANCODE_LEFT)
						{
							direction += Vector2D(-32, 0);
						}
						else if (lastKeyPressed == SCANCODE_RIGHT)
						{
							direction += Vector2D(32, 0);
						}
					}
				}

				if (!pToNode->getIsWall())
				{
					GameMessage* pMessage = new PlayerMoveToMessage(pGrid->getULCornerOfSquare(finalIndex) + Vector2D(0, 0));
					pGame->getMessageManager()->addMessage(pMessage, 0);
				}
			}
			
		}
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