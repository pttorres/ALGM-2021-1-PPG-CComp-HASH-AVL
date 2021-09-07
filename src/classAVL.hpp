#include<iostream>
#include<cstring>
#include<string>
#include<queue>

using namespace std;   // declaração global do espaço de nomes

/* Apaga espaços no final da string */
string strTrim(string nome){
    string strRetorno="";
    int tam=nome.length();
    for (int i=tam; i>0; i--){
        if (nome[i-1]!=' '){
            tam=i;
            break;
        }
    }
    for (int i=0; i<tam; i++){
        strRetorno+=nome[i];
    }
    return strRetorno;
}

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
    bool chaveNaoEncontrada=false;

    /*
    /* Libera a memória ocupada pela árvore AVL (dispensável, pois os objetos AVL estão sendo deletados junto com os elementos de listaPessoas)
    void libera (No_AVL* raiz) {                                       
        if (raiz->dir == NULL && raiz->esq == NULL){                                                  
            delete(raiz);                                                                   
        }else {
            if (raiz->dir != NULL){
                libera(raiz->dir);
            }
            if (raiz->esq != NULL){
                libera(raiz->esq);
            }
        }        
    } 
    */ 
    /* Informa o(s) telefone(s) associado(s) a um determinado nome */
    void buscaRegistrosPeloNome(string nome, No_AVL* noAtual){
        if (noAtual->nome==nome) {
            printf("Encontrado o telefone %d vinculado ao nome %s.\n", noAtual->numTelefone, strTrim(nome).c_str());
        } 
        if (noAtual->esq!=NULL){
            buscaRegistrosPeloNome(nome, noAtual->esq);
        }
        if (noAtual->dir!=NULL){
            buscaRegistrosPeloNome(nome, noAtual->dir);
        }
    }
    
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
            //printf("T: %p, *T:%p, **T: %p, pNo:%p, *pNo: %p, T->chave %d\n", T, (*T), (**T), pNo, *pNo, (**T).chave);
    		
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
            printf("Chave já existe: %d \n", (*T)->chave);
          //  perror("Chave ja existente ");
        }
    };
        
    void imprime(No_AVL* T){
        if(T != NULL){
            if (T->chave != (int) 0){
                printf("%d, ", T->chave);
            }
            imprime(T->esq);
            imprime(T->dir);
        }        
    };

    void imprimeVisitaEmNiveis(No_AVL* noAtual){
        queue<No_AVL*> fila;
        fila.push(noAtual);
        No_AVL* frente;
        int i=0;
        int nivelAtual=1;
        int nivelAnterior=1;
        while (!fila.empty()){
            frente=fila.front();
            if (frente->esq!=0){
               fila.push(frente->esq);
            }
            if (frente->dir!=0){
               fila.push(frente->dir);
            }
            fila.pop();
            noAtual=frente;                      
            nivelAnterior=(int) log2(i);
            i++;                       
            nivelAtual=(int) log2(i); 
            if(nivelAtual!=nivelAnterior){
                printf("\nNível:%d ", nivelAtual+1);
            }           
            printf("| %s, %d |", strTrim(noAtual->nome).c_str(), noAtual->numTelefone );  
        }
    }
	
   void exclui(No_AVL **T, int chave){
      No_AVL** aux1, *aux2;
      No_AVL*  aux = *T; 
      
      if(*T != NULL) {
      
        if((*T)->chave == chave){  // Chave encontrada
        
          if((*T)->esq == (*T)->dir){ // é uma folha
         
            //printf("\nDebug: r = %d\n",chave);
            free(*T); 
            *T = NULL; 
            strcpy(hAum,"V\0");
    
          } else { // Nó tem filhos 
    
            if((*T)->esq != NULL){
              aux1 = maiorEsquerdo(*T); 
              aux2 = *aux1;
              (*aux1) = (*aux1)->esq;
            } else {
              aux1 = menorDireito(*T);
              aux2 = *aux1; 
              (*aux1) = (*aux1)->dir;
            }
            (*T)->chave = aux2->chave; 
            free(aux2); 
            aux2 = NULL; 
            strcpy(hAum,"V\0");
    
          }
        } else if((*T)->chave < chave){ // Desce até encontrar ou não encontrar a chave.
    
            exclui(&(*T)->dir,chave);
            if (hAum[0]=='V'){
                atualizaBalanceamentoNoEsq(T, hAum);
            }
    
        } else if((*T)->chave > chave) {
    
           exclui(&(*T)->esq,chave);
           
            if (hAum[0]=='V'){
                atualizaBalanceamentoNoDir(T, hAum);
            }
    
        }         
      } else {// Chave não encontrada
    	perror("chave não encontrada");
        this->chaveNaoEncontrada=true;  //Esta variável é checada após a execução desta função.
      }
    
    };
    
    No_AVL** menorDireito(No_AVL *T)
    { 
      No_AVL** aux = &T;  
      if((*aux)->dir != NULL) 
      {
        aux = &(*aux)->dir;  
        while((*aux)->esq != NULL) 
        {
          aux = &(*aux)->esq; 
        }
      }
      return aux;
    };
    
    No_AVL** maiorEsquerdo(No_AVL* T)
    {
      No_AVL** aux = &T; 
      if((*aux)->esq != NULL)
      {
        aux = &(*aux)->esq;
        while((*aux)->dir != NULL) 
        {
          aux = &(*aux)->dir;
        }
      }
      return aux;
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