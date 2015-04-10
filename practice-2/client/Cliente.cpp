/* Cliente.cpp - Generated by Visual Multi-Thread Win32 */

// WIN32_LEAN_AND_MEAN macro prevents the Winsock.h from being 
// included by the Windows.h header
#define WIN32_LEAN_AND_MEAN


// The Winsock2.h header file contains most of the Winsock functions, 
// structures, and definitions. The Ws2tcpip.h header file contains 
// definitions introduced in the WinSock 2 Protocol-Specific
// The Winsock2.h header file internally includes core elements from 
// the Windows.h header file
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "AppObjects.h"
#include "my_socket.h"
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

DWORD WINAPI thread_Receive(LPVOID lpParameter);
DWORD WINAPI thread_Send(LPVOID lpParameter);

void cleanUp() {
	keepRunning = 0;

	if (ClientSocket != NULL) {
		closesocket(ClientSocket);
	}
}

bool checkOperation(message message){
	if (!strcmp(message.buf, "*disable")) {
		strcpy(message.buf, CLIENT_OP_DISABLE);
		return true;
	}
	if (!strcmp(message.buf, "*enable")) {
		strcpy(message.buf, CLIENT_OP_ENABLE);
		return true;
	}

	return false;
}

DWORD WINAPI thread_Receive(LPVOID lpParameter)
{
	SOCKET *socket = (SOCKET *)lpParameter;
	message message;

	while (keepRunning) {
		int result = recv(*socket, (char *)&message, sizeof(message), 0);

		printf("%s:  %s\n", message.name, message.buf);
		Sleep(10);
	}

	return 0;
}

DWORD WINAPI thread_Send(LPVOID lpParameter)
{
	SOCKET *socket = (SOCKET *)lpParameter;

	message message;
	int result;
	char trash;

	printf("Entre com o nome do Cliente: \n");
	gets(message.name);

	message.nro_msg = 1;

	while (keepRunning)
	{
		printf(">> ");
		gets(message.buf);

		bool isOperation = checkOperation(message);

		result = send(*socket, (const char *)&message, sizeof(message), 0);
		if (result == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		Sleep(10);
	}

	// Send a message	'0' to Server in order to close the connection
	message.buf[0] = '0';
	result = send(*socket, (const char *)&message, (int)sizeof(message), 0);

	return 0;
}
