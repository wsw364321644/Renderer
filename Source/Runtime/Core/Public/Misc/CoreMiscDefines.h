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
	const type& Get##name() const { return m_##name; }			 \
	void Set##name(const type& val) { m_##name = val; }			 \
private:													 \
	type m_##name;