#include "DateOfBirth.odb.hpp"
#include "DateOfBirth.hpp"

#include <ctime>


int DateOfBirth::getAge() const
{
	DateOfBirth now = DateOfBirth::now();
	int age = now.getYear() - this->getYear();
	if( now.getMonth() < this->getMonth() || ( now.getMonth() == this->getMonth() && now.getDay() < this->getDay() ) )
		age = age -1;
	return 0;
}


DateOfBirth DateOfBirth::now()
{
	time_t t = std::time( nullptr );
	struct std::tm * now = std::localtime( &t );
	return DateOfBirth( 1900 + now->tm_year, 1 + now->tm_mon, now->tm_mday );
}
