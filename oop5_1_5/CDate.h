#include <iosfwd>
#include <sstream>


// Месяц
enum class Month
{
	JANUARY = 1, FEBRUARY, MARCH, APRIL,
	MAY, JUNE, JULY, AUGUST, SEPTEMBER,
	OCTOBER, NOVEMBER, DECEMBER
};

// День недели
enum class WeekDay
{
	SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY,
	THURSDAY, FRIDAY, SATURDAY
};

// Дата в формате день-месяц-год. Год в диапазоне от 1970 до 9999
class CDate
{
public:
	// инициализируем дату заданными днем, месяцем и годом.
	// примечание: год >= 1970
	CDate(unsigned day, Month month, unsigned year);

	// инициализируем дату количеством дней, прошедших после 1 января 1970 года
	// например, 2 = 3 января 1970, 32 = 2 февраля 1970 года и т.д.
	explicit CDate(unsigned timestamp);

	// Конструктор по умолчанию. Создаёт дату, равную 1 января 1970.
	CDate();

	// возвращает день месяца (от 1 до 31)
	unsigned GetDay()const;

	// возвращает месяц
	Month GetMonth()const;

	// возвращает год
	unsigned GetYear()const;

	// возвращает день недели
	WeekDay GetWeekDay()const;


	CDate& operator++();
	CDate operator++(int);
	CDate& operator--();
	CDate operator--(int);
	CDate operator+(int days)const;
	CDate operator-(int days)const;
	CDate& operator+=(int days);
	CDate& operator-=(int days);

	int operator-(const CDate& other) const;
private:
	int m_timestamp = 0;
};

CDate operator+(int days, const CDate& date);

std::ostream& operator<<(std::ostream& out, const CDate& date);

std::istream& operator>>(std::istream& in, CDate& date);

bool operator==(const CDate& a, const CDate& b);
bool operator!=(const CDate& a, const CDate& b);

bool operator<(const CDate& a, const CDate& b);
bool operator>(const CDate& a, const CDate& b);

bool operator<=(const CDate& a, const CDate& b);
bool operator>=(const CDate& a, const CDate& b);