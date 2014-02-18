#include <iostream>
#include <memory>
#include <cassert>
#include <unordered_set>

#include <odb/transaction.hxx>

#include "Branch.odb.hpp"
#include "Branch.hpp"
#include "Employee.hpp"
#include "Database.hpp"


int main()
{
	try
	{
		odb::session s; // this keeps shared pointers consistent after reloading them from the db (turns on object cache)
		std::unique_ptr<odb::database> db( new Database( "Branch_addEmployee.test.db", true ) );

		// a set storing all employees we want to add
		std::unordered_set< std::shared_ptr<Employee> > employees;
		employees.insert( std::shared_ptr<Employee>( new Employee( "A", Date(1,1,1967) ) ) );
		employees.insert( std::shared_ptr<Employee>( new Employee( "B", Date(2,1,1968) ) ) );
		employees.insert( std::shared_ptr<Employee>( new Employee( "C", Date(3,1,1969) ) ) );

		// we will add all employees to one branch
		std::shared_ptr<Branch> b( new Branch( "Main", Address() ) );
		for( const std::shared_ptr<Employee> & e : employees )
		{
			assert( e->getBranch() == nullptr );
			assert( b->addEmployee( e ) );
			assert( e->getBranch() == b );
		}

		// try adding employees twice
		for( const std::shared_ptr<Employee> & e : employees )
		{
			assert( e->getBranch() == b );
			assert( !b->addEmployee( e ) );
			assert( e->getBranch() == b );
		}

		// for accessing the stored branch by id
		std::string branch_id;

		// store branch and employees
		{
			odb::transaction t( db->begin() );
			for( const std::shared_ptr<Employee> & e : employees )
			{
				db->persist( e );
				std::cout << "Stored " << e->getName() << "\n";
			}
			branch_id = db->persist( b );
			t.commit();
		}

		// retrieve branch and check if all employees can be loaded
		{
			odb::transaction t( db->begin() );
			std::shared_ptr<Branch> b( db->load<Branch>( branch_id ) );
			assert( b ); // assert that the branch could be loaded
			// cycle through each employee and remove it from the original set
			for( const odb::lazy_weak_ptr<Employee> & we : b->getEmployees() )
			{
				const std::shared_ptr<Employee> e = we.load();
				assert( e );
				bool storedIdentical = false;
				for( const std::shared_ptr<Employee> & el : employees )
				{
					if( el->getName() == e->getName() && el->getBranch() == e->getBranch() )
					{
						storedIdentical = true;
						employees.erase( el );
						break;
					}
				}
				assert( storedIdentical );
				std::cout << "Retrieved " << e->getName() << "\n";
			}
			t.commit();
		}
		// the set should be empty if every employee could be retrieved
		assert( employees.size() == 0 );
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
