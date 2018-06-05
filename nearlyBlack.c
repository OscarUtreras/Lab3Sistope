#include "bmp_format.h"
#include "image_process.h"
#include <stdio.h>
#include <unistd.h>
/* Funcion encargada clasificar la imagen binarizada.
Entrada: Estructura con la informacion de la imagen y umbral de clasificacion.
Salida: 1 si es nearly black o 0 si no es nearly black. */
int nearlyBlack(BMP *image, int umbCla)
{
    float result = ((float)image->pixelesNegros / ((float)image->alto * (float)image->ancho)) * 100;
    if (result > umbCla)
        return 1;
    return 0;
}

int main(int argc, char **argv)
{
    // Cuarta frase del pipeline: Mostrar resultados de nearly black (en caso de que se solicite a traves de la bandera)
    /* Donde:
    argv[0] = "./nearlyBlack"
    argv[1] = img (debe ser tipo BMP*, ver como le pasaremos eso :c)
    argv[2] = umbralClassi
    argv[3] = flag
    argv[4] = filename (imagen abierta)
    argv[5] = NULL
    Esto debe hacer en el main de donde llamaremos a esta etapa
    */
    if (argv[3] == 1)
    {
        if (nearlyBlack(argv[1], argv[2]) == 1)
            printf("| %s   |      yes         | \n", argv[4]);
        else
            printf("| %s   |       no         | \n", argv[4]);
    }
    return 0;
}
