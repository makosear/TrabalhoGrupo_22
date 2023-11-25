#include "Grafo.h"

#define INT_MAX 2147483640/2
#define FLT_MAX 2147483640/2

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
        ordem++;
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

float Grafo::dijkstra(int id_origem ,int id_destino , ofstream& output )
{
    if (id_origem != id_destino) {
        Vertice *vertAtual = vertices->busca(id_origem);

        if (vertAtual == nullptr || vertices->busca(id_destino) == nullptr){
            cout << "Vertice nao existe" << endl;
            return -1;
        }

        //primeiro inicializar um vetor com os vertices
        //vai armazenar o menor caminho para chegar a todos os vertices

        float *distancia = new float[ordem];

        //distancia minima da origem para um vertice

        int *visitado = new int[ordem]; //marca se um vertice foi visitado

        int *vertAdj = new int[ordem];
        //marca os vertices adjacentes

        for (int i = 0; i < ordem; i++) {
            distancia[i] = FLT_MAX;

            vertAdj[i] = -1;

            visitado[i] = -1;
        }

        distancia[vertAtual->getId()] = 0;

        ListaArestas *arestasNoAtual;

        float caminhoTam; // peso do caminho atual

        while (vertAtual->getId() != id_destino) {

            for (int i = 0; i < ordem; i++)
            {
                vertAdj[i] = -1;
            }

            caminhoTam = distancia[vertAtual->getId()];

            visitado[vertAtual->getId()] = 1;

            arestasNoAtual = vertAtual->getArestas();

            arestasNoAtual->iterator = arestasNoAtual->iteratorInicio();

            while (arestasNoAtual->iterator != nullptr) {
                Aresta *a = arestasNoAtual->iterator;

                int idVertAdj = a->getDestino()->getId();

                vertAdj[idVertAdj] = 1;

                if (distancia[idVertAdj] > caminhoTam + a->getPeso()) {

                    distancia[idVertAdj] = caminhoTam + a->getPeso();

                }

                arestasNoAtual->proximo();

                int proxCaminho = INT_MAX;

                int proxID = -1;

                for (int i = 0; i < ordem; i++) {
                    if (vertAdj[i] == 1) {
                        if (distancia[i] < proxCaminho && visitado[i] != 1) {
                            proxCaminho = distancia[i];
                            proxID = i;
                        }
                    }
                }

                vertAtual = vertices->busca(proxID);

            }
        }
        caminhoTam = distancia[vertAtual->getId()];

        delete []distancia;

        delete []visitado;

        delete []vertAdj;

        return caminhoTam;
    }
    else
    {
        //nenhum caminho se a origem == destino
        return 0;

    }
}