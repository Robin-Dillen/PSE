#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>

Parser::Parser(const string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    //Kijkt na of de file is ingeladen
    ENSURE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();

    ENSURE(root != NULL, "Failed to load file: No root element.");

    vector<VaccinatieCentrum *> vaccinatieCentra;
    //Loop over alle kinderen van root
    for (TiXmlElement *firstElement = root->FirstChildElement();
         firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
        string firstName = firstElement->Value();
        //Vacinatiecentrum aanmaken
        if (firstName == "VACCINATIECENTRUM") {
            int inwoners = -1;
            int capaciteit = -1;
            //Loop over alle atributen van Vaccinatiecentrum
            ENSURE(firstElement->FirstChildElement("naam") != NULL, "naam does not exist");
            string naam = firstElement->FirstChildElement("naam")->GetText();

            ENSURE(firstElement->FirstChildElement("adres") != NULL, "adres does not exist");
            string adres = firstElement->FirstChildElement("adres")->GetText();

            ENSURE(firstElement->FirstChildElement("inwoners") != NULL, "inwoners does not exist");
            stringstream g(firstElement->FirstChildElement("inwoners")->GetText());
            g >> inwoners;

            ENSURE(firstElement->FirstChildElement("capaciteit") != NULL, "capaciteit does not exist");
            stringstream h(firstElement->FirstChildElement("capaciteit")->GetText());
            h >> capaciteit;

            //variabelen moeten een waarde hebben gekregen
            ENSURE(!naam.empty() && !adres.empty() && inwoners >= 0 &&
                   (capaciteit > 0 || (capaciteit == 0 && inwoners == 0)),
                   "Sommige variabelen van VACCINATIECENTRUM werden niet correct meegegeven.");
            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
            vaccinatieCentra.push_back(V);
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

    ENSURE(levering > 0 && interval > 0 && transport > 0,
           "Sommige variabelen van HUB werden niet correct meegegeven.");

    Hub *H = new Hub(levering, interval, transport);
    H->addFverbondenCentra(vaccinatieCentra);
    fhub = H;
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");

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
