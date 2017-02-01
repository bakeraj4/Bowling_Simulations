#include <string>

#pragma once

class TESTER {
    private:
        double totalTests;
        double totalPasses;
        std::string header;
        std::string perfectGame;
        std::string testGame1;
        std::string game170;
        std::string game7;
        std::string diffFillBall;

        bool testReadIn();
        bool testCalcScore();
        bool testBall1Dist();
        bool testBall2Dist();
        bool testRandBall1(); // test distributions
        bool testRandBall2(); // "                "
        bool testSimulation(); // compare the distributions to one another
    public:
        TESTER();
        void Test();
};
