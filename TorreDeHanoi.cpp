#include <iostream>
#include <vector>

using namespace std;

const int totalHaste = 3;
int totalDisco;

class Estado{
    public:
        vector<int> disco;
        vector<Estado*> adjacente;
        bool marcado = false;
        int heuristica, custo = 0;

    Estado(vector<int> disco){
        int heuristica = 0;
        this->disco = disco;
        for(int i = 0; i < totalDisco; i++){
            heuristica += (i+1) * ((3-disco[i])*2);
        }
        this->heuristica = heuristica;
    }
};

void showLista(vector<int> lista){
    cout << "( ";
    for(int x: lista){
        cout << x << " ";
    }
    cout << ")";
}

bool verificaSeMenor(int *P, int i, int j){
    for(int k = 0; k < i; k++){
        if(P[k] == j)
            return true;
    }
    return false;
}

Estado* estaMarcado(vector<int> *v, vector<Estado*> *visitados){
    for(Estado *w: *visitados){
        if(w->disco == *v)
            return w;
    }
    return NULL;
}

void criaAdjacencia(Estado *Raiz, vector<Estado*> *criados){
    if(Raiz->adjacente.size() == 0){
        vector<int> vetor;
        Estado *existente;
        cout << "RAIZ:" << Raiz->heuristica << " ( ";
        for(int x: Raiz->disco){
            cout << x << " ";
        }
        cout << ")\n";
        for(int i = 0; i < totalDisco; i++){
            if(!verificaSeMenor(Raiz->disco.data(), i, Raiz->disco[i])){
                for(int j = 1; j <= totalHaste; j++){
                    if(Raiz->disco[i] != j){
                        if(!verificaSeMenor(Raiz->disco.data(), i, j)){
                            vetor = Raiz->disco;
                            vetor[i] = j;
                            existente = estaMarcado(&vetor, criados);
                            if(existente == NULL){
                                Estado *a = new Estado(vetor);
                                criados->push_back(a);
                                Raiz->adjacente.push_back(a);
                                cout << "   CRIADO: ( ";
                                for(int x: a->disco){
                                    cout << x << " ";
                                }
                                cout << ")\n";
                            }else{
                                Raiz->adjacente.push_back(existente);
                                cout << "   ADICIONADO: ( ";
                                for(int x: existente->disco){
                                    cout << x << " ";
                                }
                                cout << ")\n";
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }
}

void mostrarEstados(Estado *Raiz, vector<Estado*> *visitados){
    if(!estaMarcado(&Raiz->disco, visitados)){
        visitados->push_back(Raiz);
        cout << "( ";
        for(int x: Raiz->disco){
            cout << x << " ";
        }
        cout << ")\n";
        for(Estado *P: Raiz->adjacente){
            mostrarEstados(P, visitados);
        }
    }
}

Estado* buscaEmProfundidade(Estado *v, vector<int> *estadoFinal, vector<Estado*> *criados, vector<Estado*> *visitados, vector<Estado*> *caminho, int *cont){
    Estado *adjacente;
    visitados->push_back(v);
    caminho->push_back(v);
    *cont = *cont+1;
    if(v->disco == *estadoFinal){
        caminho->pop_back();
        return v;
    }
    criaAdjacencia(v, criados);
    for(Estado *w: v->adjacente){
        adjacente = estaMarcado(&w->disco, visitados);
        if(adjacente == NULL){
            Estado *a = buscaEmProfundidade(w, estadoFinal, criados, visitados, caminho, cont);
            if(a != NULL)
                return a;
        }
    }
    caminho->pop_back();
    return NULL;
}

Estado* buscaEmLargura(Estado *Raiz, vector<int> *estadoFinal, vector<Estado*> *criados, int *cont){
    vector<Estado*> F;
    Estado *v;
    F.push_back(Raiz);
    criados->push_back(Raiz);
    while(F.size() > 0){
        v = F.front();
        criaAdjacencia(v, criados);
        for(Estado *w: v->adjacente){
            if(!w->marcado){
                w->marcado = true;
                *cont = *cont+1;
                if(w->disco == *estadoFinal){
                    return w;
                }
                F.push_back(w);
            }
        }
        F.erase(F.begin());
    }
    return NULL;
}

int main(){
    int busca, cont = 0;
    vector<int> inicio, fim;
    vector<Estado*> criados, visitados, caminho;
    Estado *inicial, *final;

    cout << "Escolha o numero de discos: " << endl;
    cin >> totalDisco;

    for(int i = 0; i < totalDisco; i++){
        inicio.push_back(1);
        fim.push_back(3);
    }

    inicial = new Estado(inicio);

    cout << "Tipo de Busca:\nEm Largura(1), Em Profundidade(2)\n";
    cin >> busca;
    switch(busca){
        case 1:
            final = buscaEmLargura(inicial, &fim, &criados, &cont);
            showLista(final->disco);
            cout << "\nEstados Visitados:" << cont << endl;
        break;
        case 2:
            final = buscaEmProfundidade(inicial, &fim, &criados, &visitados,&caminho,&cont);
            for(Estado *e: caminho){
                showLista(e->disco);
                cout << " -> ";
            }
            showLista(final->disco);
            cout << "\nEstados Visitados:" << cont << endl;
        break;
        default:
            cout << "Opcao invalida\n";
    }
}