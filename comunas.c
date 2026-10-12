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

int pedirInt(){

    int valor;           
    while(1){
        if(scanf("%d", &valor) == 0){               //si el scan da error
            printf("te estoy pidiendo un número entero!!\n");
            getchar();                  //no entiendo por qué el getchar soluciona todos mis problemas, pero ahí está
            continue;
        }
        break;}
    return valor;
}

int pedirEnIntervalo(int min, int max){
    int n = pedirInt();
    while(n < min || max < n){
        
        if(n < min)
            printf("Muy poco!! try again\n");
        else if(n > max)
            printf("Demasiado!! try again\n");

    n = pedirInt();
    }
    return n;
}

/*
======================================================================
        | Funciones que leen los archivos con nombres |
======================================================================
*/

struct listaSimple* extraerRandomArchivo(char* nombreArchivo, int cantidad){
    //profe si ves esto, claude me ayudó bastante, tranquilo, no sabía como leer el archivo de manera random
    FILE* archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo");
        return NULL;
    }

    //leo TODAS las lineas del archivo a un arreglo que crece solo
    char** lineas = NULL;
    int total = 0, capacidad = 0;
    char buffer[128];

    while(fgets(buffer, sizeof(buffer), archivo) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';     //le quito el salto de linea
        if(buffer[0] == '\0') continue;             //ignoro lineas vacias

        if(total == capacidad){
            capacidad = (capacidad == 0) ? 16 : capacidad * 2;
            lineas = realloc(lineas, capacidad * sizeof(char*));
        }
        lineas[total++] = strdup(buffer);
    }
    fclose(archivo);

    if(cantidad > total) cantidad = total;          //no puedo sacar mas de los que hay

    //saco 'cantidad' al azar sin repetir
    struct listaSimple* lista = crearListaSimple();
    for(int i = 0; i < cantidad; i++){
        int j = randomIntervalo(i, total-1);        //escojo uno de los que faltan
        char* tmp = lineas[i]; lineas[i] = lineas[j]; lineas[j] = tmp;

        //el nodo se queda con este texto (no lo copia), por eso NO lo libero
        agregarNodoSimple(crearNodoListaSimple(lineas[i]), lista);
    }

    //libero solo los que NO se escogieron
    for(int i = cantidad; i < total; i++){
        free(lineas[i]);
    }
    free(lineas);

    return lista;
}

//diccionario de bienes

