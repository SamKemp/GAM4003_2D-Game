#include "ClientSocket.h"

//message id
MSGID CClientSocket::MSGID_SocketData=CMessageHandler::GetNextMSGID();//parm1=CTCPSocket*;parm2=char*

//constructor
CClientSocket::CClientSocket(CTCPSocket* pSocket):
CTCPSocket(pSocket)
{
	//do nothing
}

//destructor
CClientSocket::~CClientSocket()
{
	//do nothing
}

//on ready handler
void CClientSocket::OnReady()
{
	//read from socket
	string s=Receive();
	//notify application
	CApplication::GetApplication()->SendMessage(MSGID_SocketData,(MSGPARM)this,(MSGPARM)s.c_str());
}
