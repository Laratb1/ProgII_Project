#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define True 1
#define False 0
#define TAMANHOCSV 202233

typedef struct 
{
  int numeroDeCasos;
  char municipio[50];
} CasosMunicipio;


// criando struct ara trabalhar com datas
typedef struct
{
  int dia;
  int mes;
  int ano;
} Data;

// criando struct para as comorbidades
typedef struct
{
  char comorbidadePulmao[5];
  char comorbidadeCardio[5];
  char comorbidadeRenal[5];
  char comorbidadeDiabetes[5];
  char comorbidadeTabagismo[5];
  char comorbidadeObesidade[5];
} Comorbidades;

// criando struct principal para armazenar todos os dados do csv
typedef struct
{
  Data dataCadastro;
  Data dataObito;
  Comorbidades comorbidadesPacientes;
  int idadePessoa;
  char classificacao[13];
  char municipio[30];
  char ficouInternado[15];
} DadosCsv;

// criando vetor csv como global
DadosCsv vetorCsv[TAMANHOCSV];
// criando vetor que contém o numero de casos eo municipio
CasosMunicipio vetorMunicipios[78];
// criando matriz com todos os municipios do espirito santo em ordem alfabetica
char municipiosEspiritoSanto[78][30] =
{
  "AFONSO CLAUDIO", "AGUA DOCE DO NORTE", "AGUIA BRANCA", "ALEGRE",
  "ALFREDO CHAVES", "ALTO RIO NOVO", "ANCHIETA", "APIACA", "ARACRUZ",
  "ATILIO VIVACQUA", "BAIXO GUANDU", "BARRA DE SAO FRANCISCO",
  "BOA ESPERANCA", "BOM JESUS DO NORTE", "BREJETUBA", "CACHOEIRO DE ITAPEMIRIM",
  "CARIACICA", "CASTELO", "COLATINA", "CONCEICAO DA BARRA", "CONCEICAO DO CASTELO",
  "DIVINO DE SAO LOURENCO", "DOMINGOS MARTINS", "DORES DO RIO PRETO", "ECOPORANGA",
  "FUNDAO", "GOVERNADOR LINDENBERG", "GUACUI", "GUARAPARI", "IBATIBA", "IBIRACU",
  "IBITIRAMA", "ICONHA", "IRUPI", "ITAGUACU", "ITAPEMIRIM", "ITARANA", "IUNA",
  "JAGUARE", "JERONIMO MONTEIRO", "JOAO NEIVA", "LARANJA DA TERRA", "LINHARES",
  "MANTENOPOLIS", "MARATAIZES", "MARECHAL FLORIANO", "MARILANDIA", "MIMOSO DO SUL",
  "MONTANHA", "MUCURICI", "MUNIZ FREIRE", "MUQUI", "NOVA VENECIA", "PANCAS",
  "PEDRO CANARIO", "PINHEIROS", "PONTO BELO", "PRESIDENTE KENNEDY", "RIO BANANAL",
  "RIO NOVO DO SUL", "SANTA LEOPOLDIN", "SANTA MARIA DE JETIBA", "SANTA TERESA",
  "SAO DOMINGOS DO NORTE", "SAO GABRIEL DA PALHA", "SAO JOSE DO CALCADO",
  "SAO MATEUS",
  "SAO ROQUE DO CANAA", "SERRA", "SOORETAMA", "VARGEM ALTA", "VENDA NOVA DO IMIGRANTE",
  "VIANA", "VILA PAVAO", "VILA VALERIO", "VILA VELHA", "VITORIA"
};

// função que realiza a leitura do csv
void leCsv(FILE *csv);
// função que mostra os dados do csv
void mostraCsv(void);
// função que retorna a quantidade de dias de um dado mes
int quantidadeDiasDoMes(int mes);
// função que verifica se as datas são iguais
int ehMesmaData(Data data1, Data data2);
// função que passa para a próxima data
Data proximaData(Data dataInicial);
// função que determina a quantidade de casos
int calculaQuantidadeDeCasosComDatas(Data dataInicial, Data dataFinal, char municipio[]);
// função que lista em ordem alfabetica as cidades com mais de N casos
void listaCasosEmOrdemAlfabetica(int n);
// função que lista as Top N cidades
void listaTopNCidades(int n, Data data1, Data data2);
// função quel calcula o total de casos, dado um municipio
int calculaTotalCasos(char municipio[]);
// função que mostra a quantidade total de casos entre a data inicial e final
void mostraTotalCasosEntreDatas(Data dataInicial, Data dataFinal);

