#ifndef __WATCH_H
#define __WATCH_H

#include "stm32f10x.h"
#include <stdbool.h>
#include "DS3231.H"

#include "MENU.H"

#include "BUTTON.H"

#include "DISPLAY.H"

#include "bsp_oled.h"
#include "bsp_power.h"


typedef struct
{
	uint8_t x;
	uint8_t y;
	const uint8_t* bitmap;
	uint8_t w;
	uint8_t h;
	uint8_t offsetY;
	uint8_t val;
	uint8_t maxVal;
	bool moving;
}tickerData_t;

typedef enum
{
	MONTH_JAN = 0,
	MONTH_FEB = 1,
	MONTH_MAR = 2,
	MONTH_APR = 3,
	MONTH_MAY = 4,
	MONTH_JUN = 5,
	MONTH_JUL = 6,
	MONTH_AUG = 7,
	MONTH_SEP = 8,
	MONTH_OCT = 9,
	MONTH_NOV = 10,
	MONTH_DEC = 11
} month_t;

typedef enum
{
	DAY_MON = 0,
	DAY_TUE = 1,
	DAY_WED = 2,
	DAY_THU = 3,
	DAY_FRI = 4,
	DAY_SAT = 5,
	DAY_SUN = 6,
} day_t;

typedef struct {
	uint8_t secs;
	uint8_t mins;
	uint8_t hour;
	char ampm;
}time_s;

typedef struct {
	day_t day;
	uint8_t date;
	month_t month;
	uint8_t year;
}date_s;

typedef struct {
	time_s time;
	date_s date;
}timeDate_s;

extern timeDate_s timeDate;

void Watch_load(void);
	

void time_updata(void);

// Days
// Also see BUFFSIZE_STR_DAYS
#define STR_MON			"Mon"
#define STR_TUE			"Tue"
#define STR_WED			"Wed"
#define STR_THU			"Thu"
#define STR_FRI			"Fri"
#define STR_SAT			"Sat"
#define STR_SUN			"Sun"

// Months
// Also see BUFFSIZE_STR_MONTHS
#define STR_JAN			"Jan"
#define STR_FEB			"Feb"
#define STR_MAR			"Mar"
#define STR_APR			"Apr"
#define STR_MAY			"May"
#define STR_JUN			"Jun"
#define STR_JUL			"Jul"
#define STR_AUG			"Aug"
#define STR_SEP			"Sep"
#define STR_OCT			"Oct"
#define STR_NOV			"Nov"
#define STR_DEC			"Dec"

#define BUFFSIZE_DATE_FORMAT		((BUFFSIZE_STR_DAYS - 1) + (BUFFSIZE_STR_MONTHS - 1) + 10)
#define DATE_FORMAT ("%s %02hhu %s 20%02hhu")

#define BUFFSIZE_STR_DAYS	4
#define BUFFSIZE_STR_MONTHS	4


#endif /* __WATCH_H */

