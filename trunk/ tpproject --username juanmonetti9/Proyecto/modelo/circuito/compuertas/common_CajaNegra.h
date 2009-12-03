
#ifndef COMMON_CAJANEGRA_H_
#define COMMON_CAJANEGRA_H_

#include "common_Compuerta.h"
#include "common_EntradaCompuerta.h"
#include "common_SalidaCompuerta.h"
#include <vector>
#include <string>
#include "../../publicacion/common_Publicacion.h"
#include "../../publicacion/common_Servidor.h"

using namespace std;

class CajaNegra: public Compuerta {

public:

	CajaNegra(int id,EntradaCompuerta** entradas, SalidaCompuerta** salidas,int cantidadEntradas,
int cantidadSalidas, Posicion posicion,SENTIDO sentido, const std::string &nombreCircuito,Servidor servidor);

	virtual ~CajaNegra();

	TIPO_COMPUERTA getTipo() const;

	EntradaCompuerta** getEntradas();

	SalidaCompuerta** getSalidas();

	int getCantidadEntradas();

	int getCantidadSalidas();

	void guardar(DOMDocument* doc, DOMNode* padre);

protected:

	void actualizarEntradas();
	void actualizarSalidas();

	void actuarSimular(bool* valores);
	void actuarTiempo(int* tiempos);

private:

	EntradaCompuerta** entradas;
	SalidaCompuerta** salidas;
	int cantidadEntradas;
	int cantidadSalidas;
	Publicacion publicacion;
	std::string nombreCircuito;
	Servidor servidor;

};

#endif /* COMMON_CAJANEGRA_H_ */
