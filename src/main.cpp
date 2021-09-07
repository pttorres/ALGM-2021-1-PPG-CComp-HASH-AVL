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
#define QTD_EXCLUSOES 10

using namespace std;   // declaração global do espaço de nomes
/* retorna um vetor formado pelas palavras separadas pelo caracter delimitador encontrado na string de entrada
   Exemplo: splitStr("abc;def", ';') retorna o vetor ["abc","def"]
 */
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
/* Padroniza um tamanho fixo para todos os nomes - acrescenta espaços ao final, para igualar tamanhos */
string fixSize(string nome, int tam){
    string strRetorno=nome;
    if(strRetorno.length()< tam){
        while (strRetorno.length()< tam){
            strRetorno+=' ';
        }
    } else {
        strRetorno="";
        int i=0;
        while (strRetorno.length()< tam){
            strRetorno+=nome[i];
            i++;
        }
    }
    return strRetorno;
}

int main()
{
    fstream arqEntrada;
    No_AVL* listaPessoas[QTD_PESSOAS];
    for (int i=0; i< QTD_PESSOAS; i++){
        if (listaPessoas[i] != NULL) listaPessoas[i]=NULL;
        listaPessoas[i]=new No_AVL();
    }
    AVL* lstBEAMS[QTD_BEAMS];
    //No_AVL* lstBEAMS_TreeRoot[QTD_BEAMS];
    for (int i=0; i < QTD_BEAMS; i++){
        lstBEAMS[i]=new AVL();
        //lstBEAMS_TreeRoot[i] = NULL;
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
            if (i>0 && i<201){
                nome=nomeEtelefone[0];
                listaPessoas[i-1]->nome=strTrim(nome);
                telefone=std::stoi(nomeEtelefone[1]);
                listaPessoas[i-1]->numTelefone=telefone; 
                //listaPessoas[i-1]->chave = telefone + rand() % 100; // chave de telefone
                listaPessoas[i-1]->chave = telefone ; // chave de telefone
                printf(fixSize(listaPessoas[i-1]->nome, 18).c_str());
                printf("\t");
                printf("%d",listaPessoas[i-1]->numTelefone);
                printf("\n");
            }
            i++;
        }
        
        arqEntrada.close(); //fecha o arquivo de entrada.
        
    } else {
        printf("Erro: Problema na leitura do arquivo da lista de nomes! Terminando a execução...\n");
        return 0;
    }

    // A partir daqui, começa o preenchimento dos compartimentos (BEAMS e respectivas AVLs) da hashtable
    printf("\n==================\nInício das inserções\n==================\n");
       printf("\nObservação: para a saída não ficar muito grande, só serão apresentadas as árvores de inserção com o valor de N entre 5 e 10.\n");

    // Insere os nomes de acordo com a primeira letra (O 'A' de ANDREA, por exemplo, é inserido na posição 0 o B na posição 1 e assim por diante)
    for (int i=0; i< QTD_PESSOAS; i++){  
        int idBEAM=listaPessoas[i]->nome[0]-65;        
         // Método insere da classe AVL:
        //lstBEAMS[idBEAM]->insere(&lstBEAMS_TreeRoot[idBEAM], listaPessoas[i]);
        lstBEAMS[idBEAM]->insere(&lstBEAMS[idBEAM]->T, listaPessoas[i]);
        if (lstBEAMS[idBEAM]->N > 5 && lstBEAMS[idBEAM]->N < 10){
            printf ("\n\nInserção #%d: %s, %d no compartimento número %d, letra %c. Árvore AVL do compartimento atualizada :", i+1, listaPessoas[i]->nome.c_str(),  listaPessoas[i]->numTelefone, idBEAM, idBEAM+65);
            lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);
            printf("\n");
        } else {
            printf ("\nInserção #%d: %s, %d no compartimento número %d, letra %c. Omitida a árvore atualizada após essa inserção, para economizar espaço.", i+1, listaPessoas[i]->nome.c_str(),  listaPessoas[i]->numTelefone, idBEAM, idBEAM+65);
        }
    }
    printf("\n==================\nFim das inserções\n==================\n"); 
    printf("\n==================\nInício de testes de buscas e de exclusões\n==================\n"); 

    /* inicializa a semente (seed): */
    srand (time(NULL));
    for (int i=0; i< QTD_EXCLUSOES; i++){        
        int idAleatorio;

        /* gera numero entre 0 e 199: */
        idAleatorio = rand() % 200;
        printf ("\n\nNúmero aleatório, entre 0 e 199, gerado para o id a ser excluído: %d\n", idAleatorio); 

        string nome=strTrim(listaPessoas[idAleatorio]->nome);
        int telefone=listaPessoas[idAleatorio]->numTelefone;
        int idBEAM=nome[0]-65; 

        printf ("Buscaremos, na lista, o(s) telefone(s) vinculados ao nome de %s:\n", nome.c_str());

        lstBEAMS[idBEAM]->buscaRegistrosPeloNome(nome,  lstBEAMS[idBEAM]->T);

        //Demonstração do método de exclusão de elemento da árvore AVL
                  
        printf ("\nSelecionado para exclusão: %s,%d, do compartimento %d, letra %c:\n", strTrim(nome).c_str(), telefone, idBEAM, nome[0]);
        if(idBEAM >=0 && idBEAM < QTD_BEAMS){
            lstBEAMS[idBEAM]->chaveNaoEncontrada=false;
            lstBEAMS[idBEAM]->exclui(&lstBEAMS[idBEAM]->T, telefone);
            //Apresenta a AVL após a exclusão (método imprimeAVL da classe AVL)
            if (lstBEAMS[idBEAM]->chaveNaoEncontrada==false){
                printf("Excluído com sucesso o nome %s, telefone %d, do compartimento %d, letra %c. Veja, abaixo, a AVL do compartimento, atualizada após a exclusão:\n", strTrim(nome).c_str(), telefone, idBEAM, idBEAM+65);
                lstBEAMS[idBEAM]->imprimeVisitaEmNiveis(lstBEAMS[idBEAM]->T);
            } else {
                printf("Chave (telefone %d) não encontrada(o)!\n", telefone);
            }                  
        }
    }

    //Impressão da Floresta (lista telefônica completa), após inserções e exclusões
    //lstBEAMS[11]->exclui(&lstBEAMS[11]->T, 971943105);
    printf("\n\n\n---------\nImpressão da Floresta (ou seja, do catálogo inteiro), após inserções e exclusões\n---------\n");    
    for (int i=0; i< QTD_BEAMS; i++){
        if (lstBEAMS[i]->T != NULL){
            printf("\n---------\n");
            printf("Compartimento %d, letra %c:", i, i+65);
            lstBEAMS[i]->imprimeVisitaEmNiveis(lstBEAMS[i]->T);
            printf("\n---------\n");
        }
    }
    
    /* Libera memória ocupada pelas AVLs contidas na listaPessoas (ou seja: de todos os elementos da lista) */       
    for (int i=0; i< QTD_PESSOAS; i++){
        delete(listaPessoas[i]);
    }
    return 0;
}