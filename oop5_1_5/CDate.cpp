//
// Created by  Daniil on 25.04.2026.
//


/*
bool operator<(const DateDTO& other) const
{
	return std::tie(year, month, day) <
		   std::tie(other.year, other.month, other.day);
}
*/



#include "CDate.h"
#include <stdexcept>
#include <tuple>

constexpr int MIN_YEAR = 1970;
constexpr int MAX_YEAR = 9999;
constexpr int DAYS_IN_YEAR = 365;
constexpr int DAYS_IN_LEAP = 366;

constexpr int FIRST_DAY_WEEK = 4;

constexpr int FEBRUARY_LEAP = 29;
constexpr int FEBRUARY_YEAR = 28;

struct DateDTO
{
	unsigned day;
	Month month;
	unsigned year;

};

bool IsLeapYear(unsigned year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned GetDaysInMonths(Month month, unsigned year)
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
		return IsLeapYear(year) ? FEBRUARY_LEAP : FEBRUARY_YEAR;
	}

	throw std::invalid_argument("Invalid month");
}

unsigned GetDaysBeforeMonth(Month month, unsigned year)
{
	unsigned days = 0;

	for (int m = 1; m < static_cast<int>(month); ++m)
	{
		days += GetDaysInMonths(static_cast<Month>(m), year);
	}

	return days;
}

bool IsValidDate(unsigned day, Month month, unsigned year)
{
	int monthNumber = static_cast<int>(month);

	if (year < MIN_YEAR || year > MAX_YEAR || monthNumber < 1 || monthNumber > 12)
	{
		return false;
	}

	return day >= 1 && day <= GetDaysInMonths(month, year);
}

int GetDaysFromYear(unsigned year)
{
	if (year < MIN_YEAR || year > MAX_YEAR)
	{
		throw std::invalid_argument("Invalid year");
	}

	if (IsLeapYear(year))
	{
		return DAYS_IN_LEAP;
	}
	return DAYS_IN_YEAR;

}

