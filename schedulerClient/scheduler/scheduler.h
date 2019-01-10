#pragma once

#include <gtk\gtk.h>
#include <cairo.h>
#include <WinSock2.h>
#include <iostream>
//#include <string>
#include <algorithm>
//#include <thread>
//#include <exception>
using namespace std;

#define CHARBUFF 124
#define DAYCNT 31
#ifndef ARRAYSIZE
#define ARRAYSIZE(array) (sizeof(array)/sizeof(*array))
#endif


struct App {
	GtkWidget *window, *label/*, *entry*/, *button, *frame, *vbox, *hbox, *grid, *framevbox, *framehbox/*, *eventbox*/;

	GtkWidget *eventbox1, *eventbox2, *eventbox3, *eventbox4, *eventbox5, *eventbox6, *eventbox7, *eventbox8, *eventbox9, *eventbox10,
		*eventbox11, *eventbox12, *eventbox13, *eventbox14, *eventbox15, *eventbox16, *eventbox17, *eventbox18, *eventbox19, *eventbox20,
		*eventbox21, *eventbox22, *eventbox23, *eventbox24, *eventbox25, *eventbox26, *eventbox27, *eventbox28, *eventbox29, *eventbox30,
		*eventbox31;

	GtkWidget *serverEntry, *portEntry;

	/*GtkWidget *label1, *label2, *label3, *label4, *label5, *label6, *label7, *label8, *label9, *label10,
		*label11, *label12, *label13, *label14, *label15, *label16, *label17, *label18, *label19, *label20,
		*label21, *label22, *label23, *label24, *label25, *label26, *label27, *label28, *label29, *label30,
		*label31;*/
	GtkWidget *connectGuide, *schedulingGuide;

	GdkColor red, green, gray, lightgray, black, blue;

	int w_width, w_height;
};

extern App *app;
extern int freeFlags[DAYCNT];
extern char *c_freeFlags;
extern int oppoFreeFlags[DAYCNT];


//layout.cpp
void mainLayout();
void drawLayout();
static GtkWidget* drawCalendar(GtkWidget*, int, int);
void colorSet();
void updateCalendar();



//callbackClient.cpp
void schedule_sw(GtkWidget*, gpointer);
void tryConnection(GtkWidget*, gpointer);
void sendSchedule(GtkWidget*, gpointer);
void getServerIP();
void getPort();
void disconnect();
void clearSchedule();

//スレッドへの対応
/*
http://ici1974.blogspot.com/2014/01/cgtkgthread.html
*/
/*GKT+のthread関数*/
extern GThread *TCPThread;
//static GError *error;

/*C++標準のstd::threadを使用しようとしたが、「関数操作のレベルが違う」というエラーが。
extern thread tcpThread;
*/




//TCOClient.cpp
bool Connect(const char*, int);
void DisConnect();
bool isConnected();
bool send_data(const char*, int);
bool recieve_data();

gpointer mainTCPClient(gpointer);//スレッドで回すTCP通信の関数
void getOwnIPAddress();
bool oppoDataCleared();

extern WSADATA wsaData;
extern struct sockaddr_in serverinfo;
extern SOCKET sock;
extern bool isConnectedFlag;


extern bool closeTCP;
extern const char* serverIP;
extern char clientIP[16];
extern int port;




