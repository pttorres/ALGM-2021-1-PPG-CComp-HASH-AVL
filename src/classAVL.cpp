#include<iostream>
#include<cstring>
#include<string>

using namespace std;   // declaração global do espaço de nomes

struct No_AVL {
    int numTelefone;
    string nome;
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
	
    void insere(No_AVL** T, No_AVL* pNo){
        if (*T==NULL){
            (*T)=pNo;
            (**T).fb=0;
            (**T).esq=NULL;
            (**T).dir=NULL;
            this->N++;
            strcpy(hAum,"V\0");
         //   printf("T: %p, *T:%p, **T: %p, pNo:%p, *pNo: %p, T->chave %d\n", T, (*T), (**T), pNo, *pNo, (**T).chave);
    		
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
        
    void imprime(No_AVL* T){
        
        if(T != NULL){
            printf(" %d ", T->chave);
            imprime(T->esq);
            imprime(T->dir);
        }
    };
	
	void exclui(No_AVL** T, int chave){ 
	   
		No_AVL* qTemp;
        printf("T: %p, *T:%p, **T: %p, ", T, (*T), (**T));
        printf("pNo->chave:%d, ", chave);
        printf("T->chave %d \n\n", (**T).chave);
        
		if((*T) == NULL){
			cout << "Elemento inexistente";
			abort();
		}
		if(chave < (**T).chave) {
			exclui(&(**T).esq, chave);
			if (hAum[0]=='V')
				atualizaBalanceamentoNoEsq(T,hAum);
		} else if(chave > (**T).chave){
			exclui(&(**T).dir, chave);
			if (hAum[0]=='V')
				atualizaBalanceamentoNoDir(T,hAum);
		} else{
		    
			qTemp = *T;
			
			if((*qTemp).dir == NULL){ 
    	        printf("else: T: %p, *T:%p, **T: %p, ", T, (*T), (**T));
                printf("pNo->chave:%d, ", chave);
                printf("T->chave:%d, ", (**T).chave);
                printf("Q->chave:%p \n\n", qTemp->esq);
				
				*T = qTemp->esq;
				strcpy(hAum,"V\0");
			} else if(qTemp->esq == NULL){
				*T = (*qTemp).dir;
				strcpy(hAum,"V\0");
			} else{
				removeMinimo(&qTemp, qTemp->dir);
				if(hAum[0]=='V')
					atualizaBalanceamentoNoDir(T,hAum);
			}
			delete qTemp;
			this->N--;
		}
	};
	
	void removeMinimo(No_AVL** q, No_AVL* r){
		if((*r).esq != NULL){ 
			removeMinimo(q, r->esq);
			if (hAum[0]=='V')
				atualizaBalanceamentoNoEsq(&r,hAum);
		} else { 
			(**q).chave = r->chave;
			**q = (*r);
			r = r->dir;
			strcpy(hAum,"V\0");
		}
	};
	
};

AVL::AVL(){              //O(1)
    this->T = NULL;          //O(1)
    this->N = 0;          //O(1)
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