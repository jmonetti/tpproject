#include <fstream>
#include <stdexcept>
#include "common_Publicacion.h"
#include <iostream>
#include <sstream>
#include "../../common/common_Utils.h"
#include <stdexcept>


void Publicacion::enviar(const std::string &nombreCircuito,Servidor servidor) {

	Persistencia persistencia;
	Circuito* circuito= persistencia.recuperarCircuito(0,nombreCircuito);

	std::string ruta = publicarCircuito(circuito);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	protocolo.desconectar();
}

bool* Publicacion::simular(const std::string &nombreCircuito,Servidor servidor,bool* entradas,int cantidad) {

	std::string ruta = generarPedido(nombreCircuito,cantidad,entradas);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetSimulacionResponse.xml");
	frespuesta << respuesta;
	frespuesta.close();
	//frespuesta.write(respuesta.c_str(),respuesta.size()); TODO
	ruta = "temp/GetSimulacionResponse.xml";
	bool* salidas = recuperarDatosSimular(ruta);

	protocolo.desconectar();

	return salidas;

}

int* Publicacion::calcularTiempoTransicion(const std::string &nombreCircuito,Servidor servidor,int* tiempos,int cantidad) {

	std::string ruta = generarPedido(nombreCircuito,cantidad,tiempos);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetTiempoSimulacionResponse.xml");
	frespuesta << respuesta;
	frespuesta.close();
	//frespuesta.write(respuesta.c_str(),respuesta.size()); TODO
	ruta = "temp/GetTiempoSimulacionResponse.xml";
	int* salidas_tiempos = recuperarDatosTiempos(ruta);

	protocolo.desconectar();

	return salidas_tiempos;

}

TamanioCajaNegra Publicacion::recibir(const std::string &nombreCircuito,Servidor servidor) {

	std::string ruta = generarPedido(nombreCircuito);

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetCircuitoResponse.xml");
	frespuesta << respuesta;
	frespuesta.close();
	//frespuesta.write(respuesta.c_str(),respuesta.size()); //TODO
	ruta = "temp/GetCircuitoResponse.xml";
	TamanioCajaNegra tamanio = recuperarDatosCajaNegra(ruta);

	protocolo.desconectar();

	return tamanio;

}

void Publicacion::obtenerCircuitos(Servidor servidor,std::vector<char*>* circuitos) {

	std::string ruta = generarPedido();

	conectar(servidor);
	enviarPedido(ruta);
	std::string respuesta = recibirRespuesta();
	ofstream frespuesta ("temp/GetListaCircuitosResponse.xml");
	frespuesta << respuesta;
	frespuesta.close();
	ruta = "temp/GetListaCircuitosResponse.xml";
	recuperarDatosCircuitos(ruta,circuitos);

	protocolo.desconectar();

}

std::string Publicacion::generarPedido (const std::string &nombreCircuito,int cantEntradas, bool* entradas) {

	XMLCh tempStr[100];

	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetSimulacion.xml";

	Persistencia::generarSOAP(impl,doc,ruta,Mensajes::GetSimular(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;

}

std::string Publicacion::generarPedido (const std::string &nombreCircuito,int cantEntradas, int* entradas) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetTiempoSimulacion.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetTiempoSimulacion(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;


}


std::string Publicacion::publicarCircuito(Circuito *circuito) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/PublicarCircuito.xml";

	Persistencia::generarSOAP(impl,doc,ruta, circuito->obtenerCircuito(doc));

	return ruta;

}

std::string Publicacion::generarPedido() {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetListaCircuitos.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetListaCircuitos(doc));

	return ruta;

}

std::string Publicacion::generarPedido(const std::string &nombreCircuito) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "temp/GetCircuito.xml";

	Persistencia::generarSOAP(impl,doc,ruta, Mensajes::GetCircuito(doc, nombreCircuito));

	return ruta;

}

void Publicacion::conectar(Servidor servidor) {

	protocolo.conectar(servidor);

}

void Publicacion::enviarPedido(const std::string &ruta) {

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
    converter << soap.size();
    lenght = converter.str();

	total += "Content-Length: " + lenght + "\n\n";
	total += soap;
	protocolo.enviarMensaje(total);

}

std::string Publicacion::recibirRespuesta() {

	std::string linea;
	std::string mensaje;
	protocolo.recibirMensaje(linea);
	/* Codigo de error: chequeo codigo de error q llega en la primera linea
	 * desde el caracter 8 al 13
	 */

	std::string codigoError = linea.substr(9,3);

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

bool* Publicacion::recuperarDatosSimular(const std::string &ruta) {

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

int* Publicacion::recuperarDatosTiempos(const std::string &ruta) {


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


TamanioCajaNegra Publicacion::recuperarDatosCajaNegra(const std::string &ruta) {

	Persistencia persistencia;

	std::string elemento = "GetCircuitoResponse";
	DOMElement* elem_conexiones = persistencia.getElemSOAP(ruta,elemento );

	DOMNodeList* lista_attr = elem_conexiones->getChildNodes();
	DOMNode* atributo = lista_attr->item(0);
	DOMElement* ElemCte = dynamic_cast < xercesc::DOMElement* > ( atributo );

	std::string aux = persistencia.recuperarDatoTexto(ElemCte);
	int cantEntradas = atoi(aux.c_str());

	atributo = lista_attr->item(1);
	ElemCte = dynamic_cast < xercesc::DOMElement* > ( atributo );

	aux = persistencia.recuperarDatoTexto(ElemCte);

	int cantSalidas = atoi(aux.c_str());

	TamanioCajaNegra tamanio(cantEntradas,cantSalidas);
	return tamanio;

}

void Publicacion::recuperarDatosCircuitos(const std::string &ruta,std::vector<char*>* circuitos) {

	Persistencia persistencia;

	std::string elemento = "GetListaCircuitosResponse";
	DOMElement* elem_lista = persistencia.getElemSOAP(ruta,elemento );

	DOMNodeList* lista_attr = elem_lista->getChildNodes();
	DOMNode* atributo;
	std::string nombre;
	DOMElement* ElemCte;
	unsigned int length = lista_attr->getLength();
	char* aux;

	for (unsigned int i = 0; i<length; ++i) {

		atributo = lista_attr->item(i);
		if( atributo->getNodeType() &&  // true is not NULL
		atributo->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		{
			ElemCte = dynamic_cast < xercesc::DOMElement* > ( atributo );

			nombre = persistencia.recuperarDatoTexto(ElemCte);

			aux= new char[nombre.size() + 1];
			aux[nombre.size()] = '\0';
			nombre.copy(aux,nombre.size());
			circuitos->push_back(aux);
		}
	}

}

