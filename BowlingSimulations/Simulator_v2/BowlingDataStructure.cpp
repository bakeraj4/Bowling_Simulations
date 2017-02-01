#include "BowlingDataStructure.h"
#include "util.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#pragma once

// 9 / = 9 count that left the 10 and was converted
// 9 1 10

// 8 1 = 8 count that left the 6,10 and missed the 10 pin
// 8 2 6 10
// 1 1 10


/**
    The default constructor for the frame class. It will set all the data
    members to default values. The pin counts will be set to 0 and the pins
    remaining up vectors are empty.
    @return a 'dumby' frame
*/
Frame::Frame() {
    count1 = count2 = count3 = 0;
    pinsUp1 = std::vector<unsigned int>();
    pinsUp2 = std::vector<unsigned int>();
    pinsUp3 = std::vector<unsigned int>();
}

/**
    This constructor copies the content of another frame and creates a new
    frame based on those contents. The counts for each ball will be the same
    and will have the same pins up after each ball.
    @param other - the frame to be copied
    @return a copy of other
*/
Frame::Frame(const Frame& other) {
    count1 = other.getFirstBall();
    count2 = other.getSecondBall();
    count3 = other.getThirdBall();

    pinsUp1 = std::vector<unsigned int>(other.getFirstPinsUp());
    pinsUp2 = std::vector<unsigned int>(other.getSecondPinsUp());
    pinsUp3 = std::vector<unsigned int>(other.getThirdPinsUp());

}

/**
    This constructor makes a frame based on the provided pins up after ball one
    and ball 2. Count 1 is equal to 10 - num pins up after ball 1, b1. Count 2
    is the difference between the two balls, b1 - b2. This constructor is used
    for frames 1 to 9!
    @param b1 - The pins up after the first ball
    @param b2 - The pins up after the second ball
    @return a frame that has the given pin up configurations and the
        coresponding pin counts
*/
Frame::Frame(std::vector<unsigned int>& b1, std::vector<unsigned int>& b2) {
    pinsUp1 = std::vector<unsigned int>(b1);
    pinsUp2 = std::vector<unsigned int>(b2);
    pinsUp3 = std::vector<unsigned int>();

    // c1 = 10 - knocked down on b1
    count1 = STRIKE - pinsUp1.size();
    // c2 = knocked down on b1 - knocked down on b2
    count2 = pinsUp1.size() - pinsUp2.size();
    count3 = 0;
}

/**
    This constructor makes a frame based on the provided pins up after all
    three balls. Count 1 is equal to 10 - num pins up after ball 1, b1. Count 2
    is either 10 - pins up after ball 2 (if ball 1 was a strike) or the
    difference between the pins up after ball 1 and 2 (if ball 1 isn't a
    strike). The third ball is either 0 (an open frame), 10 - pins up after
    ball 3 (we got a fill ball), or the difference between b2 and b3 (the first
    ball was a strike). This constructor is used for the 10th frame!
    @param b1 - The pins up after the first ball
    @param b2 - The pins up after the second ball
    @param b3 - The pins up after the thrid ball
    @return a frame that has the given pin up configuration and the
        coresponding pin counts.
*/
Frame::Frame(std::vector<unsigned int>& b1, std::vector<unsigned int>& b2,
    std::vector<unsigned int>& b3) {

    pinsUp1 = std::vector<unsigned int>(b1);
    pinsUp2 = std::vector<unsigned int>(b2);
    pinsUp3 = std::vector<unsigned int>(b3);

    count1 = STRIKE - pinsUp1.size();
    if (count1 == STRIKE) { // strike
        count2 = STRIKE - pinsUp2.size();
        if (count2 == STRIKE) { // strike
            count3 = STRIKE - pinsUp3.size();
        } else { // the spare shot
            count3 = pinsUp2.size() - pinsUp3.size();
        }
    } else { // ball2 is a spare shot
        count2 = pinsUp1.size() - pinsUp2.size();
        // check for fill ball
        if (count1 + count2 == SPARE) { // there is a fill
            count3 = STRIKE - pinsUp3.size();
        } else {  // no fill ball
            count3 = 0;
        }
    }
}

/**
    This method is used to set the first ball's count. It returns an int to
    show success/failure.
    @param count - the number of pins knocked down
    @return -1 - failure
    @return 1 - success
*/
int Frame::setFirstBall(unsigned int count) {
    if (count > STRIKE) {
        return -1;
    } else {
        count1 = count;
        return 1;
    }
}

