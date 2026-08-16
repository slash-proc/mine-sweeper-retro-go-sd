/**************************************************
 * File  : embMineDevice.c
 * Author: MANDA (slipperstree@gmail.com)
 * Port  : Retro-Go SD homebrew glue
 ***************************************************/

#include "embMineDevice.h"
#include "boradSupport.h"
#include "gw_draw.h"
#include "mine_platform.h"

extern u8 buff[128];

u8 gDevCurPosX = 0, gDevCurPosY = 0;

u16 drawAreaStartX = 0;
u16 drawAreaStartY = 0;
u16 drawAreaEndX = 0;
u16 drawAreaEndY = 0;
u16 drawAreaCurrX = 0;
u16 drawAreaCurrY = 0;

void mine_dev_load_setting(SaveData_Struct *setting);
void mine_dev_save_setting(SaveData_Struct *setting);

void devDisplayInit(u16 bgColor)
{
    (void)bgColor;
    devScreenOFF();
    devScreenON();
}

void devPrepareForDrawArea(u16 startX, u16 startY, u16 areaWidth, u16 areaHeight)
{
    drawAreaStartX = startX;
    drawAreaStartY = startY;
    drawAreaEndX = startX + areaWidth - 1;
    drawAreaEndY = startY + areaHeight - 1;
    drawAreaCurrX = drawAreaStartX;
    drawAreaCurrY = drawAreaStartY;
}

void devPointInDrawArea(u16 color)
{
    if (drawAreaCurrX > drawAreaEndX) {
        drawAreaCurrX = drawAreaStartX;
        drawAreaCurrY++;
    }

    if (drawAreaCurrY > drawAreaEndY || drawAreaCurrY >= SCREEN_H ||
        drawAreaCurrX >= SCREEN_W)
        return;

    gw_plot(drawAreaCurrX, drawAreaCurrY, color);
    drawAreaCurrX++;
}

void devDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color)
{
    gw_line_width(x1, y1, x2, y2, width, color);
}

void devFillRectange(u16 x, u16 y, u16 width, u16 height, u16 color)
{
    u16 wTmp;
    u16 hTmp;

    devPrepareForDrawArea(x, y, width, height);
    wTmp = width;
    while (wTmp--) {
        hTmp = height;
        while (hTmp--)
            devPointInDrawArea(color);
    }
}

void devScreenOFF(void)
{
    mine_lcd_backlight_off();
}

void devScreenON(void)
{
    mine_lcd_backlight_on();
}

void devLoadSetting(SaveData_Struct *setting)
{
    mine_dev_load_setting(setting);
}

void devSaveSetting(SaveData_Struct *setting)
{
    mine_dev_save_setting(setting);
}

void devSndInit(void)
{
}

void devSndBeepShort(void)
{
    mine_sound_queue_beep(880, 40);
}

void devPlaySound(Sound_Type soundType)
{
    switch (soundType) {
    case SOUND_CLICK:
        mine_sound_queue_beep(660, 35);
        break;
    case SOUND_FLAG:
        mine_sound_queue_beep(520, 45);
        break;
    case SOUND_MENU:
        mine_sound_queue_beep(400, 30);
        break;
    case SOUND_DEAD:
        mine_sound_queue_beep(180, 180);
        break;
    case SOUND_GAMEOVER:
        mine_sound_queue_beep(220, 250);
        break;
    case SOUND_HISCORE:
        mine_sound_queue_beep(880, 120);
        break;
    default:
        break;
    }
}

void devEnterHomePage(void)
{
}

void devEnterDemoPage(void)
{
}

void devEnterGamePage(void)
{
}

void devEnterGameOverPage(void)
{
}
