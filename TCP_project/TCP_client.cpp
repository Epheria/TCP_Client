#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

#define MAX_BUFFER_SIZE 256

void err_quit(const char* msg);

int main()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;
	MessageBoxA(NULL, "윈속을 사용할 준비가 되었다.", "윈속 사용 준비 완료.", MB_OK);

	// socket() 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock) err_quit("socket");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// connect()
	if(SOCKET_ERROR == connect(sock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr))) err_quit("connect");

	int len, retval;
	char buf[MAX_BUFFER_SIZE + 1];
	while (1)
	{
		ZeroMemory(buf, sizeof(buf));
		printf("\n[보낼 데이터]");
		if (fgets(buf, MAX_BUFFER_SIZE, stdin) == NULL) break;

		len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		//데이터 보내기
		retval = send(sock, buf, sizeof(buf), 0);
		if (SOCKET_ERROR == retval) break;

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		//데이터 받기
		ZeroMemory(buf, sizeof(buf));
		retval = recv(sock, buf, sizeof(buf), 0);
		if (SOCKET_ERROR == retval) break;
		else if (0 == retval) break;

		// 받은 데이터 출력
		buf[retval - 1] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터]%s\n", buf);
	}
	closesocket(sock);

	WSACleanup();
}

void err_quit(const char* msg)
{
	LPVOID IpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&IpMsgBuf, 0, NULL);

	MessageBoxA(NULL, (LPCSTR)IpMsgBuf, msg, MB_ICONERROR);

	LocalFree(IpMsgBuf);
	exit(-1);
}