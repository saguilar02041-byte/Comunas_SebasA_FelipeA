//Archivo h
#ifndef COMUNAS_H
#define COMUNAS_H

#include <stdio.h>

/*
=================================================
        | Estructuras de datos |
=================================================
*/

//Diccionario que guarda los bienes para cada comuna
struct nodoBienes{
    int cantidad;
    int maximo;

    char* nombre;
    struct nodoBienes* sigt;
};
struct nodoDic{
    struct nodoDic* sigt;
    struct nodoBienes* bienes;
};
struct DiccionarioBienes{
    struct nodoDic* inicio;
    int tamaño;
};

struct nodoListaSimple{
    char* nombre;
    struct nodoListaSimple* sigt;
};
struct listaSimple{
    struct nodoListaSimple* inicio;
    int tamaño;
};

struct nodoPersona{
    char* nombre;
    struct listaSimple* servicios;  //los servicios que sabe hacer

    int librosLeidos;               //a los 3 libros aprende un servicio 
    int leyoHoy;
    int seleccionada;

    int criminal;                   //si es criminal :v
    int criminalidad;               //porcentaje, al llegar a 55 lo sacan

    struct nodoPersona* sigt;
};
struct listaPersonas{
    struct nodoPersona* inicio;
    int tamaño;
};


struct nodoAVL{
    char* titulo;
    char* ruta;

    int altura;

    struct nodoAVL* izq;
    struct nodoAVL* der;
};

//arbol chaflaneado
struct nodoChaf{
    char* titulo;
    long tamañoBytes;
    int lineas;
    int vecesLeido;

    struct nodoChaf* izq;
    struct nodoChaf* der;
};

//struct de las comunas
struct comuna{
    char* nombre;
    int numero;

    double necesidad;
    double satisfaccion;
    double satisfaccionAnterior;    //la del calculo pasado, la usan los vecinos
    double reciprocidad;

    struct DiccionarioBienes* diccionarioBienes;
    struct DiccionarioBienes* diccionarioServicios;

    struct listaPersonas* personas;
    struct nodoChaf* biblioteca;

    int diasTormenta;
    int intercambiosHoy;

    int disuelta;               //booleano
    int delJugador;             //booleano

    int regalosDados;
    int regalosRecibidos;
    int regalosRecientes;
    
    int desastre;
};

//lo que se ocupa llevar para los logros
struct logros{
    int intercambios;           //intercambios hechos o aceptados por el jugador
    int intercambioHoy;

    int comunasPerdidas;        //comunas que se han disuelto
    int diaPerdida1;            //dia de la ultima comuna perdida
    int diaPerdida2;            //dia de la penultima

    int caos;                   //booleano si perdio tres comunas en menos de 3 turnos
    int diasSinIntercambiar;
    int rachaSinIntercambiar;   //la racha mas larga
};

//matriz que almacena las comunas
struct matrizComunas{
    struct nodoMatriz* inicio;
    int tamaño;

    struct listaSimple* bienesDisponibles;
    struct listaSimple* serviciosDisponibles;

    int dia;
    int diasTotales;
    int diasParaEmergencia;

    struct nodoAVL* catalogo;   //todos los libros de la carpeta
    int totalLibros;

    struct listaSimple* historial;  //lo importante que ha pasado en la partida
    struct logros logros;
    int salir;
};

//nodos de la matriz de comunas
struct nodoMatriz{
    struct nodoMatriz* arriba;
    struct nodoMatriz* abajo;
    struct nodoMatriz* izquierda;
    struct nodoMatriz* derecha;

    struct comuna* comuna;
};

/*
=================================================
        | Funciones utiles|
=================================================
*/

/**
 * Función que imprime linea por linea con una pausa
 */
void imprimirLento(int retraso_ms, const char* formato, ...);

/**
 * Función que imprime el título del jueguito
 */
void TituloPrincipal();

/**
 * Función que pasa un texto a minúsculas
 */
void convertirMinusculas(char* str);

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
void agregarBienesDiccionario(struct listaSimple* bienesDisponibles, struct DiccionarioBienes* diccionario, int personas);

/**
 * Función de hash del diccionario de bienes
 * @param char*
 */
int hashDiccionarioBienes(char* texto, int tamaño);

/**
 * Función uqe imprime el diccionario de bienes
 */
void imprimirDiccionarioBienes(struct DiccionarioBienes* diccionario);

/**
 * Función que crea un diccionario de bienes
 * @param int tamaño del diccionario
 */
