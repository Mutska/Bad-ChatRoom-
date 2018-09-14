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
using namespace std;
int main(){
  stringstream reloaded;
  string testing;
  //Mensaje de prueba para enviar al servidor
  char msg[10000] = "Mensaje de prueba";
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
  server_info.sin_addr.s_addr = inet_addr("192.168.0.13");
  server_info.sin_port = htons(8080);

  //Hacemos un bind para reservar el puerto que pensamos ocupar para el servidor
  bind(server_socket,(struct sockaddr *)&server_info,server_space);
  
  //Y nos ponemos a la escucha de clientes
  listen(server_socket,30);
  
  //Imprime la ip del servidor
  cout<<"Servidor iniciado en: "<<inet_ntoa(server_info.sin_addr)<<":"<<ntohs(server_info.sin_port)<<endl;
  
  //Si hay conexion se acepta
  client_socket = accept(server_socket,NULL,NULL);
  
  //Y se envia un mensaje al cliente
  send(client_socket,msg,sizeof(msg),0);

  //Testeando la conversion de un tipo char[] a string
  cout<<"El servidor envio el siguiente mensaje: "<<testing<<endl;

  //Cerrar el servidor
  close(server_socket);
  return 0;
}



												     
			
			
