#ifndef LISTA_H
#define LISTA_H
#include "Vertice.hpp"
#include <limits>

extern float INFINITO;

class LE{

public:

    int getId() { return id; };
    void setId(int _id) { id = _id ;};

    LE* getElementoPai() { return anterior_caminho;};
    void setElementoPai(LE* v) { anterior_caminho = v;};

    float getInfo() { return info; };
    void setInfo(float i) { info = i; };

    bool getVisitado() { return visitado; };
    void setVisitado(bool b) { visitado = b; };

    Vertice* getPointer() { return pt; };
    void setPointer(Vertice* v) { pt = v ;};

    LE()                                            { id = 0 ; anterior_caminho = nullptr; info = 0 ; pt = nullptr , visitado = 0; };
    LE( int i , float p , Vertice* ptx , bool v)    { id = i ; anterior_caminho = nullptr; info = p ; pt = ptx , visitado = v; };
    ~LE()                                           { } ;
    void setAnt(LE *p)                              { ant = p; };
    void setProx(LE *p)                             { prox = p; };
    LE* getAnt()                                    { return ant; };
    LE* getProx()                                   { return prox; };
private:
    int id;
    LE* anterior_caminho;
    float info;
    Vertice* pt;
    bool visitado;

    LE *ant;  /// ponteiro para anterior
    LE *prox; /// ponteiro para proximo
};
class Lista
{
    public:
        Lista();
        ~Lista();
        void insere(int id, float info , Vertice* pt , bool visitado);
        void remove(int id);
        void limpa();
        void imprime();
        bool vazia();
        bool fechada();
        LE* buscaNaoVisitado();
        LE* busca(int id);
        LE* getPrimeiro() { return primeiro;};
    private:
        LE* primeiro;
        LE* ultimo;
};

#endif // LISTA_H
