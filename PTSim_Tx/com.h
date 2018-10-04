/**
*   \file com.h
*   \brief Permet la communication avec les ports séries
*
*/
#ifndef __COM_H
#define __COM_H

#include <Windows.h>
#include <stdio.h>
#include <string.h>

/**
*   \brief Ouvre un port série
*   \param port Le port à ouvrir
*   \return Un HANDLE "contenant" le port ouvert
*/
HANDLE _openPort(void** port);

/**
*   \brief Ecrit sur le port
*   \param port Le port à ouvrir
*   \param buf Chaîne de caractères à écrire
*   \param nbyte Taille de buf
*   \return Le nombre d'octets écrit sur le port
*/
int portWrite(void* port, const void* buf, size_t nbyte);

/**
*   \brief Lit le port
*   \param port Le port à ouvrir
*   \param buf Chaîne de caractères qui reçoit ce qui est lu
*   \param nbyte Taille de buf
*   \param timeout Temps avant l'arrêt de la lecture du port (après premier octet reçu)
*   \return Le nombre d'octets lu ou 0 quand l'EOF est déclenché
*/
int portRead(void* port, void* buf, size_t nbyte, unsigned int timeout);

#endif
