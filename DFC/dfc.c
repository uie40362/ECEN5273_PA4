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

#define BUFSIZE 8192

//structs
struct split_file_lengths{
    int p1_length;
    int p2_length;
    int p3_length;
    int p4_length;
};

struct dfs {
    int first_part;
    int second_part;
    char filename1[30];
    char filename2[30];
    int first_part_length;
    int second_part_length;
};

//function prototypes
int connect_via_ip(char * ip, int port);
void md5sum(FILE * inFile, char * md5string);
void split_file_into4(FILE * split_fp, struct split_file_lengths * splitFileDetails);
int determine_filesize(FILE * fp);
void set_dfs_struct(int key, char * filename, struct split_file_lengths * fileLengths, struct dfs * dfs1, struct dfs * dfs2, struct dfs * dfs3, struct dfs * dfs4);
int sendto_dfsX(int sockfd, struct dfs * dfsx);
void parse_user_and_pw(char * username, char * pw);
void parse_dfs_ip_port(char * ip, char * port1, char * port2, char * port3, char * port4);

int main(int argc, char **argv){
    char dfs1_port[10];
    char dfs2_port2[10];
    char dfs3_port3[10];
    char dfs4_port4[10];
    char dfs_ip[32]


    while (1) {
        char command[50];
        char *instr;
        char *filename;
        struct dfs dfs1, dfs2, dfs3, dfs4;
        struct split_file_lengths fileLengths;

        printf("List of commands:\nput [filename]\nget [filename]\nls\n\nInput command:");
        fgets(command, sizeof(command), stdin); //get input from user
        command[strcspn(command, "\n")] = 0;    //remove trailing newline

        instr = strtok(command, " ");   //determine command based on first word

        //send authentication details to server

    }

//    struct split_file_lengths fileLengths;
//    FILE * fp = fopen("mytext.txt", "r");
//    split_file_into4(fp, &fileLengths);

}

/*function to connect to dfs server given an ip and port*/
int connect_via_ip(char * ip, int port){
    if (!port)
        port = 80;

    /*convert ip to network byte order*/
    struct in_addr ipaddr;
    struct hostent * server;

    int ip_valid = inet_aton(ip, &ipaddr);
    if (!ip_valid){
        printf("IP not valid");
        exit(0);
    }

    server = gethostbyaddr((const void *)&ipaddr, sizeof(ipaddr), AF_INET);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", ip);
        exit(0);
    }

    struct sockaddr_in serveraddr;

    /* socket: create the socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
        return -1;
    }


    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port);

    //connect to host server
    int connected = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (connected<0)
        return -1;
    else
        return sockfd;
}

/*function to convert file into md5 hash
 * md5string argument is array with size 33*/
void md5sum(FILE * inFile, char * md5string){
    MD5_CTX c;
    unsigned long bytes;
    char data[1024];
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Init(&c);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&c, data, bytes);
    MD5_Final(out, &c);
//    char md5string[33];
    for(int i = 0; i < 16; ++i)
        sprintf(&md5string[i*2], "%02x", (unsigned int)out[i]);
}

/*function to split file into 4 equal parts and stores in DFC dir to be sent to DFS*/
void split_file_into4(FILE * split_fp, struct split_file_lengths * splitFileDetails){
    int size, n;
    int bytes_read = 0;
    char buf[BUFSIZE];

    size = determine_filesize(split_fp);
    int size_over4 = size / 4;

    FILE * fp1 = fopen("P1", "w");
    FILE * fp2 = fopen("P2", "w");
    FILE * fp3 = fopen("P3", "w");
    FILE * fp4 = fopen("P4", "w");

    while (bytes_read<size_over4){
       n = fread(buf, sizeof(char), size_over4, split_fp);
       fwrite(buf, sizeof(char), n, fp1);
       bytes_read += n;
    }
    splitFileDetails->p1_length = bytes_read;
    size -= bytes_read;
    bytes_read=0;

    while (bytes_read<size_over4){
        n = fread(buf, sizeof(char), size_over4, split_fp);
        fwrite(buf, sizeof(char), n, fp2);
        bytes_read += n;
    }
    splitFileDetails->p2_length = bytes_read;
    size -= bytes_read;
    bytes_read=0;

    while (bytes_read<size_over4){
        n = fread(buf, sizeof(char), size_over4, split_fp);
        fwrite(buf, sizeof(char), n, fp3);
        bytes_read += n;
    }
    splitFileDetails->p3_length = bytes_read;
    size -= bytes_read;
    bytes_read=0;

    while (bytes_read<size){
        n = fread(buf, sizeof(char), size_over4, split_fp);
        fwrite(buf, sizeof(char), n, fp4);
        bytes_read += n;
    }
    splitFileDetails->p4_length = bytes_read;

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}

int determine_filesize(FILE * fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);   //get size of file
    fseek(fp, 0, SEEK_SET);
    return size;
}

