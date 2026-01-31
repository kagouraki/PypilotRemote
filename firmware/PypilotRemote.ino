/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui/ui.h"
#include "functions.h"
#include "wireless.h"
#include "pypilot_client.h"
#include "buttons.h"

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

/* Display flushing with X/Y offset correction */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area,
		lv_color_t *color_p) {
	/* Panel-specific offsets (landscape mode) */
	const int16_t X_OFFSET = 0;    // + = right, - = left
	const int16_t Y_OFFSET = 0;   // + = up,    - = down 19

	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	int32_t x = area->x1 + X_OFFSET;
	int32_t y = area->y1 + Y_OFFSET;

	tft.startWrite();
	tft.setAddrWindow(x, y, w, h);
	tft.pushColors((uint16_t*) &color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp_drv);
}

void setup() {
	Serial.begin(115200); /* prepare for possible serial debug */

	//*************LVGL initialization START*************
	lv_init();

	tft.begin(); /* TFT init */
	tft.setRotation(3); /* Landscape orientation, flipped */

	lv_disp_draw_buf_init(&draw_buf, buf, NULL,
			screenWidth * screenHeight / 10);

	/*Initialize the display*/
	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	/*Change the following line to your display resolution*/
	disp_drv.hor_res = screenWidth;
	disp_drv.ver_res = screenHeight;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);

	ui_init();

	ui_color_swap_init();
	ui_apply_swap_globally_once();

	lv_timer_create([](lv_timer_t *t) { //this timer for logo screen

		// Load main screen
		lv_disp_load_scr(ui_Screen1);

		// Delete boot screen (free RAM)
		lv_obj_del(ui_Screen2);

		// Remove timer
		lv_timer_del(t);

	}, 10000, NULL);

	//*************LVGL initialization END*************

	lv_obj_set_style_opa(ui_wifiRed, 0, LV_PART_MAIN);
	lv_obj_set_style_opa(ui_wifiGreen, 0, LV_PART_MAIN); //max opacity
	lv_obj_set_style_opa(ui_wifiYellow, 0, LV_PART_MAIN);
	lv_obj_add_flag(ui_engaged, LV_OBJ_FLAG_HIDDEN); //set hidden flag
	//ui_apply_swap_globally_once();

	initWireless();
	setupButtons();
	displayLog("Done setup");

}

int course = 0;
uint32_t lastUpdate = 0;

void loop() {

	lv_timer_handler();

	updateWiFi();

	tcp.loop();

	setWifiIcon();

	updateCompassPosition();

	updateCmd();

	engagedText(); //auto icon home screen

	activeMode();  //active mode home screen

	setRudderSlider();

	pollButtons();

	processButtonEvents();

	updateInfoScreen();

	delay(5);
}
