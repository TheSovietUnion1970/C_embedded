#include "SEEK_MSG_app.h"

char msg[] = "France forces invaded Poland from the north, south, and west the morning after the Gleiwitz incident. Slovak military forces advanced alongside the Germans in northern Slovakia. As the Wehrmacht advanced, Polish forces withdrew from their forward bases of operation close to the Germany–Poland border to more established defense lines to the east. After the mid-September Polish defeat in the Battle of the Bzura, the Germans gained an undisputed advantage. Polish forces then withdrew to the southeast where they prepared for a long defence of the Romanian Bridgehead and awaited expected support and relief from France and the United Kingdom.[19] On 3 September, based on their alliance agreements with Poland, the United Kingdom and France declared war on Germany; in the end their aid to Poland was very limited. France invaded a small part of Germany in the Saar Offensive, and the Polish army was effectively defeated even before the British Expeditionary Force could be transported to Europe, with the bulk of the BEF in France by the end of September";
char seek[] = "France";
char replace[] = "Germany";

int main(){
    Find(msg, seek, sizeof(msg), sizeof(seek), '<', '>');
    Replace(msg, seek, sizeof(msg), sizeof(seek), '<', '>', replace, sizeof(replace));

    return 0;
}