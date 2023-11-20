#ifndef VGUI_STUB_H
#define VGUI_STUB_H

#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
#ifdef VGUI_STUB_EXPORTS
#define VGUISTUB_API __declspec( dllexport )
#else
#define VGUISTUB_API __declspec( dllimport )
#endif
#else
#error
#endif

class IVGUIStub
{
public:
	virtual ~IVGUIStub() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void *GetRootPanel() = 0;
};

extern IVGUIStub *vguistub;

typedef IVGUIStub *( *pfnGetVGUIStub_t ) ( void );

#ifdef VGUI_STUB_EXPORTS
extern "C"
{
	VGUISTUB_API IVGUIStub *GetVGUIStub( void );
}
#endif

#endif