#include <cutils/sockets.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "ftm.h"
#include "msocket.h"

#define TAG "[MSOCKET] "


MSocket::MSocket()
{
    initValues();
}

MSocket::MSocket(SOCKET_TYPE type)
{
	initValues();
	m_type = type;
}


MSocket::~MSocket(void)
{
	deinit();
}
void MSocket::initValues(void)
{
	m_serverID = -1;
	m_clientID = -1;
	m_threadID = -1;
	m_stop = 0;
	memset(&m_thread, 0, sizeof(pthread_t));
	m_type = SOCKET_END;
	m_bClientConnected = false;
}
int MSocket::initServer(const char * socket_name, int namespaceId, int bListen)
{
	LOGD(TAG "[Socket] To Create Socket Server:(%s)", socket_name);
	
	m_serverID = android_get_control_socket(socket_name);
	
	LOGD(TAG "[Socket] m_serverID = %d errno = %d string = %s", m_serverID, errno, strerror(errno));	

	if(listen(m_serverID,4) < 0)
	{
		LOGD(TAG "[Meta][Socket] failed to listen client");
		return 0;
	}

/*	if(bListen)
	{
		m_threadID = pthread_create(&m_thread, NULL, ThreadFunc,  this);
		if(m_threadID)
		{
			LOGD(TAG "[Socket] Failed to create socket thread!");
			return 0;
		}
	}
*/	
	return 1;	
		
}

int MSocket::initClient(const char * socket_name, int namespaceId, int bListen)
{
	int count = 0;
	int val = 0;
	
	LOGD(TAG "[Socket] To connect server:(%s)", socket_name);
	while(m_clientID < 0) 
	{
		count++;
		m_clientID = socket_local_client(socket_name, namespaceId, SOCK_STREAM);
		LOGD(TAG "[Socket] init client m_clientID = %d", m_clientID);
        LOGD(TAG "[Socket] errno = %d, string = %s", errno, strerror(errno));
		usleep(200*1000);
		if(count == 5)
			return 0;		
	}

	LOGD(TAG "[Socket] connect successful");
	//if bListen is true, we will create thread to read socket data.
/*	if(bListen)
	{
		m_threadID = pthread_create(&m_thread, NULL, ThreadFunc,  this);
		if(m_threadID)
		{
			LOGD(TAG "[Socket] Failed to create socket thread!");
			return 0;
		}
	}
*/	
	if(0 == setsockopt(m_clientID, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)))
	{
		LOGD(TAG "[Socket] set socket to option to TCP_NODELAY!");
	}
	
	return 1;
}

void MSocket::deinit()
{
	if(m_threadID == 0)
	{
		m_stop = 1;
		pthread_join(m_thread, NULL);
	}

    if (m_clientID > 0)
    {
       	close (m_clientID);
        m_clientID = -1;
    }

	if (m_serverID > 0)
    {
       	close (m_serverID);
        m_serverID = -1;
    }
}

int MSocket::connect()
{
	LOGD(TAG "[Socket] connect");
	if(m_bClientConnected)
		return 0;
	int retryCount = 0;

	struct sockaddr_in clientAddr;    
	socklen_t sinSize = sizeof(struct sockaddr_in);

    int flags = fcntl(m_serverID, F_GETFL, 0);
    if(flags == -1){
        LOGD(TAG "[Socket] fcntl F_GETFL failed");
  	    return -1;
    }
	flags = fcntl(m_serverID, F_SETFL, flags|O_NONBLOCK);
	if(flags == -1){
        LOGD(TAG "[Socket] fcntl F_SETFL failed");
        return -1;
    }
	while((m_clientID = accept(m_serverID, (struct sockaddr *)&clientAddr, &sinSize)) == -1)
	{	
		LOGD(TAG "[Socket] retryCount = %d", retryCount);
		
		if(retryCount >50)
		{
			LOGD(TAG "[Socket] Socket accept error, errno=%d", errno);
			return -1;
		}
		
		if(errno == EAGAIN)
		{
			usleep(100*1000);
			retryCount++;
			continue;
		}

		LOGD(TAG "[Socket] Socket accept error, errno=%d", errno);
		return -1;
	}
	
	LOGD(TAG "[Socket] Accept client connection, m_clientID = %d", m_clientID);
	
	m_bClientConnected = true;
	
	return 0;
}


void MSocket::disconnect()
{
	if (m_bClientConnected)
	{
		if (m_clientID != -1)
		{
			LOGD(TAG "[Socket] Disconnect client connection, m_clientID = %d", m_clientID);
			close(m_clientID);
			m_clientID = -1;
			m_bClientConnected = false;
		}
    }
}

void MSocket::close_server()
{
	if(m_serverID > 0)
	{
		int ret = socket_close(m_serverID);
		LOGD(TAG "[Socket] close socket server id = %d, ret = %d", m_serverID, ret);
		m_serverID = -1;
	}
}



void MSocket::send_msg(const char *msg)
{
	int nWritten = 0;

	LOGD(TAG "[Socket] send mssage (%s) - socket id = %d", msg,  m_clientID);

	if((nWritten = write(m_clientID, msg, strlen(msg))) < 0)
	{
		LOGD(TAG "[Socket] socket write error: %s", strerror(errno));
	}
	else
	{
		LOGD(TAG "[Socket] write %d Bytes, total = %zd", nWritten, strlen(msg));
	}
}


/*void *MSocket::ThreadFunc(void *p)
{
	MSocket *pSocket = (MSocket *)p;
	if(pSocket != NULL)
		pSocket->wait_msg();
	
	return NULL;
}*/



