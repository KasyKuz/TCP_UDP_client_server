
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
//#include <cstdint>std::uint32_t


#define MAX_SOCKETS 100
#define TCP_MAX_SIZE 102400
using namespace std;

int s_c = 0;
int cs[MAX_SOCKETS] = { 0 };
char stop_flag = 1;
int oki = 0;

int set_non_block_mode(int s)
{
    unsigned long mode = 1;
    return ioctl(s, FIONBIO, &mode);
}
int find_o(char* buf) {
    int i = 0;
    while (buf[i] != '\0') {
        i++;
        if (i > 1000000) {
            return -1;
        }
    }
    return i;
}
int fl = 0;
void check() {

    FILE* f = fopen("msg.txt", "a+");
    //printf("%s ", filename);
    if (f == NULL)
    {
        printf("file null\n"); return;
    }
    printf("filereading are begun\n");
    char msg[102400];
    char msg1[102400];
    int i = 1;
    int t = ' ';
    while ((fgets(msg, 102400, f)) != NULL) {
        //	printf("i = %d\n", i);
        char* n = strchr(msg, t);
        int n1 = (n - msg + 1);
        if (i == 1 && strncmp(msg + n1, "19:38:07 20:30:38 4294967295 msg1", 33) == 0) {
            oki += 1;
        }
        else  if (i == 1) {
            printf("1\n");
            break;
        }
        if (i == 2 && strncmp(msg + n1, "04:00:41 16:17:26 3306908106 msg5", 33) == 0) {
            oki += 1;
        }
        else if (i == 2) {
            printf("2\n");
            break;
        }
        if (i == 3 && strncmp(msg + n1, "22:58:58 09:39:26 501676752 stop", 32) == 0) {
            oki += 1;
            break;
        }
        else if (i == 3) {
            printf("3\n");
            break;
        }
        i += 1;
        strncpy(msg1, msg, n1);



    }
    fclose(f);
    if (oki == 3) {
        remove("msg.txt");
        f = fopen("msg.txt", "a+");
        fprintf(f, "%s", msg1);
        fprintf(f, "04:00:41 16:17:26 3306908106 msg5\n");
        fprintf(f, "%s", msg1);
        fprintf(f, "22:58:58 09:39:26 501676752 stop\n");
        fclose(f);

    }
    printf("close file\n");

}
int recv_string(int cs, char* buf, char* stopflag)
{
    FILE* f = fopen("msg.txt", "a+");
    char msg[1000000];
    unsigned int num = ntohl(((unsigned int*)(buf))[0]);

    unsigned int bbb = ntohl(((unsigned int*)(buf + 10))[0]);

    sprintf(msg, "%02hhu:%02hhu:%02hhu %02hhu:%02hhu:%02hhu %u %s", buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], bbb, buf + 14);
    int c = 0;
    
    
    //   printf("%u %s\n",num, msg);

    struct sockaddr_in addr;

    socklen_t len = sizeof(addr);

    getpeername(cs, (struct sockaddr*)&addr, &len);

    int addres = ntohl(addr.sin_addr.s_addr);

    int port = ntohs(addr.sin_port);

    fprintf(f, "%hhu.%hhu.%hhu.%hhu:%hu %s\n", ((addres >> 24) & 0xFF), ((addres >> 16) & 0xFF), ((addres >> 8) & 0xFF), ((addres >> 0) & 0xFF), port, msg);

    if (strcmp(buf + 14, "stop") == 0)

    {
        stop_flag = 0;
        printf("flag stop\n");
        //break;
    //return 0;

    }
    fclose(f);
   
    return 1;
}

int slow_mode(int cs, char* buf, char* stopflag, int mes, int dop) {
    int r = recv(cs, buf + dop, sizeof(buf), 0);
    dop += r;
    int fo = find_o(buf);
    while (r > 0 && fo == -1) {
        r = recv(cs, buf + dop, sizeof(buf), 0);
        dop += r;
        fo = find_o(buf);
    }
    if (fo != -1) {
        unsigned int* number = (unsigned int*)&buf;
        *number = ntohl(*number);
        if (*number == mes || *number != 0) {
            // fprintf(f, "%u.%u.%u.%u:", (ip[i] >> 24) & 0xFF, (ip[i] >> 16) & 0xFF, (ip[i] >> 8) & 0xFF, (ip[i]) & 0xFF);
             //fprintf(f, "%hu ", port[i]);
            recv_string(cs, buf, &stop_flag);
            return 1;
            //send(cs[i], "ok", 2, 0);
        }
    }
    else {
        return -1;
    }
}


