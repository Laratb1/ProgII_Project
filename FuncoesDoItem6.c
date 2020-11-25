#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define True 1
#define False 0
#define TAMANHOCSV 202363

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

DadosCsv vetorCsv[TAMANHOCSV];

void leCsv(FILE *csv);
void mostraCsv(DadosCsv vetorCsv[]);
int comparaStrings(char string1[], char string2[]);
double CalculaPorcentagem(double valor1, double valor2);
double PorcentagemPessoasConfirmadasInternadas(char Municipio[]);
double PorcentagemPessoasQueMorreram(char Municipio[]);
double PorcentagemPessoasInterndasQueMorreram(char Municipio);



int main(void)
{
  FILE *csv;
  csv = fopen("/home/lara/trabalho/covid19ES_ordenado.csv", "r");
  int c;

  if (csv == NULL)
  {
    printf("Nao foi possivel abrir o arquivo!");
    exit(1);
  }

  
  char Municipio[20];
  double percent;

  leCsv(csv);
  
  // para testar as funcoes   
  scanf("%s", Municipio);
  percent =  PorcentagemPessoasConfirmadasInternadas(Municipio);  
  printf("%.3f", percent);

  fclose(csv);

  return 0;
}

void leCsv(FILE *csv)
{
  int i;

  // ignorando primeira linha do csv
  fscanf(csv, "%*[^\n]");

  // lendo os dados do csv para o vetor de struct
  for (i = 0; i < TAMANHOCSV; i++)
  {
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
    fscanf(csv, "%[^\n]", vetorCsv[i].ficouInternado);
  }
}

void mostraCsv(DadosCsv vetorCsv[])
{
  int i;
  for (i = 0; i < TAMANHOCSV; i++)
  {
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

double CalculaPorcentagem(double valor1, double valor2)
{
  return (valor1 / valor2) * 100.00;
}

double PorcentagemPessoasConfirmadasInternadas(char Municipio[])
{
  int i;
  double CasosConfirmados = 0, PessoasInternadasConfirmadas = 0;
  for(i = 0; i < TAMANHOCSV; i++)
  {
    if(strcmp(Municipio, "TODAS") == 0)
    {
      if(strcmp(vetorCsv[i].classificacao, "Confirmados") == 0)
      {
        CasosConfirmados++;
      }
      if((strcmp(vetorCsv[i].classificacao, "Confirmados") == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0))
      {
        PessoasInternadasConfirmadas++;
      }
    }
    else
    {
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0))
      {
        CasosConfirmados++;
      }
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0))
      {
        PessoasInternadasConfirmadas++;
      }     
    }
  }
  return CalculaPorcentagem(PessoasInternadasConfirmadas, CasosConfirmados);
}

double PorcentagemPessoasQueMorreram(char Municipio[])
{
  int i;
  double PessoasMorreram = 0, TotalCasos = 0; 
  for(i = 0; i < TAMANHOCSV; i++)
  {
    if(strcmp(Municipio, "TODAS") == 0)
    {
      // coloquei essa parte pois aparentemente o csv nao tem 202363
      TotalCasos += 1;
      if(vetorCsv[i].dataObito.dia != 0);
      {
        PessoasMorreram += 1;
      }
      
    }
    else
    {
      // coloquei essa parte pois aparentemente o csv nao tem 202363
      TotalCasos += 1;
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (vetorCsv[i].dataObito.dia != 0))
      {
        PessoasMorreram += 1;
      }
      
    }
  }
  return CalculaPorcentagem(PessoasMorreram, TotalCasos);
}

double PorcentagemPessoasInterndasQueMorreram(char Municipio)
{
  int i;
  double PessoasInternadasMorreram = 0, PessoasInternadas = 0;
  for(i = 0; i < TAMANHOCSV; i++)
  {
    if(strcmp(Municipio, "TODAS") == 0)
    {
      if(strcmp(vetorCsv[i].ficouInternado, "Sim") == 0)
      {
        PessoasInternadas += 1;
      }
      if((strcmp(vetorCsv[i].ficouInternado, "Sim") == 0) && (vetorCsv[i].dataObito.dia != 0))
      {
        PessoasInternadasMorreram += 1;
      }
    }
    else
    {
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0))
      {
        PessoasInternadas += 1;
      }
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0) && (vetorCsv[i].dataObito.dia != 0))
      {
        PessoasInternadasMorreram += 1;
      }     
    }
  }
  return CalculaPorcentagem(PessoasInternadasMorreram, PessoasInternadas);
}

