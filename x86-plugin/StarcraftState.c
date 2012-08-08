#include "Logging.h"

#include "StarcraftState.h"

static t_sc_states sc_state = SC_NEW;

void state_set(t_sc_states state)
{
	log_add("State: %s -> %s", sc_states[sc_state], sc_states[state]);
	sc_state = state;
}

t_sc_states state_get()
{
	return sc_state;
}
