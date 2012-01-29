#include "mysqldb.h"

oaDBInterface::oaDBInterface() : mCon(mysqlpp::use_exceptions)
{
//	mCon.connect("openanno", "localhost", "root", "blablub");
}

void oaDBInterface::connect(std::string host, std::string user, std::string pass, std::string db)
{
	mCon.connect(db.c_str(), host.c_str(), user.c_str(), pass.c_str());
}

oaDBInterface::~oaDBInterface()
{
	if (mCon.connected())
		mCon.close();
}

mysqlpp::Result oaDBInterface::query(std::string querystring)
{
#ifndef NDEBUG
	std::cout << "DEBUG: Executing query: " << querystring << std::endl;
#endif
	mysqlpp::Query query = mCon.query();
	query << querystring;
	return query.store();
}

std::vector<std::string> oaDBInterface::getGameList()
{
	mysqlpp::Result res = query("SELECT * FROM games");
#ifndef NDEBUG
	std::cout << "DEBUG: Query for running games returned " << res.size() << " records." << std::endl;
#endif
	std::vector<std::string> resVector;
	for (mysqlpp::Result::iterator i = res.begin(); i != res.end(); i++)
	{
		mysqlpp::Row row = *i;
#ifndef NDEBUG
		std::cout << "DEBUG: Game ID " << row["ID"] << " has Server " << row["host"] << std::endl;
#endif
		std::string tStr = row[1].get_string();
		resVector.push_back(tStr);
	}
#ifndef NDEBUG
	std::cout << "DEBUG: Request successful." << std::endl;
#endif
	return resVector;
}

void oaDBInterface::deleteByIP(std::string IP)
{
	mysqlpp::Query query = mCon.query();
	query << "DELETE FROM games WHERE host = " << mysqlpp::quote << IP;
	query.execute();
}

void oaDBInterface::clearServerList()
{
	mysqlpp::Query query = mCon.query();
	query << "DELETE FROM games";
	query.execute();
}

unsigned int oaDBInterface::addNewServer(std::string IP)
{
	mysqlpp::Query query = mCon.query();
	query << "INSERT INTO games SET host = " << mysqlpp::quote << IP;
	mysqlpp::ResNSel result = query.execute();
	return result.insert_id;
}

void oaDBInterface::updateServerInfos(unsigned int ID, std::string name, std::string location, unsigned short port, unsigned short ping)
{
	mysqlpp::Query query = mCon.query();
	query << "UPDATE games SET  name = " << mysqlpp::quote << name << ", location = " << mysqlpp::quote << location << ", ping = " << mysqlpp::quote << ping;
	query.execute();
}

bool oaDBInterface::registerUser(std::string username, std::string password)
{
    try
    {
      mysqlpp::Query query = mCon.query();
      query << "INSERT INTO users SET username = " << mysqlpp::quote << username << ", password = " << mysqlpp::quote << password;
      query.execute();
    }
    catch (mysqlpp::Exception exp)
    {
      return false;
    }
    return true;
}
