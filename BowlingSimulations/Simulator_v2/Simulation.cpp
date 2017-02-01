#include "Simulation.h"
#include "util.h"

#pragma once

/**
*/
Random::Random() {
    #ifdef TESTING
        srand(4141993);
    #else
        srand(time(NULL));
    #endif
}

/**
*/
// for ball 1
void incrementMap(Ball1Map& map, Game& game, int frame, int ballNum) {
    
    int knockedDown;
    std::vector<unsigned int> pinsUp;

    if (ballNum == BALL_1) {
        knockedDown = game.getFrame(frame).getFirstBall();
        pinsUp = game.getFrame(frame).getFirstPinsUp();
    } else if (ballNum == BALL_2) { // for oddities of the 10th frame
        knockedDown = game.getFrame(frame).getSecondBall();
        pinsUp = game.getFrame(frame).getSecondPinsUp();
    } else if (ballNum == BALL_3) { // for oddities of the 10th frame
        knockedDown = game.getFrame(frame).getThirdBall();
        pinsUp = game.getFrame(frame).getThirdPinsUp();
    }

    // if not in map sets it to 1 else it gets incremented
    ++map[knockedDown][pinsUp];
}

/**
*/
void incrementMap(Ball2Map& map, Ball2MapKey& p) {
    
    // knocked down = 10 - ball 1 size()
    int knockedDown = N_PINS - p.first.size();
    // p.first is the pins up after ball 1
    
    // if not in map sets it to 1 else it gets incremented
    ++map[knockedDown][p];
}

/**
*/
Ball1Map Random::getBall1Totals(std::vector<Game>& games) {
    
    // can have 0, 1, ... 8, 9, 10 pins knocked down -> 11
    Ball1Map ball1Dist(N_PINS + 1, std::map<std::vector<unsigned int>, int>());
    
    for (unsigned int g = 0; g < games.size(); ++g) { // go through each game
        for (int f = 0; f < N_FRAMES; ++f) {
            incrementMap(ball1Dist, games[g], f, BALL_1);
        }

        // check that this game needs to add the rest of the 10th frame
        // 9 here b/c 10th frame and 0 indexing.
        if (games[g].getFrame(9).getFirstBall() == STRIKE) {
            // if frame 10's ball 1 was a strike add in ball 2 into ball 1 dist
            incrementMap(ball1Dist, games[g], 9, BALL_2);
        }
        if (games[g].getFrame(9).getFirstBall() + games[g].getFrame(9).getSecondBall() == SPARE ||
            games[g].getFrame(9).getFirstBall() + games[g].getFrame(9).getSecondBall() == (STRIKE + STRIKE)
            ) { // else if ball 1 & 2 = spare or both are strikes
            
            // add in ball 3 into ball 1 dist
            incrementMap(ball1Dist, games[g], 9, BALL_3);
        }
    }
    return ball1Dist;
}

/**
*/
Ball2Map Random::getBall2Totals(std::vector<Game>& games) {
    // 0, 1, 2, ... 8, 9, 10 are the set of pins dpwn why 11 is used
    Ball2Map ball2Dist(N_FRAMES + 1, std::map<Ball2MapKey, int>());

    for (unsigned int g = 0; g < games.size(); ++g) { // go through each game
        for (int f = 0; f < NINETH_FRAME; ++f) { // handle 1st -> 9th frame
            if (games[g].getFrame(f).getFirstPinsUp().size() > 0) { // only add something if not a strike





                Ball2MapKey p = Ball2MapKey(games[g].getFrame(f).getFirstPinsUp(), games[g].getFrame(f).getSecondPinsUp());
                incrementMap(ball2Dist, p); // distribution, pair to increment
            }
        }

        // handle 10th frame
        Ball2MapKey p;
        if (games[g].getFrame(9).getFirstBall() < 10) { // ball1 not a strike?
            // add in the 2nd ball in. 
            p = Ball2MapKey(games[g].getFrame(9).getFirstPinsUp(), games[g].getFrame(9).getSecondPinsUp());
            incrementMap(ball2Dist, p);
        } else if (games[g].getFrame(9).getFirstBall() == 10
            && games[g].getFrame(9).getSecondBall() < 10) { // ball1 is a strike and ball2 wasn't
            // add in the third ball in
            p = Ball2MapKey(games[g].getFrame(9).getSecondPinsUp(), games[g].getFrame(9).getThirdPinsUp());
            incrementMap(ball2Dist, p);
        }
    }
    return ball2Dist;
}

