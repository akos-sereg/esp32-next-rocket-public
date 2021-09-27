#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "include/max7219.h"
#include "include/timer.h"
#include "include/networking.h"
#include "include/response-parser.h"
#include "include/https.h"
#include "include/config.h"

/**
 * normally we would go with 1000 ms (1sec) ticks, but refreshing the timer components consume a little bit of a
 * time in every cycle, so we have to delay shorter periods.
 */
#define DELAY 987

/**
 * we need to fetch the latest launch time at a regular interval: in every hour
 */
#define REFRESH_AFTER_SECONDS	3600

/**
 * in case we are close to the launch time - eg. launch in [LAUNCH_CLOSE_SECONDS] seconds -, we want to refresh
 * the timer more frequently, as it might happen that the launch time is getting delayed.
 */
#define REFRESH_AFTER_SECONDS_WHEN_LAUNCH_IS_CLOSE   300

/**
 * we consider that we are close to the launch time, if launch will happen within [LAUNCH_CLOSE_SECONDS] seconds.
 */
#define LAUNCH_CLOSE_SECONDS    5400 /* 5400 seconds: 1.5 hours */
#define HOST HSPI_HOST

#define CHECK(expr, msg) \
    while ((res = expr) != ESP_OK) { \
        printf(msg "\n", res); \
        vTaskDelay(250 / portTICK_RATE_MS); \
    }

void task()
{
    esp_err_t res;

    // Configure SPI bus
    spi_bus_config_t cfg = {
       .mosi_io_num = GPIO_8DIGIT_DISPLAY_DATA,
       .miso_io_num = -1,
       .sclk_io_num = GPIO_8DIGIT_DISPLAY_CLOCK,
       .quadwp_io_num = -1,
       .quadhd_io_num = -1,
       .max_transfer_sz = 0,
       .flags = 0
    };
    CHECK(spi_bus_initialize(HOST, &cfg, 1), "Could not initialize SPI bus: %d");

    // Configure device
    max7219_t dev = {
       .cascade_size = 1,
       .digits = 8,
       .mirrored = true
    };
    CHECK(max7219_init_desc(&dev, HOST, GPIO_8DIGIT_DISPLAY_CS), "Could not initialize MAX7129 descriptor: %d");

    char buf[9];

    CHECK(max7219_init(&dev), "Could not initialize MAX7129: %d");

    max7219_set_brightness(&dev, BRIGHTNESS_LAUNCH_DATE);
    max7219_set_decode_mode(&dev, true);
    timer_erase_countdown();
    timer_erase_launch_time();

    int http_called = 0;
    int uptime_seconds = 0;
    int are_displays_set = 0;
    while (1)
    {
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
        uptime_seconds++;

        // at a regular interval, make a request and update the timers
        if (uptime_seconds > REFRESH_AFTER_SECONDS
            || (timer_seconds < LAUNCH_CLOSE_SECONDS && uptime_seconds > REFRESH_AFTER_SECONDS_WHEN_LAUNCH_IS_CLOSE)) {
            // esp_restart();
            http_called = 0;
            is_extracted = 0;
            are_displays_set = 0;
            uptime_seconds = 0;
        }

        // decrement countdown timer (count down in seconds)
        if (!no_upcoming_mission && are_displays_set) {
            timer_add_countdown_seconds(-1);
        }

        // fetch launch time, if its not fetched already
        if (IS_WIFI_CONNECTED && !http_called) {
            http_called = 1;
            https_do_https_request();
        }

        // once data is fetched (payload extracted), and we have not set the display yet.
        // this is a first-time initialization of the displays. launch timer and 8 digit display
        // should not be refreshed afterwards. countdown clock is being handled by timer_add_countdown_seconds()
        if (is_extracted && !are_displays_set) {
            if (no_upcoming_mission) {
                max7219_clear(&dev);
                timer_erase_countdown();
                timer_erase_launch_time();
            } else {
                max7219_clear(&dev);
                sprintf(buf, "%02d-%02d-%02d", launch_year, launch_month, launch_day);
                max7219_draw_text_7seg(&dev, 0, buf);
                timer_render_launch_time();

                // testing --------------------------
                // timer_seconds = REFRESH_AFTER_SECONDS;
                // testing --------------------------
            }

            are_displays_set = 1;
        }
    }
}

void app_main()
{
    timer_setup_timers();
    networking_initialize_wifi();
    task();
}