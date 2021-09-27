#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "include/timer.h"
#include "include/tm1637.h"
#include "include/response-parser.h"
#include "include/config.h"

int DISPLAY_PAD_ZERO = 1;
tm1637_led_t *timer_launch_time;
tm1637_led_t *timer_countdown;

int timer_seconds = 999999;

void timer_setup_timers()
{
    timer_countdown = tm1637_init(GPIO_TIMER_COUNTDOWN_CLOCK, GPIO_TIMER_COUNTDOWN_DATA);
    timer_launch_time = tm1637_init(GPIO_LAUNCH_TIME_CLOCK, GPIO_LAUNCH_TIME_DATA);
    tm1637_set_brightness(timer_countdown, BRIGHTNESS_TIMER_COUNTDOWN);
    tm1637_set_brightness(timer_launch_time, BRIGHTNESS_TIMER_LAUNCH_TIME);
}

void timer_add_countdown_seconds(int seconds) {
    timer_seconds += seconds;

    if (timer_seconds < 0) {
	    timer_seconds = 0;
	    return;
    }

    if (timer_seconds <= 5999) {
        timer_render_countdown();
        return;
    }

    if (timer_seconds > 5999) {
        timer_erase_countdown();
        return;
    }
}

void timer_render_countdown() {

    int digit1 = 0;
    int digit2 = 0;
    int digit3 = 0;
    int digit4 = 0;

    if (timer_seconds < 0 || timer_seconds > 5999) {
	    // 5999 seconds: 99 minutes and 59 seconds - max interval that can be displayed on tm1637
	    printf("erase timer, seocnds left: %d\n", timer_seconds);
	    timer_erase_countdown();
	    return;
    }

    int minutes = floor(timer_seconds / 60);
    int seconds = timer_seconds % 60;

    if (minutes < 10) {
        digit1 = 0;
        digit2 = minutes;
    } else {
        digit1 = floor(minutes / 10);
        digit2 = minutes % 10;
    }

    if (seconds < 10) {
        digit3 = 0;
        digit4 = seconds;
    } else {
        digit3 = floor(seconds / 10);
        digit4 = seconds % 10;
    }

    if (!DISPLAY_PAD_ZERO) {
        // example: 9 mins 12 seconds will be "9:12" instead of "09:12"
        if (digit1 == 0) {
            tm1637_set_segment_raw(timer_countdown, 0, 0x00);
        } else {
            tm1637_set_segment_number(timer_countdown, 0, digit1, false);
        }
    } else {
	    tm1637_set_segment_number(timer_countdown, 0, digit1, false);
    }

    tm1637_set_segment_number(timer_countdown, 1, digit2, true);
    tm1637_set_segment_number(timer_countdown, 2, digit3, false);
    tm1637_set_segment_number(timer_countdown, 3, digit4, false);
}

void timer_render_launch_time() {

    int digit1 = 0;
    int digit2 = 0;
    int digit3 = 0;
    int digit4 = 0;


    if (launch_hour < 10) {
        digit1 = 0;
        digit2 = launch_hour;
    } else {
        digit1 = floor(launch_hour / 10);
        digit2 = launch_hour % 10;
    }

    if (launch_minute < 10) {
        digit3 = 0;
        digit4 = launch_minute;
    } else {
        digit3 = floor(launch_minute / 10);
        digit4 = launch_minute % 10;
    }

    if (!DISPLAY_PAD_ZERO) {
        if (digit1 == 0) {
            tm1637_set_segment_raw(timer_launch_time, 0, 0x00);
        } else {
            tm1637_set_segment_number(timer_launch_time, 0, digit1, false);
        }
    } else {
	    tm1637_set_segment_number(timer_launch_time, 0, digit1, false);
    }

    tm1637_set_segment_number(timer_launch_time, 1, digit2, true);
    tm1637_set_segment_number(timer_launch_time, 2, digit3, false);
    tm1637_set_segment_number(timer_launch_time, 3, digit4, false);
}

void timer_erase_countdown() {
    tm1637_set_segment_raw(timer_countdown, 0, 0x00);
    tm1637_set_segment_raw(timer_countdown, 1, 0x00);
    tm1637_set_segment_raw(timer_countdown, 2, 0x00);
    tm1637_set_segment_raw(timer_countdown, 3, 0x00);
}

void timer_erase_launch_time() {
    tm1637_set_segment_raw(timer_launch_time, 0, 0x00);
    tm1637_set_segment_raw(timer_launch_time, 1, 0x00);
    tm1637_set_segment_raw(timer_launch_time, 2, 0x00);
    tm1637_set_segment_raw(timer_launch_time, 3, 0x00);
}
