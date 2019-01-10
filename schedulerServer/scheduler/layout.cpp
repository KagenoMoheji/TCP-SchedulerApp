#include "stdafx.h"
#include "scheduler.h"


void mainLayout() {
	app = new App;
	app->w_height = 450;
	app->w_width = 450;

	colorSet();
	drawLayout();
	gtk_widget_show_all(app->window);
}


void drawLayout() {
	char text[CHARBUFF];
	gchar *gLabel;

	//ウィンドウ生成
	app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(app->window, app->w_width, app->w_height);
	sprintf_s(text, "Scheduler");
	gLabel = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
	gtk_window_set_title(GTK_WINDOW(app->window), gLabel);
	gtk_window_set_position(GTK_WINDOW(app->window), GTK_WIN_POS_CENTER);
	gtk_widget_modify_bg(app->window, GTK_STATE_NORMAL, &(app->lightgray));//https://www.kksou.com/php-gtk2/sample-codes/set-the-background-color-of-GtkWindow.php
	gtk_container_set_border_width(GTK_CONTAINER(app->window), 5);

	//パーツの初期化
	/*app->button = NULL;
	app->frame = NULL;
	app->label = NULL;
	app->vbox = NULL;
	app->hbox = NULL;*/

	//列の定義
	app->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(app->window), app->vbox);

	//1行目のframe
	app->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->vbox), app->hbox, FALSE, FALSE, 0);


	app->frame = gtk_frame_new("IP Address Setting");
	gtk_box_pack_start(GTK_BOX(app->hbox), app->frame, TRUE, FALSE, 0);

	app->framevbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(app->frame), app->framevbox);

	app->framehbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);//frame内の1行目
	gtk_box_pack_start(GTK_BOX(app->framevbox), app->framehbox, FALSE, FALSE, 0);
	app->label = gtk_label_new("Server IP Address : ");
	gtk_widget_set_size_request(app->label, 100, 20);
	gtk_misc_set_alignment(GTK_MISC(app->label), 0, 0.5);//左寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->label, FALSE, FALSE, 0);
	getOwnIPAddress();
	cout << "OwnIPAddress : " << serverIP << endl;
	app->label = gtk_label_new(serverIP);
	gtk_widget_set_size_request(app->label, 200, 20);
	gtk_misc_set_alignment(GTK_MISC(app->label), 0, 0.5);//左寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->label, FALSE, FALSE, 10);

	/*
	app->framehbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->framevbox), app->framehbox, FALSE, FALSE, 0);
	app->label = gtk_label_new("Server IP Address : ");
	gtk_widget_set_size_request(app->label, 100, 20);
	gtk_misc_set_alignment(GTK_MISC(app->label), 0, 0.5);//左寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->label, FALSE, FALSE, 0);
	app->serverEntry = gtk_entry_new();
	gtk_widget_set_size_request(app->serverEntry, 200, 20);
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->serverEntry, FALSE, FALSE, 20);
	*/

	app->framehbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->framevbox), app->framehbox, FALSE, FALSE, 0);
	app->label = gtk_label_new("Port : ");
	gtk_widget_set_size_request(app->label, 30, 20);
	gtk_misc_set_alignment(GTK_MISC(app->label), 0, 0.5);//左寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->label, FALSE, FALSE, 0);
	app->portEntry = gtk_entry_new();
	gtk_widget_set_size_request(app->portEntry, 30, 20);
	gtk_misc_set_alignment(GTK_MISC(app->label), 0, 0.5);//左寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->portEntry, FALSE, FALSE, 0);

	app->framehbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->framevbox), app->framehbox, FALSE, FALSE, 0);
	app->button = gtk_button_new_with_label("try connection");
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->button, FALSE, FALSE, 0);
	g_signal_connect(app->button, "clicked", G_CALLBACK(tryConnection), NULL);
	app->connectGuide = gtk_label_new("Set a Port Number...");//エラーとか
	gtk_widget_set_size_request(app->connectGuide, 200, 20);
	gtk_misc_set_alignment(GTK_MISC(app->connectGuide), 0, 0.5);//左寄せ
	gtk_widget_modify_fg(app->connectGuide, GTK_STATE_NORMAL, &(app->red));//フォント色
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->connectGuide, FALSE, FALSE, 10);//「10」は左右の？スペース
	app->button = gtk_button_new_with_label("Disconnect");
	gtk_misc_set_alignment(GTK_MISC(app->button), 1, 0.5);//右寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->button, FALSE, FALSE, 0);
	g_signal_connect(app->button, "clicked", G_CALLBACK(disconnect), NULL);



	//2行目のフレーム
	app->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->vbox), app->hbox, FALSE, TRUE, 0);

	app->frame = gtk_frame_new("2018 / 07");
	app->frame = drawCalendar(app->frame, app->w_width, app->w_height);
	gtk_box_pack_start(GTK_BOX(app->hbox), app->frame, FALSE, TRUE, 0);



	//3行目
	app->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->vbox), app->hbox, FALSE, FALSE, 0);

	app->frame = gtk_frame_new("");
	gtk_box_pack_start(GTK_BOX(app->hbox), app->frame, TRUE, FALSE, 0);
	app->framevbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(app->frame), app->framevbox);
	app->framehbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(app->framevbox), app->framehbox, FALSE, FALSE, 0);

	app->button = gtk_button_new_with_label("Send Schedule");
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->button, FALSE, FALSE, 0);
	g_signal_connect(app->button, "clicked", G_CALLBACK(sendSchedule), NULL);
	app->schedulingGuide = gtk_label_new("You hasn't connected yet...");//"Waiting Client's Schedule..."とかエラーとか
	gtk_widget_set_size_request(app->schedulingGuide, 200, 20);
	gtk_misc_set_alignment(GTK_MISC(app->schedulingGuide), 0, 0.5);//左寄せ
	gtk_widget_modify_fg(app->schedulingGuide, GTK_STATE_NORMAL, &(app->red));//フォント色
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->schedulingGuide, FALSE, FALSE, 10);
	app->button = gtk_button_new_with_label("Clear Schedule");
	gtk_misc_set_alignment(GTK_MISC(app->button), 1, 0.5);//右寄せ
	gtk_box_pack_start(GTK_BOX(app->framehbox), app->button, FALSE, FALSE, 0);
	g_signal_connect(app->button, "clicked", G_CALLBACK(clearSchedule), NULL);




	g_signal_connect(app->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

};




