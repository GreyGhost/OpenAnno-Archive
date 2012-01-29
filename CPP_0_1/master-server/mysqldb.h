#ifndef OA_MYSQLDB_H_INCLUDED
#define OA_MYSQLDB_H_INCLUDED

//#include <mysql/mysql.h>
#include <mysql++/mysql++.h>
#include <string>
#include <vector>

class oaDBInterface {
	public:
		oaDBInterface();
		~oaDBInterface();
		void connect(std::string host, std::string user, std::string pass, std::string db);
		std::vector<std::string> getGameList(void);
		unsigned int addNewServer(std::string server);
		void deleteByIP(std::string IP);
		void clearServerList(void);
		void updateServerInfos(unsigned int ID, std::string name, std::string location, unsigned short port, unsigned short ping);
		bool registerUser(std::string username, std::string password);

	private:
		mysqlpp::Connection mCon;
		mysqlpp::Result query(std::string querystring);
};

#endif /* OA_MYSQLDB_H_INCLUDED */
