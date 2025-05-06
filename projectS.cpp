#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

class Entidad {
private:
    int genProcRabbits;
    int genProcFoxes;
    int genFoodFoxes;
    int tipoEntidad;

public:
    // Constructor
    Entidad() : genProcRabbits(0), genProcFoxes(0), genFoodFoxes(0), tipoEntidad(0) {}
    Entidad(int genProcRabbits, int genProcFoxes, int genFoodFoxes, int tipoEntidad)
        : genProcRabbits(genProcRabbits),
          genProcFoxes(genProcFoxes),
          genFoodFoxes(genFoodFoxes),
          tipoEntidad(tipoEntidad) {}

    // Métodos getter
    int getGenProcRabbits() const { return genProcRabbits; }
    int getGenProcFoxes() const { return genProcFoxes; }
    int getGenFoodFoxes() const { return genFoodFoxes; }
    int getTipoEntidad() const { return tipoEntidad; }

    // Métodos setter
    void setGenProcRabbits(int valor) { genProcRabbits = valor; }
    void setGenProcFoxes(int valor) { genProcFoxes = valor; }
    void setGenFoodFoxes(int valor) { genFoodFoxes = valor; }
    void setTipoEntidad(int valor) { tipoEntidad = valor; }
};


vector<vector<Entidad>> mundo;
int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N; 


void print_M(vector<vector<Entidad>> &M);
void read(ifstream& archivo);


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

		Entidad entidad(0, 0, 0, numTipo);

		mundo[posX][posY] = entidad;
	}


	print_M(mundo);
}

