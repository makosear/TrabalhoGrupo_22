#ifndef ARESTA_H
#define ARESTA_H
#pragma once

#include <iostream>

using namespace std;

class Vertice;
///Adotei a convencao de tudo que for referente a lista de arestas recebe sufixo LA

class Aresta
{
private:
    float peso;
    Vertice* destino;

    ///Encadeamento na lista de Arestas do vertice
    Aresta* ant;
    Aresta* prox;
public:
    Aresta()
    {
        peso = 1 ;
        ant = nullptr ;
        prox = nullptr ;
        destino = nullptr;
    };
    ~Aresta()
    {
        ant = nullptr ;
        prox = nullptr ;
        destino = nullptr;
    };

    float getPeso()
    {
        return peso;
    };
    Vertice* getDestino()
    {
        return destino;
    };
    void setPeso(float p)
    {
        peso = p;
    };
    void setDestino(Vertice* d)
    {
        destino = d;
    };

    ///Metodos da lista
    Aresta* getAnt_LA()
    {
        return ant;
    };
    Aresta* getProx_LA()
    {
        return prox;
    };
    void setAnt(Aresta* pta)
    {
        ant = pta;
    };
    void setProx(Aresta* ptp)
    {
        prox = ptp;
    };
};
#endif
