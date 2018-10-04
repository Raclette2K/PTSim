/**
*   \file lora_sim.h
*   \brief Gère la communication avec le tracker
*/

#ifndef LORA_SIM_H
#define LORA_SIM_H

#include <stddef.h>
#include <stdio.h>

#include "io.h"
#include "com.h"


/**
*   \brief Récupère les coordonnées GPS du tracker
*   \param *port Port de communication avec le tracker
*
*   La fonction se charge d'appeller les autres fonctions dans l'ordre de facçon que les coordonnées soient écrites dans GPS_Data.txt.
*/
void getGPS(void *port);

/**
*   \brief Envoie la commande "GetGPS" au tracker
*   \param *port Port de communication avec le tracker
*
*   Ecrit "GetGPS\r" dans le port émulant un utilisateur qui écrirait et appuierais sur "Entrer".
*/
void _sendCMD(void *port);

/**
*   \brief Lis le port jusqu'à recevoir les coordonnées du tracker
*   \param *port Port de communication avec le tracker
*   \param *str_coord Chaîne de caractère où mettre les coordonnées ("lat;lon")
*
*   Lis le port jusqu'à recevoir un message contenant "GPS:" signifiant que le tracker envoie ses coordonnées.
*   Si le message ne contient pas "GPS:", la commande "GetGPS" est renvoyée.
*/
void _receiveGPS(void *port,char *str_coord);

/**
*   \brief Ecrit les coordonnées dans GPS_Data.txt
*   \param *gps_str Chaîne de caractère au format "lat;lon"
*   \param str_size Longueur de la chaîne à écrire
*   \param *file_name Chemin/Nom du fichier "GPS_Data.txt"
*
*   Convertie les chaîne au format "lat;lon" en un tableau de double puis écrit dans le fichier les coordonnées avec la date et l'heure.
*/
void _writeGPS(char *gps_str, size_t str_size, char *file_name);

#endif // LORA_SIM_H
