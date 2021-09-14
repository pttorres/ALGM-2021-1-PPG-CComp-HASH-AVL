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

#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <math.h>  //abs por exemplo
#include <fstream> //Para escrita do arquivo de saída contendo a representação da árvore
#include "ExecCommand.hpp"

using namespace std; // declaração global do espaço de nomes

/** Funções genéricas (não especificamente da classe AVL): ************** */

/* retorna um vetor formado pelas palavras separadas pelo caracter delimitador encontrado na string de entrada
   Exemplo: splitStr("abc;def", ';') retorna o vetor ["abc","def"]
 */
std::vector<std::string> splitStr(string strEntrada, char delimitador) //Complexidade desta função: podemos considerá-la como O(1),
{                                                                      //uma vez que somente trabalhará com entradas bem pequenas 
    int contaPalavras = 1;                                             // (algo em torno de 15 a 20 caracteres em média)     
    char letra;
    string palavra = "";
    int tamStrEntrada = strEntrada.length();
    for (int i = 0; i < tamStrEntrada; i++)
    {
        letra = strEntrada[i];
        if (letra == delimitador)
        {
            contaPalavras++;
        }
    }
    std::vector<std::string> retorno(contaPalavras);
    int j = 0;
    for (int i = 0; i < tamStrEntrada; i++)
    {
        letra = strEntrada[i];
        if (letra != delimitador && i + 1 < tamStrEntrada)
        {
            palavra += letra;
        }
        else
        {
            retorno[j] = palavra;
            palavra = "";
            j++;
        }
    }
    return retorno;
}
/* Padroniza um tamanho fixo para todos os nomes - acrescenta espaços ao final, para igualar tamanhos */
string fixSize(string nome, int tam)  //Podemos considerar que esta função tem complexidade O(1), pois trabalha com entradas pequenas (em geral
{                                     //  em torno de 15 a 20 caracteres).
    string strRetorno = nome;
    if (strRetorno.length() < tam)
    {
        while (strRetorno.length() < tam)
        {
            strRetorno += ' ';
        }
    }
    else
    {
        strRetorno = "";
        int i = 0;
        while (strRetorno.length() < tam)
        {
            strRetorno += nome[i];
            i++;
        }
    }
    return strRetorno;
}

/* Apaga espaços no final da string */
string strTrim(string nome)        //Podemos considerar que esta função tem complexidade O(1), pois trabalha com entradas pequenas (em geral
                                     //  em torno de 15 a 20 caracteres).
{
    string strRetorno = "";
    int tam = nome.length();
    for (int i = tam; i > 0; i--)
    {
        if (nome[i - 1] != ' ')
        {
            tam = i;
            break;
        }
    }
    for (int i = 0; i < tam; i++)
    {
        strRetorno += nome[i];
    }
    return strRetorno;
}
/*******************************************************************/
// Aqui começam as rotinas, funções e definições da classe AVL
/******************************************************************/

struct No_AVL
{
    int numTelefone;
    string nome;
    int fb;      // Fator de Balanço
    No_AVL *esq; // aponta subárvore esquerda
    No_AVL *dir; // aponta subárvore direita
};
///////////////////////  Implementação da classe AVL ////////////////////////////
class AVL
{

public:
    int N;
    No_AVL *T;
    //char hAum[2];
    bool hAum; //Houve aumento da altura da subárvore?
    AVL();
    No_AVL *chaveEncontrada;
    int contaNosNulos = 0;

