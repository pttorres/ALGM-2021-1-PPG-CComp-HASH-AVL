/*
Implementação da classe AVL
(aproveitei estas linhas de código do exercício de pilhas. Então, claro, elas precisam ser alteradas para o código de árvore AVL)
Deixei apenas para o caso de servirem para alguma lembrança.
*/

#include<iostream>
#include<cstring>
#include<string>

using namespace std;   // declaração global do espaço de nomes

struct No_AVL {
    int numTelefone;
    string nome;
    int fb; // Fator de Balanço
    No_AVL* esq;  // aponta subárvore esquerda
    No_AVL* dir;  // aponta subárvore direita
};
///////////////////////  Implementação da classe AVL ////////////////////////////
class AVL
{
public:
    int N; // num. de elementos na AVL          //O(1)
    No_AVL* noRaiz;          //O(1)
    AVL();          //O(1)
    void libera() { //Libera a memória ocupada pela AVL          //O(n)
        // for (int i=0; i<this->N; i++){          //O(n)
        //     No_AVL* proxNode=this->nodeTopo->prox;              //O(1)        
        //     delete(this->nodeTopo);          //O(1)
        //     this->nodeTopo=proxNode;          //O(1)
        // }        
    } 
    //Retorna o elemento situado na raiz da AVL
    No_AVL* getRaiz(){          //O(1)
        // if (this->noRaiz>0){          //O(1)
        //     return this->noRaiz;          //O(1)
        // } else {
        //     return 0;          //O(1)
        // }
        return 0;
    };
    //Insere o elemento x na AVL
    void insere(No_AVL* x){          //O(1)
        // No_AVL* novoNodeTopo=x;          //O(1)
        // No_AVL* nodeTopoAntigo;          //O(1)
        // if(this->N>0){          //O(1)
        //     nodeTopoAntigo=this->nodeTopo;          //O(1)
        //     novoNodeTopo->prox=nodeTopoAntigo;          //O(1)
        //     this->nodeTopo=novoNodeTopo;          //O(1)
        // } else{
        //     this->nodeTopo=x;          //O(1)
        // }        
        // this->N++;                           //O(1)
    };
    //Exclui o elemento com o nome e número determinados
    No_AVL* exclui(string nome, int telefone){          //O(1)
        // No_AVL* retorno=this->topo();          //O(1)
        // No_AVL* novoNodeTopo;          //O(1)
        // if(this->nodeTopo->prox>0){          //O(1)
        //     novoNodeTopo=this->nodeTopo->prox;               //O(1)            
        //     this->nodeTopo=novoNodeTopo;                       //O(1)
        // } else {
        //     this->nodeTopo=0;          //O(1)
        // }  
        // this->N--;             //O(1)
        // return retorno;          //O(1)
    };
    //Busca o elemento x na pilha
    No_AVL* getElemento(No_AVL* elemento){          //O(1)
        // No_AVL* retorno=this->topo();          //O(1)
        // No_AVL* novoNodeTopo;          //O(1)
        // if(this->nodeTopo->prox>0){          //O(1)
        //     novoNodeTopo=this->nodeTopo->prox;               //O(1)            
        //     this->nodeTopo=novoNodeTopo;                       //O(1)
        // } else {
        //     this->nodeTopo=0;          //O(1)
        // }  
        // this->N--;             //O(1)
        // return retorno;          //O(1)
    };     
    //imprime todos os elementos da pilha (recebe o nó raiz ou aproveita o atributo raiz da própria classe)
    void imprimeAVL(){          //O(n)
        // No_AVL* proxNode=this->nodeTopo;            //O(1)
        // for (int i=0; i<this->N; i++){                 //O(n)
        //     if(proxNode->chrc>0)               //O(1)
        //         printf("%c\t",proxNode->chrc);          //O(1)
        //     proxNode=proxNode->prox;                                  //O(1)
        // }
    }
};
AVL::AVL(){              //O(1)
    this->noRaiz=new No_AVL;          //O(1)
    this->N=0;          //O(1)
};

template<typename Agregado, typename Tipo, typename Tipo1, typename Tipo2>
concept bool
AVL_TAD = requires(Agregado a, Tipo t, Tipo1 t1, Tipo2 t2 )
{
// libera a memória ocupada pelos nós da AVL
{ a.libera() };
// obtem o elemento situado na raiz da AVL
{ a.getRaiz() };
// insere o elemento x
{ a.insere(t) };
// deleta o elemento x
{ a.exclui(t1, t2) };
// busca o elemento x
{ a.getElemento(t) };
// imprime os elementos da árvore (recebe o nó raiz ou aproveita o atributo raiz da própria classe)
{ a.imprimeAVL() };
};

// verifica se os agregados satisfazem o conceito
static_assert(AVL_TAD<AVL, No_AVL*, string, int>);
