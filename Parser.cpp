#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>


Parser::Parser(const string &filename) : _initCheck(this) {
    try {
        TiXmlDocument doc;
        //Kijkt na of de file is ingeladen
        ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
        //Kijkt na of er een root aanwezig is
        TiXmlElement *root = doc.RootElement();
        ENSURE(root != NULL, "Failed to load file: No root element.");
        vector<VaccinatieCentrum *> vaccinatieCentra;
        //Loop over alle kinderen van root
        for (TiXmlElement *firstElement = root->FirstChildElement(); firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
            string firstName = firstElement->Value();
            //Vacinatiecentrum aanmaken
            if (firstName == "VACCINATIECENTRUM") {
                bool accepting = true;
                int inwoners = -1;
                int capaciteit = -1;
                //Loop over alle atributen van Vaccinatiecentrum
                if (firstElement->FirstChildElement("naam") == NULL) {
                    accepting = false;
                    throw 'A';
                }
                string naam = firstElement->FirstChildElement("naam")->GetText();

                if (firstElement->FirstChildElement("adres") == NULL) {
                    accepting = false;
                    throw 'A';
                }
                string adres = firstElement->FirstChildElement("adres")->GetText();

                if (firstElement->FirstChildElement("inwoners") == NULL) {
                    accepting = false;
                    throw 'A';
                };
                stringstream g(firstElement->FirstChildElement("inwoners")->GetText());
                g >> inwoners;

                if (firstElement->FirstChildElement("capaciteit") == NULL) {
                    accepting = false;
                    throw 'A';
                }
                stringstream h(firstElement->FirstChildElement("capaciteit")->GetText());
                h >> capaciteit;

                //variabelen moeten een waarde hebben gekregen
                if (naam.empty() || adres.empty() || inwoners < 0 || capaciteit <= 0) {
                    accepting = false;
                    throw 'A';
                }
                if (accepting) {
                    VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
                    vaccinatieCentra.push_back(V);
                }
                for(TiXmlElement *secondElement = firstElement->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
                    string secondName = secondElement->Value();
                    if(secondName != "naam" && secondName != "adres" && secondName != "capaciteit" && secondName != "inwoners") {
                        throw 'B';
                    }
                }
            }
            else if(firstName != "HUB"){
                throw 'B';
            }
        }
        ENSURE(vaccinatieCentra.size() != 0, "Er is geen vaccinatiecentrum gegeven.");
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
        H->addFverbondenCentra(vaccinatieCentra);
        fhub = H;
        ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
        for(TiXmlElement *secondElement = hub->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
            string secondName = secondElement->Value();
            if(secondName != "levering" && secondName != "interval" && secondName != "transport" && secondName != "CENTRA") {
                throw 'B';
            }
        }
        for(TiXmlElement *thirdElement = hub->FirstChildElement("CENTRA")->FirstChildElement(); thirdElement != NULL; thirdElement = thirdElement->NextSiblingElement()) {
            string thirdName = thirdElement->Value();
            if(thirdName != "centrum") {
                throw 'B';
            }
        }
    }
    catch(char e){
        if(e == 'A'){
            cout << "Sommige variabelen van een VACCINATIECENTRUM werden niet correct meegegeven." << endl;
        }
        else if(e == 'B'){
            cout << "Er bevinden zich onherkenbare elementen in het bestand." << endl;
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
