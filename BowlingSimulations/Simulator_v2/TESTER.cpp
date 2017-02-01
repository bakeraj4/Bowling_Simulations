#include "TESTER.h"
#include "BowlingDataStructure.h"
#include "Simulation.h"
#include "util.h"
#include <iostream>

#pragma once

/**
*/
double getSizeofB1Map(Ball1Map& m) {
    double size = 0.0;
    for (unsigned int i = 0; i < m.size(); ++i) {
        for (auto it = m[i].begin(); it != m[i].end(); ++it) {
            size += it->second;
        }
    }
    return size;
}

/**
*/
double getSizeofB2Map(Ball2Map& m) {
    double size = 0.0;
    for (int b1_count = 0; b1_count < STRIKE; ++b1_count) {
        std::map<std::vector<unsigned int>, int> possibleBall2s;
        for (std::map<Ball2MapKey, int>::iterator it = m[b1_count].begin(); it != m[b1_count].end(); ++it) {
            std::vector<unsigned int> pins;
            for (int i = 0; i < it->first.second.size(); ++i) {
                pins.push_back(it->first.second[i]);
            }
            possibleBall2s[pins] = it->second;
            // b/c unique key we know that no other of the second part of the key can be added
            size += possibleBall2s[pins];
        }
    }
    return size;
}

/**
*/
bool sameDistB1(Ball1Map& inputB1Map, double iB1MapSize, Ball1Map& outputB1Map, double oB1MapSize) {
    double err = 0.1;
    for (unsigned int i = 0; i < inputB1Map.size(); ++i) {
        for (auto it = inputB1Map[i].begin(); it != inputB1Map[i].end(); ++it) {
            if (abs((((double) it->second) / iB1MapSize) - (((double) outputB1Map[i][it->first]) / oB1MapSize)) > err ) {
                return false;
            }
        }
    }
    return true;
}

/**
*/
bool sameDistB2(Ball2Map& inputB2Map, double iB2MapSize, Ball2Map& outputB2Map, double oB2MapSize) {
    double err = 0.1;
    for (unsigned int i = 0; i < inputB2Map.size(); ++i) {
        for (auto it = inputB2Map[i].begin(); it != inputB2Map[i].end(); ++it) {
            if (abs((((double) it->second)/ iB2MapSize) - (((double) outputB2Map[i][it->first]) / oB2MapSize)) > err) {
                return false;
            }
        }
    }
    return true;
}

/**
*/
TESTER::TESTER() {
    totalPasses = totalTests = 0;
    header = "C:\\Users\\Aaron\\Dropbox\\c++\\Bowling_Simulator\\Bowling_Simulations\\testData\\";

    testGame1 = header + "testGame.txt";
    perfectGame = header + "testGamePerfect.txt";
    game170 = header + "170game.txt";
    game7 = header + "7game.txt";
    diffFillBall = header + "diffFillBall.txt";
}

/**
*/
void TESTER::Test() {
    // test the readIn constructor
    if (testReadIn()) {
        std::cout << "Passed Reading in data tests\n";
        ++totalPasses;
    } else {
        std::cout << "Failed Reading in data tests\n";
    }
    ++totalTests;

    // test the scoring
    if (testCalcScore()) {
        std::cout << "Passed the scoring tests\n";
        ++totalPasses;
    } else {
        std::cout << "Failed the scoring tests\n";
    }
    ++totalTests;

    // test ball 1 dist
    if (testBall1Dist()) {
        std::cout << "Passed the ball 1 dist tests\n";
        ++totalPasses;
    } else {
        std::cout << "Failed the ball 1 dist tests\n";
    }
    ++totalTests;

    // test ball 2 dist
    if (testBall1Dist()) {
        std::cout << "Passed the ball 2 dist tests\n";
        ++totalPasses;
    } else {
        std::cout << "Failed the ball 2 dist tests\n";
    }
    ++totalTests;

    // test ball 1 samples
    if (testRandBall1()) {
       std::cout << "Passed the ball 1 samples\n"; 
       ++totalPasses;
    } else {
        std::cout << "Failed the ball 1 samples\n";
    }
    ++totalTests;

    // test ball 2 dist
    if (testRandBall2()) {
        std::cout << "Passed the ball 2 samples\n";
        ++totalPasses;
    } else {
        std::cout << "Failed the ball 2 samples\n";
    }
    ++totalTests;

    // test simulation
    if (testSimulation()) {
        std::cout << "Passed the simulation tests\n";
        ++totalPasses;
    } else {
        std::cout << "Failed the simulation tests\n";
    }
    ++totalTests;

    // final printing
    std::cout << "Passed " << totalPasses << " out of " << totalTests << "\n";
}

