#ifndef __config_h_included__
#define __config_h_included__

#define BRIGHTNESS_TIMER_COUNTDOWN    6     /* 0-7 */
#define BRIGHTNESS_TIMER_LAUNCH_TIME  3     /* 0-7 */
#define BRIGHTNESS_LAUNCH_DATE        5     /* 0-31 */

#define GPIO_TIMER_COUNTDOWN_CLOCK    27
#define GPIO_TIMER_COUNTDOWN_DATA     26
#define GPIO_LAUNCH_TIME_CLOCK        25
#define GPIO_LAUNCH_TIME_DATA         33

#define GPIO_8DIGIT_DISPLAY_CS        5     /* Load */
#define GPIO_8DIGIT_DISPLAY_CLOCK     18
#define GPIO_8DIGIT_DISPLAY_DATA      19

#define REST_HOST     "www.your-relay-server.io"
#define REST_ENDPOINT "/rest/iot/next-rocket"

#endif