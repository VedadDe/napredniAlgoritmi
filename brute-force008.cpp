#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

// Klasa za raspored
class Raspored {
public:
    std::string kurs;
    std::string ucionica;
    std::vector<int> slotovi;
};

// Klasa za predavače
class Predavac {
public:
    std::string ime;
    std::vector<int> slotovi;
};

// Klasa za kurseve
class Kurs {
public:
    std::string naziv;
    int trajanje;
    std::string predavac;
};

// Klasa za učionice
class Ucionica {
public:
    std::string naziv;
};

// Funkcija za učitavanje CSV datoteke i generiranje objekata klase Predavac
std::vector<Predavac> ucitajPredavace(const std::string& filename) {
    std::vector<Predavac> predavaci;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nemoguće otvoriti datoteku " << filename << std::endl;
        return predavaci;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Predavac predavac;
        std::string slot;
        std::getline(iss, predavac.ime, ',');
        while (std::getline(iss, slot, ',')) {
            predavac.slotovi.push_back(std::stoi(slot));
        }
        predavaci.push_back(predavac);
    }

    file.close();
    return predavaci;
}

// Funkcija za učitavanje CSV datoteke i generiranje objekata klase Kurs
std::vector<Kurs> ucitajKurseve(const std::string& filename) {
    std::vector<Kurs> kursevi;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nemoguće otvoriti datoteku " << filename << std::endl;
        return kursevi;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Kurs kurs;
        std::getline(iss, kurs.naziv, ',');
        std::string trajanjeStr;
        std::getline(iss, trajanjeStr, ',');
        kurs.trajanje = std::stoi(trajanjeStr);
        std::getline(iss, kurs.predavac, ',');
        kursevi.push_back(kurs);
    }

    file.close();
    return kursevi;
}

// Funkcija za učitavanje CSV datoteke i generiranje objekata klase Ucionica
std::vector<Ucionica> ucitajUcionice(const std::string& filename) {
    std::vector<Ucionica> ucionice;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nemoguće otvoriti datoteku " << filename << std::endl;
        return ucionice;
    }

    std::string line;
    while (std::getline(file, line)) {
        Ucionica ucionica;
        std::istringstream iss(line);
        std::getline(iss, ucionica.naziv, ',');
        ucionice.push_back(ucionica);
    }

    file.close();
    return ucionice;
}
// Funkcija za generisanje svih mogućih rasporeda
std::vector<Raspored> generisiRasporede(const std::vector<Kurs>& kursevi, const std::vector<Ucionica>& ucionice, const std::vector<int>& slotovi) {
    std::vector<Raspored> rasporedi;
    for (const auto& kurs : kursevi) {
        for (const auto& ucionica : ucionice) {
            for (const auto& slot : slotovi) {
                Raspored raspored;
                raspored.kurs = kurs.naziv;
                raspored.ucionica = ucionica.naziv;
                // Ovde pretpostavljamo da su svi časovi jednako dugi
                for (int i = 0; i < kurs.trajanje; ++i) {
                    raspored.slotovi.push_back(slot + i);
                }
                rasporedi.push_back(raspored);
            }
        }
    }
    return rasporedi;
}

// Funkcija za proveru da li je neki raspored validan
bool jeValidan(const Raspored& raspored, const std::vector<Predavac>& predavaci, const std::vector<Kurs>& kursevi) {
    // Nađi predavača za ovaj kurs
    std::string imePredavaca;
    for (const auto& kurs : kursevi) {
        if (kurs.naziv == raspored.kurs) {
            imePredavaca = kurs.predavac;
            break;
        }
    }

    // Nađi slotove koje predavač preferira
    std::vector<int> preferiraniSlotovi;
    for (const auto& predavac : predavaci) {
        if (predavac.ime == imePredavaca) {
            preferiraniSlotovi = predavac.slotovi;
            break;
        }
    }

    // Proveri da li se slotovi kursa preklapaju sa preferiranim slotovima predavača
    for (const auto& slot : raspored.slotovi) {
        if (std::find(preferiraniSlotovi.begin(), preferiraniSlotovi.end(), slot) == preferiraniSlotovi.end()) {
            // Ako se neki slot kursa ne nalazi u preferiranim slotovima, rasporednije validan
            return false;
        }
    }

    // Ako su svi slotovi kursa u preferiranim slotovima predavača, raspored je validan
    return true;
}

int main() {
    // Konfiguracija
    int broj_instanci = 10;

    // Generisanje podataka o predavačima
    std::vector<Predavac> predavaci = ucitajPredavace("predavaci.csv");

    // Generisanje podataka o kursevima
    std::vector<Kurs> kursevi = ucitajKurseve("kursevi.csv");

    // Generisanje podataka o učionicama
    std::vector<Ucionica> ucionice = ucitajUcionice("ucionice.csv");

    // Ispis podataka
    std::cout << "Podaci o predavačima:" << std::endl;
    for (const auto& predavac : predavaci) {
        std::cout << "Ime: " << predavac.ime << std::endl;
        std::cout << "Slotovi: ";
        for (const auto& slot : predavac.slotovi) {
            std::cout << slot << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Podaci o kursevima:" << std::endl;
    for (const auto& kurs : kursevi) {
        std::cout << "Naziv: " << kurs.naziv << std::endl;
        std::cout << "Trajanje: " << kurs.trajanje << std::endl;
        std::cout << "Predavac: " << kurs.predavac << std::endl;
    }

    std::cout << "Podaci o učionicama:" << std::endl;
    for (const auto& ucionica : ucionice) {
        std::cout << "Naziv: " << ucionica.naziv << std::endl;
    }
    // Konfiguracija
//    int broj_instanci = 10;
    std::vector<int> slotovi(50); // pretpostavimo da postoji 50 slotova (5 dana * 10 slotova po danu)
    for (int i = 0; i < 50; ++i) {
        slotovi[i] = i;
    }
        // Generisanje svih mogućih rasporeda
    std::vector<Raspored> rasporedi = generisiRasporede(kursevi, ucionice, slotovi);

    // Provera svih rasporeda
    for (const auto& raspored : rasporedi) {
        if (jeValidan(raspored, predavaci, kursevi)) {
            std::cout << "Validan raspored: " << std::endl;
            std::cout << "Kurs: " << raspored.kurs << std::endl;
            std::cout << "Ucionica: " << raspored.ucionica << std::endl;
            std::cout << "Slotovi: ";
            for (const auto& slot : raspored.slotovi) {
                std::cout << slot << " ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
