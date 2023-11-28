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

bool Grafo::insereVertice(int id_origem, float peso = 1, int coordX, int coordY)
{
    if (vertices->busca(id_origem) == nullptr)
    {
        vertices->insere(id_origem, new Vertice(id_origem, peso,coordX, coordY));
        ordem++;
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
bool Grafo::insereAresta(int id_origem, int id_destino, float peso, bool ehRetorno)
{
    //cout << "inserindo aresta origem: " << id_origem << " destino " << id_destino << endl;
    Vertice *origem = vertices->busca(id_origem);
    Vertice *destino = vertices->busca(id_destino);

    if(origem != nullptr && destino != nullptr)
    {
        peso = this->tem_peso_aresta ? peso : 1;
        origem->insereAresta(destino, peso, ehRetorno);
        origem->incSaida();

        if (!this->eh_direcionado)
        {
            destino->insereAresta(origem, peso, ehRetorno);
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

Vertice* Grafo::getVertice(int id)
{

    return this->vertices->busca(id);
}

void Grafo::geraDistanciasDeVertices()
{

    float distancia = 0;

    vertices->imprimeComoTabela();

    vertices->iterator = vertices->iteratorInicio();
    Vertice*  atual = vertices->iterator;
    while(vertices->iterator != nullptr)
    {
        cout<< "atual ---" << endl;
        atual->imprime();
        Vertice* iteracao = vertices->iterator = vertices->iteratorInicio();
        while(iteracao != nullptr)
        {
            if(atual->getId() != iteracao->getId())
            {
                //cout<< "    iteracao ---" << endl;
                //iteracao->imprime();
                distancia = sqrt(pow(iteracao->getX() - atual->getX(), 2) + pow(iteracao->getY() - atual->getY(), 2));
                this->insereAresta(atual->getId(), iteracao->getId(), distancia, false);
            }
            iteracao = vertices->proximo();
        }
        vertices->setIterator(atual);
        atual = vertices->proximo();
        //system("pause");
    }
}

void Grafo::imprimirGraphviz(string nome)
{
    ofstream output;
    output.open(nome + ".dot");

    Grafo* validacao = new Grafo(0,0,0,0);

    cout << "Abrindo o arquivo " << endl;

    if (output.is_open())
    {
        string conector;
        if (this->direcionado())
        {
            /// inserindo no output um grafo direcionado
            output << "digraph " << nome <<" {" << endl;
            conector = " -> ";
            cout << "Grafo direcionado " << endl;
        }
        else
        {
            /// inserindo no output um grafo n�o direcionado
            output << "strict graph " << nome << " {" << endl;
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
                    output << vertices->iterator->getId() << conector << a->getDestino()->getId();

                    if(a->ehArestaRetorno()) output << " [ color=\"red\" ]";

                    output << endl;
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
    system(command.data());
    system("pause");
}

Grafo* Grafo::SGVI_feixoTransitivoIndireto(int id_origem, ofstream& output)
{
    int index = 0;

    Grafo* resultado = new Grafo(0,1,0,0);
    Vertice* verticeInicio = this->vertices->busca(id_origem);
    TabelaHash* visitados = new TabelaHash(2);

    if(verticeInicio == nullptr) return nullptr;

    resultado->insereVertice(verticeInicio->getId(), verticeInicio->getPeso());

    ///iterando todos os vertices do grafo atual
    this->vertices->iterator = this->vertices->iteratorInicio();
    while(this->vertices->iterator != nullptr)
    {
        aux(resultado, visitados,this->vertices->iterator);
        //stringstream ss;
        //ss << index;
        //string str = ss.str();
        //std::string nome = "fecho_";
        //nome.append(str);
        //resultado->imprimirGraphviz(nome);
        this->vertices->proximo();
        index++;
    }

    resultado->imprimirGraphviz("fechoTransitivoIndireto");
    return resultado;
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
                resultado->insereAresta(atual->getId(),arestas->iterator->getDestino()->getId(), 1, false);
                pilhaVertices->insereInicio(arestas->iterator->getDestino());
            }
            else if(!arestas->iterator->getDestino()->comparaId(atual->getId()))
            {
                //resultado->insereAresta(atual->getId(),arestas->iterator->getDestino()->getId(), 1, true);
            }
            arestas->proximo();
        }
    }
    resultado->imprimirGraphviz("buscaEmLargura");
    return resultado;
}


bool Grafo::aux(Grafo* resultado, TabelaHash* visitados, Vertice*  inicio)
{
    ListaVertices* pilhaDeVertices = new ListaVertices();
    ListaVertices* caminho = new ListaVertices();
    pilhaDeVertices->insereInicio(inicio);

    //cout << "aux inicio " << inicio->getId() << endl;
    //system("pause");

    while(!pilhaDeVertices->ehVazia())
    {
        Vertice* atual= pilhaDeVertices->desempilhaPrimeiro();
        //atual->imprime();
        caminho->insereInicio(atual);
        ///se existe na solução atual
        if(resultado->vertices->busca(atual->getId()) != nullptr)
        {
            cout << "adicione todo o caminho ah solução e retorna" << endl;
            ///adicione todo o caminho ah solução e retorna
            Vertice* destino = caminho->desempilhaPrimeiro();
            caminho->imprimeLista();
            system("pause");
            while(!caminho->ehVazia())
            {
                Vertice* origem = caminho->desempilhaPrimeiro();
                //Aresta *a = origem->buscaAresta(destino->getId());
                resultado->insereVertice(origem->getId());
                resultado->insereAresta(origem->getId(), destino->getId(), 1, false);
                destino = origem;
            }
            visitados->insere(atual->getId(), atual);

            delete caminho;
            delete pilhaDeVertices;
            return true;

        }
        else if(atual->getArestas()->ehVazia() || (visitados->busca(atual->getId()) != nullptr))
        {
            ///desempilha do caminho pois chegamos a um fim
            cout << "desempilha do caminho pois chegamos a um fim" << endl;
            caminho->desempilhaPrimeiro();
        }
        else
        {
            ///poe todas as adjacencias na pilha de execução
            cout << "poe todas as adjacencias na pilha de execução" << endl;
            cout << "Adjacencias" << endl;
            atual->getArestas()->iterator = atual->getArestas()->iteratorInicio();

            while(atual->getArestas()->iterator != nullptr)
            {
                Vertice* verticeDestino = atual->getArestas()->iterator->getDestino();
                bool naoVisitadoOuSolucao = visitados->busca(verticeDestino->getId()) == nullptr ||
                                            resultado->vertices->busca(verticeDestino->getId()) != nullptr;

                if( naoVisitadoOuSolucao && pilhaDeVertices->busca(atual->getArestas()->iterator->getDestino()->getId()) == nullptr)
                {
                    cout <<"inserindo" << endl;
                    atual->getArestas()->iterator->getDestino()->imprime();
                    pilhaDeVertices->insereInicio(atual->getArestas()->iterator->getDestino());
                }
                atual->getArestas()->proximo();
            }
        }

        visitados->insere(atual->getId(), atual);
        pilhaDeVertices->imprimeLista();
    }

    delete caminho;
    delete pilhaDeVertices;
    return false;
}

float Grafo::dijkstra(int id_origem,int id_destino, ofstream& output )
{
    if (id_origem != id_destino)
    {
        Vertice *vertAtual = vertices->busca(id_origem);

        if (vertAtual == nullptr || vertices->busca(id_destino) == nullptr)
        {
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

        for (int i = 0; i < ordem; i++)
        {
            distancia[i] = FLT_MAX;

            vertAdj[i] = -1;

            visitado[i] = -1;
        }

        distancia[vertAtual->getId()] = 0;

        ListaArestas *arestasNoAtual;

        float caminhoTam; // peso do caminho atual

        while (vertAtual->getId() != id_destino)
        {

            for (int i = 0; i < ordem; i++)
            {
                vertAdj[i] = -1;
            }

            caminhoTam = distancia[vertAtual->getId()];

            visitado[vertAtual->getId()] = 1;

            arestasNoAtual = vertAtual->getArestas();

            arestasNoAtual->iterator = arestasNoAtual->iteratorInicio();

            while (arestasNoAtual->iterator != nullptr)
            {
                Aresta *a = arestasNoAtual->iterator;

                int idVertAdj = a->getDestino()->getId();

                vertAdj[idVertAdj] = 1;

                if (distancia[idVertAdj] > caminhoTam + a->getPeso())
                {

                    distancia[idVertAdj] = caminhoTam + a->getPeso();

                }

                arestasNoAtual->proximo();

                int proxCaminho = INT_MAX;

                int proxID = -1;

                for (int i = 0; i < ordem; i++)
                {
                    if (vertAdj[i] == 1)
                    {
                        if (distancia[i] < proxCaminho && visitado[i] != 1)
                        {
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
