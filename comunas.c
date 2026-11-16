#include "comunas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>         //para leer la carpeta de libros
#include <sys/stat.h>       //para saber si algo es carpeta y para crearla

/*
    Progra 02  -  Estructuras de Datos
    Made by: Sebastián Aguilar y Felipe Álvarez
    Espero le gusto muhco profe aurelio
    Ahora con bibliotecas :D
*/



/*
======================================================================
            | Constantes del juego   |
======================================================================
*/

//colores (son codigos de la terminal, no ocupan libreria)
#define AZUL                    "\033[1;34m"
#define SIN_COLOR               "\033[0m"

//dias y consumo
#define CONSUMO_MAX             30      //lo maximo que baja un bien en un dia
#define PRODUCCION_PROB         35      //probabilidad de que un bien suba en el dia

//emergencias
#define EMERGENCIA_MIN_DIAS     1       //cada cuantos dias ocurre una emergencia, minimo
#define EMERGENCIA_MAX_DIAS     3       //maximo
#define TORMENTA_MAX_DIAS       3       //duracion de tormenta
#define DESASTRE_MAX            12      //lo maximo que un desastre le suma a la necesidad

//intercambios
#define RETORNO_REGALO          30      //porcentaje de regreso al intercambiar
#define INTERCAMBIOS_MAX_DIA    3       //cuantos intercambios puede hacer una comuna por dia
#define VUELTAS_AUTONOMAS_MIN   2       //vueltas de intercambios de las comunas autonomas por dia, minimo
#define VUELTAS_AUTONOMAS_MAX   3       //maximo
#define INTENTOS_AUTONOMOS      8       //con cuantas comunas prueba una autonoma antes de rendirse
#define PROB_SOLICITUD          20      //probabilidad de que le pidan un intercambio a una comuna del jugador

//disolucion de comunas (se tienen que cumplir las dos)
#define NECESIDAD_DISOLUCION    75.0
#define SATISFACCION_DISOLUCION 30.0

//jugador
#define COMUNAS_JUGADOR         3

//biblioteca
#define CARPETA_LIBROS          "libros"
#define LIBROS_PARA_APRENDER    3
#define PROB_CACHE              60      //probabilidad de escoger un libro que ya esta en la cache
#define NIVELES_ARBOL           4       //niveles del arbol que se imprimen
#define ANCHO_ARBOL             88

//criminalidad
#define CRIMINALES_MIN          2       //personas marcadas por comuna
#define CRIMINALES_MAX          5
#define PROB_CRIMEN             25      //probabilidad de que una persona marcada haga algo en el dia
#define CRIMINALIDAD_EXPULSION  55

//logros
#define SATISFACCION_LOGRO      90.0
#define NECESIDAD_AL_FILO       65.0    //para el logro de ganar con las comunas al minimo
#define SATISFACCION_AL_FILO    40.0

//guardado
#define ARCHIVO_GUARDADO        "partida_guardada.txt"

//para que el arbol chaflaneado avise cada rotacion que hace
int verRotaciones = 0;

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

void agregarAlFinalSimple(struct nodoListaSimple* nodo, struct listaSimple* lista){
    nodo->sigt = NULL;
    lista->tamaño++;

    if(lista->inicio == NULL){
        lista->inicio = nodo;
        return;
    }

    struct nodoListaSimple* actual = lista->inicio;
    while(actual->sigt != NULL){
        actual = actual->sigt;
    }
    actual->sigt = nodo;
}

int estaEnLista(struct listaSimple* lista, char* nombre){
    struct nodoListaSimple* actual = lista->inicio;
    while(actual != NULL){
        if(strcmp(actual->nombre, nombre) == 0)
            return 1;
        actual = actual->sigt;
    }
    return 0;
}

char* nombreEnPosicion(struct listaSimple* lista, int posicion){
    struct nodoListaSimple* actual = lista->inicio;
    for(int i = 0; i < posicion && actual != NULL; i++){
        actual = actual->sigt;
    }
    if(actual == NULL)
        return NULL;
    return actual->nombre;
}

/*
=================================================
                | Personas |
=================================================
*/

struct listaPersonas* crearListaPersonas(){
    struct listaPersonas* nuevaLista = calloc(1, sizeof(struct listaPersonas));
    return nuevaLista;
}

//el calloc ya deja en 0 los libros leidos y la criminalidad
struct persona* crearPersona(char* nombre){
    struct persona* nueva = calloc(1, sizeof(struct persona));
    nueva->nombre = nombre;
    nueva->servicios = crearListaSimple();
    return nueva;
}

void agregarPersona(struct persona* persona, struct listaPersonas* lista){
    persona->sigt = lista->inicio;
    lista->inicio = persona;
    lista->tamaño++;
}

void sacarPersona(struct persona* persona, struct listaPersonas* lista){
    struct persona* anterior = NULL;
    struct persona* actual = lista->inicio;

    //busco la persona y la que esta antes
    while(actual != NULL && actual != persona){
        anterior = actual;
        actual = actual->sigt;
    }
    if(actual == NULL)
        return;

    if(anterior == NULL)
        lista->inicio = actual->sigt;
    else
        anterior->sigt = actual->sigt;

    actual->sigt = NULL;
    lista->tamaño--;
}

struct persona* obtenerPersona(struct listaPersonas* lista, int posicion){
    struct persona* actual = lista->inicio;
    for(int i = 1; i < posicion && actual != NULL; i++){
        actual = actual->sigt;
    }
    return actual;
}

void darServicio(struct persona* persona, char* servicio){
    agregarNodoSimple(crearNodoListaSimple(strdup(servicio)), persona->servicios);
}

//la mayoria de personas sabe 1 servicio o ninguno, pocas saben 2 o 3
void repartirServicios(struct matrizComunas* tablero){
    struct listaSimple* disponibles = tablero->serviciosDisponibles;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){

        struct persona* persona = actual->comuna->personas->inicio;
        while(persona != NULL){
            int sorteo = randomIntervalo(1, 100);
            int cantidad;

            if(sorteo <= 40)        cantidad = 0;
            else if(sorteo <= 85)   cantidad = 1;
            else if(sorteo <= 97)   cantidad = 2;
            else                    cantidad = 3;

            for(int k = 0; k < cantidad; k++){
                char* servicio = nombreEnPosicion(disponibles, randomIntervalo(0, disponibles->tamaño - 1));

                //si le sale uno repetido se queda sin ese
                if(!estaEnLista(persona->servicios, servicio))
                    darServicio(persona, servicio);
            }
            persona = persona->sigt;
        }

        //ya con la gente lista se cuenta cuantos saben cada oficio
        actualizarServicios(actual->comuna);
        actual = recorrerMatriz(actual);
    }
}

//cuenta cuantas personas de la comuna saben cada servicio
//cantidad = personas que lo saben | maximo = personas que la comuna ocupa con ese oficio
void actualizarServicios(struct comuna* comuna){
    struct DiccionarioBienes* diccionario = comuna->diccionarioServicios;

    //se ocupa la gente como si se repartiera parejo entre todos los oficios, y minimo una persona
    int necesarias = comuna->personas->tamaño / diccionario->tamaño;
    if(necesarias < 1) necesarias = 1;

    //todo empieza en cero
    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* servicio = casilla->bienes;
        while(servicio != NULL){
            servicio->cantidad = 0;
            servicio->maximo = necesarias;
            servicio = servicio->sigt;
        }
        casilla = casilla->sigt;
    }

    //y cada persona suma 1 en lo que sabe hacer
    struct persona* persona = comuna->personas->inicio;
    while(persona != NULL){
        struct nodoListaSimple* sabe = persona->servicios->inicio;
        while(sabe != NULL){
            struct nodoBienes* enComuna = buscarBienExacto(diccionario, sabe->nombre);
            if(enComuna != NULL)
                enComuna->cantidad++;
            sabe = sabe->sigt;
        }
        persona = persona->sigt;
    }
}

//imprime cada servicio con las personas que lo saben hacer
void imprimirServicios(struct DiccionarioBienes* diccionario, int numerado){
    int numero = 1;

    printf("\n");

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* servicio = casilla->bienes;
        while(servicio != NULL){
            if(numerado)
                printf(" %2d. ", numero);
            else
                printf(" - ");

            printf("%s: %d %s\n", servicio->nombre, servicio->cantidad, servicio->cantidad == 1 ? "persona" : "personas");
            numero++;
            servicio = servicio->sigt;
        }
        casilla = casilla->sigt;
    }
    printf("\n");
}

//saca de la comuna hasta 'cantidad' personas que sepan ese servicio y las retorna en una lista aparte
struct listaPersonas* sacarPersonasConServicio(struct comuna* comuna, char* servicio, int cantidad){
    struct listaPersonas* salen = crearListaPersonas();

    struct persona* persona = comuna->personas->inicio;
    while(persona != NULL && salen->tamaño < cantidad){
        struct persona* siguiente = persona->sigt;      //lo guardo porque la persona se va a desenlazar

        if(estaEnLista(persona->servicios, servicio)){
            sacarPersona(persona, comuna->personas);
            agregarPersona(persona, salen);
        }
        persona = siguiente;
    }
    return salen;
}

//mete todas las personas de la lista en la comuna destino
void pasarPersonas(struct listaPersonas* personas, struct comuna* destino){
    while(personas->inicio != NULL){
        struct persona* persona = personas->inicio;
        sacarPersona(persona, personas);
        agregarPersona(persona, destino->personas);
    }
    free(personas);
}

