#include<string>
#include<iostream>
#include "sqlite_operation.h"
#include"Email_operation.h"
#include"mail.h"
#include"Email.h"
/*
void Email_Home(string address, string password) {
	cout << "----------------------------------------------------" << endl;
	cout << ">>>		Welcome," << address << "		<<<" << endl;
	cout << ">>>  退出登录：T" << endl;
	cout << ">>>  写信：W | 收件箱：Q " << endl;
	cout << "----------------------------------------------------" << endl;
	char input;
	cout << ">>>  ";
	cin >> input;
	if (input == 'T') {
		begin();
	}
	else if (input == 'Q') {
		cout << "----------------------------------------------------" << endl;
		cout << ">>> >>> >>> >>>	InBox	<<< <<< <<< <<<" << endl;
		cout << "----------------------------------------------------" << endl;
		Email_inbox(address, password);
	}
	else if (input == 'W') {
		cout << "----------------------------------------------------" << endl;
		cout << ">>> >>> >>> >>>	Write Email	<<< <<< <<< <<<" << endl;
		cout << "----------------------------------------------------" << endl;
		Email_write1(address, password);
	}
}

void Email_inbox(string address, string password) {
	string input;
	Email_list(address);
	while (true) {
		cout << ">>>  返回：B / 输入ID阅读" << endl;
		cout << ">>>  ";
		cin >> input;
		if (input == "B") {
			Email_Home(address, password);
			break;
		}
		else {
			Email_Read(input);
		}
	}
}

void Email_Read(string ID) {
	char input;
	Show_Email(ID);
	while (true) {
		cout << ">>>  返回：B " << endl;
		cout << ">>>  ";
		cin >> input;
		if (input == 'B') {
			break;
		}
	}
}

void Email_write1(string address, string password) {
	string receiver;
	string content;
	string title;
	string path = " ";
	char input;
	int check = 0;
	while (check == 0) {
		cout << ">>>  返回：B" << endl;
		cout << ">>>  收件人：";
		cin >> receiver;
		if (receiver == "B") {
			Email_Home(address, password);
			break;
		}
		check = Check_Account(receiver);
		if (check == 0) {
			cout << "收件人地址不存在！" << endl;
		}
		else {
			cout << ">>>  标题：";
			cin >> title;
			cout << ">>>  正文：" << endl;
			cin >> content;
			cout << "是否添加附件？[Y/N]" << endl;
			cin >> input;
			if (input == 'Y')
			{
				cout << "请输入附件路径：";
				cin >> path;
			}
			cout << "确认发送：[Y/N]" << endl;
			cin >> input;
			if (input == 'N') {
				continue;
			}
			Email_Send(address, receiver, title, content, path);
			Email_Home(address, password);
		}
	}
}
*/