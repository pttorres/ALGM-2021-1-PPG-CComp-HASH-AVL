/*
Trabalho sobre Tabela Hash com colisão linear e árvores AVL
Alunos: Piettro Alessandro Rodrigues Torres - mat. ME2112004, 
	     Roberto Carlos dos Santos - mat. ME2112005
Professor: Igor Machado Coelho
Disciplina: Algoritmos (Algoritmos-2021-1-PPG-CComp)
Data de apresentação: 15/09/2021
***
Observação sobre a notação utilizada para informar a complexidade do código da aplicação: toda linha que não contiver explicitamente a 
indicação de sua complexidade deve ser considerada como sendo de complexidade O(1). As linhas de código que tiverem complexidade diferente
disso terão explicitamente indicadas sua complexidade, em comentários ao lado do código, por exemplo:
      for (int i=0; i<n; i++){ // Complexidade do bloco: O(n^2) 
          for (int j=0; j<n; j++){ Complexidade do bloco: O(n)
              ...      // Nas linhas de complexidade O(1), não escreveremos nada ao lado
          }
      }

*/

#include <iostream> // biblioteca necessária para a função std::cout
#include <stdio.h>
#include <math.h> //abs por exemplo
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "classAVL.hpp"

#define QTD_PESSOAS 250 // número de pessoas na lista de nomes fictícios
//#define QTD_PESSOAS 16 // número de pessoas na lista de nomes fictícios

#define QTD_BEAMS 26    //número de compartimentos = número de letras do alfabeto
#define QTD_EXCLUSOES 230
#define QTD_BUSCAS 10

using namespace std; // declaração global do espaço de nomes

/* Loop de exclusão - sequência contínua de registros  */
void realizaExclusoes(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS])            //Complexidade: O(N)
{
    char letra = 'A';  // Para experimentar outras letras, trocar esta variável de acordo.
    int idBEAM = letra - 65;

    lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore(0);      //Complexidade: O(N)

    /* Loop por todas as ocorrências da letra em questão (no caso, letra A)*/
    for (int i = 0; i < 16; i++)    // Para experimentar outras letras, acertar esta faixa de valores
    {                                                                      //Complexidade deste bloco for: O(N)
        printf("\n\nNúmero do id a ser excluído: %d\n", i);

        string nome = strTrim(listaPessoas[i]->nome);
        int telefone = listaPessoas[i]->numTelefone;
        
        if (lstBEAMS[idBEAM]->T != NULL)
        {

            printf("Buscaremos, na lista, o(s) telefone(s) vinculados ao nome de %s:\n", nome.c_str());

            lstBEAMS[idBEAM]->chaveEncontrada = NULL;
            lstBEAMS[idBEAM]->buscaRegistrosPeloNome(nome, lstBEAMS[idBEAM]->T); //Deve retornar chaveEncontrada==true (se encontrar a chave, claro)

            //Demonstração do método de exclusão de elemento da árvore AVL

            if (lstBEAMS[idBEAM]->chaveEncontrada != NULL)
            {
                if (idBEAM >= 0 && idBEAM < QTD_BEAMS)
                {
                    printf("\nSelecionado para exclusão: %s,%d, do compartimento %d, letra %c:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);
                    
                    lstBEAMS[idBEAM]->exclui(&lstBEAMS[idBEAM]->T, telefone);           //Complexidade: O(N)
                    //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
                    printf("Excluído com sucesso o nome %s, telefone %d, do compartimento %d, letra %c. Veja, abaixo, a AVL do compartimento, atualizada após a exclusão:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);
                    lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);       //Complexidade: O(N)
                    lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore((i + 1));        //Complexidade: O(N)
                }
            }
            else
            {
                printf("Chave (telefone %d) não encontrada(o), nome: %s!\n", telefone, nome.c_str());
            }
        }
    }
}
/* Loop de inserção de pessoas 
Ao final, todas as pessoas fictícias estarão inseridas no catálogo, ou seja, tanto no vetor de pessoas quanto nas AVLs da lista
de compartimentos */
void realizaInsercoes(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){      //Complexidade: O(N^2) (observação: estamos contando
                                                                                         // com a impressão em níveis logo após a inserção.
                                                                                         // Sem essa impressão, a função fica com 
                                                                                         // complexidade O(N)   )
    // A partir daqui, começa o preenchimento dos compartimentos (BEAMS e respectivas AVLs) da hashtable
    printf("\n==================\nInício das inserções\n==================\n");
    printf("\nObservação: para a saída não ficar muito grande, só serão apresentadas as árvores de inserção com o valor de N entre 5 e 10.\n");

    // Insere os nomes de acordo com a primeira letra (O 'A' de ANDREA, por exemplo, é inserido na posição 0 o B na posição 1 e assim por diante)
    for (int i = 0; i < QTD_PESSOAS; i++)                   //Complexidade do bloco: O(N^2)
    {
        char letra = listaPessoas[i]->nome[0];
        int idBEAM = letra - 65;
        // Método insere da classe AVL:
        lstBEAMS[idBEAM]->insere(&lstBEAMS[idBEAM]->T, listaPessoas[i]);     //Complexidade: O(1)
        //if (lstBEAMS[idBEAM]->N > 5 && lstBEAMS[idBEAM]->N < 10){
        if (lstBEAMS[idBEAM]->N > 0 && lstBEAMS[idBEAM]->N < 17 && letra == 'A')
        {
            printf("\n\nInserção #%d: %s, %d no compartimento número %d, letra %c. Árvore AVL do compartimento atualizada :", i + 1, listaPessoas[i]->nome.c_str(), listaPessoas[i]->numTelefone, idBEAM, letra);
            lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);     //Complexidade: O(N)
            printf("\n");
            //lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore(i+1);
        }
        else
        {
            printf("\nInserção #%d: %s, %d no compartimento número %d, letra %c. Omitida a árvore atualizada após essa inserção, para economizar espaço.", i + 1, listaPessoas[i]->nome.c_str(), listaPessoas[i]->numTelefone, idBEAM, letra);
        }
    }
    printf("\n==================\nFim das inserções\n==================\n");
    printf("\n==================\nInício de testes de buscas e de exclusões\n==================\n");
}

