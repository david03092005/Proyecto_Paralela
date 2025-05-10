#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
#include <omp.h>
#include <algorithm>

using namespace std;

class Entidad {
private:
    int genProcRabbits;
    int genProcFoxes;
    int genFoodFoxes;
    int tipoEntidad;
    int posX;
    int posY;
	bool movimiento;

public:
    // Constructor
    Entidad() : genProcRabbits(0), genProcFoxes(0), genFoodFoxes(0), tipoEntidad(0), posX(0), posY(0), movimiento(false) {}
    Entidad(int genProcRabbits, int genProcFoxes, int genFoodFoxes, int tipoEntidad, int posX, int posY, bool movimiento)
        : genProcRabbits(genProcRabbits),
          genProcFoxes(genProcFoxes),
          genFoodFoxes(genFoodFoxes),
          tipoEntidad(tipoEntidad),
       	  posX(posX),
	  posY(posY),
          movimiento(movimiento) {}

    // Métodos getter
    int getGenProcRabbits() const { return genProcRabbits; }
    int getGenProcFoxes() const { return genProcFoxes; }
    int getGenFoodFoxes() const { return genFoodFoxes; }
    int getTipoEntidad() const { return tipoEntidad; }
    int getPosX() const { return posX; }
    int getPosY() const { return posY; }
    bool getMovimiento() const { return movimiento; }

    // Métodos setter
    void setGenProcRabbits(int valor) { genProcRabbits = valor; }
    void setGenProcFoxes(int valor) { genProcFoxes = valor; }
    void setGenFoodFoxes(int valor) { genFoodFoxes = valor; }
    void setTipoEntidad(int valor) { tipoEntidad = valor; }
    void setPosX(int valor) { posX = valor; }
    void setPosY(int valor) { posY = valor; }
    void setMovimiento(bool valor) { movimiento = valor; }
};


vector<vector<Entidad>> mundo;
vector<Entidad> conejos;
vector<Entidad> zorros;
int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N; 


void print_M(vector<vector<Entidad>> &M);
void read(ifstream& archivo);
void simulacion();
void movimientosConejos(int gen);
void movimientosZorros(int gen);
void printFinalResult();


int main(int argc, char* argv[]) {
    	if (argc < 2) {
        	printf("Uso: %s <nombre_archivo.txt>\n", argv[0]);
        	return 1;
    	}

    	ifstream archivo(argv[1]);
    	if (!archivo.is_open()) {
        	printf("No se pudo abrir el archivo: %s\n", argv[1]);
        	return 1;
    	}

    	read(archivo);

    	archivo.close();

	simulacion();
	
	printFinalResult();
	
	return 0;
}


void print_M(vector<vector<Entidad>> &M){
        int n = M.size();
        for (int i = 0; i < n; i++){
                for (int j = 0; j < n; j++){
                        printf(" %d ", M[i][j].getTipoEntidad());
                }
                printf("\n");
        }
}

void read(ifstream& archivo) {
	string linea;
	getline(archivo, linea);
    	vector<int> datos;
    	istringstream stream(linea);
    	string palabra;
    	while (stream >> palabra){
    		datos.push_back(stoi(palabra));
    	}

	GEN_PROC_RABBITS = datos[0];
	GEN_PROC_FOXES = datos[1];
	GEN_FOOD_FOXES = datos[2];
	N_GEN = datos[3];
	R = datos[4];
	C = datos[5];
	N = datos[6];

	mundo = vector<vector<Entidad>>(R, vector<Entidad>(C));

	for (int i = 0; i < N; i++){
		getline(archivo, linea);
		istringstream stream(linea);
		string tipo;
		int posX, posY, numTipo;
		stream >> tipo >> posX >> posY;
		
		if (tipo == "ROCK"){
			numTipo = 1;
		}
		else if (tipo == "RABBIT"){
			numTipo = 2;
		}
		else{
			numTipo = 3;
		}

		Entidad entidad(0, 0, 0, numTipo, posX, posY, false);

		mundo[posX][posY] = entidad;
	
		if (entidad.getTipoEntidad() == 2){
			conejos.push_back(entidad);
		}
		else if (entidad.getTipoEntidad() == 3){
			zorros.push_back(entidad);
		}
	}


	printf("Gen 0\n");
	print_M(mundo);
}


void simulacion(){
	for (int i = 0; i < N_GEN; i++){
		movimientosConejos(i);
		movimientosZorros(i);
		printf("Gen %d\n",  i + 1);
		print_M(mundo);
	}
}


