#include <stdio.h>
#include <stdlib.h>

// Define color codes
#define RESET       "\033[0m"
#define BLACK       "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"
#define BOLD_BLACK  "\033[1;30m"
#define BOLD_RED    "\033[1;31m"
#define BOLD_GREEN  "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE   "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN   "\033[1;36m"
#define BOLD_WHITE  "\033[1;37m"

// Function to print colored text
void print_colored(const char *color, const char *text) {
    printf("%s%s%s\n", color, text, RESET);
}

void Find(char* msg, char* seek, int s1, int s2, char signal){
    /* num detected */
    int num_detected = 0;
    int i_captured[100]; /* capture index of cases detected */
    int msg_display_index = 0;
    int msg_part_s[100]; /* size of each meg separate */

    int i = 0;
    int j = 0;
    /* Detect how many cases and get index of these cases */
    while(1){
        if (msg[i] == seek[j]){
            j++;
        }

        else {
            j = 0; // index seek (to begin a new comparison)
        }

        /* check full seek, sub sizeof '\0' */
        if (j == s2 - 1){
            i_captured[num_detected] = i - (s2 - 2);
            num_detected++;
            //printf("detected at index: %d\n", i - (s2 - 2));
        }
        i_captured[num_detected] = s1;

        /* check if scan all msg */
        if (i == s1 - 1){
            break;
        }

        i++;
    }

    char* msg_display = (char*)malloc(s1 + num_detected*2 + 5); // size of origin msg + size of all signals + size of some paddings

    /* Add the firsst msg to msg_display */
    for (int i = 0; i < i_captured[0]; i++){
        msg_display[i] = msg[i];
    }
    msg_display_index = i_captured[0];

    /* Create a msg_ptr handling all strings separated */
    char** msg_ptr = (char**)malloc(sizeof(char**)*(num_detected + 1));

    /* Separate each string with '|' */
    for (int i = 0; i < num_detected; i++){
        int s_part = i_captured[i+1] - i_captured[i] + 3;
        msg_part_s[i] = s_part;
        char* msg_part = (char*)malloc(s_part); /* 3 = 2*'|' + '\0' */
        msg_ptr[i] = msg_part;

        /* Add the first msg to msg_display */
        msg_part[0] = signal;

        /* Add msg found */
        int j = 1;
        for (int k = i_captured[i]; k < i_captured[i]+s2-1; k++){
            msg_part[j] = msg[k];
            j++;
        }

        /* Add second signal */
        msg_part[j] = signal;
        j++;

        /* Add the rest string of each msg */
        for (int k = i_captured[i]+s2-1; k < i_captured[i+1]; k++){
            msg_part[j] = msg[k];
            j++;
        }

        /* Add null terminator */
        msg_part[s_part - 1] = '\0';

        //printf("msg = %s\n", msg_part);
    }

    /* Combine all strings above */
    for (int i = 0; i < num_detected; i++){
        for (int j = 0; j < msg_part_s[i]-1; j++){
            msg_display[msg_display_index] = msg_ptr[i][j];
            msg_display_index++;
        }
    }

    /* temp buffer */
    char temp_buffer[100];

    sprintf(temp_buffer, " ========== %d '%s' is found ========================\n", num_detected, seek);
    print_colored(GREEN, temp_buffer);
    
    print_colored(YELLOW, "Origin msg: ");
    printf("'%s'\n\n", msg);

    print_colored(YELLOW, "Msg found: ");
    printf("'%s'\n", msg_display);


    /* Free resources */
    for (int i = 0; i < num_detected; i++){
        free(msg_ptr[i]);
    }
    free(msg_ptr);

    free(msg_display);
}

