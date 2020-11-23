#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0
#define TAMANHOCSV 202633

typedef struct
{
    char municipio[50];
    int quantidadeCasos;

} CasosMunicipio;

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
int TopNcasos(DadosCsv vetorCsv[], int vetorN[], int N);
void ContaNumerosDeCasosPorMunicipio(CasosMunicipio vetorCasos[], DadosCsv vetorCsv[]);

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
  CasosMunicipio vetorCasos[78];

  leCsv(csv, vetorCsv);
  //mostraCsv(vetorCsv);
  ContaNumerosDeCasosPorMunicipio(vetorCasos, vetorCsv);
  printf("%s %d", vetorCasos[0].municipio, vetorCasos[0].quantidadeCasos);

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
        fscanf(csv, "%[^\n]", vetorCsv[i].ficouInternado);
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

void ContaNumerosDeCasosPorMunicipio(CasosMunicipio vetorCasos[], DadosCsv vetorCsv[]){
    int i; 
    for(i = 0; i < 10; i++){
        if(comparaStrings(vetorCsv[i].municipio, "AFONSO CLAUDIO") == 0){
            vetorCasos[i].quantidadeCasos += 1; 
            strcpy(vetorCasos[i].municipio, "AFONSO CLAUDIO");
        }
    }
}

int comparaStrings(char string1[], char string2[])
{
  int i;
  // compara caractere a caractere da string
  for (i = 0; string1[i] != '\0' && string2 != '\0'; i++)
  {
    if (string1[i] < string2[i])
      return -1;
    else if (string1[i] > string2[i])
      return 1;
  }

  // se as cadeias tem o mesmo tamanho
  if (string1[i] == string2[i])
    return 0;
  else if (string2[i] != '\0') // se a string 1 é menor
    return -1;
  else 
    return 1; // se a string2 é menor
}
