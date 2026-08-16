/*
 * Minesweeper — Retro-Go SD GWHB homebrew
 *
 * Port of https://github.com/slipperstree/game-and-watch-mine-sweeper
 * (MANDA / slipperstree) onto the freestanding Retro-Go SD ABI.
 *
 * Controls (same as upstream):
 *   Home: Up/Down select level, A start
 *   Game: D-pad move, A dig, B flag
 *   Game Over: any button → home; TIME → info page
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "gw_lcd.h"
#include "gw_audio.h"
#include "rom_manager.h"
#include "odroid_system.h"
#include "odroid_overlay.h"
#include "odroid_settings.h"
#include "gw_malloc.h"

#include "control.h"
#include "mine_platform.h"
#include "myMathUtil.h"

#include "gw_core_bridge.h"
#include "gw_core_i18n.h"

#define APP_ID       14 /* APPID_HOMEBREW */
#define FPS          60
#define SAMPLE_RATE  16000
#define AUDIO_LENGTH (SAMPLE_RATE / FPS)

static odroid_gamepad_state_t pad;
static odroid_gamepad_state_t pad_prev;

/* --- settings persistence (replaces unused EEPROM in upstream) ------------ */

/* Declared in embMineDevice.h — keep the struct layout here without
 * pulling mine color.h (WORD) into the FatFs / ABI header soup. */
typedef struct {
    unsigned int hiScoreLvl1;
    unsigned int hiScoreLvl2;
    unsigned int hiScoreLvl3;
    unsigned char soundOnOff;
    unsigned int colorBackGround;
    unsigned int colorFront;
    unsigned int colorFrame;
} SaveData_Struct;

void mine_dev_load_setting(SaveData_Struct *setting)
{
    int32_t magic = odroid_settings_app_int32_get("ms_magic", 0);
    if (magic != 0x4D494E45) /* 'MINE' */
        return;

    setting->hiScoreLvl1 = (unsigned int)odroid_settings_app_int32_get("ms_hs1", (int32_t)setting->hiScoreLvl1);
    setting->hiScoreLvl2 = (unsigned int)odroid_settings_app_int32_get("ms_hs2", (int32_t)setting->hiScoreLvl2);
    setting->hiScoreLvl3 = (unsigned int)odroid_settings_app_int32_get("ms_hs3", (int32_t)setting->hiScoreLvl3);
    setting->soundOnOff = (unsigned char)odroid_settings_app_int32_get("ms_snd", setting->soundOnOff);
}

void mine_dev_save_setting(SaveData_Struct *setting)
{
    odroid_settings_app_int32_set("ms_magic", 0x4D494E45);
    odroid_settings_app_int32_set("ms_hs1", (int32_t)setting->hiScoreLvl1);
    odroid_settings_app_int32_set("ms_hs2", (int32_t)setting->hiScoreLvl2);
    odroid_settings_app_int32_set("ms_hs3", (int32_t)setting->hiScoreLvl3);
    odroid_settings_app_int32_set("ms_snd", setting->soundOnOff);
}

/* --- system callbacks ----------------------------------------------------- */

static bool LoadState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static bool SaveState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static void present(void)
{
    uint16_t *dst = lcd_get_active_buffer();
    memcpy(dst, mine_framebuffer(), WIDTH * HEIGHT * sizeof(uint16_t));
    common_ingame_overlay();
}

static void *Screenshot(void)
{
    lcd_wait_for_vblank();
    present();
    return lcd_get_active_buffer();
}

static void Shutdown(void)
{
}

static void SleepWake(void)
{
    odroid_audio_init(SAMPLE_RATE);
    audio_clear_buffers();
    audio_start_playing(AUDIO_LENGTH);
}

static void SramSave(void)
{
}

static uint32_t map_buttons(const odroid_gamepad_state_t *joy)
{
    uint32_t b = 0;

    if (joy->values[ODROID_INPUT_LEFT])
        b |= B_Left;
    if (joy->values[ODROID_INPUT_UP])
        b |= B_Up;
    if (joy->values[ODROID_INPUT_RIGHT])
        b |= B_Right;
    if (joy->values[ODROID_INPUT_DOWN])
        b |= B_Down;
    if (joy->values[ODROID_INPUT_A])
        b |= B_A;
    if (joy->values[ODROID_INPUT_B])
        b |= B_B;
    /* GAME / TIME on G&W — also START/SELECT / X/Y depending on face. */
    if (joy->values[ODROID_INPUT_X] || joy->values[ODROID_INPUT_START])
        b |= B_GAME;
    if (joy->values[ODROID_INPUT_Y] || joy->values[ODROID_INPUT_SELECT])
        b |= B_TIME;
    /* Do not forward PAUSE/POWER — firmware owns the system menu / sleep. */
    return b;
}

static void submit_audio(void)
{
    int16_t *buf;
    uint16_t len;
    uint16_t i;
    int32_t vol;

    if (common_emu_sound_loop_is_muted())
        return;

    buf = audio_get_active_buffer();
    len = audio_get_buffer_length();
    if (!buf || !len)
        return;

    vol = common_emu_sound_get_volume();
    for (i = 0; i < len; i++) {
        int16_t s = 0;
        mine_sound_pop_sample(&s);
        buf[i] = (int16_t)((s * vol) / 255);
    }
}

void app_main(uint8_t load_state, uint8_t start_paused, int8_t save_slot)
{
    odroid_gamepad_state_t joystick;
    odroid_dialog_choice_t options[1];

    (void)load_state;
    (void)save_slot;

    gw_core_bridge_init();
    ram_start = (uint32_t)&__CORE_BSS_END__;
    memset(&pad, 0, sizeof(pad));
    memset(&pad_prev, 0, sizeof(pad_prev));
    mine_sound_set_sample_rate(SAMPLE_RATE);

    if (start_paused) {
        common_emu_state.pause_after_frames = 2;
        odroid_audio_mute(true);
    } else {
        common_emu_state.pause_after_frames = 0;
    }
    common_emu_state.frame_time_10us = (uint16_t)(100000 / FPS + 0.5f);
    lcd_set_refresh_rate(FPS);

    odroid_system_init(APP_ID, SAMPLE_RATE);
    odroid_system_emu_init(&LoadState, &SaveState, &Screenshot,
                           &Shutdown, &SleepWake, &SramSave, NULL);

    /* No pause-menu game options for now. */
    options[0] = (odroid_dialog_choice_t)ODROID_DIALOG_CHOICE_LAST;

    audio_start_playing(AUDIO_LENGTH);
    lcd_clear_buffers();

    /* Vary the PRNG a bit across launches (upstream used a fixed seed). */
    my_seed_rand((s32)((uintptr_t)&joystick ^ (uintptr_t)&pad ^ 0x4D494E45));

    CTL_init();

    while (1) {
        wdog_refresh();

        bool draw_frame = common_emu_frame_loop();

        odroid_input_read_gamepad(&joystick);
        common_emu_input_loop(&joystick, options, &present);
        common_emu_input_loop_handle_turbo(&joystick);

        pad = joystick;
        mine_platform_set_buttons(map_buttons(&pad));

        CTL_run();

        if (draw_frame) {
            present();
            lcd_swap();
        }

        submit_audio();
        common_emu_sound_sync(false);

        pad_prev = pad;
    }
}
