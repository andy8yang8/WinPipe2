//     When this application successfully connects to a named pipe, 
//     the message "This is a test string" is written to the server.

//     There are four basic steps needed to implement a client:
//
//     1. Wait for a Named Pipe instance to become available using the WaitNamedPipe() API function.
//     2. Connect to the Named Pipe using the CreateFile() API function.
//     3. Send data to or receive data from the server using the WriteFile() and ReadFile() API functions.
//     4. Close the Named Pipe session using the CloseHandle() API functions.
//
// Command line options: None

#include <windows.h>
#include <stdio.h>

#define PIPE_NAME "\\\\.\\Pipe\\Jim"

void main(int argc, char **argv)
{
            HANDLE PipeHandle;
            DWORD BytesWritten;

            if (WaitNamedPipe((LPCWSTR)PIPE_NAME, NMPWAIT_WAIT_FOREVER) == 0)
            {
                        printf("WaitNamedPipe() failed with error code %d\n", GetLastError());
                        return;

            }
            else
                        printf("WaitNamedPipe() is OK!\n");
 

            // Create the named pipe file handle

            if ((PipeHandle = CreateFile((LPCWSTR)PIPE_NAME,
                        GENERIC_READ | GENERIC_WRITE, 0,
                        (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        (HANDLE) NULL)) == INVALID_HANDLE_VALUE)
            {
                        printf("CreateFile() failed with error code %d\n", GetLastError());
                        return;
            }
            else
                        printf("CreateFile() is OK!\n");
 

            if (WriteFile(PipeHandle, "This is a test string", 21, &BytesWritten,  NULL) == 0)
            {
                        printf("WriteFile() failed with error code %d\n", GetLastError());
                        printf("Closing the handle...\n");
                        CloseHandle(PipeHandle);
                        return;
            }

            else
                        printf("WriteFile() is fine!\n");
           
           printf("Wrote %d bytes", BytesWritten);
           printf("Closing the handle...\n");

           CloseHandle(PipeHandle);

}