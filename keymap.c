#include QMK_KEYBOARD_H
#include "../../../../quantum/keymap_extras/keymap_spanish.h"

enum tap_dance_codes {
    DOT_DANCE,
    COMMA_DANCE,
    ACCENTS_DANCE,
    QUOTES,
    EXCLAMATION,
    QUESTION,
    DANCE_54,
    DANCE_55,
    DANCE_56,
    DANCE_57,
    DANCE_58,
    DANCE_59,
    DANCE_60,
    DANCE_63,
    PLUS_HME,
    BKSL_END,
    NTIL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
      //---------------------------------------------------------------------     ---------------------------------------------------------------------------
      ES_Q, ES_W              , ES_F              , ES_P              , ES_B,     ES_J, ES_L             , ES_U           , ES_Y         , ES_NTIL          ,
      //---------------------------------------------------------------------     ---------------------------------------------------------------------------
      ES_A, KC_R              , KC_S              , KC_T              , KC_G,     KC_M, ES_N             , KC_E           , KC_I         , ES_O             ,
      //---------------------------------------------------------------------     ---------------------------------------------------------------------------
      ES_Z, MT(MOD_RCTL, ES_X), MT(MOD_LALT, ES_C), MT(MOD_LGUI, ES_D), ES_V,     ES_K, MT(MOD_LGUI,ES_H), TD(COMMA_DANCE), TD(DOT_DANCE), TD(ACCENTS_DANCE),
      //---------------------------------------------------------------------     ---------------------------------------------------------------------------
                                              LT(2, KC_SPACE), OSM(MOD_LSFT),     LT(3, KC_ENTER), LT(1, KC_BSPC)
  ),

  [1] = LAYOUT(
      //-----------------------------------------------------------------------       --------------------------------------------------------------------------
      KC_ESCAPE      , ES_AT       , ES_HASH      , ES_DLR        , ES_PERC   ,       ES_AMPR     , ES_LPRN      , ES_RPRN      , ES_SLSH      , KC_BSPC       ,
      //-----------------------------------------------------------------------       --------------------------------------------------------------------------
      KC_TAB         , ES_MINS     ,  TD(QUESTION), TD(EXCLAMATION), TD(QUOTES),      TD(PLUS_HME), ES_LBRC      , ES_RBRC      , TD(BKSL_END) , KC_ENTER      ,
      //-----------------------------------------------------------------------       --------------------------------------------------------------------------
      OSM(MOD_LSFT)  , ES_CIRC     , ES_LABK      ,  ES_RABK      , ES_TILD   ,       ES_EQL      , ES_LCBR      , ES_RCBR      , OSM(MOD_RCTL), KC_NO         ,
      //-----------------------------------------------------------------------       --------------------------------------------------------------------------
                                                     KC_SPACE     , KC_LGUI   ,       KC_RALT    , KC_NO
  ),

  [2] = LAYOUT(
      //--------------------------------------------------------------------------------       ------------------------------------
      KC_ESCAPE, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, ES_ASTR,       ES_PLUS, KC_7, KC_8, KC_9, KC_BSPC ,
      //--------------------------------------------------------------------------------       ------------------------------------
      KC_TAB   , KC_PSCR            , KC_AUDIO_VOL_DOWN  , KC_AUDIO_VOL_UP    , ES_SLSH,       ES_MINS , KC_4, KC_5, KC_6, KC_ENTER,
      //--------------------------------------------------------------------------------       ------------------------------------
      QK_BOOT  , KC_KB_MUTE         , KC_BRIGHTNESS_DOWN , KC_BRIGHTNESS_UP   , KC_PDOT,       KC_0   , KC_1, KC_2, KC_3, ES_EQL   ,
      //--------------------------------------------------------------------------------       ------------------------------------
                                                           KC_NO              , KC_LGUI,       KC_RALT, KC_ENTER
  ),

  [3] = LAYOUT(
      //---------------------------------------------      -----------------------------------------------------------------------
      KC_F1, KC_F2 , KC_F3 , KC_F4   , LSFT(KC_PGUP),      LSFT(KC_PGDN) , LCTL(LSFT(KC_TAB)), RCTL(KC_TAB)  , KC_NO   , KC_BSPC ,
      //---------------------------------------------      -----------------------------------------------------------------------
      KC_F5, KC_F6 , KC_F7 , KC_F8   , KC_DEL       ,      KC_LEFT       , KC_DOWN           , KC_UP         , KC_RIGHT, KC_ENTER,
      //---------------------------------------------      -----------------------------------------------------------------------
      KC_F9, KC_F10, KC_F11, KC_F12  , LSFT(KC_HOME),      LSFT(KC_END)  , LALT(KC_TAB)      , LALT(KC_GRAVE), KC_NO   , KC_NO   ,
      //---------------------------------------------      -----------------------------------------------------------------------
                             KC_SPACE, KC_LGUI      ,      KC_NO         , KC_NO
  )
};

extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode  = KC_NO;
uint8_t  last_modifier = 0;

typedef struct {
    bool    is_press_action;
    uint8_t step;
} tap;

enum { SINGLE_TAP = 1, SINGLE_HOLD, DOUBLE_TAP, DOUBLE_HOLD, DOUBLE_SINGLE_TAP, MORE_TAPS };

static tap dance_state = {.is_press_action = true, .step = 0};

void    on_comma_dance(tap_dance_state_t *state, void *user_data);
uint8_t comma_dance_dance_step(tap_dance_state_t *state);
void    comma_dance_finished(tap_dance_state_t *state, void *user_data);
void    comma_dance_reset(tap_dance_state_t *state, void *user_data);

void on_comma_dance(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(ES_COMM);
        tap_code16(ES_COMM);
        tap_code16(ES_COMM);
    }
    if (state->count > 3) {
        tap_code16(ES_COMM);
    }
}

uint8_t comma_dance_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void comma_dance_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = comma_dance_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_COMM);
            break;
        case SINGLE_HOLD:
            register_code16(KC_LALT);
            break;
        case DOUBLE_TAP:
            register_code16(ES_MINS);
            break;
    }
}

void comma_dance_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(ES_COMM);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_LALT);
            break;
        case DOUBLE_TAP:
            unregister_code16(ES_MINS);
            break;
    }
    dance_state.step = 0;
}
void    on_accents_dance(tap_dance_state_t *state, void *user_data);
uint8_t accents_dance_dance_step(tap_dance_state_t *state);
void    accents_dance_finished(tap_dance_state_t *state, void *user_data);
void    accents_dance_reset(tap_dance_state_t *state, void *user_data);

void on_accents_dance(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(ES_GRV);
        tap_code16(ES_GRV);
        tap_code16(ES_GRV);
    }
    if (state->count > 3) {
        tap_code16(ES_ACUT);
    }
}

uint8_t accents_dance_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void accents_dance_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = accents_dance_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_ACUT);
            break;
        case SINGLE_HOLD:
            register_code16(ES_CCED);
            break;
        case DOUBLE_TAP:
            register_code16(ES_GRV);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(ES_ACUT);
            register_code16(ES_ACUT);
    }
}

void accents_dance_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(ES_ACUT);
            break;
        case SINGLE_HOLD:
            unregister_code16(ES_CCED);
            break;
        case DOUBLE_TAP:
            unregister_code16(ES_GRV);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(ES_ACUT);
            break;
    }
    dance_state.step = 0;
}

void    on_dot_dance(tap_dance_state_t *state, void *user_data);
uint8_t dot_dance_dance_step(tap_dance_state_t *state);
void    dot_dance_finished(tap_dance_state_t *state, void *user_data);
void    dot_dance_reset(tap_dance_state_t *state, void *user_data);

void on_dot_dance(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
    }
    if (state->count > 3) {
        tap_code16(KC_DOT);
    }
}

uint8_t dot_dance_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dot_dance_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dot_dance_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_DOT);
            break;
        case SINGLE_HOLD:
            register_code16(KC_RCTL);
            break;
        case DOUBLE_TAP:
            register_code16(KC_DOT);
            register_code16(KC_DOT);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_DOT);
            register_code16(KC_DOT);
    }
}

void dot_dance_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_DOT);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_RCTL);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_DOT);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_DOT);
            break;
    }
    dance_state.step = 0;
}
void    on_dot_dance5(tap_dance_state_t *state, void *user_data);
uint8_t dot_dance5_dance_step(tap_dance_state_t *state);
void    dot_dance5_finished(tap_dance_state_t *state, void *user_data);
void    dot_dance5_reset(tap_dance_state_t *state, void *user_data);

