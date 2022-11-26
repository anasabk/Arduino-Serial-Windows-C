#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "RWf.h"                       //Header file for read and write functions

int main()
{
    int portnum, in;
    char buffer[20];
    printf(" Please enter the port number: ");
    scanf_s("%d", &portnum);

    HANDLE hComm;                       // Handle to the Serial port
    char PortName[12];                  // Name of the Serial port(May Change) to be opened,
    BOOL Status;
    sprintf_s(PortName, sizeof(PortName), "\\\\.\\COM%d", portnum);

/*------------------------------------ Interface --------------------------------------------------*/

    printf("\n\n  +====================================================+");
    printf("\n |     Arduino Custom Serial Communication Project      |");
    printf("\n  +====================================================+\n\n");
    printf(" ## WELCOME TO GTU ARDUINO LAB       ##\n");
    printf(" ## STUDENT NAME: Anas Abdul Khalek  ##\n");
    printf(" ## PLEASE SELECT FROM THE FOLLOWING ##\n");
    printf(" ## MENU :                           ##\n\n");
    printf(" (1) TURN ON LED ON ARDUINO\n");
    printf(" (2) TURN OFF LED ON ARDUINO\n");
    printf(" (3) FLASH ARDUINO LED 3 TIMES\n");
    printf(" (4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n");
    printf(" (5) BUTTON PRESS COUNTER\n");
    printf(" (6) ACTIVATE TEMPERATURE MONITOR WITH LCD\n");
    printf(" (0) EXIT\n\n");

/*----------------------------------- Opening the Serial Port --------------------------------------------*/

    hComm = CreateFile(PortName,                  // Name of the Port to be Opened
                        GENERIC_READ | GENERIC_WRITE, // Read/Write Access
                        0,                            // No Sharing, ports cant be shared
                        NULL,                         // No Security
                        OPEN_EXISTING,                // Open existing port only
                        0,                            // Non Overlapped I/O
                        NULL);                        // Null for Comm Devices
                           
        
    if (hComm == INVALID_HANDLE_VALUE)
        printf("\n    Error! - Port %s can't be opened\n", PortName);

/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

    DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams);  //retreives  the current settings

    if (Status == FALSE)
    {
        printf("\n   Couldn't open the port.");
        exit(0);
    }

    dcbSerialParams.BaudRate = CBR_115200;             // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;                    // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;           // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;               // Setting Parity = None

    Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB

    if (Status == FALSE)
    {
        printf("\n    Error! in Setting DCB Structure");
        exit(0);
    }

/*------------------------------------ Setting Timeouts --------------------------------------------------*/

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    SetCommTimeouts(hComm, &timeouts);

/*------------------------------------ Main Functions --------------------------------------------------*/
        
    while(1)
    {
        printf(" PLEASE SELECT A TASK: ");
        scanf_s("%d", &in);
        while (!getchar());

        delay(50);

        if(in==0){
            CloseHandle(hComm);
            return 0;
        }

        else if(in==4){
            writecomint(in, hComm, Status);
            printf("\n Please enter the number to find it's square: ");
            scanf_s("%d", &in);
            writecomint(in, hComm, Status);
            readcom(hComm, Status, buffer);
            printf(" The square of the number %d is %s", in, buffer);
            printf("\n\n ==========================================\n");
        }
            
        else if(in==5){
            int k;
            writecomint(in, hComm, Status);
            printf("\n Counting will stop at 10.\n");

            k = 0;
            while (1)
            {
                readcom(hComm, Status, buffer);
                k = buffer[0] - 48;
                if(k == 11)
                {
                    printf("\n ==========================================\n");
                    break;
                }
            }
        }

        else if(in==6){
            writecomint(in, hComm, Status);
            printf("\n Temperature monitoring through LCD is active.");
            printf("\n Please enter 'a' to stop monitoring.\n");
            scanf_s("%c", buffer, 1);
            buffer[1] = 0;
            writecomchar(buffer, hComm, Status);
            printf("\n ==========================================\n");
        }

        else if (in == 3) {
            writecomint(in, hComm, Status);
            delay(3000);
        }

        else 
            writecomint(in, hComm, Status);

        printf("\n ==========================================\n");
    }

    return 0;
}