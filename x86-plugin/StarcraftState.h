#ifndef __STARCRAFT_STATE_H__
#define __STARCRAFT_STATE_H__

/* These are some states I defined for the Starcraft login to help determine when 
 * to do some patching (we want to patch as soon as possible after CheckRevision and
 * ExtraWork. */

typedef enum
{
	/* Starcraft has just started, Battle.net has been exited. */
	SC_NEW,
	/* ws2_32.dll is loaded, connection is happening (send() is patched). */
	SC_STARTINGCONNECT,
	/* CheckRevision .dll file loaded. */
	SC_CHECKREVISION_HAPPENING,
	/* CheckRevision .dll file freed. */
	SC_CHECKREVISION_OVER,
	/* ExtraWork .dll file loaded. */
	SC_EXTRAWORK_HAPPENING,
	/* ExtraWork .dll file freed (we're now free to do all patches). */
	SC_EXTRAWORK_OVER
} t_sc_states;

/* Text based states. */
static char *sc_states[] = { "SC_NEW", "SC_STARTINGCONNECT", "SC_CHECKREVISION_HAPPENING", "SC_CHECKREVISION_OVER", "SC_EXTRAWORK_HAPPENING", "SC_EXTRAWORK_OVER" };

/* All we have is a getter/setter. Simple stuff. */
void state_set(t_sc_states state);
t_sc_states state_get();

#endif