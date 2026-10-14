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

#define NECESIDAD_MIN 5.0       //siempre hay necesidades extra: nunca llega a 0
#define NECESIDAD_MAX 99.0      //nunca llega a 100

#define PESO_PROPIA   0.55      //que tan bien estoy yo
#define PESO_VECINOS  0.25      //que tan bien estan los de alrededor
#define PESO_EQUIDAD  0.20      //que tan parecido es mi nivel al de mis vecinos

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

//necesidad = % de bienes y servicios que faltan para llegar al maximo (entre 5 y 99)
double calcularNecesidad(struct comuna* comuna){
    int faltante = 0, maximo = 0;
    sumarFaltantes(comuna->diccionarioBienes, &faltante, &maximo);
    sumarFaltantes(comuna->diccionarioServicios, &faltante, &maximo);

    double necesidad = (maximo == 0) ? NECESIDAD_MIN : 100.0 * faltante / maximo;

    if(necesidad < NECESIDAD_MIN) necesidad = NECESIDAD_MIN;
    if(necesidad > NECESIDAD_MAX) necesidad = NECESIDAD_MAX;
    return necesidad;
}

//suma el "bienestar" (100 - necesidad) de un vecino, si existe
void sumarVecino(struct nodoMatriz* vecino, double* suma, int* cuenta){
    if(vecino == NULL) return;

    *suma += 100.0 - calcularNecesidad(vecino->comuna);
    (*cuenta)++;
}

//vecinos = las (hasta) 8 comunas que rodean: esquina=3, borde=5, centro=8
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
    double propia = 100.0 - necesidad;                  //bienestar propio (0-100)

    double suma = 0;
    int cuenta = 0;
    sumarVecinos(nodo, &suma, &cuenta);
    double vecinos = (cuenta > 0) ? suma / cuenta : propia;     //bienestar promedio de los vecinos

    double diferencia = propia - vecinos;
    if(diferencia < 0) diferencia = -diferencia;
    double equidad = 100.0 - diferencia;                //100 = igual que sus vecinos

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

void imprimirComuna(struct nodoMatriz* nodo){

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

        int opcion = pedirEnIntervalo(0,3);

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
        }
        else
            return;
    }
    printf("\n___________________________________________________\n");
}

struct nodoMatriz* pedirComuna(struct nodoMatriz* actual, struct matrizComunas* tablero){
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Comuna actual: %d    | Día actual: \n", actual->comuna->numero);
    printf(" ║\n");
    printf(" ║ Tus opciones son: \n");
    printf(" ║ 0. Escribir el numero de la comuna\n");
    printf(" ║ 1. Moverte al adyacente | arriba | abajo | izq | der |\n");
    printf(" ║ 3. Terminar día\n");
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
    int opcion = pedirEnIntervalo(0,1);

    if(opcion == 0){
        return buscarComuna(tablero);
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
        " ║ * Dime el numero de la comuna que quieres ║\n"
        " ╚═══════════════════════════════════════════╝\n\n"
    );

    int tamaño = tablero->tamaño;
    int pos = 0;
    while(1){
        pos = pedirInt();

        if(pos%10 >= tamaño || pos/10 >= tamaño){
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
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    struct matrizComunas* tablero = crearTablero();

    struct nodoMatriz* actual = tablero->inicio;

    while(1){
        imprimirTablero(tablero);

        actual = pedirComuna(actual, tablero);

        imprimirComuna(actual);
    }


    //calcularIndicesTablero(tablero);
    return 0;
}



