#ifndef _DATEOFBIRTH_INCLUDED_
#define _DATEOFBIRTH_INCLUDED_


#include <string>
#include <odb/core.hxx>


#pragma db value
class Date
{
public:
	static Date now();

	Date( const int & day, const int & month, const int & year ) : year(year), month(month), day(day) {}
	Date() { *this = Date::now(); }

	const int & getYear() const { return this->year; }
	const int & getMonth() const { return this->month; }
	const int & getDay() const { return this->day; }
	void setYear( const int & year ) { this->year = year; }
	void setMonth( const int & month ) { this->month = month; }
	void setDay( const int & day ) { this->day = day; }

	int getAge() const;

	inline bool operator==( const Date & other ) const { return this->day == other.day && this->month == other.month && this->year == other.year;}

private:
	friend class odb::access;

	int year;
	int month;
	int day;
};

#endif
