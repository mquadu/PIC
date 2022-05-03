import serial
import time
from tkinter import ttk
from ttkthemes import ThemedTk

serialPort = serial.Serial('COM4', baudrate=9600, timeout=1)

print("connecte au port serie: " + serialPort.portstr)


# Configuration du seuil critique
def seuilCritique():
    resultat = spinbox1.get()
    serialPort.write(resultat.encode())
    time.sleep(3)
    return resultat


# Lecture des mesures reçues du PIC à travers le port serie
def readFromSerial():
    data = serialPort.readline().decode("ascii")

    if data != "":
        distance = int(data)

        print(distance)

        seuil = seuilCritique()
        if distance < int(seuil):
            texte = "La distance mesurée est de {} cm".format(distance)
            label2.config(text=texte)
        elif distance >= int(seuil):
            texte = "{} cm Attention le seuil critique est atteint!".format(distance)
            label2.config(text=texte)

    window.after(50, readFromSerial)


window = ThemedTk(themebg=True)
window.set_theme("equilux")
window.title("Projet Programmation PIC")

message1 = ttk.Label(window, text="Veuillez sélectionner un seuil critique:")
message1.grid(column=0, row=0, padx=10, pady=10)

spinbox1 = ttk.Spinbox(window, from_=0, to=10, width=10, state='readonly')
spinbox1.set(0)
spinbox1.grid(column=1, row=0, padx=10, pady=10)

button1 = ttk.Button(window, text="Valider", command=seuilCritique)
button1.grid(column=0, row=1, padx=10, pady=10)

label2 = ttk.Label(window, text="", width=20)
label2.grid(column=1, row=1, padx=10, pady=10)
window.update()

if __name__ == '__main__':
    window.after(50, readFromSerial)
    window.mainloop()
