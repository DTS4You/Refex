// LED-Stripes werden initalisiert
#ifndef LED_SETUP_H
#define LED_SETUP_H

#include <Arduino.h>
#include "global_init.h"
#include "stripe_init.h"
#include "Ledsegment.h"


extern class Ledsegment stripe[];

void led_setup() {

	// Segmente definieren
	stripe[0].setup(STRIPE_01_DDB, STRIPE_01_START, STRIPE_01_NUM);

	stripe[0].setColorDef(F_LED_DEF);

	stripe[0].showRange(F_LED_DEF);
	
}

#endif
