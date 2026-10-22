# Práctica: Árboles de Expresiones
**Asignatura**: Algoritmia y Estructuras de Datos (AED I) — Bloque II
**Titulación**: Ingeniería Informática – Inteligencia Artificial
**Duración**: 100 min en clase (+ lo que quede, en casa)
**Peso**: [Especificar] de la nota del bloque
**Lenguaje**: Python 3.10 o posterior, sin librerías externas (hay una versión opcional en C++17)

> Este es el **enunciado**: qué hay que hacer. La explicación paso a paso está en las diapositivas (`diapositivas/`) y el resumen para tener a mano, en `chuleta.md`.

## Contexto

En teoría hemos visto los **árboles binarios** y sus tres recorridos en profundidad: **preorden**, **inorden** y **postorden**. En esta práctica los vamos a usar para algo muy concreto: representar una **expresión aritmética**.

```
   (3 + 4) * 2              3 + 4 * 2

        *                       +
       / \                     / \
      +   2                   3   *
     / \                         / \
    3   4                       4   2

     = 14                        = 11
```

Las **hojas** son números y los **nodos internos** son operadores (`+ - * /`). En el árbol no hacen falta paréntesis: la forma del árbol ya dice qué se calcula antes (lo que está más abajo).

En esta práctica vas a:

1. **Construir** el árbol a partir de la expresión en notación **postfija** (`"3 4 + 2 *"`), con una pila de nodos.
2. **Evaluarlo**: `14`.
3. **Escribirlo** en las tres notaciones, que salen de los tres recorridos:

| Notación | `(3 + 4) * 2` | Recorrido |
|----------|---------------|-----------|
| Prefija | `* + 3 4 2` | Preorden: raíz, izquierdo, derecho |
| Infija | `((3 + 4) * 2)` | Inorden: izquierdo, raíz, derecho (+ paréntesis) |
| Postfija | `3 4 + 2 *` | Postorden: izquierdo, derecho, raíz |

4. Calcular sus **métricas**: altura y número de hojas.

Solo hay números **enteros no negativos** y los operadores `+ - * /`.

## Ficheros

Trabaja siempre desde la carpeta `python/` (`cd python`). En Windows, si `python3` no existe, usa `python` o `py`.

| Fichero | Qué es | ¿Lo modificas? |
|---------|--------|----------------|
| `python/arbol_expresiones.py` | `Nodo` y `ArbolExpresion`, con huecos (`TODO`) | **Sí** |
| `python/test_arbol_expresiones.py` | Pruebas de autocomprobación | No |
| `python/utilidades.py` | `dibujar(arbol)` y `infija_a_postfija("(3 + 4) * 2")`, ya hechos | No |
| `RESPUESTAS.md` | Hoja con la tabla de costes y tres preguntas | **Sí** |
| `chuleta.md` | Resumen de una página: recorridos, algoritmo de la pila, costes | No |
| `diapositivas/` | Diapositivas de la sesión en PDF | No |
| `cpp/` | Ampliación opcional: la misma práctica en C++ | Solo si la haces en C++ |

## Qué hay que hacer

### La representación

```python
class Nodo:                         # ya hecho
    valor        # str: un número ("3") o un operador ("+")
    izquierdo    # Nodo o None
    derecho      # Nodo o None

class ArbolExpresion:
    raiz         # Nodo o None (árbol vacío)

ArbolExpresion()               # árbol vacío
ArbolExpresion("3 4 + 2 *")    # el constructor llama a construir(...)
```

`num_nodos()` ya está hecho como **ejemplo del patrón recursivo** que vas a usar en el resto: un método público que llama a un método auxiliar recursivo que recibe el nodo.

Completa los métodos marcados con `TODO` **en este orden** y ejecuta el grupo de tests de cada paso:

| Orden | Métodos | Tests |
|-------|---------|-------|
| 1 | `construir(postfija)` | `python3 -m unittest -v test_arbol_expresiones.PruebasConstruccion` |
| 2 | `evaluar()` | `... test_arbol_expresiones.PruebasEvaluacion` |
| 3 | `prefija()`, `infija()`, `postfija()` | `... test_arbol_expresiones.PruebasNotaciones` |
| 4 | `altura()`, `num_hojas()` | `... test_arbol_expresiones.PruebasMetricas` |

Algunas indicaciones:

