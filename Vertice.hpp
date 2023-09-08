#ifndef VERTICE_H
#define VERTICE_H
#pragma once

#include "Aresta.hpp"

class Vertice
{
	private:
		int id;
		int entrada;
		int saida;
		float peso;

		//listas de aresta de cada vertice
		Aresta* primeiro_LA;
		Aresta* ultimo_LA;

		//encadeamento da lista de vertices do grafo
		Vertice* ant;
		Vertice* prox;

	public:
		Vertice(int novoId, float novoPeso);
		~Vertice();
		int getId() { return this->id;};
		int getGrauSaida() { return this->saida;};
		int getGrauEntrada() { return this->entrada;};
		void incSaida() { saida++;};
		void incEntrada() { entrada++;};
		void decSaida() { saida--;};
		void decEntrada() { entrada--;};
		float getPeso() { return this->peso;};
		void setPeso(float p) { this->peso = p;};

		///encadeamento da lista de arestas do vertice
		void insereAresta_LA(Vertice* pt_destino , float peso);
		void removeAresta_LA(int id_destino);
		void removeTodasArestas_LA(bool ord);
		Aresta* buscaAresta_LA(int id_destino);

		///encadeamento da lista de vertices do grafo
		Aresta* getPrimeiro_LA() { return primeiro_LA;};
		Vertice* getAnt_LV() { return this->ant;};
		Vertice* getProx_LV() { return this->prox;};
		void setAnt_LV(Vertice* a) { this->ant = a;};
		void setProx_LV(Vertice* p) { this->prox = p;};

		void imprimeLista();
};
#endif
