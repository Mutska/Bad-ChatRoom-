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
  server_info.sin_addr.s_addr = inet_addr("192.168.15.8");
  server_info.sin_port = htons(8080);

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
  string need = "IDENTIFY ";
  char error[256] = "EL comando que introduciste es incorrecto vuelve a conectarde.. adios :V";
  while(1){
    //Si hay conexion se acepta
    client_socket = accept(server_socket,(struct sockaddr*) &client_info, (socklen_t*) &client_space);
    // Imprime el ip del cliente que entra
    getpeername(client_socket, (struct sockaddr*) &client_info, (socklen_t*) &client_space);
    cout<<"Cliente "<<inet_ntoa(client_info.sin_addr)<<":"<<ntohs(client_info.sin_port)<<"."<<"Entrando"<<endl;
    
    //Y se envia un mensaje al cliente
    send(client_socket,msg,sizeof(msg),0);
    
    //Se recibe un mensaje del cliente
    recv(client_socket,&client_response,sizeof(client_response),0);
    stream.str(client_response);
    cadena  = stream.str();
    found = cadena.find(need);
    if(found != std::string::npos){
      name = cadena.substr(found+9);
      cout<<"El cliente envio su nombre: "<<name<<endl;
    }else{
      send(client_socket,error,sizeof(error),0);
      cout<<"Servidor cerrandose ante agresividad del cliente :("<<endl;
      break;
    }
    name = cadena.substr(found+9);
    //guardamos el nuevo cliente en el diccionario si es que pasa la prueba del nombre
    temporal = newNode(client_socket, inet_ntoa(client_info.sin_addr),name);
    list[name] = temporal;
    
    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, (void *)manager, (void *)temporal) != 0) {
      perror("Error al crear el hilo!\n");
      exit(EXIT_FAILURE);
    }
    
  }
  //Cerrar el servidor
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
  int flag = 0;
  char nickname[50] = {};
  char recv_buffer[250000] = {};
  char send_buffer[250000] = {};
  Usuario *cliente = (Usuario *)client;
    // Ciclo  que esta pendiente a las solicitudes del cliente, esta sera la funcion mas importante del servidor
  int receive;
  while (1) {
    if (flag) {//si esta activada la bandera el servidor cierra
      break;
    }
    receive = recv(cliente->data, recv_buffer,1000, 0);
    if (receive > 0) {
      if (strlen(recv_buffer) == 0) {
	continue;
      }
      sprintf(nickname, "%s" ,cliente->name.c_str());
      trimm(nickname, 50);
      strcpy(send_buffer,"");
      sprintf(send_buffer, "%s：%s" , nickname, recv_buffer);
    } else if (receive == 0) {
      cout<<cliente->name<<"("<<cliente->ip<<")"<<"("<<cliente->data<<")"<<" dejo el chat"<<endl;
      sprintf(send_buffer, "%s(%s) dejo el chat.", nickname, cliente->ip);
      flag = 1;
    } else {
      cout<<"Error :("<<endl;
      flag = 1;
    }
    send_to_all(list,cliente, send_buffer);
  }
  // Cuando termina el cliente se cierra su "FILE DESCRIPTOR" y lo elimina del diccionario
  close(cliente->data);
  list.erase(cliente->name);
  delete cliente;
}



												     
			
			
