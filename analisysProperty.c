#include "bmp_format.h"
#include "image_process.h"
#include "utils.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

/* Función encargada clasificar la imagen binarizada.
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
    // Cuarta frase del pipeline: Mostrar resultados de nearly black
    int pip[2];
    int fd, image_n, status, flag, umbClassi, result;
    pid_t childpid;
    char *filename = (char *)malloc(sizeof(char) * 16);
    pipe(pip);
    sscanf(argv[5], "%d", &fd);
    BMP *img = catchHeadBMP(fd);
    catchMatrixBMP(fd, img);
    sscanf(argv[1],"%d",&image_n);
    sprintf(filename, "imagen_%d.bmp", image_n);
    sscanf(argv[4],"%d",&flag);
    sscanf(argv[3],"%d",&umbClassi);
    if (flag == 1)
    {
      if (flag == 1 && image_n==1)
          printf("|    Image       |   nearly black?  |\n");
      result = nearlyBlack(img, umbClassi);
      if (result == 1)
          printf("| %s   |      yes         | \n", filename);
      else
          printf("| %s   |       no         | \n", filename);
    }
    childpid=fork();
    if(childpid==0)
    {
        char *num_fd = (char *)malloc(sizeof(char) * 4);
        sprintf(num_fd, "%d", pip[0]);
        argv[0] = "./saveBMP";
        argv[5] = num_fd;  
        execvp(argv[0], argv);
    }
    else
    {
        sendHeadBMP(pip, img);
        sendMatrixBMP(pip[1], img);
        waitpid(childpid, &status, 0);

    }
    return 0;
}