#include "stdafx.h"
#include "scheduler.h"
using namespace std;


gpointer mainTCPServer(gpointer data) {
	char text[CHARBUFF];
	gchar *gLabel;
	int autoSendCnt = 0;


	if (Open(port)) {
		fprintf_s(stdout, "wait client connection with %d port\n", port);

		while (true) {

			if (!isConnected()) {
				AcceptClient();

				sprintf_s(text, "Successed in connecting!");
				gLabel = text;
				gtk_widget_modify_fg(app->connectGuide, GTK_STATE_NORMAL, &(app->blue));
				gtk_label_set_label(GTK_LABEL(app->connectGuide), gLabel);
			}


			while (true) {
				//char dataBuff[CHARBUFF];



				if (recieve_data()) { //(recieve_text(dataBuff, sizeof(dataBuff))) {
					/*sprintf_s(text, "Successed in recieving data");
					gLabel = text;
					gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
					gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);*/
					
					updateCalendar();

					if (oppoDataCleared()) {//����̃f�[�^��0�ɏ���������Ă�����
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
					if (autoSendCnt < 1) {
						if (!send_data(reinterpret_cast<const char*>(freeFlags), sizeof(freeFlags))) {
							sprintf_s(text, "Failed in sending data!");
							gLabel = text;
							gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
							gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
							break;
						}

						autoSendCnt++;
					}
					else {
						break;
					}


					//��M�ł�����ʐM��؂�i�����Ă��ǂ��Ȃ����j
					//closeTCP = true;
					//break;
				}
				else {
					closeTCP = true;
					break;
				}


				//��M�����f�[�^�\��
				//fprintf_s(stdout, "%s\n", dataBuff);
				//�������̏�����
				//memset(dataBuff, 0, sizeof(dataBuff));
				//���͓��e�̎擾
				//fprintf_s(stdout, "waiting input <<");
				//scanf_s("%s", dataBuff, CHARBUFF);
				//���͓��e�̕\��
				//fprintf_s(stdout, "original send : %s\n", dataBuff);


				/*
				if (send_text(dataBuff, sizeof(dataBuff))) {

				}
				else {
				//clientConnection->DisConnect();
				break;
				}
				*/


			}

			if (closeTCP) {
				DisConnect();
				cout << "Connect stopped!" << endl;
				break;
			}

			autoSendCnt = 0;
		}
		Close();
	}

	return NULL;
}



bool Open(int port) {
	//Winsock�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//���������s
		return false;
	}

	//�\�P�b�g�̍쐬
	sock4connect = socket(AF_INET, SOCK_STREAM, 0);
	if (sock4connect == INVALID_SOCKET) {
		//�쐬���s
		fprintf_s(stderr, "socket : %d\n", WSAGetLastError());
		return false;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//�|�[�g�̊J��
	if (bind(sock4connect, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		//�J�����s
		fprintf_s(stderr, "bind : %d\n", WSAGetLastError());
		return false;
	}

	if (listen(sock4connect, maxClients) != 0) {
		fprintf_s(stderr, "listen : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

void Close() {
	if (sock4connect != NULL) {
		closesocket(sock4connect);
		WSACleanup();
	}
}

bool AcceptClient() {
	SOCKET sock2client;

	int len = sizeof(client);
	sock2client = accept(sock4connect, (struct sockaddr *)&client, &len);//�N���C�A���g����̐ڑ��������炻�̒ʐM�̂��߂̃\�P�b�g���쐬
	if (sock2client == INVALID_SOCKET) { //�N���C�A���g�Ƃ̐ڑ����s
		fprintf_s(stderr, "accept error : %d\n", WSAGetLastError());
		return 0;
	}
	fprintf_s(stdout, "accepted connection %d@%s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));//�N���C�A���g�Ƃ̐ڑ������̕\��

	sock = sock2client;
	isConnectedFlag = true;
	isConnected();

	//return *tcpSocket;
}

bool send_data(const char* data, int size) {
	cout << "sended : " << data << endl;
	if (!isConnected()) {
		cout << "Failed to send" << endl;
		DisConnect();
		WSACleanup();
		return false;
	}

	send(sock, data, size, 0);//���M
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

	//�m�F�p
	cout << "recieved : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	return true;
}


void  getOwnIPAddress() {
	/*
	�y�Q�l�z
	https://github.com/tanaka0079/c/blob/master/winsock/get_ip_host.c
	*/
	HOSTENT *lpHost;
	IN_ADDR inaddr;
	char szBuf[256], szIP[16];//�z�X�g���EIP�A�h���X���i�[����z��

							  //������PC�̃z�X�g�����擾
	gethostname(szBuf, (int)sizeof(szBuf));
	cout << "Host Name : " << szBuf << endl;

	lpHost = gethostbyname(szBuf);
	//IP�A�h���X���擾
	for (int i = 0; lpHost->h_addr_list[i]; i++) {
		memcpy(&inaddr, lpHost->h_addr_list[i], 4);
		strcpy_s(szIP, inet_ntoa(inaddr));
		cout << "IPAddress : " << szIP << endl;
	}

	/*
	https://teratail.com/questions/57546
	https://www.sejuku.net/blog/25859
	*/
	strcpy_s(serverIP, sizeof(szIP), szIP);

}


bool oppoDataCleared() {
	for (int i = 0; i < DAYCNT; i++) {
		if (oppoFreeFlags[i] != 0) {
			return false;
		}
	}
	return true;
}