struct DiccionarioBienes* crearDiccionarioBienes(int tamaño);

/**
 * Función que crea un nodo con un bien y su respectiva cantidad
 * @param int cantidad
 * @param char* nombre
 */
struct nodoBienes* crearNodoBien(int cantidad, char* nombre);

/**
 * Función que crea y añade un elemento al diccionario de bienes
 */
void añadirAlDiccionario(struct DiccionarioBienes* diccionario);

/**
 * Función que añade un nodoBienes al diccionario
 * @param nodo del bien
 * @param int posición
 * @param diccionario a añadir
 */
void añadirBien(struct nodoBienes* bien, int pos, struct DiccionarioBienes* diccionario);

/**
 * Función que busca un bien en el diccionario a partir del nombre
 * @param diccionario
 * @param char* nombre del bien a buscar
 */
struct nodoBienes* buscarBien(struct DiccionarioBienes* diccionario, char* nombre);

/**
 * Función que libera un diccionario completo (cuando la comuna se disuelve)
 */
void liberarDiccionario(struct DiccionarioBienes* diccionario);

/*
=================================================
    | Funciones de la lista simple |
=================================================
*/

struct listaSimple* crearListaSimple();
struct nodoListaSimple* crearNodoListaSimple(char* nombre);

/**
 * Función que añade un nodo al inicio de la lista
 */
void agregarNodoSimple(struct nodoListaSimple* nodo, struct listaSimple* lista);

/**
 * Función que añade un nodo al final de la lista
 */
void agregarAlFinalSimple(struct nodoListaSimple* nodo, struct listaSimple* lista);

/**
 * Función que elimina el primero de una lista
 */
void eliminarInicioSimple(struct listaSimple* lista);

void imprimirListaSimple(struct listaSimple* lista);

/**
 * Función que dice si un nombre está en la lista, 1=sí 0=no
 */
int estaEnLista(struct listaSimple* lista, char* nombre);

/**
 * Función que retorna el nombre en la posición dada
 */
char* nombreEnPosicion(struct listaSimple* lista, int posicion);

/*
=================================================
    | Funciones de las personas |
=================================================
*/

/**
 * Función que crea una lista de personas
 */
struct listaPersonas* crearListaPersonas();

/**
 * Función que crea una personita
 */
struct nodoPersona* crearPersona(char* nombre);

/**
 * Función que agrega una persona a la lista de personas
 */
void agregarPersona(struct nodoPersona* persona, struct listaPersonas* lista);

/**
 * Función que quita una persona de su lista
 */
void sacarPersona(struct nodoPersona* persona, struct listaPersonas* lista);

/**
 * Función que retorna la persona en una posición
 */
struct nodoPersona* obtenerPersona(struct listaPersonas* lista, int posicion);

/**
 * Función que le agrega un servicio a una persona
 */
void darServicio(struct nodoPersona* persona, char* servicio);

/**
 * Función que le reparte 0, 1 o más servicios al azar a cada persona
 */
void repartirServicios(struct matrizComunas* tablero);

/**
 * Función que cuenta cuántas personas de la comuna saben cada servicio
 * (en el diccionario de servicios cantidad = personas que lo saben, maximo = personas que se ocupan)
 */
void actualizarServicios(struct comuna* comuna);

/**
 * Función que imprime los servicios con la cantidad de personas que los saben
 * @param int
 */
void imprimirServicios(struct DiccionarioBienes* diccionario, int numerado);

/**
 * Función que saca una cantidad de personas que tengan dicho servicio
 * @return lista con las personas que salieron
 */
struct listaPersonas* sacarPersonasConServicio(struct comuna* comuna, char* servicio, int cantidad);

/**
 * Función que pasa todas las personas de una lista a la comuna destino
 */
void pasarPersonas(struct listaPersonas* personas, struct comuna* destino);

/**
 * Función que marca de 2 a 5 personas por comuna como las que pueden cometer crímenes
 */
void marcarCriminales(struct matrizComunas* tablero);

/*
=================================================
        | Funciones de las comunas |
=================================================
*/

struct comuna* crearComuna();

/*
=================================================
    | Funciones de la matriz de comunas|
=================================================
*/

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

struct nodoMatriz* crearNodoMatriz();

/**
 * Función que ayuda a recorrer la matriz, retorna el nodo de la derecha, si no hay se devuelve toda la fila y retorna el de abajo
 */
struct nodoMatriz* recorrerMatriz(struct nodoMatriz* actual);

