// test_arbol_expresiones.cpp
// Pruebas de autocomprobacion del ArbolExpresion en C++. No hay que modificarlo.
//
// Compilar (desde la carpeta cpp/):
//   g++ -std=c++17 -Wall test_arbol_expresiones.cpp -o test_arbol
// Ejecutar todo, o solo algunos grupos:
//   ./test_arbol
//   ./test_arbol PruebasConstruccion PruebasEvaluacion
// Grupos, en el orden de trabajo:
//   obligatorio: PruebasConstruccion PruebasEvaluacion PruebasNotaciones
//                PruebasMetricas PruebasMemoria
//   ampliacion:  PruebasAmpliacion (opcional)

#include <cmath>
#include <string>

#include "arbol_expresiones.hpp"
#include "mini_test.hpp"

using std::string;

// true si f() lanza la excepcion E (un TODO sin hacer se deja pasar: PENDIENTE)
template <typename E, typename F>
static bool lanza(F f) {
    try {
        f();
    } catch (const E& e) {
        if (string(e.what()).rfind("TODO", 0) == 0) throw;
        return true;
    }
    return false;
}

static bool cerca(double a, double b) { return std::fabs(a - b) < 1e-9; }

// Ejecuta una operacion de la ampliacion; si sigue en TODO, la prueba se salta.
template <typename F>
static auto opcional(F f) -> decltype(f()) {
    try {
        return f();
    } catch (const std::logic_error& e) {
        if (string(e.what()).rfind("TODO", 0) == 0) throw mini_test::Saltar("ampliacion opcional");
        throw;
    }
}

// ---------------------------------------------------------------- construccion

PRUEBA(PruebasConstruccion, un_numero) {
    ArbolExpresion a("7");
    COMPROBAR(a.raiz != nullptr, "la raiz no puede ser nullptr");
    COMPROBAR(a.raiz->valor == "7", "la raiz debe ser 7");
    COMPROBAR(a.raiz->es_hoja(), "un numero solo es una hoja");
}

PRUEBA(PruebasConstruccion, suma) {
    ArbolExpresion a("3 4 +");
    COMPROBAR(a.raiz->valor == "+", "raiz");
    COMPROBAR(a.raiz->izquierdo->valor == "3", "hijo izquierdo");
    COMPROBAR(a.raiz->derecho->valor == "4", "hijo derecho");
}

PRUEBA(PruebasConstruccion, orden_de_los_hijos) {
    ArbolExpresion a("8 2 -");
    COMPROBAR(a.raiz->izquierdo->valor == "8", "\"8 2 -\" es 8 - 2: el 8 es el hijo IZQUIERDO");
    COMPROBAR(a.raiz->derecho->valor == "2", "el 2 es el hijo DERECHO");
}

PRUEBA(PruebasConstruccion, expresion_compuesta) {
    ArbolExpresion a("3 4 + 2 *");
    Nodo* r = a.raiz;
    COMPROBAR(r->valor == "*", "raiz");
    COMPROBAR(r->izquierdo->valor == "+", "hijo izquierdo");
    COMPROBAR(r->derecho->valor == "2", "hijo derecho");
    COMPROBAR(r->izquierdo->izquierdo->valor == "3", "nieto izquierdo");
    COMPROBAR(r->izquierdo->derecho->valor == "4", "nieto derecho");
}

PRUEBA(PruebasConstruccion, mal_formadas) {
    COMPROBAR(lanza<std::invalid_argument>([] { ArbolExpresion a("3 +"); }), "faltan operandos");
    COMPROBAR(lanza<std::invalid_argument>([] { ArbolExpresion a("3 4 5 +"); }), "sobran operandos");
    COMPROBAR(lanza<std::invalid_argument>([] { ArbolExpresion a("3 x +"); }), "token no valido");
    COMPROBAR(lanza<std::invalid_argument>([] { ArbolExpresion a(""); }), "cadena vacia");
}

// ---------------------------------------------------------------- evaluacion

PRUEBA(PruebasEvaluacion, operaciones) {
    COMPROBAR(cerca(ArbolExpresion("42").evaluar(), 42), "42");
    COMPROBAR(cerca(ArbolExpresion("3 4 +").evaluar(), 7), "3 + 4");
    COMPROBAR(cerca(ArbolExpresion("10 4 -").evaluar(), 6), "10 - 4");
    COMPROBAR(cerca(ArbolExpresion("6 7 *").evaluar(), 42), "6 * 7");
    COMPROBAR(cerca(ArbolExpresion("7 2 /").evaluar(), 3.5), "7 / 2");
}

PRUEBA(PruebasEvaluacion, compuesta) {
    COMPROBAR(cerca(ArbolExpresion("3 4 + 2 *").evaluar(), 14), "(3 + 4) * 2");
    COMPROBAR(cerca(ArbolExpresion("3 4 2 * +").evaluar(), 11), "3 + 4 * 2");
    COMPROBAR(cerca(ArbolExpresion("10 4 3 - -").evaluar(), 9), "10 - (4 - 3)");
}

PRUEBA(PruebasEvaluacion, errores) {
    COMPROBAR(lanza<std::domain_error>([] { ArbolExpresion("1 2 2 - /").evaluar(); }), "division por cero");
    COMPROBAR(lanza<std::invalid_argument>([] { ArbolExpresion().evaluar(); }), "arbol vacio");
}

