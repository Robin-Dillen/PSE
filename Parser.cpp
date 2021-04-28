//============================================================================
// Name        : Parser.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : parser for xml files
//============================================================================

#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "VaccinatieCentrum.h"
#include "Hub.h"
#include "Utils.h"
#include "Vaccins.h"

Parser::Parser(const string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    VaccinsFactorySingleton &vaccin_factory = VaccinsFactorySingleton::getInstance();
    TiXmlText space = " ";
    //Kijkt na of de file is ingeladen
    ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();
    //ENSURE(root != NULL, "Failed to load file: No root element.");
    // doc.RootElement(); geeft de eerste tag terug dus we moeten testen of die tag geen siblings heeft
    ENSURE(root->NextSibling() == NULL, "Er is geen root aanwezig in het document!");
    // checkt of er vaccinatiecentra zijn
    ENSURE(root->FirstChildElement("VACCINATIECENTRUM") != NULL, "Er zijn geen vaccinatiecentra.");

    map<string, VaccinatieCentrum *> vaccinatieCentra;
    //Loop over alle kinderen van root
    for (TiXmlElement *centrum = root->FirstChildElement();
         centrum != NULL; centrum = centrum->NextSiblingElement()) {
        string centrumName = centrum->Value();

        if (centrumName == "VACCINATIECENTRUM") {
            //Vacinatiecentrum aanmaken
            bool valid = true; // checkt of het vaccinatie centrum geldig is
            string naam;
            string adres;
            int inwoners = -1;
            int capaciteit = -1;
            //Loop over alle atributen van het Vaccinatiecentrum

            // naam moet bestaan
            if (centrum->FirstChildElement("naam") == NULL) {
                valid = false;
                cerr << "de tag naam"
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << locationToString(centrum) << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else if (centrum->FirstChildElement("naam")->GetText() == NULL) {
                valid = false;
                cerr << "de naam" << locationToString(centrum->FirstChildElement("naam"))
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << " heeft een niet toegestaane waarde(leeg)." << endl;
                errors.push_back(WRONG_VALUE);
            } else {
                naam = centrum->FirstChildElement("naam")->GetText();
            }

            // adres moet bestaan
            if (centrum->FirstChildElement("adres") == NULL) {
                valid = false;
                cerr << "het adres"
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);

            } else if (centrum->FirstChildElement("adres")->GetText() == NULL) {
                valid = false;
                cerr << "het adres" << locationToString(centrum->FirstChildElement("adres"))
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << " heeft een niet toegestaane waarde(leeg)." << endl;
                errors.push_back(WRONG_VALUE);
            } else {
                centrum->FirstChildElement("adres")->InsertEndChild(space);
                adres = centrum->FirstChildElement("adres")->GetText();
            }

            // inwoners moet bestaan
            if (centrum->FirstChildElement("inwoners") == NULL) {
                valid = false;
                cerr << "het aantal inwoners"
                     << " van het vaccinatiecentrum" << locationToString(centrum) << " werd niet meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else if (centrum->FirstChildElement("inwoners")->GetText() == NULL) {
                valid = false;
                cerr << "het aantal inwoners" << locationToString(centrum->FirstChildElement("inwoners"))
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << " heeft een niet toegestaane waarde(negatief)." << endl;
                errors.push_back(WRONG_VALUE);
            } else {
                inwoners = to_int(centrum->FirstChildElement("inwoners")->GetText());
                if (inwoners < 0) {
                    valid = false;
                    cerr << "het aantal inwoners" << locationToString(centrum->FirstChildElement("inwoners"))
                         << " van het vaccinatiecentrum" << locationToString(centrum)
                         << " heeft een niet toegestaane waarde(negatief)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
            }

            // capaciteit moet bestaan
            if (centrum->FirstChildElement("capaciteit") == NULL) {
                valid = false;
                cerr << "de capaciteit van het vaccinatiecentrum " << locationToString(centrum)
                     << " werd niet meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else if (centrum->FirstChildElement("capaciteit")->GetText() == NULL) {
                valid = false;
                cerr << "de capaciteit" << locationToString(centrum->FirstChildElement("inwoners"))
                     << " van het vaccinatiecentrum" << locationToString(centrum)
                     << " heeft een niet toegestaane waarde(negatief)." << endl;
                errors.push_back(WRONG_VALUE);
            } else {
                capaciteit = to_int(centrum->FirstChildElement("capaciteit")->GetText());
                if (capaciteit < 0) {
                    valid = false;
                    cerr << "de capaciteit" << locationToString(centrum->FirstChildElement("inwoners"))
                         << " van het vaccinatiecentrum" << locationToString(centrum)
                         << " heeft een niet toegestaane waarde(negatief)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
            }
            if ((capaciteit == 0 && inwoners > 0)) {
                cerr
                        << "de capaciteit" << centrum->FirstChildElement("capaciteit")->Row() << ", "
                        << centrum->FirstChildElement("capaciteit")->Column() << ") van het vaccinatiecentrum, op rij: "
                        << centrum->Row() << " en kolom: " << centrum->Column()
                        << " heeft een niet toegestaane waarde(0 en aantal inwoners"
                        << centrum->FirstChildElement("inwoners")->Row() << ", "
                        << centrum->FirstChildElement("inwoners")->Column() << ") niet 0)."
                        << endl;
                errors.push_back(WRONG_VALUE);
            }

            // check voor attributen met een niet herkende tag
            for (TiXmlElement *secondElement = centrum->FirstChildElement();
                 secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
                string secondName = secondElement->Value();
                if (secondName != "naam" && secondName != "adres" && secondName != "capaciteit" &&
                        secondName != "inwoners") {
                    cerr << secondName << "(" << secondElement->Row() << ", " << secondElement->Column()
                            << ") wordt niet herkent als tag!" << endl;
                    errors.push_back(UNKNOWN_TAG);
                }
            }
            // check of de naam van het vaccinatiecentrum uniek is
            if (vaccinatieCentra.find(naam) != vaccinatieCentra.end()) {
                cerr << "(" << centrum->Row() << ", " << centrum->Column()
                        << ") Er mogen geen centra met dezelfde naam zijn!" << endl;
                errors.push_back(DUPLICATE_NAME);
                valid = false;
            }
            if (!valid) continue;

            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
            vaccinatieCentra[naam] = V;

        } else if (centrumName != "HUB") {
            cerr << "Het element " << centrumName << locationToString(centrum) << " wordt niet herkent!" << endl;
            errors.push_back(UNKNOWN_TAG);
        }
    }

    ENSURE(vaccinatieCentra.size() > 0, "Er moet minstens 1(geldig) vaccinatiecentrum zijn!");

    ENSURE(root->FirstChildElement("HUB") != NULL, "Er is geen HUB aanwezig");

    // maak de hubs aan
    for (TiXmlElement *hub = root->FirstChildElement("HUB"); hub != NULL; hub = hub->NextSiblingElement("HUB")) {

        ENSURE(hub->FirstChildElement("CENTRA") != NULL,
               ("Hub" + locationToString(hub) + "bevat geen 'CENTRA' tag").c_str());

        ENSURE(hub->FirstChildElement("VACCIN") != NULL,
               ("Hub" + locationToString(hub) + " moet een vaccin gedefineerd hebben").c_str());
        // lees vaccins in
        map<string, Vaccin *> vaccins;
        for (TiXmlElement *vaccin = hub->FirstChildElement("VACCIN");
             vaccin != NULL; vaccin = vaccin->NextSiblingElement("VACCIN")) {
            bool correct = true;
            string naam = "";
            int levering = -1;
            int interval = -1;
            int transport = -1;
            int hernieuwing = 0;
            int temperatuur = 25;

            //ENSURE(vaccin->FirstChildElement("type") != NULL, ("Type van het vaccin" + locationToString(vaccin) + " bestaat niet!").c_str());
            if (vaccin->FirstChildElement("type") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "het element 'type' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("type")->GetText() == NULL) {
                correct = false;
                cerr << "Het type" << locationToString(vaccin->FirstChildElement("type"))
                     << " van het vaccin" << locationToString(vaccin)
                     << " heeft een niet toegestaane waarde (leeg)." << endl;
                errors.push_back(WRONG_VALUE);
            } else {
                naam = vaccin->FirstChildElement("type")->GetText();
            }
            //ENSURE(vaccin->FirstChildElement("levering") != NULL, "...");
            if (vaccin->FirstChildElement("levering") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'levering' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("levering")->GetText() == NULL) {
                correct = false;
                cerr << "de levering" + locationToString(vaccin->FirstChildElement("type")) +
                        " van het vaccin heeft een ongeldige waarde";
                errors.push_back(WRONG_VALUE);
            } else {
                levering = to_int(vaccin->FirstChildElement("levering")->GetText());
            }
            if (vaccin->FirstChildElement("interval") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'interval' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("interval")->GetText() == NULL) {
                errors.push_back(WRONG_VALUE);
                cerr << "Het element 'interval'" + locationToString(vaccin->FirstChildElement("type")) +
                        " van het vaccin heeft een ongeldige waarde";
                correct = false;
            } else {
                interval = to_int(vaccin->FirstChildElement("interval")->GetText());
                if (interval <= 0) {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'interval'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
            }

            if (vaccin->FirstChildElement("transport") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'transport' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("transport")->GetText() == NULL) {
                errors.push_back(WRONG_VALUE);
                cerr << "Het element 'transport'" + locationToString(vaccin->FirstChildElement("type")) +
                        " van het vaccin heeft een ongeldige waarde";
                correct = false;
            } else {
                transport = to_int(vaccin->FirstChildElement("transport")->GetText());
                if (transport <= 0) {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'transport'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
            }
            if (vaccin->FirstChildElement("hernieuwing") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'hernieuwing' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("hernieuwing")->GetText() == NULL) {
                errors.push_back(WRONG_VALUE);
                cerr << "Het element 'hernieuwing'" + locationToString(vaccin->FirstChildElement("type")) +
                        " van het vaccin heeft een ongeldige waarde";
                correct = false;
            } else {
                hernieuwing = to_int(vaccin->FirstChildElement("hernieuwing")->GetText());
                if (hernieuwing < 0) {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'hernieuwing'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
            }

            if (vaccin->FirstChildElement("temperatuur") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'temperatuur' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            } else if (vaccin->FirstChildElement("temperatuur")->GetText() == NULL) {
                errors.push_back(WRONG_VALUE);
                cerr << "Het element 'temperatuur'" + locationToString(vaccin->FirstChildElement("type")) +
                        " van het vaccin heeft een ongeldige waarde";
                correct = false;
            } else {
                temperatuur = to_int(vaccin->FirstChildElement("temperatuur")->GetText());
            }
            if (!correct) continue;

            vaccins[naam] = vaccin_factory.getVaccin(naam, levering, interval, transport, hernieuwing, temperatuur);

            for (TiXmlElement *el = vaccin->FirstChildElement();
                 el != NULL; el = el->NextSiblingElement("VACCIN")) {
                string el_name = el->Value();
                if (el_name != "type" && el_name != "levering" && el_name != "interval" && el_name != "transport" &&
                    el_name != "hernieuwing" && el_name != "temperatuur") {
                    errors.push_back(UNKNOWN_TAG);
                    cerr << "Het element " << el_name << locationToString(el) << " wordt niet herkent!";
                }
            }

        }
        ENSURE(vaccins.size() > 0, ("Er zijn geen geldige vaccin voor hub" + locationToString(hub) + " !").c_str());

        Hub *H = new Hub(vaccins);
        fhubs.push_back(H);

        for (
                TiXmlElement *thirdElement = hub->FirstChildElement("CENTRA")->FirstChildElement();
                thirdElement != NULL;
                thirdElement = thirdElement->NextSiblingElement()
                ) {
            // koppel de hubs aan hun vaccinatiecentra

            if(thirdElement->GetText() == NULL){
                cerr << "Een vaccinatiecentrum werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else if(vaccinatieCentra[thirdElement->GetText()] != NULL){
                H->addCentrum(vaccinatieCentra[thirdElement->GetText()]);
//                for(map<string, Vaccin*>::iterator it = vaccins.begin(); it != vaccins.end(); it++){
//                    vaccinatieCentra[thirdElement->GetText()]->addVaccinType(vaccins[it->first]);
//                }
            }

            // check of er extra, foute, tags aanwezig zijn
            string thirdName = thirdElement->Value();
            if (thirdName != "centrum") {
                cerr << "Er zit een foute tag op locatie: " << locationToString(thirdElement) << " !" << endl;
                errors.push_back(WRONG_TAG);
            }
        }

        // check voor niet herkende tags
        for (
                TiXmlElement *secondElement = hub->FirstChildElement();
                secondElement != NULL;
                secondElement = secondElement->NextSiblingElement()
                ) {
            string secondName = secondElement->Value();
            if (secondName != "levering" && secondName != "interval" && secondName != "transport" &&
                secondName != "CENTRA" && secondName != "VACCIN") {
                cerr << secondName << "(" << secondElement->Row() << ", " << secondElement->Column()
                     << ") wordt niet herkent als tag!" << endl;
                errors.push_back(UNKNOWN_TAG);
            }
        }
    }

    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

Parser::Parser() : _initCheck(this) {
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

vector<Hub *> Parser::getFhubs() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
    return fhubs;
}

bool Parser::isProperlyInitialized() const {
    return _initCheck == this;
}

Parser::~Parser() {
    for (unsigned int i = 0; i < fhubs.size(); i++) {
        delete fhubs[i];
    }
}

int Parser::errorOccured(EExeption error) const {
    int count_ = count(errors.begin(), errors.end(), error);
    return count_;
}

string Parser::locationToString(TiXmlElement *el) {
    return "(" + to_string(el->Row()) + ", " + to_string(el->Column()) + ")";
}
