#ifndef GOLDSRC_CONST_H
#define GOLDSRC_CONST_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSrc
{
	/*
	 * Type Definitions
	 */

	typedef float vec_t;
	typedef vec_t vec3_t[3];

	typedef unsigned int func_t;
	typedef unsigned int string_t;

	typedef unsigned char byte;
	typedef unsigned short word;

	typedef int qboolean;

	struct color24
	{
		unsigned int r, g, b;
	};

	struct colorVec
	{
		unsigned int r, g, b, a;
	};

#ifdef _WIN32
#pragma pack(push, 2)
#endif
	struct PackedColorVec
	{
		unsigned short r, g, b, a;
	};
#ifdef _WIN32
#pragma pack(pop)
#endif

	struct link_t
	{
		link_t *prev, *next;
	};

	struct edict_t;

	struct plane_t
	{
		vec3_t normal;
		float dist;
	};

	struct trace_t
	{
		qboolean allsolid;
		qboolean startsolid;
		qboolean inopen, inwater;
		float fracton;
		vec3_t endpos;
		plane_t plane;
		edict_t* ent;
		int hitgroup;
	};
}


#endif // GOLDSRC_CONST_H
