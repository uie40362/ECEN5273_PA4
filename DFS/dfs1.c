#include <stdio.h>
#include <stdlib.h>
#include <string.h>      /* for fgets */
#include <strings.h>     /* for bzero, bcopy */
#include <unistd.h>      /* for read, write */
#include <sys/socket.h>  /* for socket use */
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <openssl/md5.h>

#define LISTENQ 1024
#define BUFSIZE 8192

//globals
static volatile int keep_running = 1;

//function prototypes
int open_listenfd(int port);
void intHandler();
void *thread(void *vargp);
void service_client_session(int connfd);
int check_user_and_pw(char * username, char * pw);

int main(int argc, char ** argv){
    int listenfd, *connfdp, port, clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    listenfd = open_listenfd(port);
    while (1) {
        /*register signal handler*/
        signal(SIGINT, intHandler);

        connfdp = malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
        if (keep_running==0){
            exit(0);
        }
    }
}


int open_listenfd(int port)
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
        return -1;

    /* listenfd will be an endpoint for all requests to port
       on any IP address for this host */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
        return -1;
    return listenfd;
} /* end open_listenfd */

/*signal handler (ctrl+c)*/
void intHandler() {
    keep_running = 0;
    printf("\nWEB SERVER SHUTDOWN\n");
    exit(0);
}

void * thread(void * vargp)
{
    int connfd = *((int *)vargp);
    pthread_detach(pthread_self());
    free(vargp);
    service_client_session(connfd);
    close(connfd);
    return NULL;
}

/*function to service a client session in a thread*/
void service_client_session(int connfd){
    while (1){
        char instr[12];
        char filename[40];
        char buf[BUFSIZE];
        char username[30];
        char password[30];

        //receive username and pw from client
        recv(connfd, buf, sizeof(username), 0);
        strcpy(username, buf);
        bzero(buf, BUFSIZE);
        recv(connfd, buf, sizeof(password), 0);
        strcpy(password, buf);
        bzero(buf, BUFSIZE);


        //check if username and password correct
        int user_verified = check_user_and_pw(username, password);
        if (!user_verified){
            sprintf(buf, "NOK");
            send(connfd, buf, strlen(buf), 0);
            continue;
        }

        //send OK message if verified
        bzero(buf, BUFSIZE);
        sprintf(buf, "OK");
        send(connfd, buf, BUFSIZE, 0);

        //receive command from client
        recv(connfd, buf, BUFSIZE, 0);
        strncpy(instr, buf, strlen(buf));
        bzero(buf, BUFSIZE);

        /*put command*/
        if (strcmp(instr, "put")==0){
            //receive filename for first pair
            recv(connfd, buf, BUFSIZE, 0);
            strcpy(filename, buf);

            //receive filesize of first pair
            bzero(buf, BUFSIZE);
            recv(connfd, buf, BUFSIZE, 0);
            int filesize = atoi(buf);

            //receive first file part
            int n;
            char path[] = "DFS1/";
            strcat(path, filename);
            FILE * fp = fopen(path, "w");
            while (filesize>0){
                bzero(buf, BUFSIZE);
                n = recv(connfd, buf, BUFSIZE, 0);
                fwrite(buf, sizeof(char), n, fp);
                filesize -= n;
            }
            fclose(fp);

            //receive filename for second pair
            recv(connfd, buf, BUFSIZE, 0);
            strcpy(filename, buf);

            //receive filesize of second pair
            bzero(buf, BUFSIZE);
            recv(connfd, buf, BUFSIZE, 0);
            filesize = atoi(buf);

            //receive second file part
            char path2[] = "DFS1/";
            strcat(path2, filename);
            fp = fopen(path2, "w");
            while (filesize>0){
                bzero(buf, BUFSIZE);
                n = recv(connfd, buf, BUFSIZE, 0);
                fwrite(buf, sizeof(char), n, fp);
                filesize -= n;
            }
            fclose(fp);

            printf("SERVER EXECUTED THE COMMAND: %s %s\n", instr, filename);
        }/*end put command*/
    }
}

int check_user_and_pw(char * username, char * pw){
    char buf[1024];
    char * line;
    char * user;
    char * pass;

    FILE * fp = fopen("dfs.conf", "r");
    fread(buf, sizeof(char), sizeof(buf), fp);
    line = strtok(buf, "\r\n");

    while(line){
        if (strstr(line, "Username:")){
            user = strchr(line, ':');
            user += 2;
        }

        else if(strstr(line, "Password:")){
            pass = strchr(line, ':');
            pass += 2;
        }

        if (pass && user) {
            if (strcmp(user, username) == 0 && strcmp(pass, pw) == 0) {
                return 1;
            }
        }
        line = strtok(NULL, "\r\n");
    }
    fclose(fp);
    return 0;
}