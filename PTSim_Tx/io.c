// io.c, created by Pierre Boisselier on 02.21.2018

#include "io.h"


void writeGPSFile(double lat, double lon, char* fileName)
{
	FILE *gpsF = fopen(fileName, "a");      // Argument 'a' pour "append" donc on écrit à la suite
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	if(!fprintf(gpsF, "%.7f;%.7f;%d-%d;%d-%d-%d;\\\n", lat, lon, tm->tm_hour, tm->tm_min, tm->tm_mday, tm->tm_mon+1,tm->tm_year-100))
		printf("ERROR : Can't write to %s\n",fileName);

}

double *parseCoord(char *message, int size_msg)
{

	static double coord[2]={0.0,0.0};

	// On utilise le token ';' pour séparer
	if(message != NULL)
	{
		char tmp[size_msg];
        strcpy(tmp,message);
		coord[0] = atof(strtok(tmp,";"));
		if(tmp != NULL)
			coord[1] = atof(strtok(NULL,","));
	}
	return coord;
}


