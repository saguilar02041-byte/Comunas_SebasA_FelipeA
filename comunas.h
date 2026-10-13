//Archivo h

/**
 * Función que imprime el título del jueguito
 */
void TituloPrincipal();

/*
=================================================
                | Funciones utiles|
=================================================
*/

/**
 * Función que saca nombres random de un archivo
 * @param char* nombre del archivo
 * @param int cantidad de elementos a sacar
 * @return lista simple con los elementos, NULL no hay archivo
 */
struct listaSimple* extraerRandomArchivo(char* nombreArchivo, int cantidad);

/**
 * Función que saca un numero random entre un intervalo
 * @param int min
 * @param int max
 */
int randomIntervalo(int min, int max);


/**
* Función que pide un int al usuario
* @return int
*/
int pedirInt();

/**
 * Función que pide un valor entre un minimo y un maximo
 */
int pedirEnIntervalo(int min, int max);


/*
=================================================
        | Funciones Diccionario de Bienes|
=================================================
*/

/**
 * Función que le agrega los bienes a 1 diccionario
 */
void agregarBienesDiccionario();

/**
 * Función de hash del diccionario de bienes
 * @param char*
 */
int hashDiccionarioBienes();


/**
 * Función uqe imprime el diccionario de bienes
 */
void imprimirDiccionarioBienes();

//Diccionario que guarda los bienes para cada comuna
struct DiccionarioBienes{
    struct nodoDic* inicio;
    int tamaño;
};
struct nodoDic{
    struct nodoDic* sigt;
    struct nodoBienes* bienes;
};
struct nodoBienes{
    int cantidad;
    int maximo;

    char* nombre;
    struct nodoBienes* sigt;
};

/**
 * Función que crea un diccionario de bienes
 * @param int tamaño del diccionario
 */
struct DiccionarioBienes* crearDiccionarioBienes();

/**
 * Función que crea un nodo con un bien y su respectiva cantidad 
 * @param int cantidad
 * @param char* nombre
 */
struct nodoBienes* crearNodoBien(int cantidad, char* nombre);

/**
 * Función que crea y añade un elemento al diccionario de bienes 
 */
void añadirAlDiccionario();

/**
 * Función que crea y añade un nodoBienes al diccionario
 * @param char* nombre del bien
 * @param int posición
 * @param diccionario a añadir
 */
void añadirBien();

/**
 * Función que busca un bien en el diccionario a partir del nombre
 * @param diccionario
 * @param char* nombre del bien a buscar
 */
struct nodoBienes* buscarBien(struct DiccionarioBienes* diccionario, char* nombre);

/*
=================================================
    | Funciones de la lista de Personas|
=================================================
*/

//lista simple para guardar los habitantes de una comuna y los servicios
struct listaSimple{
    struct nodoListaSimple* inicio;
    int tamaño;
};
struct nodoListaSimple{
    char* nombre;
    struct nodoListaSimple* sigt;
};

/**
 * Función que crea una lista de personas/servicios para cada comuna
 */
struct listaSimple* crearListaSimple();

/**
 * Función que crea un nodo de las lista de personas/servicios y le agrega el nombre
 */
struct nodoListaSimple* crearNodoListaSimple(char* nombre);

/**
 * Función que añade un nodo a la lista de personas/servicios
 */
void agregarNodoSimple(struct nodoListaSimple* nodo, struct listaSimple* lista);

/**
 * Función que elimina la primer persona/servicios de una lista
 */
void eliminarInicioSimple(struct listaSimple* lista);

/**
 * Función que imprime una lista simple
 */
void imprimirListaSimple(struct listaSimple* lista);

/*
=================================================
    | Funciones de la lista de servicios|
=================================================
*/

//lista enlazada simple de los servicios
//luego hacer esto

/*
=================================================
        | Funciones de las comunas |
=================================================
*/

//struct de las comunas
struct comuna{
    char* nombre;
    int numero;

    double necesidad;
    double satisfaccion;

    struct DiccionarioBienes* diccionarioBienes;
    struct DiccionarioBienes* diccionarioServicios;

    struct listaSimple* personas;

};

/**
 * Función para crear una comuna
 */
struct comuna* crearComuna();

/**
 * Función que lee el archivo de comunas y las agrega todas en la matriz 
 * @param struct de la matriz donde se agregará
 */


