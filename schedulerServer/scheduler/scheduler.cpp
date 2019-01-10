// scheduler.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "scheduler.h"

App *app;
int freeFlags[DAYCNT] = { 0 };
char *c_freeFlags;
int oppoFreeFlags[DAYCNT];

WSADATA wsaData;
struct sockaddr_in server;
struct sockaddr_in addr;
struct sockaddr_in client;
SOCKET sock;
SOCKET sock4connect;
int maxClients = 1;
bool isConnectedFlag = false;

//bool connectTryFlag = false;
char serverIP[16];
const char* clientIP;
int port = 0;
bool closeTCP = false;

GThread *TCPThread;


int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);

	mainLayout();

	//GThread *thread = g_thread_new("thread_sample")

	gtk_main();

	return 0;
}

