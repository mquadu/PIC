import serial
import time
from tkinter import ttk
from ttkthemes import ThemedTk

serialPort = serial.Serial('COM3', baudrate=9600, timeout=1)

print("connecte au port serie: " + serialPort.portstr)


# Configuration du seuil critique
def seuilCritique():
    resultat = int(spinbox1.get())
    serialPort.write(bytes(":" + "%03d" % resultat, 'UTF-8'))
    time.sleep(3)
    return resultat


# Lecture des mesures reçues du PIC à travers le port serie
def readFromSerial():
    data = serialPort.readline().decode("ascii")

    if data != "":
        # if "distance" in data:

        #     tab_distance = [int(s) for s in data.split() if s.isdigit()]
        #     distance = int(tab_distance[0])
        # else :
        #     distance = int (data)
        distance = str(data)
        valeur_distance = 0
        if "distance" in distance:
            tab_distance = [int(s) for s in data.split() if s.isdigit()]
            valeur_distance = int(tab_distance[0])

        seuil = seuilCritique()
        print("seuil", seuil)
        if valeur_distance < int(seuil):
            status = "OK"
            texte = f"{distance} La distance mesurée est de {valeur_distance} cm".format()
            label2.config(text=texte)
            label3.config(text=status)
        elif valeur_distance >= int(seuil):
            status = "ALARM"
            texte = f"{distance} cm Attention le seuil critique est atteint!".format(
                valeur_distance)
            label2.config(text=texte)
            label3.config(text=status)

    window.after(50, readFromSerial)


window = ThemedTk(themebg=True)
window.set_theme("equilux")
window.title("Projet Programmation PIC")

message1 = ttk.Label(window, text="Veuillez sélectionner un seuil critique:")
message1.grid(column=0, row=0, padx=10, pady=10)

spinbox1 = ttk.Spinbox(window, from_=0, to=10, width=10, state='normal')
spinbox1.set(100)
spinbox1.grid(column=1, row=0, padx=10, pady=10)

button1 = ttk.Button(window, text="Valider", command=seuilCritique)
button1.grid(column=0, row=1, padx=10, pady=10)

label2 = ttk.Label(window, text="", width=40)
label2.grid(column=1, row=1, padx=10, pady=10)
window.update()

label3 = ttk.Label(window, text="", width=40)
label3.grid(column=1, row=2, padx=10, pady=10)
window.update()

if __name__ == '__main__':
    window.after(50, readFromSerial)
    window.mainloop()
