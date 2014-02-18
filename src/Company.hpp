#ifndef _COMPANY_INCLUDED_
#define _COMPANY_INCLUDED_


#include "Customer.hpp"
#include "Person.hpp"

#include <string>
#include <odb/core.hxx>


#pragma db object
class Company : public Customer
{
public:
	Company( const std::string & name ) : Customer( name ) {}

	const std::unordered_set< std::shared_ptr<Person> > & getRepresentatives() const { return representatives; }
	bool addRepresentative( std::shared_ptr<Person> r ) { return representatives.insert( r ).second; }
	bool removeRepresentative( std::shared_ptr<Person> r ) { return representatives.erase( r ) == 1; }

protected:
	Company() : Customer() {}

private:
	friend class odb::access;

	#pragma db unordered
	std::unordered_set< std::shared_ptr<Person> > representatives;
};


#endif
