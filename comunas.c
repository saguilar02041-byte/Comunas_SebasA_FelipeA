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
            | Constantes del juego |
======================================================================
*/

//dias y consumo
#define CONSUMO_MAX             12       //lo maximo que baja un bien/servicio en un dia
#define PRODUCCION_PROB         50      //probabilidad de que un bien/servicio suba en el dia

//emergencias
#define EMERGENCIA_MIN_DIAS     1       //cada cuantos dias ocurre una emergencia, minimo
#define EMERGENCIA_MAX_DIAS     3       //maximo
#define TORMENTA_MAX_DIAS       3       //duracion de tormenta

//intercambios
#define RETORNO_REGALO          30      //porcentaje de regreso al intercambiar
#define INTERCAMBIOS_MAX_DIA    3       //cuantos intercambios puede hacer una comuna por dia

/*
======================================================================
            | Funcion que Imprime Lentamente |
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
        "════════════════════════════════════════════\n"
        "-----------By Sebas & Felipe----------------\n"
        "════════════════════════════════════════════\n\n"
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
            printf("| !! Se necesita un número ENTERO  !! | \n\n");
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
            printf("\n| !! Muy poco | try again !! \n\n");
        else if(n > max)
            printf("\n| !! Demasiado | try again !! \n\n");

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
        perror("| !! No se pudo abrir el archivo !! |");
        return NULL;
    }

    //paso todas las lineas del archivo a una lista simple
    struct listaSimple* todas = crearListaSimple();
    char buffer[128];
    while(fgets(buffer, sizeof(buffer), archivo) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(buffer[0] == '\0') continue;

        agregarNodoSimple(crearNodoListaSimple(strdup(buffer)), todas);
    }
    fclose(archivo);

    if(cantidad > todas->tamaño) cantidad = todas->tamaño;

    //saco 'cantidad' nodos al azar de 'todas' y los paso a 'escogidos'
    struct listaSimple* escogidos = crearListaSimple();
    for(int i = 0; i < cantidad; i++){
        int pos = randomIntervalo(0, todas->tamaño - 1);

        //busco el nodo en esa posicion (y el anterior, para poder desenlazarlo)
        struct nodoListaSimple* anterior = NULL;
        struct nodoListaSimple* actual = todas->inicio;
        for(int j = 0; j < pos; j++){
            anterior = actual;
            actual = actual->sigt;
        }

        //lo desenlazo de 'todas'
        if(anterior == NULL)
            todas->inicio = actual->sigt;
        else
            anterior->sigt = actual->sigt;
        todas->tamaño--;

        //y lo agrego a 'escogidos' (el nodo se mueve, no se copia)
        agregarNodoSimple(actual, escogidos);
    }

    //libero lo que no se escogio
    while(todas->inicio != NULL){
        struct nodoListaSimple* sig = todas->inicio->sigt;
        free(todas->inicio->nombre);
        free(todas->inicio);
        todas->inicio = sig;
    }
    free(todas);

    return escogidos;
}

//diccionario de bienes

void agregarBienesDiccionario(struct listaSimple* bienesDisponibles, struct DiccionarioBienes* diccionario, int personas){

    struct nodoListaSimple* actual = bienesDisponibles->inicio;
    while(actual != NULL){
        char* bien = actual->nombre;

        int posicion = hashDiccionarioBienes(bien, diccionario->tamaño);    //calculo su posicion

        //calculo su maximo
        int maximo = personas * randomIntervalo(1,3);

        //saco una cantidad random
        int cantBien = randomIntervalo(maximo * 4/10, maximo * 8/10);

        struct nodoBienes* nodoBien = crearNodoBien(cantBien, bien);        //creo su nodo
        añadirBien(nodoBien, posicion, diccionario);                        //lo añado
        nodoBien->maximo = maximo;                                          //añado el maximo
        
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

    printf(
        "\n"
    );

    //recorro cada nodo del diccionario
    for(int i=0; i < diccionario->tamaño; i++){
        
        //busco todos lo que cayeron en la lista de dicho nodo
        struct nodoBienes* bien = actual->bienes;
        while(bien != NULL){
            printf(" - %s %d , max:%d\n", bien->nombre, bien->cantidad, bien->maximo);
            bien = bien->sigt;
        }
        actual = actual->sigt;
    }
    printf("\n");

}

/*
=================================================
                | Lista simple |
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

void imprimirResumenTablero(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* c = actual->comuna;
        printf("\n |  %02d %-14s  |  personas = %3d  |  necesidad = %5.1f  |  satisfaccion = %5.1f  |\n",
                c->numero, c->nombre, c->personas->tamaño, c->necesidad, c->satisfaccion);
        actual = recorrerMatriz(actual);
    }
}

void imprimirNumero(struct comuna* comuna){
//╔══00══╗
    printf(" ╔══%02d══╗ ", comuna->numero);
}

void imprimirSatisfaccion(struct comuna* comuna){
//╠S:99.9║
    printf(" ╠S%5.1f║ ", comuna->satisfaccion);
}

void imprimirNecesidad(struct comuna* comuna){
//╠N:99.9║
    printf(" ╠N%5.1f║ ", comuna->necesidad);
}

void imprimirRelleno(){
//╚══════╝
    printf(" ╚══════╝ ");
}

void imprimirLineaFila(struct nodoMatriz* inicioFila, int linea){
    struct nodoMatriz* actual = inicioFila;

    while(actual != NULL){              //avanzo a la derecha hasta que se acabe la fila
        if(linea == 0)
            imprimirNumero(actual->comuna);
        else if(linea == 1)
            imprimirSatisfaccion(actual->comuna);
        else if(linea == 2)
            imprimirNecesidad(actual->comuna);
        else
            imprimirRelleno();

        actual = actual->derecha;
    }
    printf("\n");                       //termino la linea
}

void imprimirTablero(struct matrizComunas* tablero){
    struct nodoMatriz* inicioFila = tablero->inicio;

    while(inicioFila != NULL){
        for(int linea = 0; linea < 4; linea++){
            imprimirLineaFila(inicioFila, linea);
        }
        inicioFila = inicioFila->abajo;     //bajo a la siguiente fila (despues de la ultima es NULL)
    }
}


/*
======================================================================
                            | Tablero |
======================================================================
*/