/**
*/
bool TESTER::testReadIn() {
    // test a perfect game
    Game perfect(perfectGame);

    for (int i = 0; i < N_FRAMES; ++i) {
        if (perfect.getFrame(i).getFirstBall() != STRIKE) {
            return false; // should be a strike
        }
        if (perfect.getFrame(i).getFirstPinsUp().size() != 0) {
            return false;
        }
    }
    
    // Test a non perfect game, I comparing it to the data it is reading in
    Game nonPerfect1(testGame1);
    // 1 X
    if (nonPerfect1.getFrame(0).getFirstBall() != STRIKE) {
        return false;
    }
    if (perfect.getFrame(0).getFirstPinsUp().size() != 0) {
        return false;
    }

    // 2 9/ (10 pin)
    if (nonPerfect1.getFrame(1).getFirstBall() != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(1).getSecondBall() != 1) {
        return false;
    }
    if (nonPerfect1.getFrame(1).getFirstPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(1).getFirstPinsUp()[0] != 10) {
        return false;
    }
    if (nonPerfect1.getFrame(1).getSecondPinsUp().size() != 0) {
        return false;
    }

    // 3 81 (1,2 pin) -> (2 pin)
    if (nonPerfect1.getFrame(2).getFirstBall() != 8) {
        return false;
    }
    if (nonPerfect1.getFrame(2).getSecondBall() != 1) {
        return false;
    }
    if (nonPerfect1.getFrame(2).getFirstPinsUp().size() != 2) {
        return false;
    } else if (nonPerfect1.getFrame(2).getFirstPinsUp()[0] != 1) {
        return false;
    }
    if (nonPerfect1.getFrame(2).getSecondPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(2).getSecondPinsUp()[0] != 2) {
        return false;
    }

    // 4 X
    if (nonPerfect1.getFrame(3).getFirstBall() != STRIKE) {
        return false;
    }
    if (perfect.getFrame(3).getFirstPinsUp().size() != 0) {
        return false;
    }

    // 5 7/ (4,7,8 pin)
    if (nonPerfect1.getFrame(4).getFirstBall() != 7) {
        return false;
    }
    if (nonPerfect1.getFrame(4).getSecondBall() != 3) {
        return false;
    }
    if (nonPerfect1.getFrame(4).getFirstPinsUp().size() != 3) {
        return false;
    } else if (nonPerfect1.getFrame(4).getFirstPinsUp()[0] != 4) {
        return false;
    }
    if (nonPerfect1.getFrame(4).getSecondPinsUp().size() != 0) {
        return false;
    }    

    // 6 9- (10 pin) -> (10 pin)
    if (nonPerfect1.getFrame(5).getFirstBall() != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(5).getSecondBall() != 0) {
        return false;
    }
    if (nonPerfect1.getFrame(5).getFirstPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(5).getFirstPinsUp()[0] != 10) {
        return false;
    }
    if (nonPerfect1.getFrame(5).getSecondPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(5).getSecondPinsUp()[0] != 10) {
        return false;
    }

    // 7 81 (9,10 pin) -> (10 pin)
    if (nonPerfect1.getFrame(6).getFirstBall() != 8) {
        return false;
    }
    if (nonPerfect1.getFrame(6).getSecondBall() != 1) {
        return false;
    }
    if (nonPerfect1.getFrame(6).getFirstPinsUp().size() != 2) {
        return false;
    } else if (nonPerfect1.getFrame(6).getFirstPinsUp()[0] != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(6).getSecondPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(6).getSecondPinsUp()[0] != 10) {
        return false;
    }

    // 8 X
    if (nonPerfect1.getFrame(7).getFirstBall() != STRIKE) {
        return false;
    }
    if (perfect.getFrame(7).getFirstPinsUp().size() != 0) {
        return false;
    }

    // 9 X
    if (nonPerfect1.getFrame(8).getFirstBall() != STRIKE) {
        return false;
    }
    if (perfect.getFrame(8).getFirstPinsUp().size() != 0) {
        return false;
    }

    // 10 9/9 (9 pin) -> (), (9 pin)
    if (nonPerfect1.getFrame(9).getFirstBall() != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(9).getSecondBall() != 1) {
        return false;
    }
    if (nonPerfect1.getFrame(9).getThirdBall() != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(9).getFirstPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(9).getFirstPinsUp()[0] != 9) {
        return false;
    }
    if (nonPerfect1.getFrame(9).getSecondPinsUp().size() != 0) {
        return false;
    }
    if (nonPerfect1.getFrame(9).getThirdPinsUp().size() != 1) {
        return false;
    } else if (nonPerfect1.getFrame(9).getThirdPinsUp()[0] != 9) {
        return false;
    }

    return true;
}

