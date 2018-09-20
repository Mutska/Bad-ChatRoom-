#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <map>
#include "Usuario.h"
#include "Auxiliares.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;
Usuario* temporal;
map<string,Usuario*> list;
void get_out(int signal);
void manager(void *);
int main(){
  //Este tipo de variable me ayuda a guardar las cadenas de estilo c
  stringstream stream;
  //Esta variable me ayudara a hacer operaciones sobre los comandos recibidos
  string cadena;
  //Mensaje de prueba para enviar al cliente
  char msg[10000] = "Identificate por favor";
  char client_response[256000];
  stream.str(msg);
  cadena  = stream.str();
  int server_socket,client_socket;
  //Estas son las estructuras que corresponden al servidor y al cliente, las cuales hay que configurar
  struct sockaddr_in server_info,client_info;
  //Estas variables guardan la cantidad de memoria necesaria que se debe reservar para el cliente y el servidor.
  int server_space = sizeof(server_info);
  int client_space = sizeof(client_info);
  //Creamos el socket del servidor
  server_socket = socket(AF_INET,SOCK_STREAM,0);
  if(server_socket == -1){
    cout<<"El socket no se pudo iniciar";
    exit(EXIT_FAILURE);
  }
  //Pedimos memoria al sistema para las estructuras del servidor y cliente
  memset(&server_info,0,server_space);
  memset(&client_info,0,client_space);
  
  //Configuramos las estructura del servidor 
  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_info.sin_port = htons(1234);

  //Hacemos un bind para reservar el puerto que pensamos ocupar para el servidor
  bind(server_socket,(struct sockaddr *)&server_info,server_space);
  
  //Y nos ponemos a la escucha de clientes
  listen(server_socket,30);
  
  //Imprime la ip del servidor
  cout<<"Servidor iniciado en: "<<inet_ntoa(server_info.sin_addr)<<":"<<ntohs(server_info.sin_port)<<endl;

  //Se agrega el servidor como primer elemento en el diccionario(test)
  list["Server"]  = newNode(server_socket, inet_ntoa(server_info.sin_addr),"Server");
  
  //Variables necesarias para probar la veracidad el comando que el cliente conectado introduce
  size_t found;
  string name;
  string real;
  string need = "IDENTIFY ";
  char error[256] = "EL comando que introduciste es incorrecto vuelve a conectarde.. adios :V";
  int band = 0;
  while(1){
    //Si hay conexion se acepta
    client_socket = accept(server_socket,(struct sockaddr*) &client_info, (socklen_t*) &client_space);
    
    // Imprime el ip del cliente que entra
    getpeername(client_socket, (struct sockaddr*) &client_info, (socklen_t*) &client_space);
    cout<<"Cliente "<<inet_ntoa(client_info.sin_addr)<<":"<<ntohs(client_info.sin_port)<<"."<<"Entrando"<<endl;
    
    //Y se envia un mensaje al cliente
    send(client_socket,msg,sizeof(msg),0);
    memset(client_response,0,sizeof(client_response));
    
    //Se recibe un mensaje del cliente
    recv(client_socket,&client_response,sizeof(client_response),0);
    stream.str(client_response);
    cadena  = stream.str();
    found = cadena.find(need);
    real.clear();
    if(found != std::string::npos){
      name = cadena.substr(found+9);
      name.erase(remove(name.begin(), name.end(), ' '), name.end());
      for(int i = 0;i<name.size();i++){
	if(isalpha(name[i]))
	  real += name[i];	
      }
      cout<<"El cliente envio su nombre: "<<real<<endl;
      band = 0;
    }else if(found == std::string::npos){
      send(client_socket,error,sizeof(error),0);
      cout<<"Cliente no aceptado :("<<endl;
      close(client_socket);
      band = 1;
    }
    if(band)
      continue;
    //guardamos el nuevo cliente en el diccionario si es que pasa la prueba del nombre
    temporal = newNode(client_socket, inet_ntoa(client_info.sin_addr),real);
    list[real] = temporal;
    
    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, (void *)manager, (void *)temporal) != 0) {
      perror("Error al crear el hilo!\n");
      exit(EXIT_FAILURE);
    }
    
  }
  //Cerrar el servidor
  close(client_socket);
  close(server_socket);
  return 0;
}

//Esta funcion es para  cuando te sales de la sala, libera la memoria  y cierra el socket de cada cliente  con su int data
void get_out(int sig) {
  Usuario *tmp;
  for (map<std::string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    close(it->second->data);
    tmp = it->second;
    delete tmp;
  }
  cout<<endl<<"Cerrando servidor"<<endl;
  exit(EXIT_SUCCESS);
}