// ---------------------------------------------------------------- notaciones

PRUEBA(PruebasNotaciones, tres_notaciones) {
    ArbolExpresion a("3 4 + 2 *");
    COMPROBAR(a.prefija() == "* + 3 4 2", "prefija");
    COMPROBAR(a.postfija() == "3 4 + 2 *", "postfija");
    COMPROBAR(a.infija() == "((3 + 4) * 2)", "infija");
}

PRUEBA(PruebasNotaciones, hoja_y_vacio) {
    ArbolExpresion h("7"), v;
    COMPROBAR(h.prefija() == "7" && h.postfija() == "7" && h.infija() == "7", "una hoja");
    COMPROBAR(v.prefija().empty() && v.postfija().empty() && v.infija().empty(), "arbol vacio");
}

PRUEBA(PruebasNotaciones, ida_y_vuelta) {
    string p = "5 1 2 + 4 * + 3 -";
    COMPROBAR(ArbolExpresion(p).postfija() == p, "construir y volver a postfija");
}

// ---------------------------------------------------------------- metricas

PRUEBA(PruebasMetricas, vacio_y_hoja) {
    ArbolExpresion v, h("5");
    COMPROBAR(v.altura() == -1 && v.num_hojas() == 0, "arbol vacio");
    COMPROBAR(h.altura() == 0 && h.num_hojas() == 1, "una hoja");
}

PRUEBA(PruebasMetricas, ejemplo) {
    ArbolExpresion a("3 4 + 2 *");
    COMPROBAR(a.altura() == 2, "altura");
    COMPROBAR(a.num_hojas() == 3, "hojas");
    COMPROBAR(a.num_nodos() == 5, "nodos");
}

PRUEBA(PruebasMetricas, degenerado) {
    COMPROBAR(ArbolExpresion("1 2 + 3 + 4 + 5 +").altura() == 4, "1 + 2 + 3 + 4 + 5");
}

PRUEBA(PruebasMetricas, num_internos) {
    // Nodos internos = operadores; las hojas (numeros) no cuentan
    COMPROBAR(ArbolExpresion().num_internos() == 0, "arbol vacio");
    COMPROBAR(ArbolExpresion("5").num_internos() == 0, "una hoja");
    COMPROBAR(ArbolExpresion("3 4 +").num_internos() == 1, "3 + 4");
    COMPROBAR(ArbolExpresion("3 4 + 2 *").num_internos() == 2, "(3 + 4) * 2");
    COMPROBAR(ArbolExpresion("1 2 + 3 + 4 + 5 +").num_internos() == 4, "1 + 2 + 3 + 4 + 5");
    COMPROBAR(ArbolExpresion("5 1 2 + 4 * + 3 -").num_internos() == 4, "5 + (1 + 2) * 4 - 3");
}

PRUEBA(PruebasMetricas, internos_mas_hojas_igual_a_nodos) {
    for (string p : {"7", "1 2 +", "1 2 + 3 4 - *", "9 8 7 6 5 + + + +"}) {
        ArbolExpresion a(p);
        COMPROBAR(a.num_internos() + a.num_hojas() == a.num_nodos(), "internos + hojas == nodos: " + p);
        COMPROBAR(a.num_hojas() == a.num_internos() + 1, "hojas == internos + 1: " + p);
    }
}

// ---------------------------------------------------------------- memoria

PRUEBA(PruebasMemoria, sin_fugas) {
    int antes = Nodo::vivos;
    {
        ArbolExpresion a("1 2 + 3 4 - *");
        COMPROBAR(Nodo::vivos - antes == 7, "deberia haber 7 nodos vivos");
    }
    COMPROBAR(Nodo::vivos == antes, "el destructor debe liberar todos los nodos");
    lanza<std::invalid_argument>([] { ArbolExpresion a("1 2 3 +"); });
    lanza<std::invalid_argument>([] { ArbolExpresion a("1 2 + x"); });
    COMPROBAR(Nodo::vivos == antes, "si construir lanza una excepcion, no puede dejar nodos vivos");
}

// ---------------------------------------------------------------- ampliacion

PRUEBA(PruebasAmpliacion, infija_minima) {
    COMPROBAR(opcional([] { return ArbolExpresion("3 4 + 2 *").infija_minima(); }) == "(3 + 4) * 2", "(3 + 4) * 2");
    COMPROBAR(opcional([] { return ArbolExpresion("3 4 2 * +").infija_minima(); }) == "3 + 4 * 2", "3 + 4 * 2");
    COMPROBAR(opcional([] { return ArbolExpresion("8 4 1 - -").infija_minima(); }) == "8 - (4 - 1)", "8 - (4 - 1)");
}

PRUEBA(PruebasAmpliacion, por_niveles) {
    COMPROBAR(opcional([] { return ArbolExpresion("3 4 + 2 *").por_niveles(); }) == "* + 2 3 4", "nivel a nivel");
}

PRUEBA(PruebasAmpliacion, construir_prefija) {
    ArbolExpresion a;
    a.raiz = opcional([&] { return a.construir_prefija("* + 3 4 2"); });
    COMPROBAR(a.postfija() == "3 4 + 2 *", "construir desde prefija");
}

int main(int argc, char** argv) { return mini_test::ejecutar(argc, argv); }