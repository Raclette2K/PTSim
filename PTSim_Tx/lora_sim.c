#include "lora_sim.h"


void getGPS(void *port)
{
    // File in which coordinates will be written.
    // Fichier dans lequel les coordonnées seront écrites
    char *gps_filename = "C:\\PetTracker\\GPS_Data.txt";
    // Send "GetGPS" command to tracker (receiver)
    // Envoie de la commande "GetGPS" au tracker
    printf("- Sending command to tracker...\n");
    _sendCMD(port);
    // Receive answer from tracker with GPS coordinates
    // Réception des coordonnées du tracker
    printf("- Receiving GPS coordinates...\n");
    char *gps_str[21] = {0};
    _receiveGPS(port,gps_str);
    // Parse and write GPS coordinates into the GPS_Data.txt file
    // Convertie les coordonnées et formatte pour écrire dans GPS_Data.txt
    printf("- Writing coordinates to file...\n");
    _writeGPS(gps_str, 21, gps_filename);           // 21 est la taille totale du texte à écrire
}

void _sendCMD(void *port)
{
    size_t msg_size = 7;            // "GetGPS\r" fait 7 caractères
    char *buf = "GetGPS\r";
    // Ecrit le message dans le port
    portWrite(port,buf,msg_size);
    printf(">Command sent.\n");
}

void _receiveGPS(void *port, char *str_coord)
{
    size_t msg_size = 25;   // Le tracker envoie une chaîne de 25 caractères
    int timeout = 10;       // Temps avant d'arrêter la réception dès que le premier octet est reçu
    char *buf[25] = {0};    // Buffer où est stocké le message lu

    // On lit une première fois
    portRead(port,buf,msg_size,timeout);
    printf("INFO : Read : %s\n",buf);
    // Tant que le message lu ne contient pas "GPS:" on continue de lire le port
    while(strstr(buf,"GPS:") == NULL)
    {
        _sendCMD(port);                         // On renvoie la commande "GetGPS"
        portRead(port,buf,msg_size,timeout);    // Et on relit le port
        printf("INFO : Read : %s\n",buf);
    }
    printf(">Received GPS coordinates.\n");
    strtok(buf,":");                            // On enlève "GPS:" devant les coordonnées
    strcpy(str_coord,strtok(NULL,":"));         // On mets "lat;lon" reçu dans str_coord
}


void _writeGPS(char *gps_str, size_t str_size, char *file_name)
{
    double *gps_coord = parseCoord(gps_str,str_size);   // Retourne un tableau de double contenant lattitude et longitude
    writeGPSFile(gps_coord[0],gps_coord[1],file_name);  // Ecrit les coordonnées avec la date et l'heure dans le fichier
    printf(">GPS Coordinates wrote to file.\n");
}

