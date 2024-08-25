#include <stdio.h>

char msg[] = "German forces invaded Poland from the north, south, and west the morning after the Gleiwitz incident. Slovak military forces advanced alongside the Germans in northern Slovakia. As the Wehrmacht advanced, Polish forces withdrew from their forward bases of operation close to the Germany–Poland border to more established defense lines to the east. After the mid-September Polish defeat in the Battle of the Bzura, the Germans gained an undisputed advantage. Polish forces then withdrew to the southeast where they prepared for a long defence of the Romanian Bridgehead and awaited expected support and relief from France and the United Kingdom.[19] On 3 September, based on their alliance agreements with Poland, the United Kingdom and France declared war on Germany; in the end their aid to Poland was very limited. France invaded a small part of Germany in the Saar Offensive, and the Polish army was effectively defeated even before the British Expeditionary Force could be transported to Europe, with the bulk of the BEF in France by the end of September";
char seek[] = "an";

void FFFF(char* msg, char* seek, int s1, int s2){
    /* num detected */
    int num_detected = 0;
    int check_remain = 0; /* For the total passed chars */

    int i = 0;
    int j = 0;
    /* Get size */
    while(1){
        if (msg[i] == seek[j]){
            j++;
        }

        else {
            j = 0; // index seek (to begin a new comparison)
        }

        /* check full seek, sub sizeof '\0' */
        if (j == s2 - 1){
            num_detected++;
        }

        /* check if scan all msg */
        if (i == s1 - 1){
            break;
        }

        i++;
    }

    printf("num_detected = %d\n", num_detected);
}
int main(){
    FFFF(msg, seek, sizeof(msg), sizeof(seek));

    return 0;
}