void movimientosConejos(int gen) {
    vector<vector<int>> movimientos = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<Entidad> nuevosConejos;
    vector<vector<Entidad>> nuevoMundo = mundo;

    for (Entidad& conejo : conejos) {
        int posX = conejo.getPosX();
        int posY = conejo.getPosY();
        vector<pair<int, int>> posiblesMov;

        for (auto& mov : movimientos) {
            int nx = posX + mov[0];
            int ny = posY + mov[1];
            if (nx >= 0 && nx < R && ny >= 0 && ny < C && mundo[nx][ny].getTipoEntidad() == 0) {
                posiblesMov.push_back({nx, ny});
            }
        }

        if (!posiblesMov.empty()) {
            int decision = (gen + posX + posY) % posiblesMov.size();
            int destX = posiblesMov[decision].first;
            int destY = posiblesMov[decision].second;

            bool seProcreo = (conejo.getGenProcRabbits() >= GEN_PROC_RABBITS);

            Entidad nuevoConejo = conejo;
            nuevoConejo.setPosX(destX);
            nuevoConejo.setPosY(destY);

            if (seProcreo) {
                nuevoConejo.setGenProcRabbits(0);
                Entidad hijo(0, 0, 0, 2, posX, posY, false);
                nuevoMundo[posX][posY] = hijo;
                nuevosConejos.push_back(hijo);
            } else {
                nuevoConejo.setGenProcRabbits(conejo.getGenProcRabbits() + 1);
                nuevoMundo[posX][posY] = Entidad();
            }

            nuevoMundo[destX][destY] = nuevoConejo;
            nuevosConejos.push_back(nuevoConejo);
        } else {
            conejo.setGenProcRabbits(conejo.getGenProcRabbits() + 1);
            nuevoMundo[posX][posY] = conejo;
            nuevosConejos.push_back(conejo);
        }
    }

    // Resolver conflictos
    vector<vector<Entidad>> mundoFinal(R, vector<Entidad>(C));
    for (int x = 0; x < R; ++x) {
        for (int y = 0; y < C; ++y) {
            if (mundo[x][y].getTipoEntidad() == 1 || mundo[x][y].getTipoEntidad() == 3) {
                mundoFinal[x][y] = mundo[x][y];
            }
        }
    }

    map<pair<int, int>, vector<Entidad>> conejosEnPosicion;
    for (Entidad& conejo : nuevosConejos) {
        int x = conejo.getPosX();
        int y = conejo.getPosY();
        conejosEnPosicion[{x, y}].push_back(conejo);
    }

    for (auto& [pos, lista] : conejosEnPosicion) {
        int x = pos.first;
        int y = pos.second;
        if (lista.size() > 1) {
            Entidad* mejorConejo = &lista[0];
            for (Entidad& conejo : lista) {
                if (conejo.getGenProcRabbits() > mejorConejo->getGenProcRabbits()) {
                    mejorConejo = &conejo;
                }
            }
            mundoFinal[x][y] = *mejorConejo;
        } else {
            mundoFinal[x][y] = lista[0];
        }
    }

    // Actualizar mundo y lista de conejos
    mundo = mundoFinal;
    conejos.clear();
    for (int x = 0; x < R; ++x) {
        for (int y = 0; y < C; ++y) {
            if (mundo[x][y].getTipoEntidad() == 2) {
                conejos.push_back(mundo[x][y]);
            }
        }
    }
}


