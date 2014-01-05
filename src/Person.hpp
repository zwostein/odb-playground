#ifndef _PERSON_INCLUDED_
#define _PERSON_INCLUDED_

#include <string>
#include <odb/core.hxx>
#include "Customer.hpp"

#pragma db object
class Person : public Customer
{
public:
	Person( const std::string & name ) : Customer( name ) {}
	Person( const std::string & name, const unsigned short & age ) : Customer( name ), age(age) {}

	const unsigned short & getAge() { return this->age; }
	void setAge( const unsigned short & age ) { this->age = age; }

private:
	friend class odb::access;
	Person() {}

	unsigned short age = 0;
};

#endif
