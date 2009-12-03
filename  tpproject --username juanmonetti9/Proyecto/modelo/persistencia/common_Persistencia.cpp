
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "../circuito/common_Circuito.h"
#include "../circuito/common_FactoryCompuerta.h"
#include "../publicacion/common_Servidor.h"
#include "common_Mensajes.h"

#include "common_Persistencia.h"

Persistencia::Persistencia() {

	try
	{
		XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
	}
	catch( XMLException& e )
	{
		std::string mensaje = XMLString::transcode( e.getMessage() );
		throw runtime_error("XML toolkit initialization error: " + mensaje);
	}

	TAG_INITIALIZER = XMLString::transcode("XML");
	TAG_CIRCUITO = XMLString::transcode("Circuito");

	TAG_SALIDA= XMLString::transcode("Salida");
	TAG_ENTRADA= XMLString::transcode("Entrada");
	TAG_NOT= XMLString::transcode("NOT");
	TAG_AND= XMLString::transcode("AND");
	TAG_OR= XMLString::transcode("OR");
	TAG_XOR= XMLString::transcode("XOR");
	TAG_PISTA= XMLString::transcode("Pista");



}

Persistencia::~Persistencia() {

	try
	{
	  XMLString::release( &TAG_INITIALIZER);
	  XMLString::release( &TAG_CIRCUITO);
	  XMLString::release( &TAG_SALIDA );
	  XMLString::release( &TAG_ENTRADA );
	  XMLString::release( &TAG_NOT );
	  XMLString::release( &TAG_AND );
	  XMLString::release( &TAG_OR );
	  XMLString::release( &TAG_XOR );
	  XMLString::release( &TAG_PISTA );
	}
	catch( ... )
	{
	  throw runtime_error("Unknown exception encountered in TagNamesdtor");
	}

	try
	{
		XMLPlatformUtils::Terminate();  // Terminate Xerces
	}
	catch( xercesc::XMLException& e )
	{
		std::string message = xercesc::XMLString::transcode( e.getMessage() );
		throw runtime_error("XML ttolkit teardown error: " + message);
	}

}

void Persistencia::guardar(const Circuito &circuito) {

	XMLCh tempStr[100];
	XMLString::transcode("XML", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument*   doc = impl->createDocument(0, TAG_INITIALIZER, 0);

	circuito.guardar(doc);

    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();

    // optionally you can set some features on this serializer
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
        theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
         theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

    XMLFormatTarget *myFormatTarget = new LocalFileFormatTarget(circuito.getNombre().c_str());

    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormatTarget);

    DOMNode* nodo = dynamic_cast < xercesc::DOMNode* >( doc );

    try {
        theSerializer->write(nodo, theOutput);
    }
    catch (const XMLException& toCatch) {
        std::string message = XMLString::transcode(toCatch.getMessage());
        throw runtime_error("Exception message is: \n" + message);
    }
    catch (const DOMException& toCatch) {
        std::string message = XMLString::transcode(toCatch.msg);
        throw runtime_error("Exception message is: \n" + message);
    }
    catch (...) {
        throw runtime_error("Unexpected Exception");
    }

    doc->release();
    theOutput->release();
    theSerializer->release();
    delete myFormatTarget;

}

void Persistencia::generarSOAP(DOMImplementation *impl,DOMDocument* doc,std::string &ruta, DOMElement* datos) {

	XMLCh nombre[100];
	XMLCh valor[100];

	XMLString::transcode("soap:Envelope",nombre,99);
	DOMElement* envelope = doc->createElement(nombre);

	/** xmlns:soap **/
    XMLString::transcode("xmlns:soap", nombre, 99);

    std::string aux = "http://www.w3.org/2001/12/soap-envelope";

    XMLString::transcode(aux.c_str(),valor,99);

    envelope->setAttribute(nombre,valor);

	XMLString::transcode("soap:Body",nombre,99);
	DOMElement* body = doc->createElement(nombre);

    body->appendChild(datos);

	envelope->appendChild(body);

	doc->appendChild(envelope);

    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();

    // optionally you can set some features on this serializer
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
        theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
         theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

    XMLFormatTarget *myFormatTarget = new LocalFileFormatTarget(ruta.c_str());

    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormatTarget);

    DOMNode* nodo = dynamic_cast < xercesc::DOMNode* >( doc );

    try {
        theSerializer->write(nodo, theOutput);
    }
    catch (const XMLException& toCatch) {
        std::string message = XMLString::transcode(toCatch.getMessage());
        throw runtime_error("Exception message is: \n" + message);
    }
    catch (const DOMException& toCatch) {
        std::string message = XMLString::transcode(toCatch.msg);
        throw runtime_error("Exception message is: \n" + message);
    }
    catch (...) {
        throw runtime_error("Unexpected Exception");
    }

    doc->release();
    theOutput->release();
    theSerializer->release();
    delete myFormatTarget;

}


