#include "masterserver.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <string.h>

oaMasterServer::oaMasterServer()
{
	rakPeer = RakNetworkFactory::GetRakPeerInterface();
	config = new TiXmlDocument (CONFIGFILE);
	bool loadOkay = config->LoadFile();
	if (!loadOkay)
		throw "Failed to read " CONFIGFILE ".";
}	

oaMasterServer::~oaMasterServer()
{
	if (config)
	{
		delete config;
		config = NULL;
	}
	ClearServerList();
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

inline std::string _oa_getConfigString(TiXmlNode* configNode, std::string settingName)
{
	assert(configNode);
	TiXmlNode* temp = configNode->FirstChild(settingName);
	if (!temp)
		throw "The required config setting " + settingName + " was not found in " CONFIGFILE ".";
	temp = temp->FirstChild();
	if (!temp)
		throw "The required config setting " + settingName + " couldn´t be read correctly from " CONFIGFILE ".";
	return temp->ValueStr();
}

inline int 	  _oa_getConfigInteger(TiXmlNode* configNode, std::string settingName)
{
	std::string sConfigInteger = _oa_getConfigString(configNode, settingName);
	const char* cConfigInteger = sConfigInteger.c_str();
	return atoi(cConfigInteger);
}

void oaMasterServer::run()
{
	assert(config);
	TiXmlElement* root = config->RootElement();
	if (!root)
		throw "The configuration file " CONFIGFILE " couldn´t be read.";
	TiXmlNode* dbConfig = root->FirstChild("database");
	if (!dbConfig)
		throw "No suitable database configuration found in " CONFIGFILE ".";
	std::string dbHost = _oa_getConfigString(dbConfig, "host");
	std::string dbUser = _oa_getConfigString(dbConfig, "user");
	std::string dbPass = _oa_getConfigString(dbConfig, "password");
	std::string db     = _oa_getConfigString(dbConfig, "database");
	mDB.connect(dbHost, dbUser, dbPass, db);
	mDB.clearServerList();

	TiXmlNode* netConfig = root->FirstChild("network");
	if (!netConfig)
		throw "No suitable network configuration found in " CONFIGFILE ".";
	int maximumConnections = _oa_getConfigInteger(netConfig, "maximum-connections");
	int localPort          = _oa_getConfigInteger(netConfig, "port");
	int _threadSleepTimer  = _oa_getConfigInteger(netConfig, "thread-sleep-time");
	if (rakPeer->Initialize(maximumConnections, localPort, _threadSleepTimer)==false)
		throw "Couldn´t initialize networking layer";
	rakPeer->SetOccasionalPing(true);
	//rakPeer->StartSynchronizedRandomInteger(); // Keepalive
	rakPeer->SetMaximumIncomingConnections(maximumConnections);
	struct timeval lastUpdate;
	int result = gettimeofday(&lastUpdate, NULL);
	if (result == -1)
		perror("gettimeofday");
	while (1)
	{
		Packet *packet;
		unsigned serverIndex;
		unsigned int time;
		RakNetStatisticsStruct *rss;
		unsigned int connectionTime;
		packet = rakPeer->Receive();
	
		// TODO - should have multiple listing security
		struct timeval timeNow;
		result = gettimeofday(&timeNow, NULL);
		if (result == -1)
			perror("gettimeofday");
		if ((timeNow.tv_sec - lastUpdate.tv_sec) >= 10)
		{
			lastUpdate = timeNow;
			for (unsigned int i = 0; i < gameServerList.serverList.size(); i++)
			{
				exportRulesToMySQL(gameServerList.serverList[i]);
			}
		}
		time = RakNet::GetTime();
	
		while (packet)
		{
			// Quick and dirty flood attack security:
			// If a client has been connected for more than 5 seconds,
			// and has sent more than 1000 bytes per second on average then ban them
			rss=rakPeer->GetStatistics(packet->playerId);
			if (rss)
			{
				connectionTime=time-rss->connectionStartTime;
				if (connectionTime > FLOOD_ATTACK_CHECK_DELAY &&
					(float)(rss->bitsReceived/8) / (float) connectionTime > FLOOD_ATTACK_BYTES_PER_MS)
				{
					rakPeer->CloseConnection(packet->playerId, true,0);
#ifndef NDEBUG
					std::cout << "DEBUG: " << (char*)rakPeer->PlayerIDToDottedIP(packet->playerId) << " banned for session due to for flood attack" << std::endl;
#endif
					rakPeer->AddToBanList(rakPeer->PlayerIDToDottedIP(packet->playerId));

					// Find all servers with this IP and kill them.
					mDB.deleteByIP(rakPeer->PlayerIDToDottedIP(packet->playerId));
					serverIndex=0;
					while (serverIndex < gameServerList.serverList.size())
					{
						if (gameServerList.serverList[serverIndex]->connectionIdentifier.binaryAddress==packet->playerId.binaryAddress)
						{
							delete gameServerList.serverList[serverIndex];
							gameServerList.serverList.del(serverIndex);
						}
						else
							serverIndex++;
					}
					break;
				}
	
			}
			
			switch(packet->data[0])
			{
			case ID_NEW_INCOMING_CONNECTION:
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				break;
			case ID_CONNECTION_LOST:
				break;
			case ID_RECEIVED_STATIC_DATA:
				break;
			case ID_MODIFIED_PACKET:
				OnModifiedPacket();
				break;
			case ID_REMOTE_PORT_REFUSED:
				break;
			case ID_MASTER_REGISTER_USER:
				HandleRegisterUser(packet);
				break;
			case ID_QUERY_MASTER_SERVER:
				HandleQuery(packet);
				break;
			case ID_MASTER_SERVER_DELIST_SERVER:
				HandleDelistServer(packet);
				break;
			case ID_MASTER_SERVER_SET_SERVER:
				HandleUpdateServer(packet);
				break;
			case ID_PONG:
				HandlePong(packet);
				break;
			case ID_RELAYED_CONNECTION_NOTIFICATION:
				HandleRelayedConnectionNotification(packet);
				break;
			default:
				break;
			}
			rakPeer->DeallocatePacket(packet);
			packet = rakPeer->Receive();
		}
	
		serverIndex=0;
		while (serverIndex < gameServerList.serverList.size())
		{
			if (time >= gameServerList.serverList[serverIndex]->nextPingTime)
			{
				if (gameServerList.serverList[serverIndex]->failedPingResponses>=NUMBER_OF_MISSED_PINGS_TO_DROP)
				{
#ifndef NDEBUG				
					std::cout << "DEBUG: Deleting " << (char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier) << " for lack of ping response." << std::endl;
#endif
					mDB.deleteByIP(rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier));
					gameServerList.serverList[serverIndex]->Clear();
					delete gameServerList.serverList[serverIndex];
					gameServerList.serverList.del(serverIndex);
				}
				else
				{
//					rakPeer->Ping((char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier),
//						gameServerList.serverList[serverIndex]->connectionIdentifier.port, false);
					rakPeer->Ping(gameServerList.serverList[serverIndex]->connectionIdentifier);
					std::cout << "updating ping to " << rakPeer->GetLastPing(gameServerList.serverList[serverIndex]->connectionIdentifier) << std::endl;
					UpdateServerRule(gameServerList.serverList[serverIndex], "SPing", NULL, rakPeer->GetLastPing(gameServerList.serverList[serverIndex]->connectionIdentifier));
					gameServerList.serverList[serverIndex]->nextPingTime = time + KEEP_ALIVE_PING_FREQUENCY;
					gameServerList.serverList[serverIndex]->failedPingResponses++;
					serverIndex++;
				}
			}
			else
				serverIndex++;
		}
		usleep(50);
	}
	rakPeer->Disconnect(200);
}

