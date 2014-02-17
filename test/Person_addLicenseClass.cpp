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
		std::unique_ptr<odb::database> db( new Database( "Person_addLicenseClass.test.db", true ) );

		// a set storing all license classes we want to add
		std::unordered_set< std::shared_ptr<LicenseClass> > licenses;
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "A", "Alubber" ) ) );
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "B", "Blubber" ) ) );
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "C", "Clubber" ) ) );

		// we will add all licenses to one person
		std::shared_ptr<Person> p( new Person( "John", Date(1,1,1969) ) );
		for( const std::shared_ptr<LicenseClass> & l : licenses )
			assert( p->addLicenseClass( l ) );

		// for accessing the stored person by id
		std::string john_id;

		// store person and license classes
		{
			odb::transaction t( db->begin() );
			for( const std::shared_ptr<LicenseClass> & l : licenses )
			{
				db->persist( l );
				std::cout << "Stored " << l->getAbbreviation() << "\n";
			}
			john_id = db->persist( p );
			t.commit();
		}

		// retrieve person and check if all license classes could be loaded
		{
			odb::transaction t( db->begin() );
			std::shared_ptr<Person> john( db->load<Person>( john_id ) );
			assert( john.use_count() != 0 ); // assert that the person could be loaded
			// cycle through each license class and remove it from the original set
			for( const std::shared_ptr<LicenseClass> & ld : john->getLicenseClasses() )
			{
				bool storedIdentical = false;
				for( const std::shared_ptr<LicenseClass> & ll : licenses )
				{
					if( ll->getAbbreviation() == ld->getAbbreviation() && ll->getDescription() == ld->getDescription() )
					{
						storedIdentical = true;
						licenses.erase( ll );
						break;
					}
				}
				assert( storedIdentical );
				std::cout << "Retrieved " << ld->getAbbreviation() << "\n";
			}
			t.commit();
		}
		// the set should be empty if every license class could be retrieved
		assert( licenses.size() == 0 );
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
