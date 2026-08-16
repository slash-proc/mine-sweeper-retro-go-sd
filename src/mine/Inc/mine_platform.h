#ifndef __MINE_PLATFORM_H_
#define __MINE_PLATFORM_H_

#include <stdint.h>

/* Button bitmasks — same layout as the standalone G&W port. */
#define B_Left  (1u << 0)
#define B_Up    (1u << 1)
#define B_Right (1u << 2)
#define B_Down  (1u << 3)
#define B_A     (1u << 4)
#define B_B     (1u << 5)
#define B_TIME  (1u << 6)
#define B_GAME  (1u << 7)
#define B_PAUSE (1u << 8)
#define B_POWER (1u << 9)

uint32_t buttons_get(void);

/* Feed the key scanner from the Retro-Go pad each frame. */
void mine_platform_set_buttons(uint32_t buttons);

/* RGB565 staging buffer the game draws into (copied to LCD each frame). */
uint16_t *mine_framebuffer(void);

void mine_lcd_backlight_on(void);
void mine_lcd_backlight_off(void);

/* Optional short beeps queued for the audio half-buffer filler. */
void mine_sound_set_sample_rate(uint32_t hz);
void mine_sound_queue_beep(uint16_t hz, uint16_t ms);
int mine_sound_pop_sample(int16_t *out);

#endif
