/**************************   Clase Decorador_Ventana  ************************/
/**************************   Grupo 8                  ************************/

#include "common_Decorador_Ventana.h"
#include "../controlador/common_Controladores_Window.h"

/*----------------------------------------------------------------------------*/

Decorador_Ventana::Decorador_Ventana(Componente_Visual* componente): Decorador(componente){

		//Creo la ventana
		_ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (_ventana), "Simulador de Circuitos Logicos");
		gtk_signal_connect (GTK_OBJECT (_ventana), "delete_event",
		                        GTK_SIGNAL_FUNC (Controlador_Ventana::delete_event), NULL);

		gtk_signal_connect (GTK_OBJECT (_ventana), "destroy",
		                        GTK_SIGNAL_FUNC (Controlador_Ventana::destroy), NULL);

		//TODO
		gtk_widget_set_usize(_ventana,500,500);
		//fin TODO

		//incluyo el componente
		gtk_container_add (GTK_CONTAINER (_ventana),componente->getWidget());


}
/*----------------------------------------------------------------------------*/

void Decorador_Ventana::show(){

	//muestro el componente
	show_componente();

	//muestro la ventana
	gtk_widget_show (_ventana);
	//TODO Dejo a la ventana en espera de que suceda algún tipo de evento
	// gtk_main ();
}

/*----------------------------------------------------------------------------*/

GtkWidget* Decorador_Ventana::getWidget(){

	return _ventana;
}
/*----------------------------------------------------------------------------*/
