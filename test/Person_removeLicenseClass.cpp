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
		odb::session s; // this keeps shared pointers consistent after reloading them from the db (turns on object cache)
		std::unique_ptr<odb::database> db( new Database( "Person_removeLicenseClass.test.db", true ) );

		// a set storing all license classes we want to add
		std::unordered_set< std::shared_ptr<LicenseClass> > licenses;
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "A", "Alubber" ) ) );
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "B", "Blubber" ) ) );
		licenses.insert( std::shared_ptr<LicenseClass>( new LicenseClass( "C", "Clubber" ) ) );

		// we will add all licenses to one person
		std::shared_ptr<Person> p( new Person( "John", Date(1,1,1969) ) );
		for( const std::shared_ptr<LicenseClass> & l : licenses )
			p->addLicenseClass( l );

		// for accessing the stored person by id
		std::string john_id;

		// store person and license classes
		{
			odb::transaction t( db->begin() );
			for( const std::shared_ptr<LicenseClass> & l : licenses )
			{
				db->persist( l );
				std::cout << "Stored " << l << " - " << l->getAbbreviation() << "\n";
			}
			john_id = db->persist( p );
			t.commit();
		}

		// remove a license from the stored person one after the other
		for( auto i = licenses.begin(); i != licenses.end(); )
		{
			// retrieve the stored person and remove a license class
			{
				odb::transaction t( db->begin() );
				std::shared_ptr<Person> john( db->load<Person>( john_id ) );
				for( const std::shared_ptr<LicenseClass> & l : john->getLicenseClasses() )
				{
					std::cout << "Still persistent: " << l << " - " << l->getAbbreviation() << "\n";
				}
				assert( john->removeLicenseClass( (*i) ) );
				assert( john->getLicenseClasses().size() == licenses.size()-1 ); // should be removed locally
				db->update( john );
				t.commit();
			}
			// reload person from database and check if license class got removed
			{
				odb::transaction t( db->begin() );
				std::shared_ptr<Person> john( db->load<Person>( john_id ) );
				assert( john->getLicenseClasses().size() == licenses.size()-1 ); // should be removed in database too
				t.commit();
			}
			std::cout << "Deleting: " << (*i) << " - " << (*i)->getAbbreviation()  << "\n";
			i = licenses.erase( i );
		}
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