/*
=================================================
    | Funciones de la matriz de comunas|
=================================================
*/

//matriz que almacena las comunas
struct matrizComunas{
    struct nodoMatriz* inicio;
    int tamaño;
    
    struct listaSimple* bienesDisponibles;
    struct listaSimple* serviciosDisponibles;
};
//nodos de la matriz de comunas
struct nodoMatriz{
    struct nodoMatriz* arriba;
    struct nodoMatriz* abajo;
    struct nodoMatriz* izquierda;
    struct nodoMatriz* derecha;

    struct comuna* comuna;
};

/**
 * Función que crea un nodo de matriz a la derecha y lo enlaza
 * @return int 0=no lo logró, 1=sí
 */
int enlazarDerecha(struct nodoMatriz* nodo);

/**
 * Función que crea un nodo de matriz abajo y lo enlaza
 * @return int 0=no lo logró, 1=sí
 */
int enlazarAbajo(struct nodoMatriz* nodo);

/**
 * Función que anlaza dos nodos, el que tiene abajo con el que debería de tener a su derecha
 * @return int 0=no lo logró, 1=sí
 */
int enlazarNodosAbajo(struct nodoMatriz* nodo);

/**
 * Función que crea la matriz de comuna
 * @param tamaño
 */
struct matrizComunas* crearMatriz(int tamaño);

/**
 * Función que crea un nodo de una matriz
 */
struct nodoMatriz* crearNodoMatriz();

/**
 * Función que ayuda a recorrer la matriz, retorna el nodo de la derecha, si no hay se devuelve toda la fila y retorna el de abajo
 */
struct nodoMatriz* recorrerMatriz(struct nodoMatriz*);

/*
=================================================
        | Funciones del tablero |
=================================================
*/

/**
 * Función que pregunta todo para crear un tablero y empezar a jugar
 * @return matriz con todas las comunas ya puestas
 */
struct matrizComunas* crearTablero();

/**
 * Función que pregunta al usario el tamaño del tablero
 * @return int
 */
int preguntarDimensión();

/**
 * Función que pregunta al usario cuantos bienes quiere utilizar en el juego
 * @return int
 */
int preguntarCantidadBienes();

/**
 * Función que pregunta al usuario cuantos servicios quiere utilizar en el juego
 * @return int
 */
int preguntarCantidadServicios();

/**
 * Función que le crea y agrega los diccionarios a todas las comunas
 */
void meterDiccionarios(struct listaSimple* serviciosDisponibles, struct listaSimple* bienesDisponibles, struct matrizComunas* tablero);

/**
 * Función que le agrega un numero a cada comuna
 */
void meterNumeros(struct matrizComunas* tablero);

/**
 * Función que le da nombres a todas las comunas
 */
void meterNombres(struct listaSimple* nombresComunas, struct matrizComunas* tablero);

/**
 * Función que le mete todas las personas a un tablero
 */
void meterPersonas(struct listaSimple* personasDisponibles, struct matrizComunas* tablero);

/*
=================================================
    | Funciones que ayudan a hacer calculos |
=================================================
*/

void sumarFaltantes(struct DiccionarioBienes* diccionario, int* faltante, int* maximo);

double calcularNecesidad(struct comuna* comuna);

void sumarVecino(struct nodoMatriz* vecino, double* suma, int* cuenta);

void sumarVecinos(struct nodoMatriz* nodo, double* suma, int* cuenta);

void calcularIndices(struct nodoMatriz* nodo);

void calcularIndicesTablero(struct matrizComunas* tablero);

/*
=================================================
    | Funciones para imprimir el tablero |
=================================================
*/

/**
 * Función que imprime el resumen del tablero
 */
void imprimirResumenTablero(struct matrizComunas* tablero);

/**
 * Funciçon qu eimprime una linea del tablero
 */
void imprimirLineaFila(struct nodoMatriz* inicioFila, int linea);

/**
 * Función que imprime todo el tablero
 */
void imprimirTablero(struct matrizComunas* tablero);

void imprimirNumero(struct comuna* comuna);
void imprimirSatisfaccion(struct comuna* comuna);
void imprimirNecesidad(struct comuna* comuna);
void imprimirRelleno();