/**
*/
bool TESTER::testCalcScore() {
    // test a perfect game
    Game perfect(perfectGame);
    if (perfect.getScore() != PERFECT_GAME) {
        return false;
    }

    // test a non perfect game with score 172
    Game nonPerfect1(testGame1);
    if (nonPerfect1.getScore() != 172) {
        return false;
    }

    Game g170(game170);
    if (g170.getScore() != 170) {
        return false;
    }
    
    return true;
}

/**
*/
bool TESTER::testBall1Dist() {
    std::vector<Game> games;
    Random r;
    std::vector<unsigned int> pinsUp;

    // test only 1 perfect game
    // should be 12 strikes
    games.push_back(Game(perfectGame));
    Ball1Map p1 = r.getBall1Totals(games);
    if (p1[STRIKE][pinsUp] != 12) {
        return false;
    } 

    // test 12 perfect games
    // should be 144 strikes
    while (games.size() < 10) {
        games.push_back(Game(perfectGame));
    }
    Ball1Map p12 = r.getBall1Totals(games);
    if (p12[STRIKE][pinsUp] != 120) {
        return false;
    }

    // test a 170 game
    // has different 7 pin configurations
    games.clear();
    games.push_back(Game(game170));
    Ball1Map g170 = r.getBall1Totals(games);
    // 6 of 6,9,10's
    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    if (g170[7][pinsUp] != 6) {
        return false;
    }
    // 2 of 3,6,10's
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    if (g170[7][pinsUp] != 3) {
        return false;
    }
    // 2 of 4,7,8's
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    if (g170[7][pinsUp] != 2) {
        return false;
    }

    // test 7game
    games.clear();
    games.push_back(Game(game7));
    Ball1Map g7 = r.getBall1Totals(games);
    // 6 of 6,9,10's
    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    if (g7[7][pinsUp] != 6) {
        return false;
    }
    // 2 of 3,6,10's
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    if (g7[7][pinsUp] != 3) {
        return false;
    }
    // 2 of 4,7,8's
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    if (g7[7][pinsUp] != 2) {
        return false;
    }

    return true;
}

