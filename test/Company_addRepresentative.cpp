#include <iostream>
#include <memory>
#include <cassert>
#include <unordered_set>

#include <odb/transaction.hxx>

#include "Company.odb.hpp"
#include "Company.hpp"
#include "Person.hpp"
#include "Database.hpp"


int main()
{
	try
	{
		odb::session s; // this keeps shared pointers consistent after reloading them from the db (turns on object cache)
		std::unique_ptr<odb::database> db( new Database( "Company_addRepresentative.test.db", true ) );

		// a set storing all representatives we want to add
		std::unordered_set< std::shared_ptr<Person> > representatives;
		representatives.insert( std::shared_ptr<Person>( new Person( "A", Date(1,1,1967) ) ) );
		representatives.insert( std::shared_ptr<Person>( new Person( "B", Date(2,1,1968) ) ) );
		representatives.insert( std::shared_ptr<Person>( new Person( "C", Date(3,1,1969) ) ) );

		// we will add all representatives to one company
		std::shared_ptr<Company> c( new Company( "MyCompany" ) );
		for( const std::shared_ptr<Person> & r : representatives )
			assert( c->addRepresentative( r ) );

		// for accessing the stored company by id
		std::string company_id;

		// store company and representatives
		{
			odb::transaction t( db->begin() );
			for( const std::shared_ptr<Person> & r : representatives )
			{
				db->persist( r );
				std::cout << "Stored " << r->getName() << "\n";
			}
			company_id = db->persist( c );
			t.commit();
		}

		// retrieve company and check if all representatives could be loaded
		{
			odb::transaction t( db->begin() );
			std::shared_ptr<Company> company( db->load<Company>( company_id ) );
			assert( company ); // assert that the company could be loaded
			// cycle through each representative and remove it from the original set
			for( const std::shared_ptr<Person> & representative : company->getRepresentatives() )
			{
				bool storedIdentical = false;
				for( const std::shared_ptr<Person> & r : representatives )
				{
					if( representative == r )
					{
						storedIdentical = true;
						representatives.erase( r );
						break;
					}
				}
				assert( storedIdentical );
				std::cout << "Retrieved " << representative->getName() << "\n";
			}
			t.commit();
		}
		// the set should be empty if every representative could be retrieved
		assert( representatives.size() == 0 );
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
