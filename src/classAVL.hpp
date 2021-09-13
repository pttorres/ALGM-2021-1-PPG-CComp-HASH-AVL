#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <math.h> //abs por exemplo
#include <fstream> //Para escrita do arquivo de saída contendo a representação da árvore
#include "ExecCommand.hpp"

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
    int fb; // Fator de Balanço
    No_AVL* esq;  // aponta subárvore esquerda
    No_AVL* dir;  // aponta subárvore direita
};
///////////////////////  Implementação da classe AVL ////////////////////////////
class AVL{

public:
    int N;
    No_AVL* T; 
    //char hAum[2];
    bool hAum;  //Houve aumento da altura da subárvore?
    AVL();
    bool chaveEncontrada=false;
    int contaNosNulos=0;

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
    /*Visita em pré-ordem os nós da árvore para obtenção da representação gráfica (Graphviz)*/
    void visitaEmPreOrdemParaArquivoGraphviz(No_AVL* noAtual, ofstream* arq){
        if (noAtual==NULL ) return;                
        int fb=noAtual->fb;  //Fator de balanceamento
        if (noAtual->esq!=0){            
            int fbEsq=noAtual->esq->fb;  //Fator de balanceamento do filho à esquerda
            *arq <<"\t\""<< noAtual->nome << ","<< noAtual->numTelefone << " [fb:"<< fb<<"]\" -> \""<< noAtual->esq->nome << ","<< noAtual->esq->numTelefone<<" [fb:"<< fbEsq<<"]\";\n";
            visitaEmPreOrdemParaArquivoGraphviz(noAtual->esq, arq);      
        } else {
            string strNulo="N"+std::to_string(this->contaNosNulos);
            *arq <<"\t\""<< noAtual->nome << ","<< noAtual->numTelefone << " [fb:"<< fb<<"]\" -> \""<< strNulo << "\"; \n";            
            this->contaNosNulos++;
        }
        if (noAtual->dir!=0){
            int fbDir=noAtual->dir->fb;  //Fator de balanceamento do filho à direita
            *arq <<"\t\""<< noAtual->nome << ","<< noAtual->numTelefone <<" [fb:"<< fb<<"]\" -> \""<< noAtual->dir->nome << ","<< noAtual->dir->numTelefone<<" [fb:"<< fbDir<<"]\";\n";
            visitaEmPreOrdemParaArquivoGraphviz(noAtual->dir, arq);       
        } else {
            string strNulo="N"+std::to_string(this->contaNosNulos);
            *arq <<"\t\""<< noAtual->nome << ","<< noAtual->numTelefone << " [fb:"<< fb<<"]\" -> \""<< strNulo << "\"; \n";            
            this->contaNosNulos++;
        }
    }
    /* Imprime a representação da árvore, para ser exportada em formato Graphviz
    O texto gerado pode ser levado a um editor on-line como, por exemplo, este abaixo referido:
    https://dreampuf.github.io/GraphvizOnline/
    A imagem da árvore gerada segue em anexo a este trabalho.
     */
    void imprimeRepresentacaoGraficaArvore(int id){
        ofstream arq;
        char c=(this->T->nome[0]);                
        string nomeArq="arvoreParaGraphviz_";
        nomeArq.push_back(c);
        string strId=std::to_string(id);
        int tamStrId=strId.length();
        for (int i=0; i< tamStrId; i++){
            nomeArq.push_back(strId[i]);
        }
        nomeArq+=".gdot";
        arq.open (nomeArq);
        arq << "digraph BST {\n";
        ofstream* pArq=&arq;
        this->contaNosNulos=0;
        visitaEmPreOrdemParaArquivoGraphviz(this->T, pArq); 
        string strNosNulos="";
        //Referências aos ponteiros nulos (esquerdo e direito)
        for (int i=0; i<this->contaNosNulos; i++){
            strNosNulos.push_back('N');
            strNosNulos=strNosNulos+std::to_string(i);
            if (i<this->contaNosNulos-1) strNosNulos.push_back(',');
        }
        arq << "\t"+strNosNulos+" [label=null,shape=circle]\n";
        arq << "}\n";       
        arq.close();
        string comandoGDot="dot -Tsvg "+nomeArq+" > "+nomeArq+".svg";
        exec(comandoGDot.c_str());
    }
    /* Informa o(s) telefone(s) associado(s) a um determinado nome */
    void buscaRegistrosPeloNome(string nome, No_AVL* noAtual){
        if (this->chaveEncontrada==true) return;
        if (noAtual->nome==nome) {
            printf("Encontrado o telefone %d vinculado ao nome %s.\n", noAtual->numTelefone, strTrim(nome).c_str());
            this->chaveEncontrada=true;
        } 
        if (noAtual->esq!=NULL){
            buscaRegistrosPeloNome(nome, noAtual->esq);
        }
        if (noAtual->dir!=NULL){
            buscaRegistrosPeloNome(nome, noAtual->dir);
        }
    }
    
