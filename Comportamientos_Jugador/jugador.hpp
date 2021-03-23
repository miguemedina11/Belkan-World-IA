#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      copiaDestino = destino;
      posBate = destino;
      posZapa = destino;
      posBiki = destino;
      hayplan=false;
      zapa=false;
      biki=false;
      vistaModificada=false;
      ultNoAvanza=true;
      cargando=false;
      zapaE = false;
      bikiE = false;
      batE = false;
      cercaBat = false;
      destPBate = destino;
      destPZapa = destino;
      destPBiki = destino;
      tiempo=3000;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      copiaDestino = destino;
      posBate = destino;
      posZapa = destino;
      posBiki = destino;
      hayplan=false;
      zapa=false;
      biki=false;
      vistaModificada=false;
      ultNoAvanza=true;
      cargando=false;
      zapaE = false;
      bikiE = false;
      batE = false;
      cercaBat = false;
      destPBate = destino;
      destPZapa = destino;
      destPBiki = destino;
      tiempo=3000;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula, tiempo;
    estado actual, destino, copiaDestino, posBate, posZapa, posBiki, destPBate, destPZapa, destPBiki;
    list<Action> plan;
    bool hayplan;
    bool zapa, biki, vistaModificada, ultNoAvanza, cargando, zapaE, bikiE, batE, cercaBat;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Costo_Uniforme(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Nivel2(const estado &origen, const estado &destino, list<Action> &plan);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);
    void rellenarVista(Sensores sensores);
};

#endif