void oaMasterServer::HandleRegisterUser(Packet *packet)
{
    RakNet::BitStream incomingBitStream((char*)packet->data, packet->length, false);
    incomingBitStream.IgnoreBits(8*sizeof(unsigned char));
    oaMasterUser newUser = DeserializeUser(&incomingBitStream);
    bool regSuccess = mDB.registerUser(newUser.user, newUser.password);
    if (!regSuccess)
    {
	RakNet::BitStream outgoingBitStream;
	outgoingBitStream.Write((unsigned char)ID_MASTER_SERVER_REGISTRATION_FAILED);
	rakPeer->Send(&outgoingBitStream, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, false);
    }
}

oaMasterUser oaMasterServer::DeserializeUser(RakNet::BitStream *inputBitStream)
{
  oaMasterUser newUser;
  char tmpBuffer[257];
  tmpBuffer[0] = 0;
  stringCompressor->DecodeString(tmpBuffer, 256, inputBitStream);
  newUser.user = tmpBuffer;
  tmpBuffer[0] = 0;
  stringCompressor->DecodeString(tmpBuffer, 256, inputBitStream);
  newUser.password = tmpBuffer;
  return newUser;
}

void oaMasterServer::SerializeUser(oaMasterUser user, RakNet::BitStream *outputBitStream)
{
  stringCompressor->EncodeString((char*)user.user.c_str(), 256, outputBitStream);
  stringCompressor->EncodeString((char*)user.password.c_str(), 256, outputBitStream);
}