struct matrizComunas* crearTablero(){
    TituloPrincipal();
    imprimirLento(150,
        " ╔════════════════════════════════════╗\n"
        " ║¡ES HORA DE PERSONALIZAR TU PARTIDA!║\n"
        " ╚════════════════════════════════════╝\n"
        "                                        \n\n"
    );

    //creo el tablero
    int tamañoTablero = preguntarDimensión();
    struct matrizComunas* tablero = crearMatriz(tamañoTablero);

    //le meto un nombre a cada comuna
    struct listaSimple* nombresComunas = extraerRandomArchivo("comunas", tablero->tamaño * tablero->tamaño);
    meterNombres(nombresComunas, tablero);

    //le meto un numero a cada comuna
    meterNumeros(tablero);

    //le meto las personas a cada comuna
    struct listaSimple* personasAJugar = extraerRandomArchivo("personas", 500); 
    meterPersonas(personasAJugar, tablero);

    //saco los bienes
    int tamañoDicc = preguntarCantidadBienes();
    struct listaSimple* bienesAJugar = extraerRandomArchivo("bienes", tamañoDicc);

    //saco los servicios
    tamañoDicc = preguntarCantidadServicios();
    struct listaSimple* serviciosAJugar = extraerRandomArchivo("servicios", tamañoDicc);    
    
    //dias a jugar y dejo listo el dia actual y la primera emergencia, viste jeje
    tablero->diasTotales = preguntarCantidadDias();
    tablero->dia = 1;
    tablero->diasParaEmergencia = randomIntervalo(EMERGENCIA_MIN_DIAS, EMERGENCIA_MAX_DIAS);

    //meto los dos a cada comuna
    meterDiccionarios(serviciosAJugar, bienesAJugar, tablero);

    //guardo los bienes y servicios disponibles
    tablero->bienesDisponibles = bienesAJugar;
    tablero->serviciosDisponibles = serviciosAJugar;

    calcularIndicesTablero(tablero);

    return tablero;
}

int preguntarCantidadBienes(){
    imprimirLento(100,
        " ╔════════════════════════════════════╗\n"
        " ║ 2. Cantidad de bienes a jugar: __  ║\n"
        " ╚════════════════════════════════════╝\n"
        " |   Mínimo = 5    |    Máximo = 30   |\n"
        "                                       \n\n"
    );

    int tamañoDicc = pedirEnIntervalo(5, 30);
    return tamañoDicc;
}

int preguntarCantidadServicios(){
    imprimirLento(100,
        " ╔═══════════════════════════════════════╗\n"
        " ║ 3. Cantidad de Servicios a jugar: __  ║\n"
        " ╚═══════════════════════════════════════╝\n"
        " |    Mínimo = 5     |    Máximo = 30    |\n"
        "                                          \n\n"
    );

    int tamañoDicc = pedirEnIntervalo(5, 30);
    return tamañoDicc;
}

int preguntarCantidadDias(){
    imprimirLento(100,
        " ╔════════════════════════════════════╗\n"
        " ║ 4. Cantidad de días a jugar: __    ║\n"
        " ╚════════════════════════════════════╝\n"
        " |   Mínimo = 4    |    Máximo = 99   |\n"
        "                                       \n\n"
    );

    int dias = pedirEnIntervalo(4, 99);
    return dias;
}

void meterNumeros(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    int numero = 0;
    int tamaño = tablero->tamaño * tablero->tamaño;

    for(int i=0; i < tamaño; i++){
        struct comuna* comunaActual = actual->comuna;
        
        comunaActual->numero = numero;      //le pongo el numero

        //si es el ultimo de la linea entonces paso de 00 a 10 a 20, como sea
        if(actual->derecha == NULL)
            numero = numero - tablero->tamaño + 11;
        //sino solo se suma 1
        else
            numero++;

        actual = recorrerMatriz(actual);
    }
}


void meterDiccionarios(struct listaSimple* serviciosDisponibles, struct listaSimple* bienesDisponibles, struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    int i=0;
    int tamaño = tablero->tamaño;
    while(i < tamaño*tamaño){
        struct comuna* comunaActual = actual->comuna;
        
        //creo el diccionario y agrego los bienes
        comunaActual->diccionarioBienes = crearDiccionarioBienes(bienesDisponibles->tamaño);
        agregarBienesDiccionario(bienesDisponibles, comunaActual->diccionarioBienes, comunaActual->personas->tamaño);

        //creo el diccionario y agrego los servicios
        comunaActual->diccionarioServicios = crearDiccionarioBienes(serviciosDisponibles->tamaño);
        agregarBienesDiccionario(serviciosDisponibles, comunaActual->diccionarioServicios, comunaActual->personas->tamaño);

        //paso al siguiente
        actual = recorrerMatriz(actual);
        i++;
    }
}