float calculaMediaDeIdadeEntreDatas(Data dataInicial, Data dataFinal);

float calculaDesvioPradraoEntreDatas(Data dataInicial, Data dataFinal);

int main(void)
{
  // abrindo o arquivo csv em modo leitura
  FILE *csv;
  csv = fopen("covid19ES.csv", "r");
  double media;

  // caso não seja possivel abrir o arquivo, o programa é abortado
  if (csv == NULL)
  {
    printf("Nao foi possivel abrir o arquivo!");
    exit(1);
  }

  // criando datas de teste
  Data data1, data2;
  data1.ano = data2.ano = 2020;
  data1.dia = data2.dia = 10;
  data1.mes = 7;
  data2.mes = 8;

  // lendo csv
  leCsv(csv);
 
  media = calculaDesvioPradraoEntreDatas(data1, data2);
  printf("%.3f", media);

  // fechando arquivo csv
  fclose(csv);

  return 0;
}

int quantidadeDiasDoMes(int mes)
{
  // se for fevereiro
  if (mes == 2)
    return 29;
  // se for o mês 4, 6, 9, ou 11
  else if ((mes == 4) || (mes == 6) || (mes == 9) || (mes == 11))
    return 30;
  // caso seja outro mês
  else
    return 31;
}

int ehMesmaData(Data data1, Data data2)
{
  // se os dias e meses da data inicial e final sejam iguais
  return ((data1.dia == data2.dia) && (data1.mes == data2.mes)) ? True : False;
}

Data proximaData(Data dataInicial)
{
  // encontrando a quantidade de dias da data inicial
  int quantidadeDiasDAtaInicial = quantidadeDiasDoMes(dataInicial.mes);

  // se o mês for 12 e a data esteja no seu limite de dias
  if (dataInicial.mes == 12 && quantidadeDiasDAtaInicial == dataInicial.dia)
  {
    // incrementando dia mês e ano
    dataInicial.dia = 1;
    dataInicial.mes = 1;
    dataInicial.ano++;
  }
  // caso contrário, se apenas a quantidade de dias estiver no limite
  else if (dataInicial.dia == quantidadeDiasDAtaInicial)
  {
    // incrementando dia e mes
    dataInicial.dia = 1;
    dataInicial.mes++;
  }
  // se houver menos dias do que a quantidade máxima de dias da data
  else if (dataInicial.dia < quantidadeDiasDAtaInicial)
  // incrementando os dias
    dataInicial.dia++;
  // se o mês for menor que 12
  else if (dataInicial.mes < 12)
    // incrementando o mês
    dataInicial.mes++;

  return dataInicial;
}

int calculaQuantidadeDeCasosComDatas(Data dataInicial, Data dataFinal, char municipio[])
{
  // declarando variaveis para o programa
  int i, quantidadeDeCasos = 0;

  // loop de uma data até a outra
  for (dataInicial; !ehMesmaData(dataInicial, dataFinal); dataInicial =  proximaData(dataInicial))
  {
    // percorrendo o vetor do csv
    for (i = 0; i < TAMANHOCSV; i++)
    {
      // se a data do cadastro for igual a data no loop
      if (ehMesmaData(vetorCsv[i].dataCadastro, dataInicial))
      {
        // se o município no vetor do csv for igual ao municipio informado
        if (strcmp(vetorCsv[i].municipio, municipio) == 0)
        {
        // caso a classificação esteja confirmada, incrementamos a quantidade de casos
          if (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0)
            quantidadeDeCasos++;
        }
      }
    }
  }

  return quantidadeDeCasos;
}

int calculaTotalCasos(char municipio[])
{
  int i, total = 0;

  // percorrendo o vetor todo
  for (i = 0; i < TAMANHOCSV; i++)
  {
    // se o municipio passado for igual ao municipio no vetor
    if (strcmp(municipio, vetorCsv[i].municipio) == 0)
    {
      // se a classificação for confirmada, incrementamos o total de casos
      if (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0)
      {
        total++;
      }
    }
  }

  return total;
}

void mostraTotalCasosEntreDatas(Data dataInicial, Data dataFinal)
{
  int i, totalCasos = 0;

  // loop de uma data até a outra
  for (dataInicial; !ehMesmaData(dataInicial, dataFinal); dataInicial =  proximaData(dataInicial))
  {
    // percorrendo o vetor do csv
    for (i = 0; i < TAMANHOCSV; i++)
    {
      // se a data do cadastro for igual a data no loop
      if (ehMesmaData(vetorCsv[i].dataCadastro, dataInicial))
      {
        // caso a classificação esteja confirmada, incrementamos a quantidade de casos
        if (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0)
        {
          totalCasos++;
        }
      }
    }
  }

  printf("total casos = %d\n\n", totalCasos);
}

