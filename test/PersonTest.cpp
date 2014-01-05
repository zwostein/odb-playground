#include <iostream>
#include <memory>
#include <cassert>
#include <odb/transaction.hxx>
#include "Person.hpp"
#include "Person.odb.hpp"
#include "Database.hpp"


int main()
{
	try
	{
		std::auto_ptr<odb::database> db( new Database( "PersonTest.db", true ) );

		std::string john_id, jane_id, joe_id;
		{
			Person john( "John", 33 );
			Person jane( "Jane", 32 );
			Person joe( "Joe" );

			odb::transaction t( db->begin() );

			john_id = db->persist( john );
			jane_id = db->persist( jane );
			joe_id = db->persist( joe );

			t.commit();
		}

		size_t personCount = 0;
		{
			odb::transaction t( db->begin() );
			odb::query<Person> q;
			odb::result<Person> r( db->query<Person>( q ) );
			for( const Person & p : r )
			{
				(void)p;
				personCount++;
			}
			t.commit();
		}
		assert( personCount == 3 );
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::exit( EXIT_FAILURE );
	}
	return 0;
}
