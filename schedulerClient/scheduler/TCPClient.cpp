#include "stdafx.h"
#include "scheduler.h"

//スレッドで回すTCP通信の関数
gpointer mainTCPClient(gpointer data) {
	char text[CHARBUFF];
	gchar *gLabel;
	int autoSendCnt = 0;


	while (true) {

		if (!isConnected()) {
			if (!Connect(serverIP, port)) {
				sprintf_s(text, "Failed in connecting!");
				gLabel = text;
				gtk_widget_modify_fg(app->connectGuide, GTK_STATE_NORMAL, &(app->red));
				gtk_label_set_label(GTK_LABEL(app->connectGuide), gLabel);

				cout << "A" << endl;
				closeTCP = true;
				break;
			}

			sprintf_s(text, "Successed in connecting!");
			gLabel = text;
			gtk_widget_modify_fg(app->connectGuide, GTK_STATE_NORMAL, &(app->blue));
			gtk_label_set_label(GTK_LABEL(app->connectGuide), gLabel);
		}


		while (true) {
			if (recieve_data()) {
				/*sprintf_s(text, "Successed in recieving data");
				gLabel = text;
				gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
				gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);*/


				updateCalendar();

				if (oppoDataCleared()) {//相手のデータが0に初期化されていたら
					sprintf_s(text, "Partner has initialized Schedule.");
					gLabel = text;
					gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
					gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
				}
				else {
					sprintf_s(text, "Updated Schedule.");
					gLabel = text;
					gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
					gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
				}


				cout << autoSendCnt << endl;
				//相手から受け取ったら自分の最新スケジュールを自動送信
				if (autoSendCnt < 1) {
					if (!send_data(reinterpret_cast<const char*>(freeFlags), sizeof(freeFlags))) {
						sprintf_s(text, "Failed in sending data!");
						gLabel = text;
						gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
						gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
						break;
					}
				}
				else {
					break;
				}


				//受信できたら通信を切る（無くても良くなった）
				//closeTCP = true;
				//break;
			}
			else {//サーバ側が接続を斬った場合
				closeTCP = true;
				break;
			}
		}


		if (closeTCP) {
			DisConnect();
			cout << "Connect stopped!" << endl;
			closeTCP = false;
			break;
		}

		autoSendCnt = 0;
	}


	return NULL;




	/*
	http://www.yunabe.jp/docs/cpp_casts.html
	*
	/*
	＜send()使う時はこっちでやる。.hでもconst付けておくように＞
	c_freeFlags = reinterpret_cast<const char*>(freeFlags);
	*
	c_freeFlags = reinterpret_cast<char*>(freeFlags);
	//cout << c_freeFlags << endl;
	for (int i = 0; i < DAYCNT; i++) {
		cout << *(c_freeFlags+i) << " ";
	}
	cout << endl;

	/*
	＜受信を想定　※recvによる配列へのキャストではないもの。＞
	int a[3];
	for (int i = 0; i < 3; i++) {
	a[i] = *reinterpret_cast<int*>(&c_a[i * sizeof(int)]);
	}
	とか，
	// #include <algorithm> 必要
	std::copy_n(reinterpret_cast<int*>(c_a), 3, std::begin(a));
	*
	/*
	＜recvを使う場合はこっちの方法で。＞
	配列だったら，
	int a[] = {123, 456, 789};
	send(s, reinterpret_cast<char*>(a), sizeof(a), 0);
	- - -
	int a[3];  // 受信側の要素数は多めでもおk
	recv(s, reinterpret_cast<char*>(a), sizeof(a), 0);
	*

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *reinterpret_cast<int*>(&c_freeFlags[i * sizeof(int)]);
	}
	cout << "1つ目 : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	copy_n(reinterpret_cast<int*>(c_freeFlags), 3, begin(oppoFreeFlags));
	cout << "2つ目 : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *(c_freeFlags + i * sizeof(int));
	}
	cout << "単純なポインタ指定 : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *reinterpret_cast<int*>(c_freeFlags+i*sizeof(int));
	}
	cout << "1つ目アレンジ : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;
	*/

}


