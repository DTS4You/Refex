
#include <Arduino.h>
#include "Ledsegment.h"
#include "global_init.h"
#include "DDBooster.h"

#include "color_tables.h"

extern class DDBooster ddb_leds[];

extern uint8_t color[];

Ledsegment::Ledsegment() {
	_ddb				= 0;
	_num				= 0;
	_begin 				= 0;
	_step 				= 0;
	_pos				= 0;
	_color_def			= 0;
	_color_off			= 0;
	_color_on			= 0;
	_dir 				= false;		// false -> right ; true -> left
	_toggle				= false;
	_end_of_run			= false;
	_blank_start_end	= false;		// Start and end with a blank LED
}

Ledsegment::~Ledsegment() {
    //dtor
}
// Led Segmnent Setup
// ddb , begin , number
void Ledsegment::setup(uint8_t ddb, uint8_t begin, uint8_t number) {
	_ddb		= ddb;
	_begin 		= begin;
	_num		= number;
}
// Led Segment set color def
void Ledsegment::setColorDef(uint8_t c_index) {
	_color_def 	= c_index;
}
// Led Segment set color off
void Ledsegment::setColorOff(uint8_t c_index) {
	_color_off 	= c_index;
}
// Led Segment set color off
void Ledsegment::setColorOn(uint8_t c_index) {
	_color_on 	= c_index;
}
// Led Segment Reset
void Ledsegment::reset() {
	_dir 		= false;
	_end_of_run	= false;
	stop();
}
// Led segment set to default with Color-Index
void Ledsegment::showRange(uint8_t c_index) {
	ddb_leds[_ddb].setRGB(color_[c_index][0], color_[c_index][1], color_[c_index][2]);
	ddb_leds[_ddb].setRange(_begin, _begin + _num - 1);
	//led_stripe[_ddb].show();
}
// Led segment color on position with Color-Index
void Ledsegment::showPosition(uint8_t c_index) {
	ddb_leds[_ddb].setRGB(color_[c_index][0], color_[c_index][1], color_[c_index][2]);
	ddb_leds[_ddb].setLED(_begin + _pos);
	//led_stripe[_ddb].show();
}
void Ledsegment::start() {
	_enable = true;
}
void Ledsegment::stop() {
	_step		= 0;
	_enable 	= false;
	_end_of_run	= false;
	showRange(_color_def);
}
// Led segment set direction right
void Ledsegment::setRunRight() {
	_dir	= false;
}
// Led segment set direction left
void Ledsegment::setRunLeft() {
	_dir 	= true;
}
// Led segment set toggle on
void Ledsegment::setRunToggleOn() {
	_toggle = true;
}
// Led animation step
bool Ledsegment::stepUp() {
	if(_enable) {
		showRange(_color_def);
		if (_end_of_run == true) {
			_step 		= 0;
			_end_of_run	= false;
		}
		if (_step < _num  ) {
			if (!_dir) {
				_pos = _step;
			} else {
				_pos = _num - _step - 1;
			}
			//showRange(_color_def);
			showPosition(_color_on);
			_step++;
		} else {
			if(_toggle) {
				_dir = !_dir;
			}
			_end_of_run = true;
		}
	}
	return _end_of_run;
}

uint8_t Ledsegment::getPosition() {
	return _step;
}
