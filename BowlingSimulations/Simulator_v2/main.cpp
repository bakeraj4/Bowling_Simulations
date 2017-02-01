#include "BowlingDataStructure.h"
#include "Simulation.h"
#include "Util.h"
#ifdef TESTING
    #include "TESTER.h"
#endif

#include <iostream>
#include <fstream>
#include <string>


#pragma once

int main(int argc, char *argv[]) {
    #ifdef TESTING
        TESTER tests = TESTER();
        tests.Test();
    #endif

    std::string header = "C:\\Users\\Aaron\\Dropbox\\c++\\Bowling_Simulator\\Bowling_Simulations\\testData\\";
    std::string game1 = header + "testGame.txt";
    std::string game2 = header + "testGamePerfect.txt";
    std::string diffGame = header + "diffFillBall.txt";

    Game g1(game1);
    Game g2(game2);
    Game g3(diffGame);


    std::cout << g1.getScore() << "\n"
        << g2.getScore() << "\n";

    std::vector<Game> games;
    games.push_back(g1); games.push_back(g2);

    Simulator s = Simulator(games);
    
    std::vector<Game> simGames = s.makeGames(100);
    int ave = 0;
    for (int i = 0; i < simGames.size(); ++i) {
        ave += simGames[i].getScore();
    }
    ave *= 0.01;
    std::cout << "\nThe 100 games averaged: " << ave << "\n"; 

    games.clear();
    games.push_back(g3);
    s = Simulator(games);
    games = s.makeGames(100);


    return 0;
}