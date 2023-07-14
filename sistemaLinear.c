#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int COLS;
int ROWS;

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

void gaussElimination(double M[ROWS][COLS])
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
        print_matrix(M2);
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

  printf("\nThe solution is: \n");
  for (int i = 0; i < ROWS; i++)
  {
    printf("\nx%d=%lf\t", i, x[i]);
  }
  printf("\n\n");
}

int main(int argc, char *argv[])
{

  int N_SISTEMAS, DIMENSAO;
  double PRECISAO;

  FILE *arquivo = fopen(argv[1], "r");
  fscanf(arquivo, "%d %d %lf\n", &N_SISTEMAS, &DIMENSAO, &PRECISAO);

  ROWS = DIMENSAO;
  COLS = DIMENSAO + 1;

  double A[ROWS][COLS];

  for (int i = 0; i < DIMENSAO; i++)
  {
    for (int j = 0; j < DIMENSAO; j++)
    {
      fscanf(arquivo, "%lf", &A[i][j]);
    }
  }

  for (int sistema = 0; sistema < N_SISTEMAS; sistema++)
  {
    for (int x = 0; x < DIMENSAO; x++)
    {
      fscanf(arquivo, "%lf", &A[x][DIMENSAO]);
    }

    print_matrix(A);
    printf("\n");

    gaussElimination(A);
  }
}