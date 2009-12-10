/*
 * common_Boton_InvertirL.cpp
 *
 *  Created on: 19/11/2009
 *      Author: juanm
 */

/**************************   Clase BOTON Invertir *******************/
/**************************   	Grupo 8            *******************/

#include "common_Boton_InvertirL.h"
#include "../../controlador/common_Controladores_Circuito.h"

Boton_InvertirL::Boton_InvertirL():Boton() {

	agregarImagen((gchar*)"imagenes/rotar_left.png");
	agregarToolTip((gchar*)"Invertir Izquierda");
	gtk_signal_connect (GTK_OBJECT (getWidget()), "clicked", GTK_SIGNAL_FUNC (Controlador_Circuito::callback_InvertirL), NULL);


}

Boton_InvertirL::~Boton_InvertirL() {

}
