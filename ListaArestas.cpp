#include "ListaArestas.h"

ListaArestas::ListaArestas()
{
    tamanho = 0;
    fim = nullptr;
    inicio = nullptr;
    iterator = nullptr;
}

ListaArestas::~ListaArestas()
{
    tamanho = 0;
    fim = nullptr;
    inicio = nullptr;


}
ListaArestas *ListaArestas::insereInicio(Aresta *item)
{
    if(item == nullptr) return this;
    Aresta* novoItem = new Aresta(item);


    // se lista vazia
    if (this->inicio == nullptr && this->fim == nullptr)
    {
        inicio = fim = novoItem;
        novoItem->proximo = nullptr;
        novoItem->anterior = nullptr;
    }
    else
    {
        novoItem->proximo = inicio;
        novoItem->anterior = nullptr;
        inicio->anterior = novoItem;
        this->inicio = novoItem;
    }
    tamanho++;
    return this;
}
ListaArestas *ListaArestas::insereFinal(Aresta *item)
{
    if(item == nullptr) return this;
    Aresta* novoItem = new Aresta(item);

    // se lista vazia
    if (this->inicio == nullptr && this->fim == nullptr)
    {
        inicio = fim = novoItem;
        novoItem->proximo = nullptr;
        novoItem->anterior = nullptr;
    }
    else
    {
        novoItem->anterior = fim;
        novoItem->proximo = nullptr;
        fim->proximo = novoItem;
        fim = novoItem;
    }
    tamanho++;

    return this;
}

void ListaArestas::deleteAuxiliar(Aresta *iteracaoAtual)
{
    if (iteracaoAtual == inicio)
    {
        inicio->anterior = nullptr;
        inicio = iteracaoAtual->proximo;
    }

    if (iteracaoAtual == fim)
    {
        fim->proximo = nullptr;
        fim = iteracaoAtual->anterior;
    }

    if (iteracaoAtual->anterior != nullptr)
        iteracaoAtual->anterior->proximo = iteracaoAtual->proximo;

    if (iteracaoAtual->proximo != nullptr)
        iteracaoAtual->proximo->anterior = iteracaoAtual->anterior;

    delete iteracaoAtual;
}

ListaArestas *ListaArestas::deleta(int id)
{
    Aresta *busca = this->busca(id);

    if (busca != nullptr)
        deleteAuxiliar(busca);

    tamanho--;
    return this;
}

Aresta *ListaArestas::busca(int id)
{
    Aresta *iteracaoAtual = inicio;

    while (iteracaoAtual != nullptr)
    {
        if (iteracaoAtual->getDestino()->getId() == id)
        {
            return iteracaoAtual;
        }
        iteracaoAtual = iteracaoAtual->proximo;
    }

    return nullptr;
}

Aresta *ListaArestas::iteratorInicio()
{
    this->iterator = inicio;
    return iterator;
}
Aresta *ListaArestas::iteratorFim()
{
    this->iterator = fim;
    return iterator;
}
Aresta *ListaArestas::proximo()
{
    if (this->iterator != nullptr)
    {
        iterator = iterator->proximo;
        return iterator;
    }
    return nullptr;
}

Aresta *ListaArestas::anterior()
{
    if (this->iterator != nullptr)
    {
        iterator = iterator->anterior;
        return iterator;
    }
    return nullptr;
}

bool ListaArestas::ehVazia()
{
    return inicio == fim && inicio == nullptr;
}
void ListaArestas::limpaLista()
{

    Aresta *iteracao = inicio;

    while (iteracao != nullptr)
    {
        Aresta *toDelete = iteracao;
        iteracao = iteracao->proximo;
        delete toDelete;
    }

    inicio = fim = nullptr;
}

void ListaArestas::imprimeLista()
{

    Aresta* it = inicio;

    while(it != nullptr)
    {
        if(it->anterior == nullptr)
        {
            cout << "<-- ";
        }

        cout << it->getDestino()->getId() << " ";

        if(it->proximo == nullptr)
        {
            cout << "--> ";
        }

        it = it->proximo;
    }

    cout << endl;;

}

