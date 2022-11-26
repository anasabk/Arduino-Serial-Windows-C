#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "RWf.h"                       //Header file for read and write functions

int main()
{
    int portnum, in;
    char buffer[1024];
    printf(" Please enter the port number: ");
    scanf_s("%d", &portnum);

    HANDLE hComm;                       // Handle to the Serial port
    char PortName[12];                  // Name of the Serial port(May Change) to be opened,
    BOOL Status;
    sprintf_s(PortName, sizeof(PortName), "\\\\.\\COM%d", portnum);

/*------------------------------------ Interface --------------------------------------------------*/

    printf("\n\n +====================================================+");
    printf("\n |    Arduino Custom Serial Communication Project     |");
    printf("\n +====================================================+\n\n");
    printf(" ## WELCOME TO ARDUINO LAB              ##\n");
    printf(" ## STUDENT NAME: ENES ABDULHALIK       ##\n");
    printf(" ## PLEASE SELECT FROM THE FOLLOWING    ##\n");
    printf(" ## MENU :                              ##\n\n");
    printf(" (1) TURN ON LED\n");
    printf(" (2) TURN OFF LED\n");
    printf(" (3) FLASH ARDUINO LED 3 TIMES\n");
    printf(" (4) SEND A NUMBER TO THE MICROCONTROLLER TO COMPUTE ITS SQUARE\n");
    printf(" (5) BUTTON PRESS COUNTER\n");
    printf(" (6) ACTIVATE TEMPERATURE MONITOR ON LCD\n");
    printf(" (0) EXIT\n\n");

/*----------------------------------- Opening the Serial Port --------------------------------------------*/

    hComm = CreateFile(PortName,                        // Name of the Port to be Opened
                        GENERIC_READ | GENERIC_WRITE,   // Read/Write Access
                        0,                              // No Sharing, ports cant be shared
                        NULL,                           // No Security
                        OPEN_EXISTING,                  // Open existing port only
                        0,                              // Non Overlapped I/O
                        NULL);                          // Null for Comm Devices
                           
        
    if (hComm == INVALID_HANDLE_VALUE)
        printf("\n    Error! - Port %s can't be opened\n", PortName);

/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

    DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams);     //retreives the current settings

    if (Status == FALSE)
    {
        printf("\n   Couldn't open the port.");
        exit(0);
    }

    dcbSerialParams.BaudRate = CBR_115200;           // Setting BaudRate = 115200
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

    readcom(hComm, Status, buffer);
    if (buffer[0] == 'b') {                     //Check if the Arduino board is ready
        while (1) {
            writecomchar("a", hComm, Status);   //Send a message through the port
            readcom(hComm, Status, buffer);     //Read the response
            if (buffer[0] != 'b') {             //If the respon came as "b" then the Arduino board is up and running
                continue;
            }

            printf(" PLEASE SELECT A TASK: ");
            scanf_s("%d", &in);
            while (!getchar());

            delay(50);

            //Exit command
            if (in == 0) {
                CloseHandle(hComm);
                return 0;
            }

            //Square finder
            else if (in == 4) {
                writecomint(in, hComm, Status);         //Send the command code to the Arduino board
                printf("\n Please enter the number to find it's square: ");
                scanf_s("%d", &in);

                writecomint(in, hComm, Status);         //Send the integer requested to the Arduino board
                readcom(hComm, Status, buffer);         //Read the response

                printf(" The square of the number %d is %s", in, buffer);
                printf("\n\n ==========================================\n");
            }

            //Button press counter
            else if (in == 5) {
                int k = 0, in2 = 0;
                char* counterS = 0;

                printf("\n Please enter the maximum number of counts: ");
                scanf_s("%d", &in2);
                printf("\n Counting will stop at %d.\n", in2);

                writecomint(in, hComm, Status);         //Send the command code to the Arduino board

                while (in2 > 0) {                       //Keep running until the counter limit is reached
                    readcom(hComm, Status, buffer);     //Read the input

                    for (int i = 0; i < strlen(buffer); ++i)    //Take the integer from the buffer
                        if (buffer[i] < 58 && buffer[i] > 48) {
                            counterS = &buffer[i];
                            break;
                        }

                    printf("%s\n", counterS);
                    --in2;
                }

                writecomchar("a", hComm, Status);       //Tell the arduino board that the operation is done
            }

            //Temperature display
            else if (in == 6) {
                writecomint(in, hComm, Status);         //Send the command code to the Arduino board
                printf("\n Temperature monitoring through LCD is active.");
                
                //Check if the input is valid
                buffer[0] = 0;
                while (buffer[0] != 'a') {
                    printf("\n Please enter 'a' to stop monitoring.\n");
                    scanf_s("%c", buffer, 1);
                    if (buffer[0] != 'a')
                        printf("\n Enter a valid value.");
                }
                    
                buffer[1] = 0;
                writecomchar(buffer, hComm, Status);    //Send the command code to the Arduino board
                printf("\n ==========================================\n");
            }

            //LED flasher
            else if (in == 3) {
                writecomint(in, hComm, Status);
                delay(3000);
            }

            //Turn LED on or off
            else
                writecomint(in, hComm, Status);

            printf("\n ==========================================\n");
        }
    }

    return 0;
}