#include "cbase.h"
#include "goldsrc_model.h"
#include "studio.h"
#include "bone_setup.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


namespace GoldSRC
{
	const char *szActivityNames[] = {
		"ACT_RESET",
		"ACT_IDLE",
		"ACT_GUARD",
		"ACT_WALK",
		"ACT_RUN",
		"ACT_FLY",
		"ACT_SWIM",
		"ACT_HOP",
		"ACT_LEAP",
		"ACT_FALL",
		"ACT_LAND",
		"ACT_STRAFE_LEFT",
		"ACT_STRAFE_RIGHT",
		"ACT_ROLL_LEFT",
		"ACT_ROLL_RIGHT",
		"ACT_TURN_LEFT",
		"ACT_TURN_RIGHT",
		"ACT_CROUCH",
		"ACT_CROUCHIDLE",
		"ACT_STAND",
		"ACT_USE",
		"ACT_SIGNAL1",
		"ACT_SIGNAL2",
		"ACT_SIGNAL3",
		"ACT_TWITCH",
		"ACT_COWER",
		"ACT_SMALL_FLINCH",
		"ACT_BIG_FLINCH",
		"ACT_RANGE_ATTACK1",
		"ACT_RANGE_ATTACK2",
		"ACT_MELEE_ATTACK1",
		"ACT_MELEE_ATTACK2",
		"ACT_RELOAD",
		"ACT_ARM",
		"ACT_DISARM",
		"ACT_EAT",
		"ACT_DIESIMPLE",
		"ACT_DIEBACKWARD",
		"ACT_DIEFORWARD",
		"ACT_DIEVIOLENT",
		"ACT_BARNACLE_HIT",
		"ACT_BARNACLE_PULL",
		"ACT_BARNACLE_CHOMP",
		"ACT_BARNACLE_CHEW",
		"ACT_SLEEP",
		"ACT_INSPECT_FLOOR",
		"ACT_INSPECT_WALL",
		"ACT_IDLE_ANGRY",
		"ACT_WALK_HURT",
		"ACT_RUN_HURT",
		"ACT_HOVER",
		"ACT_GLIDE",
		"ACT_FLY_LEFT",
		"ACT_FLY_RIGHT",
		"ACT_DETECT_SCENT",
		"ACT_SNIFF",
		"ACT_BITE",
		"ACT_THREAT_DISPLAY",
		"ACT_FEAR_DISPLAY",
		"ACT_EXCITED",
		"ACT_SPECIAL_ATTACK1",
		"ACT_SPECIAL_ATTACK2",
		"ACT_COMBAT_IDLE",
		"ACT_WALK_SCARED",
		"ACT_RUN_SCARED",
		"ACT_VICTORY_DANCE",
		"ACT_DIE_HEADSHOT",
		"ACT_DIE_CHESTSHOT",
		"ACT_DIE_GUTSHOT",
		"ACT_DIE_BACKSHOT",
		"ACT_FLINCH_HEAD",
		"ACT_FLINCH_CHEST",
		"ACT_FLINCH_STOMACH",
		"ACT_FLINCH_LEFTARM",
		"ACT_FLINCH_RIGHTARM",
		"ACT_FLINCH_LEFTLEG",
		"ACT_FLINCH_RIGHTLEG",
	};

	int NUM_ACTS = 77;
}


void SetActivityForSequence( CStudioHdr *pstudiohdr, int i )
{
	mstudioseqdesc_t &seqdesc = pstudiohdr->pSeqdesc( i );

	const char *pszActivityName = seqdesc.pszActivityName();

	seqdesc.flags |= STUDIO_ACTIVITY;
	
	if ( pszActivityName[0] )
	{
		for ( int a = 0; a < GoldSRC::NUM_ACTS; a++ )
		{
			if ( !Q_strcmp( pszActivityName, GoldSRC::szActivityNames[a] ) )
			{
				seqdesc.activity = a;
				return;
			}
		}
	}
}


