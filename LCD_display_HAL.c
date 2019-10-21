/*
 * LCD_display_HAL.c
 *
 */

#include "LCD_display_HAL.h"

// This function is not used in upper level functions, so it does not need to be declared in the graphics_HAL.h
void init_fonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void init_graphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmss12);

    init_fonts();

    Graphics_clearDisplay(g_sContext_p);
}

