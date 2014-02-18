#ifndef _BRANCH_INCLUDED_
#define _BRANCH_INCLUDED_


#include "Address.hpp"
#include "Employee.hpp"

#include <string>
#include <vector>

#include <odb/lazy-ptr.hxx>
#include <odb/core.hxx>


#pragma db object
class Branch : public std::enable_shared_from_this<Branch>
{
public:
	Branch( const std::string & name, const Address & address ) : name(name), address(address) {}

	const std::string & getName() const { return this->name; }
	const Address & getAddress() const { return this->address; }

	const std::vector< odb::lazy_weak_ptr<Employee> > & getEmployees() { return employees; }

	bool addEmployee( std::shared_ptr<Employee> e );
	bool removeEmployee( std::shared_ptr<Employee> e );

private:
	friend class odb::access;
	Branch() {}
	std::vector< odb::lazy_weak_ptr<Employee> >::iterator findEmployee( const std::shared_ptr<Employee> & e );

	#pragma db id
	std::string name;

	Address address;

	#pragma db value_not_null inverse(branch)
	std::vector< odb::lazy_weak_ptr<Employee> > employees;
};


#endif
