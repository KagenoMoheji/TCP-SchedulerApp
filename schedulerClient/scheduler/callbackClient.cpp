#include "stdafx.h"
#include "scheduler.h"

void schedule_sw(GtkWidget* eventbox, gpointer data) {
	//各ラベルのフラグ配列（長さ31）を用意して，そのフラグ（0もしくは1）で条件分岐して背景色の変更・OK日リストの追加/排除を行う	
	int labelNum = 0;
	GtkWidget *updateEventbox;
	updateEventbox = NULL;
	char text[CHARBUFF];
	const gchar *gLabel;

	if (app->eventbox1 == eventbox) {
		labelNum = 0;//配列のインデックスに対応させるため「日にち―1」とする
		updateEventbox = app->eventbox1;
	}
	else if (app->eventbox2 == eventbox) {
		labelNum = 1;
		updateEventbox = app->eventbox2;
	}
	else if (app->eventbox3 == eventbox) {
		labelNum = 2;
		updateEventbox = app->eventbox3;
	}
	else if (app->eventbox4 == eventbox) {
		labelNum = 3;
		updateEventbox = app->eventbox4;
	}
	else if (app->eventbox5 == eventbox) {
		labelNum = 4;
		updateEventbox = app->eventbox5;
	}
	else if (app->eventbox6 == eventbox) {
		labelNum = 5;
		updateEventbox = app->eventbox6;
	}
	else if (app->eventbox7 == eventbox) {
		labelNum = 6;
		updateEventbox = app->eventbox7;
	}
	else if (app->eventbox8 == eventbox) {
		labelNum = 7;
		updateEventbox = app->eventbox8;
	}
	else if (app->eventbox9 == eventbox) {
		labelNum = 8;
		updateEventbox = app->eventbox9;
	}
	else if (app->eventbox10 == eventbox) {
		labelNum = 9;
		updateEventbox = app->eventbox10;
	}
	else if (app->eventbox11 == eventbox) {
		labelNum = 10;
		updateEventbox = app->eventbox11;
	}
	else if (app->eventbox12 == eventbox) {
		labelNum = 11;
		updateEventbox = app->eventbox12;
	}
	else if (app->eventbox13 == eventbox) {
		labelNum = 12;
		updateEventbox = app->eventbox13;
	}
	else if (app->eventbox14 == eventbox) {
		labelNum = 13;
		updateEventbox = app->eventbox14;
	}
	else if (app->eventbox15 == eventbox) {
		labelNum = 14;
		updateEventbox = app->eventbox15;
	}
	else if (app->eventbox16 == eventbox) {
		labelNum = 15;
		updateEventbox = app->eventbox16;
	}
	else if (app->eventbox17 == eventbox) {
		labelNum = 16;
		updateEventbox = app->eventbox17;
	}
	else if (app->eventbox18 == eventbox) {
		labelNum = 17;
		updateEventbox = app->eventbox18;
	}
	else if (app->eventbox19 == eventbox) {
		labelNum = 18;
		updateEventbox = app->eventbox19;
	}
	else if (app->eventbox20 == eventbox) {
		labelNum = 19;
		updateEventbox = app->eventbox20;
	}
	else if (app->eventbox21 == eventbox) {
		labelNum = 20;
		updateEventbox = app->eventbox21;
	}
	else if (app->eventbox22 == eventbox) {
		labelNum = 21;
		updateEventbox = app->eventbox22;
	}
	else if (app->eventbox23 == eventbox) {
		labelNum = 22;
		updateEventbox = app->eventbox23;
	}
	else if (app->eventbox24 == eventbox) {
		labelNum = 23;
		updateEventbox = app->eventbox24;
	}
	else if (app->eventbox25 == eventbox) {
		labelNum = 24;
		updateEventbox = app->eventbox25;
	}
	else if (app->eventbox26 == eventbox) {
		labelNum = 25;
		updateEventbox = app->eventbox26;
	}
	else if (app->eventbox27 == eventbox) {
		labelNum = 26;
		updateEventbox = app->eventbox27;
	}
	else if (app->eventbox28 == eventbox) {
		labelNum = 27;
		updateEventbox = app->eventbox28;
	}
	else if (app->eventbox29 == eventbox) {
		labelNum = 28;
		updateEventbox = app->eventbox29;
	}
	else if (app->eventbox30 == eventbox) {
		labelNum = 29;
		updateEventbox = app->eventbox30;
	}
	else if (app->eventbox31 == eventbox) {
		labelNum = 30;
		updateEventbox = app->eventbox31;
	}

	if (freeFlags[labelNum]) {//"1"になっている上で選択されたら解除。
		freeFlags[labelNum] = 0;
		gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->lightgray));//https://hobnote.net/hn_gtk_plus_prog01.html

		sprintf_s(text, "Removed the %d", (labelNum + 1));
		gLabel = text;
		gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
		gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
	}
	else {
		freeFlags[labelNum] = 1;
		gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->gray));

		sprintf_s(text, "Selected the %d", (labelNum + 1));
		gLabel = text;
		gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
		gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
	}


	cout << "clicked! : " << (labelNum + 1) << "日" << endl;

	cout << "[ ";
	for (int i = 0; i < (sizeof(freeFlags) / sizeof(*freeFlags)); i++) {
		cout << freeFlags[i] << " ";
	}
	cout << "]\n";
}


