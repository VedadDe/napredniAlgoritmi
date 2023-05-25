#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // za rand()
#include <ctime> // za time()
#include <map>
#include <algorithm>


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

// Struktura slot
struct Slot {
    Kurs* kurs;
    Predavac* predavac;
    Ucionica* ucionica;
    int pocetak; // Početak slota
    int kraj;    // Kraj slota
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

//logika

bool jeKriterijumZaustavljanjaIspunjen(int brojacIteracija) {
    // Pretpostavimo da je maksimalni broj iteracija 10000
    int maxBrojIteracija = 10000;

    // Provjerava da li je brojač iteracija dostigao ili premašio maksimalni broj
    if (brojacIteracija >= maxBrojIteracija) {
        return true;
    }

    // Ako nije, nastavite pretragu
    return false;
}

std::vector<Slot> generisiPocetnoResenje(std::vector<Kurs>& kursevi, std::vector<Predavac>& predavaci, std::vector<Ucionica>& ucionice, int dana, int slotova) {
    std::vector<Slot> raspored(dana * slotova);
    std::vector<std::vector<Ucionica*>> zauzeteUcionice(dana * slotova);
    std::vector<std::vector<Predavac*>> zauzetiPredavaci(dana * slotova);
    int trenutniSlot = 0;

    // Prolazimo kroz sve kurseve i dodeljujemo im slotove
    for (auto& kurs : kursevi) {
        // Pronađi predavača za ovaj kurs
        Predavac* predavacKursa = nullptr;
        for (auto& predavac : predavaci) {
            if (predavac.ime == kurs.predavac) {
                predavacKursa = &predavac;
                break;
            }
        }

        // Ako ne postoji predavač za ovaj kurs, to je problem sa podacima
        if (!predavacKursa) {
            std::cerr << "Nije pronađen predavač za kurs " << kurs.naziv << std::endl;
            continue;
        }

        // Ako nema dovoljno slotova za ovaj kurs, prekinuti
        if (trenutniSlot + kurs.trajanje > dana * slotova) {
            break;
        }

        // Provera da li je predavač dostupan za sve slotove kursa
        bool predavacDostupan = true;
        for (int i = 0; i < kurs.trajanje; ++i) {
            if (predavacKursa->slotovi[trenutniSlot + i] == 0) {
                predavacDostupan = false;
                break;
            }
        }

        // Ako predavač nije dostupan, preskočite ovaj kurs
        if (!predavacDostupan) {
            continue;
        }

        // Provjeri da li je predavač zauzet u ovom slotu
        for (int i = 0; i < kurs.trajanje; ++i) {
            for (auto& zauzetiPredavac : zauzetiPredavaci[trenutniSlot + i]) {
                if (zauzetiPredavac == predavacKursa) {
                    predavacDostupan = false;
                    break;
                }
            }
            if (!predavacDostupan) {
                break;
            }
        }

        // Ako je predavač zauzet, preskočiti ovaj kurs
        if (!predavacDostupan) {
            continue;
        }

        // Pronađi dostupnu učionicu
        Ucionica* dostupnaUcionica = nullptr;
        for (auto& ucionica : ucionice) {
            bool zauzeta = false;
            for (int i = 0; i < kurs.trajanje; ++i) {
                for (auto& zauzetaUcionica : zauzeteUcionice[trenutniSlot + i]) {
                    if (zauzetaUcionica == &ucionica) {
                        zauzeta = true;
                        break;
                    }
                }
                if (zauzeta) {
                    break;
                }
            }
            if (!zauzeta) {
                dostupnaUcionica = &ucionica;
                break;
            }
        }

        // Ako nema dostupnih učionica, preskočiti ovaj kurs
        if (!dostupnaUcionica) {
            continue;
        }

        // Dodeli ovaj kurs, predavača i dostupnu učionicu prvom dostupnom slotu
        for (int i = 0; i < kurs.trajanje; ++i) {
            raspored[trenutniSlot + i].kurs = &kurs;
            raspored[trenutniSlot + i].predavac = predavacKursa;
            raspored[trenutniSlot + i].ucionica = dostupnaUcionica;
            raspored[trenutniSlot + i].pocetak = trenutniSlot;
            raspored[trenutniSlot + i].kraj = trenutniSlot + kurs.trajanje - 1;

            // Označi učionicu i predavača kao zauzete za ovaj slot
            zauzeteUcionice[trenutniSlot + i].push_back(dostupnaUcionica);
            zauzetiPredavaci[trenutniSlot + i].push_back(predavacKursa);
        }

        // Pomeri trenutniSlot za trajanje ovog kursa
        trenutniSlot += kurs.trajanje;
    }

    return raspored;
}

bool jeValidno(std::vector<Slot>& resenje, std::vector<Predavac>& predavaci, std::vector<Ucionica>& ucionice) {
    std::map<Predavac*, std::vector<Slot*>> predavacSlots;
    std::map<Ucionica*, std::vector<Slot*>> ucionicaSlots;

    for (auto& slot : resenje) {
        if (slot.kurs == nullptr) {
            continue;
        }

        // Provjerava se da li predavač drži dva kursa u istom terminu
        for (auto& slotPredavaca : predavacSlots[slot.predavac]) {
            if ((slotPredavaca->pocetak <= slot.kraj && slotPredavaca->kraj >= slot.pocetak) ||
                (slot.pocetak <= slotPredavaca->kraj && slot.kraj >= slotPredavaca->pocetak)) {
                return false;
            }
        }
        predavacSlots[slot.predavac].push_back(&slot);

        // Provjerava se da li je učionica zauzeta u istom terminu
        for (auto& slotUcionice : ucionicaSlots[slot.ucionica]) {
            if ((slotUcionice->pocetak <= slot.kraj && slotUcionice->kraj >= slot.pocetak) ||
                (slot.pocetak <= slotUcionice->kraj && slot.kraj >= slotUcionice->pocetak)) {
                return false;
            }
        }
        ucionicaSlots[slot.ucionica].push_back(&slot);

        // Provjerava se da li predavač može držati čas u datom terminu
        bool predavacDostupan = true;
        for (auto& predavac : predavaci) {
            if (predavac.ime == slot.predavac->ime) {
                for (int i = slot.pocetak; i <= slot.kraj; ++i) {
                    if (predavac.slotovi[i] == 0) {
                        predavacDostupan = false;
                        break;
                    }
                }
                break;
            }
        }

        if (!predavacDostupan) {
            return false;
        }
    }

    return true;
}

double evaluiraj(std::vector<Slot>& resenje) {
    // Broj održanih časova
    int brojOdrzanihCasova = 0;

    // Ukupan broj praznih slotova
    int ukupanBrojPraznihSlotova = 0;

    // Prođimo kroz sve slotove
    for (auto& slot : resenje) {
        if (slot.predavac != nullptr && slot.kurs != nullptr) {
            brojOdrzanihCasova++;
        } else {
            ukupanBrojPraznihSlotova++;
        }
    }

    // Izračunamo evaluaciju tako da oduzmemo ukupan broj praznih slotova od broja održanih časova
    // Više održanih časova i manje praznih slotova rezultira većom ocjenom
    double ocjena = brojOdrzanihCasova - ukupanBrojPraznihSlotova;

    return ocjena;
}

std::vector<Slot> generisiNovoResenje(std::vector<Slot>& trenutnoResenje, std::vector<Predavac>& predavaci, std::vector<Kurs>& kursevi, std::vector<Ucionica>& ucionice, std::vector<std::vector<int>>& dostupnostUcionica) {
    // Kopirajte trenutno rešenje u novo rešenje
    std::vector<Slot> novoResenje = trenutnoResenje;

    // Izaberite nasumično kurs iz trenutnog rasporeda
    int indeks = rand() % trenutnoResenje.size();
    Kurs& kurs = kursevi[indeks];

    // Pronađite predavača za izabrani kurs
    Predavac predavacKursa;
    for(const auto& predavac: predavaci){
        if(predavac.ime == kurs.predavac){
            predavacKursa = predavac;
            break;
        }
    }

    // Izaberite novu učionicu i slot
    int novaUcionica = rand() % ucionice.size();
    int noviSlot = rand() % (5*10); // za 5 dana sa 10 slotova

    // Proverite da li su nova učionica i slot dostupni i da li je predavač dostupan
    bool ucionicaDostupna = dostupnostUcionica[novaUcionica][noviSlot] == 1;
    bool predavacDostupan = std::all_of(predavacKursa.slotovi.begin() + noviSlot, predavacKursa.slotovi.begin() + noviSlot + kurs.trajanje, [](int i) {return i==1;});

    // Proverite da li je predavač već zauzet u novom slotu držeći neki drugi kurs
    bool predavacZauzet = false;
    for(const auto& slot: novoResenje) {
    if(slot.predavac->ime == predavacKursa.ime && slot.pocetak == noviSlot) {
        predavacZauzet = true;
        break;
        }
    }


      if (ucionicaDostupna && predavacDostupan && !predavacZauzet) {
        // Ako jesu, ažurirajte kurs u novom rešenju
        novoResenje[indeks].ucionica = &ucionice[novaUcionica];
        novoResenje[indeks].pocetak = noviSlot;
    }

    return novoResenje;
}

// Globalna varijabla
int brojacIteracija = 0;
void lokalnaPretraga(std::vector<Predavac>& predavaci, std::vector<Kurs>& kursevi, std::vector<Ucionica>& ucionice, int maxIteracija, std::vector<std::vector<int>>& dostupnostUcionica) {
    // Resetujte brojacIteracija na 0
    brojacIteracija = 0;

    // Generisite pocetno resenje
    std::vector<Slot> trenutnoResenje = generisiPocetnoResenje(predavaci, kursevi, ucionice, dostupnostUcionica);

    // Glavna petlja lokalne pretrage
    while (!jeKriterijumZaustavljanjaIspunjen(maxIteracija)) {
        // Inkrementirajte brojacIteracija
        brojacIteracija++;

        // Generisite novo resenje
        std::vector<Slot> novoResenje = generisiNovoResenje(trenutnoResenje, predavaci, kursevi, ucionice, dostupnostUcionica);

        // Ako je novo resenje validno i bolje od trenutnog, prihvatite ga
        if (jeValidno(novoResenje, predavaci, kursevi, ucionice, dostupnostUcionica) && evaluiraj(novoResenje) > evaluiraj(trenutnoResenje)) {
            trenutnoResenje = novoResenje;
        }
    }
}

//


int main() {
    // Konfiguracija
    int broj_instanci = 10;
/*
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
    }*/
        srand(time(0)); // inicijalizacija generatora nasumičnih brojeva

    // Pretpostavimo da su ovi podaci učitani iz datoteke
    std::vector<Predavac> predavaci; // Vaša lista predavača
    std::vector<Kurs> kursevi; // Vaša lista kurseva
    std::vector<Ucionica> ucionice; // Vaša lista učionica
    std::vector<std::vector<int>> dostupnostUcionica; // Vaša matrica dostupnosti učionica

    int brojIteracija = 1000;

    // Generisanje početnog rešenja
    std::vector<Slot> resenje = generisiPocetnoResenje(predavaci, kursevi, ucionice, dostupnostUcionica);

    // Izvođenje lokalne pretrage
    lokalnaPretraga(resenje, predavaci, kursevi, ucionice, dostupnostUcionica, brojIteracija);

    // Ispisivanje rešenja
    std::cout << "Raspored:\n";
    for (const auto& slot : resenje) {
        std::cout << "Kurs: " << slot.kurs << ", Predavac: " << slot.predavac << ", Ucionica: " << slot.ucionica << ", Slot: " << slot.slot << "\n";
    }


    return 0;
}
