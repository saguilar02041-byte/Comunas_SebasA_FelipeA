#include "comunas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
======================================================================
            | Funcion que imprime el Titulo Principal |
======================================================================
*/
void TituloPrincipal(){
printf("╔══════════════════════════════════════════╗\n");
printf("║ ▄▄·       • ▌ ▄ ·. ▄• ▄▌ ▐ ▄  ▄▄▄· .▄▄ · ║\n");
printf("║▐█ ▌▪▪     ·██ ▐███▪█▪██▌•█▌▐█▐█ ▀█ ▐█ ▀. ║\n");
printf("║██ ▄▄ ▄█▀▄ ▐█ ▌▐▌▐█·█▌▐█▌▐█▐▐▌▄█▀▀█ ▄▀▀▀█▄║\n");
printf("║▐███▌▐█▌.▐▌██ ██▌▐█▌▐█▄█▌██▐█▌▐█ ▪▐▌▐█▄▪▐█║\n");
printf("║·▀▀▀  ▀█▄▀▪▀▀  █▪▀▀▀ ▀▀▀ ▀▀ █▪ ▀  ▀  ▀▀▀▀ ║\n");
printf("║ ▄▄▄· ▄▄▄· ▄▄▄   ▄▄▄· ·▄▄▄▄        ▐▄• ▄  ║\n");
printf("║▐█ ▄█▐█ ▀█ ▀▄ █·▐█ ▀█ ██▪ ██ ▪      █▌█▌▪ ║\n");
printf("║ ██▀·▄█▀▀█ ▐▀▀▄ ▄█▀▀█ ▐█· ▐█▌ ▄█▀▄  ·██·  ║\n");
printf("║▐█▪·•▐█ ▪▐▌▐█•█▌▐█ ▪▐▌██. ██ ▐█▌.▐▌▪▐█·█▌ ║\n");
printf("║.▀    ▀  ▀ .▀  ▀ ▀  ▀ ▀▀▀▀▀•  ▀█▄▀▪•▀▀ ▀▀ ║\n");
printf("╚══════════════════════════════════════════╝\n");
printf("════════════════════════════════════════════\n\n");
printf("          By Sebas & Felipe\n\n");
printf("════════════════════════════════════════════\n\n");
}

/*
======================================================================
        | Funciones que leen los archivos con nombres |
======================================================================
*/

int leerPersonas(){
    //abro el archivo
    FILE* personas = fopen("bienes", "r");

    //verifico que se encuentre
    if(personas == NULL){
        printf("ok, no encontré el archivo\n");
        return -1;
    }

    char nombre[30];
    
    while( feof(personas) == 0){
        fgets(nombre, 100, personas);
        printf("%s", nombre);
    }

    fclose("personas");
    return 0;
}

int main(){

    //Llamar funcion que imprime el titulo
    TituloPrincipal();
    leerPersonas();

    return 0;
}