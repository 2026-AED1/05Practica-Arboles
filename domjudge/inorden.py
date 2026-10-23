# inorden.py - Codigo base para DomJudge (Arboles de expresiones)
# Completa infija(). Puedes anadir metodos auxiliares.
# No cambies la lectura ni la escritura, y no imprimas nada mas.
import sys

sys.setrecursionlimit(10000)


class Nodo:
    def __init__(self, valor, izquierdo=None, derecho=None):
        self.valor = valor
        self.izquierdo = izquierdo
        self.derecho = derecho

    def es_hoja(self):
        return self.izquierdo is None and self.derecho is None


class ArbolExpresion:
    def __init__(self):
        self.raiz = None

    def infija(self):
        # TODO: inorden.  (3 + 4) * 2  ->  "((3 + 4) * 2)"
        return ""


def leer_arbol():
    n = int(input())
    nodos = []
    hijos = []
    for i in range(n):
        valor, izq, der = input().split()
        nodos.append(Nodo(valor))
        hijos.append((int(izq), int(der)))
    for i in range(n):
        izq, der = hijos[i]
        if izq != -1:
            nodos[i].izquierdo = nodos[izq]
        if der != -1:
            nodos[i].derecho = nodos[der]
    arbol = ArbolExpresion()
    arbol.raiz = nodos[0]
    return arbol


casos = int(input())
for c in range(casos):
    arbol = leer_arbol()
    print(arbol.infija())
