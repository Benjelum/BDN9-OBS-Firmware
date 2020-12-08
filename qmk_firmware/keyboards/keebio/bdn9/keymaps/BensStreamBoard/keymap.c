/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include QMK_KEYBOARD_H
//Set up the follwing macros in your machine:
//OBS Mute - Ctrl,Alt,Shift+F20
//OBS Unmute - C+A+S+F21
//toggle mute discord - Page up

//Mapable Buttons:
//Stream Buttons Ctrl,Alt,Shift+F13-F19 
//   count up from F13 at the upper left key, reading order (left to right, top down)
//   the top middle button is F19


void matrix_user_init(void){
	//don't perform LED logid here
	backlight_enable();
}
void keyboard_post_init_user(void) {
// RGB Modes
// 1 = Static
// 2-5 = Breathing
// 6-8 = Rainbow
// 9-14 = Swirl
// 15-20 = Snake
// 21-24 = Nightrider
// 25 = Christmas
// 26-30 = Static Gradient
//rgblight_mode(desired);

	rgblight_sethsv_noeeprom(HSV_BLUE);//default untouched position
    rgblight_enable_noeeprom(); // enables Rgb, without saving settings to ensure reset on power loss
    rgblight_mode_noeeprom(3); //default light mode after plugged in
	//per layer customizations can be added in SetLayerLights()
}
/*
==============================
LAYERS & LIGHTS
==============================
*/
//define layers for QMK
#define SFX1   TO(_SFX1)
#define SFX2  TO(_SFX2)
#define SFX3 TO(_SFX3)
#define SFX4 TO(_SFX4)
#define STREAM    TO(_STREAM)
//enum for qmk keymap assignments
enum my_layers {
    _SFX1 = 0,
    _SFX2,
    _SFX3,
    _SFX4,
    _STREAM
};
//for use in encoder logic
bool sfx1_mode = false;
bool sfx2_mode = false;
bool sfx3_mode = false;
bool sfx4_mode = false;
bool stream_mode = false;

uint8_t selected_knob_pos = 1;//defaults to middle pos 1
bool toggle_mute_all_enabled = false; //mute all defaults off
uint8_t selected_layer = 0; //instatiate layer selector variable globally
uint8_t selected_hue_mode = 1; //for use if you want to tweak rgb with knobs
uint8_t selected_rgb_brightness = 75; //for use if you want to tweak rgb rightness with knobs

