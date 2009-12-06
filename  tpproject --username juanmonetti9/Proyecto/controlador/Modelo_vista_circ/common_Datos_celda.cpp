
#include "common_Datos_celda.h"
#include "common_Celda.h"

Datos_celda::Datos_celda() {

	this->ocupada = false;
	this->colum_ppal= -1;
	this->fila_ppal= -1;
	this->id = -1;
	this->sentido = ESTE;
	this->tipo= T_VACIA;

}

TIPO_COMPUERTA Datos_celda::get_tipo()const{

	return this->tipo;
}

void Datos_celda::set_tipo(TIPO_COMPUERTA nuevo_tipo){

	this->tipo = nuevo_tipo;

}

SENTIDO Datos_celda::get_sentido()const{

	return this->sentido;
}

void Datos_celda::set_sentido(SENTIDO nuevo_sentido){

	this->sentido = nuevo_sentido;
}

int Datos_celda::get_id()const{

	return this->id;
}

void Datos_celda::set_id(int nuevo_id){

	if(nuevo_id >= 0){

		this->id = nuevo_id;
	}
}

int Datos_celda::get_fila_ppal()const{

	return this->fila_ppal;
}

void Datos_celda::set_fila_ppal(int nueva_fila_ppal){

	if(nueva_fila_ppal > 0){
		this->fila_ppal = nueva_fila_ppal;
	}
}

int Datos_celda::get_col_ppal()const{

	return this->colum_ppal;
}

void Datos_celda::set_col_ppal(int nueva_col_ppal){

	if( nueva_col_ppal > 0 ){

		this->colum_ppal = nueva_col_ppal;
	}
}

bool Datos_celda::esta_ocupada()const{

	return this->ocupada;

}

void Datos_celda::ocupar(){

	this->ocupada = true;
}

void Datos_celda::desocupar(){

	this->ocupada = false;
	this->celda_ppal=false;
	this->colum_ppal= -1;
	this->fila_ppal= -1;
	this->id = -1;
	this->sentido = ESTE;
	this->tipo= T_VACIA;

}

bool Datos_celda::es_principal()const{

	return this->celda_ppal;
}

void Datos_celda::set_ppal(bool nueva_ppal){

	this->celda_ppal = nueva_ppal;
}

void Datos_celda::agregar_a_entorno(Celda* nueva_celda){
	//todo agregar a ebntoprno
}

void Datos_celda::vaciar_entorno(){
	//todo vaciar todo el entorno?
}

std::list<Celda*>& Datos_celda::get_entorno(){

	return entorno;
}


Datos_celda::~Datos_celda() {

}
