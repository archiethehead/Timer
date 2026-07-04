#include <stdlib.h>
#include <windows.h>
#include <signal.h>
#include <conio.h>

#define SECONDS 1000
#define MINUTES 60000
#define HOURS 3600000
#define BOOL char
#define STRCMP(x, y) !(strcmp(x, y))


typedef struct timeUnits{

    int secondsLeft;
    int minutesLeft;
    int hoursLeft;
    int milliseconds;
    int modifier;    

} timeInfo;
   
HANDLE consoleHandle;
CONSOLE_CURSOR_INFO info;

void handleSIGINT(int signalNum) {
    
    puts("");    
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);  
    exit(0);

}

void printTime(timeInfo* timeinfo){
    
    if (!(timeinfo->hoursLeft) && !(timeinfo->minutesLeft)) { 
                
        printf("\r   %d seconds remaining . . .", timeinfo->secondsLeft);
        printf("                                       ");            
        
    }

    else if (!(timeinfo->hoursLeft)) {
       
        printf("\r    %d minutes and ", timeinfo->minutesLeft);
        printf("%d seconds remaining . . .", timeinfo->secondsLeft % 60);
        printf("                                       ");            

    }
        
    else {
        
        printf("\r    %d hours, ", timeinfo->hoursLeft);
        printf("%d minutes, and ", timeinfo->minutesLeft % 60);
        printf("%d seconds remaining . . .", timeinfo->secondsLeft % 60);           
        printf("                                       ");            
                    

    }

}

void calculateTime(timeInfo* timeinfo) {

    int milliseconds = timeinfo->milliseconds;    

    timeinfo->hoursLeft = milliseconds / HOURS;
    timeinfo->minutesLeft = milliseconds / MINUTES;
    timeinfo->secondsLeft = milliseconds / SECONDS;

}

void timer(timeInfo* timeinfo) {
    
    BOOL paused = FALSE;
    
    while (timeinfo->milliseconds > 0) { 

        if (!paused) {

            Sleep(25);
            timeinfo->milliseconds -= 25;
            calculateTime(timeinfo);                        
            printTime(timeinfo);

        }
 
        if (_kbhit()) {

            char inputChar = _getch();

            if (inputChar == 32) {
               
                paused ^= 1;    
                
            }

            else if (inputChar == 43) {

                timeinfo->milliseconds += MINUTES;
                calculateTime(timeinfo);
                printTime(timeinfo);

            }

            else if (inputChar == 45) {

                timeinfo->milliseconds -= MINUTES;
                calculateTime(timeinfo);
                printTime(timeinfo);                

            }

        }        

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
    timeInfo timeinfo;
    timeinfo.milliseconds = milliseconds;
    timeinfo.modifier = modifier;
    timer(&timeinfo);
    
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