/* Lê o arquivo de entrada (lista de nomes fictícios contidas no arquivo listaEntrada.csv) */ 
void leituraArquivoEntrada(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){      //Complexidade: O(N)
    //Leitura do arquivo de entrada:
    fstream arqEntrada;
    for (int i = 0; i < QTD_PESSOAS; i++)                //Complexidade: O(N)
    {
        if (listaPessoas[i] != NULL)
            listaPessoas[i] = NULL;
        listaPessoas[i] = new No_AVL();
    }
    
    for (int i = 0; i < QTD_BEAMS; i++)               //Complexidade: O(1) (dado que o valor de compartimentos é constante=26)
    {
        lstBEAMS[i] = new AVL();
    }    
    arqEntrada.open("listaEntrada.csv", ios::in); //Abre o arquivo com a lista de nomes e telefones
    if (arqEntrada.is_open())
    { //O arquivo foi aberto corretamente?
        string linha;
        string nome;
        int telefone;
        int i = 0;
        // Apenas carrega a lista de nomes e de telefones para a memória:
        printf("Lista de nomes e de telefones fictícios que serão incluídos na hashTable / AVLs:\n---------------------\n");
        while (getline(arqEntrada, linha))                                   //Complexidade: O(N)
        { //leitura das linhas do arquivo de entrada
            std::vector<std::string> nomeEtelefone = splitStr(linha, ';');
            if (i > 0 && i < QTD_PESSOAS+1)
            {
                nome = nomeEtelefone[0];
                listaPessoas[i - 1]->nome = strTrim(nome);
                telefone = std::stoi(nomeEtelefone[1]);
                listaPessoas[i - 1]->numTelefone = telefone;
                printf("%s", fixSize(listaPessoas[i - 1]->nome, 18).c_str());
                printf("\t");
                printf("%d", listaPessoas[i - 1]->numTelefone);
                printf("\n");
            }
            i++;
        }

        arqEntrada.close(); //fecha o arquivo de entrada.
    }
    else
    {
        printf("Erro: Problema na leitura do arquivo da lista de nomes! Terminando a execução...\n");
        return;
    }
}

