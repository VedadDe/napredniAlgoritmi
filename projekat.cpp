#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class Kurs {
public:
    std::string sifra;
    int trajanje;
    int broj_studenata;

    Kurs(std::string s, int t, int b)
        : sifra(s), trajanje(t), broj_studenata(b) {}
};

class Predavac {
public:
    std::string sifra;
    std::vector<int> slotovi;

    Predavac(std::string s, std::vector<int> sl)
        : sifra(s), slotovi(sl) {}
};

class Ucionica {
public:
    std::string sifra;
    int kapacitet;

    Ucionica(std::string s, int k)
        : sifra(s), kapacitet(k) {}
};

std::vector<Kurs> ucitajKurseve(const std::string& putanja) {
    std::vector<Kurs> kursevi;
    std::ifstream file(putanja);
    std::string linija;

    while (getline(file, linija)) {
        std::istringstream ss(linija);
        std::string sifra, temp;
        std::getline(ss, sifra, ',');
        std::getline(ss, temp, ',');
        int trajanje = std::stoi(temp);
        std::getline(ss, temp);
        int broj_studenata = std::stoi(temp);

        kursevi.push_back(Kurs(sifra, trajanje, broj_studenata));
    }

    return kursevi;
}

std::vector<Predavac> ucitajPredavace(const std::string& putanja) {
    std::vector<Predavac> predavaci;
    std::ifstream file(putanja);
    std::string linija;

    while (getline(file, linija)) {
        std::istringstream ss(linija);
        std::string sifra;
        std::getline(ss, sifra, ',');

        std::vector<int> slotovi(50);
        for (int i = 0; i < 50; i++) {
            std::string temp;
            std::getline(ss, temp, ',');
            slotovi[i] = std::stoi(temp);
        }

        predavaci.push_back(Predavac(sifra, slotovi));
    }

    return predavaci;
}

std::vector<Ucionica> ucitajUcionice(const std::string& putanja) {
    std::vector<Ucionica> ucionice;
    std::ifstream file(putanja);
    std::string linija;

    while (getline(file, linija)) {
        std::istringstream ss(linija);
        std::string sifra, temp;
        std::getline(ss, sifra, ',');
        std::getline(ss, temp);
        int kapacitet = std::stoi(temp);

        ucionice.push_back(Ucionica(sifra, kapacitet));
    }

    return ucionice;
}

int main() {
    std::vector<Kurs> kursevi = ucitajKurseve("kursevi.csv");
    std::vector<Predavac> predavaci = ucitajPredavace("predavaci.csv");
    std::vector<Ucionica> ucionice = ucitajUcionice("ucionice.csv");

    std::cout << "Kursevi:\n";
    for (const auto& kurs : kursevi) {
        std::cout << "Sifra: " << kurs.sifra << ", Trajanje: " << kurs.trajanje << ", Broj studenata: " << kurs.broj_studenata << "\n";
    }

    std::cout << "\nPredavaci:\n";
    for (const auto& predavac : predavaci) {
        std::cout << "Sifra: " << predavac.sifra << ", Slotovi: ";
        for (const auto& slot : predavac.slotovi) {
            std::cout << slot << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nUcionice:\n";
    for (const auto& ucionica : ucionice) {
        std::cout << "Sifra: " << ucionica.sifra << ", Kapacitet: " << ucionica.kapacitet << "\n";
    }

    return 0;
}


