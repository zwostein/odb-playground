#include <iostream>
#include <memory>
#include <cassert>
#include <unordered_set>
#include <odb/transaction.hxx>

#include "Person.odb.hpp"
#include "Person.hpp"
#include "Database.hpp"

int main()
{
	try
	{
		std::unique_ptr<odb::database> db( new Database( "Person_StoreLoad.test.db", true ) );

		// a set storing all persons we want to add
		std::unordered_set<std::shared_ptr<Person>> persons;
		persons.insert( std::shared_ptr<Person>( new Person( "John", Date(1,1,1969) ) ) );
		persons.insert( std::shared_ptr<Person>( new Person( "Jane", Date(15,2,1969) ) ) );
		persons.insert( std::shared_ptr<Person>( new Person( "Joe", Date::now() ) ) );

		// store every person in database
		{
			odb::transaction t( db->begin() );
			for( std::shared_ptr<Person> p : persons )
			{
				db->persist( p );
				std::cout << "Stored " << p->getName() << "\n";
			}
			t.commit();
		}

		// retrieve every person and compare to original set
		{
			odb::transaction t( db->begin() );
			odb::query<Person> q;
			odb::result<Person> r( db->query<Person>( q ) );
			for( const Person & pd : r )
			{
				bool storedIdentical = false;
				for( std::shared_ptr<Person> pl : persons )
				{
					if(
						pl->getName() == pd.getName() &&
						pl->getEmail() == pd.getEmail() &&
						pl->getDateOfBirth() == pd.getDateOfBirth() &&
						pl->getRegistrationDate() == pd.getRegistrationDate()
					)
					{
						persons.erase( pl );
						storedIdentical = true;
						break;
					}
				}
				assert( storedIdentical );
				std::cout << "Retrieved " << pd.getName() << "\n";
			}
			t.commit();
		}
		// the set should be empty if every person could be retrieved
		assert( persons.size() == 0 );
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