/* Impressão da Floresta (lista telefônica completa), após inserções e exclusões */
void imprimeFloresta(AVL *lstBEAMS[QTD_BEAMS]){             //Complexidade: O(N)
    
    printf("\n\n\n---------\nImpressão da Floresta (ou seja, do catálogo inteiro), após inserções e exclusões\n---------\n");    
    for (int i=0; i< QTD_BEAMS; i++){                                       //Complexidade: O(N) (dado que QTD_BEAMS é constante = 26)
        if (lstBEAMS[i]->T != NULL && lstBEAMS[i]->N > 0){
            printf("\nImpressão em nível: ---------\n");
            printf("Compartimento %d, letra %c:", i, i+65);
            lstBEAMS[i]->imprimeVisitaEmNiveis(lstBEAMS[i]->T);            //Complexidade: O(N)
            lstBEAMS[i]->imprimeRepresentacaoGraficaArvore((i+1));          //Complexidade: O(N)
            printf("\n---------\n");
            printf("\nImpressão em ordem: ---------\n");
            lstBEAMS[i]->imprimeEmOrdem(lstBEAMS[i]->T);                    //Complexidade: O(N)
            printf("\n---------\n");
        }
    }
    
}
/* Realiza um ciclo de buscas por nomes aleatórios */
void realizaBuscasAleatoriasPeloNome(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){   //Complexidade: O(N^2)
    /* inicializa a semente (seed): */
    srand (time(NULL));
    for (int i=0; i< QTD_BUSCAS; i++){                     //Complexidade: O(N^2) (considerando-se que o valor de QTD_BUSCAS cresça 
                                                                                   // proporcionalmente ao valor de N )
        int idAleatorio;

        /* gera numero entre 0 e 249: */
        idAleatorio = rand() % 250;
        printf ("\n\nNúmero aleatório, entre 0 e 249, gerado para o id a ser procurado: %d\n", idAleatorio); 

        string nome=strTrim(listaPessoas[idAleatorio]->nome);
        int telefone=listaPessoas[idAleatorio]->numTelefone;
        char letra= nome[0];
        int idBEAM= letra-65; 
        if (lstBEAMS[idBEAM]->T !=NULL){   

            printf ("\nBuscaremos, na lista, o(s) telefone(s) vinculados ao nome de %s:\n", nome.c_str());

            lstBEAMS[idBEAM]->chaveEncontrada=NULL;

            //Deve retornar chaveEncontrada==true (se encontrar a chave, claro)
            lstBEAMS[idBEAM]->buscaRegistrosPeloNome(nome, lstBEAMS[idBEAM]->T);      //Complexidade: O(N)

            if (lstBEAMS[idBEAM]->chaveEncontrada==NULL){
                printf ("\nNão foi encontrado telefone vinculado ao nome de %s:\n", nome.c_str());
            }
        }
    }
}

/* Realiza um ciclo de buscas por números de telefone aleatórios */
void realizaBuscasAleatoriasPeloTelefone(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){    //Complexidade: O(N^2)
    /* inicializa a semente (seed): */
    srand (time(NULL));
    for (int i=0; i< QTD_BUSCAS; i++){         //Complexidade: O(N^2)
        int idAleatorio;

        /* gera numero entre 0 e 249: */
        idAleatorio = rand() % 250;
        printf ("\n\nNúmero aleatório, entre 0 e 249, gerado para o id a ser procurado: %d\n", idAleatorio); 

        string nome=strTrim(listaPessoas[idAleatorio]->nome);
        int telefone=listaPessoas[idAleatorio]->numTelefone;
        char letra= nome[0];
        int idBEAM= letra-65; 

        printf ("\nBuscaremos, na lista, o nome vinculado ao telefone %d:\n", telefone);
        /* Realiza a busca em cada um dos 26 compartimentos de letras do alfabeto */
        for (int j=0; j< QTD_BEAMS; j++){

            if (lstBEAMS[j]->T !=NULL){                 

                lstBEAMS[j]->chaveEncontrada=NULL;

                //Deve retornar chaveEncontrada==true (se encontrar a chave, claro)
                lstBEAMS[j]->buscaRegistrosPeloTelefone(telefone, lstBEAMS[j]->T);  //Complexidade: O(N)
                if (lstBEAMS[j]->chaveEncontrada!=NULL){
                    printf ("\nEncontrado nome de %s vinculado ao telefone %d:\n",lstBEAMS[j]->chaveEncontrada->nome.c_str(), telefone);
                    break;
                } else {
                    printf ("\nNão foi encontrado nome vinculado ao telefone %d, na letra %c:\n", telefone, j+65);
                }
            }
        }
    }
}