/**
*/
bool TESTER::testBall2Dist() {
    std::vector<Game> games;
    Random r;
    std::vector<unsigned int> pinsUp1;
    std::vector<unsigned int> pinsUp2;

    // test only 1 perfect game
    games.push_back(Game(perfectGame));
    Ball2Map p1 = r.getBall2Totals(games);
    if (p1[10][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 0) {
        return false;
    }

    // test 10 perfect games
    while (games.size() != 10) {
        games.push_back(Game(perfectGame));
    }
    Ball2Map p10 = r.getBall2Totals(games);
    if (p10[10][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 0) {
        return false;
    }

    // test test game 1
    games.clear();
    games.push_back(Game(testGame1));
    Ball2Map g1 = r.getBall2Totals(games);
    // strikes
    if (g1[10][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 0) {
        return false;
    }
    pinsUp1.push_back(10);
    // 9 count leaving 10 pin. 1 conversion and 1 miss.
    if (g1[9][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    pinsUp2.push_back(10);
    if (g1[9][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    // 9 count leaving 9 pin. 1 make and a fill so only 1
    pinsUp2.clear();
    if (g1[9][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    // 8 count leaving 1 and 2 pins. leaving 2 pin up
    pinsUp1.clear();
    pinsUp2.clear();
    pinsUp1.push_back(1);
    pinsUp1.push_back(2);
    pinsUp2.push_back(2);
    if (g1[8][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    // 8 count leaving 9 and 10 pins. leaving 10 pin
    pinsUp1.clear();
    pinsUp2.clear();
    pinsUp1.push_back(9);
    pinsUp1.push_back(10);
    pinsUp2.push_back(10);
    if (g1[8][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    // 7 count leaving 4,7,8 pins. converted.
    pinsUp1.clear();
    pinsUp2.clear();
    pinsUp1.push_back(4);
    pinsUp1.push_back(7);
    pinsUp1.push_back(8);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }

    // test 170 game
    games.clear();
    games.push_back(Game(game170));
    pinsUp1.clear();
    pinsUp2.clear();
    // 7 count 6,9,10
    pinsUp1.push_back(6);
    pinsUp1.push_back(9);
    pinsUp1.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 6) {
        return false;
    }

    // 7 count 3,6,10
    pinsUp1.clear();
    pinsUp1.push_back(3);
    pinsUp1.push_back(6);
    pinsUp1.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 3) {
        return false;
    }

    // 7 count 4,7,8
    pinsUp1.clear();
    pinsUp1.push_back(4);
    pinsUp1.push_back(7);
    pinsUp1.push_back(8);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }

    // test game w/ all 7's
    games.clear();
    games.push_back(Game(game7));
    pinsUp1.clear();
    pinsUp2.clear();
    // 7 count 6,9,10 3 convert
    pinsUp1.push_back(6);
    pinsUp1.push_back(9);
    pinsUp1.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 3) {
        return false;
    }
    // "            " 3 miss
    pinsUp2.push_back(6);
    pinsUp2.push_back(9);
    pinsUp2.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 3) {
        return false;
    }
    // 7 count 3,6,10 1 convert
    pinsUp1.clear();
    pinsUp2.clear();
    pinsUp1.push_back(3);
    pinsUp1.push_back(6);
    pinsUp1.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }
    // 7 count 3,6,10 2 miss
    pinsUp2.push_back(3);
    pinsUp2.push_back(6);
    pinsUp2.push_back(10);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 2) {
        return false;
    }
    // 7 count 4,7,8 1 convert
    pinsUp1.clear();
    pinsUp2.clear();
    pinsUp1.push_back(4);
    pinsUp1.push_back(7);
    pinsUp1.push_back(8);
    if (g1[7][std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(pinsUp1, pinsUp2)] != 1) {
        return false;
    }

    return true;
}

/**
*/
bool TESTER::testRandBall1() {
    int numTest = 1000;
    double ERR = 0.1; // items to be within 10%
    std::vector<Game> games;

    // test 1 perfect game
    games.push_back(Game(perfectGame));
    unsigned int strikeCount = 0;
    Simulator sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i){
        if (sim.getBall1PinsUp() == std::vector<unsigned int>()) {
            ++strikeCount;
        }
    }
    // this game has only strikes so numTest many samples should provide 100 strikes
    if (strikeCount != numTest) {
        return false;
    }

    // test 10 perfect games
    while (games.size() != 10) {
        games.push_back(Game(perfectGame));
    }
    strikeCount = 0;
    sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i) {
        if (sim.getBall1PinsUp() == std::vector<unsigned int>()) {
            ++strikeCount;
        }
    }
    // these games have only strikes so numTest many samples should provide 100 strikes
    if (strikeCount != numTest) {
        return false;
    }
    
    // test game 1
    games.clear();
    std::map<std::vector<unsigned int>, int> ball1;
    games.push_back(Game(testGame1));
    sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i) {
        ++ball1[sim.getBall1PinsUp()];
    }

    if (abs((((double)ball1[std::vector<unsigned int>()]) / ((double)numTest)) - 0.36) > ERR) {
        // strikes should be ~36% (4/11)
        return false;
    }
    std::vector<unsigned int> pinsUp;
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.18) > ERR) {
        // 9 count leaving 10 pins ~18% (2/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(1);
    pinsUp.push_back(2);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.09) > ERR) {
        // 8 count leaving 1,2 pins ~9% (1/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.09) > ERR) {
        // 7 count leaving 4,7,8 pins ~9% (1/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.09) > ERR) {
        // 8 count leaving 9,10 pins ~9% (1/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(9);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.18) > ERR) {
        // 9 count leaving 9 pins ~18% (2/11)
        return false;
    }

    // 170 game
    ball1.clear();
    games.clear();
    games.push_back(Game(game170));
    sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i) {
        ++ball1[sim.getBall1PinsUp()];
    }
    // no strikes
    if (ball1.find(std::vector<unsigned int>()) != ball1.end()) {
        return false;
    }

    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.54) > ERR) {
        // 7 count 6,9,10 should be ~54% (6/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.27) > ERR) {
        // 7 count 3,6,10 should be ~27% (3/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.18) > ERR) {
        // 7 count 4,7,8 should be ~18% (2/11)
        return false;
    }

    // 7 game, should be the same ball 1 results as 170
    ball1.clear();
    games.clear();
    games.push_back(Game(game7));
    sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i) {
        ++ball1[sim.getBall1PinsUp()];
    }

    // no strikes
    if (ball1.find(std::vector<unsigned int>()) != ball1.end()) {
        return false;
    }

    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.54) > ERR) {
        // 7 count 6,9,10 should be ~54% (6/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.27) > ERR) {
        // 7 count 3,6,10 should be ~27% (3/11)
        return false;
    }
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.18) > ERR) {
        // 7 count 4,7,8 should be ~18% (2/11)
        return false;
    }

    // diff fill ball game
    games.clear();
    games.push_back(Game(diffFillBall));
    sim = Simulator(games);
    ball1.clear();
    for (unsigned int i = 0; i < numTest; ++i) {
        ++ball1[sim.getBall1PinsUp()];
    }
    // no strikes
    if (ball1.find(std::vector<unsigned int>()) != ball1.end()) {
        return false;
    }

    pinsUp.clear();
    pinsUp.push_back(10);
    if (abs((((double)ball1[pinsUp]) / ((double)numTest)) - 0.09) > ERR) {
        // should be a 9 count fill ~9% 1/11
        return false;
    }

    return true;
}

