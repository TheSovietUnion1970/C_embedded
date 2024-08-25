#include <stdio.h>
#include <stdlib.h>

char msg[] = "France forces invaded Poland from the north, south, and west the morning after the Gleiwitz incident. Slovak military forces advanced alongside the Germans in northern Slovakia. As the Wehrmacht advanced, Polish forces withdrew from their forward bases of operation close to the Germany–Poland border to more established defense lines to the east. After the mid-September Polish defeat in the Battle of the Bzura, the Germans gained an undisputed advantage. Polish forces then withdrew to the southeast where they prepared for a long defence of the Romanian Bridgehead and awaited expected support and relief from France and the United Kingdom.[19] On 3 September, based on their alliance agreements with Poland, the United Kingdom and France declared war on Germany; in the end their aid to Poland was very limited. France invaded a small part of Germany in the Saar Offensive, and the Polish army was effectively defeated even before the British Expeditionary Force could be transported to Europe, with the bulk of the BEF in France by the end of September";
char msg2[] = "manl, danl, aaanl, ghanlvb";
char seek[] = "anl";
char replace[] = "xxxx";

void Find(char* msg, char* seek, int s1, int s2, char signal){
    char msg_display[2000];
    /* num detected */
    int num_detected = 0;
    int i_captured[100];
    int msg_display_index = 0;
    int msg_part_s[100];

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
            printf("detected at index: %d\n", i - (s2 - 2));
        }
        i_captured[num_detected] = s1;

        /* check if scan all msg */
        if (i == s1 - 1){
            break;
        }

        i++;
    }

    printf("num_detected = %d\n", num_detected);

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

        msg_part[0] = signal;

        int j = 1;
        for (int k = i_captured[i]; k < i_captured[i]+s2-1; k++){
            msg_part[j] = msg[k];
            j++;
        }
        msg_part[j] = signal;
        j++;
        for (int k = i_captured[i]+s2-1; k < i_captured[i+1]; k++){
            msg_part[j] = msg[k];
            j++;
        }
        msg_part[s_part - 1] = '\0';

        printf("msg = %s\n", msg_part);
    }

    /* Combine all strings above */
    for (int i = 0; i < num_detected; i++){
        for (int j = 0; j < msg_part_s[i]-1; j++){
            msg_display[msg_display_index] = msg_ptr[i][j];
            msg_display_index++;
        }
    }

    printf("===========\n");
    printf("msg = %s\n", msg);
    printf("msg_display = %s\n", msg_display);



    for (int i = 0; i < num_detected; i++){
        free(msg_ptr[i]);
    }
    free(msg_ptr);
}

void Replace(char* msg, char* seek, int s1, int s2, char signal, char* replace, int s3){
    char msg_display[2000];
    /* num detected */
    int num_detected = 0;
    int i_captured[100];
    int msg_display_index = 0;
    int msg_part_s[100];

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
            // printf("detected at index: %d\n", i - (s2 - 2));
        }
        i_captured[num_detected] = s1;

        /* check if scan all msg */
        if (i == s1 - 1){
            break;
        }

        i++;
    }

    // printf("num_detected = %d\n", num_detected);

    /* Add the firsst msg to msg_display */
    for (int i = 0; i < i_captured[0]; i++){
        msg_display[i] = msg[i];
    }
    msg_display_index = i_captured[0];

    /* Create a msg_ptr handling all strings separated */
    char** msg_ptr = (char**)malloc(sizeof(char**)*(num_detected + 1));

    /* new replacing string */

    /* Separate each string with '|' */
    for (int i = 0; i < num_detected; i++){
        int s_part = i_captured[i+1] - i_captured[i] + 3 - s2 + s3; /* remove size of string found and replace string replaced */
        msg_part_s[i] = s_part;
        char* msg_part = (char*)malloc(s_part); /* 3 = 2*'|' + '\0' */
        msg_ptr[i] = msg_part;

        msg_part[0] = signal;

        int j = 1;
        int i_replace = 0;
        for (int k = i_captured[i]; k < i_captured[i]+s3-1; k++){
            msg_part[j] = replace[i_replace];
            i_replace++;
            j++;
        }

        msg_part[j] = signal;
        j++;

        for (int k = i_captured[i]+s2-1; k < i_captured[i+1]; k++){
            msg_part[j] = msg[k];
            j++;
        }

        msg_part[s_part - 1] = '\0';

        // printf("msg = %s\n", msg_part);
    }

    /* Combine all strings above */
    for (int i = 0; i < num_detected; i++){
        for (int j = 0; j < msg_part_s[i]-1; j++){
            msg_display[msg_display_index] = msg_ptr[i][j];
            msg_display_index++;
        }
    }

    printf(" ========== %d string is replaced =========\n", num_detected);
    printf("origin msg: '%s'\n", msg);
    printf("replaced msg = '%s'\n", msg_display);



    for (int i = 0; i < num_detected; i++){
        free(msg_ptr[i]);
    }
    free(msg_ptr);
}

int main(){
    Replace(msg, seek, sizeof(msg), sizeof(seek), '|', replace, sizeof(replace));

    return 0;
}