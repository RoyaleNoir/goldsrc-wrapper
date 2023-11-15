#include "cbase.h"
#include "goldsrc_input.h"
#include "cdll_int.h"
#include "globalvars_base.h"

#include <vgui/ISurface.h>
#include <vgui/IInput.h>
#include <vgui_controls/Controls.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IVEngineClient *engine;
extern CGlobalVarsBase *gpGlobals;

static CGoldSRCInput g_GoldSRCInput;
CGoldSRCInput *g_pGoldSRCInput = &g_GoldSRCInput;

static CUserCmdList s_NullList = { {}, 1, -1 };

struct kbutton_t
{
	int down[2];
	int state;
};


kbutton_t	in_left = {};
kbutton_t	in_right = {};
kbutton_t	in_forward = {};
kbutton_t	in_back = {};
kbutton_t	in_moveleft = {};
kbutton_t	in_moveright = {};
kbutton_t	in_use = {};
kbutton_t	in_jump = {};
kbutton_t	in_duck = {};


void KeyDown( kbutton_t *b, const char *c )
{
	int k = -1;
	
	if ( c && c[0] )
	{
		k = atoi( c );
	}

	if ( k == b->down[0] || k == b->down[1] )
		return;

	if ( !b->down[0] )
		b->down[0] = k;
	else if ( !b->down[1] )
		b->down[1] = k;
	else
		return;

	if ( b->state & 1 )
		return;

	b->state |= 1 + 2;
}


void KeyUp( kbutton_t *b, const char *c )
{
	if ( !c || !c[0] )
	{
		b->down[0] = b->down[1] = 0;
		b->state = 4;
		return;
	}

	int k = atoi( c );

	if ( b->down[0] == k )
		b->down[0] = 0;
	else if ( b->down[1] == k )
		b->down[1] = 0;
	else
		return;

	if ( k == b->down[0] || k == b->down[1] )
		return;

	if ( !( b->state & 1 ) )
		return;

	b->state &= ~1;
	b->state |= 4;
}


void IN_LeftDown( const CCommand &args ) { KeyDown( &in_left, args[1] ); }
void IN_LeftUp( const CCommand &args ) { KeyUp( &in_left, args[1] ); }
void IN_RightDown( const CCommand &args ) { KeyDown( &in_right, args[1] ); }
void IN_RightUp( const CCommand &args ) { KeyUp( &in_right, args[1] ); }
void IN_ForwardDown( const CCommand &args ) { KeyDown( &in_forward, args[1] ); }
void IN_ForwardUp( const CCommand &args ) { KeyUp( &in_forward, args[1] ); }
void IN_BackDown( const CCommand &args ) { KeyDown( &in_back, args[1] ); }
void IN_BackUp( const CCommand &args ) { KeyUp( &in_back, args[1] ); }
void IN_MoveleftDown( const CCommand &args ) { KeyDown( &in_moveleft, args[1] ); }
void IN_MoveleftUp( const CCommand &args ) { KeyUp( &in_moveleft, args[1] ); }
void IN_MoverightDown( const CCommand &args ) { KeyDown( &in_moveright, args[1] ); }
void IN_MoverightUp( const CCommand &args ) { KeyUp( &in_moveright, args[1] ); }
void IN_UseDown( const CCommand &args ) { KeyDown( &in_use, args[1] ); }
void IN_UseUp( const CCommand &args ) { KeyUp( &in_use, args[1] ); }
void IN_JumpDown( const CCommand &args ) { KeyDown( &in_jump, args[1] ); }
void IN_JumpUp( const CCommand &args ) { KeyUp( &in_jump, args[1] ); }
void IN_DuckDown( const CCommand &args ) { KeyDown( &in_duck, args[1] ); }
void IN_DuckUp( const CCommand &args ) { KeyUp( &in_duck, args[1] ); }

static ConCommand startleft( "+left", IN_LeftDown );
static ConCommand endleft( "-left", IN_LeftUp );
static ConCommand startright( "+right", IN_RightDown );
static ConCommand endright( "-right", IN_RightUp );
static ConCommand startforward( "+forward", IN_ForwardDown );
static ConCommand endforward( "-forward", IN_ForwardUp );
static ConCommand startback( "+back", IN_BackDown );
static ConCommand endback( "-back", IN_BackUp );
static ConCommand startmoveleft( "+moveleft", IN_MoveleftDown );
static ConCommand endmoveleft( "-moveleft", IN_MoveleftUp );
static ConCommand startmoveright( "+moveright", IN_MoverightDown );
static ConCommand endmoveright( "-moveright", IN_MoverightUp );
static ConCommand startuse( "+use", IN_UseDown );
static ConCommand enduse( "-use", IN_UseUp );
static ConCommand startjump( "+jump", IN_JumpDown );
static ConCommand endjump( "-jump", IN_JumpUp );
static ConCommand startduck( "+duck", IN_DuckDown );
static ConCommand endduck( "-duck", IN_DuckUp );