void meterNombres(struct listaSimple* nombresComunas, struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    int i=0;
    int tamaño = tablero->tamaño;
    while(i < tamaño*tamaño){
        struct comuna* comunaActual = actual->comuna;

        //le pongo el primer nombre de la lista
        comunaActual->nombre = nombresComunas->inicio->nombre;
        //quito ese primer nombre
        eliminarInicioSimple(nombresComunas);

        actual = recorrerMatriz(actual);
        i++;
    }
}

void meterPersonas(struct listaSimple* personasDisponibles, struct matrizComunas* tablero){
    int totalComunas = tablero->tamaño * tablero->tamaño;
    int total = personasDisponibles->tamaño;

    int base = total / totalComunas;        //lo que le tocaria a cada una si fuera parejo
    int resto = total % totalComunas;       //lo que sobra de esa division
    int desviacionMax = base * 6 / 10;      //hasta 60% arriba o abajo del promedio

    int compensacion = 0;                   //lo que una comuna gana/pierde respecto a su pareja
    struct nodoMatriz* actual = tablero->inicio;

    for(int i = 0; i < totalComunas; i++){
        int cantidad;

        if(i % 2 == 0 && i < totalComunas-1){
            //primera de la pareja: se desvia del promedio (para arriba o para abajo)
            compensacion = randomIntervalo(-desviacionMax, desviacionMax);
            cantidad = base + compensacion;
        }
        else if(i % 2 == 1){
            //segunda de la pareja: recibe lo contrario, así la pareja suma 2*base
            cantidad = base - compensacion;
        }
        else{
            //si son impares, la última se queda sola con el promedio
            cantidad = base;
        }

        if(i < resto) cantidad++;           //reparto lo que sobro de la division

        //le paso 'cantidad' personas
        struct comuna* comunaActual = actual->comuna;
        comunaActual->personas = crearListaSimple();

        for(int k = 0; k < cantidad && personasDisponibles->inicio != NULL; k++){
            struct nodoListaSimple* persona = personasDisponibles->inicio;

            //la saco de la lista general
            personasDisponibles->inicio = persona->sigt;
            personasDisponibles->tamaño--;

            //y la meto en la comuna
            agregarNodoSimple(persona, comunaActual->personas);
        }

        actual = recorrerMatriz(actual);
    }

}

int preguntarDimensión(){
    imprimirLento(100,
        " ╔══════════════════════════════════════════╗\n"
        " ║ 1.  Escoge el tamaño del tablero  !!     ║\n"
        " ╚══════════════════════════════════════════╝\n"
        " | se juega en una matriz cuadrada de 3 a 7 |\n"
        "                                             \n\n"
    );

    int tamañoTablero = pedirEnIntervalo(3,7);
    return tamañoTablero;
}

/*
======================================================================
                            | Cálculos |
======================================================================
*/

#define NECESIDAD_MIN 5.0  //siempre hay necesidades extra: nunca llega a 0
#define NECESIDAD_MAX 99.0  //nunca llega a 100

#define PESO_PROPIA   0.55  //que tan bien estoy yo
#define PESO_VECINOS  0.25  //que tan bien estan los de alrededor
#define PESO_EQUIDAD  0.20  //que tan parecido es mi nivel al de mis vecinos

//suma cuanto falta y cuanto es el maximo de todos los elementos de un diccionario
void sumarFaltantes(struct DiccionarioBienes* diccionario, int* faltante, int* maximo){
    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            *maximo += bien->maximo;
            if(bien->cantidad < bien->maximo)
                *faltante += bien->maximo - bien->cantidad;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
}

//necesidad = % de bienes y servicios que faltan para llegar al maximo
double calcularNecesidad(struct comuna* comuna){
    int faltante = 0, maximo = 0;
    sumarFaltantes(comuna->diccionarioBienes, &faltante, &maximo);
    sumarFaltantes(comuna->diccionarioServicios, &faltante, &maximo);

    double necesidad = (maximo == 0) ? NECESIDAD_MIN : 100.0 * faltante / maximo;

    if(necesidad < NECESIDAD_MIN) necesidad = NECESIDAD_MIN;
    if(necesidad > NECESIDAD_MAX) necesidad = NECESIDAD_MAX;
    return necesidad;
}

//suma el "bienestar" de un vecino, si existe
void sumarVecino(struct nodoMatriz* vecino, double* suma, int* cuenta){
    if(vecino == NULL) return;

    *suma += 100.0 - calcularNecesidad(vecino->comuna);
    (*cuenta)++;
}

//vecinos = las 8 comunas que rodean: esquina=3, borde=5, centro=8
void sumarVecinos(struct nodoMatriz* nodo, double* suma, int* cuenta){
    sumarVecino(nodo->arriba, suma, cuenta);
    sumarVecino(nodo->abajo, suma, cuenta);
    sumarVecino(nodo->izquierda, suma, cuenta);
    sumarVecino(nodo->derecha, suma, cuenta);

    //las diagonales se alcanzan dando dos pasos
    sumarVecino(nodo->arriba ? nodo->arriba->izquierda : NULL, suma, cuenta);
    sumarVecino(nodo->arriba ? nodo->arriba->derecha : NULL, suma, cuenta);
    sumarVecino(nodo->abajo ? nodo->abajo->izquierda : NULL, suma, cuenta);
    sumarVecino(nodo->abajo ? nodo->abajo->derecha : NULL, suma, cuenta);
}

