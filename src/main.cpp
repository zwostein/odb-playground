#include <iostream>
#include <memory>
#include <odb/transaction.hxx>
#include "Person.hpp"
#include "Person.odb.hpp"
#include "Database.hpp"


int main()
{
	try
	{
		std::auto_ptr<odb::database> db( new Database( "test.db" ) );

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
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
