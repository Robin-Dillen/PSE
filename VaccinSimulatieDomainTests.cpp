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