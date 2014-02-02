#ifndef _DATEOFBIRTH_INCLUDED_
#define _DATEOFBIRTH_INCLUDED_

#include <string>
#include <odb/core.hxx>

#pragma db value
class DateOfBirth
{
public:
	DateOfBirth( const int & year, const int & month, const int & day ) : year(year), month(month), day(day) {}

	const int & getYear() const { return this->year; }
	const int & getMonth() const { return this->month; }
	const int & getDay() const { return this->day; }
	void setYear( const int & year ) { this->year = year; }
	void setMonth( const int & month ) { this->month = month; }
	void setDay( const int & day ) { this->day = day; }

	int getAge() const;

	static DateOfBirth now();

private:
	friend class odb::access;
	DateOfBirth() {}

	int year;
	int month;
	int day;
};

#endif
