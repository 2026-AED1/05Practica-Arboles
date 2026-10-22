# Chuleta — Árboles de expresiones

```
   (3 + 4) * 2          hojas          = números      "3"  "4"  "2"
                        nodos internos = operadores   "+"  "*"
        *
       / \              Nodo:          valor | izquierdo | derecho
      +   2             ArbolExpresion: raiz        (None = árbol vacío)
     / \
    3   4
```

## Tres notaciones = tres recorridos

| Notación | `(3 + 4) * 2` | Recorrido | Orden |
|----------|---------------|-----------|-------|
| Prefija | `* + 3 4 2` | Preorden | raíz → izq → der |
| Infija | `((3 + 4) * 2)` | Inorden | izq → raíz → der (+ paréntesis) |
| Postfija | `3 4 + 2 *` | Postorden | izq → der → raíz |

## Construir desde postfija: pila de NODOS

```
token       acción                          pila (fondo → cima)
  3         apilar hoja                     [3]
  4         apilar hoja                     [3, 4]
  +         der = pop, izq = pop → nodo +   [(3+4)]
  2         apilar hoja                     [(3+4), 2]
  *         der = pop, izq = pop → nodo *   [((3+4)*2)]
fin         debe quedar exactamente 1 nodo  → es la raíz
```

- El **primero** que sale es el hijo **derecho**: `8 2 -` es 8 − 2.
- Errores (`ValueError`): faltan operandos (pila con < 2 al llegar a un operador), sobran (más de 1 al final), token raro, cadena vacía.

## El patrón recursivo (como `num_nodos`)

```python
def num_nodos(self):                    # público: arranca en la raíz
    return self._num_nodos(self.raiz)

def _num_nodos(self, nodo):             # auxiliar: recibe el nodo
    if nodo is None:                    # caso base: árbol vacío
        return 0
    i = self._num_nodos(nodo.izquierdo)
    d = self._num_nodos(nodo.derecho)
    return 1 + i + d
```

**Nunca** `def altura(self, nodo=None): if nodo is None: nodo = self.raiz` → con un hijo vacío vuelve a la raíz: recursión infinita.

## Convenios y costes

- Altura: árbol vacío = −1, una hoja = 0, `altura(n) = 1 + max(altura(izq), altura(der))`.
- Árbol de expresión = árbol binario **lleno** (0 o 2 hijos) → hojas = operadores + 1.
- Todas las operaciones de la práctica recorren cada nodo una vez: **O(n)** en tiempo. Memoria extra: la pila de `construir`, O(n); la recursión, O(h) (O(n) si el árbol es degenerado).
