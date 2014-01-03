cmake_minimum_required( VERSION 2.8 )
project( odbcrs )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )

include( ODB )


set( odbcrs_headers
	Person.hpp
	Customer.hpp
)

set( odbcrs_sources
	Person.cpp
	Customer.cpp
	main.cpp
)

set( odbcrs_include_dirs
	.
)

set( odbcrs_library_dirs
)

set( odbcrs_libraries
	odb
	odb-sqlite
)

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

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-unknown-pragmas" )

link_directories( ${odbcrs_library_dirs} )
include_directories( ${odbcrs_include_dirs} )

find_package( Doxygen )
if( DOXYGEN_FOUND )
	add_custom_target( doc ${DOXYGEN_EXECUTABLE} Doxyfile )
	set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc/html;doc/latex;" )
else( DOXYGEN_FOUND )
	message( "Doxygen not found - Documentation can't be generated." )
endif( DOXYGEN_FOUND )

add_executable( odbcrs ${odbcrs_sources} ${ODB_GENERATED_SOURCES} )
target_link_libraries( odbcrs ${odbcrs_libraries} )
