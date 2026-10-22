#ifndef KBD_LIGHT_KEY_H
#define KBD_LIGHT_KEY_H

#include <stdbool.h>
#include <stdint.h>

#define KBD_LIGHT_KEY_DOUBLE_MS 300u
#define KBD_LIGHT_KEY_HOLD_MS 800u

typedef enum {
    KBD_LIGHT_KEY_NONE,
    KBD_LIGHT_KEY_COLOR,
    KBD_LIGHT_KEY_MODE,
    KBD_LIGHT_KEY_BRIGHTNESS,
} kbd_light_key_action_t;

/* All timestamps use the same millisecond clock as key_event_t.tick_ms. */
void KBD_LightKey_Reset(void);
kbd_light_key_action_t KBD_LightKey_HandleEvent(bool pressed, uint32_t now);
kbd_light_key_action_t KBD_LightKey_Process(uint32_t now);

#endif