void agregarBienesDiccionario(struct listaSimple* bienesDisponibles, struct DiccionarioBienes* diccionario){

    struct nodoListaSimple* actual = bienesDisponibles->inicio;
    while(actual != NULL){
        char* bien = actual->nombre;

        int posicion = hashDiccionarioBienes(bien, diccionario->tamaño);    //calculo su posicion

        //saco una cantidad random
        int cantBien = randomIntervalo(3,15);

        struct nodoBienes* nodoBien = crearNodoBien(cantBien, bien);        //creo su nodo
        añadirBien(nodoBien, posicion, diccionario);                        //lo añado
    
        actual = actual->sigt;
    }
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
=================================================
    | Lista enlazada doble de Servicios|
=================================================
*/




/*
=================================================
        | Lista simple de Personas|
=================================================
*/

struct listaSimple* crearListaSimple(){
    struct listaSimple* nuevaLista = calloc(1, sizeof(struct listaSimple));
    nuevaLista->tamaño = 0;
    return nuevaLista;
}

struct nodoListaSimple* crearNodoListaSimple(char* nombre){
    struct nodoListaSimple* nuevoNodo = calloc(1, sizeof(struct nodoListaSimple));
    nuevoNodo->nombre = nombre;
    return nuevoNodo;
}

void agregarNodoSimple(struct nodoListaSimple* nodo, struct listaSimple* lista){
    nodo->sigt = lista->inicio;
    lista->inicio = nodo;
    lista->tamaño++;
}

void eliminarInicioSimple(struct listaSimple* lista){
    //agarro el que quiero eliminar
    struct nodoListaSimple* eliminado = lista->inicio;
    //hago cambio de inicio
    lista->inicio = eliminado->sigt;
    //lo borro
    free(eliminado);
    lista->tamaño--;
}

void imprimirListaSimple(struct listaSimple* lista){
    struct nodoListaSimple* actual = lista->inicio;

    while(actual != NULL){
        printf("%s\n", actual->nombre);
        actual = actual->sigt;
    }
}

/*
=================================================
                    | Comuna |
=================================================
*/

struct comuna* crearComuna(){
    struct comuna* nueva = calloc(1, sizeof(struct comuna));
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
    nuevoNodo->comuna = crearComuna();
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

struct nodoMatriz* recorrerMatriz(struct nodoMatriz* actual){
    struct nodoMatriz* sigt = actual->derecha;

    //si hay uno a la derecha entonces no pasa nada
    if(sigt != NULL)
        return sigt;

    //sino hay que devolverse
    sigt = actual;
    while(1){
        sigt = sigt->izquierda;

        if(sigt->izquierda == NULL)
            return sigt->abajo;
    }   
}

/*
======================================================================
                            | Imrpimir Matriz |
======================================================================
*/
void imprimirNumero(){


}
void imprimirSatis(){


}

void imprimirNece(){


}

void imprimirMatriz(struct matrizComunas* matriz){


}

/*
======================================================================
                            | Menues Juego |
======================================================================
*/

void iniciarPartida(){
    imprimirLento(75,
        "═════════════════════════════════════════════\n"
        "        Bienvenido a Comunas Paradox         \n"
        "                                             \n"
        "Un juego dónde debes administrar una sociedad\n"
        "de comunas para lograr aguantar la mayor     \n"
        "cantidad de días que puedas.                 \n"
        "                                             \n"
        "ENTER para continuar                         \n\n"
    );
    getchar();
    return;
}

/*
======================================================================
                            | Tablero |
======================================================================
*/

struct matrizComunas* crearTablero(){

    printf("Para empezar a jugar debes responder unas par de preguntas\n");

    //creo el tablero
    int tamañoTablero = preguntarDimensión();
    struct matrizComunas* tablero = crearMatriz(tamañoTablero);

    //le meto los diccionarios de bienes a cada comuna
    int tamañoDicc = preguntarCantidadBienes();
    struct listaSimple* bienesAJugar = extraerRandomArchivo("bienes", tamañoDicc);
    meterDiccionarios(bienesAJugar, tablero);

    return tablero;
}

int preguntarCantidadBienes(){
    printf("Bueno, ahora necesito la cantidad de bienes a jugar\n");
    printf("(mínimo 5, máximo 30)\n");

    int tamañoDicc = pedirEnIntervalo(5, 30);
    return tamañoDicc;
}

void meterDiccionarios(struct listaSimple* bienesDisponibles, struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    int i=0;
    int tamaño = tablero->tamaño;
    while(i < tamaño*tamaño){
        struct comuna* comunaActual = actual->comuna;
        
        //creo el diccionario y agrego los bienes
        comunaActual->diccionario = crearDiccionarioBienes(bienesDisponibles->tamaño);
        agregarBienesDiccionario(bienesDisponibles, comunaActual->diccionario);

        //paso al siguiente
        actual = recorrerMatriz(actual);
        i++;
    }
}

int preguntarDimensión(){
    printf("Necesito saber de qué tamaño quieres el tablero: \n");
    printf("(es una matriz cuadrada entre 3 a 7 de largo/ancho)\n");

    int tamañoTablero = pedirEnIntervalo(3,7);
    return tamañoTablero;
}


/*
======================================================================
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    struct matrizComunas* tablero = crearTablero();
    imprimirDiccionarioBienes(tablero->inicio->comuna->diccionario);

    return 0;
}



