#ifndef GOLDSRC_MODEL_H
#define GOLDSRC_MODEL_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_studio_shared.h"

class CStudioHdr;

extern GoldSRC::studiohdr_t *WrapStudioModel( CStudioHdr *pHdr );

extern void UnwrapStudioModel( GoldSRC::studiohdr_t *pModel );

#endif // GOLDSRC_MODEL_H