//solo estas personas van a poder cometer crimenes en toda la partida
void marcarCriminales(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct listaPersonas* personas = actual->comuna->personas;

        int cantidad = randomIntervalo(CRIMINALES_MIN, CRIMINALES_MAX);
        if(cantidad > personas->tamaño) cantidad = personas->tamaño;

        int marcadas = 0;
        while(marcadas < cantidad){
            struct persona* persona = obtenerPersona(personas, randomIntervalo(1, personas->tamaño));
            if(persona->criminal)
                continue;       //ya estaba marcada, saco otra

            persona->criminal = 1;
            marcadas++;
        }
        actual = recorrerMatriz(actual);
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
        if(!c->disuelta)
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

//el espacio que deja una comuna disuelta: un cuadro del mismo tamaño pero vacio
void imprimirDisuelta(int linea){
    if(linea == 0)
        printf(" ┌╌╌╌╌╌╌┐ ");
    else if(linea == 3)
        printf(" └╌╌╌╌╌╌┘ ");
    else
        printf(" ╎      ╎ ");
}

void imprimirLineaFila(struct nodoMatriz* inicioFila, int linea){
    struct nodoMatriz* actual = inicioFila;

    while(actual != NULL){              //avanzo a la derecha hasta que se acabe la fila
        struct comuna* comuna = actual->comuna;

        if(comuna->disuelta){
            imprimirDisuelta(linea);
            actual = actual->derecha;
            continue;
        }

        //las comunas del jugador van en azul
        if(comuna->delJugador)
            printf(AZUL);

        if(linea == 0)
            imprimirNumero(comuna);
        else if(linea == 1)
            imprimirSatisfaccion(comuna);
        else if(linea == 2)
            imprimirNecesidad(comuna);
        else
            imprimirRelleno();

        if(comuna->delJugador)
            printf(SIN_COLOR);

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
    imprimirLento(150,
        " ╔════════════════════════════════════╗\n"
        " ║¡ES HORA DE PERSONALIZAR TU PARTIDA!║\n"
        " ╚════════════════════════════════════╝\n"
        "                                        \n\n"
    );

    //creo el tablero
    int tamañoTablero = preguntarDimensión();
    struct matrizComunas* tablero = crearMatriz(tamañoTablero);
    tablero->historial = crearListaSimple();

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

    //ahora los servicios los saben las personas
    repartirServicios(tablero);
    marcarCriminales(tablero);

    //las 3 comunas que le tocan al jugador
    asignarComunasJugador(tablero);

    //el catalogo de libros para las bibliotecas
    prepararBiblioteca(tablero);

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
        " |   Mínimo = 15   |    Máximo = 99   |\n"
        "                                       \n\n"
    );

    int dias = pedirEnIntervalo(15, 99);
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
        comunaActual->personas = crearListaPersonas();

        for(int k = 0; k < cantidad && personasDisponibles->inicio != NULL; k++){
            struct nodoListaSimple* nodo = personasDisponibles->inicio;

            //la saco de la lista general
            personasDisponibles->inicio = nodo->sigt;
            personasDisponibles->tamaño--;

            //y la meto en la comuna, ya como persona
            agregarPersona(crearPersona(nodo->nombre), comunaActual->personas);
            free(nodo);
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

//pesos de la necesidad (suman 1): los bienes pesan mas que los servicios
#define PESO_BIENES         0.80
#define PESO_SERVICIOS      0.20

//pesos de la satisfaccion (suman 1)
#define PESO_LOCAL          0.35    //que tan bien estoy yo (necesidad local)
#define PESO_NEC_VECINOS    0.10    //necesidad de las comunas de alrededor
#define PESO_SAT_VECINOS    0.10    //satisfaccion de las comunas de alrededor
#define PESO_RECIPROCIDAD   0.25    //balance entre lo que doy y lo que recibo
#define PESO_REGALOS        0.20    //regalos recibidos hace poco

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

//necesidad = lo que falta de bienes (pesa mas) y lo que falta de gente que sepa cada servicio
//solo depende de lo que tiene la comuna, y si hubo un desastre hace poco sube mas
double calcularNecesidad(struct comuna* comuna){
    actualizarServicios(comuna);

    //los bienes se cuentan en unidades
    int faltanteBienes = 0, maximoBienes = 0;
    sumarFaltantes(comuna->diccionarioBienes, &faltanteBienes, &maximoBienes);
    double necesidadBienes = (maximoBienes == 0) ? NECESIDAD_MIN : 100.0 * faltanteBienes / maximoBienes;

    //y los servicios en personas, por eso se sacan aparte y no se suman con los bienes
    int faltanteServicios = 0, maximoServicios = 0;
    sumarFaltantes(comuna->diccionarioServicios, &faltanteServicios, &maximoServicios);
    int cubiertos = maximoServicios - faltanteServicios;

    //una comuna con poca gente no puede ocupar mas puestos que las personas que tiene
    int puestos = maximoServicios;
    if(puestos > comuna->personas->tamaño) puestos = comuna->personas->tamaño;

    double necesidadServicios = NECESIDAD_MIN;
    if(puestos > 0 && cubiertos < puestos)
        necesidadServicios = 100.0 * (puestos - cubiertos) / puestos;

    double necesidad = PESO_BIENES * necesidadBienes + PESO_SERVICIOS * necesidadServicios;

    necesidad += comuna->desastre;

    if(necesidad < NECESIDAD_MIN) necesidad = NECESIDAD_MIN;
    if(necesidad > NECESIDAD_MAX) necesidad = NECESIDAD_MAX;
    return necesidad;
}

//reciprocidad: 100 si ha dado lo mismo que ha recibido, baja entre mas desbalanceado este
double calcularReciprocidad(struct comuna* comuna){
    int total = comuna->regalosDados + comuna->regalosRecibidos;

    //si nunca ha intercambiado se queda en el medio
    if(total == 0)
        return 50.0;

    int diferencia = comuna->regalosDados - comuna->regalosRecibidos;
    if(diferencia < 0) diferencia = -diferencia;

    return 100.0 - 100.0 * diferencia / total;
}

//regalos recibidos ultimamente comparado con la gente que hay
double calcularNivelRegalos(struct comuna* comuna){
    if(comuna->personas->tamaño == 0)
        return 0;

    //con un regalo por cada 3 personas ya se llega al 100
    double nivel = 100.0 * comuna->regalosRecientes * 3 / comuna->personas->tamaño;
    if(nivel > 100.0) nivel = 100.0;
    return nivel;
}

//suma la necesidad y la satisfaccion de un vecino, si existe y sigue vivo
void sumarVecino(struct nodoMatriz* vecino, double* sumaNecesidad, double* sumaSatisfaccion, int* cuenta){
    if(vecino == NULL) return;
    if(vecino->comuna->disuelta) return;

    *sumaNecesidad += vecino->comuna->necesidad;
    *sumaSatisfaccion += vecino->comuna->satisfaccionAnterior;
    (*cuenta)++;
}

//vecinos = las 8 comunas que rodean: esquina=3, borde=5, centro=8
void sumarVecinos(struct nodoMatriz* nodo, double* sumaNecesidad, double* sumaSatisfaccion, int* cuenta){
    sumarVecino(nodo->arriba, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->abajo, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->izquierda, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->derecha, sumaNecesidad, sumaSatisfaccion, cuenta);

    //las diagonales se alcanzan dando dos pasos
    sumarVecino(nodo->arriba ? nodo->arriba->izquierda : NULL, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->arriba ? nodo->arriba->derecha : NULL, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->abajo ? nodo->abajo->izquierda : NULL, sumaNecesidad, sumaSatisfaccion, cuenta);
    sumarVecino(nodo->abajo ? nodo->abajo->derecha : NULL, sumaNecesidad, sumaSatisfaccion, cuenta);
}

//calcula la satisfaccion de la comuna de ese nodo (la necesidad ya tiene que estar calculada)
void calcularIndices(struct nodoMatriz* nodo){
    struct comuna* comuna = nodo->comuna;

    double sumaNecesidad = 0, sumaSatisfaccion = 0;
    int cuenta = 0;
    sumarVecinos(nodo, &sumaNecesidad, &sumaSatisfaccion, &cuenta);

    //si se quedo sin vecinos se compara consigo misma
    double necesidadVecinos = (cuenta > 0) ? sumaNecesidad / cuenta : comuna->necesidad;
    double satisfaccionVecinos = (cuenta > 0) ? sumaSatisfaccion / cuenta : comuna->satisfaccionAnterior;

    comuna->reciprocidad = calcularReciprocidad(comuna);

    comuna->satisfaccion = PESO_LOCAL * (100.0 - comuna->necesidad)
                         + PESO_NEC_VECINOS * (100.0 - necesidadVecinos)
                         + PESO_SAT_VECINOS * satisfaccionVecinos
                         + PESO_RECIPROCIDAD * comuna->reciprocidad
                         + PESO_REGALOS * calcularNivelRegalos(comuna);
}

//recalcula los indices de todas las comunas del tablero
void calcularIndicesTablero(struct matrizComunas* tablero){
    int total = tablero->tamaño * tablero->tamaño;

    //primero la necesidad de todas, porque la satisfaccion ocupa la de los vecinos
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        struct comuna* comuna = actual->comuna;

        if(!comuna->disuelta){
            comuna->necesidad = calcularNecesidad(comuna);

            //guardo la satisfaccion que tenia, la primera vez todavia no hay
            if(comuna->satisfaccion <= 0)
                comuna->satisfaccionAnterior = 100.0 - comuna->necesidad;
            else
                comuna->satisfaccionAnterior = comuna->satisfaccion;
        }
        actual = recorrerMatriz(actual);
    }

    //y ya despues la satisfaccion
    actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        if(!actual->comuna->disuelta)
            calcularIndices(actual);
        actual = recorrerMatriz(actual);
    }
}

/*
======================================================================
                        | Menú de comuna |
======================================================================
*/

//ahora cada persona sale en una linea con lo que sabe hacer, sus libros y su criminalidad
void imprimirPersonas(struct listaPersonas* personas, char* nombre){
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Personas de la comuna %s: %d\n", nombre, personas->tamaño);
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");

    struct persona* actual = personas->inicio;
    while(actual != NULL){
        printf(" - %-12s | Libros: %d | Criminalidad: %2d%% | Servicios: ",
                actual->nombre, actual->librosLeidos, actual->criminalidad);

        //sus servicios separados por coma
        struct nodoListaSimple* servicio = actual->servicios->inicio;
        if(servicio == NULL)
            printf("ninguno");
        while(servicio != NULL){
            printf("%s", servicio->nombre);
            if(servicio->sigt != NULL)
                printf(", ");
            servicio = servicio->sigt;
        }
        printf("\n");

        actual = actual->sigt;
    }
    printf("\n");
}

void imprimirComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero){

    struct comuna* comuna = nodo->comuna;
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Te encuentras en la comuna %02d %s\n", comuna->numero, comuna->nombre);
    printf(" ╚═════════════════════════════════════════════===+- -\n");
    printf(" |                      |                    |                      |\n");
    printf(" |  Satisfacción: %5.1f |  Necesidad: %5.1f  |  Reciprocidad: %5.1f |\n\n", comuna->satisfaccion, comuna->necesidad, comuna->reciprocidad);

    while(1){
        imprimirLento(100,
        " ╔════════════════════════╗\n"
        " ║  * Escoge una acción   ║\n"
        " ╚════════════════════════╝\n"
        " | 0. Ver Bienes          |\n"
        " | 1. Ver Servicios       |\n"
        " | 2. Ver Personas        |\n"
        " | 3. Revisar otra comuna |\n"
        " | 4. Intercambiar        |\n"
        " | 5. Biblioteca          |\n\n"
        );

        int opcion = pedirEnIntervalo(0,5);

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
            imprimirServicios(comuna->diccionarioServicios, 0);
            printf("\n___________________________________________________\n");
        }else if(opcion == 2){
            imprimirPersonas(comuna->personas, comuna->nombre);
        }else if(opcion == 3){
            return;
        }else if(opcion == 4){
            realizarIntercambio(nodo, tablero);
        }else{
            menuBiblioteca(comuna, tablero);
        }
    }
    printf("\n___________________________________________________\n");
}

//retorna NULL cuando no se escogio una comuna (paso el dia, vio logros o guardo)
struct nodoMatriz* pedirComuna(struct nodoMatriz* actual, struct matrizComunas* tablero){
    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ Comuna actual: %02d    | Día actual: %d / %d\n", actual->comuna->numero, tablero->dia, tablero->diasTotales);
    printf(" ║\n");
    printf(" ║ Tus opciones son: \n");
    printf(" ║ 0. Cambiar comuna\n");
    printf(" ║ 1. Terminar día\n");
    printf(" ║ 2. Ver logros\n");
    printf(" ║ 3. Guardar y salir\n");
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
    int opcion = pedirEnIntervalo(0,3);

    if(opcion == 0){
        return buscarComuna(tablero, 1);      //solo deja escoger las del jugador
    }else if(opcion == 1){
        pasarDia(tablero);
        return NULL;
    }else if(opcion == 2){
        imprimirLogros(tablero, 0);
        return NULL;
    }else{
        imprimirLento(70,
        " ╔════════════════════════════════════════╗\n"
        " ║ * ¿Seguro que quieres guardar y salir? ║\n"
        " ╚════════════════════════════════════════╝\n"
        " | 1. Sí                                  |\n"
        " | 2. No                                  |\n\n"
        );

        if(pedirEnIntervalo(1,2) == 1){
            if(guardarPartida(tablero))
                tablero->salir = 1;
            else
                imprimirAdvertencia("No se pudo crear el archivo de guardado");
        }
        return NULL;
    }
}

