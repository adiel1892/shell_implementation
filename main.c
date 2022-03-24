
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#define PATH_MAX 1000
#define MAX_LENGTH 1000


int main(){
    int flag = 0;
    int std_out = dup(1);
    int tcp_sock = socket(AF_INET,SOCK_STREAM,0);
    // getcwd - from stackoverflow
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }else{
        perror("getcwd() error");
    }
    printf("yes master?\n");
    // running program until user insert EXIT
    while(1){
        // declaring a string for the user input.
        char input[MAX_LENGTH];
        
        fgets(input , sizeof(input) , stdin);
        // declaring a string to check if user insert "ECHO" or "EXIT".
        char user_echo_exit[5];
        strncpy(user_echo_exit, input, 4);
        //declaring a string to check if user insert "DIR".
        char user_dir[4];
        strncpy(user_dir, input, 3);
        // declaring a string for the rest of the sentence after "echo".
        char rest_of_sentence[MAX_LENGTH];
         //declaring a string to check if user insert "TCP PORT".
        char user_tcp_port[9];
        strncpy(user_tcp_port, input, 8);
        //declaring a string to check if user insert "LOCAL".
        char user_local[6];
        strncpy(user_local, input, 5);
        //declaring a string to check if user insert "CD".
        char user_cd[3];
        strncpy(user_cd , input, 2);
        //declaring a string to check if user insert "COPY SRC DST".
        char user_copy_src_dst[13];
        strncpy(user_copy_src_dst , input, 12);
        //declaring a string to check if user insert "DELETE FILENAME".
        char user_delete_filename[16];
        strncpy(user_delete_filename , input, 15);
        if(flag == 1){
            dup2(std_out,1);
        }
        // exit from the program.
        if(strcmp(user_echo_exit , "EXIT\0") == 0 && strlen(input) == 5){
            printf("GOODBYE\n");
            break;
        // checking if user insert "LOCAL".
        }else if(strcmp(user_local , "LOCAL") == 0){
            printf("LOCAL");
            flag = 1;
        // checking if user insert "DIR".
        }else if(strcmp(user_dir , "DIR") == 0){
             // prompt the files of the current directory (from stackoverflow).
            DIR *d;
            struct dirent *dir;
            d = opendir(cwd);
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    printf("%s\n", dir->d_name);
                }
                closedir(d);
            }    
        // checking if user insert "TCP PORT".
        }else if(strcmp(user_tcp_port , "TCP PORT") == 0){
            // sending to server
            dup2(tcp_sock, 1);
            int const port = 5566;
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(5566);
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            connect(tcp_sock,(struct sockaddr*)&addr,sizeof(addr));   
        // checking if user insert "CD".
        }else if(strcmp(user_cd , "CD") == 0 && input[2] == ' '){
            int count = 0;
            for(int i = 3; i < strlen(input); i++){
                rest_of_sentence[i - 3] = *(input + i);
                count++;
            }
            char rest_cd[count];
            strncpy(rest_cd , rest_of_sentence , count - 1);
            if(chdir(rest_cd) < 0){
                printf("No such directory.\n");
            }else{
                // about the question - it's a system call which is used to change the current working directory of the calling process.
                // print the current path
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("Current working dir: %s\n", cwd);
                }else{
                    perror("getcwd() error");
                }
            }
        }
            // printing the sentence after "echo".
        else if(strcmp(user_echo_exit , "ECHO\0") == 0 && input[4] == ' '){
            for(int i = 5; i < strlen(input); i++){
                rest_of_sentence[i - 5] = *(input + i);
                printf("%c" , *(input + i));
            }
        }else if(strcmp(user_copy_src_dst , "COPY SRC DST") == 0){

        }else if(strcmp(user_delete_filename , "DELETE FILENAME") == 0){

        }else{
            system(input);
            pid_t child_pid;
            int child_status;
            char *exec_input[2];
            char first_word[strlen(input)];
            int i = 0;
            while(input[i] != ' ' && input[i] != '\n'){
                first_word[i] = input[i];
                i++;
            }
            exec_input[0] = first_word;
            exec_input[1] = NULL;
            child_pid = fork();
            if(child_pid == 0){
                execvp(exec_input[0], exec_input);
                printf("Unknown command\n");
                exit(0);
            }else{
                wait(&child_status);
            }
        }        
    }
}