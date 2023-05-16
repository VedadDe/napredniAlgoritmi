import csv
import random

# Konfiguracija
broj_instanci = 10

# Generisanje podataka o kursevima
with open('kursevi.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    for i in range(broj_instanci):
        kurs = f"CS{i:02d}"
        trajanje = random.randint(2, 5)
        broj_studenata = random.randint(5, 50)
        writer.writerow([kurs, trajanje, broj_studenata])

# Generisanje podataka o predavačima
with open('predavaci.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    for i in range(broj_instanci):
        predavac = f"P{i:03d}"
        slotovi = [random.randint(0, 1) for _ in range(50)]
        writer.writerow([predavac] + slotovi)

# Generisanje podataka o učionicama
with open('ucionice.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    for i in range(broj_instanci):
        ucionica = f"U{i:03d}"
        kapacitet = random.randint(10, 100)
        writer.writerow([ucionica, kapacitet])
