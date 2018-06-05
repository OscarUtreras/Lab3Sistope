#include "bmp_format.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

/*Función que construye los argumentos para llamar una nueva imagen de proceso.
Entrada: El nombre del siguiente proceso, el número de la imagen BMP,umbral de binarización,
umbral de clasificación, bandera para mostrar los resultados, file descriptor out del pipe.
Salida: Los argumentos concatenados.
*/
char **buildArguments(char* file, int i, int umbBinary, int umbClassi, int flag, int fd)
{
  char **args = (char**)malloc(sizeof(char*)*7);

  char *image_id = (char *)malloc(sizeof(char) * 4);
  char *umbBinaryS = (char *)malloc(sizeof(char) * 4);
  char *umbClassiS = (char *)malloc(sizeof(char) * 4);
  char *flagS = (char *)malloc(sizeof(char) * 4);
  char *fdS = (char *)malloc(sizeof(char) * 4);  

  sprintf(image_id, "%d", i);
  sprintf(umbBinaryS, "%d", umbBinary);
  sprintf(umbClassiS, "%d", umbClassi);
  sprintf(flagS, "%d", flag);
  sprintf(fdS, "%d", fd);

  args[0] = file;
  args[1] = image_id;
  args[2] = umbBinaryS;
  args[3] = umbClassiS;
  args[4] = flagS;
  args[5] = fdS;  
  args[6] = NULL;
  return args;
}

/*Procedimiento que envía la información de la cabecera de la imagen BMP a través de pipe.
Entrada: El pipe, el puntero a la imagen BMP cargada.
*/
void sendHeadBMP(int fd[], BMP *img)
{
    close(fd[0]);
    int buffer_int;
    char buffer_char;
    short int buffer_short_int;
    buffer_char=img->bm[0];
    write(fd[1], &buffer_char, sizeof(char));
    buffer_char=img->bm[1];
    write(fd[1], &buffer_char, sizeof(char));
    buffer_int=img->tamano;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->reservado;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->offset;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->tamanoMetadatos;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->alto;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->ancho;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_short_int=img->numeroPlanos;
    write(fd[1], &buffer_short_int, sizeof(short int));
    buffer_short_int=img->profundidadColor;
    write(fd[1], &buffer_short_int, sizeof(short int));
    buffer_int=img->tipoCompresion;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->tamanoEstructura;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->pxmh;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->pxmv;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->coloresUsados;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->coloresImportantes;
    write(fd[1], &buffer_int, sizeof(int));
    buffer_int=img->pixelesNegros;
    write(fd[1], &buffer_int, sizeof(int));
}

/*Función para obtener los datos de cabecera de una imagen BMP.
Entrada: El file descriptor out del pipe.
Salida: Un puntero a una estructura de imagen BMP con el header.
*/
BMP *catchHeadBMP(int fd)
{
  BMP *img = (BMP *)malloc(sizeof(BMP));
  ;
  int buffer_int;
  char buffer_char;
  short int buffer_short_int;
  read(fd, &buffer_char, sizeof(char));
  img->bm[0] = buffer_char;
  read(fd, &buffer_char, sizeof(char));
  img->bm[1] = buffer_char;
  read(fd, &buffer_int, sizeof(int));
  img->tamano = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->reservado = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->offset = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->tamanoMetadatos = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->alto = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->ancho = buffer_int;
  read(fd, &buffer_short_int, sizeof(short int));
  img->numeroPlanos = buffer_short_int;
  read(fd, &buffer_short_int, sizeof(short int));
  img->profundidadColor = buffer_short_int;
  read(fd, &buffer_int, sizeof(int));
  img->tipoCompresion = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->tamanoEstructura = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->pxmh = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->pxmv = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->coloresUsados = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->coloresImportantes = buffer_int;
  read(fd, &buffer_int, sizeof(int));
  img->pixelesNegros = buffer_int;
  return img;
}

/*Procedimiento para mostrar por pantalla el contenido de la cabecera de la imagen BMP.
Entrada: Un puntero a una estructura de imagen BMP.
*/
void printBMP(BMP *img)
{
    printf("- -BM[0]:%c\n", img->bm[0]);
    printf("- -BM[1]:%c\n", img->bm[1]);
    printf("- -Tamano:%d\n", img->tamano);
    printf("- -Reservado:%d\n", img->reservado);
    printf("- -Offset:%d\n", img->offset);
    printf("- -Tamano Metadatos: %d\n", img->tamanoMetadatos);
    printf("- -Alto:%d\n", img->alto);
    printf("- -Ancho:%d\n", img->ancho);
    printf("- -Numero Palanos:%d\n", img->numeroPlanos);
    printf("- -Profundidad Color:%d\n", img->profundidadColor);
    printf("- -Tipo Compresion:%d\n", img->tipoCompresion);
    printf("- -Tamano Estructura:%d\n", img->tamanoEstructura);
    printf("- -Pxmh:%d\n", img->pxmh);
    printf("- -Pxmv:%d\n", img->pxmv);
    printf("- -Colores Usados:%d\n", img->coloresUsados);
    printf("- -Colores Importantes:%d\n", img->coloresImportantes);
    printf("- -Pixeles Negros:%d\n", img->pixelesNegros);
}

/*Procedimiento para enviar los valores de la matriz de colores de una imagen BMP a través
de un pipe.
Entrada: File descriptor in del pipe, un puntero a la imagen con la información rellenar.
*/
void sendMatrixBMP(int fd, BMP* img)
{
    int i, j, k, resto, var;
    unsigned char buffer_char;
    resto = (img->ancho * 3) % 4; //Padding (Bytes necesarios para que el Pad row alcance a ser multiplo de 4 Bytes)
    if (resto != 0)
        resto = 4 - resto;
    //Iterar a través de las filas de píxeles, teniendo en cuenta que los datos comienza en la esquina inferior izquierda de la imagen BMP
    int contV = 0;
    for (i = img->alto - 1; i >= 0; i--)
    {
        for (j = 0; j < img->ancho; j++)
        {
            //Ecribir los 3 bytes BGR al archivo BMP, en este caso todos se igualan al mismo valor (Valor del pixel en la matriz de la estructura imagen)
            buffer_char=img->pixelB[i][j];
            write(fd, &buffer_char, sizeof(char)); //Escribir el Byte Blue del pixel
            buffer_char=img->pixelG[i][j];
            write(fd, &buffer_char, sizeof(char)); //Escribir el Byte Green del pixel
            buffer_char=img->pixelR[i][j];
            write(fd, &buffer_char, sizeof(char)); //Escribir el Byte Red del pixel
            if (img->profundidadColor == 32)
            {
                buffer_char=img->pixelV[i][j];
                write(fd, &buffer_char, sizeof(char));
                contV++; //Escribir el Byte Red del pixel
            }
        
        }
    //Padding (Bytes necesarios para que el Pad row alcance a ser multiplo de 4 Bytes)
    for (k = 1; k <= resto; k++)
      write(fd, &var, sizeof(char)); //Escribir los pixeles de relleno
    }
}

/*Procedimiento para obtener los valores de la matriz de colores de una imagen BMP a través
de un pipe.
Entrada: File descriptor out del pipe, un puntero de la imagen a rellenar.
*/
void catchMatrixBMP(int fd, BMP *img)
{
    int i, j, k, resto, var;
    unsigned char R, B, G, V;
    img->pixelR = malloc(img->alto * sizeof(char *));
    img->pixelG = malloc(img->alto * sizeof(char *));
    img->pixelB = malloc(img->alto * sizeof(char *));
    if (img->profundidadColor == 32)
        img->pixelV = malloc(img->alto * sizeof(char *));
    for (i = 0; i < img->alto; i++)
    {
        img->pixelR[i] = malloc(img->ancho * sizeof(char));
        img->pixelG[i] = malloc(img->ancho * sizeof(char));
        img->pixelB[i] = malloc(img->ancho * sizeof(char));
        if (img->profundidadColor == 32)
            img->pixelV[i] = malloc(img->alto * sizeof(char));
    }
    resto = (img->ancho * 3) % 4;
    if (resto != 0)
        resto = 4 - resto;
    int contV = 0;
    for (i = img->alto - 1; i >= 0; i--)
    {
        for (j = 0; j < img->ancho; j++)
        {
            read(fd, &B, sizeof(char));
            read(fd, &G, sizeof(char));
            read(fd, &R, sizeof(char));
            if (img->profundidadColor == 32)
            {
                read(fd, &V, sizeof(char));
                img->pixelV[i][j] = V;
                contV++;
            }
            
            img->pixelR[i][j] = R;
            img->pixelG[i][j] = G;
            img->pixelB[i][j] = B;
        }
        for (k = 1; k <= resto; k++)
            read(fd, &var, sizeof(char));
        
    }
}