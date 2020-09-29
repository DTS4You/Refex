#include <Arduino.h>
#include "Ledsegment.h"
#include "global_init.h"
#include "stripe_init.h"


extern class Ledsegment stripe;
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
		// Debug on COM
		#ifdef DEBUG_COM
			Serial.println("Animation_State = 1 -> Step");
		#endif
		stripe.stepUp();
	}
	if(stripe.isEndposition()) {
		// Debug on COM
		#ifdef DEBUG_COM
			Serial.println("Animation_State = 0 -> Stop !!!");
		#endif
		stripe.stop();
		stripe.start();
		animation_state = 1;
		
		//global_output	= global_output |  ( 1 << 0 );
		//state_value		= state_value   & ~( 1 << 0 );

	}
}
