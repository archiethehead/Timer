#include <stdlib.h>
#include <windows.h>
#include <signal.h>
#include <conio.h>

#define SECONDS 1000
#define MINUTES 60000
#define HOURS 3600000
#define STRCMP(x, y) !(strcmp(x, y))

HANDLE consoleHandle;
CONSOLE_CURSOR_INFO info;

void handleSIGINT(int signalNum) {
    
    puts("");    
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);  
    exit(0);

}

void timer(int modifier, int milliseconds) {
    
    while (milliseconds > 0) {
        
        if ((modifier == SECONDS) | (milliseconds < MINUTES)) { 
            
            double secondsLeft = milliseconds / SECONDS;        
            printf("\r   %d seconds remaining . . .", (int)secondsLeft);
            printf("                                       ");            
            
        }

        else if ((modifier == MINUTES) | (milliseconds < HOURS)){
           
            int minutesLeft = milliseconds / MINUTES;
            int secondsLeft = (milliseconds / SECONDS) % 60;
            printf("\r    %d minutes and ", minutesLeft);
            printf("%d seconds remaining . . .", secondsLeft);
            printf("                                       ");            

        }
            
        else {
            
            int hoursLeft = milliseconds / HOURS;
            int minutesLeft = (milliseconds / MINUTES) % 60;
            int secondsLeft = (milliseconds / SECONDS) % 60;
            printf("\r    %d hours, ", hoursLeft);
            printf("%d minutes, and ", minutesLeft);
            printf("%d seconds remaining . . .", secondsLeft);           
            printf("                                       ");            
                        
    
        }
        
        if (_kbhit()) {

            char inputChar = _getch();

            if (inputChar == 32) {
                
                char unpausedChar = 0;
                do {
                    
                    Sleep(25);
                
                    if (_kbhit()) {

                        unpausedChar = _getch();
    
                    }
                
                } while (unpausedChar != 32);
                
            }

            else if (inputChar == 43) {

                milliseconds += MINUTES;

            }

            else if (inputChar == 45) {

                milliseconds -= MINUTES;

            }

        }        

        Sleep(25);

        milliseconds -= 25;

    }

}

int main(int argc, char *argv[]) {
    
    if (STRCMP(argv[1], "help")) {
        
        printf("\n    TIMER\n    -----\n");
        printf("\n    Time Unit Args:");
        printf("\n    -m/--minutes");
        printf("\n    -s/--seconds");
        printf("\n    -h/--hours");
        printf("\n    -p/--pomodoro");
        printf("\n\n    Example:\n    timer -h 1.5\n");
        return 0;

    }
    
    
    signal(SIGINT, handleSIGINT);   
 
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);    

    if (STRCMP(argv[1], "-p") || STRCMP(argv[1], "--pomodoro")) {
                
        puts("");
        while (TRUE) {          
           
            timer(MINUTES, 25 * MINUTES);
            timer(MINUTES, 5 * MINUTES);
          
        } 

    }

    if (argc == 1 || argc == 2) {
    
        printf("\n    ERROR: Incorred number of arguments specified.\n");
        goto exit;

    }

    if (STRCMP(argv[1], "0")) {

        goto exit;

    }

    float time = atof(argv[2]);
    int modifier;    

    if (time == 0) {
    
        printf("\n    ERROR: Argument must be a number.\n");
         goto exit;

    }
   
    if (time < 0) {

        printf("\n    ERROR: Time must be positive, whole integer.\n");
        goto exit;   

    }

    if (STRCMP(argv[1], "-s") || STRCMP(argv[1], "--seconds")) {

        modifier = SECONDS;

    }        

    else if (STRCMP(argv[1], "-m") || STRCMP(argv[1], "--minutes") ) {

        modifier = MINUTES;

    }

    else if (STRCMP(argv[1], "-h") || STRCMP(argv[1], "--hours")) {

        modifier = HOURS;

    }

    else {

        printf("\n    ERROR: Specify minutes or seconds (-s/--seconds, -m/--minutes)\n");
        goto exit;

    }

    puts("");

    int milliseconds = time * modifier;

    timer(modifier, milliseconds); 
    
    if (!argc) {

        return 0;

    } 

    printf("\n   Done waiting, press Ctrl^C to exit.\n");
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);    
    
    for (;;) {

        printf("\a");
        Sleep(2000);

    }
    
    exit:
    
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);  
    return 0;

}
