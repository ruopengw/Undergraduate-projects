#include<Python.h>
#include<iostream>

using namespace std;
int isSpam(const char* mail);

int main()
{
	const char* mail;
	int flag;
	mail = "watch this";
	flag = isSpam(mail);
	cout << "mail:" <<"'"<< mail <<"'";
	if (flag == 1) cout << " is a spam";
	else cout << " is not a spam";
	return 0;
}

int isSpam(const char* mail)
{
	int res = 0;
	Py_SetPythonHome(L"E:/anaconda/setup");//path to python.exe

	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
	if (!Py_IsInitialized())
	{
		printf("初始化失败！");
		return 0;
	}
	else {
		PyRun_SimpleString("# -*- coding: utf-8 -*-");
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append(r'D:/junior/OOP/proj/ConsoleApplication2')");//add the path to python file to system


		PyObject* pModule = NULL;//声明变量
		PyObject* pFunc = NULL;// 声明变量

		pModule = PyImport_ImportModule("oop_bayes");//file name 
		if (pModule == NULL)
		{
			cout << "没找到该Python文件" << endl;
		}
		else {
			pFunc = PyObject_GetAttrString(pModule, "evluate");//function name 
			PyObject* args = Py_BuildValue("(s)", mail);//给python函数参数赋值

			PyObject* pRet = PyObject_CallObject(pFunc, args);//调用函数
			if (pRet == NULL) cout << "call function fail";

			PyArg_Parse(pRet, "i", &res);//转换返回类型
		}
		Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
	}
	return res;
}
