#include "BowlingDataStructure.h"
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <exception>

#pragma once

typedef std::vector<std::map<std::vector<unsigned int>, int>> Ball1Map;
typedef std::vector<std::map<std::pair<std::vector<unsigned int>, std::vector<unsigned int>>, int>> Ball2Map;
typedef std::pair<std::vector<unsigned int>, std::vector<unsigned int>> Ball2MapKey;

class Random {
    private:
    public:
        Random();

        // answer[9] = set of 9 pins down combos that have 1 pin up
        // answer[9][{10}] = number of times 10 only pin up
        Ball1Map getBall1Totals(std::vector<Game>& games);
        Ball2Map getBall2Totals(std::vector<Game>& games);
        // no need for a ball 3 method.
        // if a fill ball was given then it will either be considered a 1st or 2nd ball based on what was knocked down earlier in the 10th

};

class Simulator {
    private:
        std::vector<Game> myGames;
        Ball1Map ball1Dist;
        Ball2Map ball2Dist;
        Random r;
        Game makeGame();
    public:
        Simulator(std::vector<Game>& games);
        std::vector<unsigned int> getBall1PinsUp();
        std::vector<unsigned int> getBall2PinsUp(std::vector<unsigned int>& ball1);
        std::vector<Game> makeGames(int gamesToMake);
        // can make futuer delimaters like pattern, date range, etc

};
