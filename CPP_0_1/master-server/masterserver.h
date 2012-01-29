#ifndef OA_MASTERSERVER_H_INCLUDED
#define OA_MASTERSERVER_H_INCLUDED

#include <string>
#include "RakPeerInterface.h"
#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include "GetTime.h"
#include "ArrayList.h"
#include "NetworkTypes.h"
#include "StringCompressor.h"

#include "mysqldb.h"

#include <tinyxml.h>


// IP, Port, Ping - case sensitive!
#define NUMBER_OF_DEFAULT_MASTER_SERVER_KEYS 3
// If we ping NUMBER_OF_MISSED_PINGS_TO_DROP without ever a response, that server is dropped from the list.
// This includes the last ping, so actually NUMBER_OF_MISSED_PINGS_TO_DROP-1 would be truly missed
#define NUMBER_OF_MISSED_PINGS_TO_DROP 4
// KEEP_ALIVE_PING_FREQUENCY is how often to ping servers to make sure they are active
#define KEEP_ALIVE_PING_FREQUENCY 20000
// How many ms must pass per connection before we check average bytes for a flood attack
#define FLOOD_ATTACK_CHECK_DELAY 5000
// How many bytes per ms someone has to send on average before they are banned.
#define FLOOD_ATTACK_BYTES_PER_MS 2.0f

struct GameServerRule
{
	GameServerRule();
	~GameServerRule();

	char *key;
	// stringValue and intValue are mutually exclusive
	char *stringValue;
	int intValue;
};

struct GameServer
{
	GameServer();
	~GameServer();
	void Clear(void);
	bool FindKey(char *key);
	int keyIndex;
	int numberOfKeysFound;
	unsigned lastUpdateTime;
	PlayerID connectionIdentifier; // The game server
	PlayerID originationId; // Only used by the server - the master client PlayerID
	int failedPingResponses;
	unsigned int nextPingTime;

	unsigned int mySQLListID;

	// When inserting rules, don't forget that IP and ping should always be added.
	// These are required for any game server
	BasicDataStructures::List<GameServerRule*> serverRules;
};

struct oaMasterUser
{
	std::string user;
	std::string password;
};

// This struct is for internal use.
// It represents a list of game servers.
struct GameServerList
{
public:
	GameServerList();
	~GameServerList();
	void Clear(void);
	void SortOnKey(char *key, bool ascending);
	void QuickSort(int low, int high, bool ascending);
	int Partition(int low, int high, bool ascending);
	int GetIndexByPlayerID(PlayerID playerID);

	BasicDataStructures::List<GameServer*> serverList;
};

class oaMasterServer
{
	public:
		oaMasterServer();
		~oaMasterServer();
		void run(void);

	private:
		TiXmlDocument* config;
		oaDBInterface mDB;
		void exportRulesToMySQL(GameServer*);
		void HandleDelistServer(Packet *packet);
		void HandleQuery(Packet *packet);
		void HandleUpdateServer(Packet *packet);
		void OnModifiedPacket(void);
		void HandleRelayedConnectionNotification(Packet *packet);

		RakPeerInterface *rakPeer;
		GameServerList gameServerList;

		GameServer* UpdateServerList(GameServer *gameServer, bool deleteSingleRules, bool *newServerAdded);
		void UpdateServer(GameServer *destination, GameServer *source, bool deleteSingleRules);
		void HandleRegisterUser(Packet *packet);
		void AddDefaultRulesToServer(GameServer *gameServer, PlayerID playerID);
		oaMasterUser DeserializeUser(RakNet::BitStream *inputBitStream);
		void SerializeUser(oaMasterUser user, RakNet::BitStream *outputBitStream);
		GameServer* DeserializeServer(RakNet::BitStream *inputBitStream);
		void SerializeServer(GameServer *gameServer, RakNet::BitStream *outputBitStream);
		GameServerRule *DeserializeRule(RakNet::BitStream *inputBitStream);
		void DeserializePlayerID(PlayerID *playerID, RakNet::BitStream *inputBitStream);
		void SerializeRule(GameServerRule *gameServerRule, RakNet::BitStream *outputBitStream);
		void SerializePlayerID(PlayerID *playerID, RakNet::BitStream *outputBitStream);
		bool RemoveServerRule(GameServer *gameServer, char *ruleIdentifier);
		bool UpdateServerRule(GameServer *gameServer, char *ruleIdentifier, char *stringData, int intData);
		void HandlePong(Packet *packet);
		bool IsReservedRuleIdentifier(char *ruleIdentifier);
		void ClearServerList(void);
		unsigned int GetServerListSize(void);
		int GetServerListRuleAsInt(int serverIndex, char *ruleIdentifier, bool *identifierFound);
		const char* GetServerListRuleAsString(int serverIndex, char *ruleIdentifier, bool *identifierFound);
		void SortServerListOnKey(char *ruleIdentifier, bool ascending);

};

#endif /* OA_MASTERSERVER_H_INCLUDED */
