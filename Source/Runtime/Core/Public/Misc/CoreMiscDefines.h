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


#define assertm(exp, msg) assert(((void)msg, exp))

#define RWProperty(type,name) 								 \
public:														 \
	const type& Get##name() const { return m_##name; }			 \
	void Set##name(const type& val) { m_##name = val; }			 \
private:													 \
	type m_##name;