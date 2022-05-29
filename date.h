#ifndef __DATE_H__
#define __DATE_H__

#include <string>

class Date
{
	int day;
	int month;
	int year;

	int hours;
	int minutes;
	int seconds;
public:
	Date() : day(-1), month(-1), year(-1), hours(-1), minutes(-1), seconds(-1) {}
	Date(int _d, int _m, int _y, int _h, int _min, int _s) : day(_d), month(_m), year(_y), hours(_h), minutes(_min), seconds(_s) {}

	void setDay(int _d) { day = _d; }
	void setMonth(int _m) { month = _m; }
	void setYear(int _y) { year = _y; }

	void setHours(int _h) { hours = _h; }
	void setMinutes(int _m) { minutes = _m; }
	void setSeconds(int _s) { seconds = _s; }

	int getDay() const { return day; }
	int getMonth() const { return month; }
	int getYear() const { return year; }

	auto getDate() const {
		return std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year);
	}

	auto getTime() const {
		return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
	}

	int getHour() const { return hours; }
	int getMinute() const { return minutes; }
	int getSeconds() const { return seconds; }
};
#endif __DATE_H__

