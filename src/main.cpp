#include <iostream> // biblioteca necessária para a função std::cout
#include <stdio.h>
#include <math.h> //abs por exemplo
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "classAVL.hpp"

//#define QTD_PESSOAS 250 // número de pessoas na lista de nomes fictícios
#define QTD_PESSOAS 16 // número de pessoas na lista de nomes fictícios

#define QTD_BEAMS 26    //número de compartimentos = número de letras do alfabeto
#define QTD_EXCLUSOES 150

using namespace std; // declaração global do espaço de nomes

void realizaExclusoes(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS])
{

    char letra = 'A';
    int idBEAM = letra - 65;

    lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore(0);

    for (int i = 0; i < QTD_PESSOAS; i++)
    {
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
                    
                    lstBEAMS[idBEAM]->exclui(&lstBEAMS[idBEAM]->T, telefone);
                    //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
                    printf("Excluído com sucesso o nome %s, telefone %d, do compartimento %d, letra %c. Veja, abaixo, a AVL do compartimento, atualizada após a exclusão:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);
                    lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);
                    lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore((i + 1));
                }
            }
            else
            {
                printf("Chave (telefone %d) não encontrada(o), nome: %s!\n", telefone, nome.c_str());
            }
        }
    }
}
void realizaInsersoes(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){
       // A partir daqui, começa o preenchimento dos compartimentos (BEAMS e respectivas AVLs) da hashtable
    printf("\n==================\nInício das inserções\n==================\n");
    printf("\nObservação: para a saída não ficar muito grande, só serão apresentadas as árvores de inserção com o valor de N entre 5 e 10.\n");

    // Insere os nomes de acordo com a primeira letra (O 'A' de ANDREA, por exemplo, é inserido na posição 0 o B na posição 1 e assim por diante)
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        char letra = listaPessoas[i]->nome[0];
        int idBEAM = letra - 65;
        // Método insere da classe AVL:
        lstBEAMS[idBEAM]->insere(&lstBEAMS[idBEAM]->T, listaPessoas[i]);
        //if (lstBEAMS[idBEAM]->N > 5 && lstBEAMS[idBEAM]->N < 10){
        if (lstBEAMS[idBEAM]->N > 0 && lstBEAMS[idBEAM]->N < 17 && letra == 'A')
        {
            printf("\n\nInserção #%d: %s, %d no compartimento número %d, letra %c. Árvore AVL do compartimento atualizada :", i + 1, listaPessoas[i]->nome.c_str(), listaPessoas[i]->numTelefone, idBEAM, letra);
            lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);
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

void leituraArquivoEntrada(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){
    //Leitura do arquivo de entrada:
    fstream arqEntrada;
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        if (listaPessoas[i] != NULL)
            listaPessoas[i] = NULL;
        listaPessoas[i] = new No_AVL();
    }
    
    for (int i = 0; i < QTD_BEAMS; i++)
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
        while (getline(arqEntrada, linha))
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
void imprimeFloresta(AVL *lstBEAMS[QTD_BEAMS]){
    
    printf("\n\n\n---------\nImpressão da Floresta (ou seja, do catálogo inteiro), após inserções e exclusões\n---------\n");    
    for (int i=0; i< QTD_BEAMS; i++){
        if (lstBEAMS[i]->T != NULL && lstBEAMS[i]->N > 0){
            printf("\nImpressão em nível: ---------\n");
            printf("Compartimento %d, letra %c:", i, i+65);
            lstBEAMS[i]->imprimeVisitaEmNiveis(lstBEAMS[i]->T);
            //lstBEAMS[i]->imprimeRepresentacaoGraficaArvore((i+1)*2);
            printf("\n---------\n");
            printf("\nImpressão em ordem: ---------\n");
            lstBEAMS[i]->imprimeEmOrdem(lstBEAMS[i]->T);
            printf("\n---------\n");
        }
    }
    
}

void realizaExclusoesAleatorias(AVL *lstBEAMS[QTD_BEAMS], No_AVL *listaPessoas[QTD_PESSOAS]){

    /* inicializa a semente (seed): */

    srand (time(NULL));
    for (int i=0; i< QTD_EXCLUSOES; i++){        
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
            lstBEAMS[idBEAM]->buscaRegistrosPeloNome(nome, lstBEAMS[idBEAM]->T);  //Deve retornar chaveEncontrada==true (se encontrar a chave, claro)

            //Demonstração do método de exclusão de elemento da árvore AVL       

            if( lstBEAMS[idBEAM]->chaveEncontrada!=NULL){
                if(idBEAM >=0 && idBEAM < QTD_BEAMS){   
                    printf ("\nSelecionado para exclusão: %s,%d, do compartimento %d, letra %c:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);                               
                    lstBEAMS[idBEAM]->exclui(&lstBEAMS[idBEAM]->T, telefone);            
                    //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
                    printf("Excluído com sucesso o nome %s, telefone %d, do compartimento %d, letra %c. Veja, abaixo, a AVL do compartimento, atualizada após a exclusão:\n", strTrim(nome).c_str(), telefone, idBEAM, letra);
                    lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);   
                    //lstBEAMS[idBEAM]->imprimeRepresentacaoGraficaArvore(i);                            
                } 
            } else {
                printf("Chave (telefone %d) não encontrada(o), nome: %s!\n", telefone, nome.c_str());
            }           
        }
    }
}

int main()
{    
    No_AVL *listaPessoas[QTD_PESSOAS];
    AVL *lstBEAMS[QTD_BEAMS];

    //Leitura do arquivo de entrada:  
    leituraArquivoEntrada(lstBEAMS, listaPessoas);
 
    realizaInsersoes(lstBEAMS, listaPessoas);
    realizaExclusoes(lstBEAMS, listaPessoas);
    //imprimeFloresta(lstBEAMS);
    //realizaExclusoesAleatorias(lstBEAMS, listaPessoas);
      
    /* Libera memória ocupada pelas AVLs contidas na listaPessoas (ou seja: de todos os elementos da lista) */
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        delete (listaPessoas[i]);
    }
    return 0;
}