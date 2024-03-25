#include <GameFramework/Actor.h>
#include<assert.h>
#include "GenericPlatform/IDGenerator.h"
FActor::FActor()  
{
	ID=IDGenerator::NewID();
	assert(!ID.is_nil());
}

FActor::~FActor()
{
}