/*	ARVORE AVL
	Arquivo: ArvoreAVL.c
	Autor: tokyyto
	Data: 31/10/2024 13:42
	Descricao: implementacao de funcoes basicas envolvendo Arvores AVL
	Anotacoes: N/A
*/

//diretivas
#include <stdio.h>
#include <stdlib.h>
#include "Booleano.h"
#include "FilaPointer.h"

//definicoes de tipos
typedef struct No{
	int item;
	int fBal; //fator de balanceamento
	struct No * pai; //no pai
	struct No * esq;
	struct No * dir;
} No;

typedef No * AVL;


//interface
AVL criarAVLVazia(); //cria uma AVL vazia
int obterFBal(AVL); //devolve o fBal de uma AVL
No * buscarPai(AVL, int); //devolve o futuro pai de um no
AVL inserirAVL(AVL, int); //insere um item na AVL

No * rotacaoSimplesEsquerda(No *); //rotacao simples esquerda
No * rotacaoSimplesDireita(No *); //rotacao simples para direita
No * rotacaoDuplaEsquerda(No *); //rotacao dupla direita-esquerda
No * rotacaoDuplaDireita(No *); //rotacao dupla esquerda-direita

void mostrarAVL_ERD(AVL); //mostra a AVL usando caminhamento ERD
void mostrarAVL_EDR(AVL); //mostra a AVL usando o caminhamento EDR
void mostrarAVL_RED(AVL); //mostra a AVL usando o caminhamento RED
void mostrarAVL_BFS(AVL); //mostra a AVL usando o caminhamento BFS


int main(){
	AVL A;
	A = criarAVLVazia();
	A = inserirAVL(A, 5);
	A = inserirAVL(A, 3);
	A = inserirAVL(A, 9);
	A = inserirAVL(A, 8);
	A = inserirAVL(A, 13);
	A = inserirAVL(A, 25);
	
	
	printf("\n\ncaminhamento ERD:\n");
	mostrarAVL_ERD(A);
	printf("\n\ncaminhamento EDR:\n");
	mostrarAVL_EDR(A);
	printf("\n\ncaminhamento RED:\n");
	mostrarAVL_RED(A);
	printf("\n\ncaminhamento BFS:\n");
	mostrarAVL_BFS(A);
	
	return 0;
}

//implementacao
AVL criarAVLVazia(){ //cria uma AVL vazia
	AVL A;
	A = NULL;
	return A;
}

int obterFBal(AVL A){ //devove o fBal de uma AVL
	int fator;
	if(A != NULL){
		fator = A -> fBal;
	}
	else{
		fator = 0;
	}
	return fator;
}

No * buscarPai(AVL A, int n){ //devolve o futuro pai de um no
	No * p, * vater;
	vater = NULL;
	p = A;
	while(p != NULL){
		vater = p;
		if(p -> item > n){
			p = p -> esq;
		}
		else{
			p = p -> dir;
		}
	}
	return vater;
}

AVL inserirAVL(AVL A, int n){ //insere um item na AVL
	//p = "aux";	p: filho de vater (esquerdo ou direito) (inicialmente novo)
	//vater = "fa"	vater: pai atual (inicialmente do novo)
	//sohn = "enk"	sohn: filho de p
	//				opa: pai de vater (inicialmente avo do novo)
	No * novo, * p, * vater, * opa, * sohn;
	bool hMudou;
	int B1, B2, B3;
	novo = malloc(sizeof(No));
	novo -> item = n;
	novo -> fBal = 0;
	novo -> dir = NULL;
	novo -> esq = NULL;
	
	if(A == NULL){ //se estiver vazia
		novo -> pai = NULL;
		A = novo;
	}
	else{ //se nao estiver vazia, encontra eu pai e faz autobalanceamento, se necessario
		vater = buscarPai(A, n);
		novo -> pai = vater;
		if(vater -> item > novo -> item){
			vater -> esq = novo;
		}
		else{
			vater -> dir = novo;
		}
		hMudou = TRUE;
		p = novo;
		do{
			opa = vater -> pai;
			if(vater -> item > p -> item){ //desequilibrio do lado esquerdo
				B1 = vater -> fBal;
				switch(B1){
					case -1:	//verificando rotacoes e atualizando fBals
								B2 = p -> fBal;
								if(B1 == B2){
									vater -> fBal = 0;
									p -> fBal = 0;
									vater = rotacaoSimplesDireita(vater);
								}
								else{
									sohn = p -> dir;
									B3 = sohn -> fBal;
									switch(B3){
										case -1:	p -> fBal = 0;
													vater -> fBal = 1;
										case 0:		p -> fBal = 0;
													vater -> fBal = 0;
										case 1:		p -> fBal = -1;
													vater -> fBal = 0;
									}
									sohn -> fBal = 0;
									vater = rotacaoDuplaDireita(vater);
								}
								hMudou = FALSE;
								if(opa != NULL){ //liga o avo
									if(opa -> item > vater -> item){
										opa -> esq = vater;
									}
									else{
										opa -> dir = vater;
									}
								}
								else{
									A = vater;
								}
								break;
					case 0:		vater -> fBal = -1;
								p = vater;
								vater = vater -> pai;
								break;
					case 1:		vater -> fBal = 0;
								hMudou = FALSE;
								break;
				}
			}
			else{ //desequilibrio do lado direito
				B1 = vater -> fBal;
				switch(B1){
					case -1:	vater -> fBal = 0;
								hMudou = FALSE;
								break;
					case 0:		vater -> fBal = 1;
								p = vater;
								vater = vater -> pai;
								break;
					case 1:		//verificando rotacoes e atualizando fBals
								B2 = p -> fBal;
								if(B1 == B2){
									p -> fBal = 0;
									vater -> fBal = 0;
									vater = rotacaoSimplesEsquerda(vater);
								}
								else{
									sohn = p -> esq;
									B3 = sohn -> fBal;
									switch(B3){
										case -1:	p -> fBal = 1;
													vater -> fBal = 0;
										case 0:		p -> fBal = 0;
													vater -> fBal = 0;
										case 1:		p -> fBal = 0;
													vater -> fBal = -1;
									}
									sohn -> fBal = 0;
									vater = rotacaoDuplaEsquerda(vater);
								}
								hMudou = FALSE;
								if(opa != NULL){ //liga o avo
									if(opa -> item > vater -> item){
										opa -> esq = vater;
									}
									else{
										opa -> dir = vater;
									}
								}
								else{
									A = vater;
								}
								break;
				}
			}
		} while((hMudou == TRUE) && (vater != NULL));
	}
	return A;
}

No * rotacaoSimplesEsquerda(No * p){ //rotacao simples para esquerda em p
	No * y, * rse, * e, * vater;
	rse = p;
	if(p != NULL){
		vater = p -> pai;
		y = p -> dir;
		if(y != NULL){
			e = y -> esq;
			p -> dir = e;
			y -> esq = p;
			
			if(e != NULL){ //atualiza o pai de e
				e -> pai = p;
			}
			p -> pai = y; //atualiza o pai de p
			y -> pai = vater; //atualiza o pai de y
			rse = y;
		}
	}
	return rse;
}

No * rotacaoSimplesDireita(No * p){ //rotacao simples para direita em p
	No * y, * rsd, * d, * vater;
	rsd = p;
	if(p != NULL){
		vater = p -> pai;
		y = p -> dir;
		if(y != NULL){
			d = y -> dir;
			y -> dir = p;
			p -> esq = d;
			
			if(d != NULL){
				d -> pai = p;
			}
			p -> pai = y;
			y -> pai = vater;
			rsd = y;
		}
	}
	return rsd;
}

No * rotacaoDuplaEsquerda(No * p){ //rotacao dupla direita-esquerda em p
	No * y, * z, * rde, * e, * d, * vater;
	rde = p;
	if(p != NULL){
		vater = p -> pai;
		y = p -> dir;
		if(y != NULL){
			z = y -> esq;
			if(z != NULL){
				e = z -> esq;
				d = d -> dir;
				
				y -> esq = d;
				p -> dir = e;
				z -> esq = p;
				z -> dir = y;
				
				p -> pai = z;
				y -> pai = z;
				z -> pai = vater;
				if(e != NULL){
					e -> pai = p;
				}
				if(d != NULL){
					d -> pai = y;
				}
				rde = z;
			}
		}
	}
	return rde;
}

No * rotacaoDuplaDireita(No * p){ //rotacao dupla esquerda-direita em p
	No * y, * z, * rdd, * e, * d, * vater;
	rdd = p;
	if(p != NULL){
		vater = p -> pai;
		y = p -> esq;
		if(y != NULL){
			z = y -> dir;
			if(z != NULL){
				e = z -> esq;
				d = z -> dir;
				
				y -> dir = e;
				p -> esq = d;
				z -> esq = y;
				z -> dir = p;
				
				y -> pai = z;
				p -> pai = z;
				z -> pai = vater;
				if(e != NULL){
					e -> pai = y;
				}
				if(d != NULL){
					d -> pai = p;
				}
				rdd = z;
			}
		}
	}
	return rdd;
}

void mostrarAVL_ERD(AVL A){ //mostra a AVL usando o caminhamento ERD
	No * p;
	if(A != NULL){
		p = A;
		mostrarAVL_ERD(p -> esq);
		printf(" %d ", p -> item);
		mostrarAVL_ERD(p -> dir);
	}
}

void mostrarAVL_EDR(AVL A){ //mostra a AVL usando o caminhamento EDR
	No * p;
	if(A != NULL){
		p = A;
		mostrarAVL_EDR(p -> esq);
		mostrarAVL_EDR(p -> dir);
		printf(" %d ", p -> item);
	}
}

void mostrarAVL_RED(AVL A){ //mostra a AVL usando o caminhamento RED
	No * p;
	if(A != NULL){
		p = A;
		printf(" %d ", p -> item);
		mostrarAVL_RED(p -> esq);
		mostrarAVL_RED(p -> dir);
	}
}

void mostrarAVL_BFS(AVL A){ //mostra a AVL usando o caminhamento BFS
	No * p;
	Fila F;
	if(A != NULL){
		p = A;
		criarFilaVazia(&F);
		pushFila(&F, p);
		do{
			p = acessarFila(&F);
			printf(" %d ", p -> item);
			popFila(&F);
			if(p -> esq != NULL){
				pushFila(&F, p -> esq);
			}
			if(p -> dir != NULL){
				pushFila(&F, p -> dir);
			}
		} while(verificarFilaVazia(&F) == FALSE);
	}
	else{
		printf("\nArvore vazia!");
	}
}