void Replace(char* msg, char* seek, int s1, int s2, char signal, char* replace, int s3){
    /* detect how many cases detected */
    int num_detected = 0; 
    int i_captured[100]; /* capture index of cases detected */

    int msg_display_index = 0;
    int msg_display_complete_index = 0;

    int msg_part_s[100]; /* size of each meg separate */
    int msg_complete_part_s[100];

    int first_signal, last_signal;

    int i = 0;
    int j = 0;
    /* Detect how many cases and get index of these cases */
    while(1){
        if (msg[i] == seek[j]){
            j++;
        }

        else {
            j = 0; // index seek (to begin a new comparison)
        }

        /* check full seek, sub sizeof '\0' */
        if (j == s2 - 1){
            i_captured[num_detected] = i - (s2 - 2);
            num_detected++;
        }
        i_captured[num_detected] = s1;

        /* check if scan all msg */
        if (i == s1 - 1){
            break;
        }

        i++;
    }

    char* msg_display = (char*)malloc(s1 + num_detected*2 + 5); // size of origin msg + size of all signals + size of some paddings
    char* msg_display_complete = (char*)malloc(s1 + 5); // size of origin msg + size of some paddings

    /* Add the first msg to msg_display */
    for (int i = 0; i < i_captured[0]; i++){
        msg_display[i] = msg[i];
        msg_display_complete[i] = msg[i];
    }
    /* track index for the next of different msg below */
    msg_display_index = i_captured[0];
    msg_display_complete_index = i_captured[0];

    /* Create a msg_ptr handling all strings separated */
    char** msg_ptr = (char**)malloc(sizeof(char**)*(num_detected + 1));
    char** msg_complete_ptr = (char**)malloc(sizeof(char**)*(num_detected + 1));


    /* Separate each string with signal */
    for (int i = 0; i < num_detected; i++){
        int s_part = i_captured[i+1] - i_captured[i] + 3 - s2 + s3; /* remove size of string found and replace string replaced */
        msg_part_s[i] = s_part;
        char* msg_part = (char*)malloc(s_part); /* 3 = 2*'|' + '\0' */
        char* msg_complete_part = (char*)malloc(s_part - 2); /* not need signals */
        msg_complete_part_s[i] = s_part - 2;

        msg_ptr[i] = msg_part;
        msg_complete_ptr[i] = msg_complete_part;

        /* Add first signal */
        msg_part[0] = signal;
        first_signal = 0;

        /* Add msg found and msg replaced */
        int j = 1;
        int i_replace = 0;
        for (int k = i_captured[i]; k < i_captured[i]+s3-1; k++){
            msg_part[j] = replace[i_replace];
            msg_complete_part[j-1] = msg_part[j];
            i_replace++;
            j++;
        }

        /* Add second signal */
        msg_part[j] = signal;
        last_signal = j;
        j++;

        /* Add the rest string of each msg */
        for (int k = i_captured[i]+s2-1; k < i_captured[i+1]; k++){
            msg_part[j] = msg[k];
            msg_complete_part[j-2] = msg_part[j];
            j++;
        }

        /* Add null terminator */
        msg_part[s_part - 1] = '\0';
        msg_complete_part[s_part - 3] = '\0';

        //printf("msg = %s\n", msg_complete_part);
    }

    /* Combine all strings above */
    for (int i = 0; i < num_detected; i++){
        for (int j = 0; j < msg_part_s[i]-1; j++){
            msg_display[msg_display_index] = msg_ptr[i][j];
            msg_display_index++;
        }
    }

    /* Combine all strings completed above */
    for (int i = 0; i < num_detected; i++){
        for (int j = 0; j < msg_complete_part_s[i]-1; j++){ 
            msg_display_complete[msg_display_complete_index] = msg_complete_ptr[i][j];
            msg_display_complete_index++;
        }
    }

    /* temp buffer */
    char temp_buffer[100];

    sprintf(temp_buffer, " ========== %d '%s' is replaced with '%s' =========\n", num_detected, seek, replace);
    print_colored(GREEN, temp_buffer);

    print_colored(YELLOW, "Replaced msg: ");
    printf("'%s'\n\n", msg_display);

    print_colored(YELLOW, "Replaced complete msg: ");
    printf("'%s'\n", msg_display_complete);


    /* Free resources */
    for (int i = 0; i < num_detected; i++){
        free(msg_ptr[i]);
    }
    free(msg_ptr);

    for (int i = 0; i < num_detected; i++){
        free(msg_complete_ptr[i]);
    }
    free(msg_complete_ptr);
    free(msg_display);
    free(msg_display_complete);
}
