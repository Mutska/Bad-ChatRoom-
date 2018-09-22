#!/usr/bin/env python3
from socket import AF_INET, socket, SOCK_STREAM
from threading import Thread
import tkinter
import re
import string
import os

#Funcion que se encarga de recibir los mensajes 
def receive():
    while True:
        try:
            msg = client_socket.recv(BUFFER).decode('utf-8')
            final = re.sub('[^a-zA-Z0-9():]+', ' ',msg)            
            text_list.insert(tkinter.END, final)
        except OSError: 
            break

#Funcion que se encarga de enviar los mensajes
def send(event=None):  
    msg = my_msg.get()
    my_msg.set("") 
    client_socket.send(bytes(msg, "utf8"))
    if msg == "DISCONNECT":
        client_socket.close()
        GUI.quit()


def get_out(event=None):
    my_msg.set("{quit}")
    send()


######################### Definicion de interfaz grafica ######################################
GUI = tkinter.Tk()
GUI.title("Cliente")

#Defincion del frame
FRAME = tkinter.Frame(GUI,width=35,height=100)

my_msg = tkinter.StringVar() 

#Definicion de la scrollbar
scrollbar = tkinter.Scrollbar(FRAME) 

#Configuracion de la listbox
text_list = tkinter.Listbox(FRAME, height=35, width=100, yscrollcommand=scrollbar.set,background = "#aca1a1",fg ="#44240e")

#Configuracion de la scrollbar
scrollbar.pack(side=tkinter.RIGHT, fill=tkinter.Y)
text_list.pack(side=tkinter.LEFT, fill=tkinter.BOTH)
scrollbar.config(command = text_list.yview)
text_list.pack()
FRAME.pack()

#Entrada de texto
entry_field = tkinter.Entry(GUI,width = 100 ,textvariable=my_msg)
entry_field.bind("<Return>", send)
entry_field.pack()

#Boton de envio
send_button = tkinter.Button(GUI, text="Enviar", command=send)
send_button.pack()


#Cierre de GUI
GUI.protocol("WM_DELETE_WINDOW", get_out)
###################################################################################################

#Configuracion de las variables y conexion del socket
HOST = '127.0.0.1'
PORT = 1234
BUFFER = 250000
SOCKET = (HOST, PORT)

client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect(SOCKET)

#Hilo de ejecucion de la funcion receive()
receive_thread = Thread(target=receive)
receive_thread.start()
tkinter.mainloop() 
