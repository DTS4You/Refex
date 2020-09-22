// ################################################################################
// ###                                                                          ###
// ### Projekt			: Refex 												###
// ### Version			: 0.21													###
// ### Datum			: 17.09.2020											###
// ###                                                                          ###
// ### Änderung			: keine													###
// ###																			###
// ### Hardware			: Arduino Nano											###
// ###					: Leiterplatte 17-0023-02								###
// ###					: 1 x Digi-Dot-Booster									###
// ###					: 1 x Taster mit LED Ring								###
// ################################################################################

// #define DEBUG_STRIPE
// #define DEBUG_COM

#include <arduino.h>
// #include "ascii_codes.h"
#include "global_init.h"
#include "stripe_init.h"


#include "DDBooster.h"
#include "RBD_Timer.h"  			// https://github.com/alextaujenis/RBD_Timer
#include "RBD_ButtonExtern.h"		// https://github.com/alextaujenis/RBD_Button
#include "color_tables.h"


#include "I2C.h"

#include "Ledsegment.h"
#include "animation_seq.h"
#include "led_setup.h"
// #include "collision_setup.h"

const uint8_t ddb_cs_pin[DDB_COUNT] 	= { DDB_1_CS_PIN };
const uint8_t ddb_num_led[DDB_COUNT]	= { DDB_1_MAX_LED };
bool ddb_refresh = false;

//uint8_t	global_input	= 0;	 // Wird im Objekt ButtonExtern erzeugt
uint8_t	global_output	= 0;
uint8_t blink_flag		= 0;
uint8_t state_value		= 0;
uint8_t animation_state	= 0;

// Digi-Dot-Booster Objekte erzeugen
DDBooster ddb_leds[DDB_COUNT];			// Objekt anlegen

// Timer Objekte anlegen
RBD::Timer io_task;
RBD::Timer button_blink;
RBD::Timer animation_timer;

// Button Objekte anlegen
RBD::ButtonExtern button_1(0);

// LED Segmente Objekte anlegen
Ledsegment stripe;

// ----------------------------------------------------------------------------
// Setup
// ----------------------------------------------------------------------------
void setup() {

	// Debug on COM
	#ifdef DEBUG_COM
		Serial.begin(115200); 		// open the serial port at 9600 bps:
	#endif

	// I2C Setup
	I2c.begin();
	I2c.setSpeed(1);
	I2c.write(MCP23017,0x05,0x00);
	I2c.write(MCP23017,0x00,0x00);	// Setup MCP23017 GPIO-A to Output
	I2c.write(MCP23017,0x01,0xFF);	// Setup MCP23017 GPIO-B to Input
	I2c.write(MCP23017,0x14,   0);	// MCP23017 auf Startwert -> Ausgänge 1-8 auf aus

	// Digi-Dot-Booster init
	for (uint8_t i = 0; i < DDB_COUNT; i++)
	{
		// Digi-Dot-Booster CS-Pin konfigurieren
		ddb_leds[i].configurePins(ddb_cs_pin[i]);
		delay(DDB_INIT_DELAY);
		// Digi-Dot-Booster -> Anzahl der LEDs pro Digi-Dot-Booster
		ddb_leds[i].init(ddb_num_led[i]);
		delay(DDB_INIT_DELAY);
		ddb_leds[i].clearAll();
		delay(DDB_INIT_DELAY);
		ddb_leds[i].show();
		delay(DDB_INIT_DELAY);
	}

	button_1.invertReading();

	// Timer definieren und starten
	io_task.setTimeout(IO_LOOP_TIME);
	io_task.restart();
	//refresh_task.setTimeout(Refresh_Time);
	//refresh_task.restart();
	button_blink.setTimeout(BUTTON_BLINK_TIME);
	button_blink.restart();
	animation_timer.setTimeout(ANIMATION_TIME);
	animation_timer.restart();

	led_setup();
	// collision_setup();
	
	ddb_refresh = true;
	//-------------------------------------------------------------------------

	global_output = 0xFF;

	// Debug on COM
	#ifdef DEBUG_COM
		Serial.println("Start");
	#endif
}

// ----------------------------------------------------------------------------
// Main Loop
// ----------------------------------------------------------------------------
void loop() {

	// IO-Timer
	if(io_task.onRestart()) {
		// MCP23017 Input einlesen und auf Output ausgeben
		I2c.write(MCP23017, 0x13);
		I2c.read(MCP23017, 1);
		global_input	= I2c.receive();
		//global_output	= global_input;
		I2c.write(MCP23017,0x14,global_output);
	}
	// Blink-Timer
	if(button_blink.onRestart()) {
		if (blink_flag) {
			global_output = global_output | state_value;
		}
		else {
			global_output = global_output & ~state_value;
		}
		blink_flag = !blink_flag;
	}
	//---------------------------------------------------------------------------
	// Animation Timer -> Alles einen Schritt weiter
	//---------------------------------------------------------------------------
	if(animation_timer.onRestart()) {
		if(animation_state > 0) {
			animation_step();
			ddb_refresh = true;
		}
 	}
	//---------------------------------------------------------------------------
	// Tasten abfragen
	//---------------------------------------------------------------------------
	// Taste 1 ist betätiget worden
	if(button_1.onPressed()) {
		// Debug on COM
		#ifdef DEBUG_COM
			Serial.println("Taste gedrückt");
		#endif
		global_output	= global_output & ~( 1 << 0 );
		state_value		= state_value   |  ( 1 << 0 );
		// Nur wenn keine Animation mehr läuft kann neu gestartet werden
		if(animation_state == 0) {
			// Debug on COM
			#ifdef DEBUG_COM
				Serial.println("Taste gedrückt -> Animation_State = 0");
			#endif
			stripe.start();
			animation_state = 1;
			//animation_seq();
			animation_timer.restart();
		}
	}
	//-------------------------------------------------------------------------
	// Refresh Task -> Alle Digi-Dot Booster ddb.show() aufrufen
	//-------------------------------------------------------------------------
	if(ddb_refresh)
	{
		delay(DDB_INIT_DELAY);			// Workaround for DDB SPI Problem
		for (uint8_t i = 0; i < DDB_COUNT; i++)
		{
			ddb_leds[i].show();
			delay(DDB_INIT_DELAY);		// Workaround for DDB SPI Problem
		}
		ddb_refresh = false;
	}
}
