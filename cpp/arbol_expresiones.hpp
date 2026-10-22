// arbol_expresiones.hpp
// Practica - Arboles de expresiones (version C++, opcional).
//
// Mismos ejercicios y mismos nombres que python/arbol_expresiones.py.
// Expresiones: numeros enteros no negativos y los operadores + - * /
//   ValueError         ->  std::invalid_argument
//   ZeroDivisionError  ->  std::domain_error
//   None               ->  nullptr
//
// Hay que completar los metodos marcados con TODO (cada uno lanza ahora
// std::logic_error("TODO ...")) y sustituir cada "O(?)" por su coste. Puedes
// anadir los metodos auxiliares privados que necesites. Lo demas no se toca.
//
// Compilar y probar (desde cpp/):
//   g++ -std=c++17 -Wall test_arbol_expresiones.cpp -o test_arbol
//   ./test_arbol                         (todo)
//   ./test_arbol PruebasConstruccion     (solo un grupo)
#pragma once

#include <algorithm>
#include <cctype>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline bool es_operador(const std::string& t) {
    return t == "+" || t == "-" || t == "*" || t == "/";
}

inline bool es_numero(const std::string& t) {
    if (t.empty()) return false;
    for (char c : t)
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

// "3 4 + 2 *" -> {"3", "4", "+", "2", "*"}
inline std::vector<std::string> separar(const std::string& texto) {
    std::istringstream is(texto);
    std::vector<std::string> tokens;
    for (std::string t; is >> t;) tokens.push_back(t);
    return tokens;
}

struct Nodo {
    std::string valor;              // un numero ("3") o un operador ("+")
    Nodo* izquierdo;
    Nodo* derecho;
    static inline int vivos = 0;    // nodos creados y aun no borrados (lo miran los tests)

    explicit Nodo(std::string v, Nodo* izq = nullptr, Nodo* der = nullptr)
        : valor(std::move(v)), izquierdo(izq), derecho(der) { ++vivos; }
    ~Nodo() { --vivos; }

    bool es_hoja() const { return izquierdo == nullptr && derecho == nullptr; }
};

class ArbolExpresion {
public:
    Nodo* raiz = nullptr;

    ArbolExpresion() = default;                                   // arbol vacio
    explicit ArbolExpresion(const std::string& postfija) : raiz(construir(postfija)) {}
    ~ArbolExpresion() { liberar(raiz); }
    ArbolExpresion(const ArbolExpresion&) = delete;               // sin copias
    ArbolExpresion& operator=(const ArbolExpresion&) = delete;

    // --- Ejercicio 1: construir ------------------------------------------
    // Coste: O(?)
    // Devuelve la RAIZ del arbol de la expresion postfija (usa separar()).
    // Mal formada: throw std::invalid_argument("..."), sin dejar nodos sin liberar.
    Nodo* construir(const std::string& postfija) {
        (void)postfija;
        throw std::logic_error("TODO construir");
    }

    // --- Ejercicio 2: evaluar ---------------------------------------------
    // Coste: O(?)
    // Arbol vacio: std::invalid_argument. Division por cero: std::domain_error.
    double evaluar() const { throw std::logic_error("TODO evaluar"); }

    // --- Ejercicio 3: las tres notaciones (arbol vacio -> "") -------------
    // Coste: O(?)   (3 + 4) * 2  ->  "* + 3 4 2"
    std::string prefija() const { throw std::logic_error("TODO prefija"); }
    // Coste: O(?)   (3 + 4) * 2  ->  "3 4 + 2 *"
    std::string postfija() const { throw std::logic_error("TODO postfija"); }
    // Coste: O(?)   (3 + 4) * 2  ->  "((3 + 4) * 2)"
    std::string infija() const { throw std::logic_error("TODO infija"); }

    // --- Ejercicio 4: metricas --------------------------------------------
    // O(n). Hecho como ejemplo del patron publico + auxiliar recursivo.
    int num_nodos() const { return num_nodos(raiz); }
    // Coste: O(?)   vacio -> -1, una hoja -> 0
    int altura() const { throw std::logic_error("TODO altura"); }
    // Coste: O(?)
    int num_hojas() const { throw std::logic_error("TODO num_hojas"); }

    // --- Ampliacion (opcional) --------------------------------------------
    // (3 + 4) * 2 -> "(3 + 4) * 2"     8 - (4 - 1) -> "8 - (4 - 1)"
    std::string infija_minima() const { throw std::logic_error("TODO infija_minima"); }
    // (3 + 4) * 2 -> "* + 2 3 4"   (nivel a nivel, con std::queue)
    std::string por_niveles() const { throw std::logic_error("TODO por_niveles"); }
    // "* + 3 4 2" -> raiz del arbol. Mal formada: std::invalid_argument
    Nodo* construir_prefija(const std::string& prefija) {
        (void)prefija;
        throw std::logic_error("TODO construir_prefija");
    }

private:
    // Hecho: libera un subarbol. Es un recorrido POSTORDEN (hijos antes que el padre).
    static void liberar(Nodo* n) {
        if (n == nullptr) return;
        liberar(n->izquierdo);
        liberar(n->derecho);
        delete n;
    }

    static int num_nodos(const Nodo* n) {
        if (n == nullptr) return 0;
        return 1 + num_nodos(n->izquierdo) + num_nodos(n->derecho);
    }
};
