#include "include/response-parser.h"
#include "include/timer.h"

int days_to_launch;
int hours_to_launch;
int minutes_to_launch;
int launch_year;
int launch_month;
int launch_day;
int launch_hour;
int launch_minute;

int error_response;
int no_upcoming_mission;
int is_extracted = 0;

void extract_response() {

    int i;
    int body_position = 0;
    char days[16];
    char buff[16];
    int buff_i = 0;

    if (body_position == -1) {
	printf("-> parser -> Unable to find position\n");
	return;
    }

    int current_pos = 0;

    /* *********************************************************************** */
    // Error handling
    /* *********************************************************************** */
    if ((body_position + 1) < strlen(response_payload)
	    && response_payload[body_position] == 'e'
	    && response_payload[body_position+1] == '!') {
	    error_response = 1;
	    return;
    }

    /* *********************************************************************** */
    // No upcoming mission found
    /* *********************************************************************** */
    if ((body_position + 1) < strlen(response_payload)
	    && response_payload[body_position] == 'n'
	    && response_payload[body_position+1] == '!') {
	    no_upcoming_mission = 1;
	    is_extracted = 1;
	    return;
    }

    /* *********************************************************************** */
    // Extract valid payload
    /* *********************************************************************** */

    no_upcoming_mission = 0;
    error_response = 0;

    // extract agency
    for (i=body_position; i!=strlen(response_payload); i++) {
        if (response_payload[i] != ';' && response_payload[i] != '\n' && response_payload[i] != '\r') {
            days[i-body_position] = response_payload[i];
        }
        else {
            days[i-body_position] = '\0';
            current_pos = i;
            break;
        }
    }

    // extract days to launch
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        if (response_payload[i] != ';' && response_payload[i] != '\n' && response_payload[i] != '\r') {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            days_to_launch = atoi(buff);
            current_pos = i;
            break;
        }
    }

    // extract hours to launch
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        if (response_payload[i] != ';' && response_payload[i] != '\n' && response_payload[i] != '\r') {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            hours_to_launch = atoi(buff);
            current_pos = i;
            break;
        }
    }

    // extract minutes to launch
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {

        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            minutes_to_launch = atoi(buff);
            current_pos = i;
            break;
        }
    }

    // extract date (year)
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        printf("Date processing: %c\n", response_payload[i]);
        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            launch_year = atoi(buff);
            printf("Converting year to int: '%s'\n", buff);
            current_pos = i;
            break;
        }
    }

    // extract date (month)
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        printf("Date processing: %c\n", response_payload[i]);
        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            launch_month = atoi(buff);
            printf("Converting month to int: '%s'\n", buff);
            current_pos = i;
            break;
        }
    }

    // extract date (day)
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        printf("Date processing: %c\n", response_payload[i]);
        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            launch_day = atoi(buff);
            printf("Converting day to int: '%s'\n", buff);
            current_pos = i;
            break;
        }
    }

    // extract date (hour)
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        printf("Date processing: %c\n", response_payload[i]);
        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            launch_hour = atoi(buff);
            printf("Converting day to int: '%s'\n", buff);
            current_pos = i;
            break;
        }
    }

    // extract date (minute)
    buff_i = 0;
    for (i=current_pos+1; i!=strlen(response_payload); i++) {
        printf("Date processing: %c\n", response_payload[i]);
        if (response_payload[i] >= 48 && response_payload[i] <= 57) {
            buff[buff_i] = response_payload[i];
            buff_i++;
        }
        else {
            buff[buff_i] = '\0';
            launch_minute = atoi(buff);
            printf("Converting day to int: '%s'\n", buff);
            current_pos = i;
            break;
        }
    }

    printf("-> parser -> Company: '%s', days '%d', hours '%d', minutes '%d'\n", days, days_to_launch, hours_to_launch, minutes_to_launch);
    printf("-> parser -> Launch date: %d-%d-%d %d:%d\n", launch_year, launch_month, launch_day, launch_hour, launch_minute);

    long seconds_to_launch = (days_to_launch * 24 * 60 * 60) + (hours_to_launch * 60 * 60) + (minutes_to_launch * 60);
    timer_seconds = seconds_to_launch;

    is_extracted = 1;
    printf("-> parser -> completed.\n");
}