void on_dot_dance5(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
    }
    if (state->count > 3) {
        tap_code16(KC_SPACE);
    }
}

uint8_t dot_dance5_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dot_dance5_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dot_dance5_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_SPACE);
            break;
        case SINGLE_HOLD:
            register_code16(KC_UNDS);
            break;
        case DOUBLE_TAP:
            register_code16(KC_SPACE);
            register_code16(KC_SPACE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_SPACE);
            register_code16(KC_SPACE);
    }
}

void dot_dance5_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_SPACE);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_UNDS);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_SPACE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_SPACE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_31(tap_dance_state_t *state, void *user_data);
uint8_t dance_31_dance_step(tap_dance_state_t *state);
void    dance_31_finished(tap_dance_state_t *state, void *user_data);
void    dance_31_reset(tap_dance_state_t *state, void *user_data);

void on_dance_31(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(ES_DLR);
        tap_code16(ES_DLR);
        tap_code16(ES_DLR);
    }
    if (state->count > 3) {
        tap_code16(ES_DLR);
    }
}

uint8_t dance_31_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_31_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_31_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_DLR);
            break;
        case SINGLE_HOLD:
            register_code16(KC_PSCR);
            break;
        case DOUBLE_TAP:
            register_code16(ES_DLR);
            register_code16(ES_DLR);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(ES_DLR);
            register_code16(ES_DLR);
    }
}

void dance_31_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(LSFT(KC_4));
            break;
        case SINGLE_HOLD:
            unregister_code16(LCTL(LGUI(LSFT(KC_4))));
            break;
        case DOUBLE_TAP:
            unregister_code16(LSFT(KC_4));
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(LSFT(KC_4));
            break;
    }
    dance_state.step = 0;
}
void    on_dance_32(tap_dance_state_t *state, void *user_data);
uint8_t dance_32_dance_step(tap_dance_state_t *state);
void    dance_32_finished(tap_dance_state_t *state, void *user_data);
void    dance_32_reset(tap_dance_state_t *state, void *user_data);

void on_dance_32(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_QUOTE);
        tap_code16(KC_QUOTE);
        tap_code16(KC_QUOTE);
    }
    if (state->count > 3) {
        tap_code16(KC_QUOTE);
    }
}

uint8_t dance_32_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_32_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_32_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_QUOTE);
            break;
        case SINGLE_HOLD:
            register_code16(LGUI(KC_QUOTE));
            break;
        case DOUBLE_TAP:
            register_code16(KC_GRAVE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_QUOTE);
            register_code16(KC_QUOTE);
    }
}

void dance_32_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_QUOTE);
            break;
        case SINGLE_HOLD:
            unregister_code16(LGUI(KC_QUOTE));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_GRAVE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_QUOTE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_33(tap_dance_state_t *state, void *user_data);
uint8_t dance_33_dance_step(tap_dance_state_t *state);
void    dance_33_finished(tap_dance_state_t *state, void *user_data);
void    dance_33_reset(tap_dance_state_t *state, void *user_data);

void on_dance_33(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(LALT(KC_3));
        tap_code16(LALT(KC_3));
        tap_code16(LALT(KC_3));
    }
    if (state->count > 3) {
        tap_code16(LALT(KC_3));
    }
}

uint8_t dance_33_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_33_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_33_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(LALT(KC_3));
            break;
        case SINGLE_HOLD:
            register_code16(LALT(LSFT(KC_2)));
            break;
        case DOUBLE_TAP:
            register_code16(LALT(KC_3));
            register_code16(LALT(KC_3));
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(LALT(KC_3));
            register_code16(LALT(KC_3));
    }
}

void dance_33_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(LALT(KC_3));
            break;
        case SINGLE_HOLD:
            unregister_code16(LALT(LSFT(KC_2)));
            break;
        case DOUBLE_TAP:
            unregister_code16(LALT(KC_3));
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(LALT(KC_3));
            break;
    }
    dance_state.step = 0;
}
void    on_dance_54(tap_dance_state_t *state, void *user_data);
uint8_t dance_54_dance_step(tap_dance_state_t *state);
void    dance_54_finished(tap_dance_state_t *state, void *user_data);
void    dance_54_reset(tap_dance_state_t *state, void *user_data);

