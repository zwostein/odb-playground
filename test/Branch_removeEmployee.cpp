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
		std::unique_ptr<odb::database> db( new Database( "Branch_removeEmployee.test.db", true ) );

		// a set storing all employees we want to add
		std::unordered_set< std::shared_ptr<Employee> > employees;
		employees.insert( std::shared_ptr<Employee>( new Employee( "A", Date(1,1,1967) ) ) );
		employees.insert( std::shared_ptr<Employee>( new Employee( "B", Date(2,1,1968) ) ) );
		employees.insert( std::shared_ptr<Employee>( new Employee( "C", Date(3,1,1969) ) ) );

		// we will add all employees to one branch
		std::shared_ptr<Branch> b( new Branch( "Main", Address() ) );
		for( const std::shared_ptr<Employee> & e : employees )
			assert( b->addEmployee( e ) );

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

		// remove an employee from the stored branch one after the other
		for( auto i = employees.begin(); i != employees.end(); )
		{
			// retrieve the stored branch and remove an employee
			{
				odb::transaction t( db->begin() );
				std::shared_ptr<Branch> b( db->load<Branch>( branch_id ) );
				for( const odb::lazy_weak_ptr<Employee> & le : b->getEmployees() )
				{
					std::cout << "Still persistent: " << le.object_id() << "\n";
				}

				assert( (*i)->getBranch() == b ); // should be connected to branch

				// remove employee
				assert( b->removeEmployee( (*i) ) );
				assert( b->getEmployees().size() == employees.size()-1 ); // should be removed locally
				assert( (*i)->getBranch() == nullptr ); // should not be connected to any branch anymore

				// remove second time (should fail)
				assert( !b->removeEmployee( (*i) ) ); // should not be able to remove twice
				assert( b->getEmployees().size() == employees.size()-1 ); // should stay the same
				assert( (*i)->getBranch() == nullptr ); // should still not be connected to any branch

				db->update( b );
				t.commit();
			}
			// reload person from database and check if employee got removed
			{
				odb::transaction t( db->begin() );
				std::shared_ptr<Branch> b( db->load<Branch>( branch_id ) );
				assert( b->getEmployees().size() == employees.size()-1 ); // should be removed in database too
				t.commit();
			}
			std::cout << "Deleting: " << (*i) << " - " << (*i)->getName()  << "\n";
			i = employees.erase( i );
		}
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
