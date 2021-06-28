#include <GameFramework/Actor.h>
#include<assert.h>
#include "GenericPlatform/IDGenerator.h"
Actor::Actor()  
{
	ID=IDGenerator::NewID();
	assert(!ID.is_nil());
}

Actor::~Actor()
{
}