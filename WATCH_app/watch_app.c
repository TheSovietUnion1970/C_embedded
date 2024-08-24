#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdint.h>
int x=0, y = 0;
int h=0,m=0,s=0, hs, ms, ss;
uint8_t flag, state = 0;

// delay - ms
void delay_ms(time_t ms){
    int temp = clock();
    while(clock() - temp < 1000);
}
void printData(){
    system("cls");
    printf("1. Start   2. Stop   3. Reset\n");
    printf("*****************************\n");
    printf("*         %.2d:%.2d:%.2d          *\n", h, m, s);
    printf("*****************************\n");
    printf("     c. Count     s. Set     \n");
}

void printData1(){
    system("cls");
    printf("1. Start   2. Stop   3. Reset\n");
    printf("*****************************\n");
    printf("*     %.2d:%.2d:%.2d|%.2d:%.2d:%.2d     *\n", h, m, s, hs, ms, ss);
    printf("*****************************\n");
    printf("     c. Count     s. Set     \n");
}

void printData2(){
    system("cls");
    printf("1. Start   2. Stop   3. Reset\n");
    printf("*****************************\n");
    printf("*          Time out         *\n");
    printf("*****************************\n");
    printf("     c. Count     s. Set     \n");
}

// controller
void selection(){
    switch(getch()){
        case 49:
            flag = 1; // start
            break;
        case 50:
            flag = 0; // stop
            break;
        case 51: // reset
            h = 0;m = 0;s = 0;
            flag = 0; // reset
            break;
        case 115: // s
            state = 1;
            x = 0;
            y = 0;
            break;
        case 99: // c
            h = 0;m = 0;s = 0;
            flag = 0; // reset
            state = 0;
            y = 0;
            break;
    }
}

void counter(){
    s++;
    if (s == 59){
        m++;
        s = 0;
    }
    if (m == 59){
        h++;
        s = 0;
        m = 0;
    }
    printData();
    delay_ms(1000);
}

void reverse(){
    if (x != 0 && x>0){
        if (s == 0){
            if (m == 0){
                if (h == 0){
                    x = -1;
                }
                h--;
                m = 60;
            }
            m--;
            s = 60;
        }
        s--;

    }
    else if (x == 0){
        s = ss;
        m = ms;
        h = hs;
    }

    if (x == -1){
        printData2();
        //flag = 0;
    }
    else {printData1();x++;}
    delay_ms(1000);
}

void COUNT(){
    while(1){
        while(!kbhit() && flag == 1){
            counter();
        }
        printData();
        selection();
        if (state == 1) break;
    }
}

void COUNT_REV(){
    while(1){
        if (x == -1) {h = 0; m = 0; s = 0;x = 0;}
        while(!kbhit() && flag == 1){
            reverse();
        }
        printData1();
        selection();
        if (state == 0 || y == 0) break;
    }
}


int main(){
    while(1){
        // up counter
        if (state == 0){
            COUNT();
            }

        // set time counter
        else if (state == 1) {
            printf("Enter time set `hh:mm:ss`: ");
            y = scanf("%d:%d:%d", &hs, &ms, &ss);
            state = -1;
        }
        if (y == 3) {COUNT_REV();}


    }
}