/* Realiza um loop de exclusões aleatórias. Exclui tantos elementos quanto definidos em QTD_EXCLUSOES */
void realizaExclusoesAleatorias(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){    //Complexidade: O(N^2) 

    /* inicializa a semente (seed): */
    srand (time(NULL));
    for (int i=0; i< QTD_EXCLUSOES; i++){        //Complexidade: O(N^2)  (considerando-se QTD_EXCLUSOES próximo ao valor de N)
        int idAleatorio;

        /* gera numero entre 0 e 249: */
        idAleatorio = rand() % 250;
        printf ("\n\nNúmero aleatório, entre 0 e 249, gerado para o id a ser excluído: %d\n", idAleatorio); 

        string nome=strTrim(listaPessoas[idAleatorio]->nome);
        int telefone=listaPessoas[idAleatorio]->numTelefone;
        char letra= nome[0];
        int idBEAM= letra-65; 

        if (lstBEAMS[idBEAM]->T !=NULL){   

            printf ("Buscaremos, na lista, o(s) telefone(s) vinculados ao nome de %s:\n", nome.c_str());

            lstBEAMS[idBEAM]->chaveEncontrada=NULL;

            //Deve retornar chaveEncontrada==true (se encontrar a chave, claro)
            lstBEAMS[idBEAM]->buscaRegistrosPeloNome(nome, lstBEAMS[idBEAM]->T);  //Complexidade: O(N)

            //Demonstração do método de exclusão de elemento da árvore AVL       
            if( lstBEAMS[idBEAM]->chaveEncontrada!=NULL){
                if(idBEAM >=0 && idBEAM < QTD_BEAMS){   
                    printf ("\nSelecionado para exclusão: %s,%d, do compartimento %d, letra %c:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);                               
                    lstBEAMS[idBEAM]->exclui(&lstBEAMS[idBEAM]->T, telefone);      //Complexidade: O(N)         
                    //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
                    printf("Excluído com sucesso o nome %s, telefone %d, do compartimento %d, letra %c. Veja, abaixo, a AVL do compartimento, atualizada após a exclusão:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);
                    lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);   //Complexidade: O(N)
                    //lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore(i);       //Complexidade: O(N)                     
                } 
            } else {
                printf("Chave (telefone %d) não encontrada(o), nome: %s!\n", telefone, nome.c_str());
            }           
        }
    }
}

/* Módulo principal do aplicativo (Main) */
int main()
{    
    //Define a lista de pessoas
    No_AVL *listaPessoas[QTD_PESSOAS];
    //Define a lista de compartimentos cujos nós são as raízes de árvores AVL.
    AVL *lstBEAMS[QTD_BEAMS];

    //Leitura do arquivo de entrada:  
    leituraArquivoEntrada(lstBEAMS, listaPessoas);                                     //Complexidade: O(N)
    //Realiza as inserções na lista de pessoas e na lista de árvores AVL
    realizaInsercoes(lstBEAMS, listaPessoas);                                          //Complexidade: O(N^2) 
    //Realiza uma série de exclusões sequenciais
    realizaExclusoes(lstBEAMS, listaPessoas);                                          //Complexidade: O(N)
    //Realiza um ciclo de buscas aleatórias pelo nome
    realizaBuscasAleatoriasPeloNome(lstBEAMS, listaPessoas);                           //Complexidade: O(N^2)
    //Realiza um ciclo de buscas aleatórias pelo número de telefone
    realizaBuscasAleatoriasPeloTelefone(lstBEAMS, listaPessoas);                       //Complexidade: O(N^2)
    //Realiza uma série de exclusões aleatórias
    realizaExclusoesAleatorias(lstBEAMS, listaPessoas);                                //Complexidade: O(N^2)
    //Imprime toda a floresta de árvores AVL
    // (ou seja, imprime todo o catálogo de telefones e 
    //  respectivos nomes associados a eles)
    imprimeFloresta(lstBEAMS);                                                         //Complexidade: O(N)
          
    /* Libera memória ocupada pelas AVLs contidas na listaPessoas 
    // (ou seja: de todos os elementos da lista) */
    for (int i = 0; i < QTD_PESSOAS; i++)                                             //Complexidade: O(N)  
    {
        delete (listaPessoas[i]);
    }
    return 0;
}