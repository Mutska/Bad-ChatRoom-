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

int main(){
  stringstream reloaded;
  string testing;
  //Mensaje de prueba para enviar al cliente
  char msg[10000] = "Identificate por favor";
  char client_response[10000];
  reloaded.str(msg);
  testing  = reloaded.str();
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
  
  //Si hay conexion se acepta
  client_socket = accept(server_socket,(struct sockaddr*) &client_info, (socklen_t*) &client_space);
  //Variables necesarias para probar la veracidad el comando que el cliente conectado introduce
  size_t found;
  string name;
  string need = "IDENTIFY ";
  char error[256] = "EL comando que introduciste es incorrecto vuelve a conectarde.. adios :V";
  while(1){
    // Imprime el ip del cliente que entra
    getpeername(client_socket, (struct sockaddr*) &client_info, (socklen_t*) &client_space);
    cout<<"Cliente "<<inet_ntoa(client_info.sin_addr)<<":"<<ntohs(client_info.sin_port)<<"."<<endl;
    
    //Y se envia un mensaje al cliente
    send(client_socket,msg,sizeof(msg),0);
    
    //Se recibe un mensaje del cliente
    recv(client_socket,&client_response,sizeof(client_response),0);
    reloaded.str(client_response);
    testing  = reloaded.str();
    found = testing.find(need);
    if(found != std::string::npos){
      name = testing.substr(found+9);
    }else{
      send(client_socket,error,sizeof(error),0);
      cout<<"Servidor cerrandose ante agresividad del cliente :("<<endl;
      break;
    }
    cout<<"Testing es: "<<testing<<endl;
    name = testing.substr(found+9);
    
    //guardamos el nuevo cliente si es que pasa la prueba del nombre
    temporal = newNode(client_socket, inet_ntoa(client_info.sin_addr),name);
    list[name] = temporal;
    
    //Testeando la conversion de un tipo char[] a string con el mensaje del cliente
    cout<<"El cliente envio su nombre: "<<name<<endl;
    
  }
  //Cerrar el servidor
  close(server_socket);
  return 0;
}



												     
			
			
