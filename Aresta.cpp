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
    Vertice *destino;

public:

    Aresta *proximo;
    Aresta *anterior;

    Aresta()
    {
        peso = 1;
        destino = nullptr;
    };

    Aresta(float peso, Vertice *destino, int destinoId)
    {
        this->peso = peso;
        this->destino = destino;
        this->destino_id = destinoId;
        this->proximo = nullptr;
        this->anterior = nullptr;
    };

    ~Aresta() { destino = nullptr; };

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

    float getPeso() { return peso; };
    Vertice *getDestino() { return destino; };

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
};

#endif
