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

    /* Select the internal 32 kHz source before powering down the crystal.
     * PA10 is X32KI as well as TMR1; neither peripheral may own this pin. */
    LClk32K_Cfg(Clk32K_LSI, ENABLE);
    LClk32K_Select(Clk32K_LSI);
    PWR_UnitModCfg(DISABLE, UNIT_SYS_LSE);
    TMR1_Disable();
    TMR1_PWMDisable();
    TMR1_DMACfg(DISABLE, 0, 0, Mode_Single);
    R8_TMR1_INTER_EN = 0;
    GPIOPinRemap(ENABLE, RB_PIN_TMR1); /* Inactive TMR1 now maps to PB10. */

    /* Keep DIN low until the RGB supply has stabilized. */
    PinOutput(WS2812_EN_PORT, WS2812_EN_PIN, 0);
    PinOutput(WS2812_PORT, WS2812_PIN, 0);
    PinOutput(WS2812_EN_PORT, WS2812_EN_PIN, 1); /* PA9: RGB_EN high */
    DelayUs(300);
    PinOutput(WS2812_PORT, WS2812_PIN, 1);       /* PA10: DIN high */

    /* Reassert the data GPIO directly, without a delay or scheduler call.
     * This is a diagnostic workaround, not evidence of a hardware fix. */
    while (1) {
        R32_PA_PU &= ~WS2812_PIN;
        R32_PA_PD_DRV &= ~WS2812_PIN; /* 5 mA push-pull output */
        R32_PA_OUT |= WS2812_PIN;
        R32_PA_DIR |= WS2812_PIN;
    }
}
