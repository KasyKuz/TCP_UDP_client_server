// udpserver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


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
#include <map>
#include <set>
#include <iomanip>
#define udp_mass 65507
int sendToClient(SOCKET sock, int cli_num);
using namespace std;
struct sockaddr_in addr;

void writetofile(char* buf, SOCKET sock, char client[1024]);
struct client {
	unsigned int numbers[20];
	int i_n;
	int sent;
	char id[1024];
	struct sockaddr_in* adress;
	int port;
};
struct client all_clients[256];
int all_cl = 0;

//source\repos\udpserver\x64\Debug
// udpserver.exe 8700 8703
//ruby udpclientemul.rb 192.168.0.102:8700 file1.txt



int client_detection(char *idi, struct sockaddr_in* addri , int port,int lenn) {
	for (int i = 0; i < all_cl; i++) {
		if (strncmp(all_clients[i].id,idi,lenn)==0) {
			return i;
		}
		else {
		//	printf("%s %s %d\n", all_clients[i].id, idi, strncmp(all_clients[i].id, idi, lenn));
		}
	}
	strncpy(all_clients[all_cl].id, idi,lenn);
	all_clients[all_cl].i_n = 0;
	all_clients[all_cl].sent = 0;
	all_clients[all_cl].port = port;
	all_cl++;
	return -1;
}

int find_number(char *idi, unsigned int number) {
	for (int i = 0; i < all_cl; i++) {
		if (strcmp(all_clients[i].id, idi)==0) {
			for (int j = 0; j < all_clients[i].i_n;j++) {
				if (all_clients[i].numbers[j] == number) {
					return 0;
				}
			}
			all_clients[i].numbers[all_clients[i].i_n] = number;
			all_clients[i].i_n++;
			return 1;
		}
	}
	return -1;
}

int analyseDatagram(SOCKET sock)
{
	char* buf = new char[udp_mass];
	
	int len = sizeof(addr);
	int ref = recvfrom(sock, buf, udp_mass, 0, (struct sockaddr*)&addr, &len);

	int addres = ntohl(addr.sin_addr.s_addr);
	int port = ntohs(addr.sin_port);
	
	//printf("%lu.%lu.%lu.%lu:%lu ", (addr.sin_addr.s_addr >> 0) & 0xFF, (addr.sin_addr.s_addr >> 8) & 0xFF,

		//(addr.sin_addr.s_addr >> 16) & 0xFF, (addr.sin_addr.s_addr >> 24) & 0xFF, ntohl(addr.sin_port));
	int cli_num = -1;
	char client[1024];

	sprintf(client, "%hhu.%hhu.%hhu.%hhu:%hu\0", ((addres >> 24) & 0xFF), ((addres >> 16) & 0xFF), ((addres >> 8) & 0xFF), ((addres >> 0) & 0xFF), port);
	//printf("client: %s\n", client);
	unsigned int number = ntohl(((unsigned int*)(buf))[0]);
	//printf("number %d\n", number);
	char* buffer = new char[100];
	memset(buffer, 0, 100);
	if (ref > 0)
	{

		cli_num = client_detection(client, &addr, port, strlen(client));
		if (cli_num == -1)
		{
			cli_num =all_cl-1;
			printf("New client connected: %s\n", client);
		}
		if (find_number(client, number)) {
			//printf("%d\n", number);
			writetofile(buf, sock,client);
		}
		else if (find_number(client, number) == -1) {
			printf("error in searching msg number in massive\n");
		}
		//printf("i_n: %d\n", all_clients[client_detection(client, &addr, 0, strlen(client))].i_n);
		
		//printf("\n");
		if (sendToClient(sock, cli_num) != -1)printf("massage with massive is sent\n");
		else printf("massage with massive is NOT sent\n");
	}

	return cli_num;
	
}
int sendToClient(SOCKET sock,int cli_num)
{
	int i = 0;
	char* buffer = new char[100];
	char client[1024];
	strncpy(client, all_clients[cli_num].id, strlen(all_clients[cli_num].id)+1);
	//printf("send client %s", client);
	//printf("res_detec: %d", client_detection(client, &addr, 0, strlen(client)));
	//printf("i_n: %d\ntratat:", all_clients[client_detection(client, &addr, 0, strlen(client))].i_n);
	//int u = all_clients[client_detection(client, &addr, 0, strlen(client))].sent;
	//int k = 0;
	int k = 0;
	//int u = k;
	for (int j = 0+ all_clients[client_detection(client, &addr, 0, strlen(client))].sent; j < all_clients[client_detection(client, &addr, 0, strlen(client))].i_n; j++) {
		//переделать с 4 байтами
		((unsigned int*)(buffer + i))[0] = htonl(all_clients[client_detection(client, &addr, 0, strlen(client))].numbers[j]);
		//printf("%d ", (all_clients[client_detection(client, &addr, 0, strlen(client))].numbers[j]));
		i += 4;
		//all += 1;
		//all_clients[client_detection(client, &addr, 0, strlen(client))].sent++;
		k++;
	}
	//printf("\nok %d\n", sizeof(buffer));
	//printf("\n");
	int send_to = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, sizeof(addr));
	if (send_to != -1) all_clients[client_detection(client, &addr, 0, strlen(client))].sent += k;
	//printf("send:  %d\n", send_to);
	return send_to;
}
int stop_flag = 0;
void writetofile(char *buf, SOCKET sock,char client[1024]) {
	
	FILE* f = fopen("msg.txt", "a+");
	char msg[1024000];
	unsigned int num = ntohl(((unsigned int*)(buf))[0]);
	unsigned int bbb = ntohl(((unsigned int*)(buf + 10))[0]);
	sprintf(msg, "%s %02hhu:%02hhu:%02hhu %02hhu:%02hhu:%02hhu %u %s\n",client, buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], bbb, buf + 14);
		// printf("%02hhu:%02hhu:%02hhu %02hhu:%02hhu:%02hhu %u %s\n", buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], bbb, buf + 14);

	//printf("%u %s\n", num, msg);
	fprintf(f, "%s", msg);

	if (strcmp(buf + 14, "stop") == 0) {
		stop_flag = 1;
		printf("flag stop\n");
	}
	
	fclose(f);
	//if (stop_flag == 1) return -1;
	//return 1;
}

