#ifndef CLIENTSOCKET_H__
#define CLIENTSOCKET_H__

#include "TCPSocket.h"
#include "Application.h"

//CClientSocket
//abstracts a client socket
class CClientSocket : public CTCPSocket  
{
public:
	//constructor
	CClientSocket(CTCPSocket* pSocket);
	//destructor
	virtual ~CClientSocket();
	//on ready handler
	void OnReady();
	//message id
	static MSGID MSGID_SocketData;//parm1=CTCPSocket*;parm2=char*
};

#endif 
