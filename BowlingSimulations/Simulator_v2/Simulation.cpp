#include "Simulation.h"
#include "util.h"

#pragma once

/**
    Constructor for the Random class. When TESTING is defined a predtermined
    seed will be used to initialize the srand() call. Otherwise srand() is
    initialized based on the time.
*/
Random::Random() {
    #ifdef TESTING
        srand(4141993);
    #else
        srand(time(NULL));
    #endif
}

/**
    This function is used when creating a simulation's first ball distribution.
    The distribution is a vector of maps. The vector has 11 elements
    [0, 1, 2, ..., 9, X]. Each of those contains a map of key pins still up and
    value of how many times those pins were left up. The ball number, ballNum,
    is used to determine where to pull the first ball from. That is needed
    because the 10th frame could have different three different first balls.
    The first ball, the second if the first was a strike, and the third if the
    first two were strikes or any count and a spare. The frame number to know
    which frame to pull first ball from.
    @param map - The distribution being added to.
    @param game - The game who's being pulled from.
    @param frame - The number of the frame being pulled from.
    @param ballNum - The ball number to be pulled (1,2,3).
*/
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
    This function is used when creating a simulation's second ball distribution.
    The distribution is a vector of maps. The vector has 11 elements
    [0, 1, 2, ..., 9, X]. Each of those contains a map of key pins still up for
    both first and second balls and value of how many times those pins were left
    up. This uses a BallMapKey, p which has both the first and second ball pins
    up, to directly access the map to increment it.
    @param map - The distribution being added to.
    @param p - The pair is the key to the map. The map[p] is incermented.
*/
void incrementMap(Ball2Map& map, Ball2MapKey& p) {
    
    // knocked down = 10 - ball 1 size()
    int knockedDown = N_PINS - p.first.size();
    // p.first is the pins up after ball 1
    
    // if not in map sets it to 1 else it gets incremented
    ++map[knockedDown][p];
}

/**
    In this method the distribution of the first ball is crerated. It uses the
    first balls and tenth frames from the games provided. The distribution is
    intialized to a vector of 11 maps (gutter ball, 1, 2, ..., 9, X). For the
    frames 1 to 9 the incrementMap function is used to add/increase the counts
    in the distribution. The tenth frame has its first ball and its different
    cases: 1) a strike first ball -> second ball is a 'first ball'. 2) both
    first and second are strikes -> third is a 'first ball'. 3) a spare was
    made with the first two balls -> third is a 'first ball'.
    @param games - The games that provide the first ball data.
    @return ball1Dist - The distribution of first balls created from games.
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
        int b1b2Total = games[g].getFrame(9).getFirstBall()
            + games[g].getFrame(9).getSecondBall();

        if (b1b2Total == SPARE || b1b2Total == (STRIKE + STRIKE)) {
            // if a b2 spare or b1 and b2 strikes.
  
            // add in ball 3 into ball 1 dist
            incrementMap(ball1Dist, games[g], 9, BALL_3);
        }
    }
    return ball1Dist;
}

/**
    In this method the distribution of the second ball is crerated. It uses the
    second balls and tenth frames from the games provided. The distribution is
    intialized to a vector of 11 maps (gutter ball, 1, 2, ..., 9, X). For the
    frames 1 to 9 the incrementMap function is used to add/increase the counts
    in the distribution. The tenth frame has its second ball iff the first
    wasn't a strike and the third ball iff the first was a strike and the
    second wasn't a strike.
    @param games - The games that provide the first ball data.
    @return ball2Dist - The distribution of second balls created from games.
*/
Ball2Map Random::getBall2Totals(std::vector<Game>& games) {
    // 0, 1, 2, ... 8, 9, 10 are the set of pins dpwn why 11 is used
    Ball2Map ball2Dist(N_FRAMES + 1, std::map<Ball2MapKey, int>());

    for (unsigned int g = 0; g < games.size(); ++g) { // go through each game
        for (int f = 0; f < NINETH_FRAME; ++f) { // handle 1st -> 9th frame
            if (games[g].getFrame(f).getFirstPinsUp().size() > 0) {
                // only add something if not a strike
                Ball2MapKey p =
                    Ball2MapKey(games[g].getFrame(f).getFirstPinsUp(),
                    games[g].getFrame(f).getSecondPinsUp());
                incrementMap(ball2Dist, p); // distribution, pair to increment
            }
        }

        // handle 10th frame
        Ball2MapKey p;
        if (games[g].getFrame(9).getFirstBall() < STRIKE) {
            // ball1 not a strike? Then add in the 2nd ball in. 
            p = Ball2MapKey(games[g].getFrame(9).getFirstPinsUp(),
                games[g].getFrame(9).getSecondPinsUp());
            incrementMap(ball2Dist, p);
        } else if (games[g].getFrame(9).getFirstBall() == STRIKE
            && games[g].getFrame(9).getSecondBall() < STRIKE) {
            // ball1 is a strike and ball2 wasn't then add in the third ball in
            p = Ball2MapKey(games[g].getFrame(9).getSecondPinsUp(), 
                games[g].getFrame(9).getThirdPinsUp());
            incrementMap(ball2Dist, p);
        }
    }
    return ball2Dist;
}

