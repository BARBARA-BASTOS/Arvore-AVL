#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
    int valor;
    struct nodo *esq;
    struct nodo *dir;
    struct nodo *pai;
    int h;
} Nodo;

Nodo* criarNodo(int v);
Nodo* buscar(Nodo *raiz, int v);
void mostrarArvore(Nodo *p, int nivel);
Nodo* inserirNodo(Nodo *raiz, int v);
Nodo* excluir(Nodo *raiz, int v);
int fb(Nodo *p);
Nodo* rotEsq(Nodo *raiz);
Nodo* rotDir(Nodo *raiz);
Nodo* rotEsqDir(Nodo *raiz);
Nodo* rotDirEsq(Nodo *raiz);
Nodo* balancear(Nodo *raiz);

Nodo* criarNodo(int v){
    Nodo *p = (Nodo*)malloc(sizeof(Nodo));
    p->valor = v;
    p->esq = NULL;
    p->dir = NULL;
    p->h = 0;
    return p;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int fb(Nodo *p) {
    int fator;

    if (!p) {
        fator = 0;
    } else {
        int altura_esq = p->esq ? p->esq->h : 0;
        int altura_dir = p->dir ? p->dir->h : 0;
        p->h = maior(altura_esq, altura_dir) + 1;
        fator = altura_esq - altura_dir;
    }
    return fator;
}

Nodo* rotDir(Nodo *raiz) {
    if (!raiz || !raiz->esq) {
        return raiz;
    }

    Nodo* aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;

    fb(raiz->dir);
    fb(raiz);
    return aux;
}

Nodo* rotEsq(Nodo *raiz) {
    if (!raiz || !raiz->dir) {
        return raiz;
    }

    Nodo* aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;

    fb(raiz->esq);
    fb(raiz);
    return aux;
}

Nodo* rotEsqDir(Nodo *raiz) {
    if (!raiz || !raiz->esq) {
        return raiz;
    }

    raiz->esq = rotEsq(raiz->esq);
    return rotDir(raiz);
}

Nodo* rotDirEsq(Nodo *raiz) {
    if (!raiz || !raiz->dir) {
        return raiz;
    }

    raiz->dir = rotDir(raiz->dir);
    return rotEsq(raiz);
}


Nodo* balancear(Nodo *raiz){
    int fator;
    fator = fb(raiz);
    if (fator > 1) {
        if (fb(raiz->esq) >= 0) {
            raiz = rotDir(raiz);
        } else {
            raiz = rotEsqDir(raiz);
        }
    } else if (fator < -1) {
        if (fb(raiz->dir) <= 0) {
            raiz = rotEsq(raiz);
        } else {
            raiz = rotDirEsq(raiz);
        }
    }
    fb(raiz);
    return raiz;
}

void mostraArvore(Nodo *p, int nivel){
    int i;
    if(p){
        mostraArvore(p->dir,nivel+1);
        printf("\n");
        for(i=0;i<nivel;i++)
            printf("\t");
        printf("%i [%i|%i]\n",p->valor,p->h,fb(p));
        mostraArvore(p->esq,nivel+1);
    }
}

Nodo* inserirNodo(Nodo *raiz, int v){
    if(!raiz){
        raiz = criarNodo(v);
    }else{
        if(v <= raiz->valor){
            raiz->esq = inserirNodo(raiz->esq,v);
        }else{
            raiz->dir = inserirNodo(raiz->dir,v);
        }
    }
    raiz = balancear(raiz);
    return raiz;
}

Nodo* buscar(Nodo* raiz, int v) {
    Nodo* atual = raiz;

    while (atual) {
        if (v == atual->valor) {
            return atual;
        } else if (v < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    return NULL;
}


Nodo* excluir(Nodo *p, int v) {
    if (p == NULL) {
        return NULL;
    }

    if (p->valor == v) {
        if (p->dir == NULL && p->esq == NULL) {
            free(p);
            return NULL;
        } else if (p->dir == NULL && p->esq != NULL) {
            Nodo* aux = p->esq;
            free(p);
            return balancear(aux);
        } else if (p->esq == NULL && p->dir != NULL) {
            Nodo* aux = p->dir;
            free(p);
            return balancear(aux);
        } else {
            Nodo* aux = p->esq;
            while (aux->dir != NULL) {
                aux = aux->dir;
            }
            p->valor = aux->valor;
            p->esq = excluir(p->esq, aux->valor);
            return balancear(p);
        }
    } else if (p->valor > v) {
        p->esq = excluir(p->esq, v);
    } else {
        p->dir = excluir(p->dir, v);
    }

    if (p) {
        fb(p);
        return balancear(p);
    }

    return p;
}

int main(){
    Nodo *raiz = NULL, *aux = NULL;
    int op, v;

    do{
        mostraArvore(raiz,0);
        printf("\n--------------------------------------------------\n");
        printf("1 - inserir novo nodo\n");
        printf("2 - excluir nodo\n");
        printf("3 - buscar nodo\n");
        printf("0 - sair\n");
        printf("opcao: ");
        scanf("%d",&op);

        switch(op){
        case 1:
            printf("Informe o valor (int): ");
            scanf("%d",&v);
            raiz = inserirNodo(raiz,v);
            break;
        case 2:
            printf("Informe o valor (int): ");
            scanf("%d",&v);
            raiz = excluir(raiz, v);
            break;
        case 3:
            printf("Informe o valor (int): ");
            scanf("%d",&v);
            if(aux = buscar(raiz,v))
                printf("valor encontrado\n");
            else
                printf("valor nao pertence a arvore\n");
            break;
        case 0:
            printf("saindo...\n");
            break;
        default:
            printf("opcao invalida\n");
        }
        printf("\n\n\n");
    }while(op!=0);

    return 0;
}
