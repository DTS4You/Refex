// LED-Stripes werden initalisiert
#ifndef LED_SETUP_H
#define LED_SETUP_H

#include <Arduino.h>
#include "global_init.h"
#include "stripe_init.h"
#include "Ledsegment.h"


extern class Ledsegment stipe[];

void led_setup() {
	// Segmente definieren
	stripe[0].setup(STRIPE_01_DDB, STRIPE_01_START, STRIPE_01_NUM);

    for (uint8_t i=0; i<1; i++) {
		stripe[i].setColorDef(F_LED_DEF);
	}

	//-------------------------------------------------------------------------
	// Grundeinstellungen LED Stripes
	//-------------------------------------------------------------------------

	for (uint8_t i=0; i<1; i++) {
		stripe[i].showRange(F_LED_DEF);
	}
}

#endif
