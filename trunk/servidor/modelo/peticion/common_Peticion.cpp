#include <fstream>
#include <stdexcept>
#include "common_Peticion.h"


bool* Peticion::simular(const std::string &nombreCircuito,Servidor servidor,bool* entradas,int cantidad) {

	std::string ruta = generarPedido(nombreCircuito,cantidad,entradas);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetSimulacionResponse.xml");
	frespuesta.write(respuesta.c_str(),respuesta.size());
	ruta = "temp/GetSimulacionResponse.xml";
	bool* salidas = recuperarDatosSimular(ruta);

	protocolo.desconectar();

	return salidas;

}

int* Peticion::calcularTiempoTransicion(const std::string &nombreCircuito,Servidor servidor,int* tiempos,int cantidad) {

	std::string ruta = generarPedido(nombreCircuito,cantidad,tiempos);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetTiempoSimulacionResponse.xml");
	frespuesta.write(respuesta.c_str(),respuesta.size());
	ruta = "temp/GetTiempoSimulacionResponse.xml";
	int* salidas_tiempos = recuperarDatosTiempos(ruta);

	protocolo.desconectar();

	return salidas_tiempos;

}

void Peticion::conectar(Servidor servidor) {

	protocolo.conectar(servidor);

}


void Peticion::enviarPedido(const std::string &ruta) {

	ifstream fmensaje (ruta.c_str());
	std::string linea;

	std::string soap;
	std::string total;

	while (std::getline(fmensaje,linea)) {

		soap+=linea + "\n";

	}

	total += "GET HTTP/1.1\n";

	std::string lenght;
    std::stringstream converter;
    converter << total.size();
    lenght = converter.str();

	total += "Contenten-Length: " + lenght + "\n\n";
	total += soap;
	protocolo.enviarMensaje(total);

}

std::string Peticion::recibirRespuesta() {

	std::string linea;
	std::string mensaje;
	protocolo.recibirMensaje(linea);
	/* Codigo de error: chequeo codigo de error q llega en la primera linea
	 * desde el caracter 8 al 13
	 */
	std::string codigoError = linea.substr(8,3);
	protocolo.recibirMensaje(linea);
	std::string length = linea.substr(16);
	protocolo.recibirMensaje(linea);
	if(codigoError == "400") {
		throw runtime_error("Pedido Invalido");
	}
	if(codigoError == "200") {
		int longitud =  atoi(length.c_str());

		while(longitud >0) {
			protocolo.recibirMensaje(linea);
			mensaje += linea;
			longitud -= linea.size();
		}

		return mensaje;

	}
	throw runtime_error("Codigo de error HTML invalido - recibirRespuesta()");

}

std::string Peticion::generarPedido (const std::string &nombreCircuito,int cantEntradas, bool* entradas) {

	XMLCh tempStr[100];

	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetSimulacion.xml";

	Persistencia::generarSOAP(impl,doc,ruta,Mensajes::GetSimular(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;

}

std::string Peticion::generarPedido (const std::string &nombreCircuito,int cantEntradas, int* entradas) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetTiempoSimulacion.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetTiempoSimulacion(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;


}

bool* Peticion::recuperarDatosSimular(const std::string &ruta) {

	Persistencia persistencia;

	std::string elemento = "GetSimulacionResponse";
	DOMElement* elem_salidas = persistencia.getElemSOAP(ruta,elemento );

	DOMNodeList* lista_attr = elem_salidas->getChildNodes();
	DOMNode* atributo;
	bool* salidas = new bool[lista_attr->getLength()];
	std::string str_valor;
	bool  valor;
	DOMElement* ElemCte;
	for (unsigned int i = 0; i<lista_attr->getLength() ; ++i) {

		atributo = lista_attr->item(i);
		ElemCte = dynamic_cast < xercesc::DOMElement* > ( atributo );
		str_valor = persistencia.recuperarDatoTexto(ElemCte);
		if (str_valor == "0")
			valor = false;
		else
			valor = true;
		salidas[i] = valor;

	}

	return salidas;

}

int* Peticion::recuperarDatosTiempos(const std::string &ruta) {


	Persistencia persistencia;

	std::string elemento = "GetTiempoSimulacionResponse";
	DOMElement* elem_salidas = persistencia.getElemSOAP(ruta,elemento );

	DOMNodeList* lista_attr = elem_salidas->getChildNodes();
	DOMNode* atributo;
	int* salidas = new int[lista_attr->getLength()];
	std::string str_salida;
	int salida;
	DOMElement* ElemCte;
	for (unsigned int i = 0; i<lista_attr->getLength(); ++i) {

		atributo = lista_attr->item(i);
		ElemCte = dynamic_cast < xercesc::DOMElement* > ( atributo );
		str_salida = persistencia.recuperarDatoTexto(ElemCte);
		salida = atoi(str_salida.c_str());
		salidas[i] = salida;

	}

	return salidas;

}


std::string Peticion::generarRespuesta (int cantSalidas, bool* salidas) {

	XMLCh tempStr[100];

	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetSimulacionResponse.xml";

	Persistencia::generarSOAP(impl,doc,ruta,Mensajes::GetSimularResponse(doc,cantSalidas, salidas));

	return ruta;

}

std::string Peticion::generarRespuesta (int cantSalidas, int* salidas) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetTiempoSimulacionResponse.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetTiempoSimulacionResponse(doc,cantSalidas, salidas));

	return ruta;

}



std::string Peticion::generarListaCircuitos(std::vector<Circuito*> circuitos) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetListaCircuitosResponse.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetListaCircuitosResponse(doc,circuitos));

	return ruta;

}

std::string Peticion::generarRespuesta(int cantEntradas, int cantSalidas) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetCircuitoResponse.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetCircuitoResponse(doc,cantEntradas, cantSalidas));

	return ruta;

}


