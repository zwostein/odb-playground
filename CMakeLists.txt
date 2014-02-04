cmake_minimum_required( VERSION 2.8 )
project( odbcrs )
enable_testing()
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )

# List headers with persistent classes
set( odbcrs_headers
	src/Person.hpp
	src/Customer.hpp
	src/Database.hpp
	src/Date.hpp
	src/LicenseClass.hpp
)

# Run ODB compiler for all headers
odb_compile(
	HEADERS ${odbcrs_headers}
	DB sqlite
	STANDARD c++11
	GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA
	HEADER_SUFFIX ".hpp"
	INLINE_SUFFIX ".ipp"
	SOURCE_SUFFIX ".cpp"
	FILE_SUFFIX ".odb"
)

# List all CRS source files
set( odbcrs_sources
	src/Person.cpp
	src/Customer.cpp
	src/Database.cpp
	src/Date.cpp
	${ODB_GENERATED_SOURCES}
)

set( odbcrs_libraries
	odb
	odb-sqlite
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-unknown-pragmas" )

include_directories( src )

# Main executable
add_executable( odbcrs src/main.cpp ${odbcrs_sources} )
target_link_libraries( odbcrs ${odbcrs_libraries} )

# Unit tests
add_executable( test_addPersons test/addPersons.cpp ${odbcrs_sources} )
add_executable( test_addLicenseClassesToPerson test/addLicenseClassesToPerson.cpp ${odbcrs_sources} )
target_link_libraries( test_addPersons ${odbcrs_libraries} )
target_link_libraries( test_addLicenseClassesToPerson ${odbcrs_libraries} )
add_test( addPersons test_addPersons )
add_test( addLicenseClassesToPerson test_addLicenseClassesToPerson )

find_package( Doxygen )
if( DOXYGEN_FOUND )
	add_custom_target( doc ${DOXYGEN_EXECUTABLE} Doxyfile )
	set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc/html;doc/latex;" )
else( DOXYGEN_FOUND )
	message( "Doxygen not found - Documentation can't be generated." )
endif( DOXYGEN_FOUND )
