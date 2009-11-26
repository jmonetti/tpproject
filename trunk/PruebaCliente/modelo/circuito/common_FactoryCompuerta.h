
#ifndef COMMON_FACTORYCOMPUERTA_H_
#define COMMON_FACTORYCOMPUERTA_H_

#include "compuertas/common_Compuerta.h"
#include "common_Circuito.h"
#include "../../common/constantes.h"

class FactoryCompuerta {

public:

	static void crearCompuerta(TIPO_COMPUERTA tipo, Circuito &circuito, int tiempoTransicion = 0);

private:

	static void crearAND(Circuito &circuito,int tiempo);
	static void crearOR(Circuito &circuito,int tiempo);
	static void crearNOT(Circuito &circuito,int tiempo);
	static void crearXOR(Circuito &circuito,int tiempo);
	static void crearPISTA(Circuito &circuito);
	static void crearENTRADA(Circuito &circuito);
	static void crearSALIDA(Circuito &circuito);

};

#endif /* COMMON_FACTORYCOMPUERTA_H_ */