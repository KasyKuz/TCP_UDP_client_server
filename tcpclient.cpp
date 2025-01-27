
/* #define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <stdint.h>
#define TCP_MAX_SIZE 65507

using namespace std;

int ok_num = 0;
int str_num = 0;
int current_str = 0;
int st = 0;
char* ip;
char* port;
void ok_func(SOCKET s);
int N = 0;

int init()
{
	WSADATA wsaData;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsaData));
}



void time_append(char* b, char t[8], int j) {
	char cur_time[2] = { 0 };
	int i = 0;
	int tu;
	while (i < 8) {
		cur_time[i % 3] = t[i];
		i++;
		if (i % 3 == 2) {

			b[j] = (cur_time[0] - '0') * 10 + cur_time[1] - '0';
			j++;
			i++;
		}
	}
}
void append_net_bites(char* b, u_long t, int c) {
	((unsigned int*)(b + c))[0] = t;// в сетевой порядок байт
}
int know_len(char* currentMsg, int j) {
	int i = j;
	int c = 0;
	for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

	{

		c += 1;

	}
	//printf("c = %d\n", c);
	if (c == 10240 - 15) {
		return -1;
	}
	else { return 1; }
}
bool send_all(int sock, const char* buffer, size_t length) {
	size_t total_sent = 0;
	while (total_sent < length) {
		int sent = send(sock, buffer + total_sent, length - total_sent, 0);
		if (sent < 0) {
			//std::cerr << "Failed to send data" << std::endl;
			printf("failed to send data\n");
			return false;
		}
		total_sent += sent;
	}
	return true;
}
int stop_check(char* buf, int i) {
	if (buf[i] == 's' && buf[i + 1] == 't' && buf[i + 2] == 'o' && buf[i + 3] == 'p') {
		return 1;
	}
	return -1;
}
bool transferData(int s, char* currentMsg)
{


	char bu[10240];
	char time1[8];

	int i = 0;
	//append_net_bites(bu, htonl(str_num),0);
	//unsigned int num = ntohl(htonl(str_num));
	//printf("%u %u\n", num, htonl(str_num));
	uint32_t a = htonl(str_num);
	((unsigned int*)bu)[0] = a;
	while (currentMsg[i] != ' ') {
		time1[i] = currentMsg[i];
		i++;
	}
	i++;
	time_append(bu, time1, 4);
	while (currentMsg[i] != ' ') {
		time1[i - 9] = currentMsg[i];
		i++;
	}
	time_append(bu, time1, 7);
	i++;
	int c = 0;
	char ost[11];

	while (currentMsg[i] != ' ') {
		ost[c] = currentMsg[i];
		i++;
		c++;
	}
	i++;
	ost[c] = '\0';
	char* end_shift = strchr(currentMsg + 18, ' ') - 1;

	//((unsigned int*)(bu + 10))[0] = htonl(strtoul(currentMsg + 18, &end_shift, 10));// в сетевой порядок байт
	//unsigned long int  bbb = strtoul(currentMsg + 18, &end_shift, 10);
	//((unsigned int*)(bu+10))[0] = htonl(bbb);

	//unsigned int num = ntohl(htonl(bbb));
	//printf("%u %u\n", num, htonl(str_num));
	unsigned long bbb = strtoul(ost, NULL, 10);
	append_net_bites(bu, htonl(bbb), 10);

	int j = 14;
	//i--;
	//bu[j] = currentMsg[i];
	//i++;
	if (know_len(currentMsg, i) == 1) {
		for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

		{

			bu[j] = currentMsg[i];
			j++;

		}
		printf("correct\n");
	}


	//j++;
	st = stop_check(bu, j - 4);
	bu[j] = '\0';
	j++;

	//printf("%u\n", j);
	if (!send_all(s, bu, j)) {
		printf("send error\n");
		return false;
	}
	else { printf("send\n"); }
	int oki = ok_num;
	ok_func(s);
	if (oki == ok_num) {
		return false;
	}
	return true;

}

void ok_func(SOCKET s) {
	char ok[3] = { 0 };
	while (1)
	{
		recv(s, ok, 1, 0);
		recv(s, ok + 1, 1, 0);
		if (strcmp(ok, "ok") == 0)
		{
			ok_num++;
			printf("a1 - ok\n");

			return;
		}
	}
}
int try_to_connect() {
	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return -1;

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	bool isConnected = false;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			return s;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}
}
int sock_err(const char* function, int s)
{
	int err;
	err = WSAGetLastError();
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
//tcpclient2.exe 192.168.0.102:9000 file1.txt
//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[102400];
	int i = 1;
	int oki = 0;
	while ((fgets(msg, 102400, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%d len=\n", strlen(msg));
			printf("%s\n", msg);
			if (transferData(s, msg) == false) {
				printf("retry to connect\n");
				closesocket(s);
				s = try_to_connect();
				if (s == -1)
				{
					fclose(f);
					return -1;
				}
				else {
					int startMessaging;
					startMessaging = send(s, "p", 1, 0);
					startMessaging == send(s, "u", 1, 0);
					startMessaging == send(s, "t", 1, 0);
					if (startMessaging == -1)
					{
						fclose(f);
						closesocket(s);
						WSACleanup();
						printf("invalid in sending 'put'\n");
						return -1;

					}
					if (transferData(s, msg) == false) { return -1; }
				}
			}


			//oki = ok_num;
			//ok_func(s);
			
			str_num++;
		}
		char msg[102400];
		if (feof(f)) {
			printf("feof\n");
			break;
		}




	}
	fclose(f);
	printf("close file\n");
	return 1;

}

void first_read(char* filename, int s)
{
	FILE* f = fopen(filename, "a+");
	//printf("%s ", filename);
	if (f == NULL)
	{
		printf("file null\n"); return;
	}
	printf("filereading are begun\n");
	char msg[102400];
	int i = 1;
	while ((fgets(msg, 102400, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		//printf("a %s\n", msg);
		if (strlen(msg) > 1) {

			str_num++;
		}

		if (feof(f)) {
			printf("feof\n");
			break;
		}




	}
	fclose(f);
	printf("close file\n");

}



int main(int argc, char** argv)
{
	//char t[2] = { '3','4' };

	bool isConnected = false;
	if (argc < 3 || argc >4)
	{

		printf("error in number of arguments\n");
		return 0;
	}
	ip = strtok(argv[1], ":");
	port = strtok(NULL, "\0");

	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return sock_err("socket", s);

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			break;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return 0;
	}
	else {

		int startMessaging;
		startMessaging = send(s, "p", 1, 0);
		startMessaging == send(s, "u", 1, 0);
		startMessaging == send(s, "t", 1, 0);
		if (startMessaging == -1)
		{
			closesocket(s);
			WSACleanup();
			printf("invalid in sending 'put'\n");
			return sock_err("connect", s);

		}
	}

	//printf("%s", argv[2]);
	first_read(argv[2], s);
	int r;
	if (str_num != 0) {
		N = str_num;
		str_num = 0;
		r = readFile(argv[2], s);

		//transferData(s);
		int t = 0;
		while (str_num != ok_num && t < 10) {
			printf("nums of ok - %d %d", ok_num, str_num);
			ok_func(s);
			t++;
			//Sleep(100);
		}
	}
	
	if (r == 1) {
		closesocket(s);
		WSACleanup();
	}
	closesocket(s);
	WSACleanup();
	return 0;
}*/
/* 
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <stdint.h>
#define TCP_MAX_SIZE 65507

using namespace std;

int ok_num = 0;
int str_num = 0;
int current_str = 0;
int st = 0;
char* ip;
char* port;
void ok_func(SOCKET s);
int N = 0;

int init()
{
	WSADATA wsaData;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsaData));
}



void time_append(char* b, char t[8], int j) {
	char cur_time[2] = { 0 };
	int i = 0;
	int tu;
	while (i < 8) {
		cur_time[i % 3] = t[i];
		i++;
		if (i % 3 == 2) {

			b[j] = (cur_time[0] - '0') * 10 + cur_time[1] - '0';
			j++;
			i++;
		}
	}
}
void append_net_bites(char* b, u_long t, int c) {
	((unsigned int*)(b + c))[0] = t;// в сетевой порядок байт
}
int know_len(char* currentMsg, int j) {
	int i = j;
	int c = 0;
	for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

	{

		c += 1;

	}
	//printf("c = %d\n", c);
	if (c == 10240 - 15) {
		return -1;
	}
	else { return 1; }
}
bool send_all(int sock, const char* buffer, size_t length) {
	size_t total_sent = 0;
	while (total_sent < length) {
		int sent = send(sock, buffer + total_sent, length - total_sent, 0);
		if (sent < 0) {
			//std::cerr << "Failed to send data" << std::endl;
			printf("failed to send data\n");
			return false;
		}
		total_sent += sent;
	}
	return true;
}
int stop_check(char* buf, int i) {
	if (buf[i] == 's' && buf[i + 1] == 't' && buf[i + 2] == 'o' && buf[i + 3] == 'p') {
		return 1;
	}
	return -1;
}
bool transferData(int s, char* currentMsg)
{


	char bu[10240];
	char time1[8];

	int i = 0;
	//append_net_bites(bu, htonl(str_num),0);
	//unsigned int num = ntohl(htonl(str_num));
	//printf("%u %u\n", num, htonl(str_num));
	uint32_t a = htonl(str_num);
	((unsigned int*)bu)[0] = a;
	while (currentMsg[i] != ' ') {
		time1[i] = currentMsg[i];
		i++;
	}
	i++;
	time_append(bu, time1, 4);
	while (currentMsg[i] != ' ') {
		time1[i - 9] = currentMsg[i];
		i++;
	}
	time_append(bu, time1, 7);
	i++;
	int c = 0;
	char ost[11];

	while (currentMsg[i] != ' ') {
		ost[c] = currentMsg[i];
		i++;
		c++;
	}
	i++;
	ost[c] = '\0';
	char* end_shift = strchr(currentMsg + 18, ' ') - 1;

	//((unsigned int*)(bu + 10))[0] = htonl(strtoul(currentMsg + 18, &end_shift, 10));// в сетевой порядок байт
	//unsigned long int  bbb = strtoul(currentMsg + 18, &end_shift, 10);
	//((unsigned int*)(bu+10))[0] = htonl(bbb);

	//unsigned int num = ntohl(htonl(bbb));
	//printf("%u %u\n", num, htonl(str_num));
	unsigned long bbb = strtoul(ost, NULL, 10);
	append_net_bites(bu, htonl(bbb), 10);

	int j = 14;
	//i--;
	//bu[j] = currentMsg[i];
	//i++;
	if (know_len(currentMsg, i) == 1) {
		for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

		{

			bu[j] = currentMsg[i];
			j++;

		}
		printf("correct\n");
	}


	//j++;
	st = stop_check(bu, j - 4);
	bu[j] = '\0';
	j++;

	//printf("%u\n", j);
	//int sent = send(s, bu, j, 0);
	if (!send_all(s, bu, j)) {
		printf("send error\n");
		return false;
	}
	else { printf("send\n"); }
	int oki = ok_num;
	ok_func(s);
	
	return true;

}

void ok_func(SOCKET s) {
	char ok[3] = { 0 };
	while (1)
	{
		recv(s, ok, 1, 0);
		recv(s, ok + 1, 1, 0);
		if (strcmp(ok, "ok") == 0)
		{
			ok_num++;
			printf("a1 - ok\n");
			printf("%s\n", ok);

			return;
		}
	}
}
int try_to_connect() {
	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return -1;

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	bool isConnected = false;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			return s;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}
}
int sock_err(const char* function, int s)
{
	int err;
	err = WSAGetLastError();
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
//tcpclient2.exe 192.168.0.102:9000 file1.txt
//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[102400];
	int i = 1;
	int oki = 0;
	while ((fgets(msg, 102400, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%d len=\n", strlen(msg));
			printf("%s\n", msg);
			if (transferData(s, msg) == false) {
				printf("retry to connect\n");
				closesocket(s);
				s = try_to_connect();
				if (s == -1)
				{
					fclose(f);
					return -1;
				}
				else {
					int startMessaging;
					startMessaging = send(s, "p", 1, 0);
					startMessaging == send(s, "u", 1, 0);
					startMessaging == send(s, "t", 1, 0);
					if (startMessaging == -1)
					{
						fclose(f);
						closesocket(s);
						WSACleanup();
						printf("invalid in sending 'put'\n");
						return -1;

					}
					if (transferData(s, msg) == false) { return -1; }
				}
			}


			
			str_num++;
		}
		char msg[102400];
		if (feof(f)) {
			printf("feof\n");
			break;
		}




	}
	fclose(f);
	printf("close file\n");
	return 1;

}

void first_read(char* filename, int s)
{
	FILE* f = fopen(filename, "a+");
	//printf("%s ", filename);
	if (f == NULL)
	{
		printf("file null\n"); return;
	}
	printf("filereading are begun\n");
	char msg[102400];
	int i = 1;
	while ((fgets(msg, 102400, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		//printf("a %s\n", msg);
		if (strlen(msg) > 1) {

			str_num++;
		}

		if (feof(f)) {
			printf("feof\n");
			break;
		}




	}
	fclose(f);
	printf("close file\n");

}



int main(int argc, char** argv)
{
	//char t[2] = { '3','4' };

	bool isConnected = false;
	if (argc < 3 || argc >4)
	{

		printf("error in number of arguments\n");
		return 0;
	}
	ip = strtok(argv[1], ":");
	port = strtok(NULL, "\0");

	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return sock_err("socket", s);

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			break;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return 0;
	}
	else {

		int startMessaging;
		startMessaging = send(s, "p", 1, 0);
		startMessaging == send(s, "u", 1, 0);
		startMessaging == send(s, "t", 1, 0);
		if (startMessaging == -1)
		{
			closesocket(s);
			WSACleanup();
			printf("invalid in sending 'put'\n");
			return sock_err("connect", s);

		}
	}

	//printf("%s", argv[2]);
	first_read(argv[2], s);
	int r;
	if (str_num != 0) {
		N = str_num;
		str_num = 0;
		r = readFile(argv[2], s);

		//transferData(s);
		int t = 0;
		while (str_num != ok_num && t < 10) {
			printf("nums of ok - %d %d", ok_num, str_num);
			ok_func(s);
			t++;
			//Sleep(100);
		}
	}
	
	if (r == 1) {
		closesocket(s);
		WSACleanup();
	}
	closesocket(s);
	WSACleanup();
	return 0;
}*/
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <stdint.h>
#define TCP_MAX_SIZE 65507

