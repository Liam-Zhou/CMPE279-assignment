// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    printf("argc:%d \n",argc);
    if(argc > 1){
        int valread;
        char buffer[102] = {0};
        char *hello = "Hello from server"; 
        new_socket = atoi(argv[1]);
            valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );
            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
    } else if (argc == 1){
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // printf("execve=0x%p\n\n", execve);


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080 // | SO_REUSEPORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR ,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("waiting for client mess \n");
     if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
    pid_t pid;
    pid = fork();
    int ch;
    if(pid == 0){
        printf("child pid: %d\n", getpid());
        if(!setuid(5888)){
            printf("setuid successfully!\n");
            char new_socket2[12];
            sprintf(new_socket2, "%d", new_socket);
            char filename[20];
            strcpy(filename, argv[0]);
            char *args[] = {filename, new_socket2, NULL};
            if (execvp(filename, args) < 0) {
                perror("exec");
                exit(EXIT_FAILURE);
            };
        }else{
            printf("setuid error!");
            perror("setuid");
        }
    }else if(pid == -1){
        printf("Error while calling the fork function \n");
    }else{
        printf("father pid: %d\n", getpid());
        wait(&ch);
    }
    }
    
    return 0;
}
