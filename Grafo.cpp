#include "Grafo.h"

Grafo::Grafo(int ehOrdenado, int temDirecao, int temPesoAresta, int temPesoVertice)
{
    eh_direcionado = temDirecao == 1 ? true : false;
    tem_peso_aresta = temPesoAresta == 1 ? true : false;
    tem_peso_vertice = temPesoVertice == 1 ? true : false;

    this->vertices = new TabelaHash(2);
}
Grafo::~Grafo()
{
    delete vertices;
}

bool Grafo::insereVertice(int id_origem, float peso = 1)
{
    if (vertices->busca(id_origem) == nullptr)
    {
        vertices->insere(id_origem, new Vertice(id_origem, peso));
    }
    else
    {
        cout << "ID " << id_origem << " ja existe" << endl;
    }
}
void Grafo::removeVertice(int id_origem)
{
    Vertice *v = vertices->busca(id_origem);
    // lembrar de decrementar o grau de entrada de todo vertice atingido e remover as arestas
    // que incidem sober o vertice a ser deletado
    if(v != nullptr)
    {
        ListaArestas* lista = v->getArestas();
        lista->iterator = lista->iteratorInicio();

        //precisamos ir onde cada aresta incide e deletar conexões com este vertice
        while (lista->iterator != nullptr)
        {
            lista->iterator->getDestino()->decEntrada();
            lista->iterator->getDestino()->removeAresta(id_origem);

            lista->proximo();
        }

        vertices->remove(id_origem);
    }


}
bool Grafo::insereAresta(int id_origem, int id_destino, float peso = 1)
{
    Vertice *origem = vertices->busca(id_origem);
    Vertice *destino = vertices->busca(id_destino);

    if(origem != nullptr && destino != nullptr)
    {
        peso = this->tem_peso_aresta ? peso : 1;
        origem->insereAresta(destino, peso);
        origem->incSaida();

        if (!this->eh_direcionado)
        {
            destino->insereAresta(origem, peso);
            destino->incEntrada();
        }
    }
}
void Grafo::removeAresta(int id_origem, int id_destino)
{

    Vertice *origem = vertices->busca(id_origem);
    Vertice *destino = vertices->busca(id_destino);

    if(origem != nullptr && destino != nullptr)
    {
        origem->removeAresta(destino->getId());
        origem->decSaida();

        if (!this->eh_direcionado)
        {
            destino->removeAresta(origem->getId());
            destino->decEntrada();
        }
    }
}

void Grafo::imprimirGraphviz()
{
    ofstream output;
    output.open("grafo.dot");
    cout << "Abrindo o arquivo " << endl;

    if (output.is_open())
    {
        string conector;
        if (this->direcionado())
        {
            /// inserindo no output um grafo direcionado
            output << "digraph teste{" << endl;
            conector = " -> ";
            cout << "Grafo direcionado " << endl;
        }
        else
        {
            /// inserindo no output um grafo n�o direcionado
            output << "graph teste{" << endl;
            conector = " -- ";
            cout << "Grafo não direcionado " << endl;
        }
        ListaArestas *lista;
        vertices->iterator = vertices->iteratorInicio();
        while (vertices->iterator != nullptr)
        {
            cout << "Vertice id " << vertices->iterator->getId() << endl;
            lista = vertices->iterator->getArestas();

            lista->iterator = lista->iteratorInicio();

            while (lista->iterator != nullptr)
            {
                Aresta *a = lista->iterator;
                output << vertices->iterator->getId() << conector << a->getDestino()->getId() << endl;
                lista->proximo();
            }
            vertices->proximo();
        }
    }

    output << "}" << endl;
    output.close();

    cout << "fim" << endl;
    system("pause");

    system("dot -Tpng grafo.dot -o grafo.png");
    system("pause");
}
