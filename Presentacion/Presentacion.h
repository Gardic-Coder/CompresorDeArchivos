// Presentacion.h
#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <thread>
#include <iomanip>
#include <sstream>
#include <map>
#include <chrono> // Para la funcion sleep
#include <atomic> // Para banderas atomicas
#include <utf8.h>

#define RESET "\033[0m" // Restablece el color de la fuente.
#define COD_ESC "\033["
#define COLORES "\033[38;5;" // Codigo de escape ANSI para establecer color de texto. Se usa "COLORES << Entero 0-255 << FINAL"
#define FINAL "m" // Final del codigo de escape ANSI para graficos.
#define ACOPLE ";"
#define PARPADEO "5" // Agrega parpadeo al texto.

#define GUARDAR "\0337" // Guarda la posicion del cursor.
#define CARGAR "\0338" // Mueve el cursor a la posicion cargada.
#define OCULTAR "\033[?25l" // Hace el cursor invisible.
#define MOSTRAR "\033[?25h" // Hace el cursor visible.

using namespace std;

class MenuUI {
public:
	MenuUI(); // Constructor.
	MenuUI(string cursor, string _colorSeparador, string fuente, string _caracterSeparador);
    
	// Entradas de teclado y actualizacion del cursor.
	enum Tecla { ARRIBA, ABAJO, ENTER, ESCAPE, OTRA };
	void moverCursor(const vector<string>& opcionesMenu, MenuUI::Tecla tecla);
    
    // Pantalla.
    void mostrarCentrado(const string& mensaje) const;
    void centradoVertical(const int numLineas) const;
    void mostrarMenu(const vector<string>& opcionesMenu);
    void separador();
    bool confirmacion(const string& mensaje);
    string solicitarDato(const string& mensaje); // Metodo para solicitar datos al usuario
    
    // Configuracion.
    void actualizarTamanoConsola(); // Toma el tamano de la consola y actualiza las variables.
    
    // Metodo para la pantalla de carga y su hilo
    void iniciarPantallaDeCarga();
    void detenerPantallaDeCarga();
    
    // Getters.
    int getAncho() const;
    int getAlto() const;
    int getCursor() const;
    Tecla getTecla() const;
    
    // Setters.
    void setCursor(int posicion);
    void setColorCursor(string color);
    void setColorSeparador(string color);
    void setColorFuente(string color);
    void setCaracterSeparador(string caracter);
    
    // Programa de compresion
    void displayStatistics(const vector<pair<uint32_t, int>>& stats);
    void displayTree(const vector<string>& tree);
    void displayCodeTable(const map<uint32_t, string>& codes);
    void displayCompressionResults(size_t original, size_t compressed, double ratio);
    string getBaseFilename(const string& filename);
    bool hasExtension(const string& filename, const string& extension);

private:
    int ancho; // Ancho de la ventana.
    int alto; // Alto de la ventana.
    int cursor; // Posicion del cursor.
    atomic<bool> loading; // Para detener la pantalla de carga
    string colorCursor;
    string colorSeparador;
    string colorFuente;
    string caracterSeparador;
    
    void pantallaDeCarga(); // Este metodo es llamado por iniciarPantallaDeCarga y detenerPantallaDeCarga.
};