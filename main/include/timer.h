#ifndef __timer_h_included__
#define __timer_h_included__

// countdown & launch date display modules
extern void timer_setup_timers();

// countdown
extern int timer_seconds;    /* seconds displayed on tm1637 display - all the times: in "set" mode, and in "countdown" mode as well */
extern void timer_add_countdown_seconds(int seconds); /* in "set" mode, add number of seconds to timer_seconds, update display, etc */
extern void timer_render_countdown();  /* render timer_seconds on display */
extern void timer_erase_countdown();           /* make display blank */

// launch date
extern void timer_erase_launch_time();
extern void timer_render_launch_time();

#endif