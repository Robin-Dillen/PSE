#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>

//Parser::Parser(const string &filename) {
//    TiXmlDocument doc;
//    //Kijkt na of de file is ingeladen
//    ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
//    //Kijkt na of er een root aanwezig is
//    TiXmlElement *root = doc.RootElement();
//    ENSURE(root != NULL, "Failed to load file: No root element.");
//    //Hub aanmaken
//    ENSURE(root->FirstChildElement("HUB") != NULL, "Failed to load file: No HUB element.");
//    TiXmlElement *hub = root->FirstChildElement("HUB");
//    int levering = -1;
//    int interval = -1;
//    int transport = -1;
//
//    ENSURE(hub->FirstChildElement("levering") != NULL, "levering does not exist");
//    stringstream g(hub->FirstChildElement("levering")->GetText());
//    g >> levering;
//
//    ENSURE(hub->FirstChildElement("interval") != NULL, "interval does not exist");
//    stringstream h(hub->FirstChildElement("interval")->GetText());
//    h >> interval;
//
//    ENSURE(hub->FirstChildElement("transport") != NULL, "Transport does not exist");
//    stringstream i(hub->FirstChildElement("transport")->GetText());
//    i >> transport;
//
//    ENSURE(hub->FirstChildElement("CENTRA") != NULL, "Centra does not exist");
//
//    ENSURE(levering > 0 && interval > 0 && transport > 0,
//           "Sommige variabelen van HUB werden niet correct meegegeven.");
//
//    Hub *H = new Hub(levering, interval, transport);
//    vector<VaccinatieCentrum *> vaccinatieCentra;
//    //Loop over alle kinderen van root
//    for (TiXmlElement *firstElement = root->FirstChildElement(); firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
//        string firstName = firstElement->Value();
//        //Vacinatiecentrum aanmaken
//        if (firstName == "VACCINATIECENTRUM") {
//            bool valid = true;
//            int inwoners = -1;
//            int capaciteit = -1;
//            //Loop over alle atributen van Vaccinatiecentrum
//            if (firstElement->FirstChildElement("naam") == NULL) {
//                valid = false;
//                cerr << "De naam van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
//                errors.push_back(MISSING_TAG);
//            }
//            string naam = firstElement->FirstChildElement("naam")->GetText();
//
//            if (firstElement->FirstChildElement("adres") == NULL) {
//                valid = false;
//                cerr << "het adres van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
//                errors.push_back(MISSING_TAG);
//                continue;
//            }
//            string adres = firstElement->FirstChildElement("adres")->GetText();
//
//            if (firstElement->FirstChildElement("inwoners") == NULL) {
//                valid = false;
//                cerr << "de inwoners van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
//                errors.push_back(MISSING_TAG);
//                continue;
//            };
//            stringstream j(firstElement->FirstChildElement("inwoners")->GetText());
//            j >> inwoners;
//
//            if (firstElement->FirstChildElement("capaciteit") == NULL) {
//                valid = false;
//                cerr << "de capaciteit van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
//                errors.push_back(MISSING_TAG);
//                continue;
//            }
//            stringstream k(firstElement->FirstChildElement("capaciteit")->GetText());
//            k >> capaciteit;
//
//            //variabelen moeten een Juiste waarde hebben gekregen
//            if (naam.empty() || adres.empty() || inwoners < 0 || capaciteit < 0 || (capaciteit == 0 && inwoners != 0)) {
//                valid = false;
//                if (naam.empty())
//                    cerr << "de naam van een VACCINATIECENTRUM heeft een niet toegestaane waarde(leeg)." << endl;
//                if (adres.empty())
//                    cerr << "het adres van een VACCINATIECENTRUM heeft een niet toegestaane waarde(leeg)." << endl;
//                if (inwoners < 0)
//                    cerr << "het aantal van een VACCINATIECENTRUM heeft een niet toegestaane waarde(negatief)." << endl;
//                if (capaciteit < 0)
//                    cerr << "de capaciteit van een VACCINATIECENTRUM heeft een niet toegestaane waarde(negatief)."
//                         << endl;
//                if ((capaciteit == 0 && inwoners != 0))
//                    cerr
//                            << "de capaciteit van een VACCINATIECENTRUM heeft een niet toegestaane waarde(0 en aantal inwoners niet 0)."
//                            << endl;
//                errors.push_back(WRONG_VALUE);
//                continue;
//            }
//            if (!valid) continue;
//
//            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
//            vaccinatieCentra.push_back(V);
//
//            for (TiXmlElement *secondElement = firstElement->FirstChildElement();
//                 secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
//                string secondName = secondElement->Value();
//                if (secondName != "naam" || secondName != "adres" || secondName != "capaciteit" ||
//                    secondName != "inwoners") {
//                    cerr << secondName << " wordt niet herkent als tag!" << endl;
//                    errors.push_back(UNKNOWN_TAG);
//                }
//            }
//        }
//        else if(firstName != "HUB") {
//            cerr << firstName << " wordt niet herkent als element!" << endl;
//            errors.push_back(UNKNOWN_ELEMENT);
//        }
//    }
//    ENSURE(vaccinatieCentra.size() != 0, "Er is geen vaccinatiecentrum gegeven.");
//    H->addFverbondenCentra(vaccinatieCentra);
//    fhub = H;
//    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
//    for(TiXmlElement *secondElement = hub->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
//        string secondName = secondElement->Value();
//        if (secondName != "levering" || secondName != "interval" || secondName != "transport" ||
//            secondName != "CENTRA") {
//            cerr << secondName << " wordt niet herkent als tag!" << endl;
//            errors.push_back(UNKNOWN_TAG);
//        }
//    }
//    for(TiXmlElement *thirdElement = hub->FirstChildElement("CENTRA")->FirstChildElement(); thirdElement != NULL; thirdElement = thirdElement->NextSiblingElement()) {
//        string thirdName = thirdElement->Value();
//        if(thirdName != "centrum") {
//            cerr << "Er zijn geen centra megegeven!" << endl;
//            errors.push_back(NO_CENTRA);
//        }
//    }
//}

