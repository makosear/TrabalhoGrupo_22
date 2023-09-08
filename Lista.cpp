#include "Lista.h"

Lista::Lista()
{
    primeiro = nullptr;
    ultimo = nullptr;
}
Lista::~Lista()
{
    limpa();
}
void Lista::insere(int id, float peso, Vertice* pt, bool visitado)
{
    if(this->busca(id) == nullptr)
    {
        LE* novo = new LE(id, peso, pt, visitado);

        novo->setAnt(ultimo);
        novo->setProx(nullptr);

        if(ultimo == nullptr && primeiro == nullptr) primeiro = novo;

        else ultimo->setProx(novo);

        ultimo = novo;
    }
}
LE* Lista::busca(int i)
{
    LE* b = primeiro;
    while(b!=nullptr)
    {

        if(b->getId() == i)
        {
            return b;
        }
        else
        {
            b = b->getProx();
        }
    }
    return nullptr;
}
void Lista::remove(int i)
{

    LE* b = busca(i);

    if(b != nullptr)
    {
        bool eh_primeiro = (b->getAnt() == nullptr);
        bool eh_ultimo = (b->getProx() == nullptr);
        LE* a = b->getAnt();
        LE* p = b->getProx();
        if(eh_primeiro && eh_ultimo)
        {
            primeiro = nullptr;
            ultimo = nullptr;
        }
        else if(eh_primeiro)
        {
            primeiro = p;
            p->setAnt(nullptr);
        }
        else if(eh_ultimo)
        {
            ultimo = a;
            a->setProx(nullptr);
        }
        else
        {
            a->setProx(p);
            p->setAnt(a);
        }
        b->setAnt(nullptr);
        b->setProx(nullptr);
        delete b;
    }
}
//void Lista::remove(LE* b){
//    if(b != nullptr){
//        bool eh_primeiro = (b->getAnt() == nullptr);
//        bool eh_ultimo = (b->getProx() == nullptr);
//        LE* a = b->getAnt();
//        LE* p = b->getProx();
//        if(eh_primeiro && eh_ultimo){
//            primeiro = nullptr;
//            ultimo = nullptr;
//        }else if(eh_primeiro){
//            primeiro = p;
//            p->setAnt(nullptr);
//        }else if(eh_ultimo){
//            ultimo = a;
//            a->setProx(nullptr);
//        }else{
//            a->setProx(p);
//            p->setProx(a);
//        }
//        b->setAnt(nullptr);
//        b->setProx(nullptr);
//        delete b;
//    }
//}
void Lista::imprime()
{
    LE* p = primeiro;
    if(p != nullptr)
    {
        while(p != nullptr)
        {
            cout << "[id]\t\t" << p->getId() << "[peso]" << p->getInfo() << "[visitado]" << p->getVisitado() << endl;
            p = p->getProx();
        }
        cout << "[primeiro]" << primeiro->getId() << "[ultimo] " << ultimo->getId() << endl;
    }
    else
    {
        cout << "LISTA VAZIA " << endl;
    }
}
bool Lista::vazia()
{
    if(primeiro == nullptr && ultimo == nullptr) return true;
    else return false;
}
void Lista::limpa()
{
    LE* p = primeiro;
    LE* t;
    while(p != nullptr)
    {
        t = p->getProx();
        delete p;
        p = t;
    }
    primeiro = nullptr;
    ultimo = nullptr;
}
bool Lista::fechada()
{
    LE* p = primeiro;
    while(p != nullptr)
    {
        if(p->getVisitado() == false)
        {
            return false;
        }
        p = p->getProx();
    }
    return true;
}
LE* Lista::buscaNaoVisitado()
{
    LE* p = primeiro;
    ///preferencia para vertices com peso ja avaliado
    while(p != nullptr)
    {
        if(p->getVisitado() == false && p->getInfo() != INFINITO)
        {
            return p;
        }
        p = p->getProx();
    }
    ///caso não encontrado retorna vertices de peso infino
    ///pensando nos casos de vertices isolados
    while(p != nullptr)
    {
        if(p->getVisitado() == false)
        {
            return p;
        }
        p = p->getProx();
    }
    ///nao encontrou nenhum vertice aberto
    return nullptr;
}
