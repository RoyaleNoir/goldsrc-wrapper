#ifndef BASE_CENTERPRINT_H
#define BASE_CENTERPRINT_H

#ifdef _WIN32
#pragma once
#endif

#include "ivguicenterprint.h"

class CBaseCenterPrint : public ICenterPrint
{
public:
	virtual void	SetTextColor( int r, int g, int b, int a );
	virtual void	Print( char *text );
	virtual void	Print( wchar_t *text );
	virtual void	ColorPrint( int r, int g, int b, int a, char *text );
	virtual void	ColorPrint( int r, int g, int b, int a, wchar_t *text );
	virtual void	Clear( void );
};

#endif // BASE_CENTERPRINT_H