Parser::Parser(const string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    //Kijkt na of de file is ingeladen
    ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();
    ENSURE(root != NULL, "Failed to load file: No root element.");
    // checkt of er vaccinatiecentra zijn
    ENSURE(root->FirstChildElement("VACCINATIECENTRUM") != NULL, "Er zijn geen vaccinatiecentra.");

    map<string, VaccinatieCentrum *> vaccinatieCentra;
    //Loop over alle kinderen van root met de waarde VACCINATIECENTRUM
    for (TiXmlElement *centrum = root->FirstChildElement();
         centrum != NULL; centrum = centrum->NextSiblingElement()) {
        string centrumName = centrum->Value();
        if (centrumName == "VACCINATIECENTRUM") {
            //Vacinatiecentrum aanmaken
            string naam;
            string adres;
            bool valid = true;
            int inwoners = -1;
            int capaciteit = -1;
            //Loop over alle atributen van Vaccinatiecentrum
            if (centrum->FirstChildElement("naam")->GetText() == NULL) {
                valid = false;
                cerr << "de naam" << locationToString(centrum->FirstChildElement("adres"))
                     << " van het vaccinatiecentrum"
                     << locationToString(centrum) << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else {
                naam = centrum->FirstChildElement("naam")->GetText();
            }

            // adres moet bestaan
            if (centrum->FirstChildElement("adres")->GetText() == NULL) {
                valid = false;
                cerr << "het adres" << locationToString(centrum->FirstChildElement("adres"))
                     << " van het vaccinatiecentrum"
                     << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else {
                adres = centrum->FirstChildElement("adres")->GetText();
            }
            // inwoners moet bestaan
            if (centrum->FirstChildElement("inwoners")->GetText() == NULL) {
                valid = false;
                cerr << "het aantal inwonders" << locationToString(centrum->FirstChildElement("inwoners"))
                     << " van het vaccinatiecentrum" << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else {
                stringstream j(centrum->FirstChildElement("inwoners")->GetText());
                j >> inwoners;
                if (inwoners < 0) {
                    cerr << "het aantal inwonders" << locationToString(centrum->FirstChildElement("inwoners"))
                         << " van het vaccinatiecentrum" << locationToString(centrum)
                         << " heeft een niet toegestaane waarde(negatief)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
            }

            // capaciteit moet bestaan
            if (centrum->FirstChildElement("capaciteit")->GetText() == NULL) {
                valid = false;
                cerr << "de capaciteit" << locationToString(centrum->FirstChildElement("inwoners"))
                     << " van het vaccinatiecentrum" << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else {
                stringstream j(centrum->FirstChildElement("capaciteit")->GetText());
                j >> capaciteit;
                if (capaciteit < 0) {
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

            // check voor attributen met een net herkende tag
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
            // check of de naam van het vaccinatiecentra uniek is
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
            errors.push_back(UNKNOWN_TAG);
        }
    }

    ENSURE(vaccinatieCentra.size() > 0, "Er moet minstens 1(geldig) vaccinatiecentrum zijn!");

    ENSURE(root->FirstChildElement("HUB") != NULL, "Er is geen HUB aanwezig");

    // maak de hubs aan
    for (TiXmlElement *hub = root->FirstChildElement("HUB"); hub != NULL; hub = hub->NextSiblingElement("HUB")) {
        int levering = -1;
        int interval = -1;
        int transport = -1;

        // leveringen moet bestaan
        ENSURE(hub->FirstChildElement("levering") != NULL, "levering does not exist");
        ENSURE(hub->FirstChildElement("levering")->GetText() != NULL, "levering heeft een ongeldige waarde");
        stringstream g(hub->FirstChildElement("levering")->GetText());
        g >> levering;

        // interval moet bestaan
        ENSURE(hub->FirstChildElement("interval") != NULL, "interval does not exist");
        ENSURE(hub->FirstChildElement("interval")->GetText() != NULL, "interval heeft een ongeldige waarde");
        stringstream h(hub->FirstChildElement("interval")->GetText());
        h >> interval;

        // transport moet bestaan
        ENSURE(hub->FirstChildElement("transport") != NULL, "Transport does not exist");
        ENSURE(hub->FirstChildElement("transport")->GetText() != NULL, "Transport heeft een ongeldige waarde");
        stringstream i(hub->FirstChildElement("transport")->GetText());
        i >> transport;

        ENSURE(levering > 0,
               ("Levering" + locationToString(hub->FirstChildElement("levering")) + " in hub" + locationToString(hub) +
                "heeft een waarde kleiner dan 1").c_str());
        ENSURE(interval > 0,
               ("Interval" + locationToString(hub->FirstChildElement("interval")) + " in hub" + locationToString(hub) +
                "heeft een waarde kleiner dan 1").c_str());
        ENSURE(transport > 0, ("Transport" + locationToString(hub->FirstChildElement("transport")) + " in hub" +
                               locationToString(hub) + "heeft een waarde kleiner dan 1").c_str());
        ENSURE(hub->FirstChildElement("CENTRA") != NULL,
               ("Hub" + locationToString(hub) + "bevat geen 'CENTRA' tag").c_str());


        Hub *H = new Hub(levering, interval, transport);
        fhubs.push_back(H);

        for (
                TiXmlElement *thirdElement = hub->FirstChildElement("CENTRA")->FirstChildElement();
                thirdElement != NULL;
                thirdElement = thirdElement->NextSiblingElement()
                ) {
            if(thirdElement->GetText() == NULL){
                cerr << "Een vaccinatiecentrum werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            }
            else if(vaccinatieCentra[thirdElement->GetText()] != NULL){
                H->addCentrum(vaccinatieCentra[thirdElement->GetText()]);
            }

            string thirdName = thirdElement->Value();
            if (thirdName != "centrum") {
                cerr << "Er zit een foute tag op locatie: " << locationToString(thirdElement) << " !" << endl;
                errors.push_back(WRONG_TAG);
            }
        }


        for (
                TiXmlElement *secondElement = hub->FirstChildElement();
                secondElement != NULL;
                secondElement = secondElement->NextSiblingElement()
                ) {
            string secondName = secondElement->Value();
            if (secondName != "levering" && secondName != "interval" && secondName != "transport" &&
                secondName != "CENTRA") {
                cerr << secondName << "(" << secondElement->Row() << ", " << secondElement->Column()
                     << ") wordt niet herkent als tag!" << endl;
                errors.push_back(UNKNOWN_TAG);
            }
        }
    }

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

int Parser::errorOccured(char error) const {
    int count_ = count(errors.begin(), errors.end(), error);
    return count_;
}

string Parser::locationToString(TiXmlElement *el) {
    return "(" + to_string(el->Row()) + ", " + to_string(el->Column()) + ")";
}
