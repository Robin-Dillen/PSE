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
                cerr << "de naam"
                        << " van het vaccinatiecentrum" << locationToString(centrum)
                        << locationToString(centrum) << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            }else{
                centrum->FirstChildElement("naam")->InsertEndChild(space);
                naam = centrum->FirstChildElement("naam")->GetText();
                if (naam == " ") {
                    valid = false;
                    cerr << "de naam" << locationToString(centrum->FirstChildElement("naam"))
                            << " van het vaccinatiecentrum" << locationToString(centrum)
                            << " heeft een niet toegestaane waarde(leeg)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
                //naam = naam.substr(0, naam.size()-1);
            }

            // adres moet bestaan
            if (centrum->FirstChildElement("adres") == NULL) {
                valid = false;
                cerr << "het adres"
                        << " van het vaccinatiecentrum" << locationToString(centrum)
                        << " werd niet correct meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else {
                centrum->FirstChildElement("adres")->InsertEndChild(space);
                adres = centrum->FirstChildElement("adres")->GetText();
                if (adres == " ") {
                    valid = false;
                    cerr << "het adres" << locationToString(centrum->FirstChildElement("adres"))
                            << " van het vaccinatiecentrum" << locationToString(centrum)
                            << " heeft een niet toegestaane waarde(leeg)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
                //adres = adres.substr(0, adres.size()-1);
            }

            // inwoners moet bestaan
            if (centrum->FirstChildElement("inwoners") == NULL) {
                valid = false;
                cerr << "het aantal inwoners"
                        << " van het vaccinatiecentrum" <<  locationToString(centrum)<< " werd niet meegegeven." << endl;
                errors.push_back(MISSING_TAG);
            } else{
                centrum->FirstChildElement("adres")->InsertEndChild(space);
                string Str_inwoners = centrum->FirstChildElement("inwoners")->GetText();
                if(Str_inwoners == " ") {
                    valid = false;
                    cerr << "het aantal inwoners" << locationToString(centrum->FirstChildElement("inwoners"))
                         << " van het vaccinatiecentrum" << locationToString(centrum)
                         << " heeft een niet toegestaane waarde(negatief)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
                //Str_inwoners = Str_inwoners.substr(0, Str_inwoners.size()-1);
                inwoners = to_int(Str_inwoners);
                cout<<Str_inwoners << endl << inwoners;
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
            } else {
                centrum->FirstChildElement("capaciteit")->InsertEndChild(space);
                string Str_capaciteit = centrum->FirstChildElement("capaciteit")->GetText();
                if (Str_capaciteit == " ") {
                    valid = false;
                    cerr << "de capaciteit" << locationToString(centrum->FirstChildElement("inwoners"))
                         << " van het vaccinatiecentrum" << locationToString(centrum)
                         << " heeft een niet toegestaane waarde(negatief)." << endl;
                    errors.push_back(WRONG_VALUE);
                }
                capaciteit = to_int(Str_capaciteit.substr(0, Str_capaciteit.size()-1));
                cout<<Str_capaciteit << endl << capaciteit;
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
            }
            else{
                vaccin->FirstChildElement("type")->InsertEndChild(space);
                naam = vaccin->FirstChildElement("type")->GetText();
                //ENSURE(!naam.empty(), ("Het type" + locationToString(vaccin->FirstChildElement("type")) + " van het vaccin heeft een ongeldige waarde").c_str());
                if (naam == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het type" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                //naam = naam.substr(0, naam.size()-1);
            }
            //ENSURE(vaccin->FirstChildElement("levering") != NULL, "...");
            if (vaccin->FirstChildElement("levering") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'levering' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            }else{
                vaccin->FirstChildElement("levering")->InsertEndChild(space);
                string Str_levering = vaccin->FirstChildElement("levering")->GetText();
                if (Str_levering == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "de levering" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                levering = to_int(Str_levering.substr(0, Str_levering.size()-1));
                if (levering <= 0) {
                    errors.push_back(WRONG_VALUE);
                    cerr << "de levering" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
            }
            //ENSURE(vaccin->FirstChildElement("interval") != NULL, "...");
            if (vaccin->FirstChildElement("interval") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'interval' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            }
            else{
                vaccin->FirstChildElement("interval")->InsertEndChild(space);
                string Str_interval = vaccin->FirstChildElement("interval")->GetText();
                if (Str_interval  == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'interval'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                interval = to_int(Str_interval.substr(0, Str_interval.size()-1));
                //ENSURE(interval > 0, "...");
                if (interval <= 0) {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'interval'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
            }
            //ENSURE(vaccin->FirstChildElement("transport") != NULL, "...");
            if (vaccin->FirstChildElement("transport") == NULL) {
                errors.push_back(MISSING_TAG);
                cerr << "Het element 'transport' van het vaccin" + locationToString(vaccin) + " bestaat niet!";
                correct = false;
            }
            else{
                vaccin->FirstChildElement("transport")->InsertEndChild(space);
                string Str_transport = vaccin->FirstChildElement("transport")->GetText();
                //ENSURE(transport > 0, "...");
                if (Str_transport == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'transport'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                transport = to_int(Str_transport.substr(0, Str_transport.size()-1));
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
            }
            else{
                vaccin->FirstChildElement("hernieuwing")->InsertEndChild(space);
                string Str_hernieuwing = vaccin->FirstChildElement("hernieuwing")->GetText();
                if (Str_hernieuwing == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'hernieuwing'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                hernieuwing = to_int(Str_hernieuwing.substr(0, Str_hernieuwing.size()-1));
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
            }
            else{
                vaccin->FirstChildElement("temperatuur")->InsertEndChild(space);
                string Str_temperatuur = vaccin->FirstChildElement("temperatuur")->GetText();

                if (Str_temperatuur == " ") {
                    errors.push_back(WRONG_VALUE);
                    cerr << "Het element 'temperatuur'" + locationToString(vaccin->FirstChildElement("type")) +
                            " van het vaccin heeft een ongeldige waarde";
                    correct = false;
                }
                temperatuur = to_int(Str_temperatuur.substr(0, Str_temperatuur.size()-1));
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