/**
*/
bool TESTER::testRandBall2() {
    int numTest = 1000;
    double ERR = 0.1; // items to be within 10%
    std::vector<Game> games;

    // test 1 perfect game
    games.push_back(Game(perfectGame));
    unsigned int strikeCount = 0;
    Simulator sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i){
        if (sim.getBall2PinsUp(std::vector<unsigned int>()) == std::vector<unsigned int>()) {
            ++strikeCount;
        }
    }
    // this game has only strikes so numTest many samples should provide 100 strikes
    if (strikeCount != numTest) {
        return false;
    }

    // test 10 perfect games
    while (games.size() != 10) {
        games.push_back(Game(perfectGame));
    }
    strikeCount = 0;
    sim = Simulator(games);
    for (unsigned int i = 0; i < numTest; ++i){
        if (sim.getBall2PinsUp(std::vector<unsigned int>()) == std::vector<unsigned int>()) {
            ++strikeCount;
        }
    }
    // this game has only strikes so numTest many samples should provide 100 strikes
    if (strikeCount != numTest) {
        return false;
    }

    // test game 1
    games.clear();
    games.push_back(Game(testGame1));
    sim = Simulator(games);
    std::vector<unsigned int> pinsUp;
    std::vector<unsigned int> pinsLeft;
    // not going to test strikes, done above and that this test is for 2nd ball  
    // the 9 count 10 pins was 50% make and miss
    pinsUp.push_back(10);
    double make = 0, miss = 0;
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (abs((make / (double(numTest))) - (miss / (double(numTest)))) > ERR) {
        return false;
    }
    // 8 count 1,2 miss. 0% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(1);
    pinsUp.push_back(2);
    pinsLeft.clear();
    pinsLeft.push_back(1);
    pinsLeft.push_back(2);
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != 0.0) {
        return false;
    }
    // 7 count 4,7,8. 100% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }
    // 8 count 9,10. 0% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    pinsLeft.clear();
    pinsLeft.push_back(10);
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) != pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != 0.0) {
        return false;
    }
    // 9 count 9. 100% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(9);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }

    // 170 game
    games.clear();
    games.push_back(Game(game170));
    sim = Simulator(games);
    // 7 count 6,9,10 100% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }
    // 7 count 3,6,10. 100% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }
    // 7 count 4,7,8
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }

    // 7 game
    games.clear();
    games.push_back(Game(game7));
    sim = Simulator(games);
    // 7 count 6,9,10 50% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(6);
    pinsUp.push_back(9);
    pinsUp.push_back(10);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (abs((make / (double(numTest))) - (miss / (double(numTest)))) > ERR) {
        return false;
    }
    // 7 count 3,6,10 33% make
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(3);
    pinsUp.push_back(6);
    pinsUp.push_back(10);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (abs((2.0 * (make / (double(numTest)))) - (miss / (double(numTest)))) > ERR) {
        return false;
    }
    // 7 count 4,7,8
    make = 0;
    miss = 0;
    pinsUp.clear();
    pinsUp.push_back(4);
    pinsUp.push_back(7);
    pinsUp.push_back(8);
    pinsLeft.clear();
    for (int i = 0; i < numTest; ++i) {
        if (sim.getBall2PinsUp(pinsUp) == pinsLeft) {
            ++make;
        } else {
            ++miss;
        }
    }
    if (make != numTest) {
        return false;
    }

    // fillBall different only than 170 game in that the fill ball is a 9 instead of a 7
    games.clear();
    games.push_back(Game(diffFillBall));
    sim = Simulator(games);
    // the fill was a 9 count w/ a ten pin and no previous attempts with it
    pinsUp.clear();
    pinsUp.push_back(10);
    pinsLeft.clear();
    pinsLeft = sim.getBall2PinsUp(pinsUp);
    if (pinsLeft.size() != ERROR_SIZE) {
        return false;
    }
    return true;
}