//Manejador del cliente 
void manager(void *client) {   //el parametro p_client es la direccion de p_client por eso le precede un puntero void
  Usuario *cliente = (Usuario *)client;
  int flag = 0;
  int test;
  int tam;
  int pivote;
  int roomname_len;
  char recv_buffer[250000] = {};
  string send_buffer = "DEFAULT";
  string cadena;
  string status;
  string roomname;
  string case_five;
  string roomname_message;
  stringstream stream;
  vector <string> case_seven;
    // Ciclo  que esta pendiente a las solicitudes del cliente, esta sera la funcion mas importante del servidor
  while (1) {
    if (flag) {//si esta activada la bandera el servidor cierra
      break;
    }
    memset(recv_buffer,0,sizeof(recv_buffer));
    int receive = recv(cliente->data, recv_buffer,1000, 0);
    if (receive > 0) {
      if (strlen(recv_buffer) == 0) {
	continue;
      }
      cadena.clear();
      send_buffer.clear();
      stream.str(recv_buffer);
      cadena  = stream.str();
      tam = cadena.size();
      test = detect(cadena);
      cadena.erase(std::remove(cadena.begin(), cadena.end(), '\n'), cadena.end());
      if(test != std::string::npos){
	if(test == 1){
	  send_buffer = send_arguments(1,cadena);
	  send_buffer += "\n";
	  
	}
	if(test == 2){
	  status  = send_arguments(2,cadena);
	}
	if(test == 3){
	  send_buffer = show_users(list);
	}
	if(test == 4){
	  send_buffer = cliente->name;
	  send_buffer += " : ";
	  send_buffer += send_arguments(4,cadena);
	  send_buffer += "\n";
	}
	if(test == 5){
	  send_buffer = "Mensaje privado de ";
	  send_buffer += cliente->name;
	  send_buffer += " : ";
	  send_buffer += send_arguments(5,cadena);
	  send_buffer += "\n";
	  case_five = send_five_case(cadena);//CAMBIAR AL PUNTO G :V
	  
	}
	if(test == 6){
	  roomname = send_arguments(6,cadena);
	  cliente->chatrooms[roomname] = 1;
	  send_buffer = "Chatroom: ";
	  send_buffer +=  roomname;
	  send_buffer += " creado con exito\n";
	}
	if(test == 7){
	  case_seven = send_seven_case(cadena);
	  roomname = case_seven[1];
	  send_buffer = "Se ha enviado la invitacion a los usuarios\n";
	}
	if(test == 8){
	  roomname = send_arguments(8,cadena);
	  unordered_map<string,int>::const_iterator verify = cliente->chatrooms.find(roomname);
	  if(verify == cliente->chatrooms.end() ){
	    send_buffer = "No has sido invitado a la sala\n";
	  }else{
	    cliente->chatrooms[roomname] = 2;
	    send_buffer = "Has sido agregado exitosamente al chatroom: ";
	    send_buffer += roomname;
	    send_buffer += " \n";
	  }
	}
	if(test == 9){
	  // case_nine = send_nine_case(cadena);
	  roomname = send_arguments(9,cadena);
	  roomname_len = roomname.size();
	  pivote = cadena.find(roomname);
	  roomname_message = cadena.substr(pivote+roomname_len+1);
	  send_buffer = "Mensaje enviado con exito a la sala: ";
	  send_buffer += roomname;
	  send_buffer += "\n";
	}
	if(test == 10){
	  send_buffer = "Has decidido desconectarte amigo, nos vemos la proxima\n";
	  }
      }else{
	test = 13;
	send_buffer = "El comando que introduciste es incorrecto o su sintaxis esta mal\n";
      }     
      
      
    } else if (receive == 0) {
      cout<<cliente->name<<"("<<cliente->ip<<")"<<"("<<cliente->data<<")"<<" dejo el chat"<<endl;
      send_buffer = cliente->name;
      send_buffer += "(";
      send_buffer += cliente->ip;
      send_buffer += ") ";
      send_buffer += "dejo el chat\n";
      send_buffer += "\n";
      flag = 1;
      
    } else {
      cout<<"Error :("<<endl;
      flag = 1;
    }
    if(test == 1){
      send_to_one(list,cliente,send_buffer);
    }
    if(test == 2){
      if(status == "ERROR"){
	send_buffer = "El status que introduciste es incorrecto, trata de nuevo\n";
	send_to_one(list,cliente,send_buffer);
      }else{
	cliente->status = status;
	send_buffer = "Nuevo status: ";
	send_buffer += status;
	send_buffer += "\n";
	send_to_one(list,cliente,send_buffer);
      }
      
    }
    if(test == 3){
      send_to_one(list,cliente,send_buffer);
    }
    if(test == 4){
      send_to_all(list,cliente, send_buffer);
    }
    if(test == 13){
      send_to_one(list,cliente,send_buffer);
    }
    if(test == 5){
      send_private_msg(list,cliente,send_buffer,case_five);
    }
    if(test == 6){
      send_to_one(list,cliente,send_buffer);     
    }
    if(test == 7){
      send_to_one(list,cliente,send_buffer);
      send_invitation(list,cliente,case_seven,roomname);
    }
    if(test == 8){
      send_to_one(list,cliente,send_buffer);
    }
    if(test == 9){
      send_to_one(list,cliente,send_buffer);
      send_to_room(list,cliente,roomname_message,roomname);
    }
    if(test == 10){
      send_to_one(list,cliente,send_buffer);
      break;
    }
    send_buffer.clear();    
  }
  // Cuando termina el cliente se cierra su "FILE DESCRIPTOR" y lo elimina del diccionario
  close(cliente->data);
  list.erase(cliente->name);
  delete cliente;
}



												     
			
			
