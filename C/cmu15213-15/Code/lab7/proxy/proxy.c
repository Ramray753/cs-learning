#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *requestline_format = "%s http://%s:%s%s HTTP/1.0\r\n";
static const char *user_agent_header = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_header = "Connection: close\r\n";
static const char *proxy_connection_header = "Proxy-Connection: close\r\n";
static const char *blank_line = "\r\n";
static char *dest_host = "10.20.5.5";
static char *dest_port = "10080";


void *doProxy(void *vargp);
void parse_uri(char* uri, char* path);
void build_request_header(char* request_header, char* path, char* method, rio_t* client_rio);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc,char **argv)
{
    static char hostname[MAXLINE], port[MAXLINE];
    int listenfd, *connfd;
    pthread_t tid;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("\nAccepted connection from (%s, %s)\n", hostname, port);
        Pthread_create(&tid, NULL, doProxy, connfd);                                                                                
    }
}

void *doProxy(void *vargp){
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char path[MAXLINE], request_header[MAXLINE];
    int serverfd, fd;
    rio_t rio, server_rio;

    /* Transform argv from main thread */
    fd = *(int *)vargp;
    free(vargp);

    /* kill the thread when finished */
    Pthread_detach(pthread_self());

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  
        return NULL;
    printf("In:    %s", buf);
    sscanf(buf, "%s %s", method, uri);       
    if (strcasecmp(method, "GET") && strcasecmp(method, "POST")) {                     
        clienterror(fd, method, "501", "Not Implemented", "The proxy does not implement this method");
        return NULL;
    }                                                    

    /* Parse URI from GET request */
    parse_uri(uri, path);

    /* Build the request header*/
    build_request_header(request_header, path, method, &rio);

    /*Connect to server*/
    serverfd = Open_clientfd(dest_host, dest_port);
    if(serverfd < 0){
        printf("connection failed\n");
        return NULL;
    }
    /*Write the http header to server*/
    Rio_readinitb(&server_rio, serverfd);
    Rio_writen(serverfd, request_header, strlen(request_header));

    /*receive message from end server and send to the client*/
    size_t n;
    while( (n=Rio_readlineb(&server_rio,buf,MAXLINE))!=0 )
        Rio_writen(fd, buf, n);
    Close(serverfd);
    Close(fd);   
    return NULL; 
}

void parse_uri(char* uri, char* path)
{
    path[0] = '\0';
    /*position to "//", the first"/", ":" */ 
    char* ppath;

    ppath = strstr(uri, "//") + 2;
    ppath = strchr(ppath, '/');
    if(ppath){
        strcpy(path, ppath);
    }
}

void build_request_header(char* request_header, char* path, char* method, rio_t* client_rio)
{
    char buf[MAXLINE];
    int len = 0;
    sprintf(request_header, requestline_format, method, dest_host, dest_port, path);
    printf("Out:   %s", request_header);
    
    /*check host header and get other request header for client rio then change it */
    while(Rio_readlineb(client_rio, buf, MAXLINE) > 0){
        if(strcmp(buf, blank_line)==0) {
            strcat(request_header, blank_line); // End of Header
            if(!strncasecmp("POST", method, strlen("POST"))){
                // Content
                Rio_readnb(client_rio, buf, len);
                strcat(request_header, buf);
            }
            break;
        } 
        //when this line is not proxy_connection header or connection header or user-agent:
        if(!strncasecmp("Host:", buf, strlen("Host:")) ||
            !strncasecmp("Connection:", buf, strlen("Connection:")) ||
            !strncasecmp("Proxy-Connection:", buf, strlen("Proxy-Connection:")) ||
            !strncasecmp("User-Agent:", buf, strlen("User-Agent:")))
            continue;
        if (!strncasecmp("Content-Length:", buf, strlen("Content-Length:"))){
            char* lenp = strchr(buf, ':') + 2;
            len = atoi(lenp);
        }
        strcat(request_header, buf);
    }
    strcat(request_header, connection_header);
    strcat(request_header, user_agent_header);
    strcat(request_header, proxy_connection_header);
}

void clienterror(int fd, char *cause, char *errnum, 
         char *shortmsg, char *longmsg) 
{
    static char buf[MAXLINE], body[MAXBUF];
    buf[0] = '\0';
    body[0] = '\0';

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
