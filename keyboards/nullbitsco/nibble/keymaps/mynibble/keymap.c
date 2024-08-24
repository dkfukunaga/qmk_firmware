/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layer_names {
  _MA,
  _F1,
  _F2
};

enum custom_keycodes {
  LI_COMM = SAFE_RANGE,		//comma only
  LI_DKLF,					//previous virtual desktop
  LI_DKRT,					//next virtual desktop
  LI_TSKV					//task view
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MA] = LAYOUT_ansi(
             KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
    KC_MUTE, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PSCR,
    MO(_F2), KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_HOME, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    KC_END,  KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    TT(_F1), KC_RALT, KC_RCTL, KC_LEFT,          KC_DOWN, KC_RGHT
  ),
  [_F1] = LAYOUT_ansi(
             KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_BSPC, KC_INS,
    XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PMNS, KC_LPRN, KC_RPRN, KC_TAB,  KC_PAUS,
    _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PEQL,          KC_PENT, KC_SCRL,
    KC_BRIU, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, LI_COMM, KC_P1,   KC_P2,   KC_P3,   KC_PDOT, _______,          XXXXXXX, XXXXXXX,
    KC_BRID, _______, GU_TOGG, _______,                   KC_P0,                     _______, _______, _______, LI_DKLF,          LI_TSKV, LI_DKRT
  ),
  [_F2] = LAYOUT_ansi(
            QK_BOOT,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, _______,                   _______,                   _______, _______, _______, _______,          _______, _______
  ),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Send keystrokes to host keyboard, if connected (see readme)
  process_record_remote_kb(keycode, record);
  switch(keycode) {
    case LI_COMM: //comma only for keypad
      if (record->event.pressed) {
		if (get_mods() & MOD_BIT(KC_LSFT) & MOD_BIT(KC_RSFT)) {
		  //ignore shift and do nothing
		} else {
		  tap_code(KC_COMM);
		}
      }
    break;

    case LI_DKLF: //Windows desktop left
      if (record->event.pressed) {
		register_code(KC_LWIN);
		register_code(KC_LCTL);
		tap_code(KC_LEFT);
      } else {
		unregister_code(KC_LCTL);
		unregister_code(KC_LWIN);
	  }
    break;

    case LI_DKRT: //rWindows desktop right
      if (record->event.pressed) {
		register_code(KC_LWIN);
		register_code(KC_LCTL);
		tap_code(KC_RIGHT);
      } else {
		unregister_code(KC_LCTL);
		unregister_code(KC_LWIN);
	  }
    break;

    case LI_TSKV: //Windows Task View
      if (record->event.pressed) {
		register_code(KC_LWIN);
		tap_code(KC_TAB);
      } else {
		unregister_code(KC_LWIN);
	  }
    break;

  }
return true;
}

// RGB config, for changing RGB settings on non-VIA firmwares
void change_RGB(bool clockwise) {
  bool shift = get_mods() & MOD_MASK_SHIFT;
  bool alt = get_mods() & MOD_MASK_ALT;
  bool ctrl = get_mods() & MOD_MASK_CTRL;

  if (clockwise) {
    if (alt) {
      rgblight_increase_hue();
    } else if (ctrl) {
      rgblight_increase_val();
    } else if (shift) {
      rgblight_increase_sat();
    } else {
      rgblight_step();
    }

  } else {
    if (alt) {
      rgblight_decrease_hue();
    } else if (ctrl) {
      rgblight_decrease_val();
    } else if (shift) {
      rgblight_decrease_sat();
    } else {
      rgblight_step_reverse();
    }
  }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (layer_state_is(_F2)) {
    //change RGB settings
    change_RGB(clockwise);
  } else if (layer_state_is(_F1)) {
    if (clockwise) {
      tap_code16(C(KC_Y));
    } else {
      tap_code16(C(KC_Z));
    }
  } else {
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
  }
  return true;
}

// Light all LEDs red when caps lock is active
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 10, HSV_RED}      // Light all LEDs cyan
);

// Light all LEDs cyan when _F1 is active
const rgblight_segment_t PROGMEM my_fn1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 10, HSV_CYAN}      // Light all LEDs cyan
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
  my_capslock_layer,
  my_fn1_layer           // Overrides caps lock layer
);

void keyboard_post_init_user(void) {
  // Enable the LED layers
  rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
  rgblight_set_layer_state(0, led_state.caps_lock);
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_set_layer_state(1, layer_state_cmp(state, _F1));
  return state;
}

void matrix_init_user(void) {
  // Initialize remote keyboard, if connected (see readme)
  matrix_init_remote_kb();
}

void matrix_scan_user(void) {
  // Scan and parse keystrokes from remote keyboard, if connected (see readme)
  matrix_scan_remote_kb();
}