    /*Visita em pré-ordem os nós da árvore para obtenção da representação gráfica (Graphviz)*/
    void visitaEmPreOrdemParaArquivoGraphviz(No_AVL *noAtual, ofstream *arq)
    {                           // Complexidade desta função: O(1)
        if (noAtual == NULL)
            return;
        int fb = noAtual->fb; //Fator de balanceamento
        string strNoPai = "No_" + std::to_string(noAtual->numTelefone);

        /* Para fazer com que as informações pudessem usar múltiplas linhas, foi utilizado o recurso de labels
        Exemplo de aplicação : 
        Este exemplo pode ser visto online em:
        https://dreampuf.github.io/GraphvizOnline/

        Basta copiar e colar este código abaixo e inserir no campo de entrada do portal acima referido.
        
        digraph BST {
            No_992883178 -> No_960933788;
            No_992883178 [label="Alexandre Barbosa\n992883178\nfb=-1"];
            No_960933788 [label="Alberto Campos\n960933788\nfb=0"];
            No_960933788 -> NUL_0;
            No_960933788 [label="Alberto Campos\n960933788\nfb=0"];
            No_960933788 -> NUL_1;
            No_960933788 [label="Alberto Campos\n960933788\nfb=0"];
            No_992883178 -> NUL_2;
            No_992883178 [label="Alexandre Barbosa\n992883178\nfb=-1"];
            NUL_0,NUL_1,NUL_2 [label=null,shape=square]
        }
              
        */
        if (noAtual->esq != 0)  //Escreve as ligações entre pai e filho esquerdo
        {
            int fbEsq = noAtual->esq->fb; //Fator de balanceamento do filho à esquerda
            string strNoEsq = "No_" + std::to_string(noAtual->esq->numTelefone);
            *arq << "\t" << strNoPai << " -> " << strNoEsq << ";\n";
            *arq << "\t" << strNoPai << " [label=\"" << noAtual->nome << "\\n"
                 << noAtual->numTelefone << "\\nfb=" << fb << "\"];\n";
            *arq << "\t" << strNoEsq << " [label=\"" << noAtual->esq->nome << "\\n"
                 << noAtual->esq->numTelefone << "\\nfb=" << fbEsq << "\"];\n";
            //*arq <<"\t\""<< noAtual->nome << ","<< noAtual->numTelefone << " [fb:"<< fb<<"]\" -> \""<< noAtual->esq->nome << ","<< noAtual->esq->numTelefone<<" [fb:"<< fbEsq<<"]\";\n";
            visitaEmPreOrdemParaArquivoGraphviz(noAtual->esq, arq);
        }
        else
        {    //Escreve as ligações entre pai e filho esquerdo nulo
            string strNulo = "NUL_" + std::to_string(this->contaNosNulos);
            *arq << "\t" << strNoPai << " -> " << strNulo << ";\n";
            *arq << "\t" << strNoPai << " [label=\"" << noAtual->nome << "\\n"
                 << noAtual->numTelefone << "\\nfb=" << fb << "\"];\n";
            this->contaNosNulos++;
        }
        if (noAtual->dir != 0)
        {  //Escreve as ligações entre pai e filho direito não nulo
            int fbDir = noAtual->dir->fb; //Fator de balanceamento do filho à direita
            string strNoDir = "No_" + std::to_string(noAtual->dir->numTelefone);
            *arq << "\t" << strNoPai << " -> " << strNoDir << ";\n";
            *arq << "\t" << strNoPai << " [label=\"" << noAtual->nome << "\\n"
                 << noAtual->numTelefone << "\\nfb=" << fb << "\"];\n";
            *arq << "\t" << strNoDir << " [label=\"" << noAtual->dir->nome << "\\n"
                 << noAtual->dir->numTelefone << "\\nfb=" << fbDir << "\"];\n";
            visitaEmPreOrdemParaArquivoGraphviz(noAtual->dir, arq);
        }
        else
        {   //Escreve as ligações entre pai e filho direito nulo
            string strNulo = "NUL_" + std::to_string(this->contaNosNulos);
            *arq << "\t" << strNoPai << " -> " << strNulo << ";\n";
            *arq << "\t" << strNoPai << " [label=\"" << noAtual->nome << "\\n"
                 << noAtual->numTelefone << "\\nfb=" << fb << "\"];\n";
            this->contaNosNulos++;
        }
    }
    /* Imprime a representação da árvore, para ser exportada em formato Graphviz
    O texto gerado pode ser levado a um editor on-line como, por exemplo, este abaixo referido:
    https://dreampuf.github.io/GraphvizOnline/
    As imagens das árvores geradas são apresentadas no relatório deste trabalho.
     */
    void imprimeRepresentacaoGraficaArvore(int id)   //Complexidade: O(1)
    {
        if (this->T == NULL || this->N==0)
        {
            printf("\nErro! Não foi possível imprimir a árvore de id=%d", id);
            return;
        }
        ofstream arq;
        char c = (this->T->nome[0]);
        string nomeArq = "arvoreParaGraphviz_";
        nomeArq.push_back(c);
        string strId = std::to_string(id);                
        nomeArq+=strId+"_N"+to_string(this->N);
        nomeArq += ".gdot";
        arq.open(nomeArq);
        arq << "digraph BST {\n";
        ofstream *pArq = &arq;
        this->contaNosNulos = 0;
        visitaEmPreOrdemParaArquivoGraphviz(this->T, pArq);
        string strNosNulos = "";
        //Referências aos ponteiros nulos (esquerdo e direito)
        for (int i = 0; i < this->contaNosNulos; i++)   //Podemos considerar este loop como O(1) pois trabalhará com valores pequenos de N
        {                                               //que são insignificantes sob o ponto de vista do crescimento assintótico
            strNosNulos += "NUL_" + std::to_string(i);
            if (i < this->contaNosNulos - 1)
                strNosNulos.push_back(',');
        }
        arq << "\t" + strNosNulos + " [label=null,shape=square]\n";
        arq << "}\n";
        arq.close();
        nomeArq=fixSize(nomeArq, nomeArq.length()-5);
        string comandoGDot = "dot -Tsvg " + nomeArq + ".gdot > " + nomeArq + ".svg";
        //printf("\nDebug: %s\n", comandoGDot.c_str());
        exec(comandoGDot.c_str());
    }