static GtkWidget* drawCalendar(GtkWidget* parent, int w_width, int w_height) {
	GtkWidget *frame, *label, *grid;

	grid = gtk_grid_new();
	gtk_widget_set_size_request(grid, w_width, 1);
	gtk_container_add(GTK_CONTAINER(parent), grid);

	label = gtk_label_new("Sun.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	label = gtk_label_new("Mon.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 1, 1);
	label = gtk_label_new("Tue.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 2, 0, 1, 1);
	label = gtk_label_new("Wed.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 3, 0, 1, 1);
	label = gtk_label_new("Thu.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 4, 0, 1, 1);
	label = gtk_label_new("Fri.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 5, 0, 1, 1);
	label = gtk_label_new("Sat.");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 6, 0, 1, 1);

	app->eventbox1 = gtk_event_box_new();//eventbox//http://lmj.nagaokaut.ac.jp/~lu/programming/gtk_tut_ja/gtk_tut_ja-17.html
	frame = gtk_frame_new("");
	label = gtk_label_new("1");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox1), label);//eventbox
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox1);//eventbox
	gtk_widget_set_events(app->eventbox1, GDK_BUTTON_PRESS_MASK);//eventbox
	g_signal_connect(app->eventbox1, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox1);//eventbox
	gtk_grid_attach(GTK_GRID(grid), frame, 0, 1, 1, 1);
	app->eventbox2 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("2");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox2), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox2);
	gtk_widget_set_events(app->eventbox2, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox2, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox2);
	gtk_grid_attach(GTK_GRID(grid), frame, 1, 1, 1, 1);
	app->eventbox3 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("3");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox3), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox3);
	gtk_widget_set_events(app->eventbox3, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox3, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox3);
	gtk_grid_attach(GTK_GRID(grid), frame, 2, 1, 1, 1);
	app->eventbox4 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("4");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox4), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox4);
	gtk_widget_set_events(app->eventbox4, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox4, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox4);
	gtk_grid_attach(GTK_GRID(grid), frame, 3, 1, 1, 1);
	app->eventbox5 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("5");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox5), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox5);
	gtk_widget_set_events(app->eventbox5, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox5, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox5);
	gtk_grid_attach(GTK_GRID(grid), frame, 4, 1, 1, 1);
	app->eventbox6 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("6");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox6), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox6);
	gtk_widget_set_events(app->eventbox6, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox6, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox6);
	gtk_grid_attach(GTK_GRID(grid), frame, 5, 1, 1, 1);
	app->eventbox7 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("7");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox7), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox7);
	gtk_widget_set_events(app->eventbox7, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox7, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox7);
	gtk_grid_attach(GTK_GRID(grid), frame, 6, 1, 1, 1);

	app->eventbox8 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("8");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox8), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox8);
	gtk_widget_set_events(app->eventbox8, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox8, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox8);
	gtk_grid_attach(GTK_GRID(grid), frame, 0, 2, 1, 1);
	app->eventbox9 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("9");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox9), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox9);
	gtk_widget_set_events(app->eventbox9, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox9, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox9);
	gtk_grid_attach(GTK_GRID(grid), frame, 1, 2, 1, 1);
	app->eventbox10 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("10");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox10), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox10);
	gtk_widget_set_events(app->eventbox10, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox10, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox10);
	gtk_grid_attach(GTK_GRID(grid), frame, 2, 2, 1, 1);
	app->eventbox11 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("11");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox11), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox11);
	gtk_widget_set_events(app->eventbox11, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox11, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox11);
	gtk_grid_attach(GTK_GRID(grid), frame, 3, 2, 1, 1);
	app->eventbox12 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("12");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox12), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox12);
	gtk_widget_set_events(app->eventbox12, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox12, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox12);
	gtk_grid_attach(GTK_GRID(grid), frame, 4, 2, 1, 1);
	app->eventbox13 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("13");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox13), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox13);
	gtk_widget_set_events(app->eventbox13, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox13, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox13);
	gtk_grid_attach(GTK_GRID(grid), frame, 5, 2, 1, 1);
	app->eventbox14 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("14");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox14), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox14);
	gtk_widget_set_events(app->eventbox14, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox14, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox14);
	gtk_grid_attach(GTK_GRID(grid), frame, 6, 2, 1, 1);

	app->eventbox15 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("15");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox15), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox15);
	gtk_widget_set_events(app->eventbox15, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox15, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox15);
	gtk_grid_attach(GTK_GRID(grid), frame, 0, 3, 1, 1);
	app->eventbox16 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("16");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox16), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox16);
	gtk_widget_set_events(app->eventbox16, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox16, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox16);
	gtk_grid_attach(GTK_GRID(grid), frame, 1, 3, 1, 1);
	app->eventbox17 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("17");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox17), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox17);
	gtk_widget_set_events(app->eventbox17, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox17, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox17);
	gtk_grid_attach(GTK_GRID(grid), frame, 2, 3, 1, 1);
	app->eventbox18 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("18");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox18), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox18);
	gtk_widget_set_events(app->eventbox18, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox18, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox18);
	gtk_grid_attach(GTK_GRID(grid), frame, 3, 3, 1, 1);
	app->eventbox19 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("19");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox19), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox19);
	gtk_widget_set_events(app->eventbox19, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox19, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox19);
	gtk_grid_attach(GTK_GRID(grid), frame, 4, 3, 1, 1);
	app->eventbox20 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("20");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox20), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox20);
	gtk_widget_set_events(app->eventbox20, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox20, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox20);
	gtk_grid_attach(GTK_GRID(grid), frame, 5, 3, 1, 1);
	app->eventbox21 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("21");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox21), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox21);
	gtk_widget_set_events(app->eventbox21, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox21, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox21);
	gtk_grid_attach(GTK_GRID(grid), frame, 6, 3, 1, 1);

	app->eventbox22 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("22");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox22), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox22);
	gtk_widget_set_events(app->eventbox22, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox22, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox22);
	gtk_grid_attach(GTK_GRID(grid), frame, 0, 4, 1, 1);
	app->eventbox23 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("23");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox23), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox23);
	gtk_widget_set_events(app->eventbox23, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox23, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox23);
	gtk_grid_attach(GTK_GRID(grid), frame, 1, 4, 1, 1);
	app->eventbox24 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("24");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox24), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox24);
	gtk_widget_set_events(app->eventbox24, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox24, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox24);
	gtk_grid_attach(GTK_GRID(grid), frame, 2, 4, 1, 1);
	app->eventbox25 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("25");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox25), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox25);
	gtk_widget_set_events(app->eventbox25, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox25, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox25);
	gtk_grid_attach(GTK_GRID(grid), frame, 3, 4, 1, 1);
	app->eventbox26 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("26");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox26), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox26);
	gtk_widget_set_events(app->eventbox26, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox26, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox26);
	gtk_grid_attach(GTK_GRID(grid), frame, 4, 4, 1, 1);
	app->eventbox27 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("27");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox27), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox27);
	gtk_widget_set_events(app->eventbox27, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox27, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox27);
	gtk_grid_attach(GTK_GRID(grid), frame, 5, 4, 1, 1);
	app->eventbox28 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("28");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox28), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox28);
	gtk_widget_set_events(app->eventbox28, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox28, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox28);
	gtk_grid_attach(GTK_GRID(grid), frame, 6, 4, 1, 1);

	app->eventbox29 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("29");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox29), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox29);
	gtk_widget_set_events(app->eventbox29, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox29, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox29);
	gtk_grid_attach(GTK_GRID(grid), frame, 0, 5, 1, 1);
	app->eventbox30 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("30");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox30), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox30);
	gtk_widget_set_events(app->eventbox30, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox30, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox30);
	gtk_grid_attach(GTK_GRID(grid), frame, 1, 5, 1, 1);
	app->eventbox31 = gtk_event_box_new();
	frame = gtk_frame_new("");
	label = gtk_label_new("31");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_container_add(GTK_CONTAINER(app->eventbox31), label);
	gtk_container_add(GTK_CONTAINER(frame), app->eventbox31);
	gtk_widget_set_events(app->eventbox31, GDK_BUTTON_PRESS_MASK);
	g_signal_connect_swapped(app->eventbox31, "button_press_event", G_CALLBACK(schedule_sw), app->eventbox31);
	gtk_grid_attach(GTK_GRID(grid), frame, 2, 5, 1, 1);


	return parent;
}