void on_dance_54(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_ESCAPE);
        tap_code16(KC_ESCAPE);
        tap_code16(KC_ESCAPE);
    }
    if (state->count > 3) {
        tap_code16(KC_ESCAPE);
    }
}

uint8_t dance_54_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_54_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_54_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_ESCAPE);
            break;
        case SINGLE_HOLD:
            register_code16(LALT(LGUI(KC_ESCAPE)));
            break;
        case DOUBLE_TAP:
            register_code16(KC_ESCAPE);
            register_code16(KC_ESCAPE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_ESCAPE);
            register_code16(KC_ESCAPE);
    }
}

void dance_54_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_ESCAPE);
            break;
        case SINGLE_HOLD:
            unregister_code16(LALT(LGUI(KC_ESCAPE)));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_ESCAPE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_ESCAPE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_55(tap_dance_state_t *state, void *user_data);
uint8_t dance_55_dance_step(tap_dance_state_t *state);
void    dance_55_finished(tap_dance_state_t *state, void *user_data);
void    dance_55_reset(tap_dance_state_t *state, void *user_data);

void on_dance_55(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_Q);
        tap_code16(KC_Q);
        tap_code16(KC_Q);
    }
    if (state->count > 3) {
        tap_code16(KC_Q);
    }
}

uint8_t dance_55_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_55_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_55_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_Q);
            break;
        case SINGLE_HOLD:
            register_code16(KC_H);
            break;
        case DOUBLE_TAP:
            register_code16(KC_Q);
            register_code16(KC_Q);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_Q);
            register_code16(KC_Q);
    }
}

void dance_55_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_Q);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_H);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_Q);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_Q);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_56(tap_dance_state_t *state, void *user_data);
uint8_t dance_56_dance_step(tap_dance_state_t *state);
void    dance_56_finished(tap_dance_state_t *state, void *user_data);
void    dance_56_reset(tap_dance_state_t *state, void *user_data);

void on_dance_56(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_TAB);
        tap_code16(KC_TAB);
        tap_code16(KC_TAB);
    }
    if (state->count > 3) {
        tap_code16(KC_TAB);
    }
}

uint8_t dance_56_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_56_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_56_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_TAB);
            break;
        case SINGLE_HOLD:
            register_code16(LGUI(KC_TAB));
            break;
        case DOUBLE_TAP:
            register_code16(KC_TAB);
            register_code16(KC_TAB);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_TAB);
            register_code16(KC_TAB);
    }
}

void dance_56_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_TAB);
            break;
        case SINGLE_HOLD:
            unregister_code16(LGUI(KC_TAB));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_TAB);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_TAB);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_57(tap_dance_state_t *state, void *user_data);
uint8_t dance_57_dance_step(tap_dance_state_t *state);
void    dance_57_finished(tap_dance_state_t *state, void *user_data);
void    dance_57_reset(tap_dance_state_t *state, void *user_data);

void on_dance_57(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_DELETE);
        tap_code16(KC_DELETE);
        tap_code16(KC_DELETE);
    }
    if (state->count > 3) {
        tap_code16(KC_DELETE);
    }
}

uint8_t dance_57_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_57_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_57_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_DELETE);
            break;
        case SINGLE_HOLD:
            register_code16(LCTL(KC_K));
            break;
        case DOUBLE_TAP:
            register_code16(KC_DELETE);
            register_code16(KC_DELETE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_DELETE);
            register_code16(KC_DELETE);
    }
}

void dance_57_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_DELETE);
            break;
        case SINGLE_HOLD:
            unregister_code16(LCTL(KC_K));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_DELETE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_DELETE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_58(tap_dance_state_t *state, void *user_data);
uint8_t dance_58_dance_step(tap_dance_state_t *state);
void    dance_58_finished(tap_dance_state_t *state, void *user_data);
void    dance_58_reset(tap_dance_state_t *state, void *user_data);

void on_dance_58(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_GRAVE);
        tap_code16(KC_GRAVE);
        tap_code16(KC_GRAVE);
    }
    if (state->count > 3) {
        tap_code16(KC_GRAVE);
    }
}

