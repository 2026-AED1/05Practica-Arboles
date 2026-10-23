// inorden.cpp - Codigo base para DomJudge (Arboles de expresiones)
// Completa infija(). Puedes anadir metodos auxiliares.
// No cambies la lectura ni la escritura, y no imprimas nada mas.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Nodo {
    string valor;
    Nodo* izquierdo = nullptr;
    Nodo* derecho = nullptr;

    bool es_hoja() const {
        return izquierdo == nullptr && derecho == nullptr;
    }
};

class ArbolExpresion {
public:
    Nodo* raiz = nullptr;

    string infija() const {
        // TODO: inorden.  (3 + 4) * 2  ->  "((3 + 4) * 2)"
        return "";
    }
};

int main() {
    int casos;
    cin >> casos;
    for (int c = 0; c < casos; c++) {
        int n;
        cin >> n;
        vector<Nodo> nodos(n);
        for (int i = 0; i < n; i++) {
            int izq, der;
            cin >> nodos[i].valor >> izq >> der;
            if (izq != -1) nodos[i].izquierdo = &nodos[izq];
            if (der != -1) nodos[i].derecho = &nodos[der];
        }
        ArbolExpresion arbol;
        arbol.raiz = &nodos[0];
        cout << arbol.infija() << "\n";
    }
    return 0;
}
