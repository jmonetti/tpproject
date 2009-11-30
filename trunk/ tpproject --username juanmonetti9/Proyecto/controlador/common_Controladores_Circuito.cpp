/**************************   Clase Controlador Boton  **************/
/**************************   	Grupo 8                **************/

#include "common_Controladores_Circuito.h"
#include "stdlib.h"
#include "Acciones/common_Accion_Draw_XOR.h"
#include "Acciones/common_Accion_Draw_AND.h"
#include "Acciones/common_Accion_NULA.h"
#include "Acciones/common_Accion_Draw_NOT.h"
#include "Acciones/common_Accion_Draw_OR.h"
#include "Acciones/common_Accion_Borrar.h"
#include "Acciones/common_Accion_Drag_Drop.h"
#include "Acciones/common_Accion_invertir_left.h"
#include "Acciones/common_Accion_invertir_right.h"



void Controlador_Circuito::crearVentanaDialogoServidor() {

	GtkWidget* cuadro= gtk_dialog_new();
	gtk_window_set_title (&GTK_DIALOG(cuadro)->window, "Servidor");

	GtkWidget* boton= gtk_button_new_with_label("Aceptar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->action_area), boton,TRUE, TRUE, 0);
	gtk_widget_show(boton);

	boton= gtk_button_new_with_label("Cancelar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->action_area), boton,TRUE, TRUE, 0);
	gtk_widget_show(boton);

    GtkWidget* label = gtk_label_new ("Host:");
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->vbox), label, TRUE, TRUE, 0);
    gtk_widget_show (label);
    gtk_widget_show(label);


    GtkWidget* entry= gtk_entry_new();
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show(entry);

    label = gtk_label_new ("Puerto:");
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->vbox), label, TRUE, TRUE, 0);
    gtk_widget_show (label);
    gtk_widget_show(label);


    entry= gtk_entry_new();
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->vbox), entry, TRUE, TRUE, 0);
	gtk_widget_show(entry);


	gtk_widget_show(cuadro);
}

void Controlador_Circuito::crearVentanaDialogoPuertas(const char* titulo) {

	GtkWidget* cuadro= gtk_dialog_new();
	gtk_window_set_title (&GTK_DIALOG(cuadro)->window, titulo);

	GtkWidget* boton= gtk_button_new_with_label("Aceptar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->action_area), boton,TRUE, TRUE, 0);
	gtk_widget_show(boton);

	boton= gtk_button_new_with_label("Cancelar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->action_area), boton,TRUE, TRUE, 0);
	gtk_widget_show(boton);

	GtkWidget* combo= gtk_combo_new();

    GList *glist = NULL;

    char* texto[100];

    for (int var = 0; var < 100; ++var) {

    	texto[var]= new char[4];
    	sprintf(texto[var],"%d",var);
    	glist = g_list_append (glist, texto[var]);

	}

    gtk_combo_set_popdown_strings (GTK_COMBO (combo), glist);

    for (int var = 0; var < 100; ++var) {

        delete[] texto[var];

   	}


    g_list_free( glist );

    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (cuadro)->vbox), combo, TRUE, TRUE, 0);
    gtk_widget_show(combo);

	gtk_widget_show(cuadro);


}

void Controlador_Circuito::callback_Upload( GtkWidget *widget,gpointer callback_data ){

	//TODO
	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_NULA(controlador));
	}

	crearVentanaDialogoServidor();

}

void Controlador_Circuito::callback_Download( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_NULA(controlador));
	}
	//TODO
	g_print("Apretado el download\n");
	//fin TODO

	crearVentanaDialogoServidor();

}


void Controlador_Circuito::callback_AND( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_Draw_AND(controlador));
	}

}

void Controlador_Circuito::callback_OR( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_OR(controlador));
	}


}

void Controlador_Circuito::callback_XOR( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_Draw_XOR(controlador));
	}

}

void Controlador_Circuito::callback_NOT( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_NOT(controlador));
	}

}

void Controlador_Circuito::callback_Selector( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->conectar_drag_drop();
		controlador->agregar_accion(new Accion_Drag_Drop(controlador));
	}

}

void Controlador_Circuito::callback_Pista( GtkWidget *widget,gpointer callback_data ){

	//TODO
	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_NULA(controlador));
	}

}

void Controlador_Circuito::callback_Simulacion( GtkWidget *widget,gpointer callback_data ){

	//TODO
	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_NULA(controlador));
	}

}

void Controlador_Circuito::callback_Delete( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_Borrar(controlador));
	}

}

void Controlador_Circuito::callback_InvertirL( GtkWidget *widget,gpointer callback_data ){

	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_invertir_left(controlador));
	}
}

void Controlador_Circuito::callback_InvertirR( GtkWidget *widget,gpointer callback_data ){


	Controlador* controlador=Controlador::get_instancia();

	if(controlador){
		controlador->desconectar_drag_drop();
		controlador->agregar_accion(new Accion_invertir_right(controlador));
	}
}


/*----------------------------------------------------------------------------*/

gint Controlador_Circuito::button_press_event (GtkWidget *widget, GdkEventButton *event){

  if (event->button == 1 ){
	  // si es el boton izq del raton
	  Controlador* controlador=Controlador::get_instancia();
	  int x=event->x;
	  int y=event->y;
	   if(controlador)
		  controlador->ejecutar_accion(x,y);

  }
 /* if (event->button == 3 ){
	  pos_x=event->x;
	  pos_y=event->y;

	  g_print ("Presiono el boton en (%d,%d)\n",pos_x,pos_y);
  }*/
  return true;
}

/*----------------------------------------------------------------------------*/

gboolean Controlador_Circuito::drag_drop_handl(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time,gpointer user_data){


	 // si es el boton izq del raton
	 Controlador* controlador=Controlador::get_instancia();

	 if(controlador)
		 if(controlador->get_arrastre_activo())
			 controlador->arrastrar(x,y);
    return  true;
}

