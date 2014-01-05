cmake_minimum_required( VERSION 2.8 )
project( odbcrs )
enable_testing()
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )

set( odbcrs_headers
	src/Person.hpp
	src/Customer.hpp
	src/Database.hpp
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
	OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)

set( odbcrs_sources
	src/Person.cpp
	src/Customer.cpp
	src/Database.cpp
	${ODB_GENERATED_SOURCES}
)

set( odbcrs_include_dirs
	src
)

set( odbcrs_library_dirs
)

set( odbcrs_libraries
	odb
	odb-sqlite
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


add_executable( odbcrs src/main.cpp ${odbcrs_sources} )
target_link_libraries( odbcrs ${odbcrs_libraries} )


add_executable( PersonTest test/PersonTest.cpp ${odbcrs_sources} )
target_link_libraries( PersonTest ${odbcrs_libraries} )
add_test( PersonTest PersonTest )
