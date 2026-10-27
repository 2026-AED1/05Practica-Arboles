# Práctica Árboles de Expresiones · Hoja de respuestas

**Nombre(s):**

## 1 · Costes de tu implementación

Deben coincidir con los `O(?)` que has sustituido en el código. *n* es el número de nodos y *h* la altura del árbol.

| Operación | Tiempo | Memoria extra |
|-----------|--------|---------------|
| `construir(postfija)` | | |
| `evaluar()` | | |
| `prefija()`, `infija()`, `postfija()` | | |
| `num_nodos()` (ya hecho) | O(n) | |
| `altura()` | | |
| `num_hojas()` | | |
| `num_internos()` | | |

## 2 · Preguntas

**1.** ¿Qué pasaría si en `construir` usaras el primer nodo que sale de la pila como hijo **izquierdo**? Pon un ejemplo en el que el resultado de `evaluar` cambie y otro en el que no cambie.

>

**2.** `(3 + 4) * 2` y `3 + (4 * 2)` se escriben igual si quitamos todos los paréntesis: `3 + 4 * 2`. ¿Se puede reconstruir el árbol a partir de la infija sin paréntesis? ¿Y a partir de la postfija? ¿Por qué?

>

**3.** ¿Qué altura tiene el árbol de `1 2 + 3 + 4 + ... 1000 +` (los números del 1 al 1000 sumados de izquierda a derecha)? ¿Qué le pasa a tu `evaluar` recursivo en Python con un árbol así, y por qué?

>
