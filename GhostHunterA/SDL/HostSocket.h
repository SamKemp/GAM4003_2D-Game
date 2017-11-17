#ifndef HOSTSOCKET_H__
#define HOSTSOCKET_H__

#include "TCPSocket.h"
#include "Application.h"

//CHostSocket class
//abstracts a server socket
class CHostSocket : public CTCPSocket  
{
public:
	//constructor
	CHostSocket(CIPAddress* pip);
	//destructor
	virtual ~CHostSocket();
	//when ready with data
	void OnReady();
	//creation
	static CHostSocket* Create(Uint16 port);
	//message ids
	static MSGID MSGID_NewClient;//parm1=CHostSocket*
};

#endif 
