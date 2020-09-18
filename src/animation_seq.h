#include <Arduino.h>
#include "Ledsegment.h"
#include "global_init.h"
#include "stripe_init.h"


extern class Ledsegment stripe[];
extern class RBD::Timer sequence_timer; 
extern uint8_t animation_state;
extern bool anim_flag;
extern bool ddb_refresh;
//extern global_input;
extern uint8_t global_output;
extern uint8_t state_value;

// Alles einen Schritt weiter
void animation_step() {
	if(animation_state == 1) {
		stripe[0].stepUp();
	}
	if(stripe[0].stepUp()) {
		stripe[0].stop();
		animation_state = 0;
	}
}