    /* Realiza a impressão da árvore AVL em ordem */ 
    void imprimeEmOrdem(No_AVL *T)  //Complexidade: O(N)
    {
        if (T != NULL)
        {
            imprimeEmOrdem(T->esq);
            if (T->numTelefone != (int)0)
            {
                printf("| %s, %d |", T->nome.c_str(), T->numTelefone);
            }
            imprimeEmOrdem(T->dir);
        }
    };

    /* Realiza a impressão da árvore AVL em níveis */
    void imprimeVisitaEmNiveis(No_AVL *noAtual)   // Complexidade: O(N)
    {
        if (noAtual == NULL)
        {
            printf("\nErro!: Impossível a impressão, pois a árvore AVL está vazia.");
            return;
        }
        queue<No_AVL *> fila;
        fila.push(noAtual);
        No_AVL *frente;
        int i = 0;
        int nivelAtual = 1;
        int nivelAnterior = 1;
        printf("Compartimento %d, letra %c, quantidade de registros %d :", this->T->nome[0] - 65, this->T->nome[0], this->N);
        while (!fila.empty())    //Complexidade: O(N)
        {
            frente = fila.front();
            if (frente->esq != 0)
            {
                fila.push(frente->esq);
            }
            if (frente->dir != 0)
            {
                fila.push(frente->dir);
            }
            fila.pop();
            noAtual = frente;
            nivelAnterior = (int)log2(i);
            i++;
            nivelAtual = (int)log2(i);
            if (nivelAtual != nivelAnterior)
            {
                printf("\nNível:%d ", nivelAtual + 1);
            }
            printf("| %s, %d |", strTrim(noAtual->nome).c_str(), noAtual->numTelefone);
        }
    }

    /* Obtém a altura da árvore ou subárvore que tem como raiz determinado nó 
       Esta função retorna a altura do nó até a folha
    */
    int getAlturaDaArvore(No_AVL* noRaiz){     //Complexidade: O(log(N))
        if (noRaiz==NULL){
            return 0;
        }
        int alturaNoDir=getAlturaDaArvore(noRaiz->dir)+1;  //Complexidade: O(log(N))
        int alturaNoEsq=getAlturaDaArvore(noRaiz->esq)+1;  //Complexidade: O(log(N))
        if (alturaNoDir>alturaNoEsq){
            return alturaNoDir;
        } else {
            return alturaNoEsq;
        }
    }

    /* computa o fator de balanceamento de um dado nó */
    int getFatorBalanceamento(No_AVL* noAtual){                 //Complexidade: O(log(N))
        int tamSubArvoreDir=getAlturaDaArvore(noAtual->dir);    //Complexidade: O(log(N))
        int tamSubArvoreEsq=getAlturaDaArvore(noAtual->esq);    //Complexidade: O(log(N)) 
        return (tamSubArvoreDir-tamSubArvoreEsq);
    }

    /* Após a exclusão de um item, percorre desde a raiz até o nó onde houve a substituição e atualiza os fatores de balanceamento
    */
    void atualizaFbAposDelecao(No_AVL **noAtual)      //Complexidade: O(N)
    {
        if ((*noAtual) != NULL)
        {
            (*noAtual)->fb=getFatorBalanceamento((*noAtual));   //Complexidade: O(log(N))
            atualizaFbAposDelecao( &(*noAtual)->esq);           //Complexidade: O(N)
            atualizaFbAposDelecao( &(*noAtual)->dir);           //Complexidade: O(N)
        }
    }


