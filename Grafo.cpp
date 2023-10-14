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
        return true;
    }
    else
    {
        cout << "ID " << id_origem << " ja existe" << endl;
        return false;
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

        return true;
    }

    return false;
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

void Grafo::imprimirGraphviz(string nome)
{
    ofstream output;
    output.open(nome + ".dot");

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
            output << "graph " << nome << " {" << endl;
            conector = " -- ";
            cout << "Grafo não direcionado " << endl;
        }
        ListaArestas *lista;
        vertices->iterator = vertices->iteratorInicio();
        while (vertices->iterator != nullptr)
        {
            lista = vertices->iterator->getArestas();
            lista->iterator = lista->iteratorInicio();

            if(lista->iterator == nullptr)
            {
                output << vertices->iterator->getId() << endl;
            }
            else
            {
                while (lista->iterator != nullptr)
                {
                    Aresta *a = lista->iterator;
                    output << vertices->iterator->getId() << conector << a->getDestino()->getId() << endl;
                    lista->proximo();
                }
            }
            vertices->proximo();
        }
    }

    output << "}" << endl;
    output.close();


    string command = "dot -Tpng ";
    command = command.append(nome);
    command = command.append(".dot -o ");
    command = command.append(nome + ".png");

    cout << command << endl;
    system("pause");

    system(command.data());
    system("pause");
}

Grafo* Grafo::buscaEmLargura(int id)
{
    Vertice* inicio = this->vertices->busca(id);
    Grafo* resultado = new Grafo(0,1,0,0);

    ListaVertices* pilhaVertices = new ListaVertices();
    pilhaVertices->insereFinal(inicio);

    while(!pilhaVertices->ehVazia())
    {
        Vertice* atual = pilhaVertices->desempilhaPrimeiro();
        ListaArestas* arestas = atual->getArestas();
        arestas->imprimeLista();
        arestas->iterator = arestas->iteratorInicio();

        resultado->insereVertice(atual->getId());
        while(arestas->iterator != nullptr)
        {
            if(resultado->insereVertice(arestas->iterator->getDestino()->getId()))
            {
                resultado->insereAresta( atual->getId() ,arestas->iterator->getDestino()->getId(), 1);
                cout << "Destino " << arestas->iterator->getDestino()->getId() << endl;
                pilhaVertices->insereInicio(arestas->iterator->getDestino());
            }
            arestas->proximo();
        }
    }

    system("pause");
    resultado->imprimirGraphviz("buscaEmLargura");
    return resultado;
}