//light logic for stream layer per knob position
void UpdateLightsForKnobPosition(void){
	if (toggle_mute_all_enabled){
		rgblight_mode_noeeprom(23);//Red knight rider mode to show mute
		rgblight_sethsv_noeeprom(HSV_RED);
	}
	else{
		rgblight_mode_noeeprom(1);
		switch(selected_knob_pos){
				case 0: 
					rgblight_sethsv_noeeprom(HSV_GREEN);
					break;
				case 1:
					rgblight_sethsv_at(HSV_GREEN, 0);
					rgblight_sethsv_at(HSV_GREEN, 1);
					rgblight_sethsv_at(HSV_PURPLE, 2);
					rgblight_sethsv_at(HSV_PURPLE, 3);
					break;
				case 2:
					rgblight_sethsv_noeeprom(HSV_PURPLE);
					break;
			}
	}
}
//per layer custom light logic, _mode vars set below
void SetLayerLights(void){
	//set LED per layer here

	//to set color
	//rgblight_sethsv_noeeprom(HSV_PURPLE);

	//to set individual LED Colors at specific positions
	//rgblight_sethsv_at(HSV_YELLOW, 0);
	//rgblight_sethsv_at(HSV_PURPLE, 1);
	//rgblight_sethsv_at(HSV_PURPLE, 2);
	//rgblight_sethsv_at(HSV_YELLOW, 3);

	//for custom shade
	//rgblight_sethsv_noeeprom(235,220,185);//magenta

	
	if (toggle_mute_all_enabled){
		rgblight_mode_noeeprom(23);//Red knight rider mode to show mute state
	}
	else{
		rgblight_mode_noeeprom(3);// else breathing
	}
	//SET CUSTOM LIGHTING LAYER LOGIC HERE
	if (sfx1_mode){
		rgblight_sethsv_noeeprom(HSV_BLUE);
	} else
	if (sfx2_mode){
		
        rgblight_sethsv_noeeprom(HSV_CYAN);
	}else 
	if(sfx3_mode){
		
        rgblight_sethsv_noeeprom(HSV_ORANGE);
	}else 
	if (sfx4_mode){
		
		rgblight_sethsv_noeeprom(235,220,185);//magenta
	}else 
	if (stream_mode){
		UpdateLightsForKnobPosition();
	}
}
uint32_t SetLayerLogic(uint32_t state){
// For use without QMK's state object and for your own encoder and rgb evaluation
// perform custom functionality using these vars elsewhere such as in SetLayerLights(void) or in a custom keycode in process_record_user(uint16_t keycode, keyrecord_t *record)
    sfx1_mode = false;
    sfx2_mode = false;
    sfx3_mode = false;
    sfx4_mode = false;
    stream_mode = false;
	//avoid customizing logic here, SetLayerLights(void) is the best place to extend custom functionality
    switch (biton32(state)) {
		case _SFX2:
			sfx2_mode = true; 	
			break;
		case _SFX3:
			sfx3_mode = true;
			break;
		case _SFX4:
			sfx4_mode = true;
			break;
		case _STREAM:
			stream_mode = true;
			break;
		default: //For first layer, add further layers above this
			sfx1_mode = true;
			break;
	}
	SetLayerLights();
    return state;
}
//Function called By QMK on state change, extended above for use elsewhere
uint32_t layer_state_set_user(uint32_t state) {
	SetLayerLogic(state);
	return state;
}
/*
==============================
MACROS
==============================
*/
//Macro Declarations
enum {
	QMK_BEST = SAFE_RANGE,
	Shift_1,
	Shift_2,
	Shift_3,
	Shift_4,
	Shift_5,
	Shift_6,
	Shift_7,
	Shift_8,
	Shift_9,
	Shift_0,
	Shift_Equal,
	Shift_Minus,
	Shift_F1,
	Shift_F2,
	Shift_F3,
	Shift_F4,
	Shift_F5,
	Shift_F6,
	Shift_F7,
	Shift_F8,
	Shift_F9,
	Shift_F10,
	Shift_F11,
	Shift_F12,
	StopSounds,
	Stream_1,
	Stream_2,
	Stream_3,
	Stream_4,
	Stream_5,
	Stream_6,
	Stream_Top,
	Mute_All
	// Other declarations would go here, separated by commas, if you have them
};
//Macro Functions
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode){
		case Shift_1: 
				if(record->event.pressed){
					//keydown event
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_1);
				} else {
					//keyup event
					unregister_code(KC_1);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}		
			return false;
        case Shift_2:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_2);
				} else {
					unregister_code(KC_2);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_3:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_3);
					
				} else {
					unregister_code(KC_3);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_4:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_4);
				} else {
					unregister_code(KC_4);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_5:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_5);
				} else {
					unregister_code(KC_5);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_6:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_6);
				} else {
					unregister_code(KC_6);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_7:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_7);

				} else {
					unregister_code(KC_7);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_8:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_8);
				} else {
					unregister_code(KC_8);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_9:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_9);
				} else {
					unregister_code(KC_9);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_0:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_0);
				} else {
					unregister_code(KC_0);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_Minus:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_MINUS);
				} else {
					unregister_code(KC_MINUS);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_Equal:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_EQUAL);
				} else {
					unregister_code(KC_EQUAL);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F1:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F1);

				} else {
					unregister_code(KC_F1);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F2:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F2);
				} else {
					unregister_code(KC_F2);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F3:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F3);
				} else {
					unregister_code(KC_F3);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F4:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F4);
				} else {
					unregister_code(KC_F4);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F5:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F5);
				} else {
					unregister_code(KC_F5);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F6:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F6);
				} else {
					unregister_code(KC_F6);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F7:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F7);
				} else {
					unregister_code(KC_F7);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F8:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F8);
				} else {
					unregister_code(KC_F8);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F9:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F9);
				} else {
					unregister_code(KC_F9);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F10:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F10);
				} else {
					unregister_code(KC_F10);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F11:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F11);
				} else {
					unregister_code(KC_F11);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Shift_F12:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F12);
				} else {
					unregister_code(KC_F12);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case StopSounds:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_PSCREEN);
				} else {
					unregister_code(KC_PSCREEN);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		//streaming macros	
		case Stream_1:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F13);
				} else {
					unregister_code(KC_F13);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_2:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F14);
				} else {
					unregister_code(KC_F14);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_3:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F15);
				} else {
					unregister_code(KC_F15);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_4:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F16);
				} else {
					unregister_code(KC_F16);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_5:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F17);
				} else {
					unregister_code(KC_F17);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_6:        
				if(record->event.pressed){
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F18);
				} else {
					unregister_code(KC_F18);
					unregister_code(KC_LSFT);
					unregister_code(KC_LALT);
					unregister_code(KC_LCTRL);
				}
			return false;
		case Stream_Top:        
			if(record->event.pressed){
				register_code(KC_LCTRL);
				register_code(KC_LALT);
				register_code(KC_LSFT);
				register_code(KC_F19);
			} else {
				unregister_code(KC_F19);
				unregister_code(KC_LSFT);
				unregister_code(KC_LALT);
				unregister_code(KC_LCTRL);
			}
			return false;
		case Mute_All: 
			if(record->event.pressed){	
				//keydown
				if(!toggle_mute_all_enabled){
					//perform mute if button pressed and not already enabled
					//Mute Virtual Mic
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F22);
				} else {
					//perform unmute if button pressed and mute enabled
					//Unmute Virtual Mic
					register_code(KC_LCTRL);
					register_code(KC_LALT);
					register_code(KC_LSFT);
					register_code(KC_F23);
				}
			}
			else{
				//key up
				if(!toggle_mute_all_enabled){
					//Finsihing Mute, unregister the mute buttons
					toggle_mute_all_enabled = true;
					//keyup for Virtual Mic Mute
					unregister_code(KC_F22);
					unregister_code(KC_LCTRL);
					unregister_code(KC_LALT);
					unregister_code(KC_LSFT);
					rgblight_mode_noeeprom(23);//Red knight rider mode to show mute
					rgblight_sethsv_noeeprom(HSV_RED);
				}else{
					//Finsihing Unute, unregister the unmute buttons
					toggle_mute_all_enabled = false;
					//keyup for Virtual Mic Unmute
					unregister_code(KC_F23);
					unregister_code(KC_LCTRL);
					unregister_code(KC_LALT);
					unregister_code(KC_LSFT);
					//return to correct layer and knob color
					SetLayerLights();
				}
			}
		return false;
    }
    return true;
}
/*
==============================
KEYMAPS
==============================
*/
//Press of second encoder toggles stream layer on and off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_SFX1] = LAYOUT(
        Mute_All, StopSounds, TG(_STREAM),
        Shift_1, Shift_2 , Shift_3,
        Shift_4, Shift_5, Shift_6
    ),
    [_SFX2] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS,
        Shift_7, Shift_8 , Shift_9,
        Shift_0, Shift_Minus, Shift_Equal
    ),
	[_SFX3] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS,
		Shift_F1, Shift_F2 , Shift_F3,
		Shift_F4, Shift_F5, Shift_F6
    ),
	[_SFX4] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS,
		Shift_F7, Shift_F8, Shift_F9,
		Shift_F10, Shift_F11, Shift_F12
    ),
	[_STREAM] = LAYOUT(
		Mute_All, Stream_Top, TG(_STREAM),
		Stream_1, Stream_2, Stream_3,
		Stream_4, Stream_5, Stream_6
    )
};
/*
==============================
ENCODERS
==============================
*/
void encoder_update_user(uint8_t index, bool clockwise) {
	//clockwise is backwards... idk why, don't @ me
	clockwise = !clockwise;
	
	
	switch (index){
		case 0:
		//First Encoder
			switch(biton32(layer_state)){
					//case is _layernames from the enum my_layers
					//open for feature layers 1-4
					//without a break, SFX_1-3 conditions flow to SFX_4
					case _SFX1:
					case _SFX2:
					case _SFX3:
					case _SFX4:
						if (clockwise){
							//print("RGB BRIGHT UP\n");
							//for all SFX layers, increase/ decrease brightness
							rgblight_increase_val();
						}
						else{
							//print("RGB BRIGHT DWN\n");
							rgblight_decrease_val();
						}
						break;
					case _STREAM:
					//toggle between sending audio to Only Stream, Both, or Discord
						if (clockwise) {
							switch(selected_knob_pos){
								case 2:
									//right position
									//overscroll function available
									break;
								case 1:
									//middle position
									//print("Discord Only\n");
									selected_knob_pos = 2;

									//OBS Mute C,A,S+F20
									register_code(KC_LCTRL);
									register_code(KC_LALT);
									register_code(KC_LSFT);
									register_code(KC_F20);//mute obs
									unregister_code(KC_F20);
									unregister_code(KC_LSFT);
									unregister_code(KC_LALT);
									unregister_code(KC_LCTRL);
									
									break;
								case 0:
									//left position
									//print("Both On\n");
									selected_knob_pos = 1;

									//toggle mute discord
									tap_code(KC_PGDN);
									
									break;
							}
							UpdateLightsForKnobPosition();
						}
						else {
							//Counter Clockwise Turn
							switch(selected_knob_pos){
								case 2:
									//print("Both On\n");
									selected_knob_pos = 1;

									//OBS Unmute C+A+S+F21
									register_code(KC_LCTRL);
									register_code(KC_LALT);
									register_code(KC_LSFT);
									register_code(KC_F21);//OBS umute
									unregister_code(KC_F21);
									unregister_code(KC_LSFT);
									unregister_code(KC_LALT);
									unregister_code(KC_LCTRL);
									break;
								case 1:
									//print("Stream Only\n");
									selected_knob_pos = 0;

									//mute discord
									tap_code(KC_PGDN);
									break;
								case 0:
									//Overscroll func availalbe
									break;
							}
							UpdateLightsForKnobPosition();
							break;
						}
				} 
				break;
		case 1:
		//second encoder
			switch(biton32(layer_state)){
				//switch layers back and forth, also has "overscroll function" if moving CCW on layer 0, decrease brightness, and vice versa when moving CW on layer 4
				//case is layer names defined in enum my_layers
				case _SFX1:
					if (clockwise) {
						selected_layer++;
						//sets layer
						layer_clear();
						layer_on(selected_layer);
						break;
					} else {
						//Overscroll function below bottom option
						backlight_decrease();
						break;
					}
				case _SFX2:
					if (clockwise) {
						selected_layer++;
					} else {
						selected_layer--;
					}
					layer_clear();
					layer_on(selected_layer);
					break;
				case _SFX3:
					if (clockwise) {
						selected_layer++;
					} else {
						selected_layer--;
					}
					layer_clear();
					layer_on(selected_layer);
					break;
				case _SFX4:
					if (clockwise) {
						//Overscroll function above top option
						backlight_increase();
						break;
					} else {
						selected_layer--;
					}
					layer_clear();
					layer_on(selected_layer);
					break;
					
				case _STREAM:
					if (clockwise) {
						tap_code(KC_VOLU);
					} else {
						tap_code(KC_VOLD);
					}
				break;
			}
	}
}
//if you somehow read this far, and are confused about something, feel free to reach out to me on discord at benjelum#9007 (case sensitive) happy coding!