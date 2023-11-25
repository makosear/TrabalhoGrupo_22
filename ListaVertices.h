#ifndef LISTAVERTICES_H
#define LISTAVERTICES_H

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
#include "Vertice.h"

#pragma once

class Vertice;

class ListaVertices
{
public:
    ListaVertices();
    ~ListaVertices();

    ListaVertices* insereInicio(Vertice *item);
    ListaVertices* insereFinal(Vertice *item);
    ListaVertices *deleta(int id);
    Vertice *busca(int id);
    Vertice* desempilhaPrimeiro();

    void imprimeLista();

    bool ehVazia();
    void limpaLista();

    Vertice *iteratorInicio();
    Vertice *iteratorFim();
    Vertice *proximo();
    Vertice *anterior();

    Vertice *iterator;

private:
    void deleteAuxiliar(Vertice *item);

    Vertice *inicio;
    Vertice *fim;
    int tamanho = 0;
};

#endif