void listaCasosEmOrdemAlfabetica(int n)
{
  int i, total;

  // percorrendo a matriz com os nomes das cidades em ordem alfabetica
  for (i = 0; i < 78; i++)
  {
    // calculando o total para a cidade na posição de i
    total = calculaTotalCasos(municipiosEspiritoSanto[i]);
    // caso o total seja maior, imprimimos seu valor
    if (total > n)
    {
      printf("%s, %d\n", municipiosEspiritoSanto[i], total);
    }
  }
}

void listaTopNCidades(int n, Data data1, Data data2)
{
  
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

void mostraCsv()
{
  int i;

  for (i = 0; i < TAMANHOCSV; i++)
  {
    printf("linha %d\n", i);
    printf("data cadastro = %d-%d-%d\n", vetorCsv[i].dataCadastro.dia, vetorCsv[i].dataCadastro.mes, vetorCsv[i].dataCadastro.ano);
    printf("data obito = %d-%d-%d\n", vetorCsv[i].dataObito.dia, vetorCsv[i].dataObito.mes, vetorCsv[i].dataObito.ano);
    printf("classificacao = %s\n", vetorCsv[i].classificacao);
    printf("municipio = %s\n", vetorCsv[i].municipio);
    printf("idade pessoa = %d\n", vetorCsv[i].idadePessoa);
    printf("comorbidadePulmao = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadePulmao);
    printf("comorbidadeCardio = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeCardio);
    printf("comorbidadeRenal = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeRenal);
    printf("comorbidadeDiabetes = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeDiabetes);
    printf("comorbidadeTabagismo = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeTabagismo);
    printf("comorbidadeObesidade = %s\n", vetorCsv[i].comorbidadesPacientes.comorbidadeObesidade);
    printf("ficou internado = %s\n\n", vetorCsv[i].ficouInternado);
  }
}


float calculaMediaDeIdadeEntreDatas(Data dataInicial, Data dataFinal)
{
  // inicializando variaveis 
  int i;
  float soma = 0, totalPessoas = 0;
  Data data1;
  data1.dia = 0;
  data1.mes = 0;
  data1.ano = 0;

  // loop de uma data até a outra
  for (dataInicial; !ehMesmaData(dataInicial, dataFinal); dataInicial =  proximaData(dataInicial))
  {
    // percorrendo o vetor do csv
    for (i = 0; i < TAMANHOCSV; i++)
    {
      // se a data do cadastro for igual a data no loop
      if (ehMesmaData(vetorCsv[i].dataCadastro, dataInicial))
      {
        if(!ehMesmaData(vetorCsv[i].dataObito, data1) && (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0))
        {
          soma += vetorCsv[i].idadePessoa;
          totalPessoas++;
        }
      } 
    }
  }
  // retornando a media das idades
  return (soma / totalPessoas); 
}

float calculaDesvioPradraoEntreDatas(Data dataInicial, Data dataFinal)
{
  // inicializando variaveis 
  int i;
  float totalPessoas = 0, denominador = 0, desvioPadrao = 0, media = 0 ;
  Data data1;
  data1.dia = 0;
  data1.mes = 0;
  data1.ano = 0;

  // loop de uma data até a outra
  for (dataInicial; !ehMesmaData(dataInicial, dataFinal); dataInicial =  proximaData(dataInicial))
  {
    // percorrendo o vetor do csv
    for (i = 0; i < TAMANHOCSV; i++)
    {
      // se a data do cadastro for igual a data no loop
      if (ehMesmaData(vetorCsv[i].dataCadastro, dataInicial))
      {
        // se a pessoa veio a obito com covid
        if(!ehMesmaData(vetorCsv[i].dataObito, data1) && (strcmp(vetorCsv[i].classificacao, "Confirmados") == 0))
        {
          // incrementando denominador do calculo de desvio padrao 
          totalPessoas++;
          denominador += pow((vetorCsv[i].idadePessoa - calculaMediaDeIdadeEntreDatas(dataInicial, dataFinal)*(-1)), 2);
        }
      } 
    }
  } 
  // retornando o desvio padrao das idades
  return sqrt(denominador / totalPessoas);
}
