#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>

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


/*
void movimientosConejos(int gen){
	bool confirmacion = (gen % 2) == 0;
	vector<vector<int>> movimientos = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
	for (int i = 0; i < conejos.size(); i++){
		Entidad *conejo = &conejos[i];
		int posX = conejo->getPosX();
		int posY = conejo->getPosY();
		vector<vector<int>> posiblesMov;
		int cantPosibles = 0;
		int movX;
		int movY;
		
		for (int mov = 0; mov < 4; mov++){
			movX = movimientos[mov][0];
			movY = movimientos[mov][1];
			if (posX + movX < R  && posX + movX >= 0 && posY + movY < C && posY + movY >= 0){
				if (mundo[posX+movX][posY+movY].getTipoEntidad() == 0){
					cantPosibles = cantPosibles + 1;
					posiblesMov.push_back(movimientos[mov]);
				}
				else if (mundo[posX+movX][posY+movY].getTipoEntidad() == 2 && mundo[posX+movX][posY+movY].getMovimiento() == confirmacion){
					cantPosibles = cantPosibles + 1;
					posiblesMov.push_back(movimientos[mov]);
				}
			}
		}

		if (cantPosibles > 0){
			printf("gen: %d, PosX: %d, PosY: %d, posibilidades: %d\n", gen, posX, posY, cantPosibles);
			int desicion = (gen + posX + posY) % cantPosibles;
			movX = posiblesMov[desicion][0];
			movY = posiblesMov[desicion][1];
			if ((mundo[posX+movX][posY+movY].getTipoEntidad() == 2) && (mundo[posX+movX][posY+movY].getMovimiento() == confirmacion)){
				int j = 0;
				bool conejoEncontrado = false;
				while (j < conejos.size() && !conejoEncontrado){
					if ((conejos[j].getPosX() == posX + movX) && (conejos[j].getPosY() == posY + movY)){
						conejoEncontrado = true;
					}
					else{
						j = j + 1;
					}
				}
	
				int conejoEliminado;
				if (conejo->getGenProcRabbits() < conejos[j].getGenProcRabbits()){
					conejoEliminado = j;
					mundo[posX+movX][posY+movY].setGenProcRabbits(conejo->getGenProcRabbits() + 1);
                        		mundo[posX+movX][posY+movY].setTipoEntidad(2);
             		           	mundo[posX+movX][posY+movY].setPosX(posX + movX);
                        		mundo[posX+movX][posY+movY].setPosY(posY + movY);
                        		mundo[posX+movX][posY+movY].setMovimiento(!conejo->getMovimiento());
					conejos[i] = mundo[posX+movX][posY+movY];
				}
				else{
					conejoEliminado = i;
				}

				conejos.erase(conejos.begin() + conejoEliminado);

				if (conejoEliminado <= i){
					i = i - 1;
				}
			}
			else{
				mundo[posX+movX][posY+movY].setGenProcRabbits(conejo->getGenProcRabbits() + 1);
    				mundo[posX+movX][posY+movY].setTipoEntidad(2);
				mundo[posX+movX][posY+movY].setPosX(posX + movX);
		     		mundo[posX+movX][posY+movY].setPosY(posY + movY);
		     	 	mundo[posX+movX][posY+movY].setMovimiento(!conejo->getMovimiento());
				conejos[i] = mundo[posX+movX][posY+movY];
			}
			if (mundo[posX][posY].getGenProcRabbits() == GEN_PROC_RABBITS){
				mundo[posX][posY].setGenProcRabbits(0);
                                mundo[posX][posY].setTipoEntidad(2);
                                mundo[posX][posY].setPosX(posX);
                                mundo[posX][posY].setPosY(posY);
                                mundo[posX][posY].setMovimiento(!conejo->getMovimiento());
				conejos.push_back(mundo[posX][posY]);
			}
			else{
				mundo[posX][posY].setTipoEntidad(0);
			}
		}
	}

}
*/


void movimientosConejos(int gen) {
    	bool confirmacion = (gen % 2) == 0;
    	vector<vector<int>> movimientos = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

	vector<vector<Entidad>> nuevoMundo = mundo;
	vector<Entidad> nuevosConejos;

	for (int i = 0; i < conejos.size(); i++) {
        	Entidad conejo = conejos[i];
        	int posX = conejo.getPosX();
        	int posY = conejo.getPosY();
        	vector<vector<int>> posiblesMov;
        	int cantPosibles = 0;
        	int movX, movY;

        	
        	for (int mov = 0; mov < 4; mov++) {
        		movX = movimientos[mov][0];
            		movY = movimientos[mov][1];
            		int nx = posX + movX;
            		int ny = posY + movY;

            		if (nx >= 0 && nx < R && ny >= 0 && ny < C) {
                		int tipo = mundo[nx][ny].getTipoEntidad();
                		bool mov = mundo[nx][ny].getMovimiento();

                		if (tipo == 0 || (tipo == 2 && mov == confirmacion)) {
                			posiblesMov.push_back({movX, movY});
                    			cantPosibles++;
                		}
            		}
        	}

        	if (cantPosibles > 0) {
            		int decision = (gen + posX + posY) % cantPosibles;
            		movX = posiblesMov[decision][0];
            		movY = posiblesMov[decision][1];
            		int destX = posX + movX;
            		int destY = posY + movY;

            		if (mundo[destX][destY].getTipoEntidad() == 2 && mundo[destX][destY].getMovimiento() == confirmacion) {
                     		int j = 0;
                		while (j < conejos.size()) {
                			if (conejos[j].getPosX() == destX && conejos[j].getPosY() == destY) {
                        			break;
					}
                    			j++;
                		}

                		if (j < conejos.size() && conejo.getGenProcRabbits() < conejos[j].getGenProcRabbits()) {
                    			Entidad nuevoConejo(conejo.getGenProcRabbits() + 1, 0, 0, 2, destX, destY, !conejo.getMovimiento());
                    			nuevoMundo[destX][destY] = nuevoConejo;
                    			nuevosConejos.push_back(nuevoConejo);
                		}
            		}
			else {
                		Entidad nuevoConejo(conejo.getGenProcRabbits() + 1, 0, 0, 2, destX, destY, !conejo.getMovimiento());
                		nuevoMundo[destX][destY] = nuevoConejo;
                		nuevosConejos.push_back(nuevoConejo);
            		}

              		if (conejo.getGenProcRabbits() == GEN_PROC_RABBITS) {
                		Entidad hijo(0, 0, 0, 2, posX, posY, !conejo.getMovimiento());
                		nuevoMundo[posX][posY] = hijo;
                		nuevosConejos.push_back(hijo);
            		}
			else {
                		nuevoMundo[posX][posY] = Entidad();
            		}
        	} 
		else {
            		conejo.setMovimiento(conejo.getMovimiento());
            		nuevoMundo[posX][posY] = conejo;
            		nuevosConejos.push_back(conejo);
        	}
	}

    	mundo = nuevoMundo;
    	conejos = nuevosConejos;
}


void movimientosZorros(int gen){
	bool confirmacion = (gen % 2) == 0;
    	vector<vector<int>> movimientos = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
	vector<vector<Entidad>> nuevoMundo = mundo;
    	vector<Entidad> nuevosZorros;
	printf("%d\n", zorros[0].getTipoEntidad());
    	for (int i = 0; i < zorros.size(); i++) {
		printf("Entroooooo");
		Entidad zorro = zorros[i];
                int posX = zorro.getPosX();
                int posY = zorro.getPosY();
                vector<vector<int>> posiblesMov;
                int cantPosibles = 0;
                int movX, movY;
		bool comerConejos = false;

		if (zorro.getGenFoodFoxes() < gen){
			

			for (int mov = 0; mov < 4; mov++) {
				movX = movimientos[mov][0];
                	        movY = movimientos[mov][1];
                	        int nx = posX + movX;
                	        int ny = posY + movY;

                	        if (nx >= 0 && nx < R && ny >= 0 && ny < C) {
					int tipo = mundo[nx][ny].getTipoEntidad();
                       		        bool mov = mundo[nx][ny].getMovimiento();

                                	if (tipo == 0 || (tipo == 3 && mov == confirmacion) && !comerConejos) {
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
							cantPosibles = cantPosibles + 1;
						}
					}
				}
			}
			
			if (cantPosibles > 0) {
                	        int decision = (gen + posX + posY) % cantPosibles;
                	        movX = posiblesMov[decision][0];
                       		movY = posiblesMov[decision][1];
                        	int destX = posX + movX;
                        	int destY = posY + movY;

                        	if (mundo[destX][destY].getTipoEntidad() == 3 && mundo[destX][destY].getMovimiento() == confirmacion) {
                                	int j = 0;
                                	while (j < zorros.size()) {
                                	        if (zorros[j].getPosX() == destX && zorros[j].getPosY() == destY)
                                	        break;
                                	        j++;
                                	}

                                	if (j < zorros.size() && zorro.getGenProcFoxes() > zorros[j].getGenProcFoxes()) {
                                	        Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorro.getGenFoodFoxes() + 1, 3, destX, destY, !zorro.getMovimiento());
                                	        nuevoMundo[destX][destY] = nuevoZorro;
                                	        nuevosZorros.push_back(nuevoZorro);
                                	}
					else if (j < zorros.size() && zorro.getGenProcFoxes() == zorros[j].getGenProcFoxes() && zorro.getGenFoodFoxes() < zorros[j].getGenFoodFoxes()) {
						Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorro.getGenFoodFoxes() + 1, 3, destX, destY, !zorro.getMovimiento());
                                	        nuevoMundo[destX][destY] = nuevoZorro;
                                	        nuevosZorros.push_back(nuevoZorro);
					}
                        	}
				else {
					int tipo = mundo[destX][destY].getTipoEntidad();
					if (tipo == 2){
						int j = 0;
                                		while (j < conejos.size()) {
                                        		if (conejos[j].getPosX() == destX && conejos[j].getPosY() == destY) {
                                                		break;
                                        		}
                                        		j++;
                                		}
						conejos.erase(conejos.begin() + j);
					}

					Entidad nuevoZorro(0, zorro.getGenProcFoxes() + 1, zorro.getGenFoodFoxes() + 1, 3, destX, destY, !zorro.getMovimiento());
					nuevoMundo[destX][destY] = nuevoZorro;
                        	        nuevosZorros.push_back(nuevoZorro);
				}

		
			}
		}
		else {
			Entidad vacio(0, 0, 0, 0, posX, posY, false);
		}
	}
	mundo = nuevoMundo;
        zorros = nuevosZorros;
}


























