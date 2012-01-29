#include <iostream>
#include <string>

#include "mconfig.h"
#include "masterserver.h"
#include "mysqldb.h"

int main(int argc, char* argv[])
{
	try
	{
		oaMasterServer masterserver;
		masterserver.run();
	}
	catch (std::string error)
	{
		std::cerr << "Error: " << error << std::endl;
		return -1;
	}
	catch (const char* error)
	{
		std::cerr << "Error: " << error << std::endl;
		return -1;
	}
	catch (mysqlpp::BadQuery error)
	{
		std::cerr << "Error: " << error.what() << std::endl;
		return -2;
	}
	catch (mysqlpp::BadConversion error)
	{
		std::cerr << "Error: Tried to convert \"" << error.data << "\" to a \"" << error.type_name << "\"." << std::endl;
		return -3;
	}
	catch (mysqlpp::Exception error)
	{
		std::cerr << "Error: " << error.what() << std::endl;
		return -4;
	}
	return 0;
}