void oaMasterServer::HandleDelistServer(Packet *packet)
{
	PlayerID serverPlayerID;
	int existingServerIndex;
	RakNet::BitStream bitStream((char*)packet->data, packet->length, false);

	bitStream.IgnoreBits(sizeof(unsigned char)*8); // Ignore the packet type enum
	bitStream.Read(serverPlayerID.port);
	serverPlayerID.binaryAddress=packet->playerId.binaryAddress;

	existingServerIndex=gameServerList.GetIndexByPlayerID(serverPlayerID);
	if (existingServerIndex>=0)
	{
		mDB.deleteByIP((char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[existingServerIndex]->connectionIdentifier));
		gameServerList.serverList[existingServerIndex]->Clear();
		delete gameServerList.serverList[existingServerIndex];
		gameServerList.serverList.del(existingServerIndex);
	}
	//else
		// Server does not already exist
}

void oaMasterServer::HandleQuery(Packet *packet)
{
	BasicDataStructures::List<GameServer*> serversWithKeysList;
	char ruleIdentifier[256];
	unsigned index, serverIndex;
	int key;
	bool queryAll;
	RakNet::BitStream outputBitStream;
	RakNet::BitStream compressedString((char*)packet->data, packet->length, false);
	compressedString.IgnoreBits(8*sizeof(unsigned char));

	queryAll=true;

	while (compressedString.GetNumberOfUnreadBits()>0)
	{
		// Generate a list of the indices of the servers that have one or more of the specified keys.
		stringCompressor->DecodeString(ruleIdentifier, 256, &compressedString);
		if (ruleIdentifier[0]==0)
			// If we fail to read the first string, queryAll remains true.
			break;
		
		queryAll=false;

		if (IsReservedRuleIdentifier(ruleIdentifier))
			continue;

		for (index=0; index < gameServerList.serverList.size(); index++)
		{
			if (gameServerList.serverList[index]->connectionIdentifier==UNASSIGNED_PLAYER_ID)
				continue;

			if (gameServerList.serverList[index]->FindKey(ruleIdentifier))
			{
				serverIndex=serversWithKeysList.getIndexOf(gameServerList.serverList[index]);
				if (serverIndex==MAX_UNSIGNED_LONG)
				{
					gameServerList.serverList[index]->numberOfKeysFound=1;
					serversWithKeysList.insert(gameServerList.serverList[index]);
				}
				else
				{
					serversWithKeysList[serverIndex]->numberOfKeysFound++;
				}
			}
		}
	}
	
	// Write the packet id
	if (queryAll)
		outputBitStream.Write((unsigned char) ID_MASTER_SERVER_SET_SERVER);
	else
		outputBitStream.Write((unsigned char) ID_MASTER_SERVER_UPDATE_SERVER);
	if (queryAll)
	{
		// Write the number of servers
		outputBitStream.WriteCompressed((unsigned short)gameServerList.serverList.size());

		for (index=0; index < gameServerList.serverList.size(); index++)
		{
			// Write the whole server
			SerializeServer(gameServerList.serverList[index], &outputBitStream);	
		}		
	}
	else
	{
		compressedString.ResetReadPointer();
		compressedString.IgnoreBits(8*sizeof(unsigned char));

		// Write the number of servers with requested keys
		outputBitStream.WriteCompressed((unsigned short)serversWithKeysList.size());

		// For each server, write the header which consists of the IP/PORT.
		// Then go through the list of requested keys and write those
		for (index=0; index < serversWithKeysList.size(); index++)
		{
			SerializePlayerID(&(serversWithKeysList[index]->connectionIdentifier), &outputBitStream);

			outputBitStream.WriteCompressed((unsigned short)serversWithKeysList[index]->numberOfKeysFound);
			while (compressedString.GetNumberOfUnreadBits()>0)
			{
				// Generate a list of the indices of the servers that have one or more of the specified keys.
				stringCompressor->DecodeString(ruleIdentifier, 256, &compressedString);
				if (ruleIdentifier[0]==0)
					break;
				if (IsReservedRuleIdentifier(ruleIdentifier))
					continue;

				serversWithKeysList[index]->FindKey(ruleIdentifier);
				key=serversWithKeysList[index]->keyIndex;
				if (key>=0)
					SerializeRule(serversWithKeysList[index]->serverRules[key], &outputBitStream);
			}
		}
	}

	rakPeer->Send(&outputBitStream, MEDIUM_PRIORITY, RELIABLE, 0, packet->playerId, false);
}

