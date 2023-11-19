#ifndef ARESTA_H
#define ARESTA_H

#include "Vertice.h"

#pragma once

class Vertice;
class Aresta
{
private:
    float peso;
    int destino_id;
    bool ehRetorno;

    Vertice *destino;

public:

    Aresta *proximo;
    Aresta *anterior;

    Aresta()
    {
        peso = 1;
        destino = nullptr;
        ehRetorno = false;
    };

    Aresta(Aresta* copia)
    {
        peso = copia->peso;
        destino = copia->destino;
        ehRetorno = copia->ehRetorno;
        this->proximo = nullptr;
        this->anterior = nullptr;
    };


    Aresta(float peso, Vertice *destino)
    {
        this->peso = peso;
        this->destino = destino;
        this->proximo = nullptr;
        this->anterior = nullptr;
    };

    ~Aresta()
    {
        destino = nullptr;
    };

    bool compara(Aresta *item)
    {
        return this == item;
    }
    bool comparaId(int id)
    {
        if (this->destino != nullptr)
            return this->destino_id == id;

        return false;
    }

    bool ehArestaRetorno()
    {
        return ehRetorno;
    }

    float getPeso()
    {
        return peso;
    };
    Vertice *getDestino()
    {
        return destino;
    };

    // O return this permite encadear metodos do mesmo objeto
    // por exemplo fazer aresta.setPeso().setDestino()
    Aresta *setPeso(float peso)
    {
        this->peso = peso;
        return this;
    };

    Aresta *setDestino(Vertice *destino)
    {
        this->destino = destino;
        return this;
    };

    void setEhRetorno(bool retorno)
    {
        this->ehRetorno = retorno;
    };
};

#endif