//calcula y guarda la necesidad y la satisfaccion de la comuna de ese nodo
void calcularIndices(struct nodoMatriz* nodo){
    struct comuna* comuna = nodo->comuna;

    double necesidad = calcularNecesidad(comuna);
    double propia = 100.0 - necesidad; 

    double suma = 0;
    int cuenta = 0;
    sumarVecinos(nodo, &suma, &cuenta);
    double vecinos = (cuenta > 0) ? suma / cuenta : propia;

    double diferencia = propia - vecinos;
    if(diferencia < 0) diferencia = -diferencia;
    double equidad = 100.0 - diferencia;

    comuna->necesidad = necesidad;
    comuna->satisfaccion = PESO_PROPIA * propia
                         + PESO_VECINOS * vecinos
                         + PESO_EQUIDAD * equidad;
}

//recalcula los indices de todas las comunas del tablero
void calcularIndicesTablero(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        calcularIndices(actual);
        actual = recorrerMatriz(actual);
    }
}

/*
======================================================================
                        | Menú de comuna |
======================================================================
*/

void imprimirPersonas(struct listaSimple* personas, char* nombre){
    struct nodoListaSimple* actual = personas->inicio;
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Personas de la comuna %s :\n", nombre);
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");


    int i=0;
    while(actual != NULL){
        printf("%s\t", actual->nombre);

        if(strlen(actual->nombre) < 8)
            printf("\t");

        if(i == 6){
            printf("\n");
            i = -1;
        }

        i++;
        actual = actual->sigt;
    }
    printf("\n");
}

void imprimirComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero){

    struct comuna* comuna = nodo->comuna;
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Te encuentras en la comuna %02d %s\n", comuna->numero, comuna->nombre);
    printf(" ╚═════════════════════════════════════════════===+- -\n");
    printf(" |                       |                     |\n");
    printf(" |  Satisfacción : %5.1f |  Necesidad : %5.1f  |\n\n", comuna->satisfaccion, comuna->necesidad);

    while(1){
        imprimirLento(100,
        " ╔════════════════════════╗\n"
        " ║  * Escoge una acción   ║\n"
        " ╚════════════════════════╝\n"
        " | 0. Ver Bienes          |\n"
        " | 1. Ver Servicios       |\n"
        " | 2. Ver Personas        |\n"
        " | 3. Revisar otra comuna |\n"
        " | 4. Intercambiar        |\n\n"
        );

        int opcion = pedirEnIntervalo(0,4);

        if(opcion == 0){
            imprimirLento(70,
                "\n___________________________________________________\n"
                " ╔══════════════╗\n"
                " ║  * Bienes:   ║\n"
                );
            imprimirDiccionarioBienes(comuna->diccionarioBienes);
            printf("\n___________________________________________________\n");
        }else if(opcion == 1){
            imprimirLento(70,
                "\n___________________________________________________\n"
                " ╔═════════════════╗\n"
                " ║  * Servicios:   ║\n"
                );
            imprimirDiccionarioBienes(comuna->diccionarioServicios);
            printf("\n___________________________________________________\n");
        }else if(opcion == 2){
            imprimirPersonas(comuna->personas, comuna->nombre);
        }else if(opcion == 3){
            return;
        }else{
            realizarIntercambio(tablero);
        }
    }
    printf("\n___________________________________________________\n");
}

struct nodoMatriz* pedirComuna(struct nodoMatriz* actual, struct matrizComunas* tablero){
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Comuna actual: %d    | Día actual: %d / %d\n", actual->comuna->numero, tablero->dia, tablero->diasTotales);
    printf(" ║\n");
    printf(" ║ Tus opciones son: \n");
    printf(" ║ 0. Escribir el numero de la comuna\n");
    printf(" ║ 1. Moverte al adyacente | arriba | abajo | izq | der |\n");
    printf(" ║ 2. Terminar día\n");
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
    int opcion = pedirEnIntervalo(0,2);

    if(opcion == 0){
        return buscarComuna(tablero);
    }else if(opcion == 2){
        pasarDia(tablero);
        return actual;
    }else{
        imprimirLento(70,
        " ╔════════════════════════════════╗\n"
        " ║ * Hacia dónde te quieres mover ║\n"
        " ╚════════════════════════════════╝\n"
        " | 0. Arriba                      |\n"
        " | 1. Abajo                       |\n"
        " | 2. Derecha                     |\n"
        " | 3. Izquierda                   |\n\n"
        );

        struct nodoMatriz* elegido;
        
        while(1){
            opcion = pedirEnIntervalo(0,3);
        
            if(opcion == 0)elegido = actual->arriba;
            else if(opcion == 1)elegido = actual->abajo;
            else if(opcion == 2)elegido = actual->derecha;
            else elegido = actual->izquierda;
        
            if(elegido == NULL){
                imprimirLento(70,
                    "\n______________________________________\n"
                    "                                      \n"
                    " !! NO existe | Intente de nuevo !!   \n"
                    "______________________________________\n\n"
        );
                continue;
            }

            return elegido;
        }
    }
}

struct nodoMatriz* buscarComuna(struct matrizComunas* tablero){
    imprimirLento(70,
        " ╔═══════════════════════════════════════════╗\n"
        " ║ * Numero de comuna a interactuar          ║\n"
        " ╚═══════════════════════════════════════════╝\n\n"
    );

    int tamaño = tablero->tamaño;
    int pos = 0;
    while(1){
        pos = pedirInt();

        if(pos < 0 || pos%10 >= tamaño || pos/10 >= tamaño){
            imprimirLento(70,
                    "______________________________________\n"
                    "                                      \n"
                    " !! Esa posición no existe !!         \n"
                    "______________________________________\n\n"
                    );
            continue;
        }

        int i= pos%10;
        int j= pos/10;
        return encontrarNodoMatriz(i,j,tablero);
    }
}

