
#ifndef COMMON_PUBLICACION_H_
#define COMMON_PUBLICACION_H_

#include "common_Servidor.h"
#include <vector>
#include "../circuito/common_Circuito.h"
#include "../simulacion/common_ResultadoSimulacion.h"
#include "../../common/common_TamanioCajaNegra.h"
#include "common_Mensajes.h"

class Publicacion {

public:

	void enviar(const std::string &nombreCircuito,Servidor servidor);

	bool* simular(const std::string &nombreCircuito,Servidor servidor,bool* entradas,int cantidad);

	int* calcularTiempoTransicion(const std::string &nombreCircuito,Servidor servidor, int* tiempos,int cantidad);

	TamanioCajaNegra recibir(const std::string &nombreCircuito,Servidor servidor,Circuito* circuito);

	std::vector<char*>* obtenerCircuitos(Servidor servidor);

	static std::string generarPedido (std::string &nombreCircuito,int cantEntradas, bool* entradas);

	static std::string generarPedido (std::string &nombreCircuito,int cantEntradas,int* entradas);

	static std::string publicarCircuito(Circuito* circuito);

//	static std::string ()


};

#endif /* COMMON_PUBLICACION_H_ */