void movimientosZorros(int gen) {
    bool confirmacion = (gen % 2) == 0;
    vector<vector<int>> movimientos = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<vector<Entidad>> mundoLocal = mundo;
    vector<Entidad> zorrosLocal;

    for (int i = 0; i < zorros.size(); i++) {
        Entidad zorro = zorros[i];
        int posX = zorro.getPosX();
        int posY = zorro.getPosY();
        vector<vector<int>> posiblesMov;
        int cantPosibles = 0;
        int movX, movY;
        bool comerConejos = false;
        bool gano = true;

        for (int mov = 0; mov < 4; mov++) {
            movX = movimientos[mov][0];
            movY = movimientos[mov][1];
            int nx = posX + movX;
            int ny = posY + movY;

            if (nx >= 0 && nx < R && ny >= 0 && ny < C) {
                int tipo = mundo[nx][ny].getTipoEntidad();
                bool mov = mundo[nx][ny].getMovimiento();

                if ((tipo == 0 || (tipo == 3 && mov == confirmacion)) && !comerConejos) {
                    posiblesMov.push_back({movX, movY});
                    cantPosibles++;
                }
                else if (tipo == 2) {
                    if (!comerConejos){
                        posiblesMov = vector<vector<int>>{{movX, movY}};
                        cantPosibles = 1;
                        comerConejos = true;
                    }
                    else{
                        posiblesMov.push_back({movX, movY});
                        cantPosibles++;
                    }
                }
            }
        }

        if (zorro.getGenFoodFoxes() + 1 >= GEN_FOOD_FOXES && !comerConejos){
            Entidad vacio(0, 0, 0, 0, posX, posY, false);
            mundoLocal[posX][posY] = vacio;
        }
        else if (cantPosibles > 0) {
            int decision = (gen + posX + posY) % cantPosibles;
            movX = posiblesMov[decision][0];
            movY = posiblesMov[decision][1];
            int destX = posX + movX;
            int destY = posY + movY;

            if (mundo[destX][destY].getTipoEntidad() == 3 && mundo[destX][destY].getMovimiento() == confirmacion) {
                int j = 0;
                while (j < zorros.size()) {
                    if (zorros[j].getPosX() == destX && zorros[j].getPosY() == destY){
                        break;
                    }
                    j++;
                }

                if (j < zorros.size() && zorro.getGenProcFoxes() < zorros[j].getGenProcFoxes()) {
                    Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorro.getGenFoodFoxes() + 1, 3, destX, destY, !zorro.getMovimiento());
                    mundoLocal[destX][destY] = nuevoZorro;
                    zorrosLocal.push_back(nuevoZorro);
                }
                else if (j < zorros.size() && zorro.getGenProcFoxes() == zorros[j].getGenProcFoxes() && zorro.getGenFoodFoxes() < zorros[j].getGenFoodFoxes()) {
                    Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorro.getGenFoodFoxes() + 1, 3, destX, destY, !zorro.getMovimiento());
                    mundoLocal[destX][destY] = nuevoZorro;
                    zorrosLocal.push_back(nuevoZorro);
                }
                else {
                    gano = false;
                }
            }
            else {
                int zorroFood = zorro.getGenFoodFoxes() + 1;
                int tipo = mundo[destX][destY].getTipoEntidad();

                if (tipo == 2){ // Si hay un conejo, lo come
                    zorroFood = 0;
                }

                Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorroFood, 3, destX, destY, !zorro.getMovimiento());
                mundoLocal[destX][destY] = nuevoZorro;
                zorrosLocal.push_back(nuevoZorro);
            }

            if (zorro.getGenProcFoxes() == GEN_PROC_FOXES) {
                if (gano) {
                    zorrosLocal.back().setGenProcFoxes(0);
                    mundoLocal[destX][destY].setGenProcFoxes(0);
                }
                Entidad hijo(0, 0, 0, 3, posX, posY, !zorro.getMovimiento());
                mundoLocal[posX][posY] = hijo;
                zorrosLocal.push_back(hijo);
            }
            else {
                mundoLocal[posX][posY] = Entidad();
            }
        }
        else {
            mundoLocal[posX][posY] = zorro;
            zorrosLocal.push_back(zorro);
        }
    }

    // Conflictos y combinación final
    vector<vector<Entidad>> mundoFinal(R, vector<Entidad>(C));
    map<pair<int, int>, bool> conejosComidos;
    map<pair<int, int>, vector<Entidad>> zorrosEnPosicion;

    // Copiar elementos estáticos
    for (int x = 0; x < R; x++) {
        for (int y = 0; y < C; y++) {
            if (mundo[x][y].getTipoEntidad() == 1 || mundo[x][y].getTipoEntidad() == 2) {
                mundoFinal[x][y] = mundo[x][y];
            }
        }
    }

    for (auto& zorro : zorrosLocal) {
        int x = zorro.getPosX();
        int y = zorro.getPosY();
        zorrosEnPosicion[{x, y}].push_back(zorro);

        if (mundo[x][y].getTipoEntidad() == 2) {
            conejosComidos[{x, y}] = true;
        }
    }

    for (auto& [pos, listaZorr] : zorrosEnPosicion) {
        int x = pos.first;
        int y = pos.second;

        if (listaZorr.size() > 1) {
            sort(listaZorr.begin(), listaZorr.end(), [](const Entidad& a, const Entidad& b) {
                if (a.getGenProcFoxes() != b.getGenProcFoxes()) {
                    return a.getGenProcFoxes() > b.getGenProcFoxes();
                }
                return a.getGenFoodFoxes() < b.getGenFoodFoxes();
            });
        }

        mundoFinal[x][y] = listaZorr[0];
    }

    // Actualizar mundo global
    mundo = mundoFinal;
    zorros.clear();
    conejos.clear();

    for (int x = 0; x < R; x++) {
        for (int y = 0; y < C; y++) {
            if (mundo[x][y].getTipoEntidad() == 3) {
                zorros.push_back(mundo[x][y]);
            } else if (mundo[x][y].getTipoEntidad() == 2 && !conejosComidos[{x, y}]) {
                conejos.push_back(mundo[x][y]);
            }
        }
    }
}


void printFinalResult() {
        int numObjetos = 0;
        for (int x = 0; x < R; x++) {
                for (int y = 0; y < C; y++) {
                        if (mundo[x][y].getTipoEntidad() != 0) {
                                numObjetos++;
                        }
                }
        }

        cout << GEN_PROC_RABBITS << " " << GEN_PROC_FOXES << " " << GEN_FOOD_FOXES << " "
         << 0 << " " << R << " " << C << " " << numObjetos << endl;

        // Imprimir cada objeto con base al formato requerido
        for (int x = 0; x < R; x++) {
        for (int y = 0; y < C; y++) {
            int tipo = mundo[x][y].getTipoEntidad();
            if (tipo != 0) {
                string tipoStr;
                switch (tipo) {
                    case 1: tipoStr = "ROCK"; break;
                    case 2: tipoStr = "RABBIT"; break;
                    case 3: tipoStr = "FOX"; break;
		}
                cout << tipoStr << " " << x << " " << y << endl;
	    }
	}
	}
}
