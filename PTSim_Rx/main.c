// PTSim_Rx
// Simule le comportement du tracker au niveau de l'animal.

#include <stdio.h>
#include <stdlib.h>

void sendGps(void* port)
{
    // Generate a ramdom decimal
	srand(time(NULL));
	int lat = rand() % 10;
	int lon = rand() % 10;

	char *buf[25] = {0};
	// Message to send
	sprintf(buf,"GPS:44.9%d98646;-0.7%d38005",lat,lon);

	// Send message to port
    portWrite(port,buf,25);

    printf(">GPS Coordinates sent.\n");
}

void gpsCmd(void* port)
{
    char *buf[6] = {0}; // Message is 6 characters long

    // While it didn't receive the command keep listening to port
    do{
        portRead(port,buf,6,10);
        printf("INFO : Read : %s\n",buf);
    }while(strcmp(buf,"GetGPS") != 0);

    printf(">Command GetGPS received.\n");
}
int main(int argc, char* argv[])
{
    printf("== PTLink Tracker Sim ==\n");

    void* port = "COM2";
    if(argc > 1)
        port = argv[1];

    printf(">Using port : %s.\n",port);
    while(1)
    {
    printf("- Wating for command...\n");
    // Wait for command.
    gpsCmd(port);

    printf("- Sending GPS coordinates...\n");
    // Send GPS coordinates
    sendGps(port);
    }

    return 0;
}
