#include <vector>

#include "Datos/Datos.h"
#include "Logica/Logica.h"
#include "Presentacion/Presentacion.h"

using namespace std;

int main() {
	bool salir = false;
	MenuUI ui("220", "123", "15", "+");
	FileHandler fh;
	HuffmanManager hm;
	vector<string> opcionesMenu = {"Ingresar archivo para comprimir", "Mostrar Estadisticas de ocurrencia",
	                               "Mostrar Arbol", "Mostrar Tabla de Codigos", "Mostrar Comparacion", "Salir"
	                              };
	string filename;
	string content;
	
	SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
	ui.setCursor(0);

	while(!salir) {
		try {
			ui.actualizarTamanoConsola(); // Asegurarse de que las dimensiones están actualizadas
		} catch (const runtime_error& e) {
			cerr << e.what() << endl;
		}
		ui.mostrarMenu(opcionesMenu);
		MenuUI::Tecla tecla = ui.getTecla();

		switch(tecla) {
			case MenuUI::ARRIBA: {
				ui.moverCursor(opcionesMenu, tecla);
				break;
			}
			case MenuUI::ABAJO: {
				ui.moverCursor(opcionesMenu, tecla);
				break;
			}
			case MenuUI::ENTER: {
				switch(ui.getCursor()) {
					case 0: {
						try {
							filename = ui.solicitarDato("Ingrese el nombre del archivo a comprimir: ") + ".txt";
							content = fh.readFile(filename);
							hm.processData(content);
							fh.writeCompressedFile(filename + ".huf", hm.getCompressedData());
						} catch(const std::exception& e) {
							cerr << "Error: " << e.what() << endl;
						}
						break;
					}
					case 1: {
						ui.displayStatistics(hm.getFrequencyStats());
						getch();
						break;
					}
					case 2: {
						ui.displayTree(hm.getTreeStructure());
						getch();
						break;
					}
					case 3: {
						ui.displayCodeTable(hm.getCodeTable());
						getch();
						break;
					}
					case 4: {
						ui.displayCompressionResults(hm.getOriginalSize(), hm.getCompressedSize(), hm.getCompressionRatio());
						getch();
						break;
					}
					case 5: {
						if(ui.confirmacion("¿Estas seguro que deseas salir?")) {
							ui.mostrarCentrado("Saliendo del menu...");
							salir = !salir;
						}
						break;
					}
				}
				break;
			}
			case MenuUI::ESCAPE: {
				if(ui.confirmacion("¿Estas seguro que deseas salir?")) {
					ui.mostrarCentrado("Saliendo del menu...");
					salir = !salir;
				}
				break;
			}
		}
	}

	/*try {


		string filename = ui.solicitarDato("Ingrese el nombre del archivo a comprimir: ");
		string content = fh.readFile(filename);

		hm.processData(content);

		ui.displayStatistics(hm.getFrequencyStats());
		ui.displayTree(hm.getTreeStructure());
		ui.displayCodeTable(hm.getCodeTable());
		ui.displayCompressionResults(hm.getOriginalSize(), hm.getCompressedSize(), hm.getCompressionRatio());

		// Escribir archivo comprimido
		fh.writeCompressedFile(filename + ".huf", hm.getCompressedData());

	} catch(const std::exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}*/
	return 0;
}