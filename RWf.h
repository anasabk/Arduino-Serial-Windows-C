#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

#define READ_TIMEOUT      500      // milliseconds

/*------------------------------------ Delay Function --------------------------------------------------*/

void delay(int wait_input)
{
    int milli_seconds = 1 * wait_input;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

/*------------------------------------ Read Function --------------------------------------------------*/

char* readcom(HANDLE rhComm, BOOL rStatus, char* dest)
{
    DWORD received;
    DWORD dwEventMask;
    char buffer[1024];
    int i = 0;

    rStatus = SetCommMask(rhComm, EV_RXCHAR);

    if (!rStatus) {
        printf("Error while applying CommMask\n");
        exit(0);
    }

    WaitCommEvent(rhComm, &dwEventMask, NULL);
        
    while (i < 10) {
        rStatus = ReadFile(rhComm, &buffer[i], 1, &received, NULL);
        if (!rStatus) {
            printf("Error while Reading\n");
            exit(0);
        }
        
        if (buffer[i] == '\n')
            break;

        ++i;
    }

    buffer[i] = 0;
    strcpy(dest, buffer);

    return dest;
}

/*----------------------------- Writing an Integer to the Serial Port----------------------------------------*/


int writecomint(int input, HANDLE whComm, BOOL wStatus)
{
    char lpBuffer[8] = "\0\0\0\0\0\0\0\0";  // lpBuffer should be  char or byte array, otherwise write wil fail
    DWORD dNoOFBytestoWrite;     // No of bytes to write into the port
    DWORD dNoOfBytesWritten = 0; // No of bytes written to the port

    sprintf(lpBuffer, "%d", input);
    dNoOFBytestoWrite = (int)(log10(input) + 1); // Calculating the no of bytes to write into the port

    wStatus = WriteFile(whComm,              // Handle to the Serialport
        lpBuffer,           // Data to be written to the port
        dNoOFBytestoWrite,  // No of bytes to write into the port
        &dNoOfBytesWritten, // No of bytes written to the port
        NULL);

    return 0;
}

/*----------------------------- Writing a String to the Serial Port----------------------------------------*/

int writecomchar(char* inputS, HANDLE whComm, BOOL wStatus)
{
    DWORD dNoOFBytestoWrite;
    DWORD dNoOfBytesWritten = 0;

    dNoOFBytestoWrite = strlen(inputS);

    wStatus = WriteFile(whComm,
        inputS,
        dNoOFBytestoWrite,
        &dNoOfBytesWritten,
        NULL);

    return 0;
}