struct nodoMatriz* encontrarNodoMatriz(int i, int j, struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    for(int n=0; n < i; n++){
        actual = actual->derecha;
    }
    for(int m=0; m < j; m++){
        actual = actual->abajo;
    }

    return actual;
}

/*
======================================================================
                    | Dias y consumo |
======================================================================
*/

//baja o sube de forma aleatoria cada bien o servicio
void consumirDiccionario(struct DiccionarioBienes* diccionario){
    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            //baja entre 0 y 6 segun su maximo aaaah
            int tope = bien->maximo / 4;
            if(tope > CONSUMO_MAX) tope = CONSUMO_MAX;
            if(tope < 1) tope = 1;

            bien->cantidad -= randomIntervalo(0, tope);
            if(bien->cantidad < 0) bien->cantidad = 0;

            //a veces la comuna tambien produce
            if(randomIntervalo(1, 100) <= PRODUCCION_PROB){
                bien->cantidad += randomIntervalo(1, tope);
                if(bien->cantidad > bien->maximo) bien->cantidad = bien->maximo;
            }

            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
}

//termina el dia
void pasarDia(struct matrizComunas* tablero){
    imprimirLento(100,
        "\n___________________________________________________\n"
        " ╔═══════════════════════════════════+.\n"
        " ║  * Terminó el día %2d              \n"
        " ╚═══════════════════════════════════+.\n\n",
        tablero->dia
    );

    //consumo de todas las comunas
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* comuna = actual->comuna;

        consumirDiccionario(comuna->diccionarioBienes);
        consumirDiccionario(comuna->diccionarioServicios);
        comuna->intercambiosHoy = 0;

        actual = recorrerMatriz(actual);
    }

    continuarTormentas(tablero);
    escogerEmergencias(tablero);

    //con todo ya cambiado se recalculan los indices
    calcularIndicesTablero(tablero);
    tablero->dia++;

    double satisfaccion, necesidad;
    calcularPromedios(tablero, &satisfaccion, &necesidad);
}

/*
======================================================================
                    | Emergencias |
======================================================================
*/

//decide si hoy hay emergencias, cada 1 a 3 dias, y mas a la vez entre mas grande sea el tablero
void escogerEmergencias(struct matrizComunas* tablero){
    tablero->diasParaEmergencia--;
    if(tablero->diasParaEmergencia > 0)
        return;

    //se sortea de nuevo cuantos dias faltan para la siguiente
    tablero->diasParaEmergencia = randomIntervalo(EMERGENCIA_MIN_DIAS, EMERGENCIA_MAX_DIAS);

    //maximo de emergencias a la vez
    int maximo = 1 + (tablero->tamaño - 3) / 2;
    int cantidad = randomIntervalo(1, maximo);

    printf(" |  !! Hoy ocurren %d emergencia(s) !!  |\n\n", cantidad);

    for(int i = 0; i < cantidad; i++){
        lanzarEmergencia(tablero);
    }
}

//escoge el tipo de emergencia y la comuna donde cae
void lanzarEmergencia(struct matrizComunas* tablero){
    int tipo = randomIntervalo(0, 5);

    //comuna al azar
    int i = randomIntervalo(0, tablero->tamaño - 1);
    int j = randomIntervalo(0, tablero->tamaño - 1);
    struct nodoMatriz* nodo = encontrarNodoMatriz(i, j, tablero);

    if(tipo == 0)       emergenciaVirus(nodo);
    else if(tipo == 1)  emergenciaInundacion(nodo);
    else if(tipo == 2)  emergenciaApagon(nodo);
    else if(tipo == 3)  emergenciaTerremoto(tablero);
    else if(tipo == 4)  emergenciaTsunami(nodo);
    else                emergenciaTormenta(nodo);
}
/*
Sistema del como afecta las emergencias a las comunas, segun nuestra logica y segun el nivel
0 = nada, bien
1 = leve, uy uy 
2 = media, cuidado
3 = fuerte, valieron
cada elemento del diccionario tiene su probabilidad de verse afectado y pierde una cantidad random
*/

void reducirDiccionario(struct DiccionarioBienes* diccionario, int nivel){
    if(nivel <= 0) return;

    int probabilidad = 10 + 20 * nivel;         //30%, 50%, 70% de que cada bien/servicio se vea afectado
    int porcentaje = 15 * nivel - 5;            //pierde hasta el 10%, 25%, 40% de su maximo

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){

            if(randomIntervalo(1, 100) <= probabilidad){
                int tope = bien->maximo * porcentaje / 100;
                if(tope < 1) tope = 1;

                bien->cantidad -= randomIntervalo(1, tope);
                if(bien->cantidad < 0) bien->cantidad = 0;
            }

            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
}

void afectarComuna(struct comuna* comuna, int nivelBienes, int nivelServicios){
    reducirDiccionario(comuna->diccionarioBienes, nivelBienes);
    reducirDiccionario(comuna->diccionarioServicios, nivelServicios);
}

//afecta a un vecino, si existe
void afectarVecino(struct nodoMatriz* vecino, int nivelBienes, int nivelServicios){
    if(vecino == NULL) return;

    afectarComuna(vecino->comuna, nivelBienes, nivelServicios);
}

