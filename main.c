#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

/* Función que obtiene los valores de cada parametro ingresado.
Entrada: argc, argv, cantidad de imagenes, umbral de bonarizacion, umbral de clasificacion y bandera (indica si se muestra informacion por pantalla).
Salida: cantidad de imagenes, umbral de bonarizacion, umbral de clasificacion y bandera por referencia. */
int getArguments(int argc, char **argv, int *n_images, int *umbBinary, int *umbClassi, int *flag)
{
  int argument;
  while ((argument = getopt(argc, argv, "c:u:n:b")) != -1)
  {
    switch (argument)
    {
    case 'c':
      if (strcmp(optarg, "-u") == 0)
      {
        printf("Tiene que ingresar la cantidad de imagenes luego de -c.\n");
        return 1;
      }
      sscanf(optarg, "%d", n_images);
      break;
    case 'u':
      if (strcmp(optarg, "-n") == 0)
      {
        printf("Tiene que ingresar el umbral para binarizar la imagen luego de -u.\n");
        return 1;
      }
      sscanf(optarg, "%d", umbBinary);
      break;
    case 'n':
      if (strcmp(optarg, "-b") == 0)
      {
        printf("Tiene que ingresar el umbral para la clasificacion luego de -n.\n");
        return 1;
      }
      sscanf(optarg, "%d", umbClassi);
      break;
    case 'b':
      *flag = 1;
      break;
    case '?':
      if (optopt == 'c' || optopt == 'u' || optopt == 'n')
        fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Opcion desconocida '%c'\n", optopt);
      else
        fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
      return 1;
    default:
      exit(1);
    }
  }
  if(*n_images <= 0)
  {
    printf("El número de imagenes debe ser mayor o igual a 1.\n");
    return 1;
  }
  if(*umbBinary > 255 || *umbBinary < 0)
  {
    printf("EL umbral de binarizacion tiene que estar entre 0 y 255.\n");
    return 1;
  }
  if(*umbClassi > 100 || *umbClassi < 0)
  {
    printf("El umbral de clasificación debe estar entre 0 y 100.\n");
    return 1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  int i, n_images = -1, umbBinary = -1, umbClassi = -1, flag = -1;
  int arguments = getArguments(argc, argv, &n_images, &umbBinary, &umbClassi, &flag);
  pid_t pids[n_images];
  int status[n_images];
  if (argc < 7)
    printf("Faltan argumentos, por favor ejecute el programa con los argumentos necesarios.\n");
  else
  {
    if (arguments == 0)
    {
      printf("\nCargando, por favor espere...\n\n");
      for (i = 1; i <= n_images; i++)
      {
        //Se construye los argumentos y se hace el llamado a la nueva imagen.
        char **args = buildArguments("./loadBMP", i, umbBinary, umbClassi, flag, 1);
        pids[i - 1] = fork();
        if (pids[i - 1] == 0)
        {
          //Llamada a la nueva imagen.
          execvp(args[0], args);
        }
        else
        {
          //El proceso main espera a su proceso hijo para continuar.
          waitpid(pids[i - 1], &status[i - 1], 0);
        }
      }
    }
  }
  return 0;
}
