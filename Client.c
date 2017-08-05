#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 1024
int main(int argc, char *argv[])
{
   HANDLE hPipe;
   LPTSTR lpvMessage= L"Default message from client!";
   TCHAR chBuf[BUFSIZE];
   BOOL fSuccess;
   DWORD cbRead, cbWritten, dwMode;
   LPTSTR lpszPipename = L"\\\\.\\pipe\\jim";

   if(argc > 1)
      lpvMessage = (LPTSTR)argv[1];
   while (1)
   {
      hPipe = CreateFile(
         lpszPipename,          // pipe name
         GENERIC_READ |   // read and write access
         GENERIC_WRITE,
         0,                              // no sharing
         NULL,                      // default security attributes
         OPEN_EXISTING, // opens existing pipe
         0,                              // default attributes
         NULL);                     // no template file
      // Break if the pipe handle is valid
      if (hPipe != INVALID_HANDLE_VALUE)
         break;
      // Exit if an error other than ERROR_PIPE_BUSY occurs
      if (GetLastError() != ERROR_PIPE_BUSY)
      {
         printf("CreateFile(): Could not open pipe! Error code %d\n", GetLastError());
         return 0;
      }
      // All pipe instances are busy, so wait for 20 seconds
      if (!WaitNamedPipe(lpszPipename, 20000))
      {
         printf("CreateFile(): Could not open pipe! They are busy, I do some waiting...\n");
         return 0;
      }
         printf("CreateFile() should be fine!\n");
   }

   // The pipe connected; change to message-read mode
   dwMode = PIPE_READMODE_MESSAGE;
   fSuccess = SetNamedPipeHandleState(
      hPipe,         // pipe handle
      &dwMode,  // new pipe mode
      NULL,         // don't set maximum bytes
      NULL);        // don't set maximum time
   if (!fSuccess)
   {
      printf("SetNamedPipeHandleState() failed! %d\n", GetLastError());
      return 0;
   }
   else
      printf("SetNamedPipeHandleState() is OK!\n");
   // Send a message to the pipe server
   fSuccess = WriteFile(
      hPipe,                           // pipe handle
      lpvMessage,                // message
      (lstrlen(lpvMessage)+1)*sizeof(CHAR), // message length
      &cbWritten,                  // bytes written
      NULL);                          // not overlapped

   if (!fSuccess)
   {
      printf("WriteFile() failed! Error code %d", GetLastError());
      return 0;
   }
   else
      printf("WriteFile() is OK!\n");

   do
   {
         // Read from the pipe
         fSuccess = ReadFile(
         hPipe,                                  // pipe handle
         chBuf,                                  // buffer to receive reply
         BUFSIZE*sizeof(CHAR),  // size of buffer
         &cbRead,                            // number of bytes read
         NULL);                                 // not overlapped

      if (! fSuccess && GetLastError() != ERROR_MORE_DATA)
      {
            printf("No more data!\n");
            break;
      }

      printf("%S\n", chBuf );
   } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA

 printf("Closing the pipe handle...\n");
 CloseHandle(hPipe);
 return 0;
}
