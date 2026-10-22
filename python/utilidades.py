# utilidades.py
# Codigo de apoyo para la practica. No hay que modificarlo (ni hace falta
# leerlo para hacer la practica).
#
#   dibujar(arbol)                    -> str con el arbol dibujado en ASCII
#   infija_a_postfija("(3 + 4) * 2")  -> "3 4 + 2 *"
#
# Uso tipico, para ver tu arbol mientras programas:
#   from utilidades import dibujar, infija_a_postfija
#   a = ArbolExpresion(infija_a_postfija("(3 + 4) * 2"))
#   print(dibujar(a))


def dibujar(arbol):
    """Dibuja el arbol de arriba abajo. Para (3 + 4) * 2:

          _*
         /  \\
         +  2
        / \\
        3 4
    """
    if arbol.raiz is None:
        return "(arbol vacio)"
    lineas, _, _, _ = _dibujar(arbol.raiz)
    return "\n".join(linea.rstrip() for linea in lineas)


def _dibujar(nodo):
    # Devuelve (lineas, ancho, alto, columna de la raiz)
    etiqueta = str(nodo.valor)
    u = len(etiqueta)
    if nodo.izquierdo is None and nodo.derecho is None:
        return [etiqueta], u, 1, u // 2
    izq, n, p, x = _dibujar(nodo.izquierdo)
    der, m, q, y = _dibujar(nodo.derecho)
    primera = (x + 1) * " " + (n - x - 1) * "_" + etiqueta + y * "_" + (m - y) * " "
    segunda = x * " " + "/" + (n - x - 1 + u + y) * " " + "\\" + (m - y - 1) * " "
    if p < q:
        izq = izq + [n * " "] * (q - p)
    elif q < p:
        der = der + [m * " "] * (p - q)
    lineas = [primera, segunda] + [a + u * " " + b for a, b in zip(izq, der)]
    return lineas, n + m + u, max(p, q) + 2, n + u // 2


def infija_a_postfija(expresion):
    """Convierte una expresion infija con enteros, + - * / y parentesis en la
    cadena postfija equivalente (algoritmo shunting-yard de Dijkstra, con una
    pila de operadores)."""
    prioridad = {"+": 1, "-": 1, "*": 2, "/": 2}
    salida, pila = [], []
    i = 0
    while i < len(expresion):
        c = expresion[i]
        if c.isspace():
            i += 1
        elif c.isdigit():
            j = i
            while j < len(expresion) and expresion[j].isdigit():
                j += 1
            salida.append(expresion[i:j])
            i = j
        elif c in prioridad:
            # saca los operadores de prioridad mayor o igual (todos asocian
            # por la izquierda)
            while pila and pila[-1] != "(" and prioridad[pila[-1]] >= prioridad[c]:
                salida.append(pila.pop())
            pila.append(c)
            i += 1
        elif c == "(":
            pila.append(c)
            i += 1
        elif c == ")":
            while pila and pila[-1] != "(":
                salida.append(pila.pop())
            if not pila:
                raise ValueError("parentesis desequilibrados")
            pila.pop()
            i += 1
        else:
            raise ValueError(f"caracter no valido: '{c}'")
    while pila:
        if pila[-1] == "(":
            raise ValueError("parentesis desequilibrados")
        salida.append(pila.pop())
    return " ".join(salida)
