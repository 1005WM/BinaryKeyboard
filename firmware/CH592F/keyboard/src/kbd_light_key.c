#include "kbd_light_key.h"

static bool s_down;
static bool s_pending_click;
static bool s_second_press;
static bool s_hold_handled;
static uint32_t s_press_ms;
static uint32_t s_release_ms;

void KBD_LightKey_Reset(void)
{
    s_down = false;
    s_pending_click = false;
    s_second_press = false;
    s_hold_handled = false;
}

kbd_light_key_action_t KBD_LightKey_HandleEvent(bool pressed, uint32_t now)
{
    if (pressed)
    {
        if (s_down)
            return KBD_LIGHT_KEY_NONE;

        bool expired = s_pending_click &&
                       (uint32_t)(now - s_release_ms) >= KBD_LIGHT_KEY_DOUBLE_MS;
        s_second_press = s_pending_click && !expired;
        s_pending_click = false;
        s_down = true;
        s_hold_handled = false;
        s_press_ms = now;
        return expired ? KBD_LIGHT_KEY_COLOR : KBD_LIGHT_KEY_NONE;
    }

    /* Ignore unmatched releases, for example after BOOT or a disconnect. */
    if (!s_down)
        return KBD_LIGHT_KEY_NONE;

    s_down = false;
    if (s_hold_handled)
        return KBD_LIGHT_KEY_NONE;

    /* A queued release may arrive before the main loop notices a long hold. */
    if ((uint32_t)(now - s_press_ms) >= KBD_LIGHT_KEY_HOLD_MS)
    {
        s_second_press = false;
        return KBD_LIGHT_KEY_BRIGHTNESS;
    }

    if (s_second_press)
    {
        s_second_press = false;
        return KBD_LIGHT_KEY_MODE;
    }

    s_pending_click = true;
    s_release_ms = now;
    return KBD_LIGHT_KEY_NONE;
}

kbd_light_key_action_t KBD_LightKey_Process(uint32_t now)
{
    if (s_down && !s_hold_handled &&
        (uint32_t)(now - s_press_ms) >= KBD_LIGHT_KEY_HOLD_MS)
    {
        s_hold_handled = true;
        s_second_press = false;
        return KBD_LIGHT_KEY_BRIGHTNESS;
    }

    if (!s_down && s_pending_click &&
        (uint32_t)(now - s_release_ms) >= KBD_LIGHT_KEY_DOUBLE_MS)
    {
        s_pending_click = false;
        return KBD_LIGHT_KEY_COLOR;
    }
    return KBD_LIGHT_KEY_NONE;
}
