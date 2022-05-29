#include "foundation.h"

void Foundation::recalculateModificationTime(Date& date)
{
	auto now = std::chrono::system_clock::now();
	auto nowT = std::chrono::system_clock::to_time_t(now);
	auto nowC = std::localtime(&nowT);

	date.setDay(nowC->tm_mday);
	date.setMonth(nowC->tm_mon + 1);
	date.setYear(nowC->tm_year + 1900);

	date.setHours(nowC->tm_hour);
	date.setMinutes(nowC->tm_min);
	date.setSeconds(nowC->tm_sec);
}



bool Folder::createElement(spF element)
{
	element->setParent(shared_from_this());
	subF.push_back(std::move(element));
	return true;
}

