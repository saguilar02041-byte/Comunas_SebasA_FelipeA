//Archivo h
#ifndef COMUNAS_H
#define COMUNAS_H

#include <stdio.h>

/*
=================================================
        | Estructuras de datos |
=================================================
Van todas arriba para que las funciones de abajo ya las conozcan
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

//lista simple para guardar nombres (bienes, servicios, historial)
struct nodoListaSimple{
    char* nombre;
    struct nodoListaSimple* sigt;
};
struct listaSimple{
    struct nodoListaSimple* inicio;
    int tamaño;
};

//persona: ahora los servicios son de la persona y no de la comuna
struct persona{
    char* nombre;
    struct listaSimple* servicios;  //lo que sabe hacer (puede estar vacia)

    int librosLeidos;               //al llegar a 3 aprende un servicio
    int leyoHoy;                    //solo se puede ir una vez por dia a la biblioteca
    int seleccionada;               //para escogerla en el menu de la biblioteca

    int criminal;                   //1 si es de las personas marcadas
    int criminalidad;               //porcentaje, al llegar a 55 la expulsan

    struct persona* sigt;
};
struct listaPersonas{
    struct persona* inicio;
    int tamaño;
};

//arbol AVL con los titulos de todos los libros de la carpeta
struct nodoAVL{
    char* titulo;
    char* ruta;                     //donde esta el archivo
    int altura;

    struct nodoAVL* izq;
    struct nodoAVL* der;
};

//arbol chaflaneado (splay) que funciona como cache de la biblioteca de una comuna
struct nodoSplay{
    char* titulo;
    long tamañoBytes;               //lo que se cargo del archivo
    int lineas;
    int vecesLeido;

    struct nodoSplay* izq;
    struct nodoSplay* der;
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
    struct nodoSplay* biblioteca;   //cache de libros de esta comuna

    int diasTormenta;           //CAMBIO: dias que le quedan a una tormenta que se origino en esta comuna
    int intercambiosHoy;        //CAMBIO: intercambios que ha hecho la comuna en el dia actual

    int disuelta;               //1 si la comuna ya no existe
    int delJugador;             //1 si es una de las comunas que maneja el jugador

    int regalosDados;           //unidades que ha entregado en intercambios
    int regalosRecibidos;       //unidades que ha recibido
    int regalosRecientes;       //lo recibido hace poco, se va olvidando con los dias
    int desastre;               //golpe que deja una emergencia, se recupera con los dias
};

//lo que se ocupa llevar para los logros
struct logros{
    int intercambios;           //intercambios hechos o aceptados por el jugador
    int intercambioHoy;         //si hoy hizo alguno
    int comunasPerdidas;        //comunas del jugador que se han disuelto
    int diaPerdida1;            //dia de la ultima comuna perdida
    int diaPerdida2;            //dia de la penultima
    int caos;                   //1 si perdio tres comunas en menos de 3 turnos
    int diasSinIntercambiar;    //racha actual
    int rachaSinIntercambiar;   //la racha mas larga
};

//matriz que almacena las comunas
struct matrizComunas{
    struct nodoMatriz* inicio;
    int tamaño;

    struct listaSimple* bienesDisponibles;
    struct listaSimple* serviciosDisponibles;

    int dia;                    //CAMBIO: dia actual de la partida (empieza en 1)
    int diasTotales;            //CAMBIO: cantidad de dias que escogio jugar el usuario
    int diasParaEmergencia;     //CAMBIO: cuantos dias faltan para la siguiente emergencia

    struct nodoAVL* catalogo;   //todos los libros de la carpeta
    int totalLibros;

    struct listaSimple* historial;  //lo importante que ha pasado en la partida
    struct logros logros;
    int salir;                  //1 cuando el jugador guarda y sale
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
 * Función que retorna el nombre en la posición dada (empieza en 0)
 */
char* nombreEnPosicion(struct listaSimple* lista, int posicion);

/*
=================================================
    | Funciones de las personas |
=================================================
*/

struct listaPersonas* crearListaPersonas();
struct persona* crearPersona(char* nombre);
void agregarPersona(struct persona* persona, struct listaPersonas* lista);

/**
 * Función que desenlaza una persona de su lista (no la borra)
 */
void sacarPersona(struct persona* persona, struct listaPersonas* lista);