int CountLeapYearsBefore(unsigned year)
{
	return (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
}


int GetDaysFromMinYearToYears(unsigned year)
{
	if (year <= MIN_YEAR)
	{
		return 0;
	}

	int years = year - MIN_YEAR;

	int leapYears = CountLeapYearsBefore(year) - CountLeapYearsBefore(MIN_YEAR);

	return years * DAYS_IN_YEAR + leapYears ;
}

int GetDaysFromDate(unsigned day, Month month, unsigned year)
{
	if (!IsValidDate(day, month, year))
	{
		throw std::invalid_argument("Invalid date");
	}

	return GetDaysFromMinYearToYears(year) + GetDaysBeforeMonth(month, year) + day - 1;
}


CDate::CDate(unsigned day, Month month, unsigned year)
{
	m_timestamp = GetDaysFromDate(day, month, year);
}

 CDate::CDate()
{
	m_timestamp = 0;
}

CDate::CDate(unsigned timestamp)
{
	if (timestamp > GetDaysFromDate(31, Month::DECEMBER, MAX_YEAR))
	{
		throw std::invalid_argument("Invalid date");
	}
	m_timestamp = timestamp;
}



DateDTO GetDateFromTimestamp(int timestamp)
{
	if (timestamp < 0 || timestamp > GetDaysFromDate(31, Month::DECEMBER, MAX_YEAR))
	{
		throw std::invalid_argument("Invalid timestamp");
	}

	unsigned year = MIN_YEAR + timestamp / DAYS_IN_YEAR;

	while (GetDaysFromMinYearToYears(year + 1) <= timestamp)
	{
		++year;
	}

	while (GetDaysFromMinYearToYears(year) > timestamp)
	{
		--year;
	}

	int dayOfYear = timestamp - GetDaysFromMinYearToYears(year);

	int monthNumber = 1;
	while (dayOfYear >= GetDaysInMonths(static_cast<Month>(monthNumber), year))
	{
		dayOfYear -= GetDaysInMonths(static_cast<Month>(monthNumber), year);
		++monthNumber;
	}

	return DateDTO{
		static_cast<unsigned>(dayOfYear + 1),
		static_cast<Month>(monthNumber),
		year
	};
}

unsigned CDate::GetDay() const
{
	DateDTO date;
	date = GetDateFromTimestamp(m_timestamp);
	return date.day;
}
Month CDate::GetMonth() const
{
	DateDTO date;
	date = GetDateFromTimestamp(m_timestamp);
	return date.month;
}
unsigned CDate::GetYear() const
{
	DateDTO date;
	date = GetDateFromTimestamp(m_timestamp);
	return date.year;
}

WeekDay CDate::GetWeekDay() const
{
	return WeekDay((FIRST_DAY_WEEK + m_timestamp) % 7);
}

CDate& CDate::operator++()
{
	if (m_timestamp < 0)
	{
		return *this;
	}
	if (m_timestamp >= GetDaysFromDate(31, Month::DECEMBER, MAX_YEAR))
	{
		m_timestamp = -1;
	}
	else
	{
		++m_timestamp;
	}
	return *this;
}

CDate CDate::operator++(int)
{
	CDate oldDate = *this;
	++(*this);
	return oldDate;
}

CDate& CDate::operator--() ///?
{
	if (m_timestamp <= GetDaysFromDate(1, Month::JANUARY, MIN_YEAR))
	{
		m_timestamp = -1;
	}
	else
	{
		--m_timestamp;
	}
	return *this;
}

CDate CDate::operator--(int)
{
	CDate oldDate = *this;
	--(*this);
	return oldDate;
}

CDate CDate::operator+(int days) const  // ??? а если преаращать в минус
{
	if (m_timestamp < 0)
	{
		return *this;
	}

	int newTimestamp = m_timestamp + days;

	if (newTimestamp < 0 || newTimestamp > GetDaysFromDate(31, Month::DECEMBER, MAX_YEAR))
	{
		CDate result;
		result.m_timestamp = -1;
		return result;
	}

	return CDate(static_cast<unsigned>(newTimestamp));
}

CDate CDate::operator-(int days) const
{
	return *this + (-days);
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


CDate operator+(int days, const CDate& date)
{
	return date + days;
}

int CDate::operator-(const CDate& other) const
{
	return m_timestamp - other.m_timestamp;
}



std::ostream& operator<<(std::ostream& out, const CDate& date)
{
	unsigned day, month, year;

	try
	{
		day = date.GetDay();
		month = static_cast<unsigned>(date.GetMonth());
		year = date.GetYear();
	}
	catch (...)
	{
		out << "INVALID";
		return out;
	}

	if (day < 10) out << '0';
	out << day << '.';

	if (month < 10) out << '0';
	out << month << '.';

	out << year;

	return out;
}

std::istream& operator>>(std::istream& in, CDate& date)
{
	std::string input;
	in >> input;

	if (input == "INVALID")
	{
		date = CDate();
		--date;
		return in;
	}

	std::istringstream iss(input);

	unsigned day, month, year;
	char dot1, dot2;

	if (iss >> day >> dot1 >> month >> dot2 >> year &&
		dot1 == '.' && dot2 == '.')
	{
		try
		{
			date = CDate(day, static_cast<Month>(month), year);
		}
		catch (...)
		{
			in.setstate(std::ios::failbit);
		}
	}

	return in;
}

bool operator<(const CDate& a, const CDate& b)
{
	return (a - b) < 0;
}

bool operator>(const CDate& a, const CDate& b)
{
	return b < a;
}

bool operator<=(const CDate& a, const CDate& b)
{
	return !(b < a);
}

bool operator>=(const CDate& a, const CDate& b)
{
	return !(a < b);
}

bool operator==(const CDate& a, const CDate& b)
{
	return (a - b) == 0;
}

bool operator!=(const CDate& a, const CDate& b)
{
	return !(a == b);
}