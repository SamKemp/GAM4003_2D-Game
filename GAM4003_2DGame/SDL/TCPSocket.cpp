#include "TCPSocket.h"

list < CTCPSocket* > CTCPSocket::s_lstSockets ;

//uses SDLNet_TCP_Open
CTCPSocket::CTCPSocket ( CIPAddress* pIPAddress ) 
{
	SocketList ( ).push_back ( this ) ;

	Socket ( ) = SDLNet_TCP_Open ( *pIPAddress ) ;
}

//create from an already existing TCPSocket
CTCPSocket::CTCPSocket ( TCPsocket socket ) 
{
	SocketList ( ).push_back ( this ) ;

	Socket ( ) = socket ;
}

//uses SDLNet_TCP_Accept
CTCPSocket::CTCPSocket ( CTCPSocket* pSocket ) 
{
	SocketList ( ).push_back ( this ) ;

	Socket ( ) = SDLNet_TCP_Accept ( *pSocket ) ;
}

//close socket
CTCPSocket::~CTCPSocket()
{
	SocketList ( ).remove ( this ) ;

	SDLNet_TCP_Close ( *this ) ;
}

//retrieve master socket list
list < CTCPSocket* >& CTCPSocket::SocketList ( ) 
{
	return ( s_lstSockets ) ;
}

//accessors
TCPsocket& CTCPSocket::Socket ( ) 
{
	return ( m_Socket ) ;
}

//conversion operator
CTCPSocket::operator TCPsocket ( ) 
{
	return ( m_Socket ) ;
}

//get the IP address
CIPAddress CTCPSocket::GetIP ( ) 
{
	IPaddress* ipaddress = SDLNet_TCP_GetPeerAddress ( *this ) ;
	if ( ipaddress )
	{
		return ( CIPAddress ( ipaddress ) ) ;
	}
	else
	{
		IPaddress ip;
		ip.host=0;
		ip.port=0;
		return ( CIPAddress ( &ip ) ) ;
	}
}

//send a string
bool CTCPSocket::Send ( string sData ) 
{
	if ( sData.size ( ) == 0 ) return ( true ) ;

	char* buffer = strdup ( sData.c_str ( ) ) ;

	int nBytesSent = SDLNet_TCP_Send( *this , buffer , sData.size ( ) ) ;

	free ( buffer ) ;

	return ( nBytesSent == ( int ) sData.size ( ) ) ;
}

//receive a string
string CTCPSocket::Receive ( ) 
{
	if ( !Ready ( ) ) return ( "" ) ;

	char* buffer = new char [ NETTCP_PACKETSIZE_DEFAULT + 1 ] ;

	string sData = "" ;

	int nBytesReceived ; 

	while ( Ready ( ) ) 
	{
		memset ( buffer , 0 , NETTCP_PACKETSIZE_DEFAULT + 1 ) ;
	
		nBytesReceived = SDLNet_TCP_Recv( *this , buffer , NETTCP_PACKETSIZE_DEFAULT ) ;

		if ( nBytesReceived <= 0 )
		{
			delete [] buffer ;
			return ( sData ) ;
		}

		sData += buffer ;
	}

	delete [] buffer ;

	return ( sData ) ;
}

bool CTCPSocket::Ready ( ) 
{
	return ( SDLNet_SocketReady ( m_Socket ) ) ;
}

//check this socket
void CTCPSocket::Check ( ) 
{
	if ( !Socket ( ) ) return ;

	SDLNet_SocketSet set = SDLNet_AllocSocketSet ( 1 ) ;
	SDLNet_TCP_AddSocket ( set , Socket ( ) ) ;
	SDLNet_CheckSockets ( set , 0 ) ;
	SDLNet_FreeSocketSet ( set ) ;
	
	//if the socket is ready, call the onready function
	if ( Ready ( ) )
	{
		OnReady ( ) ;
	}
}

//when a check proves that a socket is ready, call this function
void CTCPSocket::OnReady ( ) 
{
	//by default, do nothing

}

//check all sockets
void CTCPSocket::CheckAll ( ) 
{
	list < PTCPSOCKET >::iterator iter ;
	PTCPSOCKET pSocket ;
	for ( iter = SocketList ( ).begin ( ) ; iter!= SocketList ( ).end ( ) ; iter ++ )
	{
		pSocket = *iter ;
		pSocket->Check ( ) ;
	}
}

bool CTCPSocket::GetStatus ( ) 
{
	return ( m_Socket != NULL ) ;
}
