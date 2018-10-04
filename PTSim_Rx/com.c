//----------------------
//  Created 2.2.2018
//  by Pierre Boisselier
//----------------------

// This is a custom c file implementing the ATIM Arm(port) lib for use with Windows serial port.
// See https://msdn.microsoft.com/en-us/library/ff802693.aspx to learn about Windows serial communications.
// Some source code samples has been taken from MSDN.

// NOTE :
// To provide compatibility with this simulation, some methods were renamed and useless ones were removed

#include "com.h"


// Return an HANDLE linked to serial port.
HANDLE _openPort(void** port)
{

	HANDLE hComm;
	// Open port using CreateFile() : see MSDN.
	hComm = CreateFile(*port, // COM Port (name, "COMX" < 10 and "\\\\.\\COMXX" > 11)
		GENERIC_READ | GENERIC_WRITE,	// We want read and write rights, no need for elevated privileges instead of GENERIC_ALL requiring them.
		0,								// We want exclusive use of the serial port.
		0,								// Not using security attributes.
		OPEN_EXISTING,					// Only use existing ports.
		0,								// Using default attributes.
		NULL);							// Parameter ignored when using OPEN_EXISTING

    // Notify when handle is invalid, i.e. port is already in use.
	if (hComm == INVALID_HANDLE_VALUE)
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	// -- Setting serial port parameters
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	// Retrieving serial port parameters.
	if (!GetCommState(hComm, &dcbSerialParams))
		// Notify if there is an error executing last command.
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	dcbSerialParams.BaudRate = CBR_19200;	    // Setting BaudRate = 19200
	dcbSerialParams.ByteSize = 8;				// Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;		// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;			// Setting Parity = None

												// Update serial port configuration.
	if (!SetCommState(hComm, &dcbSerialParams))
		// Notify if there is an error executing last command.
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	// Return the Handle previously created and configured.
	return hComm;
}

int portWrite(void* port, const void* buf, size_t nbyte)
{
	// Checking if the pointer isn't null.
	if (port == NULL)
		return -1;

	// Open serial port.
	HANDLE hComm = _openPort(&port);

	// Number of bytes to write into serial port.
	DWORD nbyteToWrite = nbyte;
	// Number of bytes written to serial port.
	DWORD nbyteWritten = 0;

	// Write buffer in serial port.
	if (!WriteFile(hComm, buf, nbyteToWrite, &nbyteWritten, NULL))
	{
		// Notify if there is an error executing last command.
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		// Return -1 indicating error.
		return -1;
	}

	// Close the port.
	CloseHandle(hComm);

	// -- Debug purposes --
	 printf("INFO : Wrote %u bytes to %s port.\n",(int)nbyteWritten,port);
	 printf("INFO : Wrote : %s\n",buf);
	// --------------------

	// Return the number of bytes written to serial port.
	return (int)nbyteWritten;
}

int portRead(void* port, void* buf, size_t nbyte, unsigned int timeout)
{
	// Checking if the pointer isn't null.
	if (port == NULL)
		return -1;

	// Number of bytes read.
	DWORD nbyteRead = 0;
	// Event mask, see MSDN.
	DWORD dwEventMask;

	// Open port.
	HANDLE hComm = _openPort(&port);

	// Define timeouts for operations.
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = timeout;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	// Setting timeouts to port.
	if (!SetCommTimeouts(hComm, &timeouts))
	{
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		return -1;
	}
	// Setting event mask to port.
	if (!SetCommMask(hComm, EV_RXCHAR)) // EV_RXCHAR -> When a character is received
	{
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		return -1;
	}
	// Wait for event, change value of dwEventMask to which event occured.
	if (WaitCommEvent(hComm, &dwEventMask, NULL))
		// Read from port, store characters received into buffer
		ReadFile(hComm, buf, nbyte, &nbyteRead, NULL);
	else
	{
		// Error happened while trying to wait for event.
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		// Return -1 for error.
		return -1;
	}

	// Close port.
	CloseHandle(hComm);

	// -- Debug message --
	printf("INFO : Read from %s port.\n",port);
	// -------------------

	// Return number of bytes read.
	// Return 0 for EOF thus meaning everything went fine.
	return (int)nbyteRead;
}

void portDelay(unsigned int ms)
{
	// Replacing usleep() with SleepEx()
	// SleepEx pauses the current thread, instead of Sleep() that pauses the whole program.
	SleepEx(ms, FALSE);
}

