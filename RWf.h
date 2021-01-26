#include <Windows.h>
#include <stdio.h>
#include <conio.h>

/*------------------------------------ Delay Function --------------------------------------------------*/

void delay(int wait_input) 
{ 
    int milli_seconds = 1 * wait_input;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds); 
}

/*------------------------------------ Read Function --------------------------------------------------*/

int readcom(HANDLE rhComm, BOOL rStatus)
{
    DWORD received;
    DWORD dwEventMask;
    char TempChar;
    char buffer[1];
    int i = 0;

    rStatus = SetCommMask(rhComm, EV_RXCHAR);
    rStatus = WaitCommEvent(rhComm, &dwEventMask, NULL);

    while(1)
    {
        rStatus = ReadFile(rhComm, &buffer[0], sizeof(buffer), &received, NULL);
        if(received>0)
        {
            printf("%c", buffer[0]);
            rStatus = ReadFile(rhComm, &buffer[0], sizeof(buffer), &received, NULL);
            
            if(received>0)
            {
                printf("%c", buffer[0]);
                rStatus = ReadFile(rhComm, &buffer[0], sizeof(buffer), &received, NULL);

                if(received>0)
                {
                    printf("%c", buffer[0]);
                    return 0;
                }
                return 0;
            }
            return 0;
        }
    }   
}

/*----------------------------- Writing an Integer to the Serial Port----------------------------------------*/


int writecomint(int input, HANDLE whComm, BOOL wStatus)
{
    char lpBuffer[1];       // lpBuffer should be  char or byte array, otherwise write wil fail
    sprintf(lpBuffer, "%d", input);
    DWORD dNoOFBytestoWrite;     // No of bytes to write into the port
    DWORD dNoOfBytesWritten = 0; // No of bytes written to the port

    dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

    wStatus = WriteFile(whComm,              // Handle to the Serialport
                        lpBuffer,           // Data to be written to the port
                        dNoOFBytestoWrite,  // No of bytes to write into the port
                        &dNoOfBytesWritten, // No of bytes written to the port
                        NULL);

    return 0;
}

/*----------------------------- Writing a Character to the Serial Port----------------------------------------*/

int writecomchar(char inputc, HANDLE whComm, BOOL wStatus)
{
    DWORD dNoOFBytestoWrite;
    DWORD dNoOfBytesWritten = 0; 
    char lpBuffer[1]; 
    lpBuffer[0] = inputc;

    dNoOFBytestoWrite = sizeof(lpBuffer); 

    wStatus = WriteFile(whComm,              
                        lpBuffer,           
                        dNoOFBytestoWrite,  
                        &dNoOfBytesWritten, 
                        NULL);

    return 0;
}