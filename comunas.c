#include "comunas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
#include <ctype.h>
#include <time.h>

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
    imprimirLento(75,
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

void convertirMinusculas(char* str){
    for(int i=0; str[i]!= '\0'; i++){
        str[i] = tolower(str[i]);
    }
}

void agregarBienesDiccionario(int cantidad, struct DiccionarioBienes* diccionario){
    FILE* bienes = fopen("bienes", "r");

    //------------si queremos hacerlo random se debe hacer una función aparte
    
    char bien[30];
    for(int i=0; i<cantidad && fgets(bien,30,bienes) != NULL; i++){
        
        bien[strcspn(bien, "\n")] = '\0';   //para que en vez de un cambio de linea tenga \0
        convertirMinusculas(bien);          //lo convertiremos a minusculas

        int posicion = hashDiccionarioBienes(bien, diccionario->tamaño);    //calculo su posicion

        //saco una cantidad random
        int cantBien = randomIntervalo(2,15);

        struct nodoBienes* nodoBien = crearNodoBien(cantBien, bien);        //creo su nodo
        añadirBien(nodoBien, posicion, diccionario);                        //lo añado
    }

    fclose(bienes);
}

int randomIntervalo(int min, int max){
    int nRandom;
    nRandom = min + rand() % (max - min +1);
    return nRandom;
}

int hashDiccionarioBienes(char* texto, int tamaño){
    unsigned int hash = 0;                  //ok al parece ocupa ser unsigned para que si se desborda, no dé negativos
    for(int i=0; texto[i] != '\0'; i++){
        hash = (hash * 7) + texto[i];
    }
    return hash % tamaño;
}


/*
======================================================================
        | Funciones de las estructuras de datos |
======================================================================
*/

/*
=================================================
            | Diccionario de Bienes|
=================================================
*/

struct DiccionarioBienes* crearDiccionarioBienes(int tamaño){
    //creo el diccionario
    struct DiccionarioBienes* diccionario = calloc(1, sizeof(struct DiccionarioBienes)); 
    diccionario->tamaño = tamaño;
    
    //le añado la cantidad de espacios
    for(int i = 0; i < tamaño; i++){
        añadirAlDiccionario(diccionario);
    }
    return diccionario; 
}

struct nodoBienes* crearNodoBien(int cantidad, char* nombre){
    struct nodoBienes* nuevo = calloc(1, sizeof(struct nodoBienes));

    nuevo->nombre = strdup(nombre);     //al parecer el strdup mega arregla todo, no cambiar!!!!!
    nuevo->cantidad = cantidad;

    return nuevo;
}

void añadirAlDiccionario(struct DiccionarioBienes* diccionario){
    //creo el nodo
    struct nodoDic* nn = calloc(1, sizeof(struct nodoDic));

    //añado al inicio
    nn->sigt = diccionario->inicio;
    diccionario->inicio = nn;
}

void añadirBien(struct nodoBienes* bien, int pos, struct DiccionarioBienes* diccionario){
    struct nodoDic* actual = diccionario->inicio;
    
    //recorro el diccionario hasta encontrar la posicion
    for(int i=0; i<pos; i++){
        actual = actual->sigt;
    }

    //coloco el bien (ingresar al inicio) 
    struct nodoBienes* espacio = actual->bienes;    
    bien->sigt = espacio;
    actual->bienes = bien;
}

struct nodoBienes* buscarBien(struct DiccionarioBienes* diccionario, char* nombre){
    //copio lo que busco para no modificar el original  
    char buscado[30];                                   
    strncpy(buscado, nombre, 29);
    buscado[29] = '\0';
    convertirMinusculas(buscado);
    
    int pos = hashDiccionarioBienes(buscado, diccionario->tamaño);

    //recorro el diccionario hasta encontrar la posicion
    struct nodoDic* actual = diccionario->inicio;
    for(int i=0; i<pos; i++){
        actual = actual->sigt;
    }

    //dentro de la casilla busco el bien
    struct nodoBienes* bien = actual->bienes;
    while(bien != NULL){

        if(strcmp(bien->nombre, buscado) == 0)
            return bien;
        
        bien = bien->sigt;
    }

    return NULL; //si no lo encuentra
}

void imprimirDiccionarioBienes(struct DiccionarioBienes* diccionario){
    struct nodoDic* actual = diccionario->inicio;

    //recorro cada nodo del diccionario
    for(int i=0; i < diccionario->tamaño; i++){
        printf("%d. ", i);
        
        //busco todos lo que cayeron en la lista de dicho nodo
        struct nodoBienes* bien = actual->bienes;
        while(bien != NULL){
            printf("%s - ", bien->nombre);
            bien = bien->sigt;
        }

        printf("\n");
        actual = actual->sigt;
    }
}



/*
======================================================================
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    struct DiccionarioBienes* diccionario = crearDiccionarioBienes(10);
    agregarBienesDiccionario(10, diccionario);
    imprimirDiccionarioBienes(diccionario);

    char* nombre = "hArinA";
    struct nodoBienes* buscar = buscarBien(diccionario, nombre);

    printf("%d\n", buscar->cantidad);

    return 0;
}