/*function to set dfs struct which will be used to send file parts*/
void set_dfs_struct(int key, char * filename, struct split_file_lengths * fileLengths, struct dfs * dfs1, struct dfs * dfs2, struct dfs * dfs3, struct dfs * dfs4){
    switch(key){
        case 0:
            dfs1->first_part = 1;
            dfs1->second_part = 2;
            sprintf(dfs1->filename1, "%s.%d", filename, dfs1->first_part);
            sprintf(dfs1->filename2, "%s.%d", filename, dfs1->second_part);
            dfs1->first_part_length = fileLengths->p1_length;
            dfs1->second_part_length = fileLengths->p2_length;
            dfs2->first_part = 2;
            dfs2->second_part = 3;
            sprintf(dfs2->filename1, "%s.%d", filename, dfs2->first_part);
            sprintf(dfs2->filename2, "%s.%d", filename, dfs2->second_part);
            dfs2->first_part_length = fileLengths->p2_length;
            dfs2->second_part_length = fileLengths->p3_length;
            dfs3->first_part = 3;
            dfs3->second_part = 4;
            sprintf(dfs3->filename1, "%s.%d", filename, dfs3->first_part);
            sprintf(dfs3->filename2, "%s.%d", filename, dfs3->second_part);
            dfs3->first_part_length = fileLengths->p3_length;
            dfs3->second_part_length = fileLengths->p4_length;
            dfs4->first_part = 4;
            dfs4->second_part = 1;
            sprintf(dfs4->filename1, "%s.%d", filename, dfs4->first_part);
            sprintf(dfs4->filename2, "%s.%d", filename, dfs4->second_part);
            dfs4->first_part_length = fileLengths->p4_length;
            dfs4->second_part_length = fileLengths->p1_length;
            break;
        case 1:
            dfs1->first_part = 4;
            dfs1->second_part = 1;
            sprintf(dfs1->filename1, "%s.%d", filename, dfs1->first_part);
            sprintf(dfs1->filename2, "%s.%d", filename, dfs1->second_part);
            dfs1->first_part_length = fileLengths->p4_length;
            dfs1->second_part_length = fileLengths->p1_length;
            dfs2->first_part = 1;
            dfs2->second_part = 2;
            sprintf(dfs2->filename1, "%s.%d", filename, dfs2->first_part);
            sprintf(dfs2->filename2, "%s.%d", filename, dfs2->second_part);
            dfs2->first_part_length = fileLengths->p1_length;
            dfs2->second_part_length = fileLengths->p2_length;
            dfs3->first_part = 2;
            dfs3->second_part = 3;
            sprintf(dfs3->filename1, "%s.%d", filename, dfs3->first_part);
            sprintf(dfs3->filename2, "%s.%d", filename, dfs3->second_part);
            dfs3->first_part_length = fileLengths->p2_length;
            dfs3->second_part_length = fileLengths->p3_length;
            dfs4->first_part = 3;
            dfs4->second_part = 4;
            sprintf(dfs4->filename1, "%s.%d", filename, dfs4->first_part);
            sprintf(dfs4->filename2, "%s.%d", filename, dfs4->second_part);
            dfs4->first_part_length = fileLengths->p3_length;
            dfs4->second_part_length = fileLengths->p4_length;
            break;
        case 2:
            dfs1->first_part = 3;
            dfs1->second_part = 4;
            sprintf(dfs1->filename1, "%s.%d", filename, dfs1->first_part);
            sprintf(dfs1->filename2, "%s.%d", filename, dfs1->second_part);
            dfs1->first_part_length = fileLengths->p3_length;
            dfs1->second_part_length = fileLengths->p4_length;
            dfs2->first_part = 4;
            dfs2->second_part = 1;
            sprintf(dfs2->filename1, "%s.%d", filename, dfs2->first_part);
            sprintf(dfs2->filename2, "%s.%d", filename, dfs2->second_part);
            dfs2->first_part_length = fileLengths->p4_length;
            dfs2->second_part_length = fileLengths->p1_length;
            dfs3->first_part = 1;
            dfs3->second_part = 2;
            sprintf(dfs3->filename1, "%s.%d", filename, dfs3->first_part);
            sprintf(dfs3->filename2, "%s.%d", filename, dfs3->second_part);
            dfs3->first_part_length = fileLengths->p1_length;
            dfs3->second_part_length = fileLengths->p2_length;
            dfs4->first_part = 2;
            dfs4->second_part = 3;
            sprintf(dfs4->filename1, "%s.%d", filename, dfs4->first_part);
            sprintf(dfs4->filename2, "%s.%d", filename, dfs4->second_part);
            dfs4->first_part_length = fileLengths->p2_length;
            dfs4->second_part_length = fileLengths->p3_length;
            break;
        case 3:
            dfs1->first_part = 2;
            dfs1->second_part = 3;
            sprintf(dfs1->filename1, "%s.%d", filename, dfs1->first_part);
            sprintf(dfs1->filename2, "%s.%d", filename, dfs1->second_part);
            dfs1->first_part_length = fileLengths->p2_length;
            dfs1->second_part_length = fileLengths->p3_length;
            dfs2->first_part = 3;
            dfs2->second_part = 4;
            sprintf(dfs2->filename1, "%s.%d", filename, dfs2->first_part);
            sprintf(dfs2->filename2, "%s.%d", filename, dfs2->second_part);
            dfs2->first_part_length = fileLengths->p3_length;
            dfs2->second_part_length = fileLengths->p4_length;
            dfs3->first_part = 4;
            dfs3->second_part = 1;
            sprintf(dfs3->filename1, "%s.%d", filename, dfs3->first_part);
            sprintf(dfs3->filename2, "%s.%d", filename, dfs3->second_part);
            dfs3->first_part_length = fileLengths->p4_length;
            dfs3->second_part_length = fileLengths->p1_length;
            dfs4->first_part = 1;
            dfs4->second_part = 2;
            sprintf(dfs4->filename1, "%s.%d", filename, dfs4->first_part);
            sprintf(dfs4->filename2, "%s.%d", filename, dfs4->second_part);
            dfs4->first_part_length = fileLengths->p1_length;
            dfs4->second_part_length = fileLengths->p2_length;
            break;
    }
}

