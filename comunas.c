#include "comunas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
/*
======================================================================
            | Funcion que Imprime Lentamente el Texto Principal |
======================================================================
*/
void imprimirLento(int retraso_ms, const char* formato, ...){
    char buffer[4096]; //tamano del buffer
    va_list args;

    va_start(args, formato);
    vsnprintf(buffer, sizeof(buffer), formato, args);
    va_end(args);

    char* inicio = buffer;
    char* fin;

    //Recorre el buffer línea por línea
    while ((fin = strchr(inicio, '\n')) != NULL){
        fwrite(inicio, 1, fin - inicio + 1, stdout);  // línea completa con su \n
        fflush(stdout);
        usleep(retraso_ms * 1000);
        inicio = fin + 1;
    }

    //Si queda texto sin \n al final, se imprime sin pausa
    if (*inicio != '\0'){
        fputs(inicio, stdout);
        fflush(stdout);
    }
}

/*
======================================================================
            | Funcion que imprime el Titulo Principal |
======================================================================
*/
void TituloPrincipal(){
    imprimirLento(200,
        "╔══════════════════════════════════════════╗\n"
        "║ ▄▄·       • ▌ ▄ ·. ▄• ▄▌ ▐ ▄  ▄▄▄· .▄▄ · ║\n"
        "║▐█ ▌▪▪     ·██ ▐███▪█▪██▌•█▌▐█▐█ ▀█ ▐█ ▀. ║\n"
        "║██ ▄▄ ▄█▀▄ ▐█ ▌▐▌▐█·█▌▐█▌▐█▐▐▌▄█▀▀█ ▄▀▀▀█▄║\n"
        "║▐███▌▐█▌.▐▌██ ██▌▐█▌▐█▄█▌██▐█▌▐█ ▪▐▌▐█▄▪▐█║\n"
        "║·▀▀▀  ▀█▄▀▪▀▀  █▪▀▀▀ ▀▀▀ ▀▀ █▪ ▀  ▀  ▀▀▀▀ ║\n"
        "║ ▄▄▄· ▄▄▄· ▄▄▄   ▄▄▄· ·▄▄▄▄        ▐▄• ▄  ║\n"
        "║▐█ ▄█▐█ ▀█ ▀▄ █·▐█ ▀█ ██▪ ██ ▪      █▌█▌▪ ║\n"
        "║ ██▀·▄█▀▀█ ▐▀▀▄ ▄█▀▀█ ▐█· ▐█▌ ▄█▀▄  ·██·  ║\n"
        "║▐█▪·•▐█ ▪▐▌▐█•█▌▐█ ▪▐▌██. ██ ▐█▌.▐▌▪▐█·█▌ ║\n"
        "║.▀    ▀  ▀ .▀  ▀ ▀  ▀ ▀▀▀▀▀•  ▀█▄▀▪•▀▀ ▀▀ ║\n"
        "╚══════════════════════════════════════════╝\n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "                   ║║║║                     \n"
        "___________________║║║║_____________________\n"
        "       ||       || ║║║║    ||       ||      \n"
        "════════════════════════════════════════════\n\n"
        "-----------By Sebas & Felipe----------------\n\n"
        "════════════════════════════════════════════\n\n\n\n"
    );
}

/*
======================================================================
        | Funciones que leen los archivos con nombres |
======================================================================
*/

int leerPersonas(){
    //abro el archivo
    FILE* personas = fopen("personas", "r");

    //verifico que se encuentre
    if(personas == NULL){
        printf("Error, no encontré el archivo\n");
        return -1;
    }

    char nombre[15];
    
    while( feof(personas) == 0){
        fgets(nombre, 15, personas);
        printf("%s", nombre);
    }

    fclose(personas);
    return 0;
}


/*
======================================================================
        | Funciones de las estructuras de datos |
======================================================================
*/

struct DiccionarioBienes crearDiccionarioBienes(){
    struct DiccionarioBienes* diccionario = calloc(1, sizeof(struct DiccionarioBienes)); 
    
    
    
}
void añadirAlDiccionario(){
    
}



/*
======================================================================
                                | Main |
======================================================================
*/
int main(){

    //Llamar funcion que imprime el titulo
    TituloPrincipal();
    leerPersonas();

    return 0;
}