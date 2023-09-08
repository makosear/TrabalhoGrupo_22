#include "Vertice.hpp"

Vertice::Vertice(int novoId,float novoPeso)
{
    this->id = novoId;
    this->peso = novoPeso;
    this->entrada = 0;
    this->saida = 0;
    this->primeiro_LA = nullptr;
    this->ultimo_LA = nullptr;
    this->ant = nullptr;
    this->prox = nullptr;
}
Vertice::~Vertice()
{
    //this->removeTodasArestas_LA(false);
    Aresta* p = primeiro_LA;
    Aresta* t;
    while(p != nullptr)
    {
        t = p->getProx_LA();;
        delete p;
        p = t;
    }
    primeiro_LA = nullptr;
    ultimo_LA = nullptr;
    saida = 0;
    this->primeiro_LA = nullptr;
    this->ultimo_LA = nullptr;
    this->ant = nullptr;
    this->prox = nullptr;
}
Aresta* Vertice::buscaAresta_LA(int id_destino)
{
    Aresta* busca = primeiro_LA;
    while(busca != nullptr)
    {
        if(busca->getDestino() != nullptr)
        {
            if(busca->getDestino()->getId() == id_destino)
            {
                return busca;
            }
        }
        busca = busca->getProx_LA();
    }
    return nullptr;
}
void Vertice::insereAresta_LA(Vertice* pt_destino, float peso)
{

    Aresta* novaAresta = new Aresta;
    novaAresta->setDestino(pt_destino);
    novaAresta->setPeso(peso);

    novaAresta->setProx(nullptr);
    novaAresta->setAnt(ultimo_LA);

    if(primeiro_LA == nullptr && ultimo_LA == nullptr) primeiro_LA = novaAresta;

    else ultimo_LA->setProx(novaAresta);

    novaAresta->getDestino()->incEntrada();
    incSaida();
    ultimo_LA = novaAresta;
}
void Vertice::removeTodasArestas_LA(bool ord)
{
    ///essa funcao ta bugada
    Aresta* p = primeiro_LA;
    Aresta* t;
    while(p != nullptr)
    {
        //cout << "Deletando aresta para" << p->getDestino()->getId() << endl;
        if(!ord)
        {
            Vertice* temp = p->getDestino();
            if(temp != nullptr) temp->removeAresta_LA(this->id);
        }
        t = p->getProx_LA();
        delete p;
        p = t;
    }
    primeiro_LA = nullptr;
    ultimo_LA = nullptr;
    saida = 0;
}
void Vertice::removeAresta_LA(int id_destino)
{
    Aresta* busca = buscaAresta_LA(id_destino);
    if(busca != nullptr)
    {
        bool eh_primeiro = (busca->getAnt_LA() == nullptr);
        bool eh_ultimo = (busca->getProx_LA() == nullptr);
        Aresta* apt = busca->getAnt_LA();
        Aresta* ppt = busca->getProx_LA();

        if(eh_primeiro && eh_ultimo)
        {
            primeiro_LA = nullptr;
            ultimo_LA = nullptr;
        }
        else if(eh_primeiro)
        {
            primeiro_LA = ppt;
            ppt->setAnt(nullptr);
        }
        else if(eh_ultimo)
        {
            ultimo_LA = apt;
            apt->setProx(nullptr);
        }
        else
        {
            apt->setProx(ppt);
            ppt->setAnt(apt);
        }
        busca->setAnt(nullptr);
        busca->setProx(nullptr);
        busca->getDestino()->decEntrada();
        decSaida();

        delete busca;
    }
}
void Vertice::imprimeLista()
{
    Aresta* a = primeiro_LA;
    while(a != nullptr)
    {

        cout << " " << a->getDestino()->getId() << " " << endl;
        a = a->getProx_LA();
    }
    cout << endl;
}