void oaMasterServer::HandleUpdateServer(Packet *packet)
{
	GameServer *gameServer;
	bool newServerAdded;
	RakNet::BitStream incomingBitStream((char*)packet->data, packet->length, false);
	incomingBitStream.IgnoreBits(8*sizeof(unsigned char));

	gameServer = DeserializeServer(&incomingBitStream);
	gameServer->connectionIdentifier.binaryAddress=packet->playerId.binaryAddress;


	UpdateServerList(gameServer, true, &newServerAdded);

	if (newServerAdded)
	{
		gameServer->originationId=packet->playerId;
	}
}

void oaMasterServer::OnModifiedPacket(void)
{
}

void oaMasterServer::HandleRelayedConnectionNotification(Packet *packet)
{
	char str[22];
	unsigned short clientGamePort, serverGamePort;
	RakNet::BitStream incomingBitStream((char*)packet->data, packet->length, false);
	incomingBitStream.IgnoreBits(8*sizeof(unsigned char));
	incomingBitStream.Read(clientGamePort);
	incomingBitStream.Read(serverGamePort);
	if (!stringCompressor->DecodeString(str, 22, &incomingBitStream))
		return;

	if (str[0]==0)
		return;

	RakNet::BitStream outgoingBitStream;
	outgoingBitStream.Write((unsigned char)ID_RELAYED_CONNECTION_NOTIFICATION);
	// Assumes the game client is on the same computer as the master client
	outgoingBitStream.Write(packet->playerId.binaryAddress); // This is the public IP, which the sender doesn't necessarily know
	outgoingBitStream.Write(clientGamePort);

	PlayerID targetID;
	rakPeer->IPToPlayerID(str, serverGamePort, &targetID);
    
	// Given the IP and port of the game system, give me the index into the game server list
	int serverIndex = gameServerList.GetIndexByPlayerID(targetID);

	if (serverIndex>=0)
	{
#ifndef NDEBUG
		std::cout << "DEBUG: ID_RELAYED_CONNECTION_NOTIFICATION sent to " << str << ":" << serverGamePort << " from " << rakPeer->PlayerIDToDottedIP(packet->playerId) << ":" << packet->playerId.port << std::endl;
#endif
		rakPeer->Send(&outgoingBitStream, HIGH_PRIORITY, RELIABLE, 0, gameServerList.serverList[serverIndex]->originationId, false);
	}
	else
	{
#ifndef NDEBUG
		std::cout << "DEBUG: ID_RELAYED_CONNECTION_NOTIFICATION not sent to " << str << ":" << serverGamePort << " from " << rakPeer->PlayerIDToDottedIP(packet->playerId) << ":" << packet->playerId.port << "." << std::endl << "Master server does not know about target system." << std::endl;
#endif
	}

	
}
#include <stdio.h>

GameServerRule::GameServerRule()
{
	key=0;
	stringValue=0;
	intValue=-1;
}
GameServerRule::~GameServerRule()
{
	if (key)
		delete [] key;
	if (stringValue)
		delete [] stringValue;
}

GameServer::GameServer()
{
	connectionIdentifier=UNASSIGNED_PLAYER_ID;
	nextPingTime=0;
	failedPingResponses=0;
	lastUpdateTime=RakNet::GetTime();
}
GameServer::~GameServer()
{
	Clear();
}
void GameServer::Clear()
{
	unsigned i;
	for (i=0; i < serverRules.size(); i++)
		delete serverRules[i];
	serverRules.clear();
}
bool GameServer::FindKey(char *key)
{
	unsigned i;
	for (i=0; i < serverRules.size(); i++)
		if (strcmp(serverRules[i]->key, key)==0)
		{
			keyIndex=i;
			return true;
		}

	keyIndex=-1;
	return false;
}

GameServerList::GameServerList()
{
}
GameServerList::~GameServerList()
{
	Clear();
}
void GameServerList::Clear(void)
{
	unsigned i;
	for (i=0; i < serverList.size(); i++)
		delete serverList[i];
	serverList.clear();
}
void GameServerList::SortOnKey(char *key, bool ascending)
{
	unsigned i;
	// Set keyindex
	for (i=0; i < serverList.size(); i++)
		serverList[i]->FindKey(key);

	QuickSort(0, serverList.size()-1,ascending);
}


