#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>


string toString(string s){
    return s;
}

Parser::Parser(const string &filename) {
    TiXmlDocument doc;
    //Kijkt na of de file is ingeladen
    ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();
    ENSURE(root != NULL, "Failed to load file: No root element.");
    //Hub aanmaken
    ENSURE(root->FirstChildElement("HUB") != NULL, "Failed to load file: No HUB element.");
    TiXmlElement *hub = root->FirstChildElement("HUB");
    int levering = 0;
    int interval = 0;
    int transport = 0;

    ENSURE(hub->FirstChildElement("levering") != NULL, "levering does not exist");
    stringstream g(hub->FirstChildElement("levering")->GetText());
    g >> levering;

    ENSURE(hub->FirstChildElement("interval") != NULL, "interval does not exist");
    stringstream h(hub->FirstChildElement("interval")->GetText());
    h >> interval;

    ENSURE(hub->FirstChildElement("transport") != NULL, "Transport does not exist");
    stringstream i(hub->FirstChildElement("transport")->GetText());
    i >> transport;

    ENSURE(hub->FirstChildElement("CENTRA") != NULL, "Centra does not exist");

    ENSURE(levering > 0 && interval > 0 && transport > 0,
           "Sommige variabelen van HUB werden niet correct meegegeven.");

    Hub *H = new Hub(levering, interval, transport);
    vector<VaccinatieCentrum *> vaccinatieCentra;
    //Loop over alle kinderen van root
    for (TiXmlElement *firstElement = root->FirstChildElement(); firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
        string firstName = firstElement->Value();
        //Vacinatiecentrum aanmaken
        if (firstName == "VACCINATIECENTRUM") {
            bool valid = true;
            int inwoners = -1;
            int capaciteit = -1;
            //Loop over alle atributen van Vaccinatiecentrum
            if (firstElement->FirstChildElement("naam") == NULL) {
                valid = false;
                cerr << "De naam van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
                errors.push_back(NO_TAG);
            }
            string naam = firstElement->FirstChildElement("naam")->GetText();

            if (firstElement->FirstChildElement("adres") == NULL) {
                valid = false;
                cerr << "het adres van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
                errors.push_back(NO_TAG);
                continue;
            }
            string adres = firstElement->FirstChildElement("adres")->GetText();

            if (firstElement->FirstChildElement("inwoners") == NULL) {
                valid = false;
                cerr << "de inwoners van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
                errors.push_back(NO_TAG);
                continue;
            };
            stringstream j(firstElement->FirstChildElement("inwoners")->GetText());
            j >> inwoners;

            if (firstElement->FirstChildElement("capaciteit") == NULL) {
                valid = false;
                cerr << "de capaciteit van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
                errors.push_back(NO_TAG);
                continue;
            }
            stringstream k(firstElement->FirstChildElement("capaciteit")->GetText());
            k >> capaciteit;

            //variabelen moeten een Juiste waarde hebben gekregen
            if (naam.empty() || adres.empty() || inwoners < 0 || capaciteit < 0 || (capaciteit == 0 && inwoners != 0)) {
                valid = false;
                if (naam.empty())
                    cerr << "de naam van een VACCINATIECENTRUM heeft een niet toegestaane waarde(leeg)." << endl;
                if (adres.empty())
                    cerr << "het adres van een VACCINATIECENTRUM heeft een niet toegestaane waarde(leeg)." << endl;
                if (inwoners < 0)
                    cerr << "het aantal van een VACCINATIECENTRUM heeft een niet toegestaane waarde(negatief)." << endl;
                if (capaciteit < 0)
                    cerr << "de capaciteit van een VACCINATIECENTRUM heeft een niet toegestaane waarde(negatief)."
                         << endl;
                if ((capaciteit == 0 && inwoners != 0))
                    cerr
                            << "de capaciteit van een VACCINATIECENTRUM heeft een niet toegestaane waarde(0 en aantal inwoners niet 0)."
                            << endl;
                errors.push_back(WRONG_VALUE);
                continue;
            }
            if (!valid) continue;

            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
            vaccinatieCentra.push_back(V);

            for (TiXmlElement *secondElement = firstElement->FirstChildElement();
                 secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
                string secondName = secondElement->Value();
                if (secondName != "naam" || secondName != "adres" || secondName != "capaciteit" ||
                    secondName != "inwoners") {
                    cerr << secondName << " wordt niet herkent als tag!" << endl;
                    errors.push_back(UNKNOWN_TAG);
                }
            }
        }
        else if(firstName != "HUB") {
            cerr << firstName << " wordt niet herkent als element!" << endl;
            errors.push_back(UNKNOWN_ELEMENT);
        }
    }
    ENSURE(vaccinatieCentra.size() != 0, "Er is geen vaccinatiecentrum gegeven.");
    H->addFverbondenCentra(vaccinatieCentra);
    fhub = H;
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    for(TiXmlElement *secondElement = hub->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
        string secondName = secondElement->Value();
        if (secondName != "levering" || secondName != "interval" || secondName != "transport" ||
            secondName != "CENTRA") {
            cerr << secondName << " wordt niet herkent als tag!" << endl;
            errors.push_back(UNKNOWN_TAG);
        }
    }
    for(TiXmlElement *thirdElement = hub->FirstChildElement("CENTRA")->FirstChildElement(); thirdElement != NULL; thirdElement = thirdElement->NextSiblingElement()) {
        string thirdName = thirdElement->Value();
        if(thirdName != "centrum") {
            cerr << "Er zijn geen centra megegeven!" << endl;
            errors.push_back(NO_CENTRA);
        }
    }
}

Hub *Parser::getFhub() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
    return fhub;
}

bool Parser::isProperlyInitialized() const {
    return _initCheck == this;
}

Parser::~Parser() {
    delete fhub;
}

int Parser::errorOccured(char error) const {
    int count_ = count(errors.begin(), errors.end(), error);
    return count_;
}