    /* Informa o(s) telefone(s) associado(s) a um determinado nome 
    No retorno, a variável this->chaveEncontrada contém o registro encontrado
    */
    void buscaRegistrosPeloNome(string nome, No_AVL *noAtual)   //Complexidade: O(N)
    {
        if (this->chaveEncontrada != NULL)
            return;
        if (noAtual != NULL)
        {
            if (noAtual->nome == nome)
            {
                printf("\nEncontrado o telefone %d vinculado ao nome %s.\n", noAtual->numTelefone, strTrim(nome).c_str());
                this->chaveEncontrada = noAtual;
            }
            buscaRegistrosPeloNome(nome, noAtual->esq);     //Complexidade: O(N)
            buscaRegistrosPeloNome(nome, noAtual->dir);     //Complexidade: O(N)
        }
    }

    /* retorna o registro associado a um determinado telefone
    No retorno, a variável this->chaveEncontrada contém o registro encontrado   
    */
    void buscaRegistrosPeloTelefone(int telefone, No_AVL *noAtual)   //Complexidade: O(N)
    {
        if (this->chaveEncontrada != NULL)
            return;
        if (noAtual != NULL)
        {
            if (noAtual->numTelefone == telefone)
            {
                string nome=noAtual->nome;
                printf("\nEncontrado o nome %s vinculado ao telefone %d.\n", strTrim(nome).c_str(), telefone);
                this->chaveEncontrada = noAtual;
            } else if (noAtual->numTelefone > telefone){
                buscaRegistrosPeloTelefone(telefone, noAtual->esq);    //Complexidade: O(N)
            } else {
                buscaRegistrosPeloTelefone(telefone, noAtual->dir);    //Complexidade: O(N)
            }
        }
    }

    /* Atualiza o balanceamento no nó esquerdo */
    void atualizaBalanceamentoNoEsq(No_AVL **T)     //Complexidade: O(1)
    {
        if ((*T)->fb == 1)
        {
            (*T)->fb = 0;
            this->hAum = false;
        }
        else if ((*T)->fb == 0)
        {
            (*T)->fb = -1;
        }
        else
        {
            rotacaoDireita(T);                       //Complexidade: O(1)
            atualizafbT(T);                          //Complexidade: O(1)
            (*T)->fb = 0;
            this->hAum = false;
        }
    };

    /* Atualiza o fator de balanceamento do filho à direita */
    void atualizaBalanceamentoNoDir(No_AVL **T)                    //Complexidade: O(1)
    {
        if ((*T)->fb == -1)
        {
            (*T)->fb = 0;
            this->hAum = false;
        }
        else if ((*T)->fb == 0)
        {
            (*T)->fb = 1;
        }
        else
        {
            rotacaoEsquerda(T);                    //Complexidade: O(1)
            atualizafbT(T);                         //Complexidade: O(1)
            (*T)->fb = 0;
            this->hAum = false;
        }
    };

    /*Realiza rotação dos nós à direita */
    void rotacaoDireita(No_AVL **T)                                //Complexidade: O(1)
    {
        if ((*T)== NULL) return;
        No_AVL *noTemp;
        if ((*T)->esq != NULL)
        {
            noTemp = ((*T)->esq);
            if (noTemp->fb == -1)
            {
                rotacaoDireitaSimples(T);                         //Complexidade: O(1)
            }
            else
            {
                rotacaoEsquerdaSimples(&(*T)->esq);            //Complexidade: O(1)
                rotacaoDireitaSimples(T);                       //Complexidade: O(1)
            }
        }
    };

    /* Realiza um rotação à esquerda */
    void rotacaoEsquerda(No_AVL **T)                        //Complexidade: O(1)
    {
        if ((*T)== NULL) return;
        No_AVL *noTemp;
        if ((*T)->dir != NULL)
        {
            noTemp = ((*T)->dir);
            if (noTemp->fb == 1)
            {
                rotacaoEsquerdaSimples(T);                  //Complexidade: O(1)
            }
            else
            {
                rotacaoDireitaSimples(&(*T)->dir);        //Complexidade: O(1)
                rotacaoEsquerdaSimples(T);                 //Complexidade: O(1)
            }
        }
    };

