//Definicion de la clase Usuario
#include <string>
#ifndef USUARIO_H
#define USUARIO_H
//Esta es la clase de la cual se creara cada cliente que entre a la comunicacion
class Usuario{
 public:
  int data;
  char ip[16];
  std::string  name;
  std::string status;
  std::unordered_map<std::string,int> chatrooms;
 private:
  int nothing;

};
#endif