//soloJugador=1 cuando solo se puede escoger una comuna del jugador, 0 cuando sirve cualquiera del mapa
struct nodoMatriz* buscarComuna(struct matrizComunas* tablero, int soloJugador){
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
        struct nodoMatriz* nodo = encontrarNodoMatriz(i,j,tablero);

        if(nodo->comuna->disuelta){
            imprimirAdvertencia("Esa comuna ya se disolvió | Escoge otra");
            continue;
        }
        if(soloJugador && !nodo->comuna->delJugador){
            imprimirAdvertencia("Esa comuna no es tuya | Escoge otra");
            continue;
        }
        return nodo;
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

//baja o sube de forma aleatoria cada bien
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

    //las comunas autonomas hacen sus intercambios calladitas
    intercambiosAutonomos(tablero);

    //consumo de todas las comunas
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* comuna = actual->comuna;

        if(!comuna->disuelta){
            //solo se gastan los bienes: los servicios son la gente que los sabe hacer
            consumirDiccionario(comuna->diccionarioBienes);
            comuna->intercambiosHoy = 0;

            //la comuna se va recuperando del desastre y olvidando los regalos
            comuna->desastre -= 3;
            if(comuna->desastre < 0) comuna->desastre = 0;
            comuna->regalosRecientes = comuna->regalosRecientes * 2 / 3;

            //mañana todos pueden volver a la biblioteca
            struct persona* persona = comuna->personas->inicio;
            while(persona != NULL){
                persona->leyoHoy = 0;
                persona = persona->sigt;
            }
        }

        actual = recorrerMatriz(actual);
    }

    eventosCriminales(tablero);

    continuarTormentas(tablero);
    escogerEmergencias(tablero);

    //con todo ya cambiado se recalculan los indices y se ve cuales comunas no aguantaron
    calcularIndicesTablero(tablero);
    revisarDisoluciones(tablero);

    //racha de dias sin intercambiar (para los logros)
    if(tablero->logros.intercambioHoy){
        tablero->logros.diasSinIntercambiar = 0;
    }else{
        tablero->logros.diasSinIntercambiar++;
        if(tablero->logros.diasSinIntercambiar > tablero->logros.rachaSinIntercambiar)
            tablero->logros.rachaSinIntercambiar = tablero->logros.diasSinIntercambiar;
    }
    tablero->logros.intercambioHoy = 0;

    tablero->dia++;

    //al empezar el dia nuevo puede que alguna comuna le quiera proponer algo al jugador
    if(tablero->dia <= tablero->diasTotales && contarComunasVivas(tablero) > 0)
        solicitudesAutonomas(tablero);
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

//escoge el tipo de emergencia y la comuna donde cae (solo comunas que sigan vivas)
void lanzarEmergencia(struct matrizComunas* tablero){
    int tipo = randomIntervalo(0, 5);

    struct nodoMatriz* nodo = comunaAlAzar(tablero, 0);
    if(nodo == NULL)
        return;

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

    int probabilidad = 20 + 20 * nivel;         //40%, 60%, 80% de que cada bien/servicio se vea afectado
    int porcentaje = 15 * nivel;                //pierde hasta el 15%, 30%, 45% de su maximo

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
    if(comuna->disuelta) return;        //a una comuna disuelta ya no le pasa nada

    reducirDiccionario(comuna->diccionarioBienes, nivelBienes);

    //los servicios no se pueden perder porque son lo que sabe la gente,
    //lo que pasa es que por unos dias no pueden trabajar igual y eso sube la necesidad
    comuna->desastre += nivelBienes + 2 * nivelServicios;
    if(comuna->desastre > DESASTRE_MAX) comuna->desastre = DESASTRE_MAX;
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
        " ║ !! Emergencia: %s !!\n", tipo);

    if(comuna != NULL)
        imprimirLento(100, " ║ Comuna afectada: %02d \n", comuna->numero);

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
        " | 1. Un servicio                 |\n\n"
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
        if(tipo == 0)
            imprimirDiccionarioNumerado(diccionario);
        else
            imprimirServicios(diccionario, 1);       //los servicios se cuentan en personas
        printf(" 0. Cancelar\n\n");

        int posicion = pedirEnIntervalo(0, total);
        if(posicion == 0)
            return NULL;

        struct nodoBienes* escogido = obtenerElemento(diccionario, posicion);
        if(escogido->cantidad <= 0){
            imprimirAdvertencia("No hay existencias de eso | Escoge otro\n");
            continue;
        }

        return escogido;
    }
}

//Intercambio entre la comuna del jugador (donde esta parado) y cualquier otra comuna del mapa
void realizarIntercambio(struct nodoMatriz* nodoA, struct matrizComunas* tablero){
    char mensaje[256];

    //---------------- primera comuna ----------------
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
        
        nodoB = buscarComuna(tablero, 0);        //puede ser cualquiera del mapa

        if(nodoB != nodoA)
            break;

        imprimirAdvertencia("| Tiene que ser otra comuna |");
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

    //las comunas autonomas no aceptan cualquier cosa
    if(!comunaB->delJugador && !servicioA && !servicioB && cantidadA * 2 < cantidadB){
        imprimirAdvertencia("La otra comuna rechazó el intercambio | Pides demasiado para lo que das");
        return;
    }

    //y tampoco sueltan a la gente que ocupan
    if(!comunaB->delJugador && servicioB && productoB->cantidad - cantidadB < productoB->maximo){
        imprimirAdvertencia("La otra comuna rechazó el intercambio | Ocupa a esas personas");
        return;
    }

    int realA = 0, realB = 0;
    int resultado = aplicarIntercambio(comunaA, productoA, cantidadA, servicioA,
                                       comunaB, productoB, cantidadB, servicioB, &realA, &realB);
    if(!resultado){
        imprimirAdvertencia("| Ninguna de las dos comunas tiene espacio para recibir | Intercambio cancelado |");
        return;
    }

    //cuenta para los logros
    tablero->logros.intercambios++;
    tablero->logros.intercambioHoy = 1;

    calcularIndicesTablero(tablero);

    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ !! Intercambio realizado !!\n");
    printf(" ║ De la comuna %02d a la comuna %02d: %d de %s\n", comunaA->numero, comunaB->numero, realA, productoA->nombre);
    printf(" ║ De la comuna %02d a la comuna %02d: %d de %s\n", comunaB->numero, comunaA->numero, realB, productoB->nombre);
    printf(" ║\n");
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
}

//mueve los productos entre las dos comunas: A entrega productoA y B entrega productoB
//lo usan el jugador y las comunas autonomas, por eso no imprime nada
int aplicarIntercambio(struct comuna* comunaA, struct nodoBienes* productoA, int cantidadA, int servicioA,
                       struct comuna* comunaB, struct nodoBienes* productoB, int cantidadB, int servicioB,
                       int* realA, int* realB){

    //busco los productos en la comuna que los recibe
    struct nodoBienes* productoAenB = buscarBienExacto(servicioA ? comunaB->diccionarioServicios : comunaB->diccionarioBienes, productoA->nombre);
    struct nodoBienes* productoBenA = buscarBienExacto(servicioB ? comunaA->diccionarioServicios : comunaA->diccionarioBienes, productoB->nombre);
    if(productoAenB == NULL || productoBenA == NULL)
        return 0;

    //solo se puede entregar lo que le cabe a la comuna que recibe
    *realA = cantidadA;
    int espacio = productoAenB->maximo - productoAenB->cantidad;
    if(espacio < 0) espacio = 0;
    if(*realA > espacio) *realA = espacio;

    *realB = cantidadB;
    espacio = productoBenA->maximo - productoBenA->cantidad;
    if(espacio < 0) espacio = 0;
    if(*realB > espacio) *realB = espacio;

    if(*realA == 0 && *realB == 0)
        return 0;

    //un servicio no se entrega como un bien: se va a la otra comuna la persona que lo sabe hacer
    //primero se sacan las de los dos lados, para que nadie se devuelva en el mismo intercambio
    struct listaPersonas* seVanDeA = NULL;
    struct listaPersonas* seVanDeB = NULL;
    if(servicioA){
        seVanDeA = sacarPersonasConServicio(comunaA, productoA->nombre, *realA);
        *realA = seVanDeA->tamaño;
    }
    if(servicioB){
        seVanDeB = sacarPersonasConServicio(comunaB, productoB->nombre, *realB);
        *realB = seVanDeB->tamaño;
    }

    //lo que regresa a quien da, siempre menos de lo que entrego, para que no se vacien jsjs
    int retornoA = *realA * RETORNO_REGALO / 100;
    int retornoB = *realB * RETORNO_REGALO / 100;

    if(servicioA){
        pasarPersonas(seVanDeA, comunaB);
    }else{
        productoA->cantidad    += retornoA - *realA;    //la que da se queda con menos, pero le regresa una parte
        productoAenB->cantidad += *realA;               //la que recibe se queda con todo
    }

    if(servicioB){
        pasarPersonas(seVanDeB, comunaA);
    }else{
        productoB->cantidad    += retornoB - *realB;
        productoBenA->cantidad += *realB;
    }

    //si se movio gente hay que volver a contar los oficios
    actualizarServicios(comunaA);
    actualizarServicios(comunaB);

    comunaA->intercambiosHoy++;
    comunaB->intercambiosHoy++;

    //se apunta lo dado y lo recibido para el indice de reciprocidad
    comunaA->regalosDados += *realA;
    comunaA->regalosRecibidos += *realB;
    comunaA->regalosRecientes += *realB;

    comunaB->regalosDados += *realB;
    comunaB->regalosRecibidos += *realA;
    comunaB->regalosRecientes += *realA;

    return 1;
}

/*
======================================================================
                    | Comunas autónomas |
======================================================================
*/

//el producto con menos porcentaje de lleno
struct nodoBienes* masEscaso(struct DiccionarioBienes* diccionario){
    struct nodoBienes* escogido = NULL;

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            //comparo porcentajes sin dividir: a/b < c/d es lo mismo que a*d < c*b
            if(escogido == NULL || bien->cantidad * escogido->maximo < escogido->cantidad * bien->maximo)
                escogido = bien;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
    return escogido;
}

//el producto con mas porcentaje de lleno
struct nodoBienes* masAbundante(struct DiccionarioBienes* diccionario){
    struct nodoBienes* escogido = NULL;

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            if(escogido == NULL || bien->cantidad * escogido->maximo > escogido->cantidad * bien->maximo)
                escogido = bien;
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
    return escogido;
}

