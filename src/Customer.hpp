#ifndef _CUSTOMER_INCLUDED_
#define _CUSTOMER_INCLUDED_

#include <string>
#include <odb/core.hxx>

#pragma db object
class Customer
{
public:
	Customer( const std::string & name ) : name(name) {}

	const std::string & getEmail() { return this->email; }
	void setEmail( const std::string & email ) { this->email = email; }

protected:
	Customer() {}

private:
	friend class odb::access;

	#pragma db id
	std::string name;

	std::string email;
};

#endif
