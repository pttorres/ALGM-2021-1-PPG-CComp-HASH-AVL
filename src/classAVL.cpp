#include<iostream>
#include<cstring>
#include<string>

using namespace std;   // declaração global do espaço de nomes

struct No_AVL {

    int chave;
    int fb; // Fator de Balanço
    No_AVL* esq;  // aponta subárvore esquerda
    No_AVL* dir;  // aponta subárvore direita
};
///////////////////////  Implementação da classe AVL ////////////////////////////
class AVL{

public:
    int N;
    No_AVL* T; 
    char hAum[2];
    AVL();
    
   
    void insere(No_AVL** T, No_AVL* pNo){
        if (*T==NULL){
            (*T)=pNo;
            (**T).fb=0;
            (**T).esq=NULL;
            (**T).dir=NULL;
            this->N++;
            strcpy(hAum,"V\0");
           // printf("T: %p, *T:%p, **T: %p, pNo:%p, *pNo: %p, T->chave %d\n", T, (*T), (**T), pNo, *pNo, (**T).chave);
    		
        } else if ((**T).chave>(*pNo).chave){
            insere(&(**T).esq, pNo);
            if (hAum[0]=='V'){
                atualizaBalanceamentoNoEsq(T, hAum);
            }
    		
        } else if ((**T).chave<(*pNo).chave){
            insere(&(**T).dir, pNo);
            if (hAum[0]=='V'){
                atualizaBalanceamentoNoDir(T, hAum);
            }
        } else {
            perror("Chave ja existente!");
        }
    };
    
    void atualizaBalanceamentoNoEsq(No_AVL** T, char * hAum){
        if ((**T).fb==1){
            (**T).fb=0;
            strcpy(hAum,"F\0");
        } else if ((**T).fb==0){
            (**T).fb=-1;
    		
        } else {
            rotacaoDireita(T);
            (**T).fb=0;
            strcpy(hAum,"F\0");
        }
    };
    
    void atualizaBalanceamentoNoDir(No_AVL** T, char * hAum){
        if ((**T).fb==-1){
            (**T).fb=0;
            strcpy(hAum,"F\0");
        } else if ((**T).fb==0){
            (**T).fb=1;
        }
        else {
            rotacaoEsquerda(T);
            (**T).fb=0;
            strcpy(hAum,"F\0");
        }
    };
    
    void rotacaoDireita(No_AVL** T){
        No_AVL* noTemp;
        noTemp=((No_AVL*)(**T).esq);
        if ((char) noTemp->fb==-1){
            rotacaoDireitaSimples(T);
        } else {
    		rotacaoEsquerdaSimples(&(*T)->esq);
    		rotacaoDireitaSimples(T);
        }
    };
    
    
    void rotacaoEsquerda(No_AVL** T){
        No_AVL* noTemp;
        noTemp=((No_AVL*)(**T).dir);
    	if ((char) noTemp->fb==1){
    		   rotacaoEsquerdaSimples(T);
    	} else{
    		rotacaoDireitaSimples(&(*T)->dir);
    		rotacaoEsquerdaSimples(T);
    	}
    };
    
    void rotacaoDireitaSimples(No_AVL** T){
        No_AVL* noTemp = (No_AVL*)(*T)->esq;
        (*T)->esq=noTemp->dir;
        noTemp->dir=*T;
        atualizafbT(*T);
        *T=noTemp;
    };
    
    void rotacaoEsquerdaSimples(No_AVL** T){
        No_AVL* noTemp = (*T)->dir;
        (*T)->dir = noTemp->esq;
        noTemp->esq=*T;
        atualizafbT(*T);
        *T=noTemp;
    };
        
    void atualizafbT(No_AVL* T){
     if(T->dir==NULL && T->esq==NULL){
            T->fb=0;
    	}else {
            if (T->dir!=NULL && T->esq!=NULL){
                if ((T->dir)->fb==0 && (T->esq)->fb==0){
                    T->fb=0;
    			} else {
                    if ((T->dir)->fb!=0 && (T->esq)->fb==0) {
                        T->fb=1;
                    } else {
                        if ((T->dir)->fb==0 && (T->esq)->fb!=0) T->fb=-1;
                        else T->fb=0;
                    }
                }
            } else {
                if ((T->dir!=NULL && T->esq==NULL)) T->fb=1;
                else T->fb=-1;
            }
        }
    };
    
        
    //Exclui o elemento com o nome e número determinados
    No_AVL* exclui(string nome, int telefone){          //O(1)
    
    };
    //Busca o elemento x na pilha
    No_AVL* getElemento(No_AVL* elemento){          //O(1)
    
    };     
    //imprime todos os elementos da pilha (recebe o nó raiz ou aproveita o atributo raiz da própria classe)
    void imprime(No_AVL* T, int h){
        
        if(T != NULL){
            imprime(T->esq, h+1);
            printf(" %d ", T->chave);
            imprime(T->dir, h+1);
        }
    }
};
AVL::AVL(){              //O(1)
    this->T = NULL;          //O(1)
    this->N = 0;          //O(1)
};

int main(){
    
    AVL* avl_tree = new AVL();
    No_AVL* X;
    
    No_AVL*T = NULL;
    
    for(int i; i< 7; i++){
        X = new No_AVL();
        X->chave = rand() % 20;
        X->fb = 0;
        avl_tree->insere(&T, X);
    }
    
    avl_tree->imprime(T, 1);
    
    
}