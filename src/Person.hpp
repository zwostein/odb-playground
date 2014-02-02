#ifndef _PERSON_INCLUDED_
#define _PERSON_INCLUDED_

#include "Customer.hpp"
#include "DateOfBirth.hpp"

#include <string>
#include <odb/core.hxx>


#pragma db object
class Person : public Customer
{
public:
	Person( const std::string & name, const DateOfBirth & dateOfBirth ) : Customer( name ), dateOfBirth( dateOfBirth ) {}

	DateOfBirth getDateOfBirth() const { return dateOfBirth; }
	void setDateOfBirth( const DateOfBirth & dateOfBirth ) { this->dateOfBirth = dateOfBirth; }

	int getAge() { return this->dateOfBirth.getAge(); }

private:
	friend class odb::access;
	Person() : Customer(), dateOfBirth(DateOfBirth::now()) {}

	DateOfBirth dateOfBirth;
};


#endif
