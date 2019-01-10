#include "stdafx.h"
#include "scheduler.h"

//�X���b�h�ŉ�TCP�ʐM�̊֐�
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
				//���肩��󂯎�����玩���̍ŐV�X�P�W���[�����������M
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


				//��M�ł�����ʐM��؂�i�����Ă��ǂ��Ȃ����j
				//closeTCP = true;
				//break;
			}
			else {//�T�[�o�����ڑ����a�����ꍇ
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
	��send()�g�����͂������ł��B.h�ł�const�t���Ă����悤�Ɂ�
	c_freeFlags = reinterpret_cast<const char*>(freeFlags);
	*
	c_freeFlags = reinterpret_cast<char*>(freeFlags);
	//cout << c_freeFlags << endl;
	for (int i = 0; i < DAYCNT; i++) {
		cout << *(c_freeFlags+i) << " ";
	}
	cout << endl;

	/*
	����M��z��@��recv�ɂ��z��ւ̃L���X�g�ł͂Ȃ����́B��
	int a[3];
	for (int i = 0; i < 3; i++) {
	a[i] = *reinterpret_cast<int*>(&c_a[i * sizeof(int)]);
	}
	�Ƃ��C
	// #include <algorithm> �K�v
	std::copy_n(reinterpret_cast<int*>(c_a), 3, std::begin(a));
	*
	/*
	��recv���g���ꍇ�͂������̕��@�ŁB��
	�z�񂾂�����C
	int a[] = {123, 456, 789};
	send(s, reinterpret_cast<char*>(a), sizeof(a), 0);
	- - -
	int a[3];  // ��M���̗v�f���͑��߂ł���k
	recv(s, reinterpret_cast<char*>(a), sizeof(a), 0);
	*

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *reinterpret_cast<int*>(&c_freeFlags[i * sizeof(int)]);
	}
	cout << "1�� : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	copy_n(reinterpret_cast<int*>(c_freeFlags), 3, begin(oppoFreeFlags));
	cout << "2�� : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *(c_freeFlags + i * sizeof(int));
	}
	cout << "�P���ȃ|�C���^�w�� : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;

	for (int i = 0; i < DAYCNT; i++) {
		oppoFreeFlags[i] = *reinterpret_cast<int*>(c_freeFlags+i*sizeof(int));
	}
	cout << "1�ڃA�����W : [";
	for (int i = 0; i < DAYCNT; i++) {
		cout << " " << oppoFreeFlags[i] << " ";
	}
	cout << "]" << endl;
	*/

}


bool Connect(const char *server, int port_num) {
	//winsocket�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//Winsock�̏������Ɏ��s������
		fprintf_s(stderr, "WSAStartup failed\n");
		return false;
	}

	//socket�̐���
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {//�\�P�b�g�������ł��Ȃ������ꍇ
		fprintf_s(stderr, "socket : %d\n", WSAGetLastError());
		return false;
	}

	//�T�[�o�[���̎擾
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.S_un.S_addr = inet_addr(server);//IP�A�h���X�̎擾�i32bit?�̃o�C�i���ɕϊ��j
	serverinfo.sin_port = htons(port_num);//���J�|�[�g�ԍ��̎w��


	unsigned int **addrptr;
	if (serverinfo.sin_addr.S_un.S_addr == 0xffffffff) {//�z�X�g���̏ꍇ�̐ڑ�
		fprintf_s(stdout, "host name to bynary\n");
		struct hostent *host;

		host = gethostbyname(server);//�z�X�g���̎擾�i�o�C�i���ɕϊ��j
		if (host == NULL) {//�z�X�g�����擾�ł��Ȃ�������
			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
				fprintf_s(stderr, "host not found : %s\n", server);
			}
			return false;
		}

		addrptr = (unsigned int **)host->h_addr_list;

		while (*addrptr != NULL) {//�X���C�h7��4
			serverinfo.sin_addr.S_un.S_addr = *(*addrptr);

			//�ڑ������̏ꍇ���[�v�𔲂���
			if (connect(sock,
				(struct sockaddr *)&serverinfo,
				sizeof(serverinfo)) == 0) {
				break;
			}

			addrptr++;//connect�����s�����玟�̃A�h���X��
		}

		//connect���S�Ď��s�����ꍇ
		if (*addrptr == NULL) {
			fprintf_s(stderr, "connect : %d\n", WSAGetLastError());
			return false;
		}
	}
	else {//IP�A�h���X�̏ꍇ�̐ڑ�
		  //inet_addr()������������ = addr ��IP�A�h���X�ŕ\������Ă�����

		  //connect�����s������G���[��\�����ďI��
		if (connect(sock,
			(struct sockaddr *)&serverinfo,
			sizeof(serverinfo)) != 0) {
			fprintf_s(stderr, "connect : %d\n", WSAGetLastError());
			return false;
		}
	}

	isConnectedFlag = true;
	return true;//connect����
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