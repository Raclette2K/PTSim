#include "com.h"

HANDLE _openPort(void** port)
{

	HANDLE hComm;
	// Ouverture du port en utilisant CreateFile() : Voir MSDN.
	hComm = CreateFile(*port, 			// Port COM ("COMX" < 10 et "\\\\.\\COMXX" > 11)
		GENERIC_READ | GENERIC_WRITE,	// On veut seulement lire et écrire.
		0,								// On veut l'utilisation exclusive du port.
		0,								// Aucun attribut de sécurité.
		OPEN_EXISTING,					// On utilise seulement les ports existants.
		0,								// On utilise les attributs par défaut.
		NULL);							// Paramètre ignoré avec OPEN_EXISTING.

    // Notifier quand le port est en cours d'utilisation.
	if (hComm == INVALID_HANDLE_VALUE)
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	// -- Paramétrage du port
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	// Récupération des paramètres du port
	if (!GetCommState(hComm, &dcbSerialParams))
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	dcbSerialParams.BaudRate = CBR_115200;	    // BaudRate = 115200 (PyCom)
	dcbSerialParams.ByteSize = 8;				// ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;		// StopBits = 1
	dcbSerialParams.Parity = NOPARITY;			// Parity = None

	// Mise à jour des paramètres du port
	if (!SetCommState(hComm, &dcbSerialParams))
		// Notifier s'il y a eu une erreur
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());

	// Retourne le HANDLE "contenant" le port.
	return hComm;
}

int portWrite(void* port, const void* buf, size_t nbyte)
{
	// On vérifie que le pointeur n'est pas nul
	if (port == NULL)
		return -1;

	// On ouvre le port
	HANDLE hComm = _openPort(&port);

	// Octets à écrire
	DWORD nbyteToWrite = nbyte;
	// Octets écrit sur le port
	DWORD nbyteWritten = 0;

	// On écrit le message sur le port
	if (!WriteFile(hComm, buf, nbyteToWrite, &nbyteWritten, NULL))
	{
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		// Retourne -1 puisque erreur lors de l'écriture
		return -1;
	}

	// On ferme le port
	CloseHandle(hComm);

	// -- Debug purposes --
	 printf("INFO : Wrote %u bytes to %s port.\n",(int)nbyteWritten,port);
	 printf("INFO : Wrote : %s\n",buf);
	// --------------------

	// Retourne le nombre d'octets écrit sur le port
	return (int)nbyteWritten;
}

int portRead(void* port, void* buf, size_t nbyte, unsigned int timeout)
{
	if (port == NULL)
		return -1;

	// Octets lus 
	DWORD nbyteRead = 0;
	// Event mask, voir MSDN.
	DWORD dwEventMask;

	// On ouvre le port
	HANDLE hComm = _openPort(&port);

	// Définition des timeouts
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = timeout; 
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	// On applique les paramètres des timeouts
	if (!SetCommTimeouts(hComm, &timeouts))
	{
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		return -1;
	}
	// On définit l'Event Mask
	if (!SetCommMask(hComm, EV_RXCHAR)) // EV_RXCHAR -> Quand un caractère est reçu
	{
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		return -1;
	}
	// On attend que l'évenement se produise
	if (WaitCommEvent(hComm, &dwEventMask, NULL))
		// On lit le port en stockant les caractères dans buf
		ReadFile(hComm, buf, nbyte, &nbyteRead, NULL);
	else
	{
		// Une erreur est arrivée pendant l'attente de l'évenement
		fprintf(stderr, "ERROR - %s : %u\n", __func__, GetLastError());
		// Retourne -1 puisque erreur
		return -1;
	}

	// On ferme le port.
	CloseHandle(hComm);

	// -- Debug message --
	printf("INFO : Read from %s port.\n",port);
	// -------------------

	// Retourne le nombre d'octets lus
	// Retourne 0 si on atteint l'EOF 
	return (int)nbyteRead;
}


