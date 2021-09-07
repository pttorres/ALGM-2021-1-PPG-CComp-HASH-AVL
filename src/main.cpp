#include <iostream>   // biblioteca necessária para a função std::cout
#include <stdio.h>
#include <math.h> //abs por exemplo
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "classAVL.hpp"

#define QTD_PESSOAS 200  // número de pessoas na lista de nomes fictícios
#define QTD_BEAMS 26  //número de compartimentos = número de letras do alfabeto
#define QTD_EXCLUSOES 30

using namespace std;   // declaração global do espaço de nomes
std::vector<std::string> splitStr(string strEntrada, char delimitador){
    int contaPalavras=1;
    char letra;
    string palavra="";
    int tamStrEntrada=strEntrada.length();
    for (int i=0; i<tamStrEntrada; i++){
        letra=strEntrada[i];
        if (letra==delimitador){
            contaPalavras++;
        }
    }
    std::vector<std::string> retorno(contaPalavras);
    int j=0;
    for (int i=0; i<tamStrEntrada; i++){
        letra=strEntrada[i];
        if (letra!=delimitador && i+1<tamStrEntrada){
            palavra+=letra;
        } else {
            retorno[j]=palavra;
            palavra="";
            j++;
        }
    }
    return retorno;
}
string fixSize(string nome, int tam){
    string strRetorno=nome;
    while (strRetorno.length()< tam){
        strRetorno+=' ';
    }
    return strRetorno;
}

int main()
{
    fstream arqEntrada;
    No_AVL* listaPessoas[QTD_PESSOAS];
    for (int i=0; i< QTD_PESSOAS; i++){
        listaPessoas[i]=new No_AVL();
    }
    AVL* lstBEAMS[QTD_BEAMS];
    No_AVL* lstBEAMS_TreeRoot[QTD_BEAMS];
    for (int i=0; i < QTD_BEAMS; i++){
        lstBEAMS[i]=new AVL();
        lstBEAMS_TreeRoot[i] = NULL;
    }
    //Leitura do arquivo de entrada:
    arqEntrada.open("listaEntrada.csv",ios::in); //Abre o arquivo com a lista de nomes e telefones
    if (arqEntrada.is_open()){ //O arquivo foi aberto corretamente?
        string linha;
        string nome;
        int telefone;
        int i=0;
        // Apenas carrega a lista de nomes e de telefones para a memória:
        printf("Lista de nomes e de telefones fictícios que serão incluídos na hashTable / AVLs:\n---------------------\n");
        while(getline(arqEntrada, linha)){ //leitura das linhas do arquivo de entrada    
            std::vector<std::string> nomeEtelefone=splitStr(linha,';');        
            if (i>0 && i<200){
                nome=nomeEtelefone[0];
                listaPessoas[i-1]->nome=fixSize(nome,20);
                telefone=std::stoi(nomeEtelefone[1]);
                listaPessoas[i-1]->numTelefone=telefone; 
                listaPessoas[i-1]->chave = telefone + rand() % 100; // chave de telefone
                printf(listaPessoas[i-1]->nome.c_str());
                printf("\t");
                printf("%d",listaPessoas[i-1]->numTelefone);
                printf("\n");
            }
            i++;
        }
        printf("\n\n---------------------------\n");
        
        arqEntrada.close(); //fecha o arquivo de entrada.
        
    } else {
        printf("Erro: Problema na leitura do arquivo da lista de nomes! Terminando a execução...\n");
        return 0;
    }

    // A partir daqui, começa o preenchimento dos compartimentos (BEAMS e respectivas AVLs) da hashtable

    // Insere os nomes de acordo com a primeira letra (O 'A' de ANDREA, por exemplo, é inserido na posição 0 o B na posição 1 e assim por diante)
    for (int i=0; i< QTD_PESSOAS-1; i++){
        int idBEAM=listaPessoas[i]->nome[0]-65;
        printf ("Debug: número do idBEAM: %d\n, chave: ", idBEAM, listaPessoas[i]->chave);
        //A implementar (método insere da classe AVL):
        lstBEAMS[idBEAM]->insere(&lstBEAMS_TreeRoot[idBEAM], listaPessoas[i]);
        if (lstBEAMS[idBEAM]->N % 10==0){  //Quando chegar a 10, 20, 30... elementos na AVL, ela é apresentada:
            lstBEAMS[idBEAM]->imprime(lstBEAMS[idBEAM]->T);
        }
    }
    //Impressão da Floresta
    for (int i=0; i< 27-1; i++){
        printf("\n\n---------\n");
        lstBEAMS[i]->imprime(lstBEAMS_TreeRoot[i]);
        printf("\n\n---------\n");
    }
    /* inicializa a semente (seed): */
    srand (time(NULL));
    for (int i=0; i< QTD_EXCLUSOES; i++){
        
        int idExclusao;

        /* gera numero entre 0 e 199: */
        idExclusao = rand() % 200;
        printf ("Debug: número aleatório gerado para o id a ser excluído: %d\n", idExclusao); 

        //A implementar (método exclui da classe AVL)        
        string nome=listaPessoas[idExclusao]->nome;
        int telefone=listaPessoas[idExclusao]->numTelefone;
        int idBEAM=nome[0]-65;   
        printf ("Debug: número do idBEAM: %d\n", idBEAM);
        //lstBEAMS[idBEAM]->exclui(nome, telefone);
        //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
        lstBEAMS[idBEAM]->imprime(lstBEAMS[idBEAM]->T);
    }       
    return 0;
}