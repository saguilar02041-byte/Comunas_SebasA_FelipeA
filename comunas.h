//Archivo h

/**
 * Función que imprime el título del jueguito
 */
void TituloPrincipal();

/**
 * Función que lee el archivo de "personas"
 */
int leerPersonas();

//Diccionario que guarda los bienes para cada comuna 
struct DiccionarioBienes{
    struct nodoDic* inicio;
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