/**
*/
bool TESTER::testSimulation() {
    int numTrials = 100;
    std::vector<Game> input_games, output_games;
    Random r;
    
    // perfect game
    input_games.push_back(Game(perfectGame));
    Simulator sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    // compare the %'s, here the game is perfect games so we can just compare score to 300
    for (unsigned int i = 0; i < output_games.size(); ++i) {
        if (output_games[i].getScore() != PERFECT_GAME) {
            return false;
        }
    }
    
    // 10 perfect games
    while (input_games.size() < 10) {
        input_games.push_back(Game(perfectGame));
    }
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    // compare the %'s, here all of the games are perfect games so we can just compare score to 300
    for (unsigned int i = 0; i < output_games.size(); ++i) {
        if (output_games[i].getScore() != PERFECT_GAME) {
            return false;
        }
    }
    
    // test game1
    input_games.clear();
    input_games.push_back(Game(testGame1));
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    Ball1Map inputB1Map = r.getBall1Totals(input_games);
    Ball2Map inputB2Map = r.getBall2Totals(input_games);
    Ball1Map outputB1Map = r.getBall1Totals(output_games);
    Ball2Map outputB2Map = r.getBall2Totals(output_games);
    double iB1MapSize = getSizeofB1Map(inputB1Map);
    double iB2MapSize = getSizeofB2Map(inputB2Map);
    double oB1MapSize = getSizeofB1Map(outputB1Map);
    double oB2MapSize = getSizeofB2Map(outputB2Map);
    if ((!sameDistB1(inputB1Map, iB1MapSize, outputB1Map, oB1MapSize))
        && (!sameDistB2(inputB2Map, iB2MapSize, outputB2Map, oB2MapSize))) {
        
        return false;
    }

    // 170 game
    input_games.clear();
    input_games.push_back(Game(game170));
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    inputB1Map = r.getBall1Totals(input_games);
    inputB2Map = r.getBall2Totals(input_games);
    outputB1Map = r.getBall1Totals(output_games);
    outputB2Map = r.getBall2Totals(output_games);
    iB1MapSize = getSizeofB1Map(inputB1Map);
    iB2MapSize = getSizeofB2Map(inputB2Map);
    oB1MapSize = getSizeofB1Map(outputB1Map);
    oB2MapSize = getSizeofB2Map(outputB2Map);
    if ((!sameDistB1(inputB1Map, iB1MapSize, outputB1Map, oB1MapSize))
        && (!sameDistB2(inputB2Map, iB2MapSize, outputB2Map, oB2MapSize))) {

        return false;
    }

    // 7 game
    input_games.clear();
    input_games.push_back(Game(game7));
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    inputB1Map = r.getBall1Totals(input_games);
    inputB2Map = r.getBall2Totals(input_games);
    outputB1Map = r.getBall1Totals(output_games);
    outputB2Map = r.getBall2Totals(output_games);
    iB1MapSize = getSizeofB1Map(inputB1Map);
    iB2MapSize = getSizeofB2Map(inputB2Map);
    oB1MapSize = getSizeofB1Map(outputB1Map);
    oB2MapSize = getSizeofB2Map(outputB2Map);
    if ((!sameDistB1(inputB1Map, iB1MapSize, outputB1Map, oB1MapSize))
        && (!sameDistB2(inputB2Map, iB2MapSize, outputB2Map, oB2MapSize))) {

        return false;
    }

    // different fill ball game
    input_games.clear();
    input_games.push_back(Game(diffFillBall));
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    inputB1Map = r.getBall1Totals(input_games);
    inputB2Map = r.getBall2Totals(input_games);
    outputB1Map = r.getBall1Totals(output_games);
    outputB2Map = r.getBall2Totals(output_games);
    iB1MapSize = getSizeofB1Map(inputB1Map);
    iB2MapSize = getSizeofB2Map(inputB2Map);
    oB1MapSize = getSizeofB1Map(outputB1Map);
    oB2MapSize = getSizeofB2Map(outputB2Map);
    if ((!sameDistB1(inputB1Map, iB1MapSize, outputB1Map, oB1MapSize))
        && (!sameDistB2(inputB2Map, iB2MapSize, outputB2Map, oB2MapSize))) {

        return false;
    }

    // all games
    input_games.clear();
    input_games.push_back(Game(perfectGame));
    input_games.push_back(Game(game170));
    input_games.push_back(Game(game7));
    input_games.push_back(Game(diffFillBall));
    sim = Simulator(input_games);
    output_games = sim.makeGames(numTrials);
    inputB1Map = r.getBall1Totals(input_games);
    inputB2Map = r.getBall2Totals(input_games);
    outputB1Map = r.getBall1Totals(output_games);
    outputB2Map = r.getBall2Totals(output_games);
    iB1MapSize = getSizeofB1Map(inputB1Map);
    iB2MapSize = getSizeofB2Map(inputB2Map);
    oB1MapSize = getSizeofB1Map(outputB1Map);
    oB2MapSize = getSizeofB2Map(outputB2Map);
    if ((!sameDistB1(inputB1Map, iB1MapSize, outputB1Map, oB1MapSize))
        && (!sameDistB2(inputB2Map, iB2MapSize, outputB2Map, oB2MapSize))) {

        return false;
    }

    return true;
}
