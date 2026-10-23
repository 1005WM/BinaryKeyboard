/* Standalone GPIO level test. No USB/BLE, key scanning, or RGB animation. */
#include "CH59x_common.h"
#include "kbd_config.h"

static void PinOutput(gpio_port_t port, uint32_t pin, uint8_t high)
{
    if (port == GPIO_PORT_A) {
        if (high) {
            GPIOA_SetBits(pin);
        } else {
            GPIOA_ResetBits(pin);
        }
        GPIOA_ModeCfg(pin, GPIO_ModeOut_PP_5mA);
    } else {
        if (high) {
            GPIOB_SetBits(pin);
        } else {
            GPIOB_ResetBits(pin);
        }
        GPIOB_ModeCfg(pin, GPIO_ModeOut_PP_5mA);
    }
}

int main(void)
{
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    WWDG_ResetCfg(DISABLE);
    WWDG_ITCfg(DISABLE);

    /* PA10 also serves the 32 kHz crystal and TMR1: release both owners. */
    PWR_UnitModCfg(DISABLE, UNIT_SYS_LSE);
    TMR1_Disable();
    TMR1_PWMDisable();
    TMR1_DMACfg(DISABLE, 0, 0, Mode_Single);

    /* Keep DIN low until the RGB supply has stabilized. */
    PinOutput(WS2812_EN_PORT, WS2812_EN_PIN, 0);
    PinOutput(WS2812_PORT, WS2812_PIN, 0);
    PinOutput(WS2812_EN_PORT, WS2812_EN_PIN, 1); /* PA9: RGB_EN high */
    DelayUs(300);
    PinOutput(WS2812_PORT, WS2812_PIN, 1);       /* PA10: DIN high */

    /* No scheduler or sleep: both GPIO output latches remain high. */
    while (1) {
    }
}