float CL_KeyState( kbutton_t *key )
{	
	int impulsedown = key->state & 2;
	int impulseup = key->state & 4;
	int down = key->state & 1;

	float val = 0.0f;
	
	if ( impulsedown && !impulseup )
		val = down ? 0.5 : 0.0;

	if ( impulseup && !impulsedown )
		val = down ? 0.0 : 0.0;

	if ( !impulsedown && !impulseup )
		val = down ? 1.0 : 0.0;

	if ( impulsedown && impulseup )
	{
		if ( down )
			val = 0.75;	
		else
			val = 0.25;	
	}

	// clear impulses
	key->state &= 1;		
	return val;
}


CGoldSRCInput::CGoldSRCInput()
{
	ClearAllLists();
	m_bMouseActive = false;
}


CGoldSRCInput::~CGoldSRCInput()
{
}


void CGoldSRCInput::Init()
{
	m_bMouseActive = false;

	m_flPrevMouseX = 0;
	m_flPrevMouseY = 0;

	m_nLastSampleFrame = -1;
	m_flTotalSampleTime = 0;
}


void CGoldSRCInput::ClearAllLists()
{
	m_nListToWrite = 0;

	Q_memset( m_CmdLists, 0, sizeof( m_CmdLists ) );
	Q_memset( m_nListsForSeqNums, -1, sizeof( m_nListsForSeqNums ) );
	m_CmdLists[0].m_nSeqNum = -1;
}


void CGoldSRCInput::ActivateMouse()
{
	if ( m_bMouseActive )
		return;

	m_bMouseActive = true;

	int w, h;
	engine->GetScreenSize( w, h );
	vgui::input()->SetCursorPos( w >> 1, h >> 1 );

	m_flAccumulatedMouseX = 0;
	m_flAccumulatedMouseY = 0;

	//int rawX, rawY;
	//inputsystem->GetRawMouseAccumulators( rawX, rawY );
}


void CGoldSRCInput::DeactivateMouse()
{
	if ( !m_bMouseActive )
		return;

	m_bMouseActive = false;

	vgui::surface()->SetCursor( vgui::dc_arrow );

	int w, h;
	engine->GetScreenSize( w, h );
	vgui::input()->SetCursorPos( w >> 1, h >> 1 );

	m_flAccumulatedMouseX = 0;
	m_flAccumulatedMouseY = 0;
}


void CGoldSRCInput::AccumulateMouse()
{
	int w, h;
	engine->GetScreenSize( w, h );

	int cx = w >> 1;
	int cy = h >> 1;

	if ( vgui::surface()->IsCursorLocked() )
	{
		int currentX, currentY;
		vgui::input()->GetCursorPos( currentX, currentY );

		m_flAccumulatedMouseX += currentX - cx;
		m_flAccumulatedMouseY += currentY - cy;

		vgui::input()->SetCursorPos( cx, cy );
	}
	else if ( m_bMouseActive )
	{
		int ox, oy;
		vgui::input()->GetCursorPos( ox, oy );
		ox = clamp( ox, 0, w - 1 );
		oy = clamp( oy, 0, h - 1 );
		vgui::input()->SetCursorPos( ox, oy );

	}
}


void CGoldSRCInput::ClearMouseState()
{
	if ( !m_bMouseActive )
		return;

	m_flAccumulatedMouseX = 0;
	m_flAccumulatedMouseY = 0;

	//int rawX, rawY;
	//inputsystem->GetRawMouseAccumulators( rawX, rawY );
}