std::string Persistencia::generarPedido (std::string &nombreCircuito,int cantEntradas, bool* entradas) {

	XMLCh tempStr[100];

	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "GetSimulacion.xml";

	generarSOAP(impl,doc,ruta,Mensajes::GetSimular(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;

}

std::string Persistencia::generarPedido (std::string &nombreCircuito,int cantEntradas, int* entradas) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "GetTiempoSimulacion.xml";

	generarSOAP(impl,doc,ruta, Mensajes::GetTiempoSimulacion(doc,nombreCircuito,cantEntradas, entradas));

	return ruta;


}

std::string Persistencia::publicarCircuito(Circuito *circuito) {

	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);
	DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

	DOMDocument* doc = impl->createDocument();

	std::string ruta = "PublicarCircuito.xml";

	generarSOAP(impl,doc,ruta, Mensajes::PublicarCircuito(doc, circuito));

	return ruta;

}


Circuito* Persistencia::recuperar(int idCircuito, const std::string &nombreCircuito) {

	   struct stat estadoArchivo;

	   int iretStat = stat(nombreCircuito.c_str(), &estadoArchivo);

	   if( iretStat == ENOENT )
	      throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
	   else if( iretStat == ENOTDIR )
	      throw ( std::runtime_error("A component of the path is not a directory."));
	   else if( iretStat == ELOOP )
	      throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
	   else if( iretStat == EACCES )
	      throw ( std::runtime_error("Permission denied."));
	   else if( iretStat == ENAMETOOLONG )
	      throw ( std::runtime_error("File can not be read\n"));

	   // Configure DOM parser.
	   xercesc::XercesDOMParser *m_ConfigFileParser = new XercesDOMParser;

	   m_ConfigFileParser->setValidationScheme( XercesDOMParser::Val_Never );
	   m_ConfigFileParser->setDoNamespaces( false );
	   m_ConfigFileParser->setDoSchema( false );
	   m_ConfigFileParser->setLoadExternalDTD( false );

	   try
	   {
		   m_ConfigFileParser->parse( nombreCircuito.c_str() );

		   // no need to free this pointer - owned by the parent parser object
		   DOMDocument* xmlDoc = m_ConfigFileParser->getDocument();

		   // Get the top-level element: NAme is "root". No attributes for "root"

		   DOMElement* elementRoot = xmlDoc->getDocumentElement();
		   if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

		   DOMNodeList*      hijo = elementRoot->getChildNodes();

		   DOMNode* circuito = hijo->item(1);
		   if( circuito->getNodeType() &&  // true is not NULL
			 circuito->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		   {
				DOMElement* ElementoCte = dynamic_cast< xercesc::DOMElement* >( circuito );
				if( XMLString::equals(ElementoCte->getTagName(), TAG_CIRCUITO))
				{
				   return parserCircuito(ElementoCte, idCircuito,nombreCircuito);
				}
		   }
	   }
	   catch( xercesc::XMLException& e )
	   {
	      std::string message = xercesc::XMLString::transcode( e.getMessage() );
	      throw runtime_error("Error parsing file: "+ message);
	   }

	   return NULL;

}

Circuito* Persistencia::parserCircuito(DOMElement* ElementoCte, int idCircuito, const std::string &nombreCircuito) {

	Circuito* circuito = new Circuito(idCircuito, nombreCircuito);

	DOMNodeList*      salidas = ElementoCte->getChildNodes();
	const  XMLSize_t cantSalidas = salidas->getLength();

	// Para todos los nodos, hijos de "root" en el arbol XML.

	for( XMLSize_t i = 0; i < cantSalidas; ++i )
	{
	   DOMNode* SalidaActual = salidas->item(i);
	   if( SalidaActual->getNodeType() &&  // true is not NULL
			   SalidaActual->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
	   {
		   // Found node which is an Element. Re-cast node as element
		   DOMElement* ElementoCte = dynamic_cast< xercesc::DOMElement* >( SalidaActual );

		   if(XMLString::equals(ElementoCte->getTagName(), TAG_ENTRADA))
		   {
		      parserEntrada(ElementoCte, circuito);

		   }
		   else
		   {
			   if(XMLString::equals(ElementoCte->getTagName(), TAG_NOT)){

				   parserNOT(ElementoCte, circuito);

				}
			   else
			   {
				   if(XMLString::equals(ElementoCte->getTagName(), TAG_AND)){

				   parserAND(ElementoCte, circuito);

				   }
				   else
				   {
					   if(XMLString::equals(ElementoCte->getTagName(), TAG_OR)){

						   parserOR(ElementoCte, circuito);

					   }
					   else
					   {
							if(XMLString::equals(ElementoCte->getTagName(), TAG_XOR)){

							   parserXOR(ElementoCte, circuito);

							}
							else
							{
								if(XMLString::equals(ElementoCte->getTagName(), TAG_PISTA)){

									parserPista(ElementoCte, circuito);

								}
								else
								{
									if( XMLString::equals(ElementoCte->getTagName(), TAG_SALIDA))
									{
										parserSalida(ElementoCte, circuito);

									}
								}
							}
					   }
				   }
			   }
		   }
	   }
	}

	return circuito;

}

void Persistencia::parserSalida(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre="x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre="y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre="sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	XMLCh* ATTR_NOMBRE = XMLString::transcode("nombre");
	DOMAttr* attr_nombre = ElementoCte->getAttributeNode(ATTR_NOMBRE);
	nombre = XMLString::transcode(attr_nombre->getValue());

	Posicion posicion(x,y);

	FactoryCompuerta::crearSalida(*circuito, posicion,nombre,(SENTIDO)sentido);

}

void Persistencia::parserEntrada(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	XMLCh* ATTR_NOMBRE = XMLString::transcode("nombre");
	DOMAttr* attr_nombre = ElementoCte->getAttributeNode(ATTR_NOMBRE);
	nombre = XMLString::transcode(attr_nombre->getValue());

	Posicion posicion(x,y);

	FactoryCompuerta::crearEntrada(*circuito,posicion,nombre,(SENTIDO)sentido);

}

void Persistencia::parserNOT(DOMElement* ElementoCte, Circuito* circuito){

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	FactoryCompuerta::crearCompuerta(T_NOT,*circuito,posicion,(SENTIDO)sentido);

}

void Persistencia::parserAND(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	FactoryCompuerta::crearCompuerta(T_AND,*circuito,posicion,(SENTIDO)sentido);

}

void Persistencia::parserOR(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	FactoryCompuerta::crearCompuerta(T_OR,*circuito,posicion,(SENTIDO)sentido);

}

void Persistencia::parserXOR(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	FactoryCompuerta::crearCompuerta(T_XOR,*circuito,posicion,(SENTIDO)sentido);

}

void Persistencia::parserPista(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	FactoryCompuerta::crearCompuerta(T_PISTA,*circuito,posicion,(SENTIDO)sentido);

}


void Persistencia::parserCajaNegra(DOMElement* ElementoCte, Circuito* circuito) {

	std::string nombre = "entradas";
	int entradas = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "salidas";
	int salidas = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "x";
	int x = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "y";
	int y = Persistencia::recuperarDato(ElementoCte,nombre);

	nombre = "sentido";
	int sentido = Persistencia::recuperarDato(ElementoCte,nombre);

	std::string nombreCircuito;
	XMLCh* ATTR_NOMBRE = XMLString::transcode("nombre");
	DOMAttr* attr_nombre = ElementoCte->getAttributeNode(ATTR_NOMBRE);
	nombreCircuito = XMLString::transcode(attr_nombre->getValue());

	std::string host;
	XMLCh* ATTR_HOST = XMLString::transcode("host");
	DOMAttr* attr_host = ElementoCte->getAttributeNode(ATTR_HOST);
	host = XMLString::transcode(attr_host->getValue());

	nombre= "puerto";
	int puerto= Persistencia::recuperarDato(ElementoCte,nombre);

	Posicion posicion(x,y);

	Servidor servidor(host,puerto);

	FactoryCompuerta::crearCajaNegra(*circuito,posicion,nombreCircuito,(SENTIDO)sentido,servidor,entradas,salidas);

}


void Persistencia::guardarElemento(DOMDocument* doc, DOMElement* elem,std::string &nombre,int valor) {

	XMLCh tempStr[30];
	std::string aux;

    XMLString::transcode(nombre.c_str(), tempStr, 29);
    DOMAttr* atributo = doc->createAttribute(tempStr);

    std::stringstream converter;
    converter << valor;
    aux = converter.str();

    XMLString::transcode(aux.c_str(),tempStr,29);
    atributo->setNodeValue(tempStr);
    elem->setAttributeNode(atributo);

}

void Persistencia::guardarElemento(DOMDocument* doc, DOMElement* elem,std::string &nombre,std::string &valor) {

	XMLCh tempStr[30];

    XMLString::transcode(nombre.c_str(), tempStr, 29);
    DOMAttr* atributo = doc->createAttribute(tempStr);

    XMLString::transcode(valor.c_str(),tempStr,29);
    atributo->setNodeValue(tempStr);
    elem->setAttributeNode(atributo);
}

void Persistencia::guardarElementoTexto(DOMDocument* doc, DOMElement* elem,std::string &nombre,int valor) {

	XMLCh tempStr[30];
	std::string aux;

    XMLString::transcode(nombre.c_str(), tempStr, 29);
    DOMElement* atributo = doc->createElement(tempStr);

    std::stringstream converter;
    converter << valor;
    aux = converter.str();

    XMLString::transcode(aux.c_str(),tempStr,99);
    atributo->setTextContent(tempStr);
    elem->appendChild(atributo);

}

int Persistencia::recuperarDato(DOMElement* ElementoCte, std::string &nombre) {

	XMLCh* ATTR_DATO = XMLString::transcode(nombre.c_str());
	DOMAttr* attr_dato = ElementoCte->getAttributeNode(ATTR_DATO);
	std::string aux = XMLString::transcode(attr_dato->getValue());

	return atoi(aux.c_str());

}
