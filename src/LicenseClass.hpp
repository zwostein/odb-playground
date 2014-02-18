#ifndef _LICENSECLASS_INCLUDED_
#define _LICENSECLASS_INCLUDED_


#include <string>
#include <odb/core.hxx>


#pragma db object
class LicenseClass
{
public:
	LicenseClass( const std::string & abbreviation, const std::string & description ) : abbreviation( abbreviation ), description( description ) {}

	const std::string & getAbbreviation() const { return this->abbreviation; }
	const std::string & getDescription() const { return this->description; }

protected:
	LicenseClass() {}

private:
	friend class odb::access;

	#pragma db id
	std::string abbreviation;

	std::string description;
};

#endif
