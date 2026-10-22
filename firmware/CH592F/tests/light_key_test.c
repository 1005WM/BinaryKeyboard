/* Host test: cc -I keyboard/include tests/light_key_test.c
 *               keyboard/src/kbd_light_key.c -o light_key_test */
#include "kbd_light_key.h"
#include <assert.h>
#include <stdio.h>

static void single_click(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 50) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(349) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(350) == KBD_LIGHT_KEY_COLOR);
    assert(KBD_LightKey_Process(900) == KBD_LIGHT_KEY_NONE);
}

static void double_click(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 50) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(true, 200) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(400) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 450) == KBD_LIGHT_KEY_MODE);
    assert(KBD_LightKey_Process(1000) == KBD_LIGHT_KEY_NONE);
}

static void hold_once(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(799) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(800) == KBD_LIGHT_KEY_BRIGHTNESS);
    assert(KBD_LightKey_Process(5000) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 5100) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(5500) == KBD_LIGHT_KEY_NONE);
}

static void delayed_main_loop(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 800) == KBD_LIGHT_KEY_BRIGHTNESS);
    assert(KBD_LightKey_Process(1200) == KBD_LIGHT_KEY_NONE);

    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 50) == KBD_LIGHT_KEY_NONE);
    /* At the exact double-click deadline this is a separate click. */
    assert(KBD_LightKey_HandleEvent(true, 350) == KBD_LIGHT_KEY_COLOR);
    assert(KBD_LightKey_HandleEvent(false, 400) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(700) == KBD_LIGHT_KEY_COLOR);
}

static void short_then_hold(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 50) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(true, 100) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(900) == KBD_LIGHT_KEY_BRIGHTNESS);
    assert(KBD_LightKey_HandleEvent(false, 950) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(1500) == KBD_LIGHT_KEY_NONE);
}

static void cancel_on_reset(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 50) == KBD_LIGHT_KEY_NONE);
    KBD_LightKey_Reset();
    assert(KBD_LightKey_Process(1000) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(true, 1100) == KBD_LIGHT_KEY_NONE);
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(false, 2000) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(2500) == KBD_LIGHT_KEY_NONE);
}

static void clock_wrap(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, UINT32_MAX - 400u) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(398) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(399) == KBD_LIGHT_KEY_BRIGHTNESS);
    assert(KBD_LightKey_HandleEvent(false, 450) == KBD_LIGHT_KEY_NONE);

    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(true, UINT32_MAX - 200u) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, UINT32_MAX - 100u) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(198) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(199) == KBD_LIGHT_KEY_COLOR);
}

static void duplicate_edges(void)
{
    KBD_LightKey_Reset();
    assert(KBD_LightKey_HandleEvent(false, 0) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(true, 10) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(true, 100) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(810) == KBD_LIGHT_KEY_BRIGHTNESS);
    assert(KBD_LightKey_HandleEvent(false, 820) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_HandleEvent(false, 830) == KBD_LIGHT_KEY_NONE);
    assert(KBD_LightKey_Process(1500) == KBD_LIGHT_KEY_NONE);
}

int main(void)
{
    single_click();
    double_click();
    hold_once();
    delayed_main_loop();
    short_then_hold();
    cancel_on_reset();
    clock_wrap();
    duplicate_edges();
    puts("K5 gesture tests: 8 scenarios passed");
    return 0;
}
