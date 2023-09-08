#ifndef GRAFO_H
#define GRAFO_H
#include "Vertice.hpp"
#include "Lista.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <map>
#pragma once

extern float INFINITO;

class Grafo
{
	private:
		int ordem;
		bool eh_direcionado;
		bool tem_peso_aresta;
		bool tem_peso_vertice;

		///lista de vertices
		Vertice* primeiro_LV;
		Vertice* ultimo_LV;

		///auxiliares de recursao
		void profundidade(Vertice *v , Grafo* a , Grafo* r ,  Lista* l , ofstream& out, string conector);
		void auxFeixoTransitivoDireto(Vertice* vAtual , Grafo* g, ofstream& output , string conector);
		bool auxFeixoTransitivoIndireto(Vertice* vAtual , Grafo* g , ofstream& output , string conector , Lista* valido , Lista* visitado, int id_busca );

	public:

		void TESTE_imprimeGrafo();

		Grafo(int o , int d , int tpa , int tpv);
		~Grafo();

		///getters imediatos
		int getOrdem() { return this->ordem; };
		bool direcionado() { return eh_direcionado;};
		bool temPesoAresta() { return tem_peso_aresta;};
		bool temPesoVertice() { return tem_peso_vertice;};
		Vertice* getPrimeiro() { return primeiro_LV; };
		Vertice* getUltimo() { return ultimo_LV; };

		///manipulacao da lista de vertices
		bool insereVertice(int id_origem ,float peso);
		void removeVertice(int id_origem);
		bool insereAresta(int id_origem , int id_destino , float peso);
		void removeAresta(int id_origem , int id_destino);
		Vertice* buscaVertice(int id_origem);

		float dijkstra(int id_origem ,int id_destino , ofstream& output );
		float floyd(int origem , int destino , ofstream& output);
		Grafo* SGVI_feixoTransitivoDireto(int id_origem , ofstream& output);
		Grafo* SGVI_feixoTransitivoIndireto(int id_origem , ofstream& output);
		Grafo* buscaEmProfundidade(int id , ofstream& output);
		Grafo* buscaEmLargura(int id , ofstream& output);
		void ordenacaoTopologica(ofstream& output);
		Grafo* agmPrim(ofstream& output);
		Grafo* agmKruskal(ofstream& output);

};
#endif
