#ifndef GOLDSRC_SPRITES_H
#define GOLDSRC_SPRITES_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_cdll_int.h"

namespace GoldSRC
{
	struct client_sprite_t
	{
		char	szName[64];
		char	szSprite[64];
		int		hspr;
		int		iRes;
		wrect_t	rc;
	};
}


class CSpriteManager
{
public:
	CSpriteManager();
	~CSpriteManager();

	GoldSRC::client_sprite_t *GetList( const char *szName, int *pCount );

private:
	CUtlVector<GoldSRC::client_sprite_t*> m_SpriteLists;
};

extern CSpriteManager*SpriteManager();


#endif // GOLDSRC_SPRITES_H