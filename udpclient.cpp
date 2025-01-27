/*
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int str_num=0;
int ok_num=0;
int sizes[6500];
unsigned int recv_response(int s)
{
  char datagram[1024];
  struct timeval tv = {0, 100*1000};// 100 msec
  int res;
  fd_set fds;
  FD_ZERO (&fds);
  FD_SET (s, &fds);// Проверка -если в сокете входящие дейтаграммы
// (ожидание в течение tv)
  res = select( s+ 1, &fds, 0, 0, &tv);
  if( res> 0)
  {
	// Данные есть, считывание и
	struct sockaddr_in addr;
	int addrlen = sizeof (addr);
	int received = 0;
	//int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
	if ( received <= 0){ // Ошибка считывания полученной дейтаграммы
	  //sock_err("recvfrom",  s);
	  return 0;
	}
	//return get_addr_from_dns_datagram
  }
  else if (res == 0)
  {

	// Данных в сокете нет, возврат ошибки
	return 0;
  }
  else
  {
	//sock_err("select",s);
	return 0;
  }

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
void transferData(int s, char* currentMsg, char all_list[650][10240])
{

 // printf("1\n");
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
for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && j<1239; i++)


	{

		bu[j] = currentMsg[i];
		j++;

	}

	//j++;
	bu[j] = '\0';
	j++;
  for (int i=0;i<j;i++){
	all_list[str_num][i] = bu[i];
   //printf("a\n");
  }
  sizes[str_num] = j;
  //all_list[str_num] = bu;
	//printf("%s\n", bu);



}


//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s, char all_list[650][10240], int *control)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[10240];
while ((fgets(msg, 256, f)) != NULL) {
		//printf("a\n");
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%s\n", msg);
			transferData(s, msg, all_list);
	  printf("%s\n", msg);
	  control[str_num]=1;
			//ok_func(s);
			str_num++;
		}
	char msg[10240];

}
	fclose(f);

}*/
/*
cd Downloads\stubs

 ruby udpserveremul.rb 8700

  ./hel2 192.168.0.102:8700 file1.txt
*/
/*
int main(int argc, char** argv)
{
  printf("11\n");
  if (argc<3 || argc>4){
	printf("error in number of arguments\n");
	return -1;
  }
  char* ip = strtok(argv[1], ":");
  char* port = strtok(NULL, "\0");
  struct sockaddr_in addr;
 // printf("12\n");
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
  {
	//cout << "socket() function error" << endl;
	printf("socket error\n");
	return -1;

  }
  printf("ccccccc\n");
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(port));
  addr.sin_addr.s_addr = inet_addr(ip);
  //printf("13\n");
  char all_list[650][10240];
 // char all_list[2][3];
  int list_control[652];
  for (int i=0;i<65200;i++){
	list_control[i]=0;
  }
  //printf("2\n");
  printf("bbbbbbbbbbbbbbbb\n");
  int res = readFile(argv[2], s, all_list,list_control);
  //res=1;
  int datagram_send=0;
  printf("aaaaa\n");
  while (true){
	unsigned int i=0;
	while (i<str_num){
	  printf("%d\n",i);
	  if (list_control[i]==1){

		int result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
		if (result > 0){
		  struct timeval tv = { 0, 100 * 1000 };
		  fd_set fds;
		  FD_ZERO(&fds);
		  FD_SET(s, &fds);
		  int res = select(s + 1, &fds, 0, 0, &tv);
		  if (res <= 0)
		  {
			if (res<0 )printf("error in select\n");
			else if (res==0) printf("none data in socket \n");
			close(s);
			 exit(0);

		  }

		  unsigned int datagram[20] = { };
		  socklen_t addrlen = sizeof(addr);
		  int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
		  if (received <= 0)
		  {
			printf("error in reading datagtamm\n");
			close(s);
			 exit(0);
			//return;

		  }
		  else{
			for (int j = 0; j < 20; j++)

			{

			  //unsigned int net_num = 0;
			  unsigned int net_num =htonl(datagram[j]);

			  if ( list_control[net_num]==1)
			  {
				printf("find number in datargram %d\n", net_num);

				list_control[net_num] = 0;

				datagram_send+=1;

			  }

			}
		  }
		}
		else printf("error sendto\n");
		close(s);
		exit(0);
	  }
	  if (datagram_send==20 || datagram_send==str_num){
		close(s);
		exit(0);
	  }
	  printf("%d \n", datagram_send);
	  i++;
	}

  }
  //printf("Hello");
}

*/
/* 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int str_num = 0;
int ok_num = 0;
int sizes[6500];
unsigned int recv_response(int s)
{
	char datagram[1024];
	struct timeval tv = { 0, 100 * 1000 };// 100 msec
	int res;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(s, &fds);// Проверка -если в сокете входящие дейтаграммы  
	// (ожидание в течение tv)
	res = select(s + 1, &fds, 0, 0, &tv);
	if (res > 0)
	{
		// Данные есть, считывание и
		struct sockaddr_in addr;
		int addrlen = sizeof(addr);
		int received = 0;
		//int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
		if (received <= 0) { // Ошибка считывания полученной дейтаграммы 
			//sock_err("recvfrom",  s);
			return 0;
		}
		//return get_addr_from_dns_datagram 
	}
	else if (res == 0)
	{

		// Данных в сокете нет, возврат ошибки
		return 0;
	}
	else
	{
		//sock_err("select",s);
		return 0;
	}

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
void transferData(int s, char* currentMsg, char all_list[100][10240])
{

	// printf("1\n");
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
	for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && j < 1239; i++)


	{

		bu[j] = currentMsg[i];
		j++;

	}

	//j++;
	bu[j] = '\0';
	j++;
	for (int i = 0; i < j; i++) {
		all_list[str_num][i] = bu[i];
		//printf("a\n");
	}
	sizes[str_num] = j;
	//all_list[str_num] = bu;
	  //printf("%s\n", bu);



}


//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s, char all_list[100][10240], int* control)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[10240];
	while ((fgets(msg, 10240, f)) != NULL) {
		//printf("a\n");
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%s\n", msg);
			transferData(s, msg, all_list);
			printf("%s\n", msg);
			control[str_num] = 1;
			//ok_func(s);
			str_num++;
		}
		char msg[10240];
		if (feof(f)) {
			printf("feof\n");
			break;
		}


	}
	fclose(f);

}

int main(int argc, char** argv)
{
	printf("11\n");
	if (argc < 3 || argc>4) {
		printf("error in number of arguments\n");
		return -1;
	}
	char* ip = strtok(argv[1], ":");
	char* port = strtok(NULL, "\0");
	struct sockaddr_in addr;
	// printf("12\n");
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		printf("socket error\n");
		return -1;

	}
	printf("ccccccc\n");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//printf("13\n");
	char all_list[100][10240];
	// char all_list[2][3];
	int list_control[100];
	for (int i = 0; i < 100; i++) {
		list_control[i] = 1;
	}
	//printf("2\n");
	printf("bbbbbbbbbbbbbbbb\n");
	int res = readFile(argv[2], s, all_list, list_control);
	//res=1;
	int datagram_send = 0;
	printf("%d str_num\n", str_num);
	printf("aaaaa\n");
	while (true) {
		unsigned int i = 0;
		while (i < str_num) {
			//printf("%d\n", i);
			if (list_control[i] == 1) {
				int result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
				if (result < 0) {
					printf("error sendto\n");
					printf("was closed2");
					close(s);
					exit(0);
				}
				printf("msg number %d was sent \n", i);
			}
			i++;
		}
				
		struct timeval tv = { 0, 100 * 1000 };
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		int res = select(s + 1, &fds, 0, 0, &tv);
		if (res > 0) {
			unsigned int datagram[20] = { };
			socklen_t addrlen = sizeof(addr);
			int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
			if (received <= 0){
				printf("error in reading datagtamm\n");
				close(s);
				exit(0);
				//return;
			}
			else {
				for (int j = 0; j < 20 || j < str_num; j++)
				{
								//unsigned int net_num = 0;
					unsigned int net_num = htonl(datagram[j]);

					if (list_control[net_num] == 1)
					{
						printf("find number in datargram %d\n", net_num);
						list_control[net_num] = 0;
						datagram_send += 1;

					}
					if (datagram_send == 20 || datagram_send == str_num) {
						printf("all need was sent. closed11 connection");
						close(s);
						exit(0);
					}

				}
			}
		}
		else
		{
			if (res < 0)printf("error in select\n");
			else if (res == 0) printf("none data in socket \n");
			printf("was closed3");
			close(s);
			exit(0);

		}
		if (datagram_send == 20 || datagram_send == str_num) {
			printf("all need was sent. closed1 connection");
			close(s);
			exit(0);
		}
		
		
		

	}
	printf("was closed");
	close(s);

}
//printf("Hello");

//./udpclient 192.168.0.101:8700 msg.txt



*/
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int str_num=0;
int ok_num=0;
int sizes[6500];
unsigned int recv_response(int s)
{
  char datagram[1024];
  struct timeval tv = {0, 100*1000};// 100 msec
  int res;
  fd_set fds;
  FD_ZERO (&fds);
  FD_SET (s, &fds);// Проверка -если в сокете входящие дейтаграммы
// (ожидание в течение tv)
  res = select( s+ 1, &fds, 0, 0, &tv);
  if( res> 0)
  {
	// Данные есть, считывание и
	struct sockaddr_in addr;
	int addrlen = sizeof (addr);
	int received = 0;
	//int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
	if ( received <= 0){ // Ошибка считывания полученной дейтаграммы
	  //sock_err("recvfrom",  s);
	  return 0;
	}
	//return get_addr_from_dns_datagram
  }
  else if (res == 0)
  {

	// Данных в сокете нет, возврат ошибки
	return 0;
  }
  else
  {
	//sock_err("select",s);
	return 0;
  }

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
void transferData(int s, char* currentMsg, char all_list[650][10240])
{

 // printf("1\n");
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
for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && j<1239; i++)


	{

		bu[j] = currentMsg[i];
		j++;

	}

	//j++;
	bu[j] = '\0';
	j++;
  for (int i=0;i<j;i++){
	all_list[str_num][i] = bu[i];
   //printf("a\n");
  }
  sizes[str_num] = j;
  //all_list[str_num] = bu;
	//printf("%s\n", bu);



}


