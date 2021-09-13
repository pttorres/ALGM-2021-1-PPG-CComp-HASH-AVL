#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <math.h>  //abs por exemplo
#include <fstream> //Para escrita do arquivo de saída contendo a representação da árvore
#include "ExecCommand.hpp"

using namespace std; // declaração global do espaço de nomes

/* Apaga espaços no final da string */
string strTrim(string nome)
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
    {
        if (noAtual == NULL)
            return;
        int fb = noAtual->fb; //Fator de balanceamento
        string strNoPai = "No_" + std::to_string(noAtual->numTelefone);
        if (noAtual->esq != 0)
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
        {
            string strNulo = "NUL_" + std::to_string(this->contaNosNulos);
            *arq << "\t" << strNoPai << " -> " << strNulo << ";\n";
            *arq << "\t" << strNoPai << " [label=\"" << noAtual->nome << "\\n"
                 << noAtual->numTelefone << "\\nfb=" << fb << "\"];\n";
            this->contaNosNulos++;
        }
        if (noAtual->dir != 0)
        {
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
        {
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
    A imagem da árvore gerada segue em anexo a este trabalho.
     */
    void imprimeRepresentacaoGraficaArvore(int id)
    {
        if (this->T == NULL)
        {
            printf("Erro! Não foi possível imprimir a árvore de id=%d", id);
            return;
        }
        ofstream arq;
        char c = (this->T->nome[0]);
        string nomeArq = "arvoreParaGraphviz_";
        nomeArq.push_back(c);
        string strId = std::to_string(id);
        int tamStrId = strId.length();
        for (int i = 0; i < tamStrId; i++)
        {
            nomeArq.push_back(strId[i]);
        }
        nomeArq += ".gdot";
        arq.open(nomeArq);
        arq << "digraph BST {\n";
        ofstream *pArq = &arq;
        this->contaNosNulos = 0;
        visitaEmPreOrdemParaArquivoGraphviz(this->T, pArq);
        string strNosNulos = "";
        //Referências aos ponteiros nulos (esquerdo e direito)
        for (int i = 0; i < this->contaNosNulos; i++)
        {
            strNosNulos += "NUL_" + std::to_string(i);
            if (i < this->contaNosNulos - 1)
                strNosNulos.push_back(',');
        }
        arq << "\t" + strNosNulos + " [label=null,shape=square]\n";
        arq << "}\n";
        arq.close();
        string comandoGDot = "dot -Tsvg " + nomeArq + " > " + nomeArq + ".svg";
        exec(comandoGDot.c_str());
    }

    void imprimeEmOrdem(No_AVL *T)
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

    void imprimeVisitaEmNiveis(No_AVL *noAtual)
    {
        if (noAtual == NULL)
        {
            printf("Erro!: Impossível a impressão, pois a árvore AVL está vazia.");
            return;
        }
        queue<No_AVL *> fila;
        fila.push(noAtual);
        No_AVL *frente;
        int i = 0;
        int nivelAtual = 1;
        int nivelAnterior = 1;
        printf("Compartimento %d, letra %c, quantidade de registros %d :", this->T->nome[0] - 65, this->T->nome[0], this->N);
        while (!fila.empty())
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
    /* Informa o(s) telefone(s) associado(s) a um determinado nome 
    No retorno, a variável this->chaveEncontrada contém o registro encontrado
    */
    void buscaRegistrosPeloNome(string nome, No_AVL *noAtual)
    {
        if (this->chaveEncontrada != NULL)
            return;
        if (noAtual != NULL)
        {
            if (noAtual->nome == nome)
            {
                printf("Encontrado o telefone %d vinculado ao nome %s.\n", noAtual->numTelefone, strTrim(nome).c_str());
                this->chaveEncontrada = noAtual;
            }
            buscaRegistrosPeloNome(nome, noAtual->esq);
            buscaRegistrosPeloNome(nome, noAtual->dir);
        }
    }

    void atualizaBalanceamentoNoEsq(No_AVL **T)
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
            rotacaoDireita(T);
            (*T)->fb = 0;
            this->hAum = false;
        }
    };

    void atualizaBalanceamentoNoDir(No_AVL **T)
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
            rotacaoEsquerda(T);
            (*T)->fb = 0;
            this->hAum = false;
        }
    };

    void rotacaoDireita(No_AVL **T)
    {
        No_AVL *noTemp;
        if ((*T)->esq != NULL)
        {
            noTemp = ((*T)->esq);
            if (noTemp->fb == -1)
            {
                rotacaoDireitaSimples(T);
            }
            else
            {
                rotacaoEsquerdaSimples(&(*T)->esq);
                rotacaoDireitaSimples(T);
            }
        }
    };

    void rotacaoEsquerda(No_AVL **T)
    {
        No_AVL *noTemp;
        if ((*T)->dir != NULL)
        {
            noTemp = ((*T)->dir);
            if (noTemp->fb == 1)
            {
                rotacaoEsquerdaSimples(T);
            }
            else
            {
                rotacaoDireitaSimples(&(*T)->dir);
                rotacaoEsquerdaSimples(T);
            }
        }
    };

    void rotacaoDireitaSimples(No_AVL **T)
    {
        No_AVL *noTemp;
        if ((*T)->esq != NULL)
        {
            noTemp = (*T)->esq;
            (*T)->esq = noTemp->dir;
            noTemp->dir = *T;
            atualizafbT(T);
            *T = noTemp;
        }
    };

    void rotacaoEsquerdaSimples(No_AVL **T)
    {
        No_AVL *noTemp;
        if ((*T)->dir != NULL)
        {
            noTemp = (*T)->dir;
            (*T)->dir = noTemp->esq;
            noTemp->esq = *T;
            atualizafbT(T);
            *T = noTemp;
        }
    };

    void atualizafbT(No_AVL **T)
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

    void insere(No_AVL **T, No_AVL *pNo)
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
            insere(&(*T)->esq, pNo);
            if (this->hAum == true)
            {
                atualizaBalanceamentoNoEsq(T);
            }
        }
        else if ((*T)->numTelefone < pNo->numTelefone)
        {
            insere(&(*T)->dir, pNo);
            if (this->hAum == true)
            {
                atualizaBalanceamentoNoDir(T);
            }
        }
        else
        {
            printf("Número de telefone já existe: %d \n", (*T)->numTelefone);
            //  perror("Chave ja existente ");
        }
    };

    void exclui(No_AVL **T, int telefone)
    {
        No_AVL *novoEsq;
        No_AVL *novoDir;
        No_AVL **novoSubRaiz;

        if ((*T) != NULL)
        {

            if ((*T)->numTelefone == telefone)
            { // Chave encontrada

                this->N--;

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
                        (*T)->fb = 0;
                        *novoSubRaiz = NULL;
                        rotacaoDireita(&(*T)->esq);
                        this->hAum = false;
                    }
                    else if ((*T)->fb == 1)  //Nó a ser deletado está desbalanceado à direita
                    {
                        novoSubRaiz = menorDireito(*T);
                        (*T)->nome = (*novoSubRaiz)->nome;
                        (*T)->numTelefone = (*novoSubRaiz)->numTelefone;
                        (*T)->fb = 0;
                        *novoSubRaiz = NULL;
                        rotacaoEsquerda(&(*T)->dir);
                        this->hAum = false;
                    }
                    else //Nó a ser deletado está balanceado
                    { //fb==0
                        novoSubRaiz = maiorEsquerdo(*T);
                        (*T)->nome = (*novoSubRaiz)->nome;
                        (*T)->numTelefone = (*novoSubRaiz)->numTelefone;
                        //(*T)->fb = 1;
                        *novoSubRaiz = NULL;
                        //rotacaoDireita(&(*T)->esq);
                        this->hAum = false;
                    }
                }
            }
            else if ((*T)->numTelefone < telefone)
            { // Desce em busca da chave pelo ramo à direita.

                exclui(&(*T)->dir, telefone);

                if (this->hAum == true)
                {
                    atualizaBalanceamentoNoDir(T);
                }
            }
            else if ((*T)->numTelefone > telefone)
            {   // Desce em busca da chave pelo ramo à esquerda.

                exclui(&(*T)->esq, telefone);

                if (this->hAum == true)
                {
                    atualizaBalanceamentoNoEsq(T);
                }
            }
        }
        else
        { // Chave não encontrada
            perror("Erro! Chave (número de telefone) não encontrada.\n");
            this->chaveEncontrada = NULL; //Esta variável é checada após a execução desta função.
        }
    };

    No_AVL **menorDireito(No_AVL *T)
    {
        No_AVL **aux = &T;
        if ((*aux)->dir != NULL)
        {
            aux = &(*aux)->dir;
            while ((*aux)->esq != NULL)
            {
                aux = &(*aux)->esq;
            }
        }
        return aux;
    };

    No_AVL **maiorEsquerdo(No_AVL *T)
    {
        No_AVL **aux = &T;
        if ((*aux)->esq != NULL)
        {
            aux = &(*aux)->esq;
            while ((*aux)->dir != NULL)
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

/*
int main(){
    
    AVL* avl_tree[1];
    avl_tree[0] = new AVL();
    No_AVL* X;
    
    No_AVL* T[1];
    T[0] = NULL;
    
    for(int i; i< 15; i++){
        X = new No_AVL();
        X->nome = "Piettro";
        X->numTelefone = 95682;
        X->chave = 95682 +  (rand() % 100);
        X->fb = 0;
        printf("\nnumero: %d", X->chave);
        avl_tree[0]->insere(&T[0], X);
    }
    
    //No_AVL* x1 = new No_AVL();
   // x1->chave = 10;
  //  No_AVL* x2 = new No_AVL();
//    No_AVL* x3 = new No_AVL();
 //   x1->chave = 17;
 //   x2->chave = 3;
 //   x3->chave = 15;
    
  //  avl_tree->remocao(&T, x2);
   // avl_tree->remocao(&T, x3);
    
    printf("passou aqui %d\n\n", avl_tree[0]->N);
    
    avl_tree[0]->imprime(T[0]);
    printf("\n\n\n -----------------Remocao ------------------- \n\n");
    int xteste = 10;
   // avl_tree->remocao(&T, 10);
    
}*/