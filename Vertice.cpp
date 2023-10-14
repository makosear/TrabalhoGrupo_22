#include "Vertice.h"

Vertice::Vertice(int novoId, float novoPeso)
{
    id = novoId;
    grauEntrada = 0;
    grauSaida = 0;
    peso = novoPeso;
    arestas = new ListaArestas();

    this->proximo = nullptr;
    this->anterior = nullptr;

}

Vertice::~Vertice()
{
    id = -1;
    grauEntrada = 0;
    grauSaida = 0;
    peso = 0;
    arestas->limpaLista();
    delete arestas;
}

Vertice *Vertice::insereAresta(Vertice *destino, float peso)
{
    if (destino == nullptr)
        return nullptr;

    Aresta *novaAresta = new Aresta(peso, destino, destino->getId());
    // essa busca obrigatoriamente é por id
    // porque a outra busca é por endereço de memória

    if (arestas->busca(novaAresta->getDestino()->getId()) == nullptr)
        arestas->insereFinal(novaAresta);

    //cout << "Debug Arestas do Vertice id " << this->getId()<< endl;
    //arestas->imprimeLista();
    //system("pause");
}
Vertice *Vertice::removeAresta(int destino_id)
{
    arestas->deleta(destino_id);
}
Vertice *Vertice::removeTodasArestas(bool ehDirecionado)
{
}
Aresta *Vertice::buscaAresta(int destino_id)
{
    return arestas->busca(destino_id);
}

