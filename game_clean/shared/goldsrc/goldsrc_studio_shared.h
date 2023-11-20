#ifndef GOLDSRC_STUDIO_SHARED_H
#define GOLDSRC_STUDIO_SHARED_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSRC
{
	typedef float vec3_t[3];

	struct studiohdr_t
	{
		int		id;
		int		version;
		char	name[64];
		int		length;
		vec3_t	eyeposition;
		vec3_t	min;
		vec3_t	max;
		vec3_t	bbmin;
		vec3_t	bbmax;
		int		flags;
		int		numbones;
		int		boneindex;
		int		numbonecontrollers;
		int		bonecontrollerindex;
		int		numhitboxes;
		int		hitboxindex;
		int		numseq;
		int		seqindex;
		int		numseqgroups;
		int		seqgroupindex;
		int		numtextures;
		int		textureindex;
		int		texturedataindex;
		int		numskinref;
		int		numskinfamilies;
		int		skinindex;
		int		numbodyparts;
		int		bodypartindex;
		int		numattachments;
		int		attachmentindex;
		int		soundtable;
		int		soundindex;
		int		soundgroups;
		int		soundgroupindex;
		int		numtransitions;
		int		transitionindex;
	};


	struct mstudiosequence_t
	{
		char	label[32];
		float	fps;
		int		flags;
		int		activity;
		int		actweight;
		int		numevents;
		int		eventindex;
		int		numframes;
		int		numpivots;
		int		pivotindex;
		int		motiontype;
		int		motionbone;
		vec3_t	linearmovement;
		int		automoveposindex;
		int		automoveangleindex;
		vec3_t	bbmin;
		vec3_t	bbmax;
		int		numblends;
		int		animindex;
		int		blendtype[2];
		float	blendstart[2];
		float	blendend[2];
		int		blendparent;
		int		seqgroup;
		int		entrynode;
		int		exitnode;
		int		nodeflags;
		int		nextseq;
	};


	struct mstudioevent_t
	{
		int		frame;
		int		event;
		int		type;
		char	options[64];
	};
}

#endif // GOLDSRC_STUDIO_SHARED_H