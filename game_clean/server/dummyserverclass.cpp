#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CDummyServerClass
{
public:
	DECLARE_CLASS_NOBASE( CDummyServerClass );
	DECLARE_SERVERCLASS();
};

IMPLEMENT_SERVERCLASS_ST_NOBASE( CDummyServerClass, DT_DummyClass )
END_SEND_TABLE()
