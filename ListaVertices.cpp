#include "ListaVertices.h"


ListaVertices::ListaVertices()
{
    tamanho = 0;
    fim = nullptr;
    inicio = nullptr;
    iterator = nullptr;
}

ListaVertices::~ListaVertices()
{
    tamanho = 0;
    fim = nullptr;
    inicio = nullptr;

    this->limpaLista();
}
ListaVertices *ListaVertices::insereInicio(Vertice *item)
{
    if(item == nullptr) return this;
    Vertice* novoItem = new Vertice(item);

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
}
ListaVertices *ListaVertices::insereFinal(Vertice *item)
{
    if(item == nullptr) return this;
    Vertice* novoItem = new Vertice(item);


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

void ListaVertices::deleteAuxiliar(Vertice *iteracaoAtual)
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

ListaVertices *ListaVertices::deleta(int id)
{
    Vertice *busca = this->busca(id);

    if (busca != nullptr)
    {

        deleteAuxiliar(busca);
    }

    tamanho--;
    return this;
}

Vertice *ListaVertices::busca(int id)
{
    Vertice *iteracaoAtual = inicio;

    while (iteracaoAtual != nullptr)
    {
        if (iteracaoAtual->comparaId(id))
        {
            return iteracaoAtual;
        }
        iteracaoAtual = iteracaoAtual->proximo;
    }

    return nullptr;
}

Vertice *ListaVertices::iteratorInicio()
{
    return this->inicio;
}
Vertice *ListaVertices::iteratorFim()
{
    return this->fim;
}
Vertice *ListaVertices::proximo()
{
    if (this->iterator != nullptr)
    {
        iterator = iterator->proximo;
        return iterator;
    }
    return nullptr;
}

Vertice* ListaVertices::anterior()
{
    if (this->iterator != nullptr)
    {
        iterator = iterator->anterior;
        return iterator;
    }
    return nullptr;
}

bool ListaVertices::ehVazia()
{
    return inicio == fim && inicio == nullptr;
}
void ListaVertices::limpaLista()
{

    Vertice* iteracao = inicio;

    while (iteracao != nullptr)
    {
        Vertice* toDelete = iteracao;
        iteracao = iteracao->proximo;
        delete toDelete;
    }

    inicio = fim = nullptr;
}

void ListaVertices::imprimeLista()
{

    Vertice* it = inicio;

    while(it != nullptr)
    {
        if(it->anterior == nullptr)
        {
            cout << "<-- ";
        }

        cout << it->getId() << " ";

        if(it->proximo == nullptr)
        {
            cout << "--> ";
        }

        it = it->proximo;
    }
    cout << endl;
}

Vertice* ListaVertices::desempilhaPrimeiro()
{
    if(this->ehVazia()) return nullptr;

    Vertice* retorno = inicio;
    inicio=inicio->proximo;

    if (inicio != nullptr)
    {
        inicio->anterior = nullptr;
    }
    else
    {
        fim = nullptr;
    }

    tamanho--;
    retorno->proximo = retorno->anterior = nullptr;

    return retorno;
}