//la comuna 'pide' arma su propuesta: pide lo que mas le falta y da lo que mas le sobra
int armarPropuesta(struct comuna* pide, struct comuna* otra, struct nodoBienes** productoDa, int* cantidadDa, int* servicioDa,
                   struct nodoBienes** productoPide, int* cantidadPide, int* servicioPide){

    //lo que mas le falta, a veces un bien y a veces un servicio
    *servicioPide = randomIntervalo(0, 1);
    struct nodoBienes* falta = masEscaso(*servicioPide ? pide->diccionarioServicios : pide->diccionarioBienes);

    //lo que mas le sobra
    *servicioDa = randomIntervalo(0, 1);
    *productoDa = masAbundante(*servicioDa ? pide->diccionarioServicios : pide->diccionarioBienes);

    if(falta == NULL || *productoDa == NULL)
        return 0;
    if(*servicioPide == *servicioDa && strcmp(falta->nombre, (*productoDa)->nombre) == 0)
        return 0;

    //la otra comuna tiene que tener de eso
    *productoPide = buscarBienExacto(*servicioPide ? otra->diccionarioServicios : otra->diccionarioBienes, falta->nombre);
    if(*productoPide == NULL)
        return 0;

    if(*servicioPide){
        //pide una persona con ese oficio: solo si a ella le falta y a la otra le sobra
        if(falta->cantidad >= falta->maximo)
            return 0;
        if((*productoPide)->cantidad <= (*productoPide)->maximo)
            return 0;
        *cantidadPide = 1;
    }else{
        //un bien solo lo suelta la otra si tiene y si esta mejor de eso que la que pide
        if((*productoPide)->cantidad < 1)
            return 0;
        if((*productoPide)->cantidad * falta->maximo <= falta->cantidad * (*productoPide)->maximo)
            return 0;

        //no pide mas de lo que le cabe ni mas de la mitad de lo que tiene la otra
        int tope = ((*productoPide)->cantidad + 1) / 2;
        int espacio = falta->maximo - falta->cantidad;
        if(tope > espacio) tope = espacio;
        if(tope < 1)
            return 0;
        *cantidadPide = randomIntervalo(1, tope);
    }

    if(*servicioDa){
        //solo ofrece gente si le sobra
        if((*productoDa)->cantidad <= (*productoDa)->maximo)
            return 0;
        *cantidadDa = 1;
    }else{
        if((*productoDa)->cantidad < 1)
            return 0;
        *cantidadDa = randomIntervalo(1, ((*productoDa)->cantidad + 1) / 2);
    }
    return 1;
}

//una comuna autonoma busca con quien intercambiar: si con una no se puede, prueba con otra
void buscarIntercambio(struct nodoMatriz* nodoA, struct matrizComunas* tablero){
    struct comuna* comunaA = nodoA->comuna;

    for(int intento = 0; intento < INTENTOS_AUTONOMOS; intento++){
        if(comunaA->intercambiosHoy >= INTERCAMBIOS_MAX_DIA)
            return;

        struct nodoMatriz* nodoB = comunaAlAzar(tablero, 1);
        if(nodoB == NULL || nodoB == nodoA)
            continue;

        struct comuna* comunaB = nodoB->comuna;
        if(comunaB->intercambiosHoy >= INTERCAMBIOS_MAX_DIA)
            continue;

        struct nodoBienes* productoDa;
        struct nodoBienes* productoPide;
        int cantidadDa, cantidadPide, servicioDa, servicioPide;

        if(!armarPropuesta(comunaA, comunaB, &productoDa, &cantidadDa, &servicioDa, &productoPide, &cantidadPide, &servicioPide))
            continue;

        int realA, realB;
        if(aplicarIntercambio(comunaA, productoDa, cantidadDa, servicioDa,
                              comunaB, productoPide, cantidadPide, servicioPide, &realA, &realB))
            return;     //ya consiguio su intercambio de esta vuelta
    }
}

//las comunas que no maneja el jugador intercambian entre ellas, sin imprimir nada
//se dan varias vueltas al tablero y en cada vuelta a cada comuna autonoma le toca buscar un intercambio
void intercambiosAutonomos(struct matrizComunas* tablero){
    int vueltas = randomIntervalo(VUELTAS_AUTONOMAS_MIN, VUELTAS_AUTONOMAS_MAX);

    for(int v = 0; v < vueltas; v++){
        struct nodoMatriz* actual = tablero->inicio;
        for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
            if(!actual->comuna->disuelta && !actual->comuna->delJugador)
                buscarIntercambio(actual, tablero);
            actual = recorrerMatriz(actual);
        }
    }
}

//de vez en cuando una comuna autonoma le propone un intercambio a alguna comuna del jugador
void solicitudesAutonomas(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;

    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* mia = actual->comuna;
        actual = recorrerMatriz(actual);

        if(mia->disuelta || !mia->delJugador)
            continue;
        if(randomIntervalo(1, 100) > PROB_SOLICITUD)
            continue;

        struct nodoMatriz* nodoOtra = comunaAlAzar(tablero, 1);
        if(nodoOtra == NULL)
            return;         //ya no quedan comunas autonomas
        struct comuna* otra = nodoOtra->comuna;

        struct nodoBienes* productoDa;
        struct nodoBienes* productoPide;
        int cantidadDa, cantidadPide, servicioDa, servicioPide;

        if(!armarPropuesta(otra, mia, &productoDa, &cantidadDa, &servicioDa, &productoPide, &cantidadPide, &servicioPide))
            continue;

        printf(" ╔══════════════════════════════════════════════===+- -- -\n");
        printf(" ║ !! Solicitud de intercambio !!\n");
        printf(" ║ La comuna %02d %s quiere intercambiar\n", otra->numero, otra->nombre);
        printf(" ║ %d de %s por %d de %s\n", cantidadDa, productoDa->nombre, cantidadPide, productoPide->nombre);
        printf(" ║ con tu comuna %02d %s\n", mia->numero, mia->nombre);
        printf(" ╚═════════════════════════════════════════════===+- -\n");
        printf(" | 1. Aceptar            | 2. Rechazar           |\n\n");

        if(pedirEnIntervalo(1, 2) == 2){
            printf("\n | Solicitud rechazada |\n\n");
            continue;
        }

        int realA, realB;
        if(!aplicarIntercambio(otra, productoDa, cantidadDa, servicioDa,
                               mia, productoPide, cantidadPide, servicioPide, &realA, &realB)){
            imprimirAdvertencia("Al final no había espacio para recibir | No se hizo el intercambio");
            continue;
        }

        tablero->logros.intercambios++;
        tablero->logros.intercambioHoy = 1;
        calcularIndicesTablero(tablero);

        printf("\n | Intercambio aceptado: recibes %d de %s y entregas %d de %s |\n\n",
                realA, productoDa->nombre, realB, productoPide->nombre);
    }
}

/*
======================================================================
                | Disolución de comunas y jugador |
======================================================================
*/

int contarComunasVivas(struct matrizComunas* tablero){
    int vivas = 0;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        if(!actual->comuna->disuelta)
            vivas++;
        actual = recorrerMatriz(actual);
    }
    return vivas;
}

int contarComunasJugador(struct matrizComunas* tablero){
    int mias = 0;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        if(!actual->comuna->disuelta && actual->comuna->delJugador)
            mias++;
        actual = recorrerMatriz(actual);
    }
    return mias;
}

//tipo: 0=cualquiera viva, 1=solo autonomas, 2=solo del jugador
struct nodoMatriz* comunaAlAzar(struct matrizComunas* tablero, int tipo){
    int total = tablero->tamaño * tablero->tamaño;

    //primero cuento cuantas sirven
    int sirven = 0;
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        struct comuna* comuna = actual->comuna;
        if(!comuna->disuelta && (tipo == 0 || (tipo == 1 && !comuna->delJugador) || (tipo == 2 && comuna->delJugador)))
            sirven++;
        actual = recorrerMatriz(actual);
    }
    if(sirven == 0)
        return NULL;

    //y despues camino hasta la que salio en el sorteo
    int escogida = randomIntervalo(1, sirven);
    actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        struct comuna* comuna = actual->comuna;
        if(!comuna->disuelta && (tipo == 0 || (tipo == 1 && !comuna->delJugador) || (tipo == 2 && comuna->delJugador))){
            escogida--;
            if(escogida == 0)
                return actual;
        }
        actual = recorrerMatriz(actual);
    }
    return NULL;
}

//mete en el arreglo los vecinos (los mismos 8 de sumarVecinos) que sigan vivos
int obtenerVecinosVivos(struct nodoMatriz* nodo, struct nodoMatriz* vecinos[8]){
    struct nodoMatriz* todos[8];
    todos[0] = nodo->arriba;
    todos[1] = nodo->abajo;
    todos[2] = nodo->izquierda;
    todos[3] = nodo->derecha;
    todos[4] = nodo->arriba ? nodo->arriba->izquierda : NULL;
    todos[5] = nodo->arriba ? nodo->arriba->derecha : NULL;
    todos[6] = nodo->abajo ? nodo->abajo->izquierda : NULL;
    todos[7] = nodo->abajo ? nodo->abajo->derecha : NULL;

    int cantidad = 0;
    for(int i = 0; i < 8; i++){
        if(todos[i] != NULL && !todos[i]->comuna->disuelta){
            vecinos[cantidad] = todos[i];
            cantidad++;
        }
    }
    return cantidad;
}

void liberarDiccionario(struct DiccionarioBienes* diccionario){
    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            struct nodoBienes* sig = bien->sigt;
            free(bien->nombre);
            free(bien);
            bien = sig;
        }
        struct nodoDic* sigCasilla = casilla->sigt;
        free(casilla);
        casilla = sigCasilla;
    }
    free(diccionario);
}

//si llega mas gente la comuna ocupa mas de todo, entonces los maximos suben un poco
//(no en la misma proporcion porque si no las vecinas se caen una tras otra)
void ajustarMaximos(struct comuna* comuna, int personasAntes){
    int personasAhora = comuna->personas->tamaño;
    if(personasAntes <= 0 || personasAhora == personasAntes)
        return;

    struct nodoDic* casilla = comuna->diccionarioBienes->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            bien->maximo += bien->maximo * (personasAhora - personasAntes) / (personasAntes * 4);
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }

    //llego gente nueva con sus oficios
    actualizarServicios(comuna);
}

void registrarEvento(struct matrizComunas* tablero, char* texto){
    char linea[300];
    snprintf(linea, sizeof(linea), "Día %d: %s", tablero->dia, texto);

    //queda al inicio, asi lo mas nuevo sale primero
    agregarNodoSimple(crearNodoListaSimple(strdup(linea)), tablero->historial);
}

//le da comunas al jugador hasta que tenga 3, o hasta que no queden mas
void asignarComunasJugador(struct matrizComunas* tablero){
    while(contarComunasJugador(tablero) < COMUNAS_JUGADOR){
        struct nodoMatriz* nodo = comunaAlAzar(tablero, 1);
        if(nodo == NULL)
            break;

        nodo->comuna->delJugador = 1;

        //queda apuntado en el historial (al inicio de la partida no hace falta)
        if(tablero->logros.comunasPerdidas > 0){
            char mensaje[200];
            snprintf(mensaje, sizeof(mensaje), "Ahora manejas la comuna %02d %s", nodo->comuna->numero, nodo->comuna->nombre);
            registrarEvento(tablero, mensaje);
        }
    }
}