    /* Realiza um rotação simples à direita */
    void rotacaoDireitaSimples(No_AVL **T)              //Complexidade: O(1)
    {
        No_AVL *noTemp;
        if ((*T)->esq != NULL)
        {
            noTemp = (*T)->esq;
            (*T)->esq = noTemp->dir;
            noTemp->dir = *T;
            atualizafbT(T);            //Complexidade: O(1)
            *T = noTemp;
            atualizafbT(T);             //Complexidade: O(1)
        }
    };

    /* Realiza um rotação simples à esquerda */
    void rotacaoEsquerdaSimples(No_AVL **T)         //Complexidade: O(1)
    {
        No_AVL *noTemp;
        if ((*T)->dir != NULL)
        {
            noTemp = (*T)->dir;
            (*T)->dir = noTemp->esq;
            noTemp->esq = *T;
            atualizafbT(T);                      //Complexidade: O(1)
            *T = noTemp;
            atualizafbT(T);                     //Complexidade: O(1)
        }
    };

    /* Atualiza o fator de balanceamento de um dado nó */
    void atualizafbT(No_AVL **T)                                    //Complexidade: O(1)
    {
        if ((*T)->dir == NULL && (*T)->esq == NULL)
        {
            (*T)->fb = 0;
        }
        else
        {
            if ((*T)->dir != NULL && (*T)->esq != NULL)
            {
                if (((*T)->dir)->fb == 0 && ((*T)->esq)->fb == 0)
                {
                    (*T)->fb = 0;
                }
                else
                {
                    if (((*T)->dir)->fb != 0 && ((*T)->esq)->fb == 0)
                    {
                        (*T)->fb = 1;
                    }
                    else
                    {
                        if (((*T)->dir)->fb == 0 && ((*T)->esq)->fb != 0)
                            (*T)->fb = -1;
                        else
                            (*T)->fb = 0;
                    }
                }
            }
            else
            {
                if (((*T)->dir != NULL && (*T)->esq == NULL))
                    (*T)->fb = 1;
                else
                    (*T)->fb = -1;
            }
        }
    };

    /* Realiza a inserção de cada um dos nós */
    void insere(No_AVL **T, No_AVL *pNo)                       //Complexidade: O(1)
    {
        if (*T == NULL)
        {
            (*T) = pNo;
            (*T)->fb = 0;
            (*T)->esq = NULL;
            (*T)->dir = NULL;
            this->N++;
            this->hAum = true;
        }
        else if ((*T)->numTelefone > pNo->numTelefone)
        {
            insere(&(*T)->esq, pNo);                          //Complexidade: O(1)
            if (this->hAum == true)
            {
                atualizaBalanceamentoNoEsq(T);                //Complexidade: O(1)
            }
        }
        else if ((*T)->numTelefone < pNo->numTelefone)
        {
            insere(&(*T)->dir, pNo);                           //Complexidade: O(1)
            if (this->hAum == true)
            {
                atualizaBalanceamentoNoDir(T);                  //Complexidade: O(1)
            }
        }
        else
        {
            printf("Número de telefone já existe: %d \n", (*T)->numTelefone);
            //  perror("Chave ja existente ");
        }
    };

