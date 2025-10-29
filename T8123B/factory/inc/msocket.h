#ifndef _FTMSOCKET_H_
#define _FTMSOCKET_H_

#include <string>
using namespace std;

typedef enum
{
	SOCKET_ATCI    = 0,
	SOCKET_END         = 1
}SOCKET_TYPE;


class MSocket
{

public:
	MSocket();
	MSocket(SOCKET_TYPE type);
	virtual ~MSocket(void);
	int initClient(const char * socket_name, int namespaceId, int bListen=1);
	int initServer(const char * socket_name, int namespaceId, int bListen=1);
	void initValues(void);
	void deinit();
	int connect();
	void disconnect();
	void close_server();
	int getClientSocketID() const
	{
	   return m_clientID;
	}
	int getServerSocketID() const
	{
	   return m_serverID;
	}
	
	bool getClientConnectStatus()
	{
		return m_bClientConnected;
	}
	void send_msg(const char *msg);

/*private:
	static void* ThreadFunc(void*);
	void wait_msg() = 0;*/

public:
	SOCKET_TYPE m_type;
	bool m_bClientConnected;
	
protected:	
	int m_clientID;
	int m_serverID;
	int m_threadID;
	int m_stop;
	pthread_t  m_thread;
};




#endif