uint8_t dance_58_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_58_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_58_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_GRAVE);
            break;
        case SINGLE_HOLD:
            register_code16(LGUI(KC_GRAVE));
            break;
        case DOUBLE_TAP:
            register_code16(KC_GRAVE);
            register_code16(KC_GRAVE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_GRAVE);
            register_code16(KC_GRAVE);
    }
}

void dance_58_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_GRAVE);
            break;
        case SINGLE_HOLD:
            unregister_code16(LGUI(KC_GRAVE));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_GRAVE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_GRAVE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_59(tap_dance_state_t *state, void *user_data);
uint8_t dance_59_dance_step(tap_dance_state_t *state);
void    dance_59_finished(tap_dance_state_t *state, void *user_data);
void    dance_59_reset(tap_dance_state_t *state, void *user_data);

void on_dance_59(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
    }
    if (state->count > 3) {
        tap_code16(KC_SPACE);
    }
}

uint8_t dance_59_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_59_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_59_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_SPACE);
            break;
        case SINGLE_HOLD:
            register_code16(LGUI(KC_O));
            break;
        case DOUBLE_TAP:
            register_code16(KC_SPACE);
            register_code16(KC_SPACE);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_SPACE);
            register_code16(KC_SPACE);
    }
}

void dance_59_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_SPACE);
            break;
        case SINGLE_HOLD:
            unregister_code16(LGUI(KC_O));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_SPACE);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_SPACE);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_60(tap_dance_state_t *state, void *user_data);
uint8_t dance_60_dance_step(tap_dance_state_t *state);
void    dance_60_finished(tap_dance_state_t *state, void *user_data);
void    dance_60_reset(tap_dance_state_t *state, void *user_data);

void on_dance_60(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_LEFT);
        tap_code16(KC_LEFT);
        tap_code16(KC_LEFT);
    }
    if (state->count > 3) {
        tap_code16(KC_LEFT);
    }
}

uint8_t dance_60_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_60_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_60_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_LEFT);
            break;
        case SINGLE_HOLD:
            register_code16(LALT(KC_LEFT));
            break;
        case DOUBLE_TAP:
            register_code16(KC_LEFT);
            register_code16(KC_LEFT);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_LEFT);
            register_code16(KC_LEFT);
    }
}

void dance_60_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_LEFT);
            break;
        case SINGLE_HOLD:
            unregister_code16(LGUI(KC_LEFT));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_LEFT);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_LEFT);
            break;
    }
    dance_state.step = 0;
}
void    on_dance_63(tap_dance_state_t *state, void *user_data);
uint8_t dance_63_dance_step(tap_dance_state_t *state);
void    dance_63_finished(tap_dance_state_t *state, void *user_data);
void    dance_63_reset(tap_dance_state_t *state, void *user_data);

void on_dance_63(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
    }
    if (state->count > 3) {
        tap_code16(KC_RIGHT);
    }
}

uint8_t dance_63_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void dance_63_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = dance_63_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_RIGHT);
            break;
        case SINGLE_HOLD:
            register_code16(LGUI(KC_RIGHT));
            break;
        case DOUBLE_TAP:
            register_code16(KC_RIGHT);
            register_code16(KC_RIGHT);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_RIGHT);
            register_code16(KC_RIGHT);
    }
}

void dance_63_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_RIGHT);
            break;
        case SINGLE_HOLD:
            unregister_code16(LALT(KC_RIGHT));
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_RIGHT);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_RIGHT);
            break;
    }
    dance_state.step = 0;
}

void    on_plus_hme(tap_dance_state_t *state, void *user_data);
uint8_t plus_hme_dance_step(tap_dance_state_t *state);
void    plus_hme_finished(tap_dance_state_t *state, void *user_data);
void    plus_hme_reset(tap_dance_state_t *state, void *user_data);

void on_plus_hme(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(ES_PLUS);
        tap_code16(ES_PLUS);
        tap_code16(ES_PLUS);
    }
    if (state->count > 3) {
        tap_code16(ES_PLUS);
    }
}

uint8_t plus_hme_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void plus_hme_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = plus_hme_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_PLUS);
            break;
        case SINGLE_HOLD:
            register_code16(KC_HOME);
            break;
        case DOUBLE_TAP:
            register_code16(ES_PLUS);
            register_code16(ES_PLUS);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(ES_PLUS);
            register_code16(ES_PLUS);
    }
}

