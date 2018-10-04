// io.h, created by Pierre Boisselier on 02.21.2018
/**
*   \file io.h
*   \brief Gère l'écriture du fichier GPS_Data.txt
*/
#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
*   \brief Ecrit les coordonnées avec la date et l'heure dans un fichier
*   \param lat Lattitude au format double
*   \param lon Longitude au format double
*   \param *fileName Chemin/Nom du fichier où écrire
*
*
*/
void writeGPSFile (double lat,double lon,char* fileName);
/**
*   \brief Convertie la chaîne de caractère au format "lat;lon" en lat et lon
*   \param *message Chaîne au format "lat;lon"
*   \param size_msg Taille du message à convertir
*   \return Retourne un tableau de 2 double
*/
double* parseCoord(char* message, int size_msg);

#endif