void GameServerList::QuickSort(int low, int high, bool ascending)
{
	int pivot;
	if ( high > low )
	{
		pivot = Partition( low, high, ascending);
		QuickSort( low, pivot-1,ascending);
		QuickSort( pivot+1, high,ascending);
	}
}
int EQ(GameServer *left, GameServer *right)
{
	if (left->keyIndex==-1 || right->keyIndex==-1)
		return true;
	
	if (left->serverRules[left->keyIndex]->stringValue)
		return strcmp(left->serverRules[left->keyIndex]->stringValue,right->serverRules[right->keyIndex]->stringValue)==0;
	else
		return left->serverRules[left->keyIndex]->intValue == right->serverRules[right->keyIndex]->intValue;

}
int LT(GameServer *left, GameServer *right)
{
	if (left->keyIndex==-1)
		return true;
	if (right->keyIndex==-1)
		return false;

	if (left->serverRules[left->keyIndex]->stringValue)
		return strcmp(left->serverRules[left->keyIndex]->stringValue,right->serverRules[right->keyIndex]->stringValue) < 0;
	else
		return left->serverRules[left->keyIndex]->intValue < right->serverRules[right->keyIndex]->intValue;
}
int LTEQ(GameServer *left, GameServer *right)
{
	return LT(left, right) || EQ(left, right);
}
int GT(GameServer *left, GameServer *right)
{
	if (left->keyIndex==-1)
		return false;
	if (right->keyIndex==-1)
		return true;

	if (left->serverRules[left->keyIndex]->stringValue)
		return strcmp(left->serverRules[left->keyIndex]->stringValue,right->serverRules[right->keyIndex]->stringValue) > 0;
	else
		return left->serverRules[left->keyIndex]->intValue > right->serverRules[right->keyIndex]->intValue;
}
int GTEQ(GameServer *left, GameServer *right)
{
	return GT(left, right) || EQ(left, right);
}
int GameServerList::Partition(int low, int high, bool ascending)
{
	int left, right, pivot;
	GameServer *pivot_item, *temp;
	pivot_item = serverList[low];
	pivot = left = low;
	right = high;
	while ( left < right )
	{
		if (ascending)
		{
			/* Move left while item < pivot */
			while( LTEQ(serverList[left], pivot_item) && left < high) left++;
			/* Move right while item > pivot */
			while( GT(serverList[right], pivot_item) && right > 0) right--;
			if ( left < right )
			{
				temp=serverList[left];
				serverList[left]=serverList[right];
				serverList[right]=temp;
			}
		}
		else
		{
			while( GTEQ(serverList[left], pivot_item) && left < high) left++;
			while( LT(serverList[right], pivot_item) && right > 0) right--;
			if ( left < right )
			{
				temp=serverList[left];
				serverList[left]=serverList[right];
				serverList[right]=temp;
			}
		}
	}

	/* right is final position for the pivot */
	serverList[low] = serverList[right];
	serverList[right] = pivot_item;

	return right;
}

int GameServerList::GetIndexByPlayerID(PlayerID playerID)
{
	int i;
	for (i=0; i < (int)serverList.size(); i++)
	{
		if (serverList[i]->connectionIdentifier==playerID)
			return i;
	}
	return -1;
}

void oaMasterServer::ClearServerList(void)
{
	gameServerList.Clear();
	mDB.clearServerList();
}
void oaMasterServer::SortServerListOnKey(char *ruleIdentifier, bool ascending)
{
	gameServerList.SortOnKey(ruleIdentifier, ascending);
}
unsigned int oaMasterServer::GetServerListSize(void)
{
	return gameServerList.serverList.size();
}
int oaMasterServer::GetServerListRuleAsInt(int serverIndex, char *ruleIdentifier, bool *identifierFound)
{
	int keyIndex;
	if (serverIndex >= (int)gameServerList.serverList.size())
	{
		*identifierFound=false;
		return -1;
	}

	gameServerList.serverList[serverIndex]->FindKey(ruleIdentifier);
	keyIndex=gameServerList.serverList[serverIndex]->keyIndex;
	if (keyIndex==-1)
	{
		*identifierFound=false;
		return -1;
	}

	if (gameServerList.serverList[serverIndex]->serverRules[keyIndex]->stringValue)
	{
		*identifierFound=false;
		return -1;
	}

	return gameServerList.serverList[serverIndex]->serverRules[keyIndex]->intValue;
}
const char* oaMasterServer::GetServerListRuleAsString(int serverIndex, char *ruleIdentifier, bool *identifierFound)
{
	int keyIndex;
	if (serverIndex >= (int)gameServerList.serverList.size())
	{
		*identifierFound=false;
		return "serverIndex out of bounds";
	}

	gameServerList.serverList[serverIndex]->FindKey(ruleIdentifier);
	keyIndex=gameServerList.serverList[serverIndex]->keyIndex;
	if (keyIndex==-1)
	{
		*identifierFound=false;
		return "Server does not contain specified rule";
	}

	if (gameServerList.serverList[serverIndex]->serverRules[keyIndex]->stringValue==0)
	{
		*identifierFound=false;
		return "Server rule is not a string. Use GetServerListRuleAsInt";
	}

	*identifierFound=true;
	return gameServerList.serverList[serverIndex]->serverRules[keyIndex]->stringValue;
}