void CGoldSRCInput::CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
	// Set the sequence number now
	m_CmdLists[m_nListToWrite].m_nSeqNum = sequence_number;
	m_nListsForSeqNums[sequence_number % NUM_STORED_CMDLISTS] = m_nListToWrite;

	//if ( !active )
	//{
	//	m_CmdLists[m_nListToWrite].m_nNumCmds = 0;
	//}

	// Make sure it doesn't overflow 
	int indexInList = m_CmdLists[m_nListToWrite].m_nNumCmds;
	if ( indexInList >= MAX_CMDLIST_SIZE )
	{
		Warning( "Too many user input frames (%d)!\n", indexInList );
		indexInList = MAX_CMDLIST_SIZE - 1;
	}

	m_CmdLists[m_nListToWrite].m_nNumCmds = indexInList + 1;

	GoldSRC::usercmd_t *pCmd = &m_CmdLists[m_nListToWrite].m_Cmds[indexInList];
	CreateMoveInternal( pCmd, input_sample_frametime, active );
	pCmd->msec = clamp( input_sample_frametime * 1000, 0, 255 );

	// if this is the first sample this frame, clear the values
	if ( gpGlobals->framecount != m_nLastSampleFrame )
	{
		m_flTotalSampleTime = 0.0f;
		m_nLastSampleFrame = gpGlobals->framecount;
	}

	// Add on this sample to the time
	m_flTotalSampleTime += input_sample_frametime;

	//if ( active )
	//	Msg( "[C] t=%d rdt=%f dt=%f tot=%f\n", gpGlobals->framecount, gpGlobals->frametime, input_sample_frametime, m_flTotalSampleTime );

	// That was the last one in the list, so move onto the next one
	m_nListToWrite = ( m_nListToWrite + 1 ) % NUM_STORED_CMDLISTS;
	m_CmdLists[m_nListToWrite].m_nNumCmds = 0;
	m_CmdLists[m_nListToWrite].m_nSeqNum = -1;
}


void CGoldSRCInput::ExtraMouseSample( float frametime, bool active )
{
	// Avoid creating too many usercmds when paused
	//if ( !active )
	//	return;

	int indexInList = m_CmdLists[m_nListToWrite].m_nNumCmds;
	if ( indexInList >= MAX_CMDLIST_SIZE )
	{
		Warning( "Too many user input frames (%d)!\n", indexInList );
		indexInList = MAX_CMDLIST_SIZE - 1;
	}

	// Clear the sampletime if this is the first one this frame
	if ( gpGlobals->framecount != m_nLastSampleFrame )
		m_flTotalSampleTime = 0.0f;

	// Only run if there's time left in the frame
	if ( m_flTotalSampleTime < frametime )
		m_CmdLists[m_nListToWrite].m_nNumCmds = indexInList + 1;

	GoldSRC::usercmd_t *pCmd = &m_CmdLists[m_nListToWrite].m_Cmds[indexInList];

	// Only actually run this for the remaining amount of time in the frame
	CreateMoveInternal( pCmd, MAX( frametime - m_flTotalSampleTime, 0 ), active );
	pCmd->msec = clamp( ( frametime - m_flTotalSampleTime ) * 1000, 0, 255 );

	//Msg( "[E] t=%d rdt=%f dt=%f tot=%f rem=%f\n", gpGlobals->framecount, gpGlobals->frametime, frametime, m_flTotalSampleTime, remainingTime );
}

void CGoldSRCInput::CreateMoveInternal( GoldSRC::usercmd_t *pCmd, float frametime, bool active )
{
	QAngle viewAngles;

	// Cut down version of hl1 code for now
	if ( active )
	{
		// Get the view angles
		//QAngle viewAngles;
		engine->GetViewAngles( viewAngles );

		//AdjustAngles( input_sample_frametime, viewAngles );
		viewAngles.y -= frametime * 210 * CL_KeyState( &in_right );
		viewAngles.y += frametime * 210 * CL_KeyState( &in_left );

		// Set to zero
		Q_memset( pCmd, 0, sizeof( GoldSRC::usercmd_t ) );

		engine->SetViewAngles( viewAngles );

		// Side Move
		pCmd->sidemove += 400 * CL_KeyState( &in_moveright );
		pCmd->sidemove -= 400 * CL_KeyState( &in_moveleft );

		// Up Move

		// Forward Move
		pCmd->forwardmove += 400 * CL_KeyState( &in_forward );
		pCmd->forwardmove -= 400 * CL_KeyState( &in_back );

		// Scale

		// Input
		if ( m_bMouseActive )
			MouseMove( frametime, pCmd );
	}

	// Impulse command
	pCmd->impulse = 0;

	// Weapon selection
	pCmd->weaponselect = 0;

	// Button bits
	pCmd->buttons = 0;

	if ( in_duck.state & 3 )
		pCmd->buttons |= ( 1 << 2 );
	if ( in_jump.state & 3 )
		pCmd->buttons |= ( 1 << 1 );
	if ( in_use.state & 3 )
		pCmd->buttons |= ( 1 << 5 );

	//in_attack.state &= ~2;
	in_duck.state &= ~2;
	in_jump.state &= ~2;
	in_forward.state &= ~2;
	in_back.state &= ~2;
	in_use.state &= ~2;
	in_left.state &= ~2;
	in_right.state &= ~2;
	in_moveleft.state &= ~2;
	in_moveright.state &= ~2;
	//in_attack2.state &= ~2;
	//in_reload.state &= ~2;
	//in_alt1.state &= ~2;
	//in_score.state &= ~2;

	// Joystick

	// View angles
	engine->GetViewAngles( viewAngles );
	VectorCopy( viewAngles.Base(), pCmd->viewangles );
}

