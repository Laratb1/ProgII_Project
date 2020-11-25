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
double PorcentagemPessoasInterndasQueMorreram(char Municipio[]);
void converteStringMaiusculo(char string[]);
int ehMesmaData(Data data1, Data data2);

int main(void)
{
  FILE *csv;
  csv = fopen("covid19ES.csv", "r");
  

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
  percent =  PorcentagemPessoasInterndasQueMorreram(Municipio);  
  printf("%.3f%%", percent);

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
    fscanf(csv, "%*c%d %*[^\"]%*c,", &vetorCsv[i].idadePessoa);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadePulmao);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeCardio);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeRenal);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeDiabetes);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeTabagismo);
    fscanf(csv, "%[^,],", vetorCsv[i].comorbidadesPacientes.comorbidadeObesidade);
    fscanf(csv, "%[^\n]%*c", vetorCsv[i].ficouInternado);
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

void converteStringMaiusculo(char string[])
{
  int i;
  for (i = 0; i < strlen(string); i++)
  {
    string[i] = toupper(string[i]);
  }
}

int ehMesmaData(Data data1, Data data2)
{
  // se os dias e meses da data inicial e final sejam iguais
  return ((data1.dia == data2.dia) && (data1.mes == data2.mes)) ? True : False;
}

double PorcentagemPessoasConfirmadasInternadas(char Municipio[])
{
  int i;
  double CasosConfirmados = 0, PessoasInternadasConfirmadas = 0;
  
  converteStringMaiusculo(Municipio);
  
  if(strcmp(Municipio, "TODAS") == 0)
  {
    for(i = 0; i < TAMANHOCSV; i++)
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
  }
  else
  {
    for(i = 0; i < TAMANHOCSV; i++)
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
  double PessoasMorreram = 0; 
  Data data1;
  data1.dia = 0;
  data1.mes = 0;
  data1.ano = 0;

  converteStringMaiusculo(Municipio);
  
  if(strcmp(Municipio, "TODAS") == 0)
  {
    for(i = 0; i < TAMANHOCSV; i++)
    {
      if(!(ehMesmaData(vetorCsv[i].dataObito, data1)));
      {
        PessoasMorreram++;
      }
    }
  }
  else
  {
    for(i = 0; i < TAMANHOCSV; i++)
    {
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (!(ehMesmaData(vetorCsv[i].dataObito, data1))))
      {
        PessoasMorreram++;
      }    
    }
  }
  return CalculaPorcentagem(PessoasMorreram, 202362);
}

double PorcentagemPessoasInterndasQueMorreram(char Municipio[])
{
  int i;
  double PessoasInternadasMorreram = 0, PessoasInternadas = 0;
  Data data1;
  data1.dia = 0;
  data1.mes = 0;
  data1.ano = 0;

  converteStringMaiusculo(Municipio);
   
  if(strcmp(Municipio, "TODAS") == 0)
  {
    for(i = 0; i < TAMANHOCSV; i++)
    {
      if((strcmp(vetorCsv[i].ficouInternado, "Sim") == 0) && (!(ehMesmaData(vetorCsv[i].dataObito, data1))))
      {
        PessoasInternadasMorreram++;
      }
      else if(strcmp(vetorCsv[i].ficouInternado, "Sim") == 0)
      {
        PessoasInternadas++;
      }
    }
  }
  else
  {
    for(i = 0; i < TAMANHOCSV; i++)
    {
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0))
      {
        PessoasInternadas++;
      }
      if((strcmp(vetorCsv[i].municipio, Municipio) == 0) && (strcmp(vetorCsv[i].ficouInternado, "Sim") == 0) && (!(ehMesmaData(vetorCsv[i].dataObito, data1))))
      {
        PessoasInternadasMorreram++;
      }     
    }
  }
  return CalculaPorcentagem(PessoasInternadasMorreram, PessoasInternadas);
}