/*function send file pair to dfs*/
int sendto_dfsX(int sockfd, struct dfs * dfsx){
    char buf[BUFSIZE];

    //send filename for first pair
    strcpy(buf, dfsx->filename1);
    send(sockfd, buf, BUFSIZE, 0);

    //send filesize for first pair
    bzero(buf, BUFSIZE);
    sprintf(buf, "%d", dfsx->first_part_length);
    send(sockfd, buf, BUFSIZE, 0);

    //send first file part
    char part_name[3];
    int part_size, n;
    sprintf(part_name, "P%d", dfsx->first_part);
    FILE * fp1 = fopen(part_name, "r");
    part_size = dfsx->first_part_length;

    while (part_size){
        bzero(buf, BUFSIZE);
        n = fread(buf, sizeof(char), BUFSIZE, fp1);
        send(sockfd, buf, n, 0);
        part_size -= n;
    }
    fclose(fp1);

    //send filename for second pair
    strcpy(buf, dfsx->filename2);
    send(sockfd, buf, BUFSIZE, 0);

    //send filesize for second pair
    bzero(buf, BUFSIZE);
    sprintf(buf, "%d", dfsx->second_part_length);
    send(sockfd, buf, BUFSIZE, 0);

    //send second file part
    sprintf(part_name, "P%d", dfsx->second_part);
    FILE * fp2 = fopen(part_name, "r");
    part_size = dfsx->second_part_length;

    while (part_size){
        bzero(buf, BUFSIZE);
        n = fread(buf, sizeof(char), BUFSIZE, fp2);
        send(sockfd, buf, n, 0);
        part_size -= n;
    }
    fclose(fp2);
    return 1;
}

/*function to parse username and pw - arg as char arr[]*/
void parse_user_and_pw(char * username, char * pw){
    char buf[1024];
    char * line;
    char * user;
    char * pass;

    FILE * fp = fopen("dfc.conf", "r");
    fread(buf, sizeof(char), sizeof(buf), fp);
    line = strtok(buf, "\n");

    while(line){
        if (strstr(line, "Username:")){
            user = strchr(line, ':');
            user += 1;
            sprintf(username, "%s", user);
        }

        if(strstr(line, "Password:")){
            pass = strchr(line, ':');
            pass += 1;
            sprintf(pw, "%s", pass);
        }
        line = strtok(NULL, "\n");
    }
    fclose(fp);
}

void parse_dfs_ip_port(char * ip, char * port1, char * port2, char * port3, char * port4){
    char buf[1024];
    char * line;
    char * temp_ip[32];
    char * temp;

    FILE * fp = fopen("dfc.conf", "r");
    fread(buf, sizeof(char), sizeof(buf), fp);
    line = strtok(buf, "\n");

    //determine ip
    temp = strstr(line, "DFS1");
    temp += 5;
    strcpy(temp_ip, temp);
    temp_ip[strcspn(temp_ip, ':')] = 0;
    strcpy(ip, temp_ip);

     //determine port1
    temp = strchr(line, ':');
    temp += 1;
    strcpy(port1, temp);

    //determine remaining ports
    for (int i = 0; i<3; i++){
        line = strtok(NULL, "\n");
        temp = strchr(line, ':');
        temp += 1;

        switch (i) {
            case 0:
                strcpy(port2, temp);
                break;
            case 1:
                strcpy(port3, temp);
                break;
            case 2:
                strcpy(port4, temp);
                break;
        }

    }

    fclose(fp);
}