#include "Grafo.hpp"

Grafo::Grafo(int ord, int direc, int pa, int pv)
{
    this->ordem = ord;
    this->eh_direcionado = direc != 0 ? true : false;
    this->tem_peso_aresta = pa != 0 ? true : false;
    this->tem_peso_vertice = pv != 0 ? true : false;

    this->primeiro_LV = nullptr;
    this->ultimo_LV = nullptr;
}
Grafo::~Grafo()
{
    Vertice* p;
    Vertice* t;
    p = primeiro_LV;
    while (p != nullptr)
    {
        cout << "Deletando vertice " << p->getId() << endl;
        t = p->getProx_LV();
        delete p;
        p = t;
    }
}
Vertice* Grafo::buscaVertice(int id)
{
    Vertice* busca = this->primeiro_LV;

    while(busca != nullptr)
    {
        if(busca->getId()== id) return busca;

        else busca = busca->getProx_LV();
    }
    return nullptr;
}
bool Grafo::insereVertice(int id, float peso)
{
    if(this->buscaVertice(id) == nullptr)
    {
        Vertice* nv = new Vertice(id,peso);
        nv->setProx_LV(nullptr);
        nv->setAnt_LV(ultimo_LV);

        if(primeiro_LV==nullptr && ultimo_LV==nullptr) primeiro_LV = nv;

        else ultimo_LV->setProx_LV(nv);

        ultimo_LV = nv;

        ordem++;

        return true;
    }
    return false;
}
bool Grafo::insereAresta(int id_origem, int id_destino, float peso)
{
    Vertice* origem = this->buscaVertice(id_origem);
    Vertice* destino = this->buscaVertice(id_destino);

    if(origem != nullptr)
    {
        if(origem->buscaAresta_LA(id_destino) != nullptr)
        {
            //cout << "existe aresta entre " << id_origem << " e " << id_destino << endl;
            return false;
        }
        //cout << "inserindo..." << endl;
        if(!this->temPesoAresta()) peso = 1;

        if(origem != nullptr && destino != nullptr)
        {
            if(origem->buscaAresta_LA(id_destino) == nullptr)
            {
                origem->insereAresta_LA(destino, peso);
                if(!direcionado())
                {
                    destino->insereAresta_LA(origem, peso);
                }
            }
        }
        return true;
    }
    return false;
}
void Grafo::removeVertice(int id)
{
    Vertice* busca = this->buscaVertice(id);

    if(busca !=nullptr)
    {
        bool eh_primeiro = (busca->getAnt_LV() == nullptr);
        bool eh_ultimo = (busca->getProx_LV() == nullptr);
        Vertice* a = busca->getAnt_LV();
        Vertice* p = busca->getProx_LV();

        if(eh_primeiro && eh_ultimo)
        {
            this->primeiro_LV = nullptr;
            this->ultimo_LV = nullptr;
        }
        else if(eh_primeiro)
        {
            this->primeiro_LV = p;
            p->setAnt_LV(nullptr);
        }
        else if(eh_ultimo)
        {
            ultimo_LV = a;
            a->setProx_LV(nullptr);
        }
        else
        {
            a->setProx_LV(p);
            p->setAnt_LV(a);
        }
        busca->setAnt_LV(nullptr);
        busca->setProx_LV(nullptr);

        delete busca;

        ordem--;
    }
}
void Grafo::removeAresta(int id_origem, int id_destino)
{
    Vertice * origem = buscaVertice(id_origem);

    if(origem != nullptr)
    {
        if(!direcionado())
        {
            Vertice* destino = buscaVertice(id_destino);
            if(destino != nullptr)
            {
                destino->removeAresta_LA(id_origem);
            }
        }
        origem->removeAresta_LA(id_destino);
    }
}
float Grafo::dijkstra(int id_origem, int id_destino , ofstream& output)
{

    ofstream tabela;
    tabela.open("tebelaDistancias.txt");

    if(tabela.is_open())
    {
        ///inicializacao
        Lista *auxLista = new Lista;
        for(Vertice* it = primeiro_LV ; it != nullptr ; it = it->getProx_LV())
        {
            if(it->getId() == id_origem)
            {
                auxLista->insere(it->getId(), 0, it, true);
            }
            else
            {
                auxLista->insere(it->getId(), INFINITO, it, false);
            }
        }
        ///inicio das iteracoes
        int id_rf = id_origem; ///id de referencia das iteracoes
        int id_minimo; ///minimo encontrado
        LE* temp;
        while(!auxLista->fechada())
        {
            LE* rf = auxLista->busca(id_rf);
            Aresta* a;

            if(rf != nullptr) a = rf->getPointer()->getPrimeiro_LA();
            else a = nullptr;

            bool sem_saida = true;
            float minimo = INFINITO;
            for(; a != nullptr ; a = a->getProx_LA())
            {
                /// pega o id do vertice conectado na aresta e localiza ele na lista auxiliar
                int id_aux = a->getDestino()->getId();
                temp = auxLista->busca(id_aux);

                if(temp != nullptr)
                {
                    if(temp->getVisitado() == false)
                    {
                        sem_saida = false;///encontrou pelo menos 1 vizinho aberto
                        if(temp->getInfo() > (rf->getInfo() + a->getPeso()))
                        {
                            ///atualiza o peso dos caminhos
                            temp->setInfo(rf->getInfo() + a->getPeso());
                            temp->setElementoPai(rf); ///salvando o anterior para poder reconstruir o caminho
                        }
                        if(minimo > temp->getInfo())
                        {
                            ///escolhendo o minimo caminho entre as arestas possiveis
                            minimo = temp->getInfo();
                            id_minimo = temp->getId();
                        }
                    }
                }
            }
            temp = auxLista->busca(id_minimo);

            if(temp != nullptr) temp->setVisitado(true);

            ///settando a proxima iteração a partir do caminho minimo encontrado
            if(sem_saida)
            {
                ///nao encontrei vizinhos abertos
                ///pulando a iteração para outro elemento da lista que ja tenha peso atualizado
                LE* proxIteracao = auxLista->buscaNaoVisitado();///criei a funcao justamente para casos especiais de grafo
                proxIteracao->setVisitado(true);
                id_rf = proxIteracao->getId();
            }
            else
            {
                id_rf = id_minimo;
            }
        }

        tabela << "Distancias" << endl;

        for(LE* elm = auxLista->getPrimeiro() ; elm != nullptr ; elm = elm->getProx())
        {
            tabela << id_origem << " -- " << elm->getId() << " -distancia: " << elm->getInfo() << endl;
        }
        tabela.close();

        LE* ult = auxLista->busca(id_destino);

//        ofstream output;
//        output.open("Caminho_dijkstra.dot");

        if(output.is_open())
        {
            output << " graph CaminhoDijkstra { " << endl;
            int idAnt = ult->getId();

            cout << " Caminho: " << "["<<idAnt<<"]";

            for(LE* it = ult->getAnt() ; it != nullptr ; it = it->getElementoPai())
            {
                int idAtual = it->getId();
                output << idAnt << " -- " << idAtual << endl;

                cout << " " << "["<<idAtual<<"]";

                idAnt = idAtual;

                //cout << "[ID Atual]" << idAtual << endl;

                if(idAtual == id_origem) break;
            }

            cout << " [distancia total] " << ult->getInfo() << endl;
        }
        output << endl << "}" << endl;
        //output.close();

        return ult->getInfo();
    }
    return 0;
}
float Grafo::floyd(int id_origem, int id_destino , ofstream& output)
{
    vector<int> id;
    ///estou criando essas estruturas para guardar id para o caso de ids irregulares
    vector<vector<float> > matriz;
    for(Vertice* it = primeiro_LV ; it !=nullptr ; it = it->getProx_LV())
    {
        id.push_back(it->getId());
    }
    for(int i = 0 ; i < ordem ; i++)
    {
        vector<float> linha ;
        Vertice* b = this->buscaVertice(id[i]);
        if(b != nullptr)
        {
            for(int j = 0 ; j < ordem ; j++)
            {
                if(i != j)
                {
                    Aresta*a = b->buscaAresta_LA(id[j]);
                    if(a != nullptr) linha.push_back(a->getPeso());
                    else linha.push_back(INFINITO);
                }
                else linha.push_back(0);
            }
        }
        else
        {
            cout << "ERRO" << endl;
            exit(0);
        }
        matriz.push_back(linha);
    }

    for(int k =  0; k < ordem ; k++)
    {
        for(int i = 0 ; i < ordem ; i++)
        {
            for(int j = 0 ; j < ordem ; j++)
            {
                if(i != k && j != k && i!=j)
                {
                    if(matriz[i][j] > matriz[k][j] + matriz[i][k] )
                    {
                        matriz[i][j] = matriz[k][j] + matriz[i][k];
                    }
                }
            }
        }
    }
    int x, y;
    for(int i = 0 ; i < ordem ; i++)
    {
        if(id_origem == id[i]) x = i;
        if(id_destino == id[i]) y = i;
    }
    return matriz[x][y];
}
Grafo* Grafo::SGVI_feixoTransitivoDireto(int id_origem , ofstream& output)
{
    string conector;
    //output.open("SGVIFTD.dot");
    if(output.is_open())
    {
        Grafo* g = new Grafo(0,1,0,0);
        Vertice* inicio = buscaVertice(id_origem);
        if(inicio != nullptr)
        {
            if(this->direcionado())
            {
                ///inserindo no output um grafo direcionado
                output << "digraph SGVIFTD{" << endl;
                conector = " -> " ;
            }
            else
            {
                ///inserindo no output um grafo não direcionado
                output << "graph SGVIFTD{" << endl;
                conector = " -- " ;
            }

            cout << "comecando em " << id_origem << " ... " << endl;
            auxFeixoTransitivoDireto(inicio, g, output, conector);
        }
        output<< "}" << endl;
        //output.close();
        return g;
    }
    return nullptr;
}
void Grafo::auxFeixoTransitivoDireto(Vertice* vAtual, Grafo* g, ofstream& output, string conector)
{
    Aresta* busca = vAtual->getPrimeiro_LA();
    int idAtual = vAtual->getId();
    g->insereVertice(vAtual->getId(), vAtual->getPeso());

    while(busca != nullptr)
    {
        Vertice* destino = busca->getDestino();
        if(destino != nullptr)
        {
            int idDestino = destino->getId();
            ///inserção bem sucedida
            if(g->insereVertice(idDestino, destino->getPeso()))
            {
                if(g->insereAresta(idAtual, idDestino, busca->getPeso()))
                {
                    ///inserindo par de vertice no arquivo de saida
                    output << idAtual << conector << idDestino << endl;
                }
                ///descendo mais um nivel
                cout << "descendo para " << idDestino <<" ... "<< endl;
                auxFeixoTransitivoDireto(destino, g, output, conector);
                cout << "voltando para " << idAtual << " ... " << endl;

            }///vertice ja presente no grafo, fazendo a inserção da aresta de retorno
            else
            {
                if(g->insereAresta(idAtual, idDestino, busca->getPeso()))
                {
                    ///inserir aresta no arquivo de saida
                    output << idAtual << conector << idDestino << endl;
                }
            }
            busca = busca->getProx_LA();
        }
        else
        {
            cout << "ERRO ponteiro" << endl;
            exit(0);
        }
    }
}
Grafo* Grafo::SGVI_feixoTransitivoIndireto(int id_busca , ofstream& output)
{
    Lista* validado = new Lista();
    Lista* visitado = new Lista();
    string conector;
    //output.open("SGVIFTI.dot");
    if(output.is_open())
    {
        Grafo* g = new Grafo(0,1,0,0);
        if(this->direcionado())
        {
            ///inserindo no output um grafo direcionado
            output << "digraph SGVIFTI{" << endl;
            conector = " -> " ;
        }
        else
        {
            ///inserindo no output um grafo não direcionado
            output << "graph SGVI-FTD{" << endl;
            conector = " -- " ;
        }
        output << id_busca << " [color = red]" << endl;
        for(Vertice* it = primeiro_LV ; it != nullptr ; it = it->getProx_LV())
        {
            ///iniciando a recursao a partir de cada vertice do grafo nao validado
            if(!validado->busca(it->getId()))
            {
                visitado->limpa();
                auxFeixoTransitivoIndireto( it, g,  output, conector, validado, visitado,  id_busca );
            }
        }
        output<< "}" << endl;
        //output.close();
        delete visitado;
        delete validado;
        return g;
    }
    delete visitado;
    delete validado;
    return nullptr;
}
bool Grafo::auxFeixoTransitivoIndireto(Vertice* vAtual, Grafo* g, ofstream& output, string conector, Lista* validado, Lista* visitado, int id_busca )
{
    Aresta* busca = vAtual->getPrimeiro_LA();
    int idAtual = vAtual->getId();
    bool verticeValido = false; ///vertice iniciado como nao valido

    cout << "descendo para " << idAtual << " ... " << endl;

    ///evitar recursao presa num ciclo
    if(visitado->busca(idAtual) != nullptr)
    {
        if(validado->busca(idAtual) != nullptr || idAtual == id_busca)
        {
            g->insereVertice(idAtual, vAtual->getPeso());
            return true;
        }
        else
        {
            return false;
        }
    }
    ///marcando como visitado
    visitado->insere(idAtual, vAtual->getPeso(), vAtual, true);

    if(idAtual == id_busca || validado->busca(idAtual))
    {
        ///retorno da recursao
        g->insereVertice(idAtual, vAtual->getPeso());
        if(validado->busca(idAtual)== nullptr)
        {
            validado->insere(idAtual, vAtual->getPeso(), vAtual, true);
        }
        return true;
    }
    else
    {
        ///continua recursao
        while(busca != nullptr)
        {
            Vertice* destino = busca->getDestino();
            if(destino != nullptr) ///evitar nullpointer
            {
                int idDestino = destino->getId();
                ///se a recursao retornar true foi encontrado um vertice valido
                if(auxFeixoTransitivoIndireto(destino, g, output, conector, validado, visitado, id_busca))
                {
                    ///achei algum vertice valido abaixo desse atual, oq torna este vertice valido
                    verticeValido = true;
                    g->insereVertice(idAtual, vAtual->getPeso());

                    if(validado->busca(idAtual)== nullptr)///evitar insersao dupla
                    {
                        validado->insere(idAtual, vAtual->getPeso(), vAtual, true);
                    }

                    if(g->insereAresta(idAtual, idDestino, busca->getPeso()))///evitar insersao dupla
                    {
                        output<< idAtual << conector << idDestino << endl;
                    }
                }
                busca = busca->getProx_LA();
            }
            else
            {
                cout << "ERRO pointer" << endl;
                exit(0);
            }
        }
    }
    ///acabou a lista e a recursão e não foi encotrado vertice
    return verticeValido;
}
Grafo* Grafo::buscaEmLargura(int id_origem , ofstream& output)
{
    map<int,bool> visitado;
    for(Vertice* p = primeiro_LV ; p != nullptr ; p = p->getProx_LV())
    {
        int id = p->getId();
        visitado.insert(pair<int,bool>(id, false));
    }
    string conector;
    //output.open("BuscaEmLargura.dot");
    if(output.is_open())
    {
        if(this->direcionado())
        {
            output << "digraph BEL { " << endl;
            conector = " -> ";
        }
        else
        {
            output << "graph BEL { " << endl;
            conector = " -- ";
        }

        Lista* lst = new Lista;
        Vertice* rf = buscaVertice(id_origem);
        if(rf == nullptr) return nullptr;
        ///
        lst->insere(id_origem, 1, rf, true );
        Grafo* arvoreLargura = new Grafo(0,0,0,0);
        Grafo* arestasRetorno = new Grafo(0,0,0,0);
        while(!lst->vazia())
        {
            Vertice* atual = lst->getPrimeiro()->getPointer();
            int idAtual = atual->getId();
            arvoreLargura->insereVertice(idAtual, 1);
            //cout << "[ATUAL] " << idAtual << endl;
            for(Aresta* a = atual->getPrimeiro_LA() ; a != nullptr ; a = a->getProx_LA())
            {
                int idDestino = a->getDestino()->getId();
                //cout << "[VARRENDO] " <<idDestino << endl;
                if(visitado[idDestino] == false && lst->busca(idDestino) == nullptr)
                {
                    //cout << "[Nao VISITADO]" << idDestino << endl;
                    lst->insere(idDestino, 1, a->getDestino(), false);
                }

                if(arvoreLargura->insereVertice(idDestino, 1))
                {
                    ///caminho em largura
                    arvoreLargura->insereAresta(idAtual, idDestino, 1);
                    output << idAtual << conector << idDestino << endl;
                    //cout << "[Caminho] " << idAtual << "-" << idDestino << endl;
                    //arestasRetorno->TESTE_imprimeGrafo();
                }
                else
                {
                    Vertice* temp = arvoreLargura->buscaVertice(idAtual);
                    if(temp->buscaAresta_LA(idDestino) == nullptr)
                    {
                        arestasRetorno->insereVertice(idAtual, 1);
                        arestasRetorno->insereVertice(idDestino, 1);
                        if(arestasRetorno->insereAresta(idAtual, idDestino, 1))
                        {
                            //output << idAtual << "conector << idDestino << " [color = red]" << endl;
                        }
                    }
                }
            }
            visitado[idAtual] = true;
            //cout << "[REMOVENDO DA LISTA] " << atual->getId() << endl;
            lst->remove(idAtual);
            //arestasRetorno->TESTE_imprimeGrafo();

        }
        output << "} " << endl;
        //output.close();
        delete arestasRetorno;
        return arvoreLargura;
    }
    return nullptr;
}
Grafo* Grafo::buscaEmProfundidade(int id_origem , ofstream& output)
{
    string conector;
    //output.open("BuscaEmProfundidade.dot", ios::out);
    if(output.is_open())
    {
        Vertice* vb = this->buscaVertice(id_origem);
        if(vb != nullptr)
        {
            if(this->direcionado())
            {
                output << "digraph BEL { " << endl;
                conector = " -> ";
            }
            else
            {
                output << "graph BEL { " << endl;
                conector = " -- ";
            }

            ///lista para controle de vertices ja visitados
            Lista* al = new Lista();
            for(Vertice* it = primeiro_LV ; it != nullptr ; it = it->getProx_LV())
            {
                al->insere(it->getId(), 1, it, false );
            }
            ///grafos como estrutura auxiliar
            Grafo* arestasRetorno = new Grafo(0,0,0,0);
            Grafo* arvore = new Grafo(0, 0, 0, 0);
            ///vertice de inicio
            arvore->insereVertice(id_origem, 1);
            al->remove(id_origem);
            profundidade(vb, arvore, arestasRetorno, al, output, conector);

            while(!al->vazia())
            {
                ///existem vertices no grafo original que não foram adicionadas a arvore de saida
                Vertice* npt  = al->getPrimeiro()->getPointer();
                profundidade(npt, arvore, arestasRetorno,  al, output, conector);
            }
            //cout << "Arvore Resultante da busca em profundidade" << endl;
            //arvore->TESTE_imprimeGrafo();
            //cout << "Arestas de Retorno" << endl;
            //arestasRetorno->TESTE_imprimeGrafo();

            output << "} " << endl;
            //output.close();
            delete arestasRetorno;
            return arvore;
        }
    }
    //output.close();
    return nullptr;
}
void Grafo::profundidade(Vertice* v, Grafo* arvore,Grafo* ret, Lista* al, ofstream& output, string conector)
{
    //cout << "[VERTICE] " << v->getId() << endl;
    Aresta* busca = v->getPrimeiro_LA();
    int idAtual = v->getId();
    while(busca != nullptr)
    {
        ///alterei o insere para retornar true quando bem sucedido
        ///isso fara com que toda insercao de um vertice ja existente faça com que a recursao passe para a proxima da lista
        int idDestino = busca->getDestino()->getId();
        float novoPeso = busca->getDestino()->getPeso();
        if(arvore->insereVertice(idDestino, novoPeso))
        {
            //cout << "[ESTOU] " << v->getId() << endl;
            output << idAtual << conector << idDestino << endl;
            al->remove(idDestino);
            arvore->insereAresta(idAtual, busca->getDestino()->getId(), busca->getPeso());
            //cout << "[DESCENDO]" << endl;
            profundidade(busca->getDestino(), arvore,ret, al, output, conector);
        }
        else
        {
            /// se esta presente nas listas de adjacencia mas ainda nao possui uma aresta de conexao
            /// essa aresta deve ser de retorno
            Vertice* temp = arvore->buscaVertice(idAtual);
            if(temp->buscaAresta_LA(idDestino) == nullptr)
            {
                ret->insereVertice(idAtual, 1);
                ret->insereVertice(idDestino, 1);
                if(ret->insereAresta(idAtual, idDestino, 1))
                {
                    output << idAtual << conector << idDestino << " [color = red]" << endl;
                }
            }
        }
        busca = busca->getProx_LA();
    }
    //cout << "voltando de " << v->getId()<<"..."<< endl;
}
void Grafo::ordenacaoTopologica(ofstream& output)
{
    ofstream ord;
    ord.open("Ordenacao.txt");

    if(ord.is_open())
    {
        ///ids e graus de entrada
        Lista *verticesGraus = new Lista();
        vector<int> ordenacao;

        Vertice* verticeIt;
        int idIt = -1;

        ///preenchendo lista
        for(Vertice* it = primeiro_LV ; it != nullptr ; it = it->getProx_LV())
        {
            verticesGraus->insere(it->getId(), it->getGrauEntrada(), it, false);
        }

        while(!verticesGraus->vazia())
        {
            bool encontrado = false;
            ///busca grau 0
            for(LE* elm = verticesGraus->getPrimeiro() ; elm != nullptr ; elm = elm->getProx())
            {
                if(elm->getInfo() == 0)
                {
                    encontrado = true;
                    idIt = elm->getId();
                    verticeIt = elm->getPointer();
                    break;
                }
            }
            ///testando dependencia circular
            if(encontrado)
            {
                ///descobre os adjacentes e decrementa o grau de entrada
                for(Aresta* a = verticeIt->getPrimeiro_LA() ; a != nullptr ; a = a->getProx_LA())
                {
                    int idTemp = a->getDestino()->getId();
                    LE* noTemp = verticesGraus->busca(idTemp);

                    if(noTemp != nullptr)
                    {
                        float temp = noTemp->getInfo();
                        temp--;
                        noTemp->setInfo(temp);
                    }
                }
                ///remove o vertice a lista e insere no vetor
                verticesGraus->remove(idIt);
                ordenacao.push_back(idIt);

                ord << idIt << " ";
            }
            else
            {
                cout << "Foi encontrado uma dependencia circular" << endl;
                return;
            }
        }

        ord.close();
    }
    return;
}
Grafo* Grafo::agmPrim(ofstream& output)
{
    //output.open("agmPrim.dot");

    if(output.is_open())
    {
        Grafo* agm = new Grafo(0, 0, 1, 0);
        Lista* listaAux = new Lista;
        Vertice* raiz = primeiro_LV;
        agm->insereVertice(raiz->getId(), 1);
        listaAux->insere(raiz->getId(), 1, raiz, false);


        output<< "graph agmPrim { " << endl;
        ///verificando numero de vertices
        while(agm->getOrdem() < this->getOrdem())
        {
            int idMinimo;
            int idPai;
            Vertice* vMinimo;
            bool encontrado = false;
            float minimo = INFINITO;

            ///percorrendo todos os vertices ja adicionados e pegando a menor aresta não validada
            for(LE* elmLista = listaAux->getPrimeiro() ; elmLista != nullptr ; elmLista = elmLista ->getProx())
            {
                Vertice* v = elmLista->getPointer();
                //cout << "[VERTICE ORIGEM] " << v->getId() << endl;
                ///passando pelas arestas de cada vertice
                for(Aresta *a = v->getPrimeiro_LA() ; a != nullptr ; a = a->getProx_LA())
                {
                    ///pegando os destinos e verificando se são validados
                    int idTemp = a->getDestino()->getId();
                    Vertice* busca = agm->buscaVertice(idTemp);
                    //cout << "[VERTICE DESINO] " << idTemp << " [PESO] " << a->getPeso() << endl;

                    if(busca == nullptr)
                    {
                        encontrado = true;
                        ///comparando minimos
                        if(a->getPeso() < minimo)
                        {
                            minimo = a->getPeso();
                            idMinimo = a->getDestino()->getId();
                            vMinimo = a->getDestino();
                            idPai = v->getId();

                        }
                    }
                }
            }
            //cout << "[FIM DA BUSCA]" << endl;
            //cout<< "[ARESTA MINIMA] " << idPai << " -- " << idMinimo << "[peso] " << minimo << endl;
            ///testando uma excessao de minimo nao encontrado, que deveria ser impossivel, mas so para teste
            if(encontrado)
            {
                ///inserindo aresta
                agm->insereVertice(idMinimo, 1);
                if(agm->insereAresta(idPai, idMinimo, minimo))
                {
                    ///inserindo no output
                    output<< idPai << " -- " << idMinimo  << "[ label = \"" << minimo << "\" ]" << endl;
                }

                ///validando na lista
                listaAux->insere(idMinimo, 1, vMinimo, false);

            }
            else
            {
                cout << "ERRO: Minimo não encontrado" << endl;
                exit(1);
            }

            //cout << "[VERTICES ORIGINAIS] " << this->getOrdem() << " [VERTICES AGM] " << agm->getOrdem()<< endl;
        }

        //cout<< "[ACABOU}" << endl;
        output << endl << " }" << endl;
        //output.close();
        return agm;
    }
    return nullptr;
}
Grafo* Grafo::agmKruskal(ofstream& output)
{
    //output.open("agmKruskal.dot");

    if(output.is_open())
    {
        output << "graph agmKruskal { " << endl;
        Grafo* agm = new Grafo(0, 0, 1, 0);
        map<int, int> idEtiqueta;

        int i = 1;
        int tamanho = 0;
        ///inicializando estrutura auxiliar
        for(Vertice* v = primeiro_LV ; v != nullptr ; v = v->getProx_LV())
        {
            ///nesse caso o campo info representa o label da arvore
            idEtiqueta.insert(pair<int,int> (v->getId(), i));
            tamanho++;
            i++;
        }

        bool acabou = false;
        while(!acabou)
        {
            float minimo = INFINITO;
            int idOMin;
            int idDMin;

            for(Vertice* vt = primeiro_LV ; vt != nullptr ; vt = vt->getProx_LV())
            {
                int idOrigem = vt->getId();

                for(Aresta* a = vt->getPrimeiro_LA() ; a != nullptr ; a = a->getProx_LA())
                {
                    int idDestino = a->getDestino()->getId();
                    ///se labels diferentes
                    //cout << "[VERTICE ORIGEM] " << idOrigem << "[LABEL] " << idEtiqueta[idOrigem] << endl;
                    //cout << "[VERTICE DESTINO] "<< idDestino << "[LABEL] " << idEtiqueta[idDestino] << endl;

                    if(idEtiqueta[idOrigem] != idEtiqueta[idDestino])
                    {
                        //cout << "[VERTICE ORIGEM] " << idOrigem << "[LABEL] " << idEtiqueta[idOrigem] << endl;
                        //cout << "[VERTICE DESTINO] "<< idDestino << "[LABEL] " << idEtiqueta[idDestino] << endl;

                        ///testando o minimo entre todas as arestas em arvores diferentes
                        if(a->getPeso() < minimo)
                        {
                            minimo = a->getPeso();
                            idOMin = idOrigem;
                            idDMin = idDestino;
                        }
                    }
                }
                //cout << "[fim de arestas]" << endl;
            }
            //cout << "[fim dos vertices]" << endl;

            ///unindo as arvores
            agm->insereVertice(idOMin, 1);
            agm->insereVertice(idDMin, 1);
            if(agm->insereAresta(idOMin, idDMin, minimo))
            {
                ///insere no output
                output << idOMin << " -- " << idDMin << "\t [ label = \"" << minimo << "\" ]" << endl;
            }
            ///tirando o label minimo
            int labelMin = idEtiqueta[idOMin] < idEtiqueta[idDMin] ? idEtiqueta[idOMin] : idEtiqueta[idDMin];
            idEtiqueta[idOMin] = labelMin;
            idEtiqueta[idDMin] = labelMin;

            ///verificando se acabou
            acabou = true;
            //cout << "tamanho " << tamanho << endl;
            int anterior = -1;
            for(auto i : idEtiqueta)
            {
                //cout << "[" << anterior << "]";
                //cout << "{" << i.second << "}";
                if(anterior == -1)
                {
                    anterior = i.second;
                }

                if(anterior != i.second)
                {
                    acabou = false;
                    break;
                }
                else
                {
                    anterior = i.second;
                }
                //cout << "[END]" << endl;
            }

            //cout << endl <<  "[ORDEM AGM] " << agm->getOrdem() << endl;
        }

        output << endl << "}" << endl;
        //output.close();
        return agm;
    }
    return nullptr;
}
void Grafo::TESTE_imprimeGrafo()
{

    Vertice* v = primeiro_LV;
    Aresta* a;

    //system("CLS");

    for(; v != nullptr ; v = v->getProx_LV())
    {
        a = v->getPrimeiro_LA();
        cout << "| " << v->getId() << " |";
        for(; a !=nullptr ; a = a->getProx_LA())
        {
            cout << "->" << a->getDestino()->getId() << "(" << a->getPeso() << ")";
        }
        cout << endl;
    }
}
