#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0
#define TAMANHOCSV 202633

typedef struct
{
  int dia;
  int mes;
  int ano;
} Data;

typedef struct
{
  char comorbidadePulmao[5];
  char comorbidadeCardio[5];
  char comorbidadeRenal[5];
  char comorbidadeDiabetes[5];
  char comorbidadeTabagismo[5];
  char comorbidadeObesidade[5];
} Comorbidades;

typedef struct
{
  Data dataCadastro;
  Data dataObito;
  Comorbidades comorbidadesPacientes;
  int idadePessoa;
  char classificacao[60];
  char municipio[50];
  char ficouInternado[20];
} DadosCsv;

void leCsv(FILE *csv, DadosCsv vetorCsv[]);
void mostraCsv(DadosCsv vetorCsv[]);

int main(void)
{
  FILE *csv;
  csv = fopen("covid19ES_ordenado.csv", "r");

  if (csv == NULL)
  {
    printf("Nao foi possivel abrir o arquivo!");
    exit(1);
  }

  DadosCsv vetorCsv[202];

  leCsv(csv, vetorCsv);
  mostraCsv(vetorCsv);

  fclose(csv);

  return 0;
}

void leCsv(FILE *csv, DadosCsv vetorCsv[])
{
    int i;
    for(i = 0; i < 100; i++){
        fscanf(csv, "%d-%d-%d,%d-%d-%d,", &vetorCsv[i].dataCadastro.ano, &vetorCsv[i].dataCadastro.mes, &vetorCsv[i].dataCadastro.dia, &vetorCsv[i].dataObito.ano, &vetorCsv[i].dataObito.mes, &vetorCsv[i].dataObito.dia);
        fscanf(csv, "%[^,],", vetorCsv[i].classificacao);        
        fscanf(csv, "%[^,],", vetorCsv[i].municipio);
        fscanf(csv, "\"%d %*[^,], %*[^,], %*[^,],", &vetorCsv[i].idadePessoa);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadePulmao);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeCardio);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeRenal);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeDiabetes);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeTabagismo);
        fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeObesidade);
        fscanf(csv, "%4s %*[^\n]", vetorCsv[i].ficouInternado);
    }    
}

void mostraCsv(DadosCsv vetorCsv[])
{
    int i;
    for(i = 0; i < 100; i++){
        printf("data cadastro = %d%d%d\n", vetorCsv[i].dataCadastro.dia, vetorCsv[i].dataCadastro.mes, vetorCsv[i].dataCadastro.ano);
        printf("data obito = %d%d%d\n", vetorCsv[i].dataObito.dia, vetorCsv[i].dataObito.mes, vetorCsv[i].dataObito.ano);
        printf("classificacao = %s\n", vetorCsv[i].classificacao);
        printf("municipio = %s\n", vetorCsv[i].municipio);
        printf("idade pessoa = %d\n", vetorCsv[i].idadePessoa);
        printf("comorbidadePulmao = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadePulmao);
        printf("comorbidadeRenal = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeRenal);
        printf("comorbidadeCardio = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeCardio);
        printf("comorbidadeDiabetes = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeDiabetes);
        printf("comorbidadeTabagismo = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeTabagismo);
        printf("comorbidadeObesidade = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeObesidade);
        printf("ficou internado = %s\n\n", vetorCsv[i].ficouInternado);
    }
}
