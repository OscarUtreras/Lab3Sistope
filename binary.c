#include "bmp_format.h"
#include "image_process.h"
#include "utils.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

/* Procedimiento encargada de binarizar la imagen.
Entrada: Estructura con la informacion de la imagen y umbral de binarizacion.
*/
void Binary(BMP *image, int umbBinary)
{
    int i, j, contador = 0;
    for (i = image->alto - 1; i >= 0; i--)
    {
        for (j = 0; j < image->ancho; j++)
        {
            if (image->pixelB[i][j] > umbBinary)
            {
                image->pixelB[i][j] = 255;
                image->pixelG[i][j] = 255;
                image->pixelR[i][j] = 255;
            }
            else
            {
                image->pixelB[i][j] = 0;
                image->pixelG[i][j] = 0;
                image->pixelR[i][j] = 0;
                contador++;
            }
        }
    }
    image->pixelesNegros = contador;
}

int main(int argc, char **argv)
{
    // Tercera fase del pipeline: Pasar de escala de grises a imagen binarizada
    int pip[2];
    int fd, umbBinary, status;
    pid_t childpid;
    pipe(pip);
    sscanf(argv[5], "%d", &fd);
    BMP *img = catchHeadBMP(fd);
    catchMatrixBMP(fd, img);
    sscanf(argv[2],"%d",&umbBinary);
    Binary(img, umbBinary);
    childpid=fork();
    if(childpid==0)
    {
        char *num_fd = (char *)malloc(sizeof(char) * 4);
        sprintf(num_fd, "%d", pip[0]);
        argv[0] = "./analisysProperty";
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