void plus_hme_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(ES_PLUS);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_HOME);
            break;
        case DOUBLE_TAP:
            unregister_code16(ES_PLUS);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(ES_PLUS);
            break;
    }
    dance_state.step = 0;
}

void    on_bksl_end(tap_dance_state_t *state, void *user_data);
uint8_t bksl_end_dance_step(tap_dance_state_t *state);
void    bksl_end_finished(tap_dance_state_t *state, void *user_data);
void    bksl_end_reset(tap_dance_state_t *state, void *user_data);

void on_bksl_end(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(ES_BSLS);
        tap_code16(ES_BSLS);
        tap_code16(ES_BSLS);
    }
    if (state->count > 3) {
        tap_code16(ES_BSLS);
    }
}

uint8_t bksl_end_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void bksl_end_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = bksl_end_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_BSLS);
            break;
        case SINGLE_HOLD:
            register_code16(KC_END);
            break;
        case DOUBLE_TAP:
            register_code16(ES_BSLS);
            register_code16(ES_BSLS);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(ES_BSLS);
            register_code16(ES_BSLS);
    }
}

void bksl_end_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(ES_BSLS);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_END);
            break;
        case DOUBLE_TAP:
            unregister_code16(ES_BSLS);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(ES_BSLS);
            break;
    }
    dance_state.step = 0;
}

void    on_ntil_end(tap_dance_state_t *state, void *user_data);
uint8_t ntil_end_dance_step(tap_dance_state_t *state);
void    ntil_end_finished(tap_dance_state_t *state, void *user_data);
void    ntil_end_reset(tap_dance_state_t *state, void *user_data);

void on_ntil_end(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_N);
        tap_code16(KC_N);
        tap_code16(KC_N);
    }
    if (state->count > 3) {
        tap_code16(KC_N);
    }
}

uint8_t ntil_end_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void ntil_end_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = ntil_end_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(ES_N);
            break;
        case SINGLE_HOLD:
            register_code16(KC_RGUI);
            break;
        case DOUBLE_TAP:
            register_code16(ES_NTIL);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(ES_N);
            register_code16(ES_N);
    }
}

void ntil_end_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(ES_N);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_RGUI);
            break;
        case DOUBLE_TAP:
            unregister_code16(ES_NTIL);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(ES_N);
            break;
    }
    dance_state.step = 0;
}


tap_dance_action_t tap_dance_actions[] = {
    [DOT_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(on_dot_dance, dot_dance_finished, dot_dance_reset),
    [COMMA_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(on_comma_dance, comma_dance_finished, comma_dance_reset),
    [ACCENTS_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(on_accents_dance, accents_dance_finished, accents_dance_reset),
    [QUOTES] = ACTION_TAP_DANCE_DOUBLE(ES_QUOT, ES_DQUO),
    [EXCLAMATION] = ACTION_TAP_DANCE_DOUBLE(ES_EXLM, ES_IEXL),
    [QUESTION] = ACTION_TAP_DANCE_DOUBLE(ES_QUES, ES_IQUE),
    [DANCE_54] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_54, dance_54_finished, dance_54_reset),
    [DANCE_55] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_55, dance_55_finished, dance_55_reset),
    [DANCE_56] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_56, dance_56_finished, dance_56_reset),
    [DANCE_57] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_57, dance_57_finished, dance_57_reset),
    [DANCE_58] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_58, dance_58_finished, dance_58_reset),
    [DANCE_59] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_59, dance_59_finished, dance_59_reset),
    [DANCE_60] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_60, dance_60_finished, dance_60_reset),
    [DANCE_63] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_63, dance_63_finished, dance_63_reset),
    [PLUS_HME] = ACTION_TAP_DANCE_FN_ADVANCED(on_plus_hme, plus_hme_finished, plus_hme_reset),
    [BKSL_END] = ACTION_TAP_DANCE_FN_ADVANCED(on_bksl_end, bksl_end_finished, bksl_end_reset),
    [NTIL] = ACTION_TAP_DANCE_FN_ADVANCED(on_ntil_end, ntil_end_finished, ntil_end_reset),
};