//la comuna deja de existir: la gente se reparte en las vecinas y los bienes se pierden
void disolverComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero){
    struct comuna* comuna = nodo->comuna;
    comuna->disuelta = 1;
    comuna->diasTormenta = 0;

    struct nodoMatriz* vecinos[8];
    int cantidad = obtenerVecinosVivos(nodo, vecinos);

    //apunto cuanta gente tenia cada vecina antes de que le lleguen
    int personasAntes[8];
    for(int i = 0; i < cantidad; i++){
        personasAntes[i] = vecinos[i]->comuna->personas->tamaño;
    }

    int total = comuna->personas->tamaño;
    int turno = 0;

    //cada persona se va con todo y sus servicios
    while(comuna->personas->inicio != NULL){
        struct persona* persona = comuna->personas->inicio;
        struct comuna* destino;

        if(cantidad > 0){
            destino = vecinos[turno % cantidad]->comuna;    //una para cada vecina, por turnos
            turno++;
        }else{
            //si no le quedan vecinas se van a cualquier comuna que siga en pie
            struct nodoMatriz* otra = comunaAlAzar(tablero, 0);
            if(otra == NULL)
                break;      //ya no queda ninguna comuna
            destino = otra->comuna;
        }

        sacarPersona(persona, comuna->personas);
        agregarPersona(persona, destino->personas);

        if(cantidad == 0)
            actualizarServicios(destino);
    }

    for(int i = 0; i < cantidad; i++){
        ajustarMaximos(vecinos[i]->comuna, personasAntes[i]);
    }

    //los bienes quedan abandonados, y la biblioteca tambien
    liberarDiccionario(comuna->diccionarioBienes);
    liberarDiccionario(comuna->diccionarioServicios);
    comuna->diccionarioBienes = NULL;
    comuna->diccionarioServicios = NULL;

    liberarSplay(comuna->biblioteca);
    comuna->biblioteca = NULL;

    char mensaje[200];
    snprintf(mensaje, sizeof(mensaje), "Se disolvió la comuna %02d %s", comuna->numero, comuna->nombre);
    registrarEvento(tablero, mensaje);

    imprimirLento(100,
        " ╔══════════════════════════════════════════════===+- -- -\n"
        " ║ !! La comuna %02d %s se disolvió !!\n"
        " ║ Sus %d personas se fueron a otras comunas\n"
        " ║ Sus bienes quedaron abandonados\n",
        comuna->numero, comuna->nombre, total);

    //si era del jugador se apunta para los logros
    if(comuna->delJugador){
        comuna->delJugador = 0;
        printf(" ║ Era una de tus comunas\n");

        struct logros* logros = &tablero->logros;
        logros->comunasPerdidas++;

        //tres perdidas en menos de 3 turnos
        if(logros->comunasPerdidas >= 3 && tablero->dia - logros->diaPerdida2 < 3)
            logros->caos = 1;

        logros->diaPerdida2 = logros->diaPerdida1;
        logros->diaPerdida1 = tablero->dia;
    }
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");
}

//una comuna se disuelve si tiene mucha necesidad Y poca satisfaccion
void revisarDisoluciones(struct matrizComunas* tablero){
    int disueltas = 0;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* comuna = actual->comuna;

        if(!comuna->disuelta && comuna->necesidad >= NECESIDAD_DISOLUCION && comuna->satisfaccion <= SATISFACCION_DISOLUCION){
            disolverComuna(actual, tablero);
            disueltas++;
        }
        actual = recorrerMatriz(actual);
    }

    if(disueltas == 0)
        return;

    //si el jugador perdio alguna se le da otra, y los indices cambian porque se movio gente
    asignarComunasJugador(tablero);
    calcularIndicesTablero(tablero);
}

/*
======================================================================
                | Árbol AVL: catálogo de libros |
======================================================================
Se arma una sola vez al arrancar con los nombres de todos los archivos de la carpeta,
como queda balanceado buscar un libro por su nombre exacto es rapido aunque sean miles
*/

int alturaAVL(struct nodoAVL* nodo){
    if(nodo == NULL)
        return 0;
    return nodo->altura;
}

struct nodoAVL* crearNodoAVL(char* titulo, char* ruta){
    struct nodoAVL* nuevo = calloc(1, sizeof(struct nodoAVL));
    nuevo->titulo = strdup(titulo);
    nuevo->ruta = strdup(ruta);
    nuevo->altura = 1;
    return nuevo;
}

//la altura de un nodo es la del hijo mas alto + 1
void actualizarAltura(struct nodoAVL* nodo){
    int izq = alturaAVL(nodo->izq);
    int der = alturaAVL(nodo->der);
    nodo->altura = (izq > der ? izq : der) + 1;
}

struct nodoAVL* rotarDerechaAVL(struct nodoAVL* nodo){
    struct nodoAVL* sube = nodo->izq;

    nodo->izq = sube->der;
    sube->der = nodo;

    actualizarAltura(nodo);
    actualizarAltura(sube);
    return sube;
}

struct nodoAVL* rotarIzquierdaAVL(struct nodoAVL* nodo){
    struct nodoAVL* sube = nodo->der;

    nodo->der = sube->izq;
    sube->izq = nodo;

    actualizarAltura(nodo);
    actualizarAltura(sube);
    return sube;
}

struct nodoAVL* insertarAVL(struct nodoAVL* raiz, char* titulo, char* ruta){
    if(raiz == NULL)
        return crearNodoAVL(titulo, ruta);

    int comparacion = strcmp(titulo, raiz->titulo);
    if(comparacion < 0)
        raiz->izq = insertarAVL(raiz->izq, titulo, ruta);
    else if(comparacion > 0)
        raiz->der = insertarAVL(raiz->der, titulo, ruta);
    else
        return raiz;        //ya estaba

    actualizarAltura(raiz);
    int balance = alturaAVL(raiz->izq) - alturaAVL(raiz->der);

    //pesa mas a la izquierda
    if(balance > 1){
        if(strcmp(titulo, raiz->izq->titulo) > 0)       //caso izquierda-derecha
            raiz->izq = rotarIzquierdaAVL(raiz->izq);
        return rotarDerechaAVL(raiz);
    }

    //pesa mas a la derecha
    if(balance < -1){
        if(strcmp(titulo, raiz->der->titulo) < 0)       //caso derecha-izquierda
            raiz->der = rotarDerechaAVL(raiz->der);
        return rotarIzquierdaAVL(raiz);
    }

    return raiz;
}

//busqueda por coincidencia exacta del nombre
struct nodoAVL* buscarAVL(struct nodoAVL* raiz, char* titulo){
    while(raiz != NULL){
        int comparacion = strcmp(titulo, raiz->titulo);

        if(comparacion == 0)
            return raiz;
        else if(comparacion < 0)
            raiz = raiz->izq;
        else
            raiz = raiz->der;
    }
    return NULL;
}

//recorre en orden hasta llegar al libro numero 'posicion'
struct nodoAVL* obtenerAVL(struct nodoAVL* raiz, int posicion, int* contador){
    if(raiz == NULL)
        return NULL;

    struct nodoAVL* encontrado = obtenerAVL(raiz->izq, posicion, contador);
    if(encontrado != NULL)
        return encontrado;

    (*contador)++;
    if(*contador == posicion)
        return raiz;

    return obtenerAVL(raiz->der, posicion, contador);
}

//lee la carpeta (y las carpetas que tenga adentro) y mete cada archivo al AVL
struct nodoAVL* cargarCatalogo(char* carpeta, struct nodoAVL* raiz, int* total){
    DIR* directorio = opendir(carpeta);
    if(directorio == NULL)
        return raiz;

    struct dirent* entrada;
    while((entrada = readdir(directorio)) != NULL){
        if(entrada->d_name[0] == '.')
            continue;       //se salta . y .. y los archivos ocultos

        char ruta[2048];
        snprintf(ruta, sizeof(ruta), "%s/%s", carpeta, entrada->d_name);

        struct stat info;
        if(stat(ruta, &info) != 0)
            continue;

        //si es una carpeta se mete a buscar adentro
        if(S_ISDIR(info.st_mode)){
            raiz = cargarCatalogo(ruta, raiz, total);
            continue;
        }

        //el titulo es el nombre del archivo sin el .txt
        char titulo[300];
        strncpy(titulo, entrada->d_name, sizeof(titulo) - 1);
        titulo[sizeof(titulo) - 1] = '\0';

        int largo = strlen(titulo);
        if(largo > 4 && strcmp(titulo + largo - 4, ".txt") == 0)
            titulo[largo - 4] = '\0';

        if(buscarAVL(raiz, titulo) != NULL)
            continue;       //repetido

        raiz = insertarAVL(raiz, titulo, ruta);
        (*total)++;
    }

    closedir(directorio);
    return raiz;
}

void prepararBiblioteca(struct matrizComunas* tablero){
    mkdir(CARPETA_LIBROS, 0755);        //si ya existe no pasa nada

    printf("\n | Cargando el catálogo de libros... |\n");
    tablero->totalLibros = 0;
    tablero->catalogo = cargarCatalogo(CARPETA_LIBROS, NULL, &tablero->totalLibros);

    if(tablero->totalLibros == 0)
        imprimirAdvertencia("No hay libros en la carpeta 'libros' | Las bibliotecas van a estar vacías");
    else
        printf(" | Libros en el catálogo: %d | Altura del árbol AVL: %d |\n\n", tablero->totalLibros, alturaAVL(tablero->catalogo));
}

//imprime los titulos en orden alfabetico, hasta el limite
void imprimirCatalogo(struct nodoAVL* raiz, int* contador, int limite){
    if(raiz == NULL || *contador >= limite)
        return;

    imprimirCatalogo(raiz->izq, contador, limite);

    if(*contador < limite){
        (*contador)++;
        printf(" %3d. %s\n", *contador, raiz->titulo);
    }

    imprimirCatalogo(raiz->der, contador, limite);
}

/*
======================================================================
            | Árbol chaflaneado (splay): caché de la biblioteca |
======================================================================
Cada vez que se busca un libro, ese libro sube hasta la raiz con rotaciones.
Asi los libros que mas se leen quedan arriba y se encuentran casi de una vez
*/

struct nodoSplay* crearNodoSplay(char* titulo){
    struct nodoSplay* nuevo = calloc(1, sizeof(struct nodoSplay));
    nuevo->titulo = strdup(titulo);
    return nuevo;
}

//el hijo izquierdo sube y el nodo baja a la derecha
struct nodoSplay* rotarDerechaSplay(struct nodoSplay* nodo){
    struct nodoSplay* sube = nodo->izq;

    nodo->izq = sube->der;
    sube->der = nodo;

    if(verRotaciones)
        printf("    ~ rotación a la derecha: sube \"%.25s\" y baja \"%.25s\"\n", sube->titulo, nodo->titulo);
    return sube;
}

//el hijo derecho sube y el nodo baja a la izquierda
struct nodoSplay* rotarIzquierdaSplay(struct nodoSplay* nodo){
    struct nodoSplay* sube = nodo->der;

    nodo->der = sube->izq;
    sube->izq = nodo;

    if(verRotaciones)
        printf("    ~ rotación a la izquierda: sube \"%.25s\" y baja \"%.25s\"\n", sube->titulo, nodo->titulo);
    return sube;
}

//sube el titulo buscado hasta la raiz. Si no esta, sube el ultimo nodo por el que paso
struct nodoSplay* chaflanear(struct nodoSplay* raiz, char* titulo){
    if(raiz == NULL)
        return NULL;

    int comparacion = strcmp(titulo, raiz->titulo);
    if(comparacion == 0)
        return raiz;

    if(comparacion < 0){
        //esta por la izquierda
        if(raiz->izq == NULL)
            return raiz;

        int comparacionHijo = strcmp(titulo, raiz->izq->titulo);

        if(comparacionHijo < 0){
            //zig-zig: izquierda de la izquierda, primero se rota el abuelo
            raiz->izq->izq = chaflanear(raiz->izq->izq, titulo);
            raiz = rotarDerechaSplay(raiz);
        }else if(comparacionHijo > 0){
            //zig-zag: derecha de la izquierda, primero se rota el papa
            raiz->izq->der = chaflanear(raiz->izq->der, titulo);
            if(raiz->izq->der != NULL)
                raiz->izq = rotarIzquierdaSplay(raiz->izq);
        }

        //zig: la ultima rotacion lo deja en la raiz
        if(raiz->izq == NULL)
            return raiz;
        return rotarDerechaSplay(raiz);
    }