    //void atualizaBalanceamentoNoEsq(No_AVL** T, char * hAum){
    void atualizaBalanceamentoNoEsq(No_AVL** T){
        if ((*T)->fb==1){
            (*T)->fb=0;
            this->hAum=false;
        } else if ((*T)->fb==0){
            (*T)->fb=-1;    		
        } else {
            rotacaoDireita(T);
            (*T)->fb=0;            
            this->hAum=false;
        }
    };
    
    void atualizaBalanceamentoNoDir(No_AVL** T){
        if ((*T)->fb==-1){
            (*T)->fb=0;
            this->hAum=false;
        } else if ((*T)->fb==0){
            (*T)->fb=1;
        }
        else {
            rotacaoEsquerda(T);
            (*T)->fb=0;
            this->hAum=false;
        }
    };
    
    void rotacaoDireita(No_AVL** T){
        No_AVL* noTemp;
        if ((*T)->esq!=NULL){
            noTemp=((*T)->esq);
            if (noTemp->fb==-1){
                rotacaoDireitaSimples(T);
            } else {
                rotacaoEsquerdaSimples(&(*T)->esq);
                rotacaoDireitaSimples(T);
            }
        }
    };
        
    void rotacaoEsquerda(No_AVL** T){
        No_AVL* noTemp;
        if ((*T)->dir !=NULL){
            noTemp=((*T)->dir);
            if (noTemp->fb==1){
                rotacaoEsquerdaSimples(T);
            } else{
                rotacaoDireitaSimples(&(*T)->dir);
                rotacaoEsquerdaSimples(T);
            }
        }
    };
    
    void rotacaoDireitaSimples(No_AVL** T){
        No_AVL* noTemp;
        if ((*T)->esq!= NULL){
            noTemp = (*T)->esq;
            (*T)->esq=noTemp->dir;        
            noTemp->dir=*T;
            atualizafbT(T);
            *T=noTemp;
        }
    };
    
    void rotacaoEsquerdaSimples(No_AVL** T){
        No_AVL* noTemp;
        if ((*T)->dir!= NULL){
            noTemp = (*T)->dir;
            (*T)->dir = noTemp->esq;        
            noTemp->esq=*T;
            atualizafbT(T);
            *T=noTemp;
        }
    };
        
    /*void atualizafbT_OLD(No_AVL* T){
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
    }; */
    /* Obtém a altura da árvore ou subárvore que tem como 
    raiz determinado nó */
    int getAlturaDaArvore(No_AVL* noRaiz){
        if (noRaiz==0){
            return 0;
        }
        int alturaNoDir=getAlturaDaArvore(noRaiz->dir)+1;
        int alturaNoEsq=getAlturaDaArvore(noRaiz->esq)+1;
        if (alturaNoDir>alturaNoEsq){
            return alturaNoDir;
        } else {
            return alturaNoEsq;
        }
    }

    /*Computa o fator de balanceamento de um dado nó */
    int getFatorBalanceamento(No_AVL* noAtual){
        if (noAtual!=NULL){
            int tamSubArvoreDir=getAlturaDaArvore(noAtual->dir);
            int tamSubArvoreEsq=getAlturaDaArvore(noAtual->esq);
            return (tamSubArvoreDir-tamSubArvoreEsq);
        } else {
            return 0;
        }
    }