//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s, char all_list[650][10240], int *control)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[10240];
while ((fgets(msg, 256, f)) != NULL) {
		//printf("a\n");
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%s\n", msg);
			transferData(s, msg, all_list);
	  printf("%s\n", msg);
	  control[str_num]=1;
			//ok_func(s);
			str_num++;
		}
	char msg[10240];

}
	fclose(f);

}*/
/*
cd Downloads\stubs

 ruby udpserveremul.rb 8700

  ./hel2 192.168.0.102:8700 file1.txt
*/
/*
int main(int argc, char** argv)
{
  printf("11\n");
  if (argc<3 || argc>4){
	printf("error in number of arguments\n");
	return -1;
  }
  char* ip = strtok(argv[1], ":");
  char* port = strtok(NULL, "\0");
  struct sockaddr_in addr;
 // printf("12\n");
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
  {
	//cout << "socket() function error" << endl;
	printf("socket error\n");
	return -1;

  }
  printf("ccccccc\n");
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(port));
  addr.sin_addr.s_addr = inet_addr(ip);
  //printf("13\n");
  char all_list[650][10240];
 // char all_list[2][3];
  int list_control[652];
  for (int i=0;i<65200;i++){
	list_control[i]=0;
  }
  //printf("2\n");
  printf("bbbbbbbbbbbbbbbb\n");
  int res = readFile(argv[2], s, all_list,list_control);
  //res=1;
  int datagram_send=0;
  printf("aaaaa\n");
  while (true){
	unsigned int i=0;
	while (i<str_num){
	  printf("%d\n",i);
	  if (list_control[i]==1){

		int result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
		if (result > 0){
		  struct timeval tv = { 0, 100 * 1000 };
		  fd_set fds;
		  FD_ZERO(&fds);
		  FD_SET(s, &fds);
		  int res = select(s + 1, &fds, 0, 0, &tv);
		  if (res <= 0)
		  {
			if (res<0 )printf("error in select\n");
			else if (res==0) printf("none data in socket \n");
			close(s);
			 exit(0);

		  }

		  unsigned int datagram[20] = { };
		  socklen_t addrlen = sizeof(addr);
		  int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
		  if (received <= 0)
		  {
			printf("error in reading datagtamm\n");
			close(s);
			 exit(0);
			//return;

		  }
		  else{
			for (int j = 0; j < 20; j++)

			{

			  //unsigned int net_num = 0;
			  unsigned int net_num =htonl(datagram[j]);

			  if ( list_control[net_num]==1)
			  {
				printf("find number in datargram %d\n", net_num);

				list_control[net_num] = 0;

				datagram_send+=1;

			  }

			}
		  }
		}
		else printf("error sendto\n");
		close(s);
		exit(0);
	  }
	  if (datagram_send==20 || datagram_send==str_num){
		close(s);
		exit(0);
	  }
	  printf("%d \n", datagram_send);
	  i++;
	}

  }
  //printf("Hello");
}

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int str_num = 0;
int ok_num = 0;
int sizes[6500];
unsigned int recv_response(int s)
{
	char datagram[1024];
	struct timeval tv = { 0, 100 * 1000 };// 100 msec
	int res;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(s, &fds);// Проверка -если в сокете входящие дейтаграммы  
	// (ожидание в течение tv)
	res = select(s + 1, &fds, 0, 0, &tv);
	if (res > 0)
	{
		// Данные есть, считывание и
		struct sockaddr_in addr;
		int addrlen = sizeof(addr);
		int received = 0;
		//int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
		if (received <= 0) { // Ошибка считывания полученной дейтаграммы 
			//sock_err("recvfrom",  s);
			return 0;
		}
		//return get_addr_from_dns_datagram 
	}
	else if (res == 0)
	{

		// Данных в сокете нет, возврат ошибки
		return 0;
	}
	else
	{
		//sock_err("select",s);
		return 0;
	}

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
void transferData(int s, char* currentMsg, char all_list[650][10240])
{

	// printf("1\n");
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
	for (; currentMsg[i] != '\n' && currentMsg[i] != EOF && j < 1239; i++)


	{

		bu[j] = currentMsg[i];
		j++;

	}

	//j++;
	bu[j] = '\0';
	j++;
	for (int i = 0; i < j; i++) {
		all_list[str_num][i] = bu[i];
		//printf("a\n");
	}
	sizes[str_num] = j;
	//all_list[str_num] = bu;
	  //printf("%s\n", bu);



}


//добавить игнорирование пустой строки в файле
int readFile(char* filename, int s, char all_list[650][10240], int* control)
{
	FILE* f = fopen(filename, "r");

	if (f == 0)
	{
		printf("file null\n"); return -1;
	}
	printf("filereading are begun\n");
	char msg[10240];
	while ((fgets(msg, 256, f)) != NULL) {
		//printf("a\n");
		//printf("%s\n", msg);
		if (strlen(msg) > 1) {
			//printf("%s\n", msg);
			transferData(s, msg, all_list);
			printf("%s\n", msg);
			control[str_num] = 1;
			//ok_func(s);
			str_num++;
		}
		char msg[10240];

	}
	fclose(f);

}
/*
cd Downloads\stubs

 ruby udpserveremul.rb 8700

  ./hel2 192.168.0.102:8700 file1.txt
*/
int main(int argc, char** argv)
{
	printf("11\n");
	if (argc < 3 || argc>4) {
		printf("error in number of arguments\n");
		return -1;
	}
	char* ip = strtok(argv[1], ":");
	char* port = strtok(NULL, "\0");
	struct sockaddr_in addr;
	// printf("12\n");
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		//cout << "socket() function error" << endl;
		printf("socket error\n");
		return -1;

	}
	printf("ccccccc\n");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	//printf("13\n");
	char all_list[650][10240];
	// char all_list[2][3];
	int list_control[652];
	for (int i = 0; i < 65200; i++) {
		list_control[i] = 1;
	}
	//printf("2\n");
	printf("bbbbbbbbbbbbbbbb\n");
	int res = readFile(argv[2], s, all_list, list_control);
	//res=1;
	int datagram_send = 0;
	printf("%d str_num\n", str_num);
	printf("aaaaa\n");
	int res0 = 0;
	while (true && res0 != 11) {
		unsigned int i = 0;
		while (i < str_num) {
			printf("%d\n", i);
			if (list_control[i] == 1) {

				int result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
				if (result > 0) {
					struct timeval tv = { 0, 100 * 1000 };
					fd_set fds;
					FD_ZERO(&fds);
					FD_SET(s, &fds);
					int res = select(s + 1, &fds, 0, 0, &tv);

					if (res > 0) {
						unsigned int datagram[20] = { };
						socklen_t addrlen = sizeof(addr);
						int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*)&addr, &addrlen);
						if (received <= 0)
						{
							printf("error in reading datagtamm\n");
							close(s);
							exit(0);
							//return;

						}
						else {
							for (int j = 0; j < 20 || j < str_num; j++)

							{

								//unsigned int net_num = 0;
								unsigned int net_num = htonl(datagram[j]);

								if (list_control[net_num] == 1)
								{
									if (str_num == 1) {}
									/*
									int result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
									printf("loop 0\n");
									if (result >0){
										printf("loop\n");
										while (result !=0){
											result = sendto(s, all_list[i], sizes[i], MSG_NOSIGNAL, (struct sockaddr*)&addr, sizeof(addr));
										}
									}
									if (result <=0){
										printf("1 find number in datargram %d\n", net_num);

										list_control[net_num] = 0;

										datagram_send += 1;
									}

								}
								else{
									printf("find number in datargram %d\n", net_num);

									list_control[net_num] = 0;

									datagram_send += 1;
								}*/
									printf("find number in datargram %d\n", net_num);

									list_control[net_num] = 0;

									datagram_send += 1;


								}
								if (datagram_send == 20 || datagram_send == str_num) {
									printf("was closed11");
									close(s);
									exit(0);
								}

							}
						}
					}
					else
					{
						if (res < 0) {
							printf("error in select\n"); printf("was closed3");
							close(s);
							exit(0);
						}
						else if (res == 0) {
							printf("none data in socket \n");
							res0 += 1;
						}
					}

				}

				else {
					printf("error sendto\n");
					printf("was closed2");
					close(s);
					exit(0);
				}

			}
			if (datagram_send == 20 || datagram_send == str_num) {
				printf("was closed1");
				close(s);
				exit(0);
			}
			printf("%d \n", datagram_send);
			i++;
		}


	}
	printf("was closed");
	close(s);

}
//printf("Hello");

//./udpclient 192.168.0.101:8700 msg.txt



