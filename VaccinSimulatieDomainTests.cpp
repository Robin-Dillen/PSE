//============================================================================
// Name        : TicTacToeDomainTests.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <gtest/gtest.h>

using namespace std;

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Vaccins.h"
#include "Parser.h"
#include "StatisticsSingleton.h"
#include "Lib.h"
#include "Utils.h"
#include "VaccinSimulatie.h"

class VaccinSimulatieDomainTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        freopen(ERROR_FILE, "a+", stderr);
    }

    virtual void TearDown() {
        fclose(stderr);
    }
};

/**
Tests the default constructor.
*/
TEST_F(VaccinSimulatieDomainTest, DefaultConstructor) {
    Hub H;
    EXPECT_TRUE(H.isProperlyInitialized());

    VaccinatieCentrum V;
    EXPECT_TRUE(V.isProperlyInitialized());
    EXPECT_TRUE(V.getKaantalInwoners() == 0);
    EXPECT_TRUE(V.getKcapaciteit() == 0);
    EXPECT_TRUE(V.getAantalNietVaccinaties() == 0);
    EXPECT_TRUE(V.getKfname().empty());
    EXPECT_TRUE(V.getKfaddress().empty());

    VaccinsFactorySingleton &VFS = VaccinsFactorySingleton::getInstance();
    EXPECT_TRUE(VFS.isProperlyInitialized());

    Vaccin Vac;
    EXPECT_TRUE(Vac.levering == 0);
    EXPECT_TRUE(Vac.type.empty());
    EXPECT_TRUE(Vac.interval == 0);
    EXPECT_TRUE(Vac.hernieuwing == 0);
    EXPECT_TRUE(Vac.temperatuur == 0);
    EXPECT_TRUE(Vac.tijd_tot_nieuwe_levering == 0);
    EXPECT_TRUE(Vac.transport == 0);

    Parser P;
    EXPECT_TRUE(P.isProperlyInitialized());

    StatisticsSingleton &S = StatisticsSingleton::getInstance();
    EXPECT_TRUE(S.isProperlyInitialized());
}

/**
Tests the nondefault constructor.
*/
TEST_F(VaccinSimulatieDomainTest, NonDefaultConstructor) {
    VaccinsFactorySingleton &VFS = VaccinsFactorySingleton::getInstance();
    Vaccin *Vac = VFS.getVaccin("Test", 80000, 10, 1000, 10, 25);
    map<string, Vaccin *> vaccins;
    vaccins[Vac->type] = Vac;
    Hub H(vaccins);
    EXPECT_TRUE(H.isProperlyInitialized());
    EXPECT_EQ(80000, H.getAantalVaccins(Vac->type));
    EXPECT_EQ(80000, H.getKaantalVaccinsPerLevering(Vac->type));
    EXPECT_EQ(10, H.getLeveringenInterval(Vac->type));
    EXPECT_EQ(1000, H.getKaantalVaccinsPerLading(Vac->type));
}

