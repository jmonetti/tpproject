
#ifndef COMMON_SALIDA_H_
#define COMMON_SALIDA_H_

#include "common_EntradaCompuerta.h"
#include <string>

class Compuerta;

class Salida: public Compuerta {

public:

	Salida(int id, EntradaCompuerta* entrada,Posicion posicion,SENTIDO sentido,const std::string &nombre);

	virtual ~Salida();

	Salida* getSalida();

	bool getValor() const;

	int getTiempoTransicion() const;

	std::string getNombre() const;

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

	EntradaCompuerta* entrada;
	bool valor;
	int tiempoTransicion;
	std::string nombre;

};

#endif /* COMMON_SALIDA_H_ */
