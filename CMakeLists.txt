cmake_minimum_required( VERSION 2.8 )
project( odbcrs )
enable_testing()
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )


# List headers with persistent classes
set( odbcrs_headers
	src/Branch.hpp
	src/Employee.hpp
	src/Person.hpp
	src/Customer.hpp
	src/Database.hpp
	src/Date.hpp
	src/Address.hpp
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
	DEFAULT_POINTER "std::shared_ptr"
)


# List all CRS source files
set( odbcrs_sources
	src/Branch.cpp
	src/Employee.cpp
	src/Person.cpp
	src/Customer.cpp
	src/Database.cpp
	src/Date.cpp
	src/Address.cpp
	src/LicenseClass.cpp
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
function( add_crs_tests )
	foreach( testFile ${ARGN} )
		get_filename_component( testName ${testFile} NAME_WE )
		set( testTarget "test_${testName}" )
		add_executable( ${testTarget} ${testFile} ${odbcrs_sources} )
		target_link_libraries( ${testTarget} ${odbcrs_libraries} )
		add_test( ${testName} ${testTarget} )
	endforeach()
endfunction()

add_crs_tests(
	test/Person_StoreLoad.cpp
	test/Person_addLicenseClass.cpp
	test/Person_removeLicenseClass.cpp
	test/Branch_addEmployee.cpp
	test/Branch_removeEmployee.cpp
)


find_package( Doxygen )
if( DOXYGEN_FOUND )
	add_custom_target( doc ${DOXYGEN_EXECUTABLE} Doxyfile )
	set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc/html;doc/latex;" )
else( DOXYGEN_FOUND )
	message( STATUS "Doxygen not found - Documentation can't be generated." )
endif( DOXYGEN_FOUND )