    /* Realiza a remoção de cada um dos nós */
    void exclui(No_AVL **T, int telefone)              //Complexidade O(N)  (observação: pode ser otimizada)
    {
        No_AVL *novoEsq;
        No_AVL *novoDir;
        No_AVL **novoSubRaiz;


        //if (telefone==975960513){  //Apenas para DEBUG:
         //   int pararEmDebug=0;
        //}

        if ((*T) != NULL)
        {
            if ((*T)->numTelefone == telefone)
            { // Chave encontrada

                this->N--;

                int telefoneSubstituto; // Número do telefone do nó que ficará na posição do nó deletado

                if ((*T)->esq == (*T)->dir)
                { // é uma folha

                    //printf("\nDebug: r = %d\n",chave);                    
                    if ((*T)->fb == 0)   //Nó a ser deletado está balanceado
                    {
                        this->hAum = true;
                    }
                    (*T) = NULL;
                } 
                else
                { // Nó tem filhos

                    if ((*T)->fb == -1)   //Nó a ser deletado está desbalanceado à esquerda
                    {
                        novoSubRaiz = maiorEsquerdo(*T);
                        (*T)->nome = (*novoSubRaiz)->nome;
                        (*T)->numTelefone = (*novoSubRaiz)->numTelefone;
                        telefoneSubstituto=(*novoSubRaiz)->numTelefone;
                        (*T)->fb = 0;                        
                        if ((*novoSubRaiz)->esq !=NULL){
                            (*T)->esq=(*novoSubRaiz)->esq;
                        } else {
                            *novoSubRaiz = NULL;
                        } 
                        rotacaoDireita(&((*T)->esq));                  //Complexidade: O(1)                                                    
                        this->hAum = false;
                    }
                    else if ((*T)->fb == 1)  //Nó a ser deletado está desbalanceado à direita
                    {
                        novoSubRaiz = menorDireito(*T);
                        (*T)->nome = (*novoSubRaiz)->nome;
                        (*T)->numTelefone = (*novoSubRaiz)->numTelefone;
                        telefoneSubstituto=(*novoSubRaiz)->numTelefone;
                        (*T)->fb = 0;                 
                        if ((*novoSubRaiz)->dir !=NULL){
                            (*T)->dir=(*novoSubRaiz)->dir;
                        } else {
                            *novoSubRaiz = NULL;
                        }
                        rotacaoEsquerda(&((*T)->dir));                        //Complexidade: O(1)                         
                        this->hAum = false;
                    }
                    else //Nó a ser deletado está balanceado
                    { //fb==0
                        novoSubRaiz = menorDireito(*T);
                        (*T)->nome = (*novoSubRaiz)->nome;
                        (*T)->numTelefone = (*novoSubRaiz)->numTelefone;   
                        telefoneSubstituto=(*novoSubRaiz)->numTelefone;                  
                        //(*T)->fb = 1;
                        if ((*novoSubRaiz)->dir !=NULL){
                            (*T)->dir=(*novoSubRaiz)->dir;
                            rotacaoEsquerda(&(*T)->dir);                     //Complexidade: O(1)
                        } else {
                            *novoSubRaiz = NULL;
                        }                        
                        this->hAum = false;
                    }
                }
            }
            else if ((*T)->numTelefone < telefone)
            { // Desce em busca da chave pelo ramo à direita.

                exclui(&((*T)->dir), telefone);                          //Complexidade: O(log(N))

                if (this->hAum == true)
                {
                    atualizaBalanceamentoNoEsq(T);                       //Complexidade: O(1)
                }
            }
            else if ((*T)->numTelefone > telefone)
            {   // Desce em busca da chave pelo ramo à esquerda.

                exclui(&((*T)->esq), telefone);

                if (this->hAum == true)
                {
                    atualizaBalanceamentoNoDir(T);                      //Complexidade: O(1)
                }
            }
            this->atualizaFbAposDelecao(&this->T);                      //Complexidade O(N)  (observação: inserimos essa linha para tentar
        }                                                               // sanar bugs nesta função de exclusão - ou seja, esta função pode     
        else                                                            // ser otimizada)             
        { // Chave não encontrada
            perror("Erro! Chave (número de telefone) não encontrada.\n");
            this->chaveEncontrada = NULL; //Esta variável é checada após a execução desta função.
        }
    };

    /* Encontra o antecessor do nó a ser deletado, em ordem */ 
    No_AVL **menorDireito(No_AVL *T)                                //Complexidade: O(log(N))
    {
        No_AVL **aux = &T;
        if ((*aux)->dir != NULL)
        {
            aux = &(*aux)->dir;
            while ((*aux)->esq != NULL)         //Complexidade: O(log(N))
            {
                aux = &(*aux)->esq;
            }
        }
        return aux;
    };

    /* Encontra o sucessor do nó a ser deletado, em ordem */ 
    No_AVL **maiorEsquerdo(No_AVL *T)                           //Complexidade: O(log(N))
    {
        No_AVL **aux = &T;
        if ((*aux)->esq != NULL)
        {
            aux = &(*aux)->esq;
            while ((*aux)->dir != NULL)             //Complexidade: O(log(N))
            {
                aux = &(*aux)->dir;
            }
        }
        return aux;
    };
};

AVL::AVL()
{                   //O(1)
    this->T = NULL; //O(1)
    this->N = 0;    //O(1)
    this->chaveEncontrada = NULL;
};
