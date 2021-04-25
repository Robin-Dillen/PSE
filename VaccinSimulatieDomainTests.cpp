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

class VaccinSimulatieDomainTest : public ::testing::Test {
protected:
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
    EXPECT_EQ(80000, H.getKaantalVaccinsPerLading(Vac->type));
    EXPECT_EQ(10, H.getLeveringenInterval(Vac->type));
    EXPECT_EQ(1000, H.getKaantalVaccinsPerLevering(Vac->type));
}

/**
Tests the "happy day" scenario
*/
TEST_F(VaccinSimulatieDomainTest, HappyDay) {

}

/**
Verify whether unsatisfied pre-conditions indeed trigger failures
*/
TEST_F(VaccinSimulatieDomainTest, ContractViolations) {
}