// Presentacion.cpp
#include "Presentacion.h"

// Implementacion del constructor y los metodos
MenuUI::MenuUI() : ancho(0), alto(0) {
	colorCursor = "220";
	colorSeparador = "93";
	colorFuente = "255";
	caracterSeparador = "-";
	actualizarTamanoConsola();
}

MenuUI::MenuUI(string cursor, string _colorSeparador, string fuente, string _caracterSeparador) : ancho(0), alto(0) {
	colorCursor = cursor;
	colorSeparador = _colorSeparador;
	colorFuente = fuente;
	caracterSeparador = _caracterSeparador;
	actualizarTamanoConsola();
}

void MenuUI::actualizarTamanoConsola() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole == INVALID_HANDLE_VALUE) {
		throw runtime_error("No se pudo obtener el handle de la consola");
	}

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		throw runtime_error("No se pudo obtener la informacion de la ventana de consola");
	}

	ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	alto = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int MenuUI::getAncho() const {
	return ancho;
}

int MenuUI::getAlto() const {
	return alto;
}

MenuUI::Tecla MenuUI::getTecla() const {
	int c = _getch();
	switch (c) {
		case 72:
			return MenuUI::ARRIBA;
		case 80:
			return MenuUI::ABAJO;
		case 13:
			return MenuUI::ENTER;
		case 27:
			return MenuUI::ESCAPE;
		default:
			return MenuUI::OTRA;
	}
}

void MenuUI::moverCursor(const vector<string>& opcionesMenu, MenuUI::Tecla tecla) {
	switch (tecla) {
		case MenuUI::ARRIBA:
			if (cursor > 0) cursor--;
			break;
		case MenuUI::ABAJO:
			if (cursor < opcionesMenu.size() - 1) cursor++;
			break;
		default:
			break;
	}
}

int MenuUI::getCursor() const {
	return cursor;
}

void MenuUI::setCursor(int posicion) {
	cursor = posicion;
}

void MenuUI::separador() {
	cout << RESET << endl << endl << COLORES << colorSeparador << FINAL;
	int n = ancho / caracterSeparador.length();
	for(int i = 0; i < n; i++) {
		cout  << caracterSeparador;
	}
	cout << endl << endl << RESET;
}

void MenuUI::mostrarCentrado(const string& mensaje) const {
	int longitudMensaje = mensaje.length();
	int posicionCentrada = (ancho - longitudMensaje) / 2;
	// Imprimir espacios en blanco antes del mensaje para centrarlo
	cout << COLORES << colorFuente << FINAL;
	cout << string(posicionCentrada, ' ') << mensaje << flush;
}

void MenuUI::centradoVertical(const int numLineas) const {
	int centrado = (alto - numLineas) / 2;
	for(int i = 0; i < centrado; i++) {
		cout << endl;
	}
}

void MenuUI::mostrarMenu(const vector<string>& opcionesMenu) {
	string temp = colorFuente;
	system("cls");
	int lineas = opcionesMenu.size() + 10;
	centradoVertical(lineas);
	MenuUI::separador();
	for(size_t i = 0; i < opcionesMenu.size(); ++i) { // Imprime todas las opciones del menu.
		cout << OCULTAR;
		if(i == cursor) { // Coloca una flecha en la posicion del cursor.
			cout << COD_ESC << PARPADEO << FINAL;
			setColorFuente(colorCursor);
			MenuUI::mostrarCentrado("> " + opcionesMenu[i] + " <");
			setColorFuente(temp);
			cout << RESET;
		} else {
			MenuUI::mostrarCentrado(opcionesMenu[i]);
		}
		if(i < opcionesMenu.size() - 1) cout << endl;
	}
	MenuUI::separador();
}

string MenuUI::solicitarDato(const string& mensaje) {
	string entrada;
	string temp = colorFuente;
	// Mostrar el mensaje centrado
	system("cls");
	centradoVertical(12);
	MenuUI::separador();
	mostrarCentrado(mensaje);
	cout << endl << COD_ESC << PARPADEO << FINAL;
	setColorFuente(colorCursor);
	mostrarCentrado("-> ");
	cout << GUARDAR;
	setColorFuente(temp);
	cout << RESET;
	MenuUI::separador();
	cout << CARGAR << MOSTRAR;
	getline(cin, entrada);
	cout << OCULTAR;

	return entrada;
}

bool MenuUI::confirmacion(const string& mensaje) {
	vector<string> opciones = {"Si", "No"};
	int seleccion = 0;
	string temp = colorFuente;
	while (true) {
		system("cls");
		centradoVertical(13);
		MenuUI::separador();
		MenuUI::mostrarCentrado(mensaje);
		cout << endl;
		for (int i = 0; i < opciones.size(); ++i) {
			if (i == seleccion) {
				cout << COD_ESC << PARPADEO << FINAL;
				setColorFuente(colorCursor);
				MenuUI::mostrarCentrado("> " + opciones[i] + " <");
				setColorFuente(temp);
				cout << RESET;
			} else {
				MenuUI::mostrarCentrado(opciones[i]);
			}
			if(i < opciones.size() - 1) cout << endl;
		}
		MenuUI::separador();
		Tecla tecla = MenuUI::getTecla();
		if (tecla == ARRIBA && seleccion > 0) {
			seleccion--;
		} else if (tecla == ABAJO && seleccion < opciones.size() - 1) {
			seleccion++;
		} else if (tecla == ENTER) {
			return seleccion == 0;
		}
	}
}

