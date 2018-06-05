#include "bmp_format.h"
#include "image_process.h"
#include "utils.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
/* Procedimiento encargada almacenar la imagen BMP binarizada.
Entrada: nombre de la imagen y estructura con la informacion de la imagen. */
void SaveBMP(BMP *imagen, char *filename)
{
  FILE *archivo; //Puntero FILE para el archivo de imágen a abrir
  int i, j, k, resto, var;
  //Abrir el archivo de imágen
  archivo = fopen(filename, "wb+");
  if (!archivo)
  {
    //Si la imágen no se encuentra en la ruta dada
    printf("La imágen %s no se pudo crear\n", filename);
    exit(1);
  }
  //Leer la cabecera de la imagen y almacenarla en la estructura global img
  fseek(archivo, 0, SEEK_SET);
  fwrite(&imagen->bm, sizeof(char), 2, archivo);
  fwrite(&imagen->tamano, sizeof(int), 1, archivo);
  fwrite(&imagen->reservado, sizeof(int), 1, archivo);
  fwrite(&imagen->offset, sizeof(int), 1, archivo);
  fwrite(&imagen->tamanoMetadatos, sizeof(int), 1, archivo);
  fwrite(&imagen->alto, sizeof(int), 1, archivo);
  fwrite(&imagen->ancho, sizeof(int), 1, archivo);
  fwrite(&imagen->numeroPlanos, sizeof(short int), 1, archivo);
  fwrite(&imagen->profundidadColor, sizeof(short int), 1, archivo);
  fwrite(&imagen->tipoCompresion, sizeof(int), 1, archivo);
  fwrite(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
  fwrite(&imagen->pxmh, sizeof(int), 1, archivo);
  fwrite(&imagen->pxmv, sizeof(int), 1, archivo);
  fwrite(&imagen->coloresUsados, sizeof(int), 1, archivo);
  fwrite(&imagen->coloresImportantes, sizeof(int), 1, archivo);
  //*********************************************************************************************************
  //Colocar el cursor en el lugar de lectura especifica
  //*********************************************************************************************************
  fseek(archivo, imagen->offset, SEEK_SET);
  //*********************************************************************************************************
  //Pasar la estrucutra a imagen
  //*********************************************************************************************************
  resto = (imagen->ancho * 3) % 4; //Padding (Bytes necesarios para que el Pad row alcance a ser multiplo de 4 Bytes)
  if (resto != 0)
    resto = 4 - resto;
  //Iterar a través de las filas de píxeles, teniendo en cuenta que los datos comienza en la esquina inferior izquierda de la imagen BMP
  int contV = 0;
  for (i = imagen->alto - 1; i >= 0; i--)
  {
    for (j = 0; j < imagen->ancho; j++)
    {
      //Ecribir los 3 bytes BGR al archivo BMP, en este caso todos se igualan al mismo valor (Valor del pixel en la matriz de la estructura imagen)
      fwrite(&imagen->pixelB[i][j], sizeof(char), 1, archivo); //Escribir el Byte Blue del pixel
      fwrite(&imagen->pixelG[i][j], sizeof(char), 1, archivo); //Escribir el Byte Green del pixel
      fwrite(&imagen->pixelR[i][j], sizeof(char), 1, archivo); //Escribir el Byte Red del pixel
      if (imagen->profundidadColor == 32)
      {
        fwrite(&imagen->pixelV[i][j], sizeof(char), 1, archivo);
        contV++; //Escribir el Byte Red del pixel
      }
    }
    //Padding (Bytes necesarios para que el Pad row alcance a ser multiplo de 4 Bytes)
    for (k = 1; k <= resto; k++)
      fwrite(&var, sizeof(char), 1, archivo); //Escribir los pixeles de relleno
  }
  //Cerrrar el archivo
  fclose(archivo);
}

int main(int argc, char **argv)
{
    // Quinta fase del pipeline: Almacenar imagen binarizada
    char *filename = (char *)malloc(sizeof(char) * 16);
    int image_n, fd;
    sscanf(argv[1],"%d",&image_n);
    sprintf(filename, "out_%d.bmp", image_n);
    sscanf(argv[5], "%d", &fd);
    BMP *img = catchHeadBMP(fd);
    catchMatrixBMP(fd, img);  
    SaveBMP(img, filename);
    return 0;
}