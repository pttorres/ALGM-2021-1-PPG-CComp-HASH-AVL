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

    No_AVL** maiorEsquerdoOLD(No_AVL** T)
    {
        if((*T)->esq != NULL)
        {
            (*T)=(*T)->esq;
            while((*T)->dir != NULL) 
            {
                (*T) = (*T)->dir;
            }
        }
        return (T);
    };

    No_AVL** menorDireitoOLD(No_AVL** T)
    {
        if((*T)->dir != NULL)
        {
            (*T)=(*T)->dir;
            while((*T)->esq != NULL) 
            {
                (*T) = (*T)->esq;
            }
        }
        return (T);
    };    

    /* Atualiza o fator de balanceamento
    retorna T->fb==0 se a árvore estiver balanceada
    retorna T->fb==número int. negativo se estiver desbalanceada à esquerda
    retorna T->fb==número inteiro positivo se a direita estiver maior  */
    void atualizafbT_OLD(No_AVL** T){
        (*T)->fb=getFatorBalanceamento(*T);
    };

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