using namespace std;

int ok_num = 0;
int str_num = 0;
int current_str = 0;
int st = 0;
char* ip;
char* port;
void ok_func(SOCKET s);
int N = 0;

int init()
{
	WSADATA wsaData;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsaData));
}



void time_append(char* b, char t[8], int j) {
	char cur_time[2] = { 0 };
	int i = 0;
	int tu;
	while (i < 8) {
		cur_time[i % 3] = t[i];
		i++;
		if (i % 3 == 2) {

			b[j] = (cur_time[0] - '0') * 10 + cur_time[1] - '0';
			j++;
			i++;
		}
	}
}
void append_net_bites(char* b, u_long t, int c) {
	((unsigned int*)(b + c))[0] = t;// в сетевой порядок байт
}
int know_len(char* currentMsg, int j) {
	int i = j;
	int c = 0;
	for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

	{

		c += 1;

	}
	//printf("c = %d\n", c);
	if (c == 10240 - 15) {
		return -1;
	}
	else { return 1; }
}
bool send_all(int sock, const char* buffer, size_t length) {
	size_t total_sent = 0;
	while (total_sent < length) {
		int sent = send(sock, buffer + total_sent, length - total_sent, 0);
		if (sent < 0) {
			//std::cerr << "Failed to send data" << std::endl;
			printf("failed to send data\n");
			return false;
		}
		total_sent += sent;
	}
	return true;
}
int stop_check(char* buf, int i) {
	if (buf[i] == 's' && buf[i + 1] == 't' && buf[i + 2] == 'o' && buf[i + 3] == 'p') {
		return 1;
	}
	return -1;
}
bool transferData(int s, char* currentMsg)
{


	char bu[10240];
	char time1[8];

	int i = 0;
	//append_net_bites(bu, htonl(str_num),0);
	//unsigned int num = ntohl(htonl(str_num));
	//printf("%u %u\n", num, htonl(str_num));
	uint32_t a = htonl(str_num);
	((unsigned int*)bu)[0] = a;
	while (currentMsg[i] != ' ') {
		time1[i] = currentMsg[i];
		printf("%c ", time1[i]);
		i++;
	}
	i++;

	time_append(bu, time1, 4);
	//printf("append time1\n");
	while (currentMsg[i] != ' ') {
		time1[i - 9] = currentMsg[i];
		i++;
	}
	time_append(bu, time1, 7);
	//	printf("append time2\n");
	i++;
	int c = 0;
	char ost[11];

	while (currentMsg[i] != ' ') {
		ost[c] = currentMsg[i];
		i++;
		c++;
	}
	i++;
	ost[c] = '\0';
	char* end_shift = strchr(currentMsg + 18, ' ') - 1;

	//((unsigned int*)(bu + 10))[0] = htonl(strtoul(currentMsg + 18, &end_shift, 10));// в сетевой порядок байт
	//unsigned long int  bbb = strtoul(currentMsg + 18, &end_shift, 10);
	//((unsigned int*)(bu+10))[0] = htonl(bbb);

	//unsigned int num = ntohl(htonl(bbb));
	//printf("%u %u\n", num, htonl(str_num));
	unsigned long bbb = strtoul(ost, NULL, 10);
	append_net_bites(bu, htonl(bbb), 10);

	int j = 14;
	//i--;
	//bu[j] = currentMsg[i];
	//i++;
	if (know_len(currentMsg, i) == 1) {
		for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && i < 10240 - 15; i++)

		{

			bu[j] = currentMsg[i];
			j++;

		}
		printf("correct\n");
	}


	//j++;
	st = stop_check(bu, j - 4);
	bu[j] = '\0';
	j++;

	//printf("%u\n", j);
	//int sent = send(s, bu, j, 0);
	if (!send_all(s, bu, j)) {
		printf("send error\n");
		return false;
	}
	else { printf("send\n"); }
	int oki = ok_num;
	ok_func(s);
	
	return true;

}

