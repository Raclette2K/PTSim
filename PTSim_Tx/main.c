
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lora_sim.h"

int main(int argc, char* argv[])
{
    printf("== PTLink PC Sim ==\n");

    // On définit le port par défaut
    void* port = "COM1";

    // Si on a des arguments on prend le premier comme le port à utiliser
    if(argc > 1)
        port = argv[1];

    printf(">Using port : %s.\n",port);

    getGPS(port);

    return 0;
}
