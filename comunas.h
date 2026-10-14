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

    int diasTormenta;           //CAMBIO: dias que le quedan a una tormenta que se origino en esta comuna
    int intercambiosHoy;        //CAMBIO: intercambios que ha hecho la comuna en el dia actual

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

    int dia;                    //CAMBIO: dia actual de la partida (empieza en 1)
    int diasTotales;            //CAMBIO: cantidad de dias que escogio jugar el usuario
    int diasParaEmergencia;     //CAMBIO: cuantos dias faltan para la siguiente emergencia
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
 * CAMBIO: Función que pregunta al usuario cuantos días (turnos) quiere jugar
 * @return int entre 4 y 99
 */
int preguntarCantidadDias();

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

void imprimirPersonas(struct listaSimple* personas, char* nombre);

/**
 * CAMBIO: ahora también recibe el tablero (lo ocupa el menú de intercambiar)
 */
void imprimirComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero);

struct nodoMatriz* pedirComuna(struct nodoMatriz* actual, struct matrizComunas* tablero);
struct nodoMatriz* buscarComuna(struct matrizComunas* tablero);

/**
 * Función que recorre la matriz y retorna el nodo de matriz en la posición i,j
 * @param int i
 * @param int j
 * @param tablero
 */
struct nodoMatriz* encontrarNodoMatriz(int i, int j, struct matrizComunas* tablero);

/*
=================================================
    | Funciones de los días y consumo |
=================================================
*/

/**
 * Función que baja (y a veces sube) de forma aleatoria cada bien/servicio de un diccionario
 * @param diccionario de bienes o de servicios de una comuna
 */
void consumirDiccionario(struct DiccionarioBienes* diccionario);

/**
 * Función que termina el día: consumo, tormentas, emergencias, recalcula índices y sube el día
 * @param tablero
 */
void pasarDia(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de las emergencias |
=================================================
*/

/**
 * Función que dice si hoy hay emergencias y, si las hay, cuántas y de qué tipo
 * (cada 1 a 3 días, y entre más grande el tablero más emergencias a la vez)
 * @param tablero
 */
void escogerEmergencias(struct matrizComunas* tablero);

/**
 * Función que escoge un tipo de emergencia y una comuna al azar y la aplica
 */
void lanzarEmergencia(struct matrizComunas* tablero);

/**
 * Función que baja de forma aleatoria algunos elementos de un diccionario
 * @param int nivel 0=nada, 1=leve, 2=media, 3=fuerte
 */
void reducirDiccionario(struct DiccionarioBienes* diccionario, int nivel);

/**
 * Función que le baja bienes y servicios a una comuna
 */
void afectarComuna(struct comuna* comuna, int nivelBienes, int nivelServicios);

/**
 * Funciones que afectan a las comunas que rodean un nodo (las mismas 8 que usa sumarVecinos)
 */
void afectarVecino(struct nodoMatriz* vecino, int nivelBienes, int nivelServicios);
void afectarVecinos(struct nodoMatriz* nodo, int nivelBienes, int nivelServicios);

/**
 * Función que imprime el aviso de una emergencia
 * @param comuna afectada (NULL si no es una sola)
 */
void imprimirAvisoEmergencia(char* tipo, struct comuna* comuna, char* descripcion);

/**
 * Una función por cada emergencia
 */
void emergenciaVirus(struct nodoMatriz* nodo);          //su comuna fuerte y un poco las adyacentes
void emergenciaInundacion(struct nodoMatriz* nodo);     //solo su comuna (bienes)
void emergenciaApagon(struct nodoMatriz* nodo);         //solo su comuna (servicios)
void emergenciaTerremoto(struct matrizComunas* tablero);//todas las comunas
void emergenciaTsunami(struct nodoMatriz* nodo);        //una fila o columna completa
void emergenciaTormenta(struct nodoMatriz* nodo);       //muy poco, pero dura de 1 a 3 días

/**
 * Función que aplica el día de las tormentas que siguen activas
 */
void continuarTormentas(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de los intercambios |
=================================================
*/

/**
 * Función que busca un bien/servicio por su nombre exacto (sin pasarlo a minúscula como buscarBien)
 */
struct nodoBienes* buscarBienExacto(struct DiccionarioBienes* diccionario, char* nombre);

/**
 * Función que cuenta cuantos elementos tiene un diccionario
 */
int contarElementos(struct DiccionarioBienes* diccionario);

/**
 * Función que devuelve el elemento número 'posicion' (empezando en 1) de un diccionario, en el mismo orden en que se imprime
 */
struct nodoBienes* obtenerElemento(struct DiccionarioBienes* diccionario, int posicion);

/**
 * Función que imprime el diccionario con un número al lado de cada elemento
 */
void imprimirDiccionarioNumerado(struct DiccionarioBienes* diccionario);

/**
 * Función que imprime un aviso de error/advertencia con el mismo formato en todo el juego
 */
void imprimirAdvertencia(char* mensaje);

/**
 * Función que pregunta si quiere un bien o un servicio y cuál de la lista
 * @param int* esServicio queda en 0 si es bien y 1 si es servicio
 * @return el elemento escogido, NULL si cancela
 */
struct nodoBienes* escogerProducto(struct comuna* comuna, int* esServicio);

/**
 * Función que hace todo el proceso de intercambio entre dos comunas
 */
void realizarIntercambio(struct matrizComunas* tablero);

/*
=================================================
    | Funciones del final de la partida |
=================================================
*/

/**
 * Función que saca el promedio de satisfacción y de necesidad de todo el tablero
 */
void calcularPromedios(struct matrizComunas* tablero, double* satisfaccion, double* necesidad);

/**
 * Función que dice como quedó la sociedad según su satisfacción promedio
 */
const char* clasificarSociedad(double satisfaccion);

/**
 * Función que muestra el mensaje final con los promedios
 */
void mostrarFinPartida(struct matrizComunas* tablero);
