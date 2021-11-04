// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include<stdint.h>
#include <assert.h>
enum {INDEX_NONE	= -1				};
enum {UNICODE_BOM   = 0xfeff			};

enum EForceInit 
{
	ForceInit,
	ForceInitToZero
};
enum ENoInit {NoInit};
enum EInPlace {InPlace};



// Handle type to stably track users on a specific platform
typedef int32_t FPlatformUserId;
const FPlatformUserId PLATFORMUSERID_NONE = INDEX_NONE;


#define assertm(exp, msg) assert(((void)msg, exp))

#define RWProperty(type,name) 								 \
public:														 \
	const type& name() const { return _##name; }			 \
	void name(const type& val) { _##name = val; }			 \
private:													 \
	type _##name;