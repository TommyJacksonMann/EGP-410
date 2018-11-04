#pragma once

#include <string>
#include <deanlibdefines.h>

typedef int IDType;
typedef std::string IDDescriptor;
typedef std::string GraphicsBufferID;


#define MESSAGE_MANAGER gpGame->getMessageManager()
#define GRAPHICS_SYSTEM gpGame->getGraphicsSystem()

const IDType INVALID_ID = -1;

const IDType HIGHEST_ID = 9999;

typedef Uint32 UnitID;

const UnitID INVALID_UNIT_ID = UINT_MAX;