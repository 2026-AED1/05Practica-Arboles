# test_arbol_expresiones.py
# Pruebas de autocomprobacion del ArbolExpresion. No hay que modificarlo: hay
# que conseguir que pase entero.
#
# Se ejecuta por grupos, en el mismo orden en que se programa en clase:
#   python3 -m unittest -v test_arbol_expresiones.PruebasConstruccion
#   python3 -m unittest -v test_arbol_expresiones.PruebasEvaluacion
#   python3 -m unittest -v test_arbol_expresiones.PruebasNotaciones
#   python3 -m unittest -v test_arbol_expresiones.PruebasMetricas
# Ampliacion (opcional; se saltan mientras no esten hechos):
#   PruebasAmpliacion
# Todo:
#   python3 -m unittest -v test_arbol_expresiones

import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from arbol_expresiones import ArbolExpresion, Nodo  # noqa: E402


class PruebaBase(unittest.TestCase):
    def opcional(self, funcion, *args):
        # Ejecuta un metodo de la ampliacion; si no esta hecho, salta la prueba.
        try:
            return funcion(*args)
        except NotImplementedError:
            self.skipTest("ampliacion opcional sin implementar")


class PruebasConstruccion(PruebaBase):
    def test_un_numero(self):
        r = ArbolExpresion("7").raiz
        self.assertIsInstance(r, Nodo)
        self.assertEqual(r.valor, "7")
        self.assertTrue(r.es_hoja())

    def test_suma(self):
        r = ArbolExpresion("3 4 +").raiz
        self.assertEqual(r.valor, "+")
        self.assertEqual(r.izquierdo.valor, "3")
        self.assertEqual(r.derecho.valor, "4")

    def test_orden_de_los_hijos(self):
        # "8 2 -" es 8 - 2: el 8 tiene que ser el hijo IZQUIERDO
        r = ArbolExpresion("8 2 -").raiz
        self.assertEqual(r.izquierdo.valor, "8")
        self.assertEqual(r.derecho.valor, "2")

    def test_expresion_compuesta(self):
        #        *
        #       / \
        #      +   2
        #     / \
        #    3   4
        r = ArbolExpresion("3 4 + 2 *").raiz
        self.assertEqual(r.valor, "*")
        self.assertEqual(r.izquierdo.valor, "+")
        self.assertEqual(r.derecho.valor, "2")
        self.assertEqual(r.izquierdo.izquierdo.valor, "3")
        self.assertEqual(r.izquierdo.derecho.valor, "4")
        self.assertTrue(r.derecho.es_hoja())

    def test_numeros_de_varias_cifras(self):
        r = ArbolExpresion("100 25 /").raiz
        self.assertEqual(r.izquierdo.valor, "100")
        self.assertEqual(r.derecho.valor, "25")

    def test_faltan_operandos(self):
        with self.assertRaises(ValueError):
            ArbolExpresion("3 +")

    def test_sobran_operandos(self):
        with self.assertRaises(ValueError):
            ArbolExpresion("3 4 5 +")

    def test_token_no_valido(self):
        with self.assertRaises(ValueError):
            ArbolExpresion("3 x +")

    def test_cadena_vacia(self):
        with self.assertRaises(ValueError):
            ArbolExpresion("")


class PruebasEvaluacion(PruebaBase):
    def test_un_numero(self):
        self.assertEqual(ArbolExpresion("42").evaluar(), 42)

    def test_operaciones(self):
        self.assertEqual(ArbolExpresion("3 4 +").evaluar(), 7)
        self.assertEqual(ArbolExpresion("10 4 -").evaluar(), 6)
        self.assertEqual(ArbolExpresion("6 7 *").evaluar(), 42)
        self.assertAlmostEqual(ArbolExpresion("7 2 /").evaluar(), 3.5)

    def test_compuesta(self):
        self.assertEqual(ArbolExpresion("3 4 + 2 *").evaluar(), 14)
        self.assertEqual(ArbolExpresion("3 4 2 * +").evaluar(), 11)

    def test_resta_no_conmutativa(self):
        self.assertEqual(ArbolExpresion("10 4 - 3 -").evaluar(), 3)
        self.assertEqual(ArbolExpresion("10 4 3 - -").evaluar(), 9)

    def test_division_por_cero(self):
        with self.assertRaises(ZeroDivisionError):
            ArbolExpresion("1 2 2 - /").evaluar()

    def test_arbol_vacio(self):
        with self.assertRaises(ValueError):
            ArbolExpresion().evaluar()


