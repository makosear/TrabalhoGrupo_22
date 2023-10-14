#ifndef TABELAHASH_H
#define TABELAHASH_H

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
#include "ListaVertices.h"
#include "ListaArestas.h"
#include <vector>

#pragma once

using namespace std;

class TabelaHash
{
public:
    TabelaHash(int tamanho);
    ~TabelaHash();

    TabelaHash* insere(int chave, Vertice* item);
    TabelaHash* remove(int chave);
    int getTamanhoTable();


    void imprimeLista();

    Vertice *busca(int chave);

    Vertice* iteratorInicio();
    Vertice* proximo();

    Vertice *iterator;

private:

    vector<ListaVertices*>* tabela;
    int index_iterator;
    int num_registros;
    int TAMANHO_LISTA;
    int hashFunction(int chave);
};

#endif