GoldSRC::studiohdr_t *WrapStudioModel( CStudioHdr *pHdr )
{
	// Force the activity mapping to initialize
	pHdr->HaveSequenceForActivity( 0 );

	GoldSRC::studiohdr_t tempHeader = {};
	int allocSize = sizeof( GoldSRC::studiohdr_t );
	int allocPos = allocSize;

	// header stuff used by server
	VectorCopy( pHdr->eyeposition().Base(), tempHeader.eyeposition );

	// Sequences
	tempHeader.numseq = pHdr->GetNumSeq();
	tempHeader.seqindex = allocPos;
	GoldSRC::mstudiosequence_t *pSequences = new GoldSRC::mstudiosequence_t[tempHeader.numseq];
	CUtlVector<GoldSRC::mstudioevent_t *> pSequenceEvents;

	float poseparameters[24];

	for ( int i = 0; i < tempHeader.numseq; i++ )
	{
		mstudioseqdesc_t &srcDesc = pHdr->pSeqdesc( i );
		GoldSRC::mstudiosequence_t *pDestDesc = &pSequences[i];

		Q_memset( pDestDesc, 0, sizeof( GoldSRC::mstudiosequence_t ) );

		Vector deltaMovement;
		QAngle deltaAngle;
		Studio_SeqMovement( pHdr, i, 0.0, 1.0, poseparameters, deltaMovement, deltaAngle );

		float fps = Studio_FPS( pHdr, i, poseparameters );
		float duration = Studio_Duration( pHdr, i, poseparameters );

		Q_strncpy( pDestDesc->label, srcDesc.pszLabel(), 32 );
		pDestDesc->fps = fps;
		//pDestDesc->flags
		pDestDesc->activity = srcDesc.activity;
		pDestDesc->actweight = srcDesc.actweight;
		pDestDesc->numevents = srcDesc.numevents;
		//pDestDesc->eventindex	- filled in later
		pDestDesc->numframes = fps * duration;
		//pDestDesc->numpivots
		//pDestDesc->motiontype
		//pDestDesc->motionbone
		VectorCopy( deltaMovement.Base(), pDestDesc->linearmovement );
		//pDestDesc->automoveposindex
		//pDestDesc->automoveangleindex
		VectorCopy( srcDesc.bbmin.Base(), pDestDesc->bbmin );
		VectorCopy( srcDesc.bbmax.Base(), pDestDesc->bbmax );
		//pDestDesc->numblends
		//pDestDesc->animindex
		//pDestDesc->blendtype[2]
		//pDestDesc->blendstart[2]
		//pDestDesc->blendend[2]
		//pDestDesc->blendparent
		//pDestDesc->seqgroup
		//pDestDesc->entrynode
		//pDestDesc->exitnode
		//pDestDesc->nodeflags
		pDestDesc->nextseq = srcDesc.nextseq;

		pSequenceEvents.AddToTail( new GoldSRC::mstudioevent_t[pDestDesc->numevents] );
		GoldSRC::mstudioevent_t *pEvents = pSequenceEvents.Element( i );

		for ( int e = 0; e < pDestDesc->numevents; e++ )
		{
			mstudioevent_t *pSrcEvent = srcDesc.pEvent( e );
			pEvents[e].frame = pSrcEvent->cycle * pDestDesc->numframes;
			pEvents[e].event = pSrcEvent->event;
			pEvents[e].type = pSrcEvent->type;
			Q_strncpy( pEvents[e].options, pSrcEvent->options, 64 );
		}

		allocPos += sizeof( GoldSRC::mstudiosequence_t );
		allocSize += sizeof( GoldSRC::mstudiosequence_t ) + pDestDesc->numevents * sizeof( GoldSRC::mstudioevent_t );
	}

	// Copy In header
	void *pAllocModel = malloc( allocSize );
	Q_memcpy( pAllocModel, &tempHeader, sizeof( GoldSRC::studiohdr_t ) );

	// Copy in events
	for ( int i = 0; i < tempHeader.numseq; i++ )
	{
		pSequences[i].eventindex = allocPos;

		int copySize = pSequences[i].numevents * sizeof( GoldSRC::mstudioevent_t );

		Q_memcpy( (byte *)pAllocModel + allocPos, pSequenceEvents.Element( i ), copySize);
		allocPos += copySize;

		delete[] pSequenceEvents.Element( i );
	}

	// Copy in sequences
	Q_memcpy( (byte *)pAllocModel + tempHeader.seqindex, pSequences, tempHeader.numseq * sizeof( GoldSRC::mstudiosequence_t ) );
	delete[] pSequences;

	return (GoldSRC::studiohdr_t *)pAllocModel;
}


void UnwrapStudioModel( GoldSRC::studiohdr_t *pModel )
{
	free( pModel );
}
