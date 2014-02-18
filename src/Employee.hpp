#ifndef _EMPLOYEE_INCLUDED_
#define _EMPLOYEE_INCLUDED_


#include "Person.hpp"

#include <string>

#include <odb/core.hxx>


class Branch;


#pragma db object
class Employee : public Person
{
public:
	Employee( const std::string & name, const Date & dateOfBirth ) : Person( name, dateOfBirth ) {}

	int getSalary() const { return this->salary; }
	void setSalary( int salary ) { this->salary = salary; }

	const std::shared_ptr<Employee> & getSupervisor() const { return this->supervisor; }
	void setSupervisor( std::shared_ptr<Employee> & supervisor ) { this->supervisor = supervisor; }

	const std::shared_ptr<Branch> & getBranch() const { return this->branch; }

private:
	friend class odb::access;
	friend class Branch;
	Employee() : Person() {}

	int salary = 0;

	std::shared_ptr<Employee> supervisor;

	std::shared_ptr<Branch> branch;
};


#include "Branch.hpp"


#endif