bool CGoldSRCInput::WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand )
{
	CUserCmdList *pFromList = GetCmdList( from );
	CUserCmdList *pToList = GetCmdList( to );
	
	int nCmds = pToList->m_nNumCmds;
	buf->WriteByte( nCmds );

	// Start with the last entry in the previous list
	GoldSRC::usercmd_t *pFrom = &pFromList->m_Cmds[pFromList->m_nNumCmds - 1];

	for ( int i = 0; i < nCmds; i++ )
	{
		GoldSRC::usercmd_t* pTo = &pToList->m_Cmds[i];

		if ( !WriteUserCmdDelta( buf, pFrom, pTo ) )
			return false;

		pFrom = pTo;
	}

	return true;
}


void CGoldSRCInput::MouseMove( float frametime, GoldSRC::usercmd_t *pCmd )
{
	QAngle viewAngles;
	engine->GetViewAngles( viewAngles );

	if ( !vgui::surface()->IsCursorVisible() )
	{
		AccumulateMouse();

		float mx = m_flAccumulatedMouseX;
		float my = m_flAccumulatedMouseY;
		
		m_flAccumulatedMouseX = 0;
		m_flAccumulatedMouseY = 0;

		// Scale
		mx *= 3; // sensitivity
		my *= 3; // sensitivity

		// Command
		viewAngles[YAW] -= 0.022f * mx;
		viewAngles[PITCH] += 0.022f * my;

		if ( viewAngles[PITCH] > 89 )
			viewAngles[PITCH] = 89;
		if ( viewAngles[PITCH] < -89 )
			viewAngles[PITCH] = -89;

		// Reset
		if ( mx || my )
		{
			int w, h;
			engine->GetScreenSize( w, h );
			vgui::input()->SetCursorPos( w >> 1, h >> 1 );
		}
	}


	engine->SetViewAngles( viewAngles );
}


CUserCmdList *CGoldSRCInput::GetCmdList( int seqNum )
{
	if ( seqNum == -1 )
		return &s_NullList;

	int listIndex = m_nListsForSeqNums[seqNum % NUM_STORED_CMDLISTS];

	if ( listIndex == -1 )
		return &s_NullList;

	CUserCmdList *pList = &m_CmdLists[listIndex];

	if ( pList->m_nSeqNum != seqNum )
		return &s_NullList;

	return pList;
}


#define WRITE_DELTA_I( x, t, b, s ) if ( pFrom->##x != pTo->##x )\
{\
	buf->WriteOneBit( 1 );\
	buf->Write##t( pTo->##x, b );\
}\
else\
{\
	buf->WriteOneBit( 0 );\
}

#define WRITE_DELTA_F( x, t, b, s ) if ( pFrom->##x != pTo->##x )\
{\
	buf->WriteOneBit( 1 );\
	buf->Write##t( (int)( pTo->##x * s ), b );\
}\
else\
{\
	buf->WriteOneBit( 0 );\
}


bool CGoldSRCInput::WriteUserCmdDelta( bf_write *buf, const GoldSRC::usercmd_t *pFrom, const GoldSRC::usercmd_t *pTo )
{
	WRITE_DELTA_I	( lerp_msec,			UBitLong,	9,	1.0f );
	WRITE_DELTA_I	( msec,					UBitLong,	8,	1.0f );
	WRITE_DELTA_I	( viewangles[1],		BitAngle,	16,	1.0f );
	WRITE_DELTA_I	( viewangles[0],		BitAngle,	16,	1.0f );
	WRITE_DELTA_I	( buttons,				UBitLong,	8,	1.0f );
	WRITE_DELTA_F	( forwardmove,			SBitLong,	12,	1.0f );
	WRITE_DELTA_I	( lightlevel,			UBitLong,	8,	1.0f );
	WRITE_DELTA_F	( sidemove,				SBitLong,	12,	1.0f );
	WRITE_DELTA_F	( upmove,				SBitLong,	12,	1.0f );
	WRITE_DELTA_I	( impulse,				UBitLong,	8,	1.0f );
	WRITE_DELTA_I	( viewangles[2],		BitAngle,	16,	1.0f );
	WRITE_DELTA_I	( impact_index,			UBitLong,	6,	1.0f );
	WRITE_DELTA_F	( impact_position[0],	SBitLong,	16,	8.0f );
	WRITE_DELTA_F	( impact_position[1],	SBitLong,	16,	8.0f );
	WRITE_DELTA_F	( impact_position[2],	SBitLong,	16,	8.0f );

	if ( buf->IsOverflowed() )
	{
		Warning( "UserCmd buffer overflowed!\n" );
		return false;
	}

	return true;
}
