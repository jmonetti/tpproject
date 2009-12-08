
#ifndef COMMON_MODELOSERVIDOR_H_
#define COMMON_MODELOSERVIDOR_H_

#include <string>
#include <vector>
#include "persistencia/common_Persistencia.h"
#include "peticion/common_Peticion.h"
#include "simulacion/common_Simulador.h"

using namespace std;

class ModeloServidor {

public:

	ModeloServidor();

	virtual ~ModeloServidor();

	void guardar(Circuito* circuito);

	void recuperar();

	std::string generarRespuesta(std::string &ruta_pedido);

	bool* simular(int idCircuito,const std::string &nombreCircuito, bool* entradas); //TODO idCircuito

	int* calcularTiempoTransicion(int idCircuito,const std::string &nombreCircuito);

	int getId();

	int getIdCircuito(std::string &nombreCircuito);

	void guardarCircuito(DOMElement* funcion);

	std::string generarListaCircuitos();

	void recuperarDatosSimular(DOMNodeList* atributos, std::string &nombre, bool* entradas);

	void recuperarDatosTiempos(DOMNodeList* atributos, std::string &nombre, int* entradas);

private:

	std::vector<Circuito*> circuitos;

	Persistencia persistencia;
	Peticion peticion;
	Simulador simulador;
	int contadorId;

};

#endif /* COMMON_MODELOSERVIDOR_H_ */
