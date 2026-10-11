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
 * Función que saca un numero random entre un intervalo
 * @param int min
 * @param int max
 */
int randomIntervalo(int min, int max);

/*
=================================================
        | Funciones Diccionario de Bienes|
=================================================
*/

/**
 * Función que lee el archivo de bienes y los mete a un diccionario
 * @param int cantidad de bienes a agregar
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

//lista simple para guardar los habitantes de una comuna
struct listaPersonas{
    struct nodoListaPersonas* inicio;
};
struct nodoListaPersonas{
    char* nombre;
    struct nodoListaPersonas* sigt;
};

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

    struct DiccionarioBienes* diccionario;

};

/**
 * Función para crear una comuna
 * @param nombre
 * @param int tamaño del diccionario
 */
struct comuna* crearComuna(char* nombre, int tamañoDicc);

/**
 * Función que lee el archivo de comunas y las agrega todas en la matriz 
 * @param int cantidad a agregar
 * @param struct de la matriz donde se agregará
 */
void agregarComunas(int cantidad, struct matrizComunas*);


/*
=================================================
    | Funciones de la matriz de comunas|
=================================================
*/

//matriz que almacena las comunas
struct matrizComunas{
    struct nodoMatriz* inicio;
    int tamaño;
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