- **`construir`** devuelve la **raíz** del árbol (un `Nodo`). Si la expresión está mal formada (faltan o sobran operandos, hay algo que no es ni un número ni un operador, o está vacía), lanza `ValueError`.
- **`evaluar`** sobre un árbol vacío lanza `ValueError`. `/` es la división normal de Python (`7 / 2` → `3.5`); la división por cero ya lanza `ZeroDivisionError` por sí sola.
- En las tres notaciones los tokens van separados por **un espacio**. En la infija, cada operación va entre paréntesis y una hoja, sin ellos (`"7"`). Un árbol vacío da `""`.
- **Altura**: árbol vacío → −1, una hoja → 0 (el convenio de teoría).
- Para ver lo que construyes: `from utilidades import dibujar` y `print(dibujar(arbol))`. Para probar con expresiones normales: `ArbolExpresion(infija_a_postfija("(3 + 4) * 2"))`.
- Sustituye cada `O(?)` del código por el coste que consigue tu implementación.

---

## Cierre (en casa, ≈ 10 min)

Rellena `RESPUESTAS.md`: la tabla de costes de tu implementación (deben coincidir con los `O(?)` del código) y tres preguntas cortas.

Antes de entregar:

- [ ] `python3 -m unittest -v test_arbol_expresiones` pasa (las pruebas de la ampliación salen como *skipped* si no la haces).
- [ ] No queda ningún `O(?)` en el código.

## Evaluación

| Criterio | Dónde | Puntuación |
|----------|-------|------------|
| 1: `construir` | clase | 3.0 |
| 2: `evaluar` | clase | 2.0 |
| 3: las tres notaciones | clase | 2.0 |
| 4: `altura` y `num_hojas` | clase / casa | 2.0 |
| `RESPUESTAS.md` | casa | 0.5 |
| Test individual de cierre de sesión | clase | 0.5 |
| **Total** | | **10.0** |

**Entrega**: un `.zip` con `arbol_expresiones.py` y `RESPUESTAS.md`.

---

## Ampliación (opcional, no puntúa)

- **Otras operaciones** en `arbol_expresiones.py` (sus tests, `PruebasAmpliacion`, se saltan mientras no las hagas):
  - `infija_minima()`: solo los paréntesis necesarios. `(3 + 4) * 2`, `3 + 4 * 2`, `8 - (4 - 1)`.
  - `por_niveles()`: recorrido en anchura, nivel a nivel, con una cola (`collections.deque`). `(3 + 4) * 2` → `"* + 2 3 4"`.
  - `construir_prefija(prefija)`: construir desde la notación prefija (`"* + 3 4 2"`), **sin pila**, con recursión.
- **La misma práctica en C++** (carpeta `cpp/`), ver abajo.

### Versión en C++

Mismos ejercicios y mismos nombres en `cpp/arbol_expresiones.hpp`, con `TODO`. Compilar y ejecutar las pruebas (desde `cpp/`):

```bash
g++ -std=c++17 -Wall test_arbol_expresiones.cpp -o test_arbol && ./test_arbol     # o ./test_arbol PruebasConstruccion
```

En Windows: `.\test_arbol.exe`. Cada prueba sale como `ok`, `FALLO`, `PENDIENTE` (aún lanza `TODO`) o `saltada` (ampliación). Diferencias con Python:

- `None` es `nullptr`; `ValueError` es `std::invalid_argument` y la división por cero lanza `std::domain_error`.
- El destructor ya está hecho: libera el árbol con un recorrido **postorden** (los hijos antes que el padre).
- **Solo en C++**: si `construir` lanza una excepción a medias, tiene que liberar los nodos que ya había creado. `PruebasMemoria` lo comprueba con el contador `Nodo::vivos`.

Entrega en C++: `arbol_expresiones.hpp` y `RESPUESTAS.md`.

---

## Recursos

| Recurso | Para qué |
|---------|----------|
| [VisuAlgo — Binary Search Tree](https://visualgo.net/en/bst) | Animaciones de recorridos en árboles binarios |
| [Python — módulo `ast`](https://docs.python.org/3/library/ast.html) | Python también convierte tu código en un árbol: `ast.dump(ast.parse("(3 + 4) * 2", mode="eval"))` |
| [Notación polaca inversa (Wikipedia)](https://es.wikipedia.org/wiki/Notaci%C3%B3n_polaca_inversa) | Historia y uso de la postfija |

**Siguiente práctica**: Árboles equilibrados y Tries.
