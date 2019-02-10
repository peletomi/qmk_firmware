#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"

enum {
  BASE,
  SYMB,
  MDIA,
  HUN
};

enum {
  NONE = 0,
  // Buttons that do extra stuff
  A_GUI,
  A_PLVR,
  A_ESC,
  A_MPN,

  // Function / number keys
  KF_1, // 1, F1
  KF_2, // 2, F2
  KF_3, // ...
  KF_4,
  KF_5,
  KF_6,
  KF_7,
  KF_8,
  KF_9,
  KF_10,
  KF_11,
  KF_12,  // =, F12
  KF_PRN,
};

/* States & timers */
uint16_t kf_timers[14];

// make ergodox_ez:colemak_teppel
// https://docs.qmk.fm
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,---------------------------------------------------------           ,-----------------------------------------------------------.
 * |  Esc ` | ! F1  |  @ F2  | # F3  | $ F4  | % F5  |  F6   |          | ^ F7  | ~ F8  |  * F9  | = F10  | - F11  | & F12 |        |
 * |--------+-------+--------+-------+-------+-------+-------|          |-------+-------+--------+--------+--------+-------+--------|
 * |        |   Q   |    W   |   F   |   P   |   G   |  Tab  |          |  ( )  |   J   |   L    |   U    |   Y    |   ;   |   \    |
 * |--------+-------+--------+-------+-------+-------|       |          |       |-------+--------+--------+--------+-------+--------|
 * |        |   A   |    R   | S CA  | T CS  |   D   |-------|          |-------|   H   |   N    |   E    |   I    | O L2  |   '    |
 * |--------+-------+--------+-------+-------+----- -|  [    |          |  ]    |-------+--------+--------+--------+-------+--------|
 * | MShift | Z Ctrl|    X   |   C   |   V   |   B   |       |          |       |   K   |   M    |   ,    |   .    |/ Ctrl | )Shift |
 * `--------+-------+--------+-------+-------+-------+-------'          `--------------+---------+--------+--------+-------+--------'
 *   |  Ins |  Home |  PgUp  |  PgDn |  End |                                            | Left  | Down   |  Up    | Right | ~L1  |
 *   `--------------------------------------'                                            `----------------------------------------'
 *                                             ,-------------.          ,----------------.
 *                                             |      | Del  |          |   L1  |   L2   |
 *                                     ,-------|------|------|          |-------+--------+-------------.
 *                                     |       |      |Gui ` |          | Gui \ |        |             |
 *                                     |  Ctrl | Alt  |------|          |-------| Space  | Enter Shift |
 *                                     |       |      | BkSp |          | Esc ` |        |             |
 *                                     `---------------------'          `------------------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_GESC,        M(KF_1),   M(KF_2),  M(KF_3), M(KF_4),  M(KF_5), KC_F6,
        KC_TRNS,        KC_Q,         KC_W,     KC_F,    KC_P,    KC_G,   KC_TAB, // KC_LPRN, KC_RPRN
        KC_TRNS,        KC_A,         KC_R,      MT(MOD_LCTL | MOD_LALT, KC_S),  MT(MOD_LCTL | MOD_LSFT, KC_T),    KC_D,
        OSM(MOD_LSFT),  CTL_T(KC_Z),        KC_X,     KC_C,    KC_V,    KC_B,   KC_LBRC,
        KC_INS,         KC_HOME,      KC_PGUP,  KC_PGDN, KC_END,
                                               KC_TRNS, KC_DEL,
                                                              GUI_T(KC_GRV),
                                               KC_LCTL,KC_LALT,KC_BSPC,
        // right hand
             M(KF_7),     M(KF_8),M(KF_9), M(KF_10), M(KF_11), M(KF_12),         KC_TRNS, // KC_AMPR, KC_LPRN, KC_RPRN
             M(KF_PRN),   KC_J,   KC_L,    KC_U,    KC_Y,     KC_SCLN,          KC_BSLS,
                          KC_H,   KC_N,    KC_E,    KC_I,     LT(MDIA, KC_O),   KC_QUOT,
             KC_RBRC,     KC_K,   KC_M,    KC_COMM, KC_DOT,   CTL_T(KC_SLSH),   KC_RSPC,
                                  KC_LEFT, KC_DOWN, KC_UP,    KC_RGHT,          KC_FN1,
             TG(SYMB), TG(MDIA),
             GUI_T(KC_BSLS),
             KC_GESC,KC_SPC, KC_SFTENT
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 | F11  | F12  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |   0  |      |   .  |   =  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,
       KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_TRNS,
       KC_TRNS,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS,
       KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
                KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS,
       KC_TRNS, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, KC_TRNS,
                         KC_0,   KC_TRNS, KC_DOT,  KC_EQL,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           | Mute |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |VolUp |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      | Prev | Stop | Play | Next |        |
 * |--------+------+------+------+------+------|      |           |VolDn |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      | Lclk | Mclk | Rclk |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |MsLeft|MsDown| MsUp |MsRght|      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = LAYOUT_ergodox(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
       // right hand
       KC_MUTE,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_VOLU,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_MPRV, KC_MSTP, KC_MPLY, KC_MNXT, KC_TRNS,
       KC_VOLD,  KC_TRNS, KC_BTN1, KC_BTN3, KC_BTN2, KC_TRNS, KC_TRNS,
                          KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
[HUN] = LAYOUT_ergodox(  // Hungarian
        // left hand
        KC_ESC,         KC_TRNS,      M(KF_2),  M(KF_3), M(KF_4), KC_DLR, KC_PERC,
        KC_TRNS,        KC_Q,         KC_W,     KC_F,    KC_P,    KC_G,   KC_TAB, // KC_DELT
        KC_TRNS,        KC_A,         KC_R,     KC_S,    KC_T,    KC_D,
        OSM(MOD_LSFT),  CTL_T(KC_Z),  KC_X,     KC_C,    KC_V,    KC_B,   KC_LPRN,
        KC_INS,         KC_HOME,      KC_PGUP,  KC_PGDN, KC_END,
                                                     KC_LBRC, TG(SYMB),
                                                              TG(MDIA),
                                               KC_LCTL,KC_LALT,KC_BSPC,
        // right hand
             KC_CIRC,     KC_TILD,M(KF_5), M(KF_6), M(KF_10), KC_AMPR,          KC_TRNS, // KC_AMPR, KC_LPRN, KC_RPRN
             KC_TRNS,     KC_J,   KC_L,    KC_U,    KC_Y,     KC_SCLN,          KC_BSLS,
                          KC_H,   KC_N,    KC_E,    KC_I,     LT(MDIA, KC_O),   KC_QUOT,
             KC_RPRN,     KC_K,   KC_M,    KC_COMM, KC_DOT,   CTL_T(KC_SLSH),   KC_RSFT,
                                  KC_LEFT, KC_DOWN, KC_UP,    KC_RGHT,          KC_FN1,
             TG(SYMB), KC_RBRC,
             GUI_T(KC_BSLS),
             KC_ESC,KC_SPC, KC_ENT
    ),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

void shift (uint16_t kc) {
  register_code (KC_LSFT);
  register_code (kc);
  unregister_code (kc);
  unregister_code (KC_LSFT);
}
void ang_handle_kf (keyrecord_t *record, uint8_t id)
{
  uint8_t code = id - KF_1;

  if (record->event.pressed) {
    kf_timers[code] = timer_read ();
  } else {
    if (timer_elapsed (kf_timers[code]) > TAPPING_TERM) { // Long press
      if (id == KF_PRN) {
        shift(KC_0); // KC_LPRN
      } else {
        uint8_t kc = KC_F1 + code;
        register_code (kc); unregister_code (kc);
      }
    } else { // Short press
      switch(id) {
        case KF_1: shift(KC_EXLM); break;
        case KF_2: shift(KC_AT); break;
        case KF_3: shift(KC_HASH); break;
        case KF_4: shift(KC_DLR); break;
        case KF_5: shift(KC_PERC); break;
        case KF_7: shift(KC_CIRC); break;
        case KF_8: shift(KC_TILD); break;
        case KF_9: shift(KC_CIRC); break;
        case KF_10: register_code(KC_EQL); unregister_code(KC_EQL); break;
        case KF_11: register_code(KC_MINS); unregister_code(KC_MINS); break;
        case KF_12: shift(KC_AMPR); break;
        // case KF_12: shift(KC_7); break;
        // case KF_PRN: shift(KC_9); break;
        default: register_code (KC_TRNS); unregister_code (KC_TRNS); break;
      }
    }
  }
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          register_code(KC_RSFT);
        } else {
          unregister_code(KC_RSFT);
        }
        break;
        /* Function keys */
        case KF_1 ... KF_PRN:
        ang_handle_kf (record, id);
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

};
