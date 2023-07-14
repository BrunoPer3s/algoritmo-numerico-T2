#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// u(t) : N = β0 + β1t
typedef struct regressaoLinear {
  double B0;
  double B1;
  double ui;
  double d1;
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

//#define TAM 48
#define TAM 11

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

void regressaoLinear(double matrixDeSomatorio[ROWS][COLS], double vetorX[TAM], double vetorY[TAM], double somatorioDeYAoQuadrado, double somatorioDeY)
{
  Dados dados;
  dados.rLinear = malloc(sizeof(RegressaoLinear));
  dados.rQuadratica = malloc(sizeof(RegressaoQuadratica));

  double *vetorUi = malloc(sizeof(double) * TAM);
  double *vetorDi = malloc(sizeof(double) * TAM);

  double somatorioDiAoQuadrado = 0.0;
  double rAoQuadrado = 0.0;
  
  gaussElimination(matrixDeSomatorio, &dados);
  printf("b0: %lf\nb1: %lf\n", dados.rLinear->B0, dados.rLinear->B1);
  for (int i = 0; i < TAM; i++)
  {
    vetorUi[i] = dados.rLinear->B0 + dados.rLinear->B1 * vetorX[i];
    vetorDi[i] = vetorY[i] - vetorUi[i];
    somatorioDiAoQuadrado += pow(vetorDi[i], 2);
  }

  printf("\nUi\n");
  for(int i = 0; i < TAM; i++) {
    printf("%lf\n", vetorUi[i]);
  }

  printf("\nDi\n");
  for(int i = 0; i < TAM; i++) {
    printf("%lf\n", vetorDi[i]);
  }

  printf("\nNUMERO MAGICO: %lf\n", somatorioDiAoQuadrado);
  printf("\nSomatorio de Y^2: %lf\n", somatorioDeYAoQuadrado);
  printf("\n(somatorio de y): %lf\n", somatorioDeY);
  printf("\n(somatorio de y)^2: %lf\n", pow(somatorioDeY, 2));

  rAoQuadrado = 1 - (somatorioDiAoQuadrado / (somatorioDeY - (pow(somatorioDeY, 2) / TAM)));
  printf("Coeficiente de Determinação: %lf", rAoQuadrado);
}

void regressaoQuadratica(double matrixDeSomatorio[ROWS][COLS], double vetorX[TAM], double vetorY[TAM], double vetorXAoQuadrado[TAM])
{
  Dados dados;
  dados.rLinear = malloc(sizeof(RegressaoLinear));
  dados.rQuadratica = malloc(sizeof(RegressaoQuadratica));

  double *vetorUi = malloc(sizeof(double) * TAM);
  double *vetorDi = malloc(sizeof(double) * TAM);
  double somatorioVetorDiAoQuadrado = 0.0;

  double rAoQuadrado = 0.0;

  gaussElimination(matrixDeSomatorio, &dados);
  printf("b0: %lf\nb1: %lf\nb2: %lf", dados.rQuadratica->B0, dados.rQuadratica->B1, dados.rQuadratica->B2);

  for (int i = 0; i < TAM; i++)
  {
    vetorUi[i] = dados.rQuadratica->B0 + (dados.rQuadratica->B1 * vetorX[i]) + (dados.rQuadratica->B2 * vetorXAoQuadrado[i]);
    vetorDi[i] = vetorY[i] - vetorUi[i];
    somatorioVetorDiAoQuadrado += pow(vetorDi[i], 2);
  }

  printf("\nUi\n");
  for (int i = 0; i < TAM; i++)
  {
    printf("%lf\n", vetorUi[i]);
  }

  printf("\nDi\n");
  for (int i = 0; i < TAM; i++)
  {
    printf("%lf\n", vetorDi[i]);
  }

  printf("\nNUMERO MAGICO: %lf\n", somatorioVetorDiAoQuadrado);
  
}


int main(char argc, char argv[]) {
  /* FILE* arquivo = fopen("dados.txt", "r");

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

  while (feof(arquivo) == 0)
  {
    for (int i = 0; i < TAM; i++)
    {
      fscanf(arquivo, "%d %lld\n", &vetorX[i], &vetorY[i]);
      xAoQuadrado[i] = vetorX[i] * vetorX[i];
      xAoCubo[i] = pow(vetorX[i], 3);
      xAQuarta[i] = pow(vetorX[i], 4);
      xVezesY[i] = vetorX[i] * vetorY[i];
    }
  }

  if (arquivo == NULL)
  {
    printf("Erro de alocacao ou Falha no arquivo \n");
    exit(1);
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

  regressaoLinear(matrizRegressaoLinear, vetorX, vetorY);

  ROWS = 3;
  COLS = 4;

  double matrizRegressaoQuadratica[3][4] = {
      {TAM, somatorioDeX, somatorioDeXAoQuadrado, somatorioDeY},
      {somatorioDeX, somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXVezesY},
      {somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXAQuarta, somatorioDeYVezesXAoQuadrado},
  };

  regressaoQuadratica(matrizRegressaoQuadratica);

  printf("\n");
  
  printf("Somatorio de X: %d\n", somatorioDeX);
  printf("Somatorio de X^2: %d\n", somatorioDeXAoQuadrado);
  printf("Somatorio de X^3: %lld\n", somatorioDeXAoCubo);
  printf("Somatorio de X^4: %lld\n", somatorioDeXAQuarta);

  printf("Somatorio de Y: %lld\n", somatorioDeY);
  printf("Somatorio de X*Y: %lld\n", somatorioDeXVezesY);
  printf("Somatorio de Y*X^2: %lld", somatorioDeYVezesXAoQuadrado); */

  FILE* arquivo = fopen("dados.txt", "r");

  double* vetorX = malloc(sizeof(double) * TAM);
  double* xAoQuadrado = malloc(sizeof(double) * TAM);
  double* xAoCubo = malloc(sizeof(double) * TAM);
  double* xAQuarta = malloc(sizeof(double) * TAM);

  double* vetorY = malloc(sizeof(double) * TAM);
  double* yAoQuadrado = malloc(sizeof(double) * TAM);
  double* xVezesY = malloc(sizeof(double) * TAM);

  double somatorioDeX = 0.0;
  double somatorioDeXAoQuadrado = 0.0;
  double somatorioDeXAoCubo = 0.0;
  double somatorioDeXAQuarta = 0.0;

  double somatorioDeY = 0.0;
  double somatorioDeXVezesY = 0.0;
  double somatorioDeYVezesXAoQuadrado = 0.0;
  double somatorioDeYAoQuadrado = 0.0;

  if (arquivo == NULL)
  {
    printf("Erro de alocacao ou Falha no arquivo \n");
    exit(1);
  }

  while (feof(arquivo) == 0)
  {
    for (int i = 0; i < TAM; i++)
    {
      fscanf(arquivo, "%lf %lf\n", &vetorX[i], &vetorY[i]);
      xAoQuadrado[i] = vetorX[i] * vetorX[i];
      xAoCubo[i] = pow(vetorX[i], 3);
      xAQuarta[i] = pow(vetorX[i], 4);
      xVezesY[i] = vetorX[i] * vetorY[i];
      yAoQuadrado[i] = pow(vetorY[i], 2);
    }
  }

  if (arquivo == NULL)
  {
    printf("Erro de alocacao ou Falha no arquivo \n");
    exit(1);
  }

  for (int i = 0; i < TAM; i++)
  {
    somatorioDeX += vetorX[i];
    somatorioDeXAoQuadrado += xAoQuadrado[i];
    somatorioDeXAoCubo += xAoCubo[i];
    somatorioDeXAQuarta += xAQuarta[i];

    somatorioDeY += vetorY[i];
    somatorioDeYAoQuadrado += yAoQuadrado[i];
    somatorioDeXVezesY += xVezesY[i];
    somatorioDeYVezesXAoQuadrado += vetorY[i] * xAoQuadrado[i];
  }

  /* printf("Somatorio de X: %lf\n", somatorioDeX);
  printf("Somatorio de X^2: %lf\n", somatorioDeXAoQuadrado);
  printf("Somatorio de X^3: %lf\n", somatorioDeXAoCubo);
  printf("Somatorio de X^4: %lf\n", somatorioDeXAQuarta);

  printf("Somatorio de Y: %lf\n", somatorioDeY);
  printf("Somatorio de X*Y: %lf\n", somatorioDeXVezesY);
  printf("Somatorio de Y*X^2: %lf", somatorioDeYVezesXAoQuadrado); */

  ROWS = 2;
  COLS = 3;

  double matrizRegressaoLinear[2][3] = {
      {TAM, somatorioDeX, somatorioDeY},
      {somatorioDeX, somatorioDeXAoQuadrado, somatorioDeXVezesY},
  };

  regressaoLinear(matrizRegressaoLinear, vetorX, vetorY, somatorioDeYAoQuadrado, somatorioDeY);

  ROWS = 3;
  COLS = 4;

  double matrizRegressaoQuadratica[3][4] = {
      {TAM, somatorioDeX, somatorioDeXAoQuadrado, somatorioDeY},
      {somatorioDeX, somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXVezesY},
      {somatorioDeXAoQuadrado, somatorioDeXAoCubo, somatorioDeXAQuarta, somatorioDeYVezesXAoQuadrado},
  };

  regressaoQuadratica(matrizRegressaoQuadratica, vetorX, vetorY, xAoQuadrado);

  printf("\n");

  fclose(arquivo);
  return 0;
}