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
            | Funciones utiles en toda la progra |
======================================================================
*/

void convertirMinusculas(char* str){
    for(int i=0; str[i]!= '\0'; i++){
        str[i] = tolower(str[i]);
    }
}

int randomIntervalo(int min, int max){
    int nRandom;
    nRandom = min + rand() % (max - min +1);
    return nRandom;
}

/*
======================================================================
        | Funciones que leen los archivos con nombres |
======================================================================
*/

void agregarBienesDiccionario(int cantidad, struct DiccionarioBienes* diccionario){
    FILE* bienes = fopen("bienes", "r");

    if(bienes == NULL){
        perror("No se pudo abrir 'bienes'");
        return;
    }
    
    //------------si queremos hacerlo random se debe hacer una función aparte
    
    char bien[30];
    for(int i=0; i<cantidad && fgets(bien,30,bienes) != NULL; i++){
        
        bien[strcspn(bien, "\n")] = '\0';   //para que en vez de un cambio de linea tenga \0
        convertirMinusculas(bien);          //lo convertiremos a minusculas

        int posicion = hashDiccionarioBienes(bien, diccionario->tamaño);    //calculo su posicion

        //saco una cantidad random
        int cantBien = randomIntervalo(3,15);

        struct nodoBienes* nodoBien = crearNodoBien(cantBien, bien);        //creo su nodo
        añadirBien(nodoBien, posicion, diccionario);                        //lo añado
    }

    fclose(bienes);
}

int hashDiccionarioBienes(char* texto, int tamaño){
    unsigned int hash = 0;                  //ok al parece ocupa ser unsigned para que si se desborda, no dé negativos
    for(int i=0; texto[i] != '\0'; i++){
        hash = (hash * 7) + texto[i];
    }
    return hash % tamaño;
}

/* terminar luego!!!!!!!!!!!!!!!!!!!!!!!!!!1
void agregarComunas(int cantidad, struct matrizComunas*){

    FILE* comunas = fopen("comunas", "r");

    if(comunas == NULL){
        perror("No se pudo abrir 'comunas'");
        return;
    }
    
    //------------si queremos hacerlo random se debe hacer una función aparte
    
    char comuna[50];
    for(int i=0; i<cantidad && fgets(comunas,50,comunas) != NULL; i++){

        comuna[strcspn(comuna, "\n")] = '\0';   //para que en vez de un cambio de linea tenga \0
    }
}*/

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
=================================================
            | Diccionario de Bienes|
=================================================
*/

struct comuna* crearComuna(char* nombre, int tamañoDicc){
    struct comuna* nueva = calloc(1, sizeof(struct comuna));
    
    //le pongo el nombre
    nueva->nombre = nombre;

    nueva->diccionario = crearDiccionarioBienes(tamañoDicc);
    agregarBienesDiccionario(tamañoDicc, nueva->diccionario);

    return nueva;
}

/*
=================================================
            | Matriz de comunas|
=================================================
*/

struct matrizComunas* crearMatriz(int tamaño){
    struct matrizComunas* nuevaComuna = calloc(1, sizeof(struct matrizComunas));

    nuevaComuna->tamaño = tamaño;
    nuevaComuna->inicio = crearNodoMatriz();

    //agrego una linea de nodos
    struct nodoMatriz* actual = nuevaComuna->inicio;
    int i=0;
    while(i++ < tamaño-1){
        actual->derecha = crearNodoMatriz();    //creo un nodo a la par
        enlazarDerecha(actual);                 //los enlazo

        actual->abajo = crearNodoMatriz();      //creo un nodo abajo
        enlazarAbajo(actual);                   //los enlazo

        actual = actual->derecha;               //paso al siguiente
        
        //si justamente es el ultimo le agrego uno abajo
        if(i == tamaño-1){
            actual->abajo = crearNodoMatriz();
            enlazarAbajo(actual);
        }
    }

    //me devuelvo
    actual = nuevaComuna->inicio;
    i=0;
    while(i++ < tamaño-1){
        enlazarNodosAbajo(actual);      //enlazo los nodos de abajo
        actual = actual->derecha;       //paso al siguiente
    }

    //empiezo a bajar
    actual = nuevaComuna->inicio->abajo;
    i=0;
    while(i++ < tamaño-2){

        //en la fila agrego nodos abajo
        for(int j=0; j < tamaño; j++){
            actual->abajo = crearNodoMatriz();
            enlazarAbajo(actual);
        
            //en el ultimo no se mueve a la derecha
            if(j < tamaño-1)
                actual = actual->derecha;
        }

        //me devuelvo al inicio de la fila
        for(int j=0; j < tamaño-1; j++){
            actual = actual->izquierda;
        }

        //empiezo a enlazar horizontalmente los de abajo
        for(int j=0; j < tamaño-1; j++){
            enlazarNodosAbajo(actual);
            actual = actual->derecha;
        }

        //me devuelvo al inicio de la fila
        for(int j=0; j < tamaño-1; j++){
            actual = actual->izquierda;
        }

        //bajo para hacer lo mismo denuevo si se ocupa
        actual = actual->abajo;
    }
    return nuevaComuna;
}

struct nodoMatriz* crearNodoMatriz(){
    struct nodoMatriz* nuevoNodo = calloc(1, sizeof(struct nodoMatriz));
    return nuevoNodo;
}


int enlazarDerecha(struct nodoMatriz* nodo){
    struct nodoMatriz* derecha = nodo->derecha;

    if(derecha == NULL){
        return 0;
    }

    derecha->izquierda = nodo;
    return 1;
}

int enlazarAbajo(struct nodoMatriz* nodo){
    struct nodoMatriz* abajo = nodo->abajo;

    if(abajo == NULL){
        return 0;
    }

    abajo->arriba = nodo;
    return 1;
}

int enlazarNodosAbajo(struct nodoMatriz* nodo){
    struct nodoMatriz* abajo = nodo->abajo;
    struct nodoMatriz* derecha = nodo->derecha;

    if(abajo==NULL || derecha==NULL)
        return 0;

    struct nodoMatriz* abajoDerecha = derecha->abajo;
    if(abajoDerecha==NULL)
        return 0;
    
    abajo->derecha = abajoDerecha;
    abajoDerecha->izquierda = abajo;
    return 1;
}


/*
======================================================================
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    struct matrizComunas* tablero = crearMatriz(4);

    printf("%d\n", tablero->tamaño);

    tablero->inicio->derecha->derecha->abajo->abajo->abajo->comuna = crearComuna("tercera", 10);
    printf("%s\n", tablero->inicio->derecha->derecha->abajo->abajo->abajo->comuna->nombre);




    return 0;
}