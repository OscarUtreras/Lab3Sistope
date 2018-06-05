#include "handler.h"
#include "image_process.h"
#include "io_image.h"
#include "bmp_format.h"
#include <stdio.h>
#include <unistd.h>

/* Funcion encargada de invocar a las distintas funciones necesarias para obtener la imagen binarizada .
Entrada: cantidad de imagenes, umbral de bonarizacion, umbral de clasificacion y bandera (indica si se muestra informacion por pantalla).
Salida: nada. */
int pipeline(int image_n, int umbral, int flag, int umbClassi)
{
    BMP *img;
    char *filename = (char *)malloc(sizeof(char) * 16);
    int result;

    // Primera fase del pipeline: Leer la imagen
    sprintf(filename, "imagen_%d.bmp", image_n);
    img = LoadBMP(filename);
    if(img==NULL)
      return 1;

    // Segunda fase del pipeline: Pasar imagen a escala de grises
    GreyScale(img);

    // Tercera fase del pipeline: Pasar de escala de grises a imagen binarizada
    Binary(img, umbral);

    // Cuarta frase del pipeline: Mostrar resultados de nearly black (en caso de que se solicite a traves de la bandera)
    if (flag == 1)
    {
        result = nearlyBlack(img, umbClassi);
        if (result == 1)
            printf("| %s   |      yes         | \n", filename);
        else
            printf("| %s   |       no         | \n", filename);
    }

    // Quinta fase del pipeline: Almacenar imagen binarizada
    sprintf(filename, "out_%d.bmp", image_n);
    SaveBMP(img, filename);

    free(img);
    return 0;
}

/* Procedimiento encargado de manejar la ejecución del programa.
Entrada: argc y argv.
Salida: nada. */
void Handler(int argc, char **argv)
{
    int i, n_images=-1, umbBinary=-1, umbClassi=-1, flag=-1;
    int test = getArguments(argc, argv, &n_images, &umbBinary, &umbClassi, &flag);
    if(argc<7)
        printf("Faltan argumentos, por favor ejecute el programa con los argumentos necesarios.\n");
    else
    {
      if (test == 0)
      {
        if (flag == 1)
          printf("|    Image       |   nearly black?  |\n");
        for (i = 1; i <= n_images; i++)
        {
          if(pipeline(i, umbBinary, flag, umbClassi)==1)
            printf("No se encontro la imagen_%d.bmp.\n", i);
        }
      }
    }
}