//vecinos = las mismas 8 comunas que rodean que usa sumarVecinos
void afectarVecinos(struct nodoMatriz* nodo, int nivelBienes, int nivelServicios){
    afectarVecino(nodo->arriba, nivelBienes, nivelServicios);
    afectarVecino(nodo->abajo, nivelBienes, nivelServicios);
    afectarVecino(nodo->izquierda, nivelBienes, nivelServicios);
    afectarVecino(nodo->derecha, nivelBienes, nivelServicios);

    //las diagonales se alcanzan dando dos pasos
    afectarVecino(nodo->arriba ? nodo->arriba->izquierda : NULL, nivelBienes, nivelServicios);
    afectarVecino(nodo->arriba ? nodo->arriba->derecha : NULL, nivelBienes, nivelServicios);
    afectarVecino(nodo->abajo ? nodo->abajo->izquierda : NULL, nivelBienes, nivelServicios);
    afectarVecino(nodo->abajo ? nodo->abajo->derecha : NULL, nivelBienes, nivelServicios);
}

void imprimirAvisoEmergencia(char* tipo, struct comuna* comuna, char* descripcion){
    imprimirLento(100,
        " ╔══════════════════════════════════════════════===+- -- -\n"
        " ║ !! Emergencia : %s !!\n", tipo);

    if(comuna != NULL)
        imprimirLento(100, " ║ Comuna afectada : %02d \n", comuna->numero);

    imprimirLento(100,
        " ║ %s\n"
        " ╚═════════════════════════════════════════════===+- -\n\n", descripcion);
}

//virus: pega fuerte en los servicios de su comuna y un poco en las adyacentes
void emergenciaVirus(struct nodoMatriz* nodo){
    imprimirAvisoEmergencia("VIRUS", nodo->comuna, "Se contagia la comuna y afecta poco a las adyacentes");

    afectarComuna(nodo->comuna, 2, 3);
    afectarVecinos(nodo, 1, 1);
}

//inundacion: solo su comuna, se pierden sobre todo bienes
void emergenciaInundacion(struct nodoMatriz* nodo){
    imprimirAvisoEmergencia("INUNDACION", nodo->comuna, "Se inunda solo esta comuna");

    afectarComuna(nodo->comuna, 3, 2);
}

//apagon: solo su comuna
void emergenciaApagon(struct nodoMatriz* nodo){
    imprimirAvisoEmergencia("APAGON", nodo->comuna, "Sin electricidad en esta comuna");

    afectarComuna(nodo->comuna, 0, 3);
}

//terremoto: todas las comunas del tablero
void emergenciaTerremoto(struct matrizComunas* tablero){
    imprimirAvisoEmergencia("TERREMOTO", NULL, "Afecta todas las comunas del tablero");

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        afectarComuna(actual->comuna, 1, 2);
        actual = recorrerMatriz(actual);
    }
}

//tsunami: una linea completa, horizontal o vertical
void emergenciaTsunami(struct nodoMatriz* nodo){
    int horizontal = randomIntervalo(0, 1);
    struct nodoMatriz* actual = nodo;

    //me voy hasta el inicio de la linea
    if(horizontal){
        while(actual->izquierda != NULL)
            actual = actual->izquierda;
    }else{
        while(actual->arriba != NULL)
            actual = actual->arriba;
    }

    char descripcion[100];
    if(horizontal)
        snprintf(descripcion, sizeof(descripcion), "Afecta la fila de la comuna %02d", actual->comuna->numero);
    else
        snprintf(descripcion, sizeof(descripcion), "Afecta la columna de la comuna %02d", actual->comuna->numero);
    imprimirAvisoEmergencia("TSUNAMI", NULL, descripcion);

    //recorro toda la linea
    while(actual != NULL){
        afectarComuna(actual->comuna, 3, 2);

        if(horizontal)
            actual = actual->derecha;
        else
            actual = actual->abajo;
    }
}

//tormenta: afecta poco a la comuna y a las adyacentes, pero dura de 1 a 3 dias
void emergenciaTormenta(struct nodoMatriz* nodo){
    int duracion = randomIntervalo(1, TORMENTA_MAX_DIAS);

    char descripcion[100];
    snprintf(descripcion, sizeof(descripcion), "Afecta a una comuna y sus adyacentes");
    imprimirAvisoEmergencia("TORMENTA", nodo->comuna, descripcion);

    //el primer dia ya se aplica hoy
    afectarComuna(nodo->comuna, 1, 1);
    afectarVecinos(nodo, 1, 1);

    //y se guardan los dias que le quedan (si ya habia una tormenta ahi, se queda la mas larga)
    if(duracion - 1 > nodo->comuna->diasTormenta)
        nodo->comuna->diasTormenta = duracion - 1;
}

//aplica el dia de las tormentas que siguen activas
void continuarTormentas(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* comuna = actual->comuna;

        if(comuna->diasTormenta > 0){
            afectarComuna(comuna, 1, 1);
            afectarVecinos(actual, 1, 1);
            comuna->diasTormenta--;

            if(comuna->diasTormenta > 0)
                printf(" |  La tormenta sobre la comuna %02d sigue | le quedan %d dia(s))  |\n\n", comuna->numero, comuna->diasTormenta);
            else
                printf(" |  La tormenta sobre la comuna %02d termina hoy  |\n\n", comuna->numero);
        }

        actual = recorrerMatriz(actual);
    }
}

/*
======================================================================
                    |Intercambios |
======================================================================
*/

//busca un bien/servicio por su nombre
struct nodoBienes* buscarBienExacto(struct DiccionarioBienes* diccionario, char* nombre){
    int pos = hashDiccionarioBienes(nombre, diccionario->tamaño);

    //recorro el diccionario hasta encontrar la posicion
    struct nodoDic* actual = diccionario->inicio;
    for(int i=0; i<pos; i++){
        actual = actual->sigt;
    }

    //dentro de la casilla busco el bien
    struct nodoBienes* bien = actual->bienes;
    while(bien != NULL){
        if(strcmp(bien->nombre, nombre) == 0)
            return bien;

        bien = bien->sigt;
    }