    /* Atualiza o fator de balanceamento
    retorna T->fb==0 se a árvore estiver balanceada
    retorna T->fb==número int. negativo se estiver desbalanceada à esquerda
    retorna T->fb==número inteiro positivo se a direita estiver maior  */
    void atualizafbT(No_AVL** T){
        (*T)->fb=getFatorBalanceamento(*T);
    };
	
    void insere(No_AVL** T, No_AVL* pNo){
        if (*T==NULL){
            (*T)=pNo;
            (*T)->fb=0;
            (*T)->esq=NULL;
            (*T)->dir=NULL;
            //(*T)->excluido=false;
            this->N++;
            this->hAum=true;
    		
        } else if ((*T)->numTelefone> pNo->numTelefone){
            insere(&(*T)->esq, pNo);
            if (this->hAum==true){
                atualizaBalanceamentoNoEsq(T);
            }    		
        } else if ((*T)->numTelefone < pNo->numTelefone){
            insere(&(*T)->dir, pNo);
            if (this->hAum==true){
                atualizaBalanceamentoNoDir(T);
            }
        } else {
            printf("Número de telefone já existe: %d \n", (*T)->numTelefone);
          //  perror("Chave ja existente ");
        }
    };
        
    void imprimeEmOrdem(No_AVL* T){
        if(T != NULL){            
            imprimeEmOrdem(T->esq);
            if (T->numTelefone != (int) 0){   
                printf("| %s, %d |", T->nome.c_str(), T->numTelefone);                
            }
            imprimeEmOrdem(T->dir);
        }        
    };

