#ifndef VERTICE_H
#define VERTICE_H

#include "Aresta.cpp"
#include "ListaVertices.h"
#include "ListaArestas.h"

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

using namespace std;

class ListaArestas;
class Aresta;
class Vertice
{
private:
	int id;
	int grauEntrada;
	int grauSaida;
	float peso;
	ListaArestas *arestas;

public:

    Vertice *proximo;
    Vertice *anterior;

	Vertice(int novoId, float novoPeso);
	~Vertice();

	int getId() { return this->id; };
	int getGrauSaida() { return this->grauSaida; };
	int getGrauEntrada() { return this->grauEntrada; };
	float getPeso() { return this->peso; };

	void incSaida() { grauSaida++; };
	void incEntrada() { grauEntrada++; };
	void decSaida() { grauSaida--; };
	void decEntrada() { grauEntrada--; };
	void setPeso(float p) { this->peso = p; };

	Vertice *insereAresta(Vertice *destino, float peso);
	Vertice *removeAresta(int destino_id);
	//Vertice *removeAresta(Vertice *destino);
	Vertice *removeTodasArestas(bool ehOrdenado);
	Aresta *buscaAresta(int destino_id);
	//Aresta *buscaAresta(Vertice *destino);

	ListaArestas* getArestas(){ return this->arestas;};

	bool compara(Vertice *item)
	{
		return this == item;
	};
	bool comparaId(int id)
	{
		return this->getId() == id;
	};
};

#endif