bool oaMasterServer::IsReservedRuleIdentifier(char *ruleIdentifier)
{
	if (strcmp(ruleIdentifier, "Ping")==0 || 
		strcmp(ruleIdentifier, "IP")==0 || 
		strcmp(ruleIdentifier, "Port")==0)
		return true;

	return false;
}

void oaMasterServer::AddDefaultRulesToServer(GameServer *gameServer, PlayerID playerID)
{
	GameServerRule *gameServerRule;

	// Every server has NUMBER_OF_DEFAULT_MASTER_SERVER_KEYS keys by default: IP, port, and ping
	gameServerRule = new GameServerRule;
	gameServerRule->key=new char[strlen("IP")+1];
	strcpy(gameServerRule->key, "IP");
	gameServerRule->stringValue=new char[22]; // Should be enough to hold an IP address
	strncpy(gameServerRule->stringValue, rakPeer->PlayerIDToDottedIP(playerID), 21);
	gameServerRule->stringValue[21]=0;
	gameServer->serverRules.insert(gameServerRule);

	gameServerRule = new GameServerRule;
	gameServerRule->key=new char[strlen("Port")+1];
	strcpy(gameServerRule->key, "Port");
	gameServerRule->intValue=playerID.port;
	gameServer->serverRules.insert(gameServerRule);

	gameServerRule = new GameServerRule;
	gameServerRule->key=new char[strlen("Ping")+1];
	strcpy(gameServerRule->key, "Ping");
	gameServerRule->intValue=9999;
	gameServer->serverRules.insert(gameServerRule);
}
void oaMasterServer::HandlePong(Packet *packet)
{
	// Find the server specified by packet
	int serverIndex;
	unsigned int pingTime;

	serverIndex=gameServerList.GetIndexByPlayerID(packet->playerId);
	if (serverIndex>=0)
	{
		gameServerList.serverList[serverIndex]->failedPingResponses=0;
		if (gameServerList.serverList[serverIndex]->FindKey("Ping"))
		{
			RakNet::BitStream ptime( (char *)packet->data+1, sizeof(unsigned int), false);
			ptime.Read(pingTime);
			gameServerList.serverList[serverIndex]->serverRules[gameServerList.serverList[serverIndex]->keyIndex]->intValue=pingTime;
			#ifdef _SHOW_MASTER_SERVER_PRINTF
			printf("Got pong. Ping=%i\n", pingTime);
			#endif
		}
#ifdef _DEBUG
		else
			// No ping key!
			assert(0);
#endif
	}
}
bool oaMasterServer::UpdateServerRule(GameServer *gameServer, char *ruleIdentifier, char *stringData, int intData)
{
	GameServerRule *gameServerRule;
	gameServer->lastUpdateTime=RakNet::GetTime();

	// Add the rule to our local server.  If it changes the local server, set a flag so we upload the
	// local server on the next update.
	if (gameServer->FindKey(ruleIdentifier))
	{
		// Is the data the same?
		if (gameServer->serverRules[gameServer->keyIndex]->stringValue)
		{
			if (stringData==0)
			{
				// No string.  Delete the string and use int data instead
				delete [] gameServer->serverRules[gameServer->keyIndex]->stringValue;
				gameServer->serverRules[gameServer->keyIndex]->stringValue=0;
				gameServer->serverRules[gameServer->keyIndex]->intValue=intData;
				return true;
			}
			else if (strcmp(gameServer->serverRules[gameServer->keyIndex]->stringValue, stringData)!=0)
			{
				// Different string
				delete [] gameServer->serverRules[gameServer->keyIndex]->stringValue;
				gameServer->serverRules[gameServer->keyIndex]->stringValue = new char [strlen(stringData)+1];
				strcpy(gameServer->serverRules[gameServer->keyIndex]->stringValue, stringData);
				return true;
			}
		}
		else
		{
			if (stringData)
			{
				// Has a string where there is currently none
				gameServer->serverRules[gameServer->keyIndex]->stringValue = new char [strlen(stringData)+1];
				strcpy(gameServer->serverRules[gameServer->keyIndex]->stringValue, stringData);
				gameServer->serverRules[gameServer->keyIndex]->intValue=-1;
				return true;
			}
			else if (gameServer->serverRules[gameServer->keyIndex]->intValue!=intData)
			{
				// Different int value
				gameServer->serverRules[gameServer->keyIndex]->intValue=intData;
				return true;
			}
		}
	}
	else
	{
		// No such key.  Add a new one.
		gameServerRule = new GameServerRule;
		gameServerRule->key=new char[strlen(ruleIdentifier)+1];
		strcpy(gameServerRule->key, ruleIdentifier);
		if (stringData)
		{
			gameServerRule->stringValue=new char[strlen(stringData)+1];
			strcpy(gameServerRule->stringValue, stringData);
		}
		else
		{
			gameServerRule->intValue=intData;
		}
		gameServer->serverRules.insert(gameServerRule);
		return true;
	}

	return false;
}
bool oaMasterServer::RemoveServerRule(GameServer *gameServer, char *ruleIdentifier)
{
	if (gameServer->FindKey(ruleIdentifier))
	{
		delete gameServer->serverRules[gameServer->keyIndex];
		gameServer->serverRules.del(gameServer->keyIndex);
		return true;
	}

	return false;
}

