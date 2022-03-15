#include "stdio.h"
#include <string.h>
#include <unistd.h>
#define PATH_MAX 1000
#define MAX_LENGTH 1000


int main(){

    // getcwd - from stackoverflow
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }else{
        perror("getcwd() error");
    }

    // running program until user insert EXIT
    while(1){
        // declaring a string for the user input.
        char input[MAX_LENGTH];
        printf("yes master?\n");
        fgets(input , sizeof(input) , stdin);
        // declaring a string to check if user insert "echo" or "EXIT".
        char check_echo_exit[4];
        // declaring a string for the rest of the sentence after "echo".
        char rest_of_sentence[MAX_LENGTH];
        // checking if user insert "echo" or "EXIT".
        if(strlen(input) > 4){
            for(int i = 0; i < 4; i++){
                check_echo_exit[i] = *(input + i);
            }
            // printing the sentence after "echo".
            if(strcmp(check_echo_exit , "echo\0") == 0 && input[4] == ' '){
                for(int i = 5; i < strlen(input); i++){
                    rest_of_sentence[i] = *(input + i);
                    printf("%c" , *(input + i));
                }
                // exit from the program.
            }else if(strcmp(check_echo_exit , "EXIT\0") == 0){
                printf("GOODBYE\n");
                break;
            }

        }
    }
    
}