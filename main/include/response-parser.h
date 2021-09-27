#ifndef __response_parser_h_included__
#define __response_parser_h_included__

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "networking.h"

extern void extract_response();

extern int days_to_launch;
extern int hours_to_launch;
extern int minutes_to_launch;
extern int launch_year;
extern int launch_month;
extern int launch_day;
extern int launch_hour;
extern int launch_minute;

extern int error_response;
extern int no_upcoming_mission;
extern int is_extracted;

#endif