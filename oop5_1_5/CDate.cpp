//
// Created by  Daniil on 25.04.2026.
//

#include "CDate.h"
#include <stdexcept>

constexpr int MIN_YEAR = 1970;
constexpr int MAX_YEAR = 9999;
constexpr int DAYS_IN_YEAR = 365;
constexpr int DAYS_IN_LEEP = 366;

constexpr int FIRST_DAY_WEEK = 4;

constexpr int FEBRUARY_LEEP = 29;
constexpr int FEBRUARY_YEAR = 28;


const Month MONTHS[] = {
	Month::JANUARY,
	Month::FEBRUARY,
	Month::MARCH,
	Month::APRIL,
	Month::MAY,
	Month::JUNE,
	Month::JULY,
	Month::AUGUST,
	Month::SEPTEMBER,
	Month::OCTOBER,
	Month::NOVEMBER,
	Month::DECEMBER
};

bool IsLeapYear(unsigned year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned GetMonthDays(Month month, unsigned year)
{
	switch (month)
	{
	case Month::JANUARY:
	case Month::MARCH:
	case Month::MAY:
	case Month::JULY:
	case Month::AUGUST:
	case Month::OCTOBER:
	case Month::DECEMBER:
		return 31;

	case Month::APRIL:
	case Month::JUNE:
	case Month::SEPTEMBER:
	case Month::NOVEMBER:
		return 30;

	case Month::FEBRUARY:
		return IsLeapYear(year) ? FEBRUARY_LEEP : FEBRUARY_YEAR;
	}

	throw std::invalid_argument("Invalid month");
}

bool IsValidDate(unsigned day, Month month, unsigned year)
{
	if (year < MIN_YEAR || year > MAX_YEAR)
	{
		return false;
	}
	return day >= 1 && day <= GetMonthDays(month, year);
}


 CDate::CDate()
	 : m_timestamp(0)
{
}

CDate::CDate(unsigned timestamp)
{
	m_timestamp = timestamp;
}

int DaysFromMinYearToYear(unsigned year)
{
	int days = 0;

	for (unsigned y = MIN_YEAR; y < year; ++y)
	{
		days += IsLeapYear(y) ? DAYS_IN_LEEP : DAYS_IN_YEAR;
	}

	return days;
}


int DaysFromYearStart(unsigned day, Month month, unsigned year)
{
	int days = 0;
	for (Month currentMonth : MONTHS)
	{
		if (currentMonth == month)
		{
			break;
		}
		days += GetMonthDays(currentMonth, year);
	}
	days += day - 1;
	return days;
}

void GetDateFromTimestamp(int timestamp, unsigned& day, Month& month, unsigned& year)
{
	year = MIN_YEAR;
	while (true)
	{
		int daysInYear = IsLeapYear(year) ? DAYS_IN_LEEP : DAYS_IN_YEAR;

		if (timestamp < daysInYear)
		{
			break;
		}
		timestamp -= daysInYear;
		++year;
	}

	int indMonth = 0;
	while (true)
	{
		month = MONTHS[indMonth];
		int daysInMonth = GetMonthDays(month, year);
		if (timestamp < daysInMonth)
		{
			break;
		}
		timestamp -= daysInMonth;
		indMonth++;
	}
	day = timestamp + 1;
}

CDate::CDate(unsigned day, Month month, unsigned year)
{
	if (!IsValidDate(day, month, year))
	{
		throw std::invalid_argument("Invalid date");
	}

	m_timestamp = DaysFromMinYearToYear(year) + DaysFromYearStart(day, month, year);
}

unsigned CDate::GetDay() const //DTO
{
	unsigned day;
	Month month;
	unsigned year;

	GetDateFromTimestamp(m_timestamp, day, month, year);

	return day;
}
Month CDate::GetMonth() const
{
	unsigned day;
	Month month;
	unsigned year;

	GetDateFromTimestamp(m_timestamp, day, month, year);

	return month;
}
unsigned CDate::GetYear() const
{
	unsigned day;
	Month month;
	unsigned year;

	GetDateFromTimestamp(m_timestamp, day, month, year);

	return year;
}

WeekDay CDate::GetWeekDay() const
{
	return WeekDay((FIRST_DAY_WEEK + m_timestamp) % 7);
}

CDate& CDate::operator++()
{
	++m_timestamp;
	return *this;
}

CDate CDate::operator++(int)
{
	CDate oldDate = *this;
	++(*this);
	return oldDate;
}

CDate& CDate::operator--()
{
	if (m_timestamp == 0)
	{
		throw std::out_of_range("Date out of range");
	}

	--m_timestamp;
	return *this;
}

CDate CDate::operator--(int)
{
	CDate oldDate = *this;
	--(*this);
	return oldDate;
}

CDate CDate::operator+(int days) const
{
	CDate result = *this;

	int newTimestamp = result.m_timestamp + days;

	if (newTimestamp < 0)
	{
		throw std::out_of_range("Date out of range");
	}
	//TODO верхняя
	result.m_timestamp = newTimestamp;

	return result;
}

CDate operator+(int days, const CDate& date)
{
	return date + days;
}

CDate CDate::operator-(int days) const
{
	return *this + (-days);
}

int CDate::operator-(const CDate& other) const
{
	return m_timestamp - other.m_timestamp;
}

CDate& CDate::operator+=(int days)
{
	*this = *this + days;
	return *this;
}

CDate& CDate::operator-=(int days)
{
	*this = *this - days;
	return *this;
}


bool CDate::operator==(const CDate& other) const
{
	return m_timestamp == other.m_timestamp;
}

bool CDate::operator!=(const CDate& other) const
{
	return !(*this == other);
}

bool CDate::operator<(const CDate& other) const
{
	return m_timestamp < other.m_timestamp;
}

bool CDate::operator>(const CDate& other) const
{
	return other < *this;
}

bool CDate::operator<=(const CDate& other) const
{
	return !(*this > other);
}

bool CDate::operator>=(const CDate& other) const
{
	return !(*this < other);
}