bool Connect(const char *server, int port_num) {
	//winsocketの初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//Winsockの初期化に失敗したら
		fprintf_s(stderr, "WSAStartup failed\n");
		return false;
	}

	//socketの生成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {//ソケット生成ができなかった場合
		fprintf_s(stderr, "socket : %d\n", WSAGetLastError());
		return false;
	}

	//サーバー情報の取得
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.S_un.S_addr = inet_addr(server);//IPアドレスの取得（32bit?のバイナリに変換）
	serverinfo.sin_port = htons(port_num);//公開ポート番号の指定


	unsigned int **addrptr;
	if (serverinfo.sin_addr.S_un.S_addr == 0xffffffff) {//ホスト名の場合の接続
		fprintf_s(stdout, "host name to bynary\n");
		struct hostent *host;

		host = gethostbyname(server);//ホスト名の取得（バイナリに変換）
		if (host == NULL) {//ホスト名を取得できなかったら
			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
				fprintf_s(stderr, "host not found : %s\n", server);
			}
			return false;
		}

		addrptr = (unsigned int **)host->h_addr_list;

		while (*addrptr != NULL) {//スライド7の4
			serverinfo.sin_addr.S_un.S_addr = *(*addrptr);

			//接続成功の場合ループを抜ける
			if (connect(sock,
				(struct sockaddr *)&serverinfo,
				sizeof(serverinfo)) == 0) {
				break;
			}

			addrptr++;//connectが失敗したら次のアドレスに
		}

		//connectが全て失敗した場合
		if (*addrptr == NULL) {
			fprintf_s(stderr, "connect : %d\n", WSAGetLastError());
			return false;
		}
	}
	else {//IPアドレスの場合の接続
		  //inet_addr()が成功した時 = addr がIPアドレスで表現されていた時

		  //connectが失敗したらエラーを表示して終了
		if (connect(sock,
			(struct sockaddr *)&serverinfo,
			sizeof(serverinfo)) != 0) {
			fprintf_s(stderr, "connect : %d\n", WSAGetLastError());
			return false;
		}
	}

	isConnectedFlag = true;
	return true;//connect成功
}
void DisConnect() {
	if (sock != NULL) {
		isConnectedFlag = false;
		closesocket(sock);
		WSACleanup();
	}
}
bool isConnected() {
	return isConnectedFlag;
}

bool send_data(const char* data, int size) {
	cout << "sended : " << data << endl;
	if (!isConnected()) {
		cout << "Failed to send" << endl;
		DisConnect();
		WSACleanup();
		return false;
	}

	send(sock, data, size, 0);//送信
	return true;
}

bool recieve_data() {
	//memset(data, 0, size);
	int n = recv(sock, reinterpret_cast<char*>(oppoFreeFlags), sizeof(oppoFreeFlags), 0);
	if (n < 0) {
		cout << "recv : " << WSAGetLastError() << endl;
		DisConnect();
		WSACleanup();
		return false;
	}

	//確認用
	cout << "recieved : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	return true;
}


void  getOwnIPAddress() {
	/*
	【参考】
	https://github.com/tanaka0079/c/blob/master/winsock/get_ip_host.c
	*/
	HOSTENT *lpHost;
	IN_ADDR inaddr;
	char szBuf[256], szIP[16];//ホスト名・IPアドレスを格納する配列

	//自分のPCのホスト名を取得
	gethostname(szBuf, (int)sizeof(szBuf));
	cout << "Host Name : " << szBuf << endl;

	lpHost = gethostbyname(szBuf);
	//IPアドレスを取得
	for (int i = 0; lpHost->h_addr_list[i]; i++) {
		memcpy(&inaddr, lpHost->h_addr_list[i], 4);
		strcpy_s(szIP, inet_ntoa(inaddr));
		cout << "IPAddress : " << szIP << endl;
	}

	/*
	https://teratail.com/questions/57546
	https://www.sejuku.net/blog/25859
	*/
	strcpy_s(clientIP, sizeof(szIP), szIP);

}

bool oppoDataCleared() {
	for (int i = 0; i < DAYCNT; i++) {
		if (oppoFreeFlags[i] != 0) {
			return false;
		}
	}
	return true;
}