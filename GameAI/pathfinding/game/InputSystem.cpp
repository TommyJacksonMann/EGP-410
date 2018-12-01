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
			if (mBitwiseKeyStates[KeyCode::SCANCODE_G] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new FlowFieldMessage();
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 0);
			}
			if (mBitwiseKeyStates[KeyCode::SCANCODE_S] && getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameMessage* pMessage = new AddAndRemoveUnitsMessage();
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				pGame->getMessageManager()->addMessage(pMessage, 1);
			}
			if ((mBitwiseKeyStates[KeyCode::SCANCODE_UP] || mBitwiseKeyStates[KeyCode::SCANCODE_DOWN]
				|| mBitwiseKeyStates[KeyCode::SCANCODE_LEFT] || mBitwiseKeyStates[KeyCode::SCANCODE_RIGHT])
				&& getHasByte(mBitwiseKeyStates[i], StateBitValues::JUST_PRESSED))
			{
				GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
				Vector2D destination = pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
				destination += Vector2D(16, 16);
				Vector2D direction;
				if (mBitwiseKeyStates[KeyCode::SCANCODE_UP])
				{
					direction = Vector2D(0, -32);
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_DOWN])
				{
					direction = Vector2D(0, 32);
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_LEFT])
				{
					direction = Vector2D(-32, 0);
				}
				else if (mBitwiseKeyStates[KeyCode::SCANCODE_RIGHT])
				{
					direction = Vector2D(32, 0);
				}
				destination += direction;
				GridGraph* pGridGraph = pGame->getGridGraph();
				Grid* pGrid = pGame->getGrid();
				int toIndex = pGrid->getSquareIndexFromPixelXY(destination.getX(), destination.getY());
				Node* pToNode = pGridGraph->getNode(toIndex);
				
				if (!pToNode->getIsWall())
				{
					GameMessage* pMessage = new PlayerMoveToMessage(pGrid->getULCornerOfSquare(toIndex) + Vector2D(0, 0));
					pGame->getMessageManager()->addMessage(pMessage, 0);
				}			
			}
		}
	}

	if (getHasByte(mLeftMouse, StateBitValues::CURRENTLY_PRESSED))
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		static Vector2D lastPos(0.0f, 0.0f);
		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)mMouseLocation.getX(), (int)mMouseLocation.getY());
		Node* pToNode = pGridGraph->getNode(toIndex);
		Graph* pGraph = pGridGraph;
		bool viableNode = false;
		std::vector<Connection*> pConnections = pGraph->getConnections(pToNode->getId());
		//GOES THROUGH ALL OF TONODE's CONNECTIONS
		for (int i = 0; i < pConnections.size(); i++)
		{
			Node* pCurrent = pConnections[i]->getToNode();
			std::vector<Connection*> pCurrentConnections = pGraph->getConnections(pCurrent->getId());
			//CHECKS TO SEE IF THE TONODE OF THE CONNECTION HAS A CONNECTION BACK TO THE TONODE
			for (int j = 0; j < pCurrentConnections.size() && viableNode == false; j++)
			{
				if (pCurrentConnections[j]->getToNode() == pToNode)
				{
					viableNode = true;
				}
			}
			if (viableNode)
			{
				break;
			}
		}
		if ((lastPos.getX() != mMouseLocation.getX() || lastPos.getY() != mMouseLocation.getY()) && viableNode)
		{
			GameMessage* pMessage = new PathToMessage(lastPos, mMouseLocation);
			pGame->getMessageManager()->addMessage(pMessage, 0);
			pMessage = new UnitToNewLocationMessage(mMouseLocation);
			pGame->getMessageManager()->addMessage(pMessage, 0);
			lastPos = mMouseLocation;
			mLastPos = lastPos;
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