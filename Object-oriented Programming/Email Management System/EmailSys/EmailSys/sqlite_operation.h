#ifndef _SQLITE_OPERATION
#define _SQLITE_OPERATION
#include<iostream>
#include <string>
using namespace std;

int Insert_Record(char* record);

int Check_Account(string address);

int Login_Check_Account(string address, string password);

void Email_list(string address);

void Show_Email(string ID);

void Email_Send(string address, string receiver, string title, string content,string path);

void Email_Delete(string ID);

void Email_Sign(string ID);

#endif
