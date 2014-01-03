#include <iostream>
#include <memory>
#include <odb/sqlite/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include "Person.hpp"
#include "Person.odb.hpp"


int main( void )
{
	try
	{
		std::auto_ptr<odb::database> db( new odb::sqlite::database (
			"test.db",
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
		);
		
		try
		{
			odb::transaction t( db->begin() );
			odb::schema_catalog::create_schema( *db, "", false );
			t.commit();
		}
		catch( const odb::exception & e )
		{
			std::cerr << "Skipped creation of database schema: " << e.what() << std::endl;
		}

		std::string john_id, jane_id, joe_id;
		{
			Person john( "John", 33 );
			Person jane( "Jane", 32 );
			Person joe( "Joe" );

			odb::transaction t( db->begin() );

			john_id = db->persist(john);
			jane_id = db->persist(jane);
			joe_id = db->persist(joe);

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
