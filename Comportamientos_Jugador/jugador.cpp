#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

// Este es el mÃƒÂ©todo principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la prÃƒÂ¡ctica. Tiene como entrada la informaciÃƒÂ³n de los
// sensores y devuelve la acciÃƒÂ³n a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actIDLE;
	// Estoy en el nivel 1

	if (sensores.nivel != 4){

        if (!hayplan){

            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            cout << "Fila: " << actual.fila << endl;
            cout << "Col : " << actual.columna << endl;
            cout << "Ori : " << actual.orientacion << endl;

            destino.fila       = sensores.destinoF;
            destino.columna    = sensores.destinoC;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);
        }

		if (hayplan and plan.size()>0){
            accion = plan.front();
            plan.erase(plan.begin());
		}
	}
	else {

        if (!hayplan or !(plan.size()>0)){

            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            cout << "Fila: " << actual.fila << endl;
            cout << "Col : " << actual.columna << endl;
            cout << "Ori : " << actual.orientacion << endl;

            destino.fila       = sensores.destinoF;
            destino.columna    = sensores.destinoC;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);
        }

        mapaResultado[sensores.posF][sensores.posC] = sensores.terreno[0];
        rellenarVista(sensores);

        //PONER MAS OBJETIVOS
        if (copiaDestino.fila!=sensores.destinoF or copiaDestino.columna!=sensores.destinoC){
            if(biki and bikiE){
                destino.fila=destPBiki.fila;
                destino.columna=destPBiki.columna;
                destino.orientacion=destPBiki.orientacion;
                bikiE=false;
            }
            else{
                copiaDestino.fila = sensores.destinoF;
                copiaDestino.columna = sensores.destinoC;
                hayplan = false;
                ultNoAvanza = false;
            }
            if(zapa and zapaE){
                destino.fila=destPZapa.fila;
                destino.columna=destPZapa.columna;
                destino.orientacion=destPZapa.orientacion;
                zapaE=false;
            }
            else{
                copiaDestino.fila = sensores.destinoF;
                copiaDestino.columna = sensores.destinoC;
                hayplan = false;
                ultNoAvanza = false;
            }
        }


        if (sensores.terreno[0]=='K'){
            biki=true;
        }
        if (sensores.terreno[0]=='D'){
            zapa=true;
        }
        if (sensores.superficie[2]=='a' and plan.front() == actFORWARD){
            plan.push_front(actIDLE);
        }

        char terrenoDel = sensores.terreno[2];

        if ((terrenoDel == 'P' or terrenoDel == 'M') and plan.front()==actFORWARD){
            hayplan = false;
        }

        if (!biki and bikiE){
            destPBiki.fila=destino.fila;
            destPBiki.columna=destino.columna;
            destPBiki.orientacion=destino.orientacion;

            destino.fila=posBiki.fila;
            destino.columna=posBiki.columna;


        }

        if (!zapa and zapaE){
            destPZapa.fila=destino.fila;
            destPZapa.columna=destino.columna;
            destPZapa.orientacion=destino.orientacion;

            destino.fila=posZapa.fila;
            destino.columna=posZapa.columna;
        }

        if ((zapaE and !zapa) or (bikiE and !biki)){
            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);
        }





    if ((terrenoDel=='A' and plan.front()==actFORWARD and vistaModificada and !biki) or (terrenoDel=='A' and plan.front()==actFORWARD and vistaModificada and plan.size()<95) or
        (terrenoDel=='B' and plan.front()==actFORWARD and vistaModificada and !zapa) or (terrenoDel=='B' and plan.front()==actFORWARD and vistaModificada and plan.size()<95) or
        (terrenoDel=='A' and plan.front()==actFORWARD and !vistaModificada and ultNoAvanza and plan.front()==actFORWARD and !biki) or
        (terrenoDel=='B' and plan.front()==actFORWARD and !vistaModificada and ultNoAvanza and plan.front()==actFORWARD and !zapa)){

            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);
        }





        /*if ((((((terrenoDel=='A') or (terrenoDel=='B')) and plan.front()==actFORWARD)and vistaModificada) or ((((terrenoDel=='A') or (terrenoDel=='B')) and plan.front()==actFORWARD)) and !vistaModificada and ultNoAvanza and plan.front()==actFORWARD) or ){
            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);
        }*/

        bool auxbool = (tiempo>600 and sensores.bateria<1000 and cercaBat);

        if ((batE and !cargando and plan.size()>21) and (auxbool      or        (tiempo>400 and sensores.bateria<600))){
            destPBate.fila=destino.fila;
            destPBate.columna=destino.columna;
            destPBate.orientacion=destino.orientacion;

            destino.fila=posBate.fila;
            destino.columna=posBate.columna;

            actual.fila        = sensores.posF;
            actual.columna     = sensores.posC;
            actual.orientacion = sensores.sentido;

            hayplan = pathFinding (sensores.nivel, actual, destino, plan);

            cargando=true;
        }
        if (cargando and sensores.terreno[0]=='X'){
            if (tiempo>900){
                if(sensores.bateria<2000){
                    plan.push_front(actIDLE);
                }
                else{
                    destino.fila=destPBate.fila;
                    destino.columna=destPBate.columna;
                    destino.orientacion=destPBate.orientacion;
                    cargando=false;
                }
            }
            else if(tiempo>400){
                if(sensores.bateria<1000){
                    plan.push_front(actIDLE);
                }
                else{
                    destino.fila=destPBate.fila;
                    destino.columna=destPBate.columna;
                    destino.orientacion=destPBate.orientacion;
                    cargando=false;
                }
            }
            else{
                if(sensores.bateria<400){
                    plan.push_front(actIDLE);
                }
                else{
                    destino.fila=destPBate.fila;
                    destino.columna=destPBate.columna;
                    destino.orientacion=destPBate.orientacion;
                    cargando=false;
                }
            }

        }


        if (plan.front() == actFORWARD){
            ultNoAvanza = false;
        }
        else{
            ultNoAvanza = true;
        }

        //HACE COSAS
        if (hayplan and plan.size()>0){
            accion = plan.front();
            plan.erase(plan.begin());

        }
    }

    tiempo--;

  return accion;
}