    return NULL; //si no lo encuentra
}

int contarElementos(struct DiccionarioBienes* diccionario){
    int total = 0;

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            total++;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
    return total;
}

//devuelve la 'posicion'
struct nodoBienes* obtenerElemento(struct DiccionarioBienes* diccionario, int posicion){
    int contador = 0;

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            contador++;
            if(contador == posicion)
                return bien;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
    return NULL; //si no existe esa posicion
}

void imprimirDiccionarioNumerado(struct DiccionarioBienes* diccionario){
    int numero = 1;

    printf("\n");

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            printf(" %2d. %s %d , max:%d\n", numero, bien->nombre, bien->cantidad, bien->maximo);
            numero++;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
    printf("\n");
}

void imprimirAdvertencia(char* mensaje){
    imprimirLento(70,
        "\n______________________________________\n"
        "                                      \n"
        " !! %s !!\n"
        "______________________________________\n\n",
        mensaje
    );
}

//pregunta si quiere un bien o un servicio y cual de la lista, retorna NULL si cancela
struct nodoBienes* escogerProducto(struct comuna* comuna, int* esServicio){
    printf("\n | Comuna %02d %s |\n\n", comuna->numero, comuna->nombre);

    imprimirLento(70,
        " ╔════════════════════════════════╗\n"
        " ║  * Qué quieres intercambiar    ║\n"
        " ╚════════════════════════════════╝\n"
        " | 0. Un bien                     |\n"
        " | 1. Un servicio                 |\n"
    );

    int tipo = pedirEnIntervalo(0, 1);
    if(tipo == 2)
        return NULL;

    *esServicio = tipo;
    struct DiccionarioBienes* diccionario = (tipo == 0) ? comuna->diccionarioBienes : comuna->diccionarioServicios;
    int total = contarElementos(diccionario);

    while(1){
        if(tipo == 0)
            imprimirLento(70,
                "\n___________________________________________________\n"
                " ╔══════════════╗\n"
                " ║  * Bienes:   ║\n"
                );
        else
            imprimirLento(70,
                "\n___________________________________________________\n"
                " ╔═════════════════╗\n"
                " ║  * Servicios:   ║\n"
                );
        imprimirDiccionarioNumerado(diccionario);
        printf(" 0. Cancelar\n\n");

        int posicion = pedirEnIntervalo(0, total);
        if(posicion == 0)
            return NULL;

        struct nodoBienes* escogido = obtenerElemento(diccionario, posicion);
        if(escogido->cantidad <= 0){
            imprimirAdvertencia("No hay existencias de eso | Escoge otro");
            continue;
        }

        return escogido;
    }
}