void oaMasterServer::SerializePlayerID(PlayerID *playerID, RakNet::BitStream *outputBitStream)
{
	outputBitStream->Write(playerID->binaryAddress);
	outputBitStream->Write(playerID->port);
}
void oaMasterServer::SerializeRule(GameServerRule *gameServerRule, RakNet::BitStream *outputBitStream)
{
	stringCompressor->EncodeString(gameServerRule->key, 256, outputBitStream);

	if (gameServerRule->stringValue)
	{
		outputBitStream->Write(true);
		stringCompressor->EncodeString(gameServerRule->stringValue, 256, outputBitStream);
	}
	else
	{
		outputBitStream->Write(false);
		outputBitStream->WriteCompressed(gameServerRule->intValue);
	}
}
void oaMasterServer::DeserializePlayerID(PlayerID *playerID, RakNet::BitStream *inputBitStream)
{
	*playerID=UNASSIGNED_PLAYER_ID;

	inputBitStream->Read(playerID->binaryAddress);
	inputBitStream->Read(playerID->port);
}
GameServerRule * oaMasterServer::DeserializeRule(RakNet::BitStream *inputBitStream)
{
	char output[256];
	bool isAString;
	GameServerRule *newRule;

	newRule = new GameServerRule;

	stringCompressor->DecodeString(output, 256, inputBitStream);
	if (output[0]==0)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return 0;
	}

	newRule->key = new char [strlen(output)+1];
	strcpy(newRule->key, output);
	if (inputBitStream->Read(isAString)==false)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return 0;
	}

	if (isAString)
	{
		stringCompressor->DecodeString(output, 256, inputBitStream);
		if (output[0]==0)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return 0;
		}
		newRule->stringValue = new char[strlen(output)+1];
		strcpy(newRule->stringValue, output);
	}
	else
	{
		if (inputBitStream->ReadCompressed(newRule->intValue)==false)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return 0;
		}
	}

	return newRule;    
}
void oaMasterServer::SerializeServer(GameServer *gameServer, RakNet::BitStream *outputBitStream)
{
	unsigned serverIndex;
	unsigned short numberOfRulesToWrite;

	numberOfRulesToWrite=0;

	// Find out how many rules to write.
	for (serverIndex=0; serverIndex < gameServer->serverRules.size(); serverIndex++)
	{
		// We don't write reserved identifiers
		if (IsReservedRuleIdentifier(gameServer->serverRules[serverIndex]->key)==false)
			numberOfRulesToWrite++;
	}

	// Write the server identifier
	SerializePlayerID(&(gameServer->connectionIdentifier), outputBitStream);

	// Write the number of rules
	outputBitStream->WriteCompressed(numberOfRulesToWrite);

	// Write all the rules
	for (serverIndex=0; serverIndex < gameServer->serverRules.size(); serverIndex++)
	{
		if (IsReservedRuleIdentifier(gameServer->serverRules[serverIndex]->key))
			continue;

		SerializeRule(gameServer->serverRules[serverIndex], outputBitStream);
	}
}
GameServer * oaMasterServer::DeserializeServer(RakNet::BitStream *inputBitStream)
{
	unsigned serverIndex;
	unsigned short numberOfRulesToWrite;
	GameServer *gameServer;
	GameServerRule *gameServerRule;

	gameServer= new GameServer;
	DeserializePlayerID(&(gameServer->connectionIdentifier), inputBitStream);

	// Read the number of rules
	if (inputBitStream->ReadCompressed(numberOfRulesToWrite)==false)
	{
		delete gameServer;
		return 0;
	}
	// Read all the rules
	for (serverIndex=0; serverIndex < numberOfRulesToWrite; serverIndex++)
	{
		gameServerRule = DeserializeRule(inputBitStream);
		if (gameServerRule==0)
		{
			delete gameServer;
			return 0;
		}
		if (IsReservedRuleIdentifier(gameServerRule->key))
			delete gameServerRule;
		else
			gameServer->serverRules.insert(gameServerRule);
	}
	return gameServer;
}