/**
Tests the "happy day" scenario
*/
TEST_F(VaccinSimulatieDomainTest, HappyDay) {
    string testnr = "001";
    int nr = 1;
    string filename = HAPPY_DAY_TESTS_FILE_LOCATION + "test" + testnr + ".xml";
    while (FileExists(filename)) {
        Parser P(filename);
        vector<Hub *> hubs = P.getFhubs();
        std::vector<VaccinatieCentrum *> vaccinatie_centra;

        for (unsigned int i = 0; i < hubs.size(); i++) {
            for (map<string, VaccinatieCentrum *>::const_iterator centrum = hubs[i]->getFverbondenCentra().begin();
                 centrum != hubs[i]->getFverbondenCentra().end(); centrum++) {
                if (find(vaccinatie_centra.begin(), vaccinatie_centra.end(), centrum->second) ==
                    vaccinatie_centra.end()) {
                    vaccinatie_centra.push_back(centrum->second);
                }
            }
        }
        Simulatie(hubs, vaccinatie_centra, filename);

        for (vector<VaccinatieCentrum *>::iterator V = vaccinatie_centra.begin(); V != vaccinatie_centra.end(); V++) {
            EXPECT_EQ(0, (*V)->getAantalNietVaccinaties());
            EXPECT_GE((*V)->getTotaalAantalVaccins(), 0);
            EXPECT_LE((*V)->getTotaalAantalVaccins(), (*V)->getMaxStock());
            EXPECT_TRUE((*V)->getTotaalAantalVaccinaties() == (*V)->getKaantalInwoners());
            EXPECT_TRUE((*V)->isIedereenGevaccineerd());
        }

        for (vector<Hub *>::iterator H = hubs.begin(); H != hubs.end(); H++) {
            EXPECT_TRUE((*H)->isIedereenGevaccineerd());
            EXPECT_GE((*H)->getTotaalAantalVaccins(), 0);
        }

        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
    }
}

/**
Verify whether unsatisfied pre-conditions indeed trigger failures
*/
TEST_F(VaccinSimulatieDomainTest, HubContractViolations) {
    VaccinsFactorySingleton &VFS = VaccinsFactorySingleton::getInstance();
    EXPECT_DEATH(VFS.getVaccin("", -80000, -10, -1000, -10, 25), "");
    Vaccin *Vac1 = new Vaccin("", 20000, 10, 1000, 10, 25);
    Vaccin *Vac2 = new Vaccin("Test", -25000, -10, -1000, -10, 25);
    Vaccin *Vac3 = new Vaccin("Test", 25000, 10, 1000, 10, 25);
    map<string, Vaccin *> vaccins;
    vaccins[Vac1->type] = Vac1;
    vaccins[Vac2->type] = Vac2;
    EXPECT_DEATH(Hub H(vaccins), "");
    vaccins.clear();
    vaccins[Vac3->type] = Vac3;
    Hub H(vaccins);

    EXPECT_DEATH(H.getAantalVaccins(""), "");
    EXPECT_DEATH(H.getLeveringenInterval(""), "");
    EXPECT_DEATH(H.getKaantalVaccinsPerLevering(""), "");
    EXPECT_DEATH(H.getKaantalVaccinsPerLading(""), "");
    EXPECT_DEATH(H.addCentrum(NULL), "");
    EXPECT_DEATH(H.ontvangLevering("", Vac1->levering), "");
    EXPECT_DEATH(H.ontvangLevering("Test", Vac2->levering), "");

    delete Vac1;
    delete Vac2;
    delete Vac3;
}

TEST_F(VaccinSimulatieDomainTest, VaccinatieCentrumContractViolations) {
    VaccinsFactorySingleton &VFS = VaccinsFactorySingleton::getInstance();

    EXPECT_DEATH(VaccinatieCentrum(0, 1, "test", "teststraat, 1"), "");
    EXPECT_DEATH(VaccinatieCentrum(1, 0, "test", "teststraat, 1"), "");
    EXPECT_DEATH(VaccinatieCentrum(1, 1, "", "teststraat, 1"), "");
    EXPECT_DEATH(VaccinatieCentrum(1, 1, "test", ""), "");

    VaccinatieCentrum V(1, 1, "test", "teststraat, 1");
    EXPECT_DEATH(V.getAantalVaccinaties(""), "");
    EXPECT_DEATH(V.getAantalVaccins(""), "");
    EXPECT_DEATH(V.getAantalGeleverdeVaccins(""), "");
    Vaccin *V1 = VFS.getVaccin("Test", 1, 1, 1, 1, 1);
    EXPECT_DEATH(V.ontvangLevering(-100, V1), "");
    EXPECT_DEATH(V.getAantalTweedePrikken("", 1), "");
    EXPECT_DEATH(V.getAantalTweedePrikken("Test", -1), "");
}