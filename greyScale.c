#include "bmp_format.h"
#include "image_process.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* Procedimiento encargada de pasar la imagen a escala de grises.
Entrada: Estructura con la informacion de la imagen.
*/
void GreyScale(BMP *image)
{
  int R, G, B, grey, i, j;
  for (i = image->alto - 1; i >= 0; i--)
  {
    for (j = 0; j < image->ancho; j++)
    {
      B = image->pixelB[i][j] * 0.11;
      G = image->pixelG[i][j] * 0.59;
      R = image->pixelR[i][j] * 0.3;
      grey = B + G + R;
      image->pixelB[i][j] = grey;
      image->pixelG[i][j] = grey;
      image->pixelR[i][j] = grey;
    }
  }
}

int main(int argc, char **argv)
{
  // Segunda fase del pipeline: Pasar imagen a escala de grises
  int fd;
  int pip[2];
  int status;
  sscanf(argv[5], "%d", &fd);
  BMP *img = catchHeadBMP(fd);
  catchMatrixBMP(fd, img);
  GreyScale(img);
  pipe(pip);
  pid_t childpid = fork();
  if (childpid == 0)
  {
    char *num_fd = (char *)malloc(sizeof(char) * 4);
    sprintf(num_fd, "%d", pip[0]);
    argv[0] = "./binary";
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