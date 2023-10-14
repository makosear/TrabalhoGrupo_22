#ifndef LISTAARESTAS_H
#define LISTAARESTAS_H

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


#include "Aresta.cpp"

#pragma once

class Aresta;
class ListaArestas
{
public:
    ListaArestas();
    ~ListaArestas();

    ListaArestas *insereInicio(Aresta *item);
    ListaArestas *insereFinal(Aresta *item);

    ListaArestas *deleta(int id);
    Aresta *busca(int id);

    void imprimeLista();

    bool ehVazia();
    void limpaLista();

    Aresta *iteratorInicio();
    Aresta *iteratorFim();
    Aresta *proximo();
    Aresta *anterior();

    Aresta *iterator;

private:
    void deleteAuxiliar(Aresta *item);

    Aresta *inicio;
    Aresta *fim;
    int tamanho = 0;
};
#endif