void colorSet() {
	gdk_color_parse("red", &(app->red));
	gdk_color_parse("green", &(app->green));
	gdk_color_parse("gray", &(app->gray));
	gdk_color_parse("lightgray", &(app->lightgray));
	gdk_color_parse("blue", &(app->blue));
	gdk_color_parse("black", &(app->black));
}

void updateCalendar() {
	GtkWidget *updateEventbox;
	updateEventbox = NULL;

	for (int i = 0; i < DAYCNT; i++) {
		switch (i + 1) {
		case 1:
			updateEventbox = app->eventbox1;
			break;
		case 2:
			updateEventbox = app->eventbox2;
			break;
		case 3:
			updateEventbox = app->eventbox3;
			break;
		case 4:
			updateEventbox = app->eventbox4;
			break;
		case 5:
			updateEventbox = app->eventbox5;
			break;
		case 6:
			updateEventbox = app->eventbox6;
			break;
		case 7:
			updateEventbox = app->eventbox7;
			break;
		case 8:
			updateEventbox = app->eventbox8;
			break;
		case 9:
			updateEventbox = app->eventbox9;
			break;
		case 10:
			updateEventbox = app->eventbox10;
			break;
		case 11:
			updateEventbox = app->eventbox11;
			break;
		case 12:
			updateEventbox = app->eventbox12;
			break;
		case 13:
			updateEventbox = app->eventbox13;
			break;
		case 14:
			updateEventbox = app->eventbox14;
			break;
		case 15:
			updateEventbox = app->eventbox15;
			break;
		case 16:
			updateEventbox = app->eventbox16;
			break;
		case 17:
			updateEventbox = app->eventbox17;
			break;
		case 18:
			updateEventbox = app->eventbox18;
			break;
		case 19:
			updateEventbox = app->eventbox19;
			break;
		case 20:
			updateEventbox = app->eventbox20;
			break;
		case 21:
			updateEventbox = app->eventbox21;
			break;
		case 22:
			updateEventbox = app->eventbox22;
			break;
		case 23:
			updateEventbox = app->eventbox23;
			break;
		case 24:
			updateEventbox = app->eventbox24;
			break;
		case 25:
			updateEventbox = app->eventbox25;
			break;
		case 26:
			updateEventbox = app->eventbox26;
			break;
		case 27:
			updateEventbox = app->eventbox27;
			break;
		case 28:
			updateEventbox = app->eventbox28;
			break;
		case 29:
			updateEventbox = app->eventbox29;
			break;
		case 30:
			updateEventbox = app->eventbox30;
			break;
		case 31:
			updateEventbox = app->eventbox31;
			break;
		}

		if ((freeFlags[i] == 1) && (oppoFreeFlags[i])) {
			gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->green));
		}
		else if ((!freeFlags[i]) && (!oppoFreeFlags[i])) {
			gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->red));
		}
		else if ((!freeFlags[i]) && (oppoFreeFlags[i])) {
			gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->lightgray));
		}
		else {
			gtk_widget_modify_bg(updateEventbox, GTK_STATE_NORMAL, &(app->gray));
		}
	}
}