/**
 * Función que recorre la matriz y retorna el nodo de matriz en la posición i,j
 */
struct nodoMatriz* encontrarNodoMatriz(int i, int j, struct matrizComunas* tablero);

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

int preguntarDimensión();
int preguntarCantidadBienes();
int preguntarCantidadServicios();

/**
 * Función que pregunta al usuario cuantos días quiere jugar
 * @return int 
 */
int preguntarCantidadDias();

void meterDiccionarios(struct listaSimple* serviciosDisponibles, struct listaSimple* bienesDisponibles, struct matrizComunas* tablero);
void meterNumeros(struct matrizComunas* tablero);
void meterNombres(struct listaSimple* nombresComunas, struct matrizComunas* tablero);
void meterPersonas(struct listaSimple* personasDisponibles, struct matrizComunas* tablero);

/*
=================================================
    | Funciones de los índices |
=================================================
*/

void sumarFaltantes(struct DiccionarioBienes* diccionario, int* faltante, int* maximo);


double calcularNecesidad(struct comuna* comuna);

double calcularReciprocidad(struct comuna* comuna);

/**
 * Qué tantos regalos ha recibido la comuna últimamente
 */
double calcularNivelRegalos(struct comuna* comuna);

void sumarVecino(struct nodoMatriz* vecino, double* sumaNecesidad, double* sumaSatisfaccion, int* cuenta);
void sumarVecinos(struct nodoMatriz* nodo, double* sumaNecesidad, double* sumaSatisfaccion, int* cuenta);

/**
 * Satisfacción: necesidad local + necesidad y satisfacción de los vecinos + reciprocidad + regalos
 */
void calcularIndices(struct nodoMatriz* nodo);
void calcularIndicesTablero(struct matrizComunas* tablero);

/*
=================================================
    | Funciones para imprimir el tablero |
=================================================
*/

void imprimirResumenTablero(struct matrizComunas* tablero);
void imprimirLineaFila(struct nodoMatriz* inicioFila, int linea);
void imprimirTablero(struct matrizComunas* tablero);

void imprimirNumero(struct comuna* comuna);
void imprimirSatisfaccion(struct comuna* comuna);
void imprimirNecesidad(struct comuna* comuna);
void imprimirRelleno();

/**
 * Función que imprime una línea del cuadro vacío que deja una comuna disuelta
 */
void imprimirDisuelta(int linea);

void imprimirPersonas(struct listaPersonas* personas, char* nombre);

void imprimirComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero);

/**
 * Menú principal del día, retorna NULL si no se escogió una comuna
 */
struct nodoMatriz* pedirComuna(struct nodoMatriz* actual, struct matrizComunas* tablero);

/**
 * Función que pide el número de una comuna
 * @param int soloJugador 1=solo acepta comunas del jugador, 0=cualquiera que siga viva
 */
struct nodoMatriz* buscarComuna(struct matrizComunas* tablero, int soloJugador);

/*
=================================================
    | Funciones de los días y consumo |
=================================================
*/

/**
 * Función que baja (y a veces sube) de forma aleatoria cada bien de un diccionario
 */
void consumirDiccionario(struct DiccionarioBienes* diccionario);

/**
 * Función que termina el día
 */
