#ifndef _PERSON_INCLUDED_
#define _PERSON_INCLUDED_


#include "Customer.hpp"
#include "Date.hpp"

#include <string>
#include <odb/core.hxx>


#pragma db object
class Person : public Customer
{
public:
	Person( const std::string & name, const Date & dateOfBirth ) : Customer( name ), dateOfBirth( dateOfBirth ) {}

	Date getDateOfBirth() const { return dateOfBirth; }
	int getAge() const { return this->dateOfBirth.getAge(); }

	void setDateOfBirth( const Date & dateOfBirth ) { this->dateOfBirth = dateOfBirth; }

protected:
	Person() : Customer() {}

private:
	friend class odb::access;

	Date dateOfBirth;
};


#endif
