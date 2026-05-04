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
	CDate operator+(int days) const;
	friend CDate operator+(int days, const CDate& date);

	CDate operator-(int days) const;
	int operator-(const CDate& other) const;
	CDate& operator+=(int days);
	CDate& operator-=(int days);


	bool operator==(const CDate& other) const;
	bool operator!=(const CDate& other) const;
	bool operator<(const CDate& other) const;
	bool operator>(const CDate& other) const;
	bool operator<=(const CDate& other) const;
	bool operator>=(const CDate& other) const;

private:
	int m_timestamp = 0;
};