/**
*/
Simulator::Simulator(std::vector<Game>& games) {
    r = Random();
    myGames = std::vector<Game>();
    // we are going to copy over the games to here
    for (unsigned int i = 0; i < games.size(); ++i) {
        myGames.push_back(Game(games[i]));
    }

    ball1Dist = r.getBall1Totals(myGames);
    ball2Dist = r.getBall2Totals(myGames);
}

/**
*/
std::vector<unsigned int> Simulator::getBall1PinsUp() {
    int b1 = 0;
    for (unsigned int i = 0; i < ball1Dist.size(); ++i) {
        for (auto it = ball1Dist[i].begin(); it != ball1Dist[i].end(); ++it) {
            b1 += it->second;
        }
    }

    int r = std::rand() % b1; // random number to choose the shot
    b1 = 0; // reset the counter
    std::vector<unsigned int> ball1 = std::vector<unsigned int>();
    for (unsigned int i = 0; i < ball1Dist.size(); ++i) {
        for (auto it = ball1Dist[i].begin(); it != ball1Dist[i].end(); ++it) {
            b1 += it->second;
            if (b1 > r) {
                // we found our first ball
                ball1 = it->first;
                i = ball1Dist.size(); // to kill the loop
                break;
            }
        }
    }
    
    return ball1;
}

/**
*/
// error case = input was a fill ball and never had a spare attempt
std::vector<unsigned int> Simulator::getBall2PinsUp(std::vector<unsigned int>& ball1) {
    if (ball1.size() == 0) {
        // if a strike, then nothing needs to be done
        return std::vector<unsigned int>();
    }

    unsigned int ball1Count = STRIKE - ball1.size(); // 10 - #pins still up
    std::map<std::vector<unsigned int>, int> possibleBall2s;
    int counter = 0;
    for (std::map<Ball2MapKey, int>::iterator it = ball2Dist[ball1Count].begin(); it != ball2Dist[ball1Count].end(); ++it) {
        if (it->first.first == ball1) {
            // both have the same first ball
            std::vector<unsigned int> pins;
            for (int i = 0; i < it->first.second.size(); ++i) {
                pins.push_back(it->first.second[i]);
            }
            possibleBall2s[pins] = it->second;
            // b/c unique key we know that no other of the second part of the key can be added
            counter += possibleBall2s[pins];
        }
    }
    
    //
    if (counter == 0) {
        std::vector<unsigned int> err;
        for (int i = 0; i < ERROR_SIZE; ++i) {
            err.push_back(i);
        }
        return err;
        // spare shot cannot be found. the input had to be a fill ball only
    }

    // now chose one of the vectors randomly
    int r = std::rand() % counter;
    counter = 0;
    std::vector<unsigned int> ball2 = std::vector<unsigned int>();
    for (std::map<std::vector<unsigned int>, int>::iterator it = possibleBall2s.begin(); it != possibleBall2s.end(); ++it) {
        counter += it->second;
        if (counter > r) {
            ball2 = it->first;
            it = possibleBall2s.end(); // kills the loop
            break;
        }
    }

    return ball2; 
}

/**
*/
Game Simulator::makeGame() {
    std::vector<Frame> frames = std::vector<Frame>();

    // handle 1st to 9th frames
    for (int f = 0; f < 9; ++f) {
        std::vector<unsigned int> b1, b2;
        do {
            b1 = getBall1PinsUp();
            b2 = getBall2PinsUp(b1);
        } while (b2.size() == ERROR_SIZE);
        frames.push_back(Frame(b1,b2));
    }

    // handle the 10th frame
    std::vector<unsigned int> b1, b2, b3;
    do {
        b1 = getBall1PinsUp();
        if (b1.size() == 0) {
            // there was a strike
            b2 = getBall1PinsUp();
            if (b2.size() == 0) {
                // another strike
                 b3 = getBall1PinsUp();
            } else if (b2.size() < STRIKE) { // left pins up
                // get a spare shot
                b3 = getBall2PinsUp(b2);
            } // else the b2 was an error case.
        } else {
            // get a spare shot
            b2 = getBall2PinsUp(b1);
            if (b2.size() == 0) {
                // spare was made and we get the fill ball
                b3 = getBall1PinsUp();
            } else {
                // initlaize to empty vector
                b3 = std::vector<unsigned int>();
            }
        }
    } while (b2.size() == ERROR_SIZE);
    frames.push_back(Frame(b1, b2, b3));

    return Game(frames);
}

/**
*/
std::vector<Game> Simulator::makeGames(int numGames) {
    std::vector<Game> games;

    for (int i = 0; i < numGames; ++i) {
        games.push_back(makeGame());
    }

    return games;
}