void tryConnection(GtkWidget* button, gpointer data) {
	//entryに入力されたTCP通信の情報を取得
	getServerIP();
	getPort();
	cout << "Inputted Server IP Address : " << serverIP << endl;
	cout << "Inputted Port Number : " << port << endl;


	//スレッドの呼び出し
	TCPThread = g_thread_new("mainTCPClient", mainTCPClient, NULL);
	//スレッドの終了
	g_thread_unref(TCPThread);


	/*
	C++標準のthread使うのは？
	http://kaworu.jpn.org/cpp/std::thread::thread
	https://suikaba.hatenablog.com/entry/2014/08/19/225703
	*
	tcpThread = thread([&] {
		mainTCPClient(NULL);
	});
	if (tcpThread.get_id() != thread::id()) {
		tcpThread.join();
	}
	*/

}

void sendSchedule(GtkWidget* button, gpointer data) {
	char text[CHARBUFF];
	gchar *gLabel;

	cout << "[Send] clicked!" << endl;


	if (isConnected()) {//TCP接続されている時だけ更新
		if (!send_data(reinterpret_cast<const char*>(freeFlags), sizeof(freeFlags))) {
			sprintf_s(text, "Failed in sending data!");
			gLabel = text;
			gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
			gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
		}
		
		updateCalendar();

		sprintf_s(text, "Updated Schedule.");
		gLabel = text;
		gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->blue));
		gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
	}
	else {
		sprintf_s(text, "Failed in updating because of disconnecting!");
		gLabel = text;
		gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
		gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
	}
}

void getServerIP() {
	serverIP = gtk_entry_get_text(GTK_ENTRY(app->serverEntry));
}

void getPort() {
	const char* c_port;
	c_port = gtk_entry_get_text(GTK_ENTRY(app->portEntry));
	port = atoi(c_port);
}

void disconnect() {
	if (isConnected()) {//TCP通信のスレッドが走っていたら切る
		DisConnect();
		cout << "Connect stopped!" << endl;
		//closeTCP = true;

		g_thread_unref(TCPThread);
	}
}

void clearSchedule() {
	char text[CHARBUFF];
	gchar *gLabel;

	if (isConnected()) {//TCP通信中のみ実行できる制約
		//フラグリストを0に初期化
		for (int i = 0; i < DAYCNT; i++) {
			freeFlags[i] = 0;
		}

		updateCalendar();

		if (!send_data(reinterpret_cast<const char*>(freeFlags), sizeof(freeFlags))) {
			sprintf_s(text, "Failed in sending data!");
			gLabel = text;
			gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
			gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
		}
	}
	else {
		sprintf_s(text, "Failed in clearing because of disconnecting!");
		gLabel = text;
		gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));
		gtk_label_set_label(GTK_LABEL(app->schedulingGuide), gLabel);
	}
}