/**
    This method is used to set the second ball's count. It returns an int to
    show success/failure.
    @param count - the number of pins knocked down
    @return -1 - failure
    @return 1 - success
*/
int Frame::setSecondBall(unsigned int count) {
    if (count > STRIKE) {
        return -1;
    } else {
        count2 = count;
        return 1;
    }
}

/**
    This method is used to set the third ball's count. It returns an int to
    show success/failure.
    @param count - the number of pins knocked down
    @return -1 - failure
    @return 1 - success
*/
int Frame::setThirdBall(unsigned int count) {
    if (count > STRIKE) {
        return -1;
    } else {
        count3 = count;
        return 1;
    }
}

/**
    This method is used to set the first ball's pins still up. It returns an
    int to show success/failure.
    @param pins - the pins still up after the ball
    @return -1 - failure
    @return 1 - success
*/
int Frame::setFirstPinsUp(std::vector<unsigned int> &pins) { 
    // There cannot be more than 10 pins still up.
    if (pins.size() > N_PINS) { return -1; }
    
    for (unsigned int i = 0; i < pins.size(); ++i) {
        // Pin numbers are in int range [1, 10]].
        if (pins[i] > N_PINS) { return -1; }
    }
    
    pinsUp1.clear();
    for (unsigned int i = 0; i < pins.size(); ++i) {
        pinsUp1.push_back(pins[i]);
    }

    return 1; // sucess
}

/**
    This method is used to set the second ball's pins still up. It returns an
    int to show success/failure.
    @param pins - the pins still up after the ball
    @return -1 - failure
    @return 1 - success
*/
int Frame::setSecondPinsUp(std::vector<unsigned int> &pins) {
    // There cannot be more than 10 pins still up.
    if (pins.size() > N_PINS) { return -1; }
    
    for (unsigned int i = 0; i < pins.size(); ++i) {
    // Pin numbers are in int range [1, 10]
        if (pins[i] > N_PINS) { return -1; }
    }
    
    pinsUp2.clear();
    for (unsigned int i = 0; i < pins.size(); ++i) {
        pinsUp2.push_back(pins[i]);
    }

    return 1; // sucess
}

/**
    This method is used to set the third ball's pins still up. It returns an
    int to show success/failure.
    @param pins - the pins still up after the ball
    @return -1 - failure
    @return 1 - success
*/
int Frame::setThirdPinsUp(std::vector<unsigned int> &pins) {
    // There cannot be more than 10 pins still up.
    if (pins.size() > N_PINS) { return -1; } 
    
    for (unsigned int i = 0; i < pins.size(); ++i) { 
        // Pin numbers are in int range [1, 10]].
        if (pins[i] > N_PINS) { return -1; }
    }
    
    pinsUp3.clear();
    for (unsigned int i = 0; i < pins.size(); ++i) {
        pinsUp3.push_back(pins[i]);
    }

    return 1; // sucess
}

/**
    This method returns the number of pins that were knocked down on the
    first ball.
    @return count1 - the number of pins knocked down on the first ball.
*/
unsigned int Frame::getFirstBall() const {
    return count1;
}

/**
    This method returns the number of pins that were knocked down on the
    second ball.
    @return count2 - the number of pins knocked down on the second ball.
*/
unsigned int Frame::getSecondBall() const {
    return count2;
}

/**
    This method returns the number of pins that were knocked down on the
    third ball.
    @return count3 - the number of pins knocked down on the third ball.
*/
unsigned int Frame::getThirdBall() const {
    return count3;
}

/**
    This method returns a vector containing the pins that are up after the
    first ball is thrown.
    @return vector containing the pins that are up after ball 1.
*/
std::vector<unsigned int> Frame::getFirstPinsUp() const {
    return std::vector<unsigned int>(pinsUp1);
}

/**
    This method returns a vector containing the pins that are up after the
    second ball is thrown.
    @return vector containing the pins that are up after ball 2.
*/
std::vector<unsigned int> Frame::getSecondPinsUp() const {
    return std::vector<unsigned int>(pinsUp2);
}

/**
    This method returns a vector containing the pins that are up after the
    third ball is thrown.
    @return vector containing the pins that are up after ball 3.
*/
std::vector<unsigned int> Frame::getThirdPinsUp() const {
    return std::vector<unsigned int>(pinsUp3);
}