void oaMasterServer::UpdateServer(GameServer *destination, GameServer *source, bool deleteSingleRules)
{
	unsigned sourceRuleIndex,destinationRuleIndex;

	destination->lastUpdateTime=RakNet::GetTime();

	// If (deleteSingleRules) then delete any rules that exist in the old and not in the new
	if (deleteSingleRules)
	{
		destinationRuleIndex=0;
		while (destinationRuleIndex < destination->serverRules.size())
		{
			if (IsReservedRuleIdentifier(destination->serverRules[destinationRuleIndex]->key)==false &&
				source->FindKey(destination->serverRules[destinationRuleIndex]->key)==false)
			{
				delete destination->serverRules[destinationRuleIndex];
				destination->serverRules.del(destinationRuleIndex);
			}
			else
				destinationRuleIndex++;
		}
	}

	// Go through all the rules.
	for (sourceRuleIndex=0; sourceRuleIndex < source->serverRules.size(); sourceRuleIndex++)
	{
		if (IsReservedRuleIdentifier(source->serverRules[sourceRuleIndex]->key))
			continue;

		// Add any fields that exist in the new and do not exist in the old
		// Update any fields that exist in both
		UpdateServerRule(destination, source->serverRules[sourceRuleIndex]->key, source->serverRules[sourceRuleIndex]->stringValue, source->serverRules[sourceRuleIndex]->intValue);
	}
}

void oaMasterServer::exportRulesToMySQL(GameServer* gameserver)
{
	unsigned int ping = 0; std::string ip; std::string name; std::string location; unsigned short port = 0;
	std::cout << "UPDATING" << std::endl;
	for(unsigned int i = 0; i < gameserver->serverRules.size(); i++)
	{
		GameServerRule* rule = gameserver->serverRules[i];
		std::cout << "Processing rule " << rule->key << std::endl;
		if(strcmp(rule->key, "SPing")==0) {
			ping = rule->intValue;
		} else if(strcmp(rule->key, "IP")==0) {
			ip   = rule->stringValue;
		} else if(strcmp(rule->key, "Port")==0) {
			port = rule->intValue;
		} else if(strcmp(rule->key, "Game name")==0) {
			name = rule->stringValue;
		} else if(strcmp(rule->key, "Server location")==0) {
			location = rule->stringValue;
		} else {
			std::cout << "Rule skipped" << std::endl;
		}
	}
	mDB.updateServerInfos(gameserver->mySQLListID, name, location, port, ping);
}

GameServer* oaMasterServer::UpdateServerList(GameServer *gameServer, bool deleteSingleRules, bool *newServerAdded)
{
	int searchIndex;

	if (gameServer==0)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return 0;
	}

	// Find the existing game server that matches this port/address.
	searchIndex = gameServerList.GetIndexByPlayerID(gameServer->connectionIdentifier);
	if (searchIndex<0)
	{
		// If not found, then add it to the list.
		AddDefaultRulesToServer(gameServer, gameServer->connectionIdentifier);
		gameServerList.serverList.insert(gameServer);
		gameServer->mySQLListID = mDB.addNewServer(rakPeer->PlayerIDToDottedIP(gameServer->connectionIdentifier));
		exportRulesToMySQL(gameServer);
		*newServerAdded=true;
		return gameServer;
	}
	else
	{
		// Update the existing server
		UpdateServer(gameServerList.serverList[searchIndex], gameServer, deleteSingleRules);
		delete gameServer;
		*newServerAdded=false;
		return gameServerList.serverList[searchIndex];
	}
}