    void imprimeVisitaEmNiveis(No_AVL* noAtual){
        if (noAtual==NULL){
            printf("Erro!: Impossível a impressão, pois a árvore AVL está vazia.");
            return;
        } 
        queue<No_AVL*> fila;
        fila.push(noAtual);
        No_AVL* frente;
        int i=0;
        int nivelAtual=1;
        int nivelAnterior=1;
        printf("Compartimento %d, letra %c, quantidade de registros %d :", this->T->nome[0]-65, this->T->nome[0], this->N);
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

    /* Given a non-empty binary search tree,
    return the noRaiz with minimum key value
    found in that tree. Note that the entire
    tree does not need to be searched. */
    No_AVL * getNoComMenorValor(No_AVL* noRaiz)
    {
        No_AVL* noAtual = noRaiz;
    
        /* loop down to find the leftmost leaf */
        while (noAtual->esq != NULL){
            noAtual = noAtual->esq;
        }
        return noAtual;
    }

    No_AVL * getNoComMaiorValor(No_AVL* noRaiz)
    {
        No_AVL* noAtual = noRaiz;
    
        /* desce até a folha direita com maior valor */
        while (noAtual->dir != NULL){
            noAtual = noAtual->dir;
        }
        return noAtual;
    }

    // Recursive function to delete a noRaiz
    // with given telefone from subtree with
    // given noRaiz. It returns noRaiz of the
    // modified subtree.
    void excluiOLD(No_AVL** noRaiz, int telefone)
    {        
        // STEP 1: PERFORM STANDARD BST DELETE
        if ((*noRaiz) != NULL) {
            // If the telefone to be deleted is smaller
            // than the noRaiz's telefone, then it lies
            // in left subtree
            if (telefone < (*noRaiz)->numTelefone ){
                exclui(&(*noRaiz)->esq, telefone);
            }
        
            // If the telefone to be deleted is greater
            // than the noRaiz's telefone, then it lies
            // in right subtree
            else if( telefone > (*noRaiz)->numTelefone ){
                exclui(&(*noRaiz)->dir, telefone);
            }
            // if telefone is same as noRaiz's telefone, then
            // This is the noRaiz to be deleted
            else
            {
                // noRaiz with only one child or no child
                if( ((*noRaiz)->esq == NULL) ||
                    ((*noRaiz)->dir == NULL) )
                {
                    No_AVL *temp = (*noRaiz)->esq ?
                                (*noRaiz)->esq :
                                (*noRaiz)->dir;
        
                    // No child case
                    if (temp == NULL)
                    {
                        temp = (*noRaiz);
                        (*noRaiz) = NULL;
                        //(*noRaiz)->excluido=true;
                    }
                    else // One child case
                    {
                        (**noRaiz) = *temp; // Copy the contents of
                        temp=NULL;
                        // the non-empty child
                        //temp->excluido=true;
                        //delete(temp);
                    }
                    this->N--;
                }
                else
                {
                    // noRaiz with two children: Get the inorder
                    // successor (smallest in the right subtree)
                    No_AVL* temp = getNoComMenorValor((*noRaiz)->dir);
        
                    // Copy the inorder successor's
                    // data to this noRaiz
                    (*noRaiz)->nome = temp->nome;
                    (*noRaiz)->numTelefone = temp->numTelefone;
                    (*noRaiz)->fb = temp->fb;
                    (*noRaiz)->esq = temp->esq;
                    (*noRaiz)->dir = temp->dir;
        
                    // Delete the inorder successor
                    exclui(&(*noRaiz)->dir, temp->numTelefone);
                }
            }
        
            // If the tree had only one noRaiz
            // then return
            if ((*noRaiz) == NULL){
                return;
            } 
        
            // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
            //noRaiz->height = 1 + max(height(noRaiz->esq),height(noRaiz->right));
        
            // STEP 3: GET THE BALANCE FACTOR OF
            // THIS NODE (to check whether this
            // noRaiz became unbalanced)
            int fatorDeBalanceamento = getFatorBalanceamento((*noRaiz));
        
            // If this noRaiz becomes unbalanced,
            // then there are 4 cases
        
            // Left Left Case
            if (fatorDeBalanceamento > 1 && getFatorBalanceamento((*noRaiz)->esq) >= 0)
                rotacaoDireitaSimples(&(*noRaiz));
        
            // Left Right Case
            if (fatorDeBalanceamento > 1 && getFatorBalanceamento((*noRaiz)->esq) < 0)
            {
                rotacaoEsquerdaSimples(&(*noRaiz)->esq);
                rotacaoDireitaSimples(&(*noRaiz));
            }
        
            // Right Right Case
            if (fatorDeBalanceamento < -1 && getFatorBalanceamento((*noRaiz)->dir) <= 0)
                rotacaoEsquerdaSimples(&(*noRaiz));
        
            // Right Left Case
            if (fatorDeBalanceamento < -1 && getFatorBalanceamento((*noRaiz)->dir) > 0)
            {
                rotacaoDireitaSimples(&(*noRaiz)->dir);
                rotacaoEsquerdaSimples(&(*noRaiz));
            }
        } 
    }
	
   void exclui(No_AVL **T, int telefone){
      No_AVL** aux1;
      No_AVL *aux2;
      No_AVL*  aux = *T; 
      
      if(*T != NULL) {
      
        if((*T)->numTelefone == telefone){  // Chave encontrada

          this->N--;  
        
          if((*T)->esq == (*T)->dir){ // é uma folha
         
            //printf("\nDebug: r = %d\n",chave);
            *T = NULL;             
            this->hAum=true;
    
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
            (*T)->nome = aux2->nome; 
            (*T)->numTelefone = aux2->numTelefone; 
            (*T)->dir = aux2->dir; 
            (*T)->esq = aux2->esq; 
            (*T)->fb = aux2->fb; 
            aux2 = NULL; 
            int fatorDeBalanceamento=getFatorBalanceamento(*T);
            if (fatorDeBalanceamento!=0){
                this->hAum=true;    
            }
          }
        } else if((*T)->numTelefone < telefone){ // Desce em busca da chave.
    
            exclui(&(*T)->dir,telefone);            

            if (this->hAum==true){
                atualizaBalanceamentoNoDir(T);
            }
    
        } else if((*T)->numTelefone > telefone) {
    
            exclui(&(*T)->esq,telefone);
                       
            if (this->hAum==true){
                atualizaBalanceamentoNoEsq(T);
            }
    
        }         
      } else {// Chave não encontrada
    	perror("Erro! Chave (número de telefone) não encontrada.\n");
        this->chaveEncontrada=false;  //Esta variável é checada após a execução desta função.
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