void ok_func(SOCKET s) {
	char ok[3] = { 0 };
	while (1)
	{
		recv(s, ok, 1, 0);
		recv(s, ok + 1, 1, 0);
		if (strcmp(ok, "ok") == 0)
		{
			ok_num++;
			printf("a1 - ok\n");
			printf("%s\n", ok);

			return;
		}
	}
}
int try_to_connect() {
	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return -1;

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	bool isConnected = false;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			return s;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}
}
int sock_err(const char* function, int s)
{
	int err;
	err = WSAGetLastError();
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
//tcpclient2.exe 192.168.0.102:9000 file1.txt
//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s)
{
	FILE* f = fopen(filename, "a+");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char* msg = (char*)malloc(1000000);
	int i = 1;
	int oki = 0;
	while ((fgets(msg, 1000000, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			printf("%d len=\n", strlen(msg));
			//printf("%s\n", msg);
			if (transferData(s, msg) == false) {
				printf("retry to connect\n");
				closesocket(s);
				s = try_to_connect();
				if (s == -1)
				{
					fclose(f);
					return -1;
				}
				else {
					int startMessaging;
					startMessaging = send(s, "p", 1, 0);
					startMessaging == send(s, "u", 1, 0);
					startMessaging == send(s, "t", 1, 0);
					if (startMessaging == -1)
					{
						fclose(f);
						closesocket(s);
						WSACleanup();
						printf("invalid in sending 'put'\n");
						return -1;

					}
					if (transferData(s, msg) == false) { return -1; }
				}
			}


			//oki = ok_num;
			//ok_func(s);
			/* int t = 0;
			while (ok_num == oki && t < 10) {
				transferData(s, msg);
				oki = ok_num;
				ok_func(s);
				t++;
			}
			//if (ok_num==oki)
			if (ok_num == str_num) {
				fclose(f);

				return;
			}
			else {
				printf("strnum %d oknum %d", str_num, ok_num);
			}
			//	*/
			str_num++;
		}
		memset(msg, 0, 1000000);
		if (feof(f)) {
			printf("feof\n");
			break;
		}




	}
	fclose(f);
	printf("close file\n");
	return 1;

}

void first_read(char* filename, int s)
{
	printf("try to open file\n");
	FILE* f = fopen(filename, "a+");
	//printf("%s ", filename);
	if (f == NULL)
	{
		printf("file null\n"); return;
	}
	printf("filereading are begun\n");
	char* msg = (char*)malloc(1000000);
	int i = 1;
	while ((fgets(msg, 102400, f)) != NULL) {
		//	printf("i = %d\n", i);
		i += 1;
		printf("a %s\n", msg);
		if (strlen(msg) > 1) {

			str_num++;
		}
		/*
		if (feof(f)) {
			printf("feof\n");
			break;
		}*/
		memset(msg, 0, 1000000);




	}
	fclose(f);
	printf("close file\n");

}



int main(int argc, char** argv)
{
	//char t[2] = { '3','4' };

	bool isConnected = false;
	if (argc < 3 || argc >4)
	{

		printf("error in number of arguments\n");
		return 0;
	}
	ip = strtok(argv[1], ":");
	port = strtok(NULL, "\0");

	struct sockaddr_in addr;
	init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		return sock_err("socket", s);

	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//for (int connAttempt = 0; connAttempt < 10 && !isConnected; connAttempt++)
	int count = 0;
	while (count != 10)
	{
		if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1)
		{
			isConnected = true;
			printf("success in connection\n");
			break;
		}
		else
		{
			printf("try to connect...\n");
			Sleep((DWORD)100);
		}
		count++;
	}
	if (!isConnected)
	{
		printf("Connection refused\n");
		closesocket(s);
		WSACleanup();
		return 0;
	}
	else {

		int startMessaging;
		startMessaging = send(s, "p", 1, 0);
		startMessaging == send(s, "u", 1, 0);
		startMessaging == send(s, "t", 1, 0);
		if (startMessaging == -1)
		{
			closesocket(s);
			WSACleanup();
			printf("invalid in sending 'put'\n");
			return sock_err("connect", s);

		}
	}

	printf("%s", argv[2]);
	first_read(argv[2], s);
	printf("str_num %d\n", str_num);
	int r;
	if (str_num != 0) {
		N = str_num;
		str_num = 0;
		r = readFile(argv[2], s);

		//transferData(s);
		int t = 0;
		while (str_num != ok_num && t < 10) {
			printf("nums of ok - %d %d", ok_num, str_num);
			ok_func(s);
			t++;
			//Sleep(100);
		}
	}
	/*
	if (st == 1 && connect(s, (struct sockaddr*)&addr, sizeof(addr)) != -1) {
		printf("connection????\n");
		while (1) {
			readFile(argv[2], s);
			if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
				break;.
			}
		}
	}*/
	if (r == 1) {
		closesocket(s);
		WSACleanup();
	}
	closesocket(s);
	WSACleanup();
	return 0;
}