/**
    This method is used to read in data directly into an instance of a Game.
    The method puts the counts and pins up into each of the ten frames.
    @param fileName - The name of the file containing the data of the game.
*/
void Game::readIn(std::string fileName) {
    std::ifstream ifs;
    ifs.open(fileName);

    std::vector<unsigned int> pins1, pins2, pins3;

    for (unsigned int frame = 1; frame <= N_FRAMES; ++frame) {
        int ball1, ball2, ball3; // # nocked down
        int numUp1, numUp2, numUp3; // # still standing

        ifs >> ball1;
        ifs >> numUp1;
        for (int i = 0; i < numUp1; ++i) {
            int tmp;
            ifs >> tmp;
            pins1.push_back(tmp);
        }
        frames[frame - 1].setFirstPinsUp(pins1);
        frames[frame - 1].setFirstBall(ball1);

        ifs >> ball2;
        ifs >> numUp2;
        for (int i = 0; i < numUp2; ++i) {
            int tmp;
            ifs >> tmp;
            pins2.push_back(tmp);
        }

        frames[frame - 1].setSecondPinsUp(pins2);
        frames[frame - 1].setSecondBall(ball2);

        if (frame == N_FRAMES) {
            ifs >> ball3;
            ifs >> numUp3;
            for (int i = 0; i < numUp3; ++i) {
                int tmp;
                ifs >> tmp;
                pins3.push_back(tmp);
            }
            frames[frame - 1].setThirdBall(ball3);
            frames[frame - 1].setThirdPinsUp(pins3);
        }

        pins1.clear();
        pins2.clear();
        pins3.clear();
    }
    
    ifs.close();
}

/**
    This constructor takes a file's name and makes a game with the
    corresponding pin falls. It uses the readIn() to fill in the data.
    @param fileName - The name of the file containg the data about the game
*/
Game::Game(std::string fileName) {
    frames = std::vector<Frame>(N_FRAMES, Frame());
	score = 0;
    readIn(fileName);
}

/**
    This method is use to count up the score for a game. When there is a strike
    the next two balls are taken in to consideration for the score. For a spare
    only the next ball is considered. The 9th and 10th frames are special cases
    to account for the fill ball(s) that can be thrown in the 10th frame.
*/
void Game::calcScore() {
    score = 0;

    for (int frame = 1; frame <= N_FRAMES; ++frame) {
        int i = frame - 1;
        if (frame == N_FRAMES) { // just add `em up.
            score += frames[i].getFirstBall() + frames[i].getSecondBall() + frames[i].getThirdBall();
            // if an open frame then third ball will be 0 and open + 0 is still the open frame
        } else if (frame == NINETH_FRAME) { // 9th frame gets own case b/c its interaction w/ the 10th
            int b1 = frames[i].getFirstBall();
            score += b1;
            if (b1 == STRIKE) { // strike
                score += frames[i + 1].getFirstBall() + frames[i + 1].getSecondBall(); // gets the next two throws
            } else {
                int b2 = frames[i].getSecondBall();
                score += b2; // needs it even w/o the spare
                if (b1 + b2 == SPARE) { // spare
                    score += frames[i + 1].getFirstBall();
                }
            }
        } else {
            int b1 = frames[i].getFirstBall();
            score += b1;
            if (b1 == STRIKE) { // strike
                int next1 = frames[i + 1].getFirstBall();
                score += next1;
                if (next1 == STRIKE) { // another strike
                    score += frames[i + 2].getFirstBall();
                } else { // spare or open frame
                    score += frames[i + 1].getSecondBall();
                }
            } else {
                int b2 = frames[i].getSecondBall();
                score += b2; // needs it even w/o the spare
                if (b1 + b2 == SPARE) { // spare
                    score += frames[i + 1].getFirstBall();
                }
            }
        }
    }

}

/**
    This method is used to get the score of the game. The score is only
    calculated if it is 0.
    @return score - the score that the game contained.
*/
unsigned int Game::getScore() {
	if (score == 0) {
		calcScore();
	}
	return score;
}

/**
    This method returns a copy of the frame at the index.
    @return a copy of frames[index]
*/
Frame Game::getFrame(unsigned int index) const {
    return Frame(frames[index]);
}

/**
    This constructor copies data from another game. It copies over the frames
    to the newly created Game. calcScore() is called to determine the score.
    @parame other - The Game being copied.
*/
Game::Game(const Game& other) {
    frames = std::vector<Frame>();
    for (int i = 0; i < N_FRAMES; ++i) {
        frames.push_back(Frame(other.getFrame(i)));
    }
    calcScore();
}

/**
    This constructor creates a game from a vector of frames. It copies the
    frames over to the new Game. calcScore() is called to determine the score.
    @parame _frames - The Frames being copied.
*/
Game::Game(const std::vector<Frame>& _frames) {
    frames = std::vector<Frame>();
    for (unsigned int i = 0; i < _frames.size(); ++i) {
        frames.push_back(Frame(_frames[i]));
    }
    calcScore();
}
