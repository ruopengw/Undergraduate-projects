#include <iostream>
#include "sqlite3.h"
#include <string>
#include<cstdlib>
#include "sqlite_operation.h"
#pragma comment(lib, "sqlite3.lib")
using namespace std;

void begin();
void Email_Home(string address,string password);
void Email_inbox(string address, string password); \
void Email_Read(string ID);
void Email_write(string address, string password);

class Email{
	char* address;
	char* password;
public:
	Email() {};
	Email(char* Address, char* Password) {
		address = Address; password = Password;
	}
	~Email() {};
	void Register();
		/*连接数据库 查找*/
		/*连接成功Homeshow*/
	void Login();
};


void Email::Register() {
	string address;
	string password;
	string s,input;
	char *sql;
	int check;
	while (true) {
		cout << "请输入用户名（加@oop.com后缀）" ;
		cin >> address;
		/*查询合法性*/
		/*连接数据库查询用户名*/
		/*如果返回值为 ，输入password,插入*/
		check = Check_Account(address);
		if (check==1) {
			cout << "邮箱已存在，请更换注册名!" << endl;
		}
		else {
			cout << "请输入密码：";
			cin >> password;
			cout << "是否确认注册？[确认：Y / 退出：N]" << endl;
			cout << ">>>  " ;
			cin >> input;
			if (input == "Y") {
				s = "INSERT INTO EmailAccount (ADDRESS,PASSWORD) VALUES ('" + address + "','" + password + "');";
				sql = (char*)s.data();
				check=Insert_Record(sql);
				if (check == 1) {
					cout << "注册成功" << endl;
					break;
				}
				else {
					cout << "注册失败" << endl;
					break;
				}
			}
			else {
				break;
			}
		}
	}
}

void Email::Login() {
	int check=0;
	string address, password;
	char input;
	while (check == 0) {
		cout << ">>>  返回：B / 继续登录：C" << endl;
		cout << ">>>  " ;
		cin >> input;
		if (input == 'C') {
			cout << "请输入邮箱地址：";
			cin >> address;
			cout << "请输入密码：";
			cin >> password;
			check=Login_Check_Account(address, password);
			if (check == 0) {
				cout << ">>>  用户名或密码错误！" << endl;
			}
		}
		else if(input=='B') {
			check = 1;
		}
		else {
			check == 0;
		}
	}
	if (check == 1) {
		Email_Home(address,password);
	}
}

int main() {

	begin();

	system("pause");
}

void begin() {
	Email E;
	string option;
	string address, password;

	while (true) {
		cout << ">>>>>> Welcome to the EmailSystem.<<<<<<" << endl;
		cout << "  选项			按键	" << endl;
		cout << "  本地邮箱登录		L1		" << endl;
		cout << "  第三方邮箱登录	L2		" << endl;
		cout << "  注册			R		" << endl;
		cout << "  退出			Q		" << endl;
		cout << ">>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<" << endl;
		cout << "请选择操作：";
		cin >> option;
		if (option.compare("L1") == 0) {
			cout << ">>> Login with local email." << endl;
			E.Login();
			break;
		}
		else if (option.compare("L2") == 0) {
			cout << "Login with other email..." << endl;
			break;
		}
		else if (option.compare("R") == 0) {
			E.Register();
		}
		else if (option.compare("Q") == 0) {
			cout << "Quit..." << endl;
			break;
		}
		else {
			cout << "请检查指令输入！" << endl;
		}
	}
}

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
		Email_write(address, password);
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
		cout << ">>>  返回：B  \  删除：D \ 标为已读：F" << endl;
		cout << ">>>  ";
		cin >> input;
		if (input == 'B') {
			break;
		}
		else if (input == 'D') {
			Email_Delete(ID);
		}
		else if (input == 'F') {
			Email_Sign(ID);
		}
	}
}

void Email_write(string address, string password) {
	string receiver;
	string content;
	string title;
	string path;
	char input1,input2;
	int check=0;
	while (check == 0) {
		cout << ">>>  返回：B" << endl;
		cout << ">>>  收件人：";
		cin >> receiver;
		if (receiver == "B") {
			Email_Home(address,password);
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
			cin >> input1;
			if (input1=='Y')
			{
				cout << "请输入附件路径：";
				cin >> path;
			}
			cout << "确认发送：[Y/N]" << endl;
			cin >> input2;
			if (input2 == 'N') {
				Email_Home(address, password);
				break;
			}
			Email_Send(address, receiver, title, content,path);
			Email_Home(address, password);
			break;
		}
	}
}