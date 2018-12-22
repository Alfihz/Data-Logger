#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Serial comm handler
HANDLE hSerial;
//Connection status
bool connected;
//Get various information about the connection
COMSTAT status;
//Keep track of last error
DWORD errors;

//Initialize Serial communication with the given COM port
void SerialOpen(const char *portName);

//Close the connection
void SerialClose();

//Read data in a buffer, if nbChar is greater than the
//maximum number of bytes available, it will return only the
//bytes available. The function return -1 when nothing could
//be read, the number of bytes actually read.
int ReadData(char *buffer, unsigned int nbChar);

//Writes data from a buffer through the Serial connection
//return true on success.
bool WriteData(const char *buffer, unsigned int nbChar);

//Check if we are actually connected
bool IsConnected();


void SerialOpen(const char *portName)
{
	 //We're not yet connected
    connected = false;

    //Try to connect to the given port throuh CreateFile
    hSerial = CreateFile(portName,
            			 GENERIC_READ | GENERIC_WRITE,
            			 0,
            			 NULL,
			             OPEN_EXISTING,
			             FILE_ATTRIBUTE_NORMAL,
			             NULL
						);

    //Check if the connection was successfull
    if(hSerial == INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if(GetLastError() == ERROR_FILE_NOT_FOUND)
            //Print Error if neccessary
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);
        else
            printf("ERROR!!!");
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(hSerial, &dcbSerialParams))
            //If impossible, show an error
            printf("failed to get current serial parameters!");
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate = CBR_9600;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

             //Set the parameters and check for their proper application
             if(!SetCommState(hSerial, &dcbSerialParams))
                printf("ALERT: Could not set Serial Port parameters");
             else
             {
                 //If everything went fine we're connected
                 connected = true;
                 //Flush any remaining characters in the buffers 
                 PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                 //We wait 2s as the arduino board will be reseting
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }
}

void SerialClose()
{
	//Check if we are connected before trying to disconnect
    if(connected)
    {
        //We're no longer connected
        connected = false;
        //Close the serial handler
        CloseHandle(hSerial);
    }
}

int ReadData(char *buffer, unsigned int nbChar)
{
	//Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(hSerial, &errors, &status);

    //Check if there is something to read
    if(status.cbInQue > 0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(status.cbInQue > nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead = status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(hSerial, buffer, toRead, &bytesRead, NULL) )
        {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return 0
    return 0;
}

bool WriteData(const char *buffer, unsigned int nbChar)
{
	DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile(hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(hSerial, &errors, &status);

        return false;
    }
    else
        return true;
}

bool IsConnected()
{
	return connected;
}

#endif // SERIALCLASS_H_INCLUDED
