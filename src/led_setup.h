// LED-Stripes werden initalisiert
#ifndef LED_SETUP_H
#define LED_SETUP_H

#include <Arduino.h>
#include "global_init.h"
#include "stripe_init.h"
#include "Ledsegment.h"


extern class Ledsegment stripe;

void led_setup() {

	// Segmente definieren
	stripe.setup(STRIPE_01_DDB, STRIPE_01_START, STRIPE_01_NUM);

	stripe.setColorDef(F_LED_DEF);
	stripe.setColorOn(F_LED_ON);

	stripe.showRange(F_LED_DEF);
	
}

#endif