/**
 * Función que retorna la persona número 'posicion' (empieza en 1)
 */
struct persona* obtenerPersona(struct listaPersonas* lista, int posicion);

/**
 * Función que le agrega un servicio a una persona
 */
void darServicio(struct persona* persona, char* servicio);

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
 * @param int numerado 1=con un número al lado, 0=sin número
 */
void imprimirServicios(struct DiccionarioBienes* diccionario, int numerado);

/**
 * Función que saca de la comuna hasta 'cantidad' personas que sepan ese servicio
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
 * CAMBIO: Función que pregunta al usuario cuantos días (turnos) quiere jugar
 * @return int entre 15 y 99
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

/**
 * Necesidad: depende solo de los bienes y de la gente con oficio que tiene la comuna (y del golpe de un desastre)
 */
double calcularNecesidad(struct comuna* comuna);

/**
 * Reciprocidad: balance entre lo que la comuna ha dado y lo que ha recibido
 */
double calcularReciprocidad(struct comuna* comuna);

/**
 * Qué tantos regalos ha recibido la comuna últimamente (0 a 100)
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

/**
 * CAMBIO: ahora también recibe el tablero (lo ocupa el menú de intercambiar)
 */
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
 * @param comuna afectada (NULL si no es una sola)
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
 * Función que mueve los productos entre dos comunas (la usan el jugador y las comunas autónomas)
 * @return 1 si se movió algo, 0 si no se pudo
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
 * Función que le completa al jugador sus 3 comunas (o las que queden)
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
    | Funciones del árbol AVL (catálogo) |
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

/**
 * Función que retorna el libro número 'posicion' en orden alfabético
 */
struct nodoAVL* obtenerAVL(struct nodoAVL* raiz, int posicion, int* contador);

/**
 * Función que lee la carpeta de libros y mete todos los títulos al AVL
 */
struct nodoAVL* cargarCatalogo(char* carpeta, struct nodoAVL* raiz, int* total);
void prepararBiblioteca(struct matrizComunas* tablero);
void imprimirCatalogo(struct nodoAVL* raiz, int* contador, int limite);

/*
=================================================
    | Funciones del árbol chaflaneado (caché) |
=================================================
*/

struct nodoSplay* crearNodoSplay(char* titulo);
struct nodoSplay* rotarDerechaSplay(struct nodoSplay* nodo);
struct nodoSplay* rotarIzquierdaSplay(struct nodoSplay* nodo);

/**
 * Función que sube a la raíz el nodo buscado (o el último que tocó si no está)
 */
struct nodoSplay* chaflanear(struct nodoSplay* raiz, char* titulo);

/**
 * Función que inserta un libro nuevo y lo deja en la raíz
 */
struct nodoSplay* insertarSplay(struct nodoSplay* raiz, struct nodoSplay* nuevo);

/**
 * Inserción normal de árbol binario, solo se usa al cargar una partida para dejar el árbol igual
 */
struct nodoSplay* insertarSinChaflanear(struct nodoSplay* raiz, struct nodoSplay* nuevo);

int contarSplay(struct nodoSplay* raiz);
struct nodoSplay* obtenerSplay(struct nodoSplay* raiz, int posicion, int* contador);
void liberarSplay(struct nodoSplay* raiz);
void imprimirArbolSplay(struct nodoSplay* raiz);

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
void leerLibro(struct matrizComunas* tablero, struct comuna* comuna, struct persona* persona);

void aprenderServicio(struct matrizComunas* tablero, struct persona* persona);
void enviarALeer(struct comuna* comuna, struct matrizComunas* tablero);
void menuBiblioteca(struct comuna* comuna, struct matrizComunas* tablero);

/*
=================================================
    | Funciones de la criminalidad |
=================================================
*/

void cometerCrimen(struct persona* persona, struct nodoMatriz* nodo, struct matrizComunas* tablero);
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
 * @param int final 1 si ya terminó la partida (ahí se revisan los que dependen del final)
 * @return cuántos se consiguieron
 */
int imprimirLogros(struct matrizComunas* tablero, int final);

/*
=================================================
    | Funciones de guardar y cargar |
=================================================
*/

void guardarDiccionario(FILE* archivo, struct DiccionarioBienes* diccionario);
void guardarSplay(FILE* archivo, struct nodoSplay* raiz);
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
