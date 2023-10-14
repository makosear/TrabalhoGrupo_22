#include "TabelaHash.h"

using namespace std;

int TabelaHash::hashFunction(int chave)
{
    //cout <<"hash function" << endl;
    return chave % TAMANHO_LISTA;
};

TabelaHash::TabelaHash(int tamanho)
{

    index_iterator = 0;
    num_registros = 0;

    this->TAMANHO_LISTA = tamanho;
    tabela = new vector<ListaVertices*>;
    tabela->resize(tamanho);

    for (size_t i = 0; i < tabela->size(); i++)
    {
        tabela->at(i) = new ListaVertices();
    }
}

TabelaHash::~TabelaHash()
{
    for (size_t i = 0; i < tabela->size(); i++)
    {
        tabela->at(i)->limpaLista();
        delete tabela->at(i);
    }
}

Vertice *TabelaHash::busca(int chave)
{

    int indice = this->hashFunction(chave);
    ListaVertices *lista = tabela->at(indice);
    Vertice *vertice = lista->busca(chave);
    return lista->busca(chave);
}

int TabelaHash::getTamanhoTable()
{
    return this->TAMANHO_LISTA;
}

TabelaHash *TabelaHash::insere(int chave, Vertice *item)
{
    int indice = this->hashFunction(chave);
    ListaVertices *lista = tabela->at(indice);
    lista->insereFinal(item);
    num_registros++;
    return this;
};

TabelaHash *TabelaHash::remove(int chave)
{
    int indice = this->hashFunction(chave);
    ListaVertices *lista = tabela->at(indice);
    lista->deleta(chave);
    num_registros--;
    return this;
};

Vertice *TabelaHash::iteratorInicio()
{
    // cada indice da hash
    index_iterator = 0;
    this->iterator = nullptr;
    for (size_t index = 0; index < TAMANHO_LISTA; index++)
    {
        ListaVertices *listaVertices = tabela->at(index);
        if(listaVertices != nullptr && listaVertices->iteratorInicio()!= nullptr)
        {
            index_iterator = index;
            this->iterator = listaVertices->iteratorInicio();
            return this->iterator;
        }
    }
    return nullptr;
}
Vertice *TabelaHash::proximo()
{

    if (iterator == nullptr)
        return nullptr;


    //cout <<"iterator id antes " << iterator->getId() << endl;

    if (iterator->proximo == nullptr)
    {
        //cout << "fim da lista " << endl;
        while (index_iterator < TAMANHO_LISTA -1 )
        {
            index_iterator++;
            ListaVertices* lista = this->tabela->at(index_iterator);
            if(lista->iteratorInicio()!=nullptr)
            {
                this->iterator = lista->iteratorInicio();

                //cout <<"iterator id depois " << endl;
                return this->iterator;
            }
        }
        iterator = iterator->proximo;
        return iterator;

    }
    else
    {
        //cout << "dentro da lista " << endl;
        iterator = iterator->proximo;
        return iterator;
    }
}
