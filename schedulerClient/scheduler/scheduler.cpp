// scheduler.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "scheduler.h"

App *app;

int freeFlags[DAYCNT] = { 0 };
char *c_freeFlags;
int oppoFreeFlags[DAYCNT];


WSADATA wsaData;
struct sockaddr_in serverinfo;
SOCKET sock;
bool isConnectedFlag = false;
const char* serverIP;
char clientIP[16];
int port = 0;
bool closeTCP = false;

GThread *TCPThread;


int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);

	mainLayout();

	gtk_main();

	return 0;
}

