//
// Created by nibor on 2/03/2021.
//

#include "gtest/gtest.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Parser.h"

class VaccinSimulatieTest : public ::testing::Test {
public:
    VaccinSimulatieTest() {
        P = new Parser("../XMLfiles/vb.xml");
        H = P->getFhub();
    }

protected:
    friend class Hub;
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    friend class VaccinatieCentrum;
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

/**
virtual void SetUp() will be called before each test is run.  You
should define it if you need to initialize the variables.
Otherwise, this can be skipped.
*/
    virtual void SetUp() {
    }

/**
virtual void TearDown() will be called after each test is run.
You should define it if there is cleanup work to do.  Otherwise,
you don't have to provide it.
*/
    virtual void TearDown() {
    }

/**
Declares the variables your tests want to use.
*/
    Hub *H;
    Parser *P;
};


TEST_F(VaccinSimulatieTest, DefaultConstructor) {
    EXPECT_TRUE(H->isProperlyInitialized());
    EXPECT_TRUE(P->isProperlyInitialized());
    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isProperlyInitialized());
    }
}

TEST_F(VaccinSimulatieTest, AantalGeleverdeVaccins) {

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