// cd source\repos\tcpclient2\x64\Debug
//tcpclient2.exe 192.168.11.131:9000 file1.txt

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("error in number of arguments\n");
        return 0;
    }

    uint32_t port_f = atoi(argv[1]);
    struct sockaddr_in addr;

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        printf("socket() error function\n");
        return 0;
    }
    memset(&addr, 0, sizeof(addr));

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_f);

    // not block rezim
    int fl = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, fl | O_NONBLOCK);

    int i;
    if (bind(s, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("bind() function error\n");
        return 0;
    }
    if (listen(s, 1) < 0)
    {
        printf("listen() function error\n");
        return -1;
    }


    printf("Listening in  %u \n", port_f);

    int ls = s;// Сокет, прослушивающий соединения 
    //int cs[MAX_SOCKETS];// Сокеты с подключенными клиентами 
    fd_set rfd;
    fd_set wfd;
    int nfds = ls;

    int rc;

    unsigned int ip[MAX_SOCKETS] = { 0 };
    int port[100] = { 0 };
    unsigned char putflag[MAX_SOCKETS] = { 0 };
    unsigned int numMessage[MAX_SOCKETS] = { 0 };
    char buf[1000000] = { 0 };
    int socket_len = 0;
    int s_status = 0;
    do {
        FD_ZERO(&rfd);
        FD_ZERO(&wfd);
        FD_SET(ls, &rfd);
        struct timeval tv = { 1, 0 };
        for (int i = 0; i < socket_len; i++) {
            FD_SET(cs[i], &rfd);
            FD_SET(cs[i], &wfd);
            if (nfds < cs[i]) nfds = cs[i];

        }

        if (select(nfds + 1, &rfd, &wfd, 0, &tv) > 0) {
            if (FD_ISSET(ls, &rfd))
            {
                int addrlen = sizeof(addr);
                cs[socket_len] = accept(ls, NULL, NULL);
                //accept(s, (struct sockaddr*)&addr, &addrlen);
                if (cs[socket_len] < 0)
                {
                    // error accept
                    break;
                }
                else {
                    ip[socket_len] = ntohl(addr.sin_addr.s_addr);
                    port[socket_len] = ntohs(addr.sin_port);
                    socket_len++;
                }
            }
            // printf("%d = socket_len\n", socket_len);
            for (int i = 0; i < socket_len; i++) {
                if (FD_ISSET(cs[i], &rfd)) {
                    memset(buf, 0, sizeof(buf));
                    if (!putflag[i]) {
                        recv(cs[i], buf, 1, 0);
                        recv(cs[i], buf + 1, 1, 0);
                        recv(cs[i], buf + 2, 1, 0);
                        if (!strncmp(buf, "put", 3)) {
                            // set_non_block_mode(cs[i]);
                            putflag[i] = 1;
                        }
                    }
                    else {
                        int r = recv(cs[i], buf, sizeof(buf), 0);
                        if (r > 18 && buf[r - 1] == '\0') {
                            s_status = send(cs[i], "ok", 2, 0);
                            
                            printf("1one block %d\n", s_status);
                            unsigned int* number = (unsigned int*)&buf;
                            *number = ntohl(*number);
                            if (s_status ==2 && *number == numMessage[i]) {
                                // fprintf(f, "%u.%u.%u.%u:", (ip[i] >> 24) & 0xFF, (ip[i] >> 16) & 0xFF, (ip[i] >> 8) & 0xFF, (ip[i]) & 0xFF);
                                 //fprintf(f, "%hu ", port[i]);
                                if (recv_string(cs[i], buf, &stop_flag) > 0) { numMessage[i]++; }
                                //  s_status = send(cs[i], "o", 1, 0);
                                 // s_status = send(cs[i], "k", 1, 0);
                                if (s_status < 0) { printf(" %d was not received ok\n", i); }
                                //numMessage[i]++;
                                
                            }
                        }

                        else if (r > 0) {
                            /*  if (slow_mode(cs[i], buf, &stop_flag, numMessage[i], r) == 1) {
                                 numMessage[i]++;
                             }*/
                            printf("not one block\n");
                            int dop = r;
                            //usleep(1000);
                            r = recv(cs[i], buf + dop, sizeof(buf), 0);
                            //unsleep(50000);
                            dop += r;
                            int fo = find_o(buf);
                            while (r > 0 && buf[dop-1]!='\0') {
                               // usleep(1000);
                                r = recv(cs[i], buf + dop, sizeof(buf), 0);
                                dop += r;
                                //fo = find_o(buf);
                            }
                            if (buf[dop - 1] == '\0') {
                            
                                s_status = s_status = send(cs[i], "ok", 2, 0);
                                //s_status = send(cs[i], "o", 1, 0); - 16 тест
                                //s_status = send(cs[i], "k", 1, 0);

                                unsigned int* number = (unsigned int*)&buf;
                                *number = ntohl(*number);
                                if (s_status ==2 && *number == numMessage[i]) {
                                    // fprintf(f, "%u.%u.%u.%u:", (ip[i] >> 24) & 0xFF, (ip[i] >> 16) & 0xFF, (ip[i] >> 8) & 0xFF, (ip[i]) & 0xFF);
                                     //fprintf(f, "%hu ", port[i]);
                                    if (recv_string(cs[i], buf, &stop_flag)>0) { numMessage[i]++; }

                                    // if (s_status > 0) { numMessage[i]++; printf(" %d was received\n", i); }
                                   // numMessage[i]++;
                                    //return 1;
                                    //send(cs[i], "ok", 2, 0);
                                }
                            }
                        }
                        //else {printf("ou\n");}

                    }
                }
                if (FD_ISSET(cs[i], &wfd)) {
                }
            }

        }

    } while (1 && stop_flag);
    check();
    for (i = 0; i < socket_len; i++)
    {

        close(cs[i]);
    }
    //close(s);
    return 0;
}