    //esta por la derecha, es lo mismo pero al reves
    if(raiz->der == NULL)
        return raiz;

    int comparacionHijo = strcmp(titulo, raiz->der->titulo);

    if(comparacionHijo > 0){
        //zig-zig
        raiz->der->der = chaflanear(raiz->der->der, titulo);
        raiz = rotarIzquierdaSplay(raiz);
    }else if(comparacionHijo < 0){
        //zig-zag
        raiz->der->izq = chaflanear(raiz->der->izq, titulo);
        if(raiz->der->izq != NULL)
            raiz->der = rotarDerechaSplay(raiz->der);
    }

    if(raiz->der == NULL)
        return raiz;
    return rotarIzquierdaSplay(raiz);
}

//mete un libro nuevo en la raiz. El arbol ya tiene que venir chaflaneado con ese titulo
struct nodoSplay* insertarSplay(struct nodoSplay* raiz, struct nodoSplay* nuevo){
    if(raiz == NULL)
        return nuevo;

    //la raiz vieja queda de un lado y el resto del arbol del otro
    if(strcmp(nuevo->titulo, raiz->titulo) < 0){
        nuevo->der = raiz;
        nuevo->izq = raiz->izq;
        raiz->izq = NULL;
    }else{
        nuevo->izq = raiz;
        nuevo->der = raiz->der;
        raiz->der = NULL;
    }
    return nuevo;
}

//insercion normal de arbol binario de busqueda, sin rotar nada
//solo se usa al cargar una partida, para que el arbol quede con la misma forma que tenia
struct nodoSplay* insertarSinChaflanear(struct nodoSplay* raiz, struct nodoSplay* nuevo){
    if(raiz == NULL)
        return nuevo;

    if(strcmp(nuevo->titulo, raiz->titulo) < 0)
        raiz->izq = insertarSinChaflanear(raiz->izq, nuevo);
    else
        raiz->der = insertarSinChaflanear(raiz->der, nuevo);
    return raiz;
}

int contarSplay(struct nodoSplay* raiz){
    if(raiz == NULL)
        return 0;
    return 1 + contarSplay(raiz->izq) + contarSplay(raiz->der);
}

//recorre en orden hasta llegar al nodo numero 'posicion'
struct nodoSplay* obtenerSplay(struct nodoSplay* raiz, int posicion, int* contador){
    if(raiz == NULL)
        return NULL;

    struct nodoSplay* encontrado = obtenerSplay(raiz->izq, posicion, contador);
    if(encontrado != NULL)
        return encontrado;

    (*contador)++;
    if(*contador == posicion)
        return raiz;

    return obtenerSplay(raiz->der, posicion, contador);
}

void liberarSplay(struct nodoSplay* raiz){
    if(raiz == NULL)
        return;

    liberarSplay(raiz->izq);
    liberarSplay(raiz->der);
    free(raiz->titulo);
    free(raiz);
}

//escribe el nombre corto del nodo en el renglon de su nivel, centrado en el pedazo que le toca
//(esta funcion solo la usa imprimirArbolSplay, por eso no esta en el .h)
void colocarNodo(struct nodoSplay* nodo, int nivel, int izquierda, int derecha, char renglones[NIVELES_ARBOL][ANCHO_ARBOL + 1], int* cortado){
    if(nodo == NULL)
        return;

    if(nivel >= NIVELES_ARBOL){
        *cortado = 1;       //hay mas arbol abajo pero no se imprime
        return;
    }

    //solo las primeras 8 letras del titulo
    char corto[9];
    int largo = 0;
    while(largo < 8 && nodo->titulo[largo] != '\0'){
        unsigned char letra = nodo->titulo[largo];
        if(letra < 32 || letra > 126)
            letra = '?';        //las tildes descuadran todo
        corto[largo] = letra;
        largo++;
    }
    corto[largo] = '\0';

    int centro = (izquierda + derecha) / 2;
    int inicio = centro - largo / 2;
    for(int i = 0; i < largo; i++){
        renglones[nivel][inicio + i] = corto[i];
    }

    //cada hijo se queda con la mitad del espacio
    colocarNodo(nodo->izq, nivel + 1, izquierda, centro, renglones, cortado);
    colocarNodo(nodo->der, nivel + 1, centro, derecha, renglones, cortado);
}

//imprime los primeros niveles del arbol en forma de piramide
void imprimirArbolSplay(struct nodoSplay* raiz){
    if(raiz == NULL){
        printf("\n | La caché de esta biblioteca está vacía |\n\n");
        return;
    }

    //lleno los renglones de espacios
    char renglones[NIVELES_ARBOL][ANCHO_ARBOL + 1];
    for(int i = 0; i < NIVELES_ARBOL; i++){
        for(int j = 0; j < ANCHO_ARBOL; j++){
            renglones[i][j] = ' ';
        }
        renglones[i][ANCHO_ARBOL] = '\0';
    }

    int cortado = 0;
    colocarNodo(raiz, 0, 0, ANCHO_ARBOL, renglones, &cortado);

    printf("\n  Árbol de la biblioteca | raíz: \"%s\"\n\n", raiz->titulo);
    for(int i = 0; i < NIVELES_ARBOL; i++){
        //un nivel sin nodos no se imprime (queda el nombre de la raiz en el centro del primero)
        int vacio = 1;
        for(int j = 0; j < ANCHO_ARBOL; j++){
            if(renglones[i][j] != ' ')
                vacio = 0;
        }
        if(!vacio)
            printf("%s\n\n", renglones[i]);
    }
    if(cortado)
        printf("  (... el árbol sigue más abajo)\n");
    printf("\n");
}

/*
======================================================================
                        | Biblioteca |
======================================================================
*/

//"carga" el libro: lo abre y lo lee completo, contando lo que leyo
int cargarLibro(char* ruta, long* bytes, int* lineas){
    FILE* archivo = fopen(ruta, "r");
    if(archivo == NULL)
        return 0;

    *bytes = 0;
    *lineas = 0;

    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), archivo) != NULL){
        *bytes += strlen(buffer);
        (*lineas)++;
    }

    fclose(archivo);
    return 1;
}

//la persona aprende un servicio al azar de los que todavia no sabe
void aprenderServicio(struct matrizComunas* tablero, struct persona* persona){
    struct listaSimple* disponibles = tablero->serviciosDisponibles;

    int nuevos = disponibles->tamaño - persona->servicios->tamaño;
    if(nuevos <= 0){
        printf(" | %s ya sabe todos los servicios que existen |\n", persona->nombre);
        return;
    }

    //camino por los servicios saltandome los que ya sabe
    int escogido = randomIntervalo(1, nuevos);
    struct nodoListaSimple* actual = disponibles->inicio;
    while(actual != NULL){
        if(!estaEnLista(persona->servicios, actual->nombre)){
            escogido--;
            if(escogido == 0)
                break;
        }
        actual = actual->sigt;
    }
    if(actual == NULL)
        return;

    darServicio(persona, actual->nombre);
    persona->librosLeidos = 0;

    char mensaje[200];
    snprintf(mensaje, sizeof(mensaje), "%s aprendió el servicio %s", persona->nombre, actual->nombre);
    registrarEvento(tablero, mensaje);

    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ !! %s leyó %d libros y aprendió: %s !!\n", persona->nombre, LIBROS_PARA_APRENDER, actual->nombre);
    printf(" ╚═════════════════════════════════════════════===+- -\n");
}

//una persona va a la biblioteca: primero se busca el libro en la cache y si no esta se va al archivo
void leerLibro(struct matrizComunas* tablero, struct comuna* comuna, struct persona* persona){
    char* titulo;
    int contador = 0;

    //casi siempre se antoja de un libro que ya alguien leyo (esta en la cache), si no uno cualquiera del catalogo
    int enCache = contarSplay(comuna->biblioteca);
    if(enCache > 0 && randomIntervalo(1, 100) <= PROB_CACHE)
        titulo = obtenerSplay(comuna->biblioteca, randomIntervalo(1, enCache), &contador)->titulo;
    else
        titulo = obtenerAVL(tablero->catalogo, randomIntervalo(1, tablero->totalLibros), &contador)->titulo;

    printf("\n___________________________________________________\n");
    printf(" | %s leyó el libro \"%s\"\n", persona->nombre, titulo);

    //1. se busca en el arbol chaflaneado
    verRotaciones = 1;
    comuna->biblioteca = chaflanear(comuna->biblioteca, titulo);
    verRotaciones = 0;

    if(comuna->biblioteca != NULL && strcmp(comuna->biblioteca->titulo, titulo) == 0){
        comuna->biblioteca->vecesLeido++;      //estaba en la cache y ya quedo en la raiz
    }else{
        //2. no estaba: se busca por nombre en el catalogo, se carga el archivo y se guarda en la raiz
        struct nodoAVL* libro = buscarAVL(tablero->catalogo, titulo);

        long bytes = 0;
        int lineas = 0;
        if(libro == NULL || !cargarLibro(libro->ruta, &bytes, &lineas)){
            imprimirAdvertencia("No se pudo abrir el archivo de ese libro");
            return;
        }

        struct nodoSplay* nuevo = crearNodoSplay(titulo);
        nuevo->tamañoBytes = bytes;
        nuevo->lineas = lineas;
        nuevo->vecesLeido = 1;
        comuna->biblioteca = insertarSplay(comuna->biblioteca, nuevo);
    }

    imprimirArbolSplay(comuna->biblioteca);

    persona->librosLeidos++;
    persona->leyoHoy = 1;

    if(persona->librosLeidos >= LIBROS_PARA_APRENDER){
        aprenderServicio(tablero, persona);
        actualizarServicios(comuna);        //ahora hay una persona mas con ese oficio
    }
}

//muestra la gente de la comuna para escoger quienes van a leer
void enviarALeer(struct comuna* comuna, struct matrizComunas* tablero){
    if(tablero->totalLibros == 0){
        imprimirAdvertencia("No hay libros en la carpeta 'libros'");
        return;
    }

    struct listaPersonas* personas = comuna->personas;
    if(personas->tamaño == 0){
        imprimirAdvertencia("No hay nadie en esta comuna");
        return;
    }

    //lista numerada
    printf("\n");
    int numero = 1;
    struct persona* persona = personas->inicio;
    while(persona != NULL){
        persona->seleccionada = 0;
        printf(" %2d. %-12s | Libros: %d | Servicios: %d %s\n", numero, persona->nombre, persona->librosLeidos,
                persona->servicios->tamaño, persona->leyoHoy ? "| ya leyó hoy" : "");
        numero++;
        persona = persona->sigt;
    }

    imprimirLento(70,
        "\n ╔══════════════════════════════════════════════╗\n"
        " ║ * Escribe el número de cada persona a enviar ║\n"
        " ╚══════════════════════════════════════════════╝\n"
        " | 0. Enviar a las personas escogidas           |\n\n"
    );

    int escogidas = 0;
    while(1){
        int opcion = pedirEnIntervalo(0, personas->tamaño);
        if(opcion == 0)
            break;

        persona = obtenerPersona(personas, opcion);
        if(persona->leyoHoy){
            printf(" | %s ya fue hoy a la biblioteca |\n", persona->nombre);
        }else if(persona->seleccionada){
            printf(" | %s ya estaba escogida |\n", persona->nombre);
        }else{
            persona->seleccionada = 1;
            escogidas++;
            printf(" | + %s | van %d |\n", persona->nombre, escogidas);
        }
    }

    if(escogidas == 0){
        imprimirAdvertencia("No se envió a nadie");
        return;
    }

    //cada una lee su libro
    persona = personas->inicio;
    while(persona != NULL){
        if(persona->seleccionada){
            persona->seleccionada = 0;
            leerLibro(tablero, comuna, persona);
        }
        persona = persona->sigt;
    }
}

