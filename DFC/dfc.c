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
struct split_file_details{
    int p1_length;
    int p2_length;
    int p3_length;
    int p4_length;
};

//function prototypes
int connect_via_ip(char * ip, int port);
void md5sum(FILE * inFile, char * md5string);
void split_file_into4(FILE * split_fp, struct split_file_details * splitFileDetails);
int determine_filesize(FILE * fp);


int main(){
    struct split_file_details file_part_lengths;
    FILE * fp = fopen("mytext.txt", "r");
    split_file_into4(fp, &file_part_lengths);
}

/*function to connect to dfs server given an ip and port*/
int connect_via_ip(char * ip, int port){
    if (!port)
        port = 80;

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
    bcopy(ip,
          (char *)&serveraddr.sin_addr.s_addr, sizeof(ip));
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
void split_file_into4(FILE * split_fp, struct split_file_details * splitFileDetails){
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