class PruebasNotaciones(PruebaBase):
    def test_prefija(self):
        self.assertEqual(ArbolExpresion("3 4 + 2 *").prefija(), "* + 3 4 2")

    def test_postfija(self):
        self.assertEqual(ArbolExpresion("3 4 + 2 *").postfija(), "3 4 + 2 *")

    def test_infija(self):
        self.assertEqual(ArbolExpresion("3 4 + 2 *").infija(), "((3 + 4) * 2)")
        self.assertEqual(ArbolExpresion("3 4 2 * +").infija(), "(3 + (4 * 2))")

    def test_una_hoja(self):
        a = ArbolExpresion("7")
        self.assertEqual(a.prefija(), "7")
        self.assertEqual(a.postfija(), "7")
        self.assertEqual(a.infija(), "7")

    def test_arbol_vacio(self):
        a = ArbolExpresion()
        self.assertEqual(a.prefija(), "")
        self.assertEqual(a.postfija(), "")
        self.assertEqual(a.infija(), "")

    def test_ida_y_vuelta(self):
        original = "5 1 2 + 4 * + 3 -"
        self.assertEqual(ArbolExpresion(original).postfija(), original)


class PruebasMetricas(PruebaBase):
    def test_arbol_vacio(self):
        a = ArbolExpresion()
        self.assertEqual(a.altura(), -1)
        self.assertEqual(a.num_hojas(), 0)

    def test_una_hoja(self):
        a = ArbolExpresion("5")
        self.assertEqual(a.altura(), 0)
        self.assertEqual(a.num_hojas(), 1)

    def test_ejemplo(self):
        a = ArbolExpresion("3 4 + 2 *")
        self.assertEqual(a.altura(), 2)
        self.assertEqual(a.num_hojas(), 3)
        self.assertEqual(a.num_nodos(), 5)

    def test_degenerado(self):
        # 1 + 2 + 3 + 4 + 5: una rama que baja siempre por la izquierda
        self.assertEqual(ArbolExpresion("1 2 + 3 + 4 + 5 +").altura(), 4)

    def test_hojas_igual_a_operadores_mas_uno(self):
        for p in ["1 2 +", "1 2 + 3 4 - *", "9 8 7 6 5 + + + +"]:
            a = ArbolExpresion(p)
            self.assertEqual(a.num_hojas(), a.num_nodos() - a.num_hojas() + 1, p)

    def test_num_internos(self):
        # Nodos internos = operadores; las hojas (numeros) no cuentan
        self.assertEqual(ArbolExpresion().num_internos(), 0)
        self.assertEqual(ArbolExpresion("5").num_internos(), 0)
        self.assertEqual(ArbolExpresion("3 4 +").num_internos(), 1)
        self.assertEqual(ArbolExpresion("3 4 + 2 *").num_internos(), 2)
        self.assertEqual(ArbolExpresion("1 2 + 3 + 4 + 5 +").num_internos(), 4)
        self.assertEqual(ArbolExpresion("5 1 2 + 4 * + 3 -").num_internos(), 4)

    def test_internos_mas_hojas_igual_a_nodos(self):
        for p in ["7", "1 2 +", "1 2 + 3 4 - *", "9 8 7 6 5 + + + +"]:
            a = ArbolExpresion(p)
            self.assertEqual(a.num_internos() + a.num_hojas(), a.num_nodos(), p)
            self.assertEqual(a.num_hojas(), a.num_internos() + 1, p)


class PruebasAmpliacion(PruebaBase):
    def test_infija_minima(self):
        casos = {
            "3 4 + 2 *": "(3 + 4) * 2",
            "3 4 2 * +": "3 + 4 * 2",
            "8 4 1 - -": "8 - (4 - 1)",
            "8 4 - 1 -": "8 - 4 - 1",
            "1 2 + 3 4 + /": "(1 + 2) / (3 + 4)",
        }
        for postfija, esperado in casos.items():
            self.assertEqual(self.opcional(ArbolExpresion(postfija).infija_minima), esperado, postfija)

    def test_por_niveles(self):
        self.assertEqual(self.opcional(ArbolExpresion("3 4 + 2 *").por_niveles), "* + 2 3 4")
        self.assertEqual(self.opcional(ArbolExpresion().por_niveles), "")

    def test_construir_prefija(self):
        a = ArbolExpresion()
        a.raiz = self.opcional(a.construir_prefija, "* + 3 4 2")
        self.assertEqual(a.postfija(), "3 4 + 2 *")

    def test_construir_prefija_mal_formada(self):
        a = ArbolExpresion()
        for mala in ["* 3", "+ 1 2 3", "", "+ 1 x"]:
            with self.assertRaises(ValueError, msg=mala):
                self.opcional(a.construir_prefija, mala)


if __name__ == "__main__":
    unittest.main(verbosity=2)