void menuBiblioteca(struct comuna* comuna, struct matrizComunas* tablero){
    while(1){
        imprimirLento(70,
            "\n ╔════════════════════════════════════╗\n"
            " ║  * Biblioteca comunitaria          ║\n"
            " ╚════════════════════════════════════╝\n"
            " | 0. Enviar personas a leer          |\n"
            " | 1. Ver los libros que existen      |\n"
            " | 2. Ver el árbol de la biblioteca   |\n"
            " | 3. Volver                          |\n\n"
        );

        int opcion = pedirEnIntervalo(0, 3);

        if(opcion == 0){
            enviarALeer(comuna, tablero);
        }else if(opcion == 1){
            int contador = 0;
            printf("\n | Catálogo: %d libros en total |\n\n", tablero->totalLibros);
            imprimirCatalogo(tablero->catalogo, &contador, 40);
            if(tablero->totalLibros > 40)
                printf(" ... y %d más\n", tablero->totalLibros - 40);
        }else if(opcion == 2){
            imprimirArbolSplay(comuna->biblioteca);
        }else{
            return;
        }
    }
}

/*
======================================================================
                        | Criminalidad |
======================================================================
*/

//una persona marcada hace algo malo y le sube la criminalidad, si llega al limite la expulsan
void cometerCrimen(struct persona* persona, struct nodoMatriz* nodo, struct matrizComunas* tablero){
    struct comuna* comuna = nodo->comuna;

    //lo leve pasa mucho mas seguido que lo grave
    int sorteo = randomIntervalo(1, 100);
    char* crimen;
    int valor;

    if(sorteo <= 30)        { crimen = "se saltó las leyes de tránsito";  valor = 3;  }
    else if(sorteo <= 55)   { crimen = "cometió un robo";                 valor = 4;  }
    else if(sorteo <= 75)   { crimen = "provocó disturbios";              valor = 5;  }
    else if(sorteo <= 90)   { crimen = "cometió un asalto";               valor = 10; }
    else if(sorteo <= 97)   { crimen = "cometió un secuestro";            valor = 25; }
    else                    { crimen = "cometió un asesinato";            valor = 50; }

    persona->criminalidad += valor;
    if(persona->criminalidad > 100) persona->criminalidad = 100;

    //solo se avisa lo que pasa en las comunas del jugador
    if(comuna->delJugador)
        printf(" |  Comuna %02d: %s %s  |\n\n", comuna->numero, persona->nombre, crimen);

    if(persona->criminalidad < CRIMINALIDAD_EXPULSION)
        return;

    //expulsion: se va a una comuna adyacente con todo y sus servicios
    struct nodoMatriz* vecinos[8];
    int cantidad = obtenerVecinosVivos(nodo, vecinos);
    if(cantidad == 0)
        return;     //no tiene para donde irse

    struct comuna* destino = vecinos[randomIntervalo(0, cantidad - 1)]->comuna;
    sacarPersona(persona, comuna->personas);
    agregarPersona(persona, destino->personas);
    persona->criminalidad = 0;      //en la comuna nueva empieza de cero, pero sigue marcada

    //se fue con sus oficios
    actualizarServicios(comuna);
    actualizarServicios(destino);

    if(comuna->delJugador || destino->delJugador){
        char mensaje[200];
        snprintf(mensaje, sizeof(mensaje), "%s fue expulsado de la comuna %02d y se fue a la %02d", persona->nombre, comuna->numero, destino->numero);
        registrarEvento(tablero, mensaje);

        printf(" |  !! %s fue expulsado de la comuna %02d y se fue a la comuna %02d !!  |\n\n", persona->nombre, comuna->numero, destino->numero);
    }
}

//cada dia las personas marcadas pueden cometer algo
void eventosCriminales(struct matrizComunas* tablero){
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){

        if(!actual->comuna->disuelta){
            struct persona* persona = actual->comuna->personas->inicio;
            while(persona != NULL){
                struct persona* siguiente = persona->sigt;      //lo guardo por si la expulsan

                if(persona->criminal && randomIntervalo(1, 100) <= PROB_CRIMEN)
                    cometerCrimen(persona, actual, tablero);

                persona = siguiente;
            }
        }
        actual = recorrerMatriz(actual);
    }
}

/*
======================================================================
                            | Logros |
======================================================================
*/

int imprimirLogro(int conseguido, char* nombre, char* descripcion){
    printf(" ║ [%s] %-22s | %s\n", conseguido ? "X" : " ", nombre, descripcion);
    return conseguido;
}

//final=1 cuando ya termino la partida, ahi se revisan los logros que dependen de como quedo todo
int imprimirLogros(struct matrizComunas* tablero, int final){
    struct logros* logros = &tablero->logros;

    int guardian = 0, feliz = 0, alFilo = 0, secreto = 0;

    if(final){
        int mias = contarComunasJugador(tablero);

        guardian = (logros->comunasPerdidas == 0 && mias > 0);
        secreto = (guardian && logros->rachaSinIntercambiar > 10);
        alFilo = (mias > 0);        //se revisan las comunas que maneja en ese momento

        //reviso como quedaron mis comunas
        struct nodoMatriz* actual = tablero->inicio;
        for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
            struct comuna* comuna = actual->comuna;

            if(!comuna->disuelta && comuna->delJugador){
                if(comuna->satisfaccion >= SATISFACCION_LOGRO)
                    feliz = 1;
                if(comuna->necesidad < NECESIDAD_AL_FILO || comuna->satisfaccion > SATISFACCION_AL_FILO)
                    alFilo = 0;
            }
            actual = recorrerMatriz(actual);
        }
    }

    int total = 0;

    printf(" ╔══════════════════════════════════════════════===+- -- -\n");
    printf(" ║ * Logros\n");
    printf(" ║\n");
    total += imprimirLogro(guardian, "Guardian", "Termina el juego sin perder ninguna de tus comunas");
    total += imprimirLogro(logros->intercambios >= 10, "Comerciante de bronce", "Realiza 10 intercambios");
    total += imprimirLogro(logros->intercambios >= 20, "Comerciante de plata", "Realiza 20 intercambios");
    total += imprimirLogro(logros->intercambios >= 50, "Comerciante de oro", "Realiza 50 intercambios");
    total += imprimirLogro(feliz, "Comuna feliz", "Llega al final con una de tus comunas en 90% o más de satisfacción");
    total += imprimirLogro(logros->caos, "Caos total", "Pierde tres comunas tuyas en menos de 3 turnos");
    total += imprimirLogro(alFilo, "Al filo", "Gana con las comunas que manejas al mínimo (casi disueltas)");

    //el secreto solo se muestra si se consiguio
    if(secreto)
        total += imprimirLogro(1, "Autosuficiente", "SECRETO: más de 10 días sin intercambiar y sin perder comunas");
    else
        printf(" ║ [ ] ???                    | Logro secreto\n");

    //el total solo se dice cuando ya termino la partida
    if(final){
        printf(" ║\n");
        printf(" ║ Conseguiste %d de 8\n", total);
    }
    printf(" ╚═════════════════════════════════════════════===+- -\n\n");

    return total;
}

/*
======================================================================
                    | Guardar y cargar partida |
======================================================================
El archivo de guardado es de texto: cada nombre va en su propia linea (porque tienen espacios)
y los numeros van juntos en una linea. Se carga leyendo en el mismo orden en que se guardo
*/

//cantidad de elementos y despues uno por linea
void guardarLista(FILE* archivo, struct listaSimple* lista){
    fprintf(archivo, "%d\n", lista->tamaño);

    struct nodoListaSimple* actual = lista->inicio;
    while(actual != NULL){
        fprintf(archivo, "%s\n", actual->nombre);
        actual = actual->sigt;
    }
}

void guardarDiccionario(FILE* archivo, struct DiccionarioBienes* diccionario){
    fprintf(archivo, "%d %d\n", diccionario->tamaño, contarElementos(diccionario));

    struct nodoDic* casilla = diccionario->inicio;
    while(casilla != NULL){
        struct nodoBienes* bien = casilla->bienes;
        while(bien != NULL){
            fprintf(archivo, "%s\n", bien->nombre);
            fprintf(archivo, "%d %d\n", bien->cantidad, bien->maximo);
            bien = bien->sigt;
        }
        casilla = casilla->sigt;
    }
}

//se guarda en preorden (primero el nodo y despues sus hijos) para poder rearmarlo igualito
void guardarSplay(FILE* archivo, struct nodoSplay* raiz){
    if(raiz == NULL)
        return;

    fprintf(archivo, "%s\n", raiz->titulo);
    fprintf(archivo, "%ld %d %d\n", raiz->tamañoBytes, raiz->lineas, raiz->vecesLeido);

    guardarSplay(archivo, raiz->izq);
    guardarSplay(archivo, raiz->der);
}

int guardarPartida(struct matrizComunas* tablero){
    FILE* archivo = fopen(ARCHIVO_GUARDADO, "w");
    if(archivo == NULL)
        return 0;

    //datos generales
    fprintf(archivo, "%d %d %d %d\n", tablero->tamaño, tablero->dia, tablero->diasTotales, tablero->diasParaEmergencia);

    struct logros* logros = &tablero->logros;
    fprintf(archivo, "%d %d %d %d %d %d %d %d\n", logros->intercambios, logros->intercambioHoy, logros->comunasPerdidas,
            logros->diaPerdida1, logros->diaPerdida2, logros->caos, logros->diasSinIntercambiar, logros->rachaSinIntercambiar);

    guardarLista(archivo, tablero->bienesDisponibles);
    guardarLista(archivo, tablero->serviciosDisponibles);
    guardarLista(archivo, tablero->historial);

    //cada comuna
    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tablero->tamaño * tablero->tamaño; i++){
        struct comuna* comuna = actual->comuna;
        actual = recorrerMatriz(actual);

        fprintf(archivo, "%s\n", comuna->nombre);
        fprintf(archivo, "%d %d %d %d %d %d %d %d %d\n", comuna->numero, comuna->disuelta, comuna->delJugador,
                comuna->diasTormenta, comuna->intercambiosHoy, comuna->regalosDados, comuna->regalosRecibidos,
                comuna->regalosRecientes, comuna->desastre);
        fprintf(archivo, "%lf %lf\n", comuna->satisfaccion, comuna->necesidad);

        //de una comuna disuelta no queda nada mas que guardar
        if(comuna->disuelta)
            continue;

        guardarDiccionario(archivo, comuna->diccionarioBienes);
        guardarDiccionario(archivo, comuna->diccionarioServicios);

        //personas con sus servicios
        fprintf(archivo, "%d\n", comuna->personas->tamaño);
        struct persona* persona = comuna->personas->inicio;
        while(persona != NULL){
            fprintf(archivo, "%s\n", persona->nombre);
            fprintf(archivo, "%d %d %d %d\n", persona->librosLeidos, persona->leyoHoy, persona->criminal, persona->criminalidad);
            guardarLista(archivo, persona->servicios);
            persona = persona->sigt;
        }

        //la cache de la biblioteca
        fprintf(archivo, "%d\n", contarSplay(comuna->biblioteca));
        guardarSplay(archivo, comuna->biblioteca);
    }

    fclose(archivo);
    return 1;
}

//lee una linea del archivo y le quita el salto de linea
void leerLinea(FILE* archivo, char* buffer, int tamaño){
    if(fgets(buffer, tamaño, archivo) == NULL){
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

struct listaSimple* cargarLista(FILE* archivo){
    char linea[512];
    struct listaSimple* lista = crearListaSimple();

    leerLinea(archivo, linea, sizeof(linea));
    int cantidad = atoi(linea);

    //se agregan al final para que queden en el mismo orden
    for(int i = 0; i < cantidad; i++){
        leerLinea(archivo, linea, sizeof(linea));
        agregarAlFinalSimple(crearNodoListaSimple(strdup(linea)), lista);
    }
    return lista;
}

struct DiccionarioBienes* cargarDiccionario(FILE* archivo){
    char linea[512];
    int tamaño = 0, elementos = 0;

    leerLinea(archivo, linea, sizeof(linea));
    sscanf(linea, "%d %d", &tamaño, &elementos);

    struct DiccionarioBienes* diccionario = crearDiccionarioBienes(tamaño);

    for(int i = 0; i < elementos; i++){
        char nombre[512];
        int cantidad = 0, maximo = 0;

        leerLinea(archivo, nombre, sizeof(nombre));
        leerLinea(archivo, linea, sizeof(linea));
        sscanf(linea, "%d %d", &cantidad, &maximo);

        //se vuelve a meter con el mismo hash de siempre
        struct nodoBienes* bien = crearNodoBien(cantidad, nombre);
        bien->maximo = maximo;
        añadirBien(bien, hashDiccionarioBienes(nombre, tamaño), diccionario);
    }
    return diccionario;
}

struct matrizComunas* cargarPartida(){
    FILE* archivo = fopen(ARCHIVO_GUARDADO, "r");
    if(archivo == NULL)
        return NULL;

    char linea[512];
    int tamaño = 0, dia = 0, diasTotales = 0, diasParaEmergencia = 0;

    leerLinea(archivo, linea, sizeof(linea));
    if(sscanf(linea, "%d %d %d %d", &tamaño, &dia, &diasTotales, &diasParaEmergencia) != 4 || tamaño < 3 || tamaño > 7){
        fclose(archivo);
        return NULL;        //el archivo no sirve
    }

    //la matriz se crea igual que en una partida nueva y despues se llena
    struct matrizComunas* tablero = crearMatriz(tamaño);
    tablero->dia = dia;
    tablero->diasTotales = diasTotales;
    tablero->diasParaEmergencia = diasParaEmergencia;

    struct logros* logros = &tablero->logros;
    leerLinea(archivo, linea, sizeof(linea));
    sscanf(linea, "%d %d %d %d %d %d %d %d", &logros->intercambios, &logros->intercambioHoy, &logros->comunasPerdidas,
            &logros->diaPerdida1, &logros->diaPerdida2, &logros->caos, &logros->diasSinIntercambiar, &logros->rachaSinIntercambiar);

    tablero->bienesDisponibles = cargarLista(archivo);
    tablero->serviciosDisponibles = cargarLista(archivo);
    tablero->historial = cargarLista(archivo);

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < tamaño * tamaño; i++){
        struct comuna* comuna = actual->comuna;
        actual = recorrerMatriz(actual);

        leerLinea(archivo, linea, sizeof(linea));
        comuna->nombre = strdup(linea);

        leerLinea(archivo, linea, sizeof(linea));
        sscanf(linea, "%d %d %d %d %d %d %d %d %d", &comuna->numero, &comuna->disuelta, &comuna->delJugador,
                &comuna->diasTormenta, &comuna->intercambiosHoy, &comuna->regalosDados, &comuna->regalosRecibidos,
                &comuna->regalosRecientes, &comuna->desastre);

        leerLinea(archivo, linea, sizeof(linea));
        sscanf(linea, "%lf %lf", &comuna->satisfaccion, &comuna->necesidad);
        comuna->satisfaccionAnterior = comuna->satisfaccion;
        comuna->reciprocidad = calcularReciprocidad(comuna);

        comuna->personas = crearListaPersonas();
        if(comuna->disuelta)
            continue;

        comuna->diccionarioBienes = cargarDiccionario(archivo);
        comuna->diccionarioServicios = cargarDiccionario(archivo);

        //personas
        leerLinea(archivo, linea, sizeof(linea));
        int cantidad = atoi(linea);
        for(int k = 0; k < cantidad; k++){
            leerLinea(archivo, linea, sizeof(linea));
            struct persona* persona = crearPersona(strdup(linea));

            leerLinea(archivo, linea, sizeof(linea));
            sscanf(linea, "%d %d %d %d", &persona->librosLeidos, &persona->leyoHoy, &persona->criminal, &persona->criminalidad);

            free(persona->servicios);
            persona->servicios = cargarLista(archivo);

            agregarPersona(persona, comuna->personas);
        }
        actualizarServicios(comuna);

        //cache de la biblioteca: como viene en preorden, al insertar normal queda con la misma forma
        leerLinea(archivo, linea, sizeof(linea));
        cantidad = atoi(linea);
        for(int k = 0; k < cantidad; k++){
            leerLinea(archivo, linea, sizeof(linea));
            struct nodoSplay* nodo = crearNodoSplay(linea);

            leerLinea(archivo, linea, sizeof(linea));
            sscanf(linea, "%ld %d %d", &nodo->tamañoBytes, &nodo->lineas, &nodo->vecesLeido);

            comuna->biblioteca = insertarSinChaflanear(comuna->biblioteca, nodo);
        }
    }

    fclose(archivo);

    //el catalogo no se guarda, se vuelve a armar con lo que haya en la carpeta
    prepararBiblioteca(tablero);

    return tablero;
}

/*
======================================================================
                    |Final de la partida |
======================================================================
*/

//promedio de satisfaccion y necesidad de las comunas que siguen vivas
void calcularPromedios(struct matrizComunas* tablero, double* satisfaccion, double* necesidad){
    int total = tablero->tamaño * tablero->tamaño;
    double sumaSatisfaccion = 0, sumaNecesidad = 0;
    int vivas = 0;

    struct nodoMatriz* actual = tablero->inicio;
    for(int i = 0; i < total; i++){
        if(!actual->comuna->disuelta){
            sumaSatisfaccion += actual->comuna->satisfaccion;
            sumaNecesidad += actual->comuna->necesidad;
            vivas++;
        }
        actual = recorrerMatriz(actual);
    }

    //si no quedo ninguna, la sociedad quedo lo peor posible
    if(vivas == 0){
        *satisfaccion = 0;
        *necesidad = 100;
        return;
    }

    *satisfaccion = sumaSatisfaccion / vivas;
    *necesidad = sumaNecesidad / vivas;
}

//como quedo la sociedad segun su satisfaccion promedio
const char* clasificarSociedad(double satisfaccion){
    if(satisfaccion < 30) return "Deficiente, como el gobierno de Donald Trump";
    else if(satisfaccion < 50) return "Mala, como el gobierno de Laura Fernandez";
    else if(satisfaccion < 60) return "Normal, como el gobierno de nosemequedesinideas";
    else if(satisfaccion < 70) return "Bien, como el gobierno de Xi Jinping";
    else if(satisfaccion < 90) return "Excelente, como la clase de Aurelio";
    else return "¡¡Sociedad Perfecta!!, como ningun gobierno podria";
}

void mostrarFinPartida(struct matrizComunas* tablero){
    double satisfaccion, necesidad;
    calcularPromedios(tablero, &satisfaccion, &necesidad);

    int vivas = contarComunasVivas(tablero);
    int diasJugados = tablero->dia - 1;

    imprimirTablero(tablero);

    imprimirLento(150,
        "\n___________________________________________________\n"
        " ╔════════════════════════════════════╗\n"
        " ║  !!    Se termino el juego   !!    ║\n"
        " ╚════════════════════════════════════╝\n\n"
    );

    imprimirLento(100,
        " |  Días jugados: %d\n"
        " |  Comunas que sobrevivieron: %d de %d\n"
        " |  Satisfacción promedio: %5.1f\n"
        " |  Necesidad promedio: %5.1f\n\n",
        diasJugados, vivas, tablero->tamaño * tablero->tamaño, satisfaccion, necesidad
    );

    //con menos de 3 comunas la sociedad ya no se sostiene
    if(vivas < COMUNAS_JUGADOR){
        imprimirLento(150,
            " ╔══════════════════════════════════════════════===+- -- -\n"
            " ║ Resultado de la sociedad: FRACASO\n"
            " ║ Quedaron menos de 3 comunas en pie\n"
            " ╚═════════════════════════════════════════════===+- -\n\n"
        );
    }else{
        imprimirLento(150,
            " ╔══════════════════════════════════════════════===+- -- -\n"
            " ║ Resultado de la sociedad: %s\n"
            " ╚═════════════════════════════════════════════===+- -\n\n",
            clasificarSociedad(satisfaccion)
        );
    }

    imprimirLogros(tablero, 1);
}

/*
======================================================================
                            | Main |
======================================================================
*/
int main(){
    srand(time(NULL));

    TituloPrincipal();

    imprimirLento(100,
        " ╔════════════════════════════════════╗\n"
        " ║  * ¿Qué quieres hacer?             ║\n"
        " ╚════════════════════════════════════╝\n"
        " | 1. Empezar una partida de cero     |\n"
        " | 2. Rescatar la partida guardada    |\n\n"
    );

    struct matrizComunas* tablero = NULL;

    if(pedirEnIntervalo(1, 2) == 2){
        tablero = cargarPartida();

        if(tablero == NULL){
            imprimirAdvertencia("No hay ninguna partida guardada | Se empieza una nueva");
        }else{
            printf(" | Partida rescatada | Día %d de %d |\n\n", tablero->dia, tablero->diasTotales);

            //un repaso de lo que habia pasado
            struct nodoListaSimple* evento = tablero->historial->inicio;
            for(int i = 0; i < 8 && evento != NULL; i++){
                printf(" |  - %s\n", evento->nombre);
                evento = evento->sigt;
            }
            printf("\n");
        }
    }

    if(tablero == NULL)
        tablero = crearTablero();

    //se empieza parado en una de las comunas del jugador
    struct nodoMatriz* actual = comunaAlAzar(tablero, 2);

    while(tablero->dia <= tablero->diasTotales && !tablero->salir && contarComunasVivas(tablero) > 0){

        //si la comuna donde estaba se disolvio, me paso a una mia
        if(actual == NULL || actual->comuna->disuelta)
            actual = comunaAlAzar(tablero, 2);

        imprimirTablero(tablero);

        struct nodoMatriz* elegido = pedirComuna(actual, tablero);
        if(elegido == NULL)
            continue;       //paso el dia, vio los logros o guardo

        actual = elegido;
        imprimirComuna(actual, tablero);
    }

    if(tablero->salir){
        imprimirLento(100,
            "\n ╔════════════════════════════════════╗\n"
            " ║  Partida guardada | ¡Hasta luego!  ║\n"
            " ╚════════════════════════════════════╝\n\n"
        );
        return 0;
    }

    mostrarFinPartida(tablero);

    return 0;
}
