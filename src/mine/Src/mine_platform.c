#include "mine_platform.h"

#include <string.h>

#define SCREEN_W 320
#define SCREEN_H 240
#define FB_PIXELS (SCREEN_W * SCREEN_H)

static uint16_t s_fb[FB_PIXELS];
static uint32_t s_buttons;
static uint16_t s_beep_hz;
static uint32_t s_beep_samples_left;
static uint32_t s_beep_phase;
static uint32_t s_sample_rate = 16000;

uint16_t *mine_framebuffer(void)
{
    return s_fb;
}

void mine_platform_set_buttons(uint32_t buttons)
{
    s_buttons = buttons;
}

uint32_t buttons_get(void)
{
    return s_buttons;
}

void mine_lcd_backlight_on(void)
{
}

void mine_lcd_backlight_off(void)
{
}

void mine_sound_set_sample_rate(uint32_t hz)
{
    s_sample_rate = hz ? hz : 16000;
}

void mine_sound_queue_beep(uint16_t hz, uint16_t ms)
{
    if (hz == 0 || ms == 0) {
        s_beep_samples_left = 0;
        return;
    }
    s_beep_hz = hz;
    s_beep_samples_left = ((uint32_t)ms * s_sample_rate) / 1000u;
    if (s_beep_samples_left == 0)
        s_beep_samples_left = 1;
}

int mine_sound_pop_sample(int16_t *out)
{
    if (s_beep_samples_left == 0) {
        *out = 0;
        return 0;
    }

    {
        uint32_t step = ((uint32_t)s_beep_hz << 16) / s_sample_rate;
        *out = (s_beep_phase & 0x8000u) ? 6000 : -6000;
        s_beep_phase = (s_beep_phase + step) & 0xffffu;
        s_beep_samples_left--;
    }
    return 1;
}
