#ifndef CLIENTLEAFSYSTEM_H
#define CLIENTLEAFSYSTEM_H

#ifdef _WIN32
#pragma once
#endif

#include "engine/IClientLeafSystem.h"

class CClientLeafSystem : public IClientLeafSystemEngine
{
public:
	virtual void	CreateRenderableHandle( IClientRenderable *pRenderable, bool bIsStaticProp = false );
	virtual void	RemoveRenderable( ClientRenderHandle_t handle );
	virtual void	AddRenderableToLeaves( ClientRenderHandle_t renderable, int nLeafCount, unsigned short *pLeaves );
	virtual void	ChangeRenderableRenderGroup( ClientRenderHandle_t handle, RenderGroup_t group );
};

extern CClientLeafSystem *g_pClientLeafSystem;

#endif // CLIENTLEAFSYSTEM_H
