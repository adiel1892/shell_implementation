
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
        char user_echo_exit_copy[5];
        strncpy(user_echo_exit_copy, input, 4);
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
        //declaring a string to check if user insert "DELETE".
        char user_delete[7];
        strncpy(user_delete , input, 6);
        if(flag == 1){
            dup2(std_out,1);
        }
        // exit from the program.
        if(strcmp(user_echo_exit_copy , "EXIT\0") == 0 && strlen(input) == 5){
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
        else if(strcmp(user_echo_exit_copy , "ECHO\0") == 0 && input[4] == ' '){
            for(int i = 5; i < strlen(input); i++){
                rest_of_sentence[i - 5] = *(input + i);
                printf("%c" , *(input + i));
            }
        }else if(strcmp(user_echo_exit_copy , "COPY") == 0 && input[4] == ' '){         
            // about the question - the fopen() , fwrite() and fread() are all library function.
            // The difference between fopen and open is that fopen is a library function while open is a system call.   
            char *src_dst[3];
            int j = 0;
            char *p = strtok (input, " ");
            while (p != NULL)
            {
                src_dst[j++] = p;
                p = strtok (NULL, " ");
            }
            // printf("%s\n" , src_dst[1]);
            // printf("%s\n" , src_dst[2]);
            FILE *file, *file2;
            char line[MAX_LENGTH];
            file = fopen(src_dst[1], "r");
            file2 = fopen(src_dst[2], "w");
            if(file == NULL || file2 == NULL){
                perror("");
            }else{
                while (fgets(line,sizeof(line),file) != NULL){
                    fputs(line, file2);
                }
                fclose (file);
                fclose (file2);
                printf("check - close");
            }
        }else if(strcmp(user_delete , "DELETE") == 0 && input[6] == ' '){
            // about the question - unlink is a system call and a command line utility to delete files.
            char for_delete[strlen(input) - 6];
            for(int i = 7; i < strlen(input); i++){
                for_delete[i - 7] = *(input + i);
            }
            if(unlink(for_delete) == -1){
                perror("");
            }else{
                printf("deleating %s" , for_delete);
            }
        }else{
            // system(input);
            // count number of spaces
            int count = 0;
            for(int w = 0; w < strlen(input); w++){
                if(input[w] == ' ' || input[w] == '\n'){
                    count++;
                }
            }
            char *array[count + 1];
            memset(array , '0' , strlen(input));
            int j = 0;
            char *p = strtok (input, " ");
            
            while (p != NULL)
            {
                array[j++] = p;
                p = strtok (NULL, " ");
            }
            array[sizeof(array) / sizeof(array[0]) - 1] = NULL;
            pid_t child_pid;
            int child_status;

            // char *exec_input[2];
            // char first_word[strlen(input)];
            // int i = 0;
            // while(input[i] != ' ' && input[i] != '\n'){
            //     first_word[i] = input[i];
            //     i++;
            // }
            // exec_input[0] = first_word;
            // exec_input[1] = NULL;

            child_pid = fork();
            if(child_pid == 0){
                execvp(array[0], array);
                printf("Unknown command\n");
                exit(0);
            }else{
                wait(&child_status);
            }
        }        
    }
}