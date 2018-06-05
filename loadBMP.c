#include "bmp_format.h"
#include "io_image.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* Procedimiento encargado cargar la imagen BMP.
Entrada: nombre de la imagen.
Salida: imagen cargada con la estructura BMP
*/
BMP *LoadBMP(char *filename)
{
    BMP *imagen = (BMP *)malloc(sizeof(BMP));
    FILE *archivo; //Puntero FILE para el archivo de imágen a abrir
    int i, j, k, resto;
    unsigned char R, B, G, V, var;
    //Abrir el archivo de imágen
    archivo = fopen(filename, "rb+");
    if (!archivo)
    {
        //Si la imágen no se encuentra en la ruta dada
        printf("La imagen %s no se encontro\n", filename);
        exit(1);
    }
    //Leer la cabecera de la imagen y almacenarla en la estructura global img
    fseek(archivo, 0, SEEK_SET);
    fread(&imagen->bm, sizeof(char), 2, archivo);
    fread(&imagen->tamano, sizeof(int), 1, archivo);
    fread(&imagen->reservado, sizeof(int), 1, archivo);
    fread(&imagen->offset, sizeof(int), 1, archivo);
    fread(&imagen->tamanoMetadatos, sizeof(int), 1, archivo);
    fread(&imagen->alto, sizeof(int), 1, archivo);
    fread(&imagen->ancho, sizeof(int), 1, archivo);
    fread(&imagen->numeroPlanos, sizeof(short int), 1, archivo);
    fread(&imagen->profundidadColor, sizeof(short int), 1, archivo);
    fread(&imagen->tipoCompresion, sizeof(int), 1, archivo);
    fread(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
    fread(&imagen->pxmh, sizeof(int), 1, archivo);
    fread(&imagen->pxmv, sizeof(int), 1, archivo);
    fread(&imagen->coloresUsados, sizeof(int), 1, archivo);
    fread(&imagen->coloresImportantes, sizeof(int), 1, archivo);
    //Validar ciertos datos de la cabecera de la imágen
    if (imagen->bm[0] != 'B' || imagen->bm[1] != 'M')
    {
        printf("La imagen debe ser un bitmap.\n");
        exit(1);
    }
    //*********************************************************************************************************
    //Colocar el cursor en el byte (
    //*********************************************************************************************************
    fseek(archivo, imagen->offset, SEEK_SET);
    //*********************************************************************************************************
    //Reservar memoria para el arreglo que tendra los planos RGB de la imagen
    //*********************************************************************************************************
    imagen->pixelR = malloc(imagen->alto * sizeof(char *));
    imagen->pixelG = malloc(imagen->alto * sizeof(char *));
    imagen->pixelB = malloc(imagen->alto * sizeof(char *));
    if (imagen->profundidadColor == 32)
        imagen->pixelV = malloc(imagen->alto * sizeof(char *));
    for (i = 0; i < imagen->alto; i++)
    {
        imagen->pixelR[i] = malloc(imagen->ancho * sizeof(char));
        imagen->pixelG[i] = malloc(imagen->ancho * sizeof(char));
        imagen->pixelB[i] = malloc(imagen->ancho * sizeof(char));
        if (imagen->profundidadColor == 32)
            imagen->pixelV[i] = malloc(imagen->alto * sizeof(char));
    }
    //*********************************************************************************************************
    //Padding
    //*********************************************************************************************************
    resto = (imagen->ancho * 3) % 4;
    if (resto != 0)
        resto = 4 - resto;
    //*********************************************************************************************************
    //Pasar la imágen a la  matriz especfica  R / G / B
    //*********************************************************************************************************
    //Iterar a través de las filas de píxeles, teniendo en cuenta que los datos comienza en la esquina inferior izquierda de la imagen BMP
    int contV = 0;
    for (i = imagen->alto - 1; i >= 0; i--)
    {
        for (j = 0; j < imagen->ancho; j++)
        {
            fread(&B, sizeof(char), 1, archivo); //Byte Blue del pixel
            fread(&G, sizeof(char), 1, archivo); //Byte Green del pixel
            fread(&R, sizeof(char), 1, archivo); //Byte Red del pixel
            if (imagen->profundidadColor == 32)
            {
                fread(&V, sizeof(char), 1, archivo); //Byte V del pixel
                imagen->pixelV[i][j] = V;
                contV++;
            }
            imagen->pixelR[i][j] = R;
            imagen->pixelG[i][j] = G;
            imagen->pixelB[i][j] = B;
        }
        for (k = 1; k <= resto; k++)
            fread(&var, sizeof(char), 1, archivo); //Leer los pixeles de relleno (Padding)
    }
    //Cerrrar el archivo
    fclose(archivo);
    return imagen;
}

int main(int argc, char **argv)
{   // Primera fase del pipeline: Leer la imagen
    int fd[2];
    int status;
    pipe(fd);
    pid_t childpid=fork();
    if(childpid==0)
    {
        char *num_fd = (char *)malloc(sizeof(char) * 4);
        sprintf(num_fd, "%d", fd[0]);
        argv[0] = "./greyScale";
        argv[5] = num_fd;
        execvp(argv[0], argv);
    }
    else
    {
        BMP *img;
        char *filename = (char *)malloc(sizeof(char) * 16);
        int image_n;
        sscanf(argv[1],"%d",&image_n);
        sprintf(filename, "imagen_%d.bmp", image_n);
        img = LoadBMP(filename);
        if(img==NULL)
            return 1;
        sendHeadBMP(fd, img);
        sendMatrixBMP(fd[1], img);
        waitpid(childpid, &status, 0);
    }
    return 0;
}