// Llama al algoritmo de busqueda que se usarÃƒÂ¡ en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			      return pathFinding_Anchura(origen,destino,plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
                  return pathFinding_Costo_Uniforme(origen,destino,plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
                  return pathFinding_Nivel2(origen,destino,plan);
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- ImplementaciÃƒÂ³n de la busqueda en profundidad ---------------------------

// Dado el cÃƒÂ³digo en carÃƒÂ¡cter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posiciÃƒÂ³n de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parÃƒÂ¡metro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}




struct nodo{
	estado st;
	list<Action> secuencia;
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// ImplementaciÃƒÂ³n de la bÃƒÂºsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}



// Sacar por la tÃƒÂ©rminal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}


void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}


//********************************************************************************************************************
//                              BUSQUEDA HORIZONTAL
//********************************************************************************************************************

// ImplementaciÃƒÂ³n de la bÃƒÂºsqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan metodo Anchura\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	list<nodo> lista;					// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	lista.push_back(current);

  while (!lista.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		lista.pop_front();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			lista.push_back(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			lista.push_back(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				lista.push_back(hijoForward);
			}
		}

		// Tomo el siguiente valor de la lista
		if (!lista.empty()){
			current = lista.front();

			while(generados.find(current.st) != generados.end()){
                lista.pop_front();
                current = lista.front();
            }
		}
	}

  cout << "Terminada la busqueda Anchura\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}


//********************************************************************************************************************
//                              BUSQUEDA DE COSTO UNIFORME
//********************************************************************************************************************

struct nodoCoste{
	estado st;
	list<Action> secuencia;
	unsigned int coste;
	int equip;  //NO tienes nada = 0 ; Tienes Zapatillas = 1 ; Tienes Bikini = 2 ; Tienes Zapatillas y Biquini = 3
};

bool operator<(const nodoCoste &a, const nodoCoste &n){
		return a.coste > n.coste;
}

struct ComparaEstadosCostes{
	bool operator()(const nodoCoste &a, const nodoCoste &n) const{
		if ((a.st.fila > n.st.fila) or
            (a.st.fila == n.st.fila and a.st.columna > n.st.columna) or
            (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion > n.st.orientacion) or
            (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.coste > n.coste) or
            (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.coste == n.coste and a.equip > n.equip))
			return true;
		else
			return false;
	}
};


int devolverCoste(unsigned char casilla, int &equip){

    int coste=1;

    switch (casilla){
    case 'A':
        if (equip ==2 or equip==3){
            coste = 10;
        }
        else{
            coste = 100;
        }
        break;
    case 'B':
        if (equip == 1 or equip == 3){
            coste = 5;
        }
        else{
            coste = 50;
        }
        break;
    case 'T':
        coste = 2;
        break;
    case '?':
        /*
        if (equip==1 or equip==3)//Tengo Zapatillas
            coste = 4;
        else if (equip==2) //Tengo bikini
            coste = 6;
        else   //equip=0
            coste = 22;*/
        if (equip == 3)
            coste = 4;
        else
            coste=2;
        break;
    case 'K':
        coste = 1;
        if (equip==1 or equip==3)
            equip=3;
        else
            equip=2;
        break;
    case 'D':
        coste = 1;
        if (equip==2 or equip==3)
            equip=3;
        else
            equip=1;
        break;
    default:
        coste = 1;
        break;
    }

    return coste;
}

// ImplementaciÃƒÂ³n de la bÃƒÂºsqueda de costo uniforme.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Costo_Uniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan metodo Busqueda Costo Uniforme\n";
	plan.clear();
	set<nodoCoste,ComparaEstadosCostes> generados; // Lista de Cerrados
	priority_queue<nodoCoste> abiertos;		// Lista de Abiertos

  nodoCoste current;
	current.st = origen;
	current.secuencia.empty();
	current.coste = 0;

	abiertos.push(current);

  while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.pop();
		generados.insert(current);

		int costeAux =devolverCoste(mapaResultado[current.st.fila][current.st.columna],current.equip);

		// Generar descendiente de girar a la derecha
		nodoCoste hijoTurnR = current;
		hijoTurnR.coste += costeAux;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			abiertos.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodoCoste hijoTurnL = current;
		hijoTurnL.coste += costeAux;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodoCoste hijoForward = current;

		if (!HayObstaculoDelante(hijoForward.st)){
                hijoForward.coste += costeAux;
			if (generados.find(hijoForward) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la lista
		if (!abiertos.empty()){
			current = abiertos.top();

			while(generados.find(current) != generados.end()){
                abiertos.pop();
                current = abiertos.top();
            }
		}
	}

  cout << "Terminada la busqueda de costo uniforme\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan: " << current.coste << endl;

		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////               NIVEL 2            ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ComparaNodos{
	bool operator()(const nodoCoste &a, const nodoCoste &n) const{
		if ((a.st.fila > n.st.fila) or
            (a.st.fila == n.st.fila and a.st.columna > n.st.columna) or
            (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion > n.st.orientacion))
			return true;
		else
			return false;
	}
};

bool ComportamientoJugador::pathFinding_Nivel2(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan metodo Busqueda Costo Uniforme\n";
	plan.clear();
	set<nodoCoste,ComparaNodos> generados; // Lista de Cerrados
	priority_queue<nodoCoste> abiertos;		// Lista de Abiertos

  nodoCoste current;
	current.st = origen;
	current.secuencia.empty();
	current.coste = 0;
	if (biki){
        if(zapa)
            current.equip=3;
        else
            current.equip=2;
	}
	else{
        if(zapa)
            current.equip=1;
        else
            current.equip=0;
	}

	abiertos.push(current);

  while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.pop();
		generados.insert(current);

		int costeAux =devolverCoste(mapaResultado[current.st.fila][current.st.columna],current.equip);

		// Generar descendiente de girar a la derecha
		nodoCoste hijoTurnR = current;
		hijoTurnR.coste += costeAux;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			abiertos.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodoCoste hijoTurnL = current;
		hijoTurnL.coste += costeAux;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodoCoste hijoForward = current;

		if (!HayObstaculoDelante(hijoForward.st)){
                hijoForward.coste += costeAux;
			if (generados.find(hijoForward) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la lista
		if (!abiertos.empty()){
			current = abiertos.top();

			while(generados.find(current) != generados.end()){
                abiertos.pop();
                current = abiertos.top();
            }
		}
	}

  cout << "Terminada la busqueda de costo uniforme\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}

void ComportamientoJugador::rellenarVista(Sensores sensores){
    char aux, faux, caux;
    vistaModificada=false;
    cercaBat = false;
    switch (sensores.sentido){
        case norte:
            for(int i=-1;i<=1;i++){
                aux = mapaResultado[sensores.posF-1][sensores.posC+i];
                faux = sensores.posF-1;
                caux = sensores.posC+i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+2];
                    vistaModificada = true;
                    if(sensores.terreno[i+2] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+2] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-2;i<=2;i++){
                aux = mapaResultado[sensores.posF-2][sensores.posC+i];
                faux = sensores.posF-2;
                caux = sensores.posC+i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+6];
                    vistaModificada = true;
                    if(sensores.terreno[i+6] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+6] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-3;i<=3;i++){
                aux = mapaResultado[sensores.posF-3][sensores.posC+i];
                faux = sensores.posF-3;
                caux = sensores.posC+i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+12];
                    vistaModificada = true;
                    if(sensores.terreno[i+12] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+12] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
        case este:
            for(int i=-1;i<=1;i++){
                aux = mapaResultado[sensores.posF+i][sensores.posC+1];
                faux = sensores.posF+i;
                caux = sensores.posC+1;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+2];
                    vistaModificada = true;
                    if(sensores.terreno[i+2] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+2] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-2;i<=2;i++){
                aux = mapaResultado[sensores.posF+i][sensores.posC+2];
                faux = sensores.posF+i;
                caux = sensores.posC+2;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+6];
                    vistaModificada = true;
                    if(sensores.terreno[i+6] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+6] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-3;i<=3;i++){
                aux = mapaResultado[sensores.posF+i][sensores.posC+3];
                faux = sensores.posF+i;
                caux = sensores.posC+3;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+12];
                    vistaModificada = true;
                    if(sensores.terreno[i+12] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+12] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            break;
        case sur:
            for(int i=-1;i<=1;i++){
                aux = mapaResultado[sensores.posF+1][sensores.posC-i];
                faux = sensores.posF+1;
                caux = sensores.posC-i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+2];
                    vistaModificada = true;
                    if(sensores.terreno[i+2] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+2] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-2;i<=2;i++){
                aux = mapaResultado[sensores.posF+2][sensores.posC-i];
                faux = sensores.posF+2;
                caux = sensores.posC-i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+6];
                    vistaModificada = true;
                    if(sensores.terreno[i+6] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+6] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-3;i<=3;i++){
                aux = mapaResultado[sensores.posF+3][sensores.posC-i];
                faux = sensores.posF+3;
                caux = sensores.posC-i;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+12];
                    vistaModificada = true;
                    if(sensores.terreno[i+12] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+12] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            break;
        case oeste:
            for(int i=-1;i<=1;i++){
                aux = mapaResultado[sensores.posF-i][sensores.posC-1];
                faux = sensores.posF-i;
                caux = sensores.posC-1;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+2];
                    vistaModificada = true;
                    if(sensores.terreno[i+2] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+2] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-2;i<=2;i++){
                aux = mapaResultado[sensores.posF-i][sensores.posC-2];
                faux = sensores.posF-i;
                caux = sensores.posC-2;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+6];
                    vistaModificada = true;
                    if(sensores.terreno[i+6] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+6] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
            }
            for(int i=-3;i<=3;i++){
                aux = mapaResultado[sensores.posF-i][sensores.posC-3];
                faux = sensores.posF-i;
                caux = sensores.posC-3;
                switch (aux){
                case '?':
                    mapaResultado[faux][caux] = sensores.terreno[i+12];
                    vistaModificada = true;
                    if(sensores.terreno[i+12] == 'K'){
                        posBiki.fila=faux;
                        posBiki.columna=caux;
                        if (!biki){
                            bikiE=true;
                        }
                    }
                    if(sensores.terreno[i+12] == 'D'){
                        posZapa.fila=faux;
                        posZapa.columna=caux;
                        if (!zapa){
                            zapaE=true;
                        }
                    }
                    break;
                case 'X':
                    posBate.fila=faux;
                    posBate.columna=caux;
                    batE=true;
                    cercaBat=true;
                    break;
                }
        }
    }
}