/**
    This constructor for the Simulator class takes games as the games to make
    ball 1 and 2 distributions. It first makes a new instance of Random. Then
    it copies the games over then makes the distributions.
    @param games - The games to pull data from.
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
    This method is used to get a random ball 1. It first counts the number of
    first balls in the distribution. Then a random 'index' in the map is
    chosen. Then it goes back through the distribution and once it finds the
    ball 1 and the method returns it.
    @return ball1 - the randomly chosen ball 1 to use.
*/
std::vector<unsigned int> Simulator::getBall1PinsUp() {
    
    // count the number of ball 1's in the distribution
    int b1 = 0;
    for (unsigned int i = 0; i < ball1Dist.size(); ++i) {
        for (auto it = ball1Dist[i].begin(); it != ball1Dist[i].end(); ++it) {
            b1 += it->second;
        }
    }

    // choose the 'index' of the random ball
    int r = std::rand() % b1; // random number to choose the ball
    b1 = 0; // reset the counter for the next loop
    
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
    This method is used to get a random ball 2. It first counts the number of
    second balls in the distribution that have the provided ball 1. Then a
    random 'index' in the map is chosen. Then it goes back through the
    distribution and once it finds the ball 2 and the method returns it. If the
    provided ball1 is a strike an empty ball2 is returned. There is an error
    case. It happens when the provided ball 1 was a fill ball, the third, in a
    tenth frame. That means that the fill ball never got a spare attempt. If
    that b1 didn't appear any where else means that there is no b2 in the ball2
    distribution and an error ball2 is returned.
    @param ball1 - the first ball to get a ball2 for.
    @return ball2 - the randomly chosen ball 2 to use.
*/
std::vector<unsigned int> Simulator::getBall2PinsUp(
    std::vector<unsigned int>& ball1) {

    if (ball1.size() == 0) {
        // if a strike, then nothing needs to be done
        return std::vector<unsigned int>();
    }

    unsigned int ball1Count = STRIKE - ball1.size(); // 10 - #pins still up
    std::map<std::vector<unsigned int>, int> possibleBall2s;
    int counter = 0;

    for (std::map<Ball2MapKey, int>::iterator
        it = ball2Dist[ball1Count].begin(); it != ball2Dist[ball1Count].end();
        ++it) {

        if (it->first.first == ball1) {
            // if the input and this (key, val) have the same first ball
            std::vector<unsigned int> pins;
            for (int i = 0; i < it->first.second.size(); ++i) {
                // copy over the second ball results
                pins.push_back(it->first.second[i]);
            }
            possibleBall2s[pins] = it->second;
            // b/c unique key we know that no other of the second part of the
            // key can be added
            counter += possibleBall2s[pins];
        }
    }
    
    // This case occurs when the input ball 1 is not in the map. This can
    // occur when that input ball was only a fill ball in the tenth frame.
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

    // find the random ball 2
    for (std::map<std::vector<unsigned int>, int>::iterator
        it = possibleBall2s.begin(); it != possibleBall2s.end(); ++it) {

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
    This method is used to make a random game. The first 9 frames are created
    first. A random b1 and b2 are chosen and rechosen until a non fill ball was
    chosen as a b1. See getBall2PinsUp()'s documentation why a fill ball may
    not be in the ball2 distribution. Then the tenth frame is handled in the
    same way choose and rechoose if fill ball was chosen. the first ball of
    the tenth is chosen. If a strike then another first ball is chosen for the
    second. If that is a strike then another first ball is chosen for the third
    ball, otherwise a second ball is chosen. If the first ball wasn't a strike,
    then and second ball is chosen. If a spare is made then a first ball is
    chosen for the third ball.
    @return a randomly created game.
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
    This method is used to create numGames many games. The number is assumed to
    be greater than 0. The games are made one by one and placed in to gmaes.
    @return games - the vector containing numGames many, randomly constructed
    games
*/
std::vector<Game> Simulator::makeGames(int numGames) {
    std::vector<Game> games;

    for (int i = 0; i < numGames; ++i) {
        games.push_back(makeGame());
    }

    return games;
}