void MenuUI::pantallaDeCarga() {
	const vector<string> frames = {"-", "\\", "|", "/"};
	int frameIndex = 0;
	string temp = colorFuente;
	int colorIndex = 0;
	system("cls");
	centradoVertical(12);
	MenuUI::separador();
	MenuUI::mostrarCentrado(" Cargando ");
	cout << endl << GUARDAR << "\033[0K"; //<< COLORES << colorIndex << FINAL;
	MenuUI::mostrarCentrado(frames[frameIndex]);
	cout << RESET;
	MenuUI::separador();
	while (loading) {
		cout << OCULTAR;
		frameIndex = (frameIndex + 1) % frames.size();
		colorIndex = (colorIndex + 1) % 15;
		setColorFuente(to_string(colorIndex));
		cout << CARGAR << "\033[0K"; //<< COLORES << colorIndex << FINAL;
		MenuUI::mostrarCentrado(frames[frameIndex]);
		cout << RESET;
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	setColorFuente(temp);
}

void MenuUI::iniciarPantallaDeCarga() {
	loading = true;
	thread t(&MenuUI::pantallaDeCarga, this);
	t.detach();
}

void MenuUI::detenerPantallaDeCarga() {
	loading = false;
	MenuUI::mostrarCentrado(" Cargando... Completado! ");
	cout << endl;
}

void MenuUI::setColorCursor(string color) {
	colorCursor = color;
}

void MenuUI::setColorSeparador(string color) {
	colorSeparador = color;
}

void MenuUI::setColorFuente(string color) {
	colorFuente = color;
}

void MenuUI::setCaracterSeparador(string caracter) {
	caracterSeparador = caracter;
}

// Metodos para mostrar los resultados de Huffman
void MenuUI::displayStatistics(const vector<pair<uint32_t, int>>& stats) {
    system("cls");
    centradoVertical(stats.size() + 5);
    separador();
    mostrarCentrado("=== ESTADISTICAS DE FRECUENCIA ===");
    cout << endl;
    
    for (const auto& [cp, count] : stats) {
        string utf8_char;
        utf8::append(cp, back_inserter(utf8_char));
        
        stringstream ss;
        ss << "Caracter: ";
        
        if (cp == U' ') {
            ss << "[Espacio]";
        } else if (cp == U'\n') {
            ss << "[Salto de Linea]";
        } else if (cp == U'\t') {
            ss << "[Tabulador]";
        } else {
            ss << "'" << utf8_char << "'";
        }
        
        ss << " | Ocurrencias: " << count;
        
        cout << OCULTAR;
        mostrarCentrado(ss.str());
        cout << endl;
    }
    separador();
    cout << MOSTRAR;
}

void MenuUI::displayTree(const vector<string>& treeLines) {
    system("cls");
    centradoVertical(treeLines.size() + 5);
    separador();
    mostrarCentrado("=== ARBOL HUFFMAN ===");
    cout << endl << OCULTAR;
    
    // Ajustar el centrado para estructura de arbol
    int max_length = 0;
    for (const auto& line : treeLines) {
        if (line.length() > max_length) max_length = line.length();
    }
    
    for (const auto& line : treeLines) {
        int spaces = (getAncho() - max_length) / 2;
        cout << string(spaces, ' ') << line << endl;
    }
    
    separador();
    cout << MOSTRAR;
}

/*void MenuUI::displayTree(const vector<string>& treeLines) {
	system("cls");
	centradoVertical(treeLines.size() + 5);
	separador();
	mostrarCentrado("=== ARBOL HUFFMAN ===");
	cout << endl;

	for (const auto& line : treeLines) {
		cout << OCULTAR;
		mostrarCentrado(line);
		cout << endl;
	}

	separador();
	cout << MOSTRAR;
}*/

void MenuUI::displayCodeTable(const map<uint32_t, string>& codes) {
	system("cls");
	centradoVertical(codes.size() + 5);
	separador();
	mostrarCentrado("=== TABLA DE CODIGOS ===");
	cout << endl;
	
	for (const auto& [cp, code] : codes) {
        string utf8_char;
        utf8::append(cp, back_inserter(utf8_char));
        
        stringstream ss;
        ss << "Caracter: ";
        
        if (cp == U' ') {
            ss << "[Espacio]";
        } else if (cp == U'\n') {
            ss << "[Salto de Linea]";
        } else if (cp == U'\t') {
            ss << "[Tabulador]";
        } else {
            ss << "'" << utf8_char << "'";
        }
        
        ss << " | Codigo: " << code;
        
        cout << OCULTAR;
        mostrarCentrado(ss.str());
        cout << endl;
    }

	separador();
	cout << MOSTRAR;
}

void MenuUI::displayCompressionResults(size_t original, size_t compressed, double ratio) {
	vector<string> resultados = {
		"=== RESULTADOS DE COMPRESION ===",
		"Tamano original: " + to_string(original) + " bytes",
		"Tamano comprimido: " + to_string(compressed) + " bytes",
		"Porcentaje de compresion: " + to_string(ratio) + "%"
	};

	system("cls");
	centradoVertical(resultados.size() + 5);
	separador();

	for (const auto& linea : resultados) {
		cout << OCULTAR;
		mostrarCentrado(linea);
		cout << endl;
	}

	separador();
	cout << MOSTRAR;
}