void pasarDia(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de las emergencias |
=================================================
*/

void escogerEmergencias(struct matrizComunas* tablero);
void lanzarEmergencia(struct matrizComunas* tablero);

/**
 * Función que baja de forma aleatoria algunos elementos de un diccionario
 * @param int nivel 0=nada, 1=leve, 2=media, 3=fuerte
 */
void reducirDiccionario(struct DiccionarioBienes* diccionario, int nivel);

void afectarComuna(struct comuna* comuna, int nivelBienes, int nivelServicios);
void afectarVecino(struct nodoMatriz* vecino, int nivelBienes, int nivelServicios);
void afectarVecinos(struct nodoMatriz* nodo, int nivelBienes, int nivelServicios);

/**
 * Función que imprime el aviso de una emergencia
 * @param comuna afectada 
 */
void imprimirAvisoEmergencia(char* tipo, struct comuna* comuna, char* descripcion);

void emergenciaVirus(struct nodoMatriz* nodo);          //su comuna fuerte y un poco las adyacentes
void emergenciaInundacion(struct nodoMatriz* nodo);     //solo su comuna (bienes)
void emergenciaApagon(struct nodoMatriz* nodo);         //solo su comuna (servicios)
void emergenciaTerremoto(struct matrizComunas* tablero);//todas las comunas
void emergenciaTsunami(struct nodoMatriz* nodo);        //una fila o columna completa
void emergenciaTormenta(struct nodoMatriz* nodo);       //muy poco, pero dura de 1 a 3 días

void continuarTormentas(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de los intercambios |
=================================================
*/

struct nodoBienes* buscarBienExacto(struct DiccionarioBienes* diccionario, char* nombre);
int contarElementos(struct DiccionarioBienes* diccionario);
struct nodoBienes* obtenerElemento(struct DiccionarioBienes* diccionario, int posicion);
void imprimirDiccionarioNumerado(struct DiccionarioBienes* diccionario);
void imprimirAdvertencia(char* mensaje);

/**
 * Función que pregunta si quiere un bien o un servicio y cuál de la lista
 * @param int* esServicio queda en 0 si es bien y 1 si es servicio
 * @return el elemento escogido, NULL si cancela
 */
struct nodoBienes* escogerProducto(struct comuna* comuna, int* esServicio);

/**
 * Función que mueve los productos entre dos comunas
 * @return si se pudo o no
 */
int aplicarIntercambio(struct comuna* comunaA, struct nodoBienes* productoA, int cantidadA, int servicioA,
                    struct comuna* comunaB, struct nodoBienes* productoB, int cantidadB, int servicioB,
                    int* realA, int* realB);

/**
 * Función que hace todo el proceso de intercambio del jugador
 * @param nodo de la comuna del jugador desde donde se intercambia
 */
void realizarIntercambio(struct nodoMatriz* nodoA, struct matrizComunas* tablero);

/*
=================================================
    | Funciones de las comunas autónomas |
=================================================
*/

/**
 * Funciones que buscan lo que más le falta y lo que más le sobra a un diccionario
 */
struct nodoBienes* masEscaso(struct DiccionarioBienes* diccionario);
struct nodoBienes* masAbundante(struct DiccionarioBienes* diccionario);

/**
 * Función que arma la propuesta de una comuna: qué pide de la otra y qué da a cambio
 * @return 1 si hay propuesta, 0 si no
 */
int armarPropuesta(struct comuna* pide, struct comuna* otra, struct nodoBienes** productoDa, int* cantidadDa, int* servicioDa,
                struct nodoBienes** productoPide, int* cantidadPide, int* servicioPide);

/**
 * Función donde una comuna autónoma busca otra autónoma con quien intercambiar
 */
void buscarIntercambio(struct nodoMatriz* nodoA, struct matrizComunas* tablero);

/**
 * Función donde las comunas autónomas intercambian entre ellas sin imprimir nada
 */
void intercambiosAutonomos(struct matrizComunas* tablero);

/**
 * Función donde alguna comuna autónoma le pide un intercambio al jugador
 */
void solicitudesAutonomas(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de la disolución y el jugador |
=================================================
*/

int contarComunasVivas(struct matrizComunas* tablero);
int contarComunasJugador(struct matrizComunas* tablero);

/**
 * Función que retorna una comuna viva al azar
 * @param int tipo 0=cualquiera, 1=solo autónomas, 2=solo del jugador
 */
struct nodoMatriz* comunaAlAzar(struct matrizComunas* tablero, int tipo);

/**
 * Función que llena el arreglo con los vecinos que siguen vivos y retorna cuántos son
 */
int obtenerVecinosVivos(struct nodoMatriz* nodo, struct nodoMatriz* vecinos[8]);

/**
 * Función que sube los máximos de los bienes de una comuna cuando le llega gente nueva
 */
void ajustarMaximos(struct comuna* comuna, int personasAntes);

/**
 * Función que le completa al jugador sus 3 comunas
 */
void asignarComunasJugador(struct matrizComunas* tablero);

void disolverComuna(struct nodoMatriz* nodo, struct matrizComunas* tablero);
void revisarDisoluciones(struct matrizComunas* tablero);

/**
 * Función que guarda en el historial algo que pasó
 */
void registrarEvento(struct matrizComunas* tablero, char* texto);

/*
=================================================
    | Funciones del árbol AVL |
=================================================
*/

int alturaAVL(struct nodoAVL* nodo);
struct nodoAVL* crearNodoAVL(char* titulo, char* ruta);
struct nodoAVL* rotarDerechaAVL(struct nodoAVL* nodo);
struct nodoAVL* rotarIzquierdaAVL(struct nodoAVL* nodo);
struct nodoAVL* insertarAVL(struct nodoAVL* raiz, char* titulo, char* ruta);

/**
 * Búsqueda por nombre exacto del libro
 */
struct nodoAVL* buscarAVL(struct nodoAVL* raiz, char* titulo);

struct nodoAVL* obtenerAVL(struct nodoAVL* raiz, int posicion, int* contador);

/**
 * Función que lee la carpeta de libros y mete todos los títulos al AVL
 */
struct nodoAVL* cargarCatalogo(char* carpeta, struct nodoAVL* raiz, int* total);
void prepararBiblioteca(struct matrizComunas* tablero);
void imprimirCatalogo(struct nodoAVL* raiz, int* contador, int limite);

/*
=================================================
    | Funciones del árbol chaflaneado |
=================================================
*/

struct nodoChaf* crearNodoSplay(char* titulo);
struct nodoChaf* rotarDerechaChaf(struct nodoChaf* nodo);
struct nodoChaf* rotarIzquierdaChaf(struct nodoChaf* nodo);

/**
 * Función que chaflanea :vvvvv
 */
struct nodoChaf* chaflanear(struct nodoChaf* raiz, char* titulo);

/**
 * Función que inserta un libro nuevo y lo deja en la raíz
 */
struct nodoChaf* insertarChaf(struct nodoChaf* raiz, struct nodoChaf* nuevo);

/**
 * Inserción normal de árbol binario, solo se usa al cargar una partida para dejar el árbol igual
 */
struct nodoChaf* insertarSinChaflanear(struct nodoChaf* raiz, struct nodoChaf* nuevo);

int contarSplay(struct nodoChaf* raiz);
struct nodoChaf* obtenerSplay(struct nodoChaf* raiz, int posicion, int* contador);
void liberarSplay(struct nodoChaf* raiz);
void imprimirArbolChaf(struct nodoChaf* raiz);

/*
=================================================
    | Funciones de la biblioteca |
=================================================
*/

/**
 * Función que abre el archivo del libro y lo lee completo
 * @return 1 si lo pudo leer
 */
int cargarLibro(char* ruta, long* bytes, int* lineas);

/**
 * Función donde una persona va a la biblioteca de su comuna y lee un libro
 */
void leerLibro(struct matrizComunas* tablero, struct comuna* comuna, struct nodoPersona* persona);

void aprenderServicio(struct matrizComunas* tablero, struct nodoPersona* persona);
void enviarALeer(struct comuna* comuna, struct matrizComunas* tablero);
void menuBiblioteca(struct comuna* comuna, struct matrizComunas* tablero);

/*
=================================================
    | Funciones de la criminalidad |
=================================================
*/

void cometerCrimen(struct nodoPersona* persona, struct nodoMatriz* nodo, struct matrizComunas* tablero);
void eventosCriminales(struct matrizComunas* tablero);

/*
=================================================
    | Funciones de los logros |
=================================================
*/

/**
 * Función que imprime un logro y retorna 1 si está conseguido
 */
int imprimirLogro(int conseguido, char* nombre, char* descripcion);

/**
 * Función que imprime todos los logros
 * @param int 1 si ya terminó la partida
 * @return cuántos se consiguieron
 */
int imprimirLogros(struct matrizComunas* tablero, int final);

/*
=================================================
    | Funciones de guardar y cargar |
=================================================
*/

void guardarDiccionario(FILE* archivo, struct DiccionarioBienes* diccionario);
void guardarChaflaneado(FILE* archivo, struct nodoChaf* raiz);
void guardarLista(FILE* archivo, struct listaSimple* lista);

/**
 * Función que escribe toda la partida en el archivo de guardado
 * @return 1 si se pudo guardar
 */
int guardarPartida(struct matrizComunas* tablero);

void leerLinea(FILE* archivo, char* buffer, int tamaño);
struct listaSimple* cargarLista(FILE* archivo);
struct DiccionarioBienes* cargarDiccionario(FILE* archivo);

/**
 * Función que arma el tablero a partir del archivo de guardado
 * @return NULL si no hay partida guardada
 */
struct matrizComunas* cargarPartida();

/*
=================================================
    | Funciones del final de la partida |
=================================================
*/

void calcularPromedios(struct matrizComunas* tablero, double* satisfaccion, double* necesidad);
const char* clasificarSociedad(double satisfaccion);
void mostrarFinPartida(struct matrizComunas* tablero);

#endif