int init()
{
	WSADATA wsaData;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsaData));
}

int sock_err(const char* function, int s)
{
	int err;
	err = WSAGetLastError();
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
int set_non_block_mode (SOCKET s)
{
	unsigned long mode= 1;
	return 	ioctlsocket	(s, FIONBIO, &mode);

}


int main(int argc, char** argv)
{
	SOCKET sockets[150];
	
	struct sockaddr_in addre[150];

	if (argc < 3 || argc >4)
	{		
		printf("error in number of arguments\n");
		return 0;
	}
	int flags = 0;
	init();
	unsigned int less = atoi(argv[1]);
	unsigned int more = atoi(argv[2])+1;
	WSAEVENT* event = new WSAEVENT[more - less];
	for (int j = 0; j < more - less;j++) {
		sockets[j] = socket(AF_INET, SOCK_DGRAM, 0);		
		//добавить исключения	
		set_non_block_mode(sockets[j]);
		// memset(&addresses[j], 0, sizeof(addresses[j]));
		memset(&addre, 0, sizeof(addre));
		addre[j].sin_family = AF_INET;
		addre[j].sin_port = htons(less + j);
		//addresses[j].sin_family = AF_INET;
		//addresses[j].sin_port = htons(less + j);
	
		addre[j].sin_addr.s_addr = htonl(INADDR_ANY);
		//addresses[j].sin_addr.s_addr = htonl(INADDR_ANY);
		//bind(sockets[j], (struct	sockaddr*)&addresses[j], sizeof(addresses[j]));
		bind(sockets[j], (struct	sockaddr*)&addre[j], sizeof(addre[j]));

		event[j] = WSACreateEvent();
		WSAEventSelect(sockets[j], event[j], FD_READ | FD_WRITE | FD_CLOSE);
		//добавить исключения
		
	}
	
	
	//WSAEVENT* event = new WSAEVENT[more - less];
	//printf("a\n");
	while (true)
	{
		WSANETWORKEVENTS ne;
		DWORD dw = WSAWaitForMultipleEvents(more-less, event, FALSE, 1000, FALSE);
		DWORD res = 0;
		//printf("1\n");
		for (int i = 0; i < more - less; i++)
		{
			
			if ((res = WSAEnumNetworkEvents(sockets[i], event[i], &ne)) == 0)
			{
				if (ne.lNetworkEvents & FD_READ)
				{
					//printf("2\n");
					printf("Listening in  %u \n", less+i);
					int value = analyseDatagram(sockets[i]);
					//printf("value: %d\n", value);
					
					
					if (stop_flag == 1)
					{
					
						printf("stop message detected.Power off....\n");
						for (int i = 0; i < more - less; i++)
						{
							WSACloseEvent(event[i]);
							closesocket(sockets[i]);
						}

						WSACleanup();
						
						return 0;
					}
				}
				if (ne.lNetworkEvents & FD_WRITE) {
					//
				}
				if (ne.lNetworkEvents & FD_CLOSE)
				{
					printf("Remote the side closed the connection \n");
				}
			}
		}
	}
	return 0;

	
}

