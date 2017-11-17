#include "HostSocket.h"

//message ids
MSGID CHostSocket::MSGID_NewClient=CMessageHandler::GetNextMSGID();//parm1=CHostSocket*

//constructor
CHostSocket::CHostSocket(CIPAddress* pip):
CTCPSocket(pip)
{
	//do nothing in particular
}

//destructor
CHostSocket::~CHostSocket()
{
	//do nothing in particular
}

//when ready with data
void CHostSocket::OnReady()
{
	//send a new client message
	CApplication::GetApplication()->SendMessage(MSGID_NewClient,(MSGPARM)this);
}

//creation
CHostSocket* CHostSocket::Create(Uint16 port)
{
	//create the ip
	CIPAddress ip(NULL,port);
	//create socket
	return(new CHostSocket(&ip));
}

