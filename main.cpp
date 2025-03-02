#include <vector>

#include "Datos/Datos.h"
#include "Logica/Logica.h"
#include "Presentacion/Presentacion.h"

using namespace std;

int main() {
	bool salir = false;
	MenuUI ui("124", "57", "159", "=");
	FileHandler fh;
	HuffmanManager hm;
	vector<string> opcionesMenu = {"Ingresar archivo para comprimir", "Mostrar Estadisticas de ocurrencia",
	                               "Mostrar Arbol", "Mostrar Tabla de Codigos", "Mostrar Comparacion", "Descomprimir Archivo",
	                               "Salir"
	                              };
	string filename;
	string content;
	string inputFilename;
	string baseName;

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
							inputFilename = ui.solicitarDato("Ingrese el nombre del archivo a comprimir: ");
							baseName = ui.getBaseFilename(inputFilename);

							if (!ui.hasExtension(inputFilename, ".txt")) {
								inputFilename += ".txt";
							}

							content = fh.readFile(inputFilename);
							hm.processData(content);

							string outputFilename = baseName + ".huf";
							fh.writeCompressedFile(outputFilename, hm.getCompressedData(), hm.getCodeTable());

							ui.mostrarCentrado("Archivo comprimido exitosamente como: " + outputFilename);
						} catch(const std::exception& e) {
							//cerr << "Error: " << e.what() << endl;
							ui.mostrarCentrado("Error: " + string(e.what()));
						}
						getch();
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
						try {
							inputFilename = ui.solicitarDato("Ingrese el nombre del archivo comprimido: ");
							baseName = ui.getBaseFilename(inputFilename);
							
							if (!ui.hasExtension(inputFilename, ".huf")) {
								inputFilename += ".huf";
							}
							
							auto [codes, compressedData] = fh.readCompressedFile(inputFilename);

							string decompressed = hm.decompress(codes, compressedData);

							string outputFilename = baseName + "_descomprimido.txt";
							fh.writeFile(outputFilename, decompressed);

							ui.mostrarCentrado("Archivo descomprimido exitosamente como: " + outputFilename);
						} catch (const exception& e) {
							ui.mostrarCentrado("Error: " + string(e.what()));
						}
						getch();
						break;
					}
					case 6: {
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
	return 0;
}