#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <limits>


#include "Vertice.h"
#include <iostream>
#include <fstream>
#include "TabelaHash.h"
#include "ListaVertices.h"
#include "ListaArestas.h"

using namespace std;

#pragma once

class Grafo
{
	private:
		int ordem;
		bool eh_direcionado;
		bool tem_peso_aresta;
		bool tem_peso_vertice;
		///lista de vertices
        TabelaHash *vertices;

	public:
		Grafo(int ehOrdenado , int temDirecao , int temPesoAresta , int temPesoVertice);
		~Grafo();

		///getters imediatos
		int getOrdem() { return this->ordem; };
		bool direcionado() { return eh_direcionado;};
		bool temPesoAresta() { return tem_peso_aresta;};
		bool temPesoVertice() { return tem_peso_vertice;};

		///manipulacao da lista de vertices
		bool insereVertice(int id_origem ,float peso);
		void removeVertice(int id_origem);
		bool insereAresta(int id_origem , int id_destino , float peso);
		void removeAresta(int id_origem , int id_destino);

		Grafo* buscaEmLargura(int id);
		/*
		float dijkstra(int id_origem ,int id_destino , ofstream& output );
		float floyd(int origem , int destino , ofstream& output);
		Grafo* SGVI_feixoTransitivoDireto(int id_origem , ofstream& output);
		Grafo* SGVI_feixoTransitivoIndireto(int id_origem , ofstream& output);
		Grafo* buscaEmProfundidade(int id , ofstream& output);
		void ordenacaoTopologica(ofstream& output);
		Grafo* agmPrim(ofstream& output);
		Grafo* agmKruskal(ofstream& output);
		*/

		void imprimirGraphviz(string nome);

};

#endif
