#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// u(t) : N = β0 + β1t
typedef struct regressaoLinear {
  double B0;
  double B1;
} RegressaoLinear;

// u(t) : N = β0 + β1t + β2t
typedef struct regressaoQuadratica
{
  double B0;
  double B1;
  double B2;
} RegressaoQuadratica;

typedef struct dados {
  RegressaoLinear* rLinear;
  RegressaoQuadratica* rQuadratica;
} Dados;

#define TAM 48

int ROWS;
int COLS;

void print_matrix(double matrix[ROWS][COLS])
{
  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      printf("%lf ", matrix[i][j]);
    }
    printf("\n");
  }
}

void gaussElimination(double M[ROWS][COLS], Dados* dados)
{
  double M2[ROWS][COLS];

  memcpy(M2, M, ROWS * COLS * sizeof(double));

  for (int j = 0; j < COLS - 2; j++)
  {
    for (int i = j; i < ROWS; i++)
    {
      if (M2[i][j] != 0)
      {
        if (i != j)
        {
          for (int k = 0; k < COLS; k++)
          {
            double temp = M2[i][k];
            M2[i][k] = M2[j][k];
            M2[j][k] = temp;
          }
        }
        for (int m = j + 1; m < ROWS; m++)
        {
          double a = -M2[m][j] / M2[j][j];
          for (int n = j; n < COLS; n++)
          {
            M2[m][n] += a * M2[j][n];
          }
        }
        printf("\n");

        break;
      }
    }
  }

  double x[ROWS];

  x[ROWS - 1] = M2[ROWS - 1][ROWS] / M2[ROWS - 1][ROWS - 1];

  for (int i = ROWS - 2; i >= 0; i--)
  {
    double sum = 0.0;
    for (int j = i + 1; j < ROWS; j++)
    {
      sum = sum + M2[i][j] * x[j];
    }
    x[i] = (M2[i][ROWS] - sum) / M2[i][i];
  }

  /* printf("\nThe solution is: \n");
  for (int i = 0; i < ROWS; i++)
  {
    printf("\nx%d=%lf\t", i, x[i]);
  }
  printf("\n\n"); */

  if(ROWS == 2) {
    dados->rLinear->B0 = x[0];
    dados->rLinear->B1 = x[1];
    printf("Regressao Linear\n");
  } else {
    dados->rQuadratica->B0 = x[0];
    dados->rQuadratica->B1 = x[1];
    dados->rQuadratica->B2 = x[2];
    printf("Regressao Quadratica\n");
  }

}

void regressaoLinear(double matrixDeSomatorio[ROWS][COLS])
{
  Dados dados;
  dados.rLinear = malloc(sizeof(RegressaoLinear));
  dados.rQuadratica = malloc(sizeof(RegressaoQuadratica));
  
  gaussElimination(matrixDeSomatorio, &dados);
  printf("b0: %lf\nb1: %lf", dados.rLinear->B0, dados.rLinear->B1);
}

void regressaoQuadratica(double matrixDeSomatorio[ROWS][COLS])
{
  Dados dados;
  dados.rLinear = malloc(sizeof(RegressaoLinear));
  dados.rQuadratica = malloc(sizeof(RegressaoQuadratica));

  gaussElimination(matrixDeSomatorio, &dados);
  printf("b0: %lf\nb1: %lf\nb2: %lf", dados.rQuadratica->B0, dados.rQuadratica->B1, dados.rQuadratica->B2);
}

int main(char argc, char argv[]) {
  FILE* arquivo = fopen("dados.txt", "r");

  int* vetorX = malloc(sizeof(int) * TAM);
  long long int* xAoQuadrado = malloc(sizeof(long long int) * TAM);
  long long int* xAoCubo = malloc(sizeof(long long int) * TAM);
  long long int* xAQuarta = malloc(sizeof(long long int) * TAM);

  long long int* vetorY = malloc(sizeof(long long int) * TAM);
  long long int* xVezesY = malloc(sizeof(long long int) * TAM);

  int somatorioDeX = 0;
  int somatorioDeXAoQuadrado = 0;
  long long int somatorioDeXAoCubo = 0;
  long long int somatorioDeXAQuarta = 0;

  long long int somatorioDeY = 0;
  long long int somatorioDeXVezesY = 0;
  long long int somatorioDeYVezesXAoQuadrado = 0;

  if (arquivo == NULL)
  {
    printf("Erro de alocacao ou Falha no arquivo \n");
    exit(1);
  }
  
  while (feof(arquivo) == 0) {
    for (int i = 0; i < TAM; i++)
    {
      fscanf(arquivo, "%d %lld\n", &vetorX[i], &vetorY[i]);
      xAoQuadrado[i] = vetorX[i] * vetorX[i];
      xAoCubo[i] = pow(vetorX[i], 3);
      xAQuarta[i] = pow(vetorX[i], 4);
      xVezesY[i] = vetorX[i] * vetorY[i];
    }
  }

  for(int i = 0; i < TAM; i++) {
    somatorioDeX += vetorX[i];
    somatorioDeXAoQuadrado += xAoQuadrado[i];
    somatorioDeXAoCubo += xAoCubo[i];
    somatorioDeXAQuarta += xAQuarta[i];

    somatorioDeY += vetorY[i];
    somatorioDeXVezesY += xVezesY[i];
    somatorioDeYVezesXAoQuadrado += vetorY[i] * xAoQuadrado[i];
  }

  ROWS = 2;
  COLS = 3;

  double matrizRegressaoLinear[2][3] = {
      {TAM, somatorioDeX, somatorioDeY},
      {somatorioDeX, somatorioDeXAoQuadrado, somatorioDeXVezesY},
  };

  regressaoLinear(matrizRegressaoLinear);

  ROWS = 3;
  COLS = 4;

  double matrizRegressaoQuadratica[3][4] = {
      {TAM, somatorioDeX, somatorioDeXAoQuadrado, somatorioDeY},
      {somatorioDeX, somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXVezesY},
      {somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXAQuarta, somatorioDeYVezesXAoQuadrado},
  };

  regressaoQuadratica(matrizRegressaoQuadratica);

  printf("\n");
  
  /* printf("Somatorio de X: %d\n", somatorioDeX);
  printf("Somatorio de X^2: %d\n", somatorioDeXAoQuadrado);
  printf("Somatorio de X^3: %lld\n", somatorioDeXAoCubo);
  printf("Somatorio de X^4: %lld\n", somatorioDeXAQuarta);

  printf("Somatorio de Y: %lld\n", somatorioDeY);
  printf("Somatorio de X*Y: %lld\n", somatorioDeXVezesY);
  printf("Somatorio de Y*X^2: %lld", somatorioDeYVezesXAoQuadrado); */

  fclose(arquivo);
  return 0;
}