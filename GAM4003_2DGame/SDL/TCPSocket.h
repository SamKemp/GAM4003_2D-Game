#ifndef __NETTCPSOCKET_H__
#define __NETTCPSOCKET_H__

#include "sdl_net.h"
#include "IPAddress.h"
#include <list>
#include <string>
using namespace std;

enum { NETTCP_PACKETSIZE_DEFAULT = 256 };

//CTCPSocket class
//encapsulates TCPsocket
class CTCPSocket  
{
private :
	//internal representation
	TCPsocket m_Socket ;
	//static list of all sockets
	static list < CTCPSocket* > s_lstSockets ;
public:
	//uses SDLNet_TCP_Open
	CTCPSocket ( CIPAddress* pIPAddress ) ;

	//create from an already existing TCPSocket
	CTCPSocket ( TCPsocket socket ) ;

	//uses SDLNet_TCP_Accept
	CTCPSocket ( CTCPSocket* pSocket ) ;

	//close socket
	virtual ~CTCPSocket();

	//retrieve master socket list
	static list < CTCPSocket* >& SocketList ( ) ;

	//accessors
	TCPsocket& Socket ( ) ;

	//conversion operator
	operator TCPsocket ( ) ;

	//get the IP address
	CIPAddress GetIP ( ) ;

	bool GetStatus ( ) ;

	//send a string
	bool Send ( string sData ) ;

	//receive a string
	string Receive ( ) ;

	//is there data to be read?
	bool Ready ( ) ;

	//check this socket
	void Check ( ) ;

	//when a check proves that a socket is ready, call this function
	virtual void OnReady ( ) ;

	//check all sockets
	static void CheckAll ( ) ;
};

typedef CTCPSocket TCPSOCKET ;
typedef CTCPSocket* PTCPSOCKET ;

#endif