//INtercambio entre dos comunas
void realizarIntercambio(struct matrizComunas* tablero){
    char mensaje[256];

    //---------------- primera comuna ----------------
    imprimirLento(70,
        "\n___________________________________________________\n"
        " ╔══════════════════════════════════════════╗\n"
        " ║ * De cual comuna intercambiar (numero)   ║\n"
        " ╚══════════════════════════════════════════╝\n\n"
    );

    imprimirTablero(tablero);

    struct nodoMatriz* nodoA = buscarComuna(tablero);
    struct comuna* comunaA = nodoA->comuna;

    if(comunaA->intercambiosHoy >= INTERCAMBIOS_MAX_DIA){
        snprintf(mensaje, sizeof(mensaje), "La comuna %02d ya hizo sus %d intercambios de hoy", comunaA->numero, INTERCAMBIOS_MAX_DIA);
        imprimirAdvertencia(mensaje);
        return;
    }

    //lo que entrega la primera
    int servicioA;
    struct nodoBienes* productoA = escogerProducto(comunaA, &servicioA);
    if(productoA == NULL){
        imprimirAdvertencia("Intercambio cancelado");
        return;
    }

    imprimirLento(70,
        "\n ╔═══════════════════════════════╗\n"
        " ║ * Cuántas unidades entrega   ║\n"
        " ╚═══════════════════════════════╝\n"
        " |  Mínimo = 1   |  Máximo = %-3d |\n\n",
        productoA->cantidad
    );
    int cantidadA = pedirEnIntervalo(1, productoA->cantidad);

    //---------------- segunda comuna ----------------
    struct nodoMatriz* nodoB;

    while(1){
        imprimirLento(70,
            "\n___________________________________________________\n"
            " ╔═════════════════════════════════════════╗\n"
            " ║ * A cual comuna intercambiar (numero)   ║\n"
            " ╚═════════════════════════════════════════╝\n\n"
        );

        imprimirTablero(tablero);
        
        nodoB = buscarComuna(tablero);

        if(nodoB != nodoA)
            break;

        imprimirAdvertencia("| Tiene que ser OTRA comuna |");
    }
    struct comuna* comunaB = nodoB->comuna;

    if(comunaB->intercambiosHoy >= INTERCAMBIOS_MAX_DIA){
        snprintf(mensaje, sizeof(mensaje), "La comuna %02d %s ya hizo sus %d intercambios de hoy", comunaB->numero, comunaB->nombre, INTERCAMBIOS_MAX_DIA);
        imprimirAdvertencia(mensaje);
        return;
    }

    //lo que entrega la segunda a cambio
    int servicioB;
    struct nodoBienes* productoB;
    while(1){
        productoB = escogerProducto(comunaB, &servicioB);
        if(productoB == NULL){
            imprimirAdvertencia("Intercambio cancelado");
            return;
        }

        //no tiene sentido cambiar un producto por el mismo lol xd
        if(servicioA == servicioB && strcmp(productoA->nombre, productoB->nombre) == 0){
            imprimirAdvertencia("| No se puede cambiar un producto por el mismo | Escoge otro |");
            continue;
        }
        break;
    }

    imprimirLento(70,
        "\n ╔═══════════════════════════════╗\n"
        " ║ * Cuántas unidades recibe   ║\n"
        " ╚═══════════════════════════════╝\n"
        " |  Mínimo = 1   |  Máximo = %-3d |\n\n",
        productoB->cantidad
    );
    int cantidadB = pedirEnIntervalo(1, productoB->cantidad);

    //busco los productos en la comuna que los recibe
    struct nodoBienes* productoAenB = buscarBienExacto(servicioA ? comunaB->diccionarioServicios : comunaB->diccionarioBienes, productoA->nombre);
    struct nodoBienes* productoBenA = buscarBienExacto(servicioB ? comunaA->diccionarioServicios : comunaA->diccionarioBienes, productoB->nombre);
    if(productoAenB == NULL || productoBenA == NULL){
        imprimirAdvertencia("| No se encontró el producto en la otra comuna |");
        return;
    }

    //solo se puede entregar lo que le cabe a la comuna que recibe
    int realA = cantidadA;
    int espacio = productoAenB->maximo - productoAenB->cantidad;
    if(espacio < 0) espacio = 0;
    if(realA > espacio){
        printf("\n | La comuna %02d solo tiene espacio para %d de %s |\n", comunaB->numero, espacio, productoA->nombre);
        realA = espacio;
    }

    int realB = cantidadB;
    espacio = productoBenA->maximo - productoBenA->cantidad;
    if(espacio < 0) espacio = 0;
    if(realB > espacio){
        printf("\n | La comuna %02d solo tiene espacio para %d de %s |\n", comunaA->numero, espacio, productoB->nombre);
        realB = espacio;
    }

    if(realA == 0 && realB == 0){
        imprimirAdvertencia("| Ninguna de las dos comunas tiene espacio para recibir | Intercambio cancelado |");
        return;
    }

    //lo que regresa a quien da, siempre menos de lo que entrego, para que no se vacien jsjs
    int retornoA = realA * RETORNO_REGALO / 100;
    int retornoB = realB * RETORNO_REGALO / 100;

    productoA->cantidad    += retornoA - realA;         //la que da se queda con menos, pero le regresa una parte
    productoAenB->cantidad += realA;                    //la que recibe se queda con todo
    productoB->cantidad    += retornoB - realB;
    productoBenA->cantidad += realB;

    comunaA->intercambiosHoy++;
    comunaB->intercambiosHoy++;

    calcularIndicesTablero(tablero);

    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ !! Intercambio realizado !!\n");
    printf(" ║ De la comuna %02d a la comuna %02d : %d de %s\n", comunaA->numero, comunaB->numero, realA, productoA->nombre);
    printf(" ║ De la comuna %02d a la comuna %02d : %d de %s\n", comunaB->numero, comunaA->numero, realB, productoB->nombre);
    printf(" ║\n");
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
}

/*
======================================================================
                    | CAMBIO: Final de la partida |
======================================================================
*/

//promedio de satisfaccion y necesidad de todas las comunas
void calcularPromedios(struct matrizComunas* tablero, double* satisfaccion, double* necesidad){
    int total = tablero->tamaño * tablero->tamaño;
    double sumaSatisfaccion = 0, sumaNecesidad = 0;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        sumaSatisfaccion += actual->comuna->satisfaccion;
        sumaNecesidad += actual->comuna->necesidad;
        actual = recorrerMatriz(actual);
    }

    *satisfaccion = sumaSatisfaccion / total;
    *necesidad = sumaNecesidad / total;
}

//como quedo la sociedad segun su satisfaccion promedio
const char* clasificarSociedad(double satisfaccion){
    if(satisfaccion < 30)       return "DEFICIENTE";
    else if(satisfaccion < 50)  return "MALA";
    else if(satisfaccion < 60)  return "NORMAL";
    else if(satisfaccion < 70)  return "BIEN";
    else if(satisfaccion < 90)  return "EXCELENTE";
    else                        return "¡¡ SOCIEDAD PERFECTA !!";
}

void mostrarFinPartida(struct matrizComunas* tablero){
    double satisfaccion, necesidad;
    calcularPromedios(tablero, &satisfaccion, &necesidad);

    imprimirTablero(tablero);

    imprimirLento(150,
        "\n___________________________________________________\n"
        " ╔════════════════════════════════════╗\n"
        " ║  !!    Se termino el juego   !!    ║\n"
        " ╚════════════════════════════════════╝\n\n"
    );

    imprimirLento(100,
        " |  Días jugados            : %d\n"
        " |  Satisfacción promedio   : %5.1f\n"
        " |  Necesidad promedio      : %5.1f\n\n",
        tablero->diasTotales, satisfaccion, necesidad
    );

    imprimirLento(150,
        " ╔══════════════════════════════════════════════===+- -- -\n"
        " ║ Resultado de la sociedad: %s\n"
        " ╚═════════════════════════════════════════════===+- -\n\n",
        clasificarSociedad(satisfaccion)
    );
}

/*
======================================================================
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    struct matrizComunas* tablero = crearTablero();

    struct nodoMatriz* actual = tablero->inicio;

    while(tablero->dia <= tablero->diasTotales){
        imprimirTablero(tablero);

        int diaAntes = tablero->dia;
        actual = pedirComuna(actual, tablero);

        if(tablero->dia != diaAntes)
            continue;

        imprimirComuna(actual, tablero);
    }

    mostrarFinPartida(tablero);

    return 0;
}