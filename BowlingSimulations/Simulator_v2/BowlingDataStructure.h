#include <vector>
#include <string>

#pragma once

class Frame {
    private:
        unsigned int count1, count2, count3;
        std::vector<unsigned int> pinsUp1, pinsUp2, pinsUp3;
    public:
        Frame();
        Frame(const Frame& other);
        Frame(std::vector<unsigned int>& b1, std::vector<unsigned int>& b2);
        Frame(std::vector<unsigned int>& b1, std::vector<unsigned int>& b2, std::vector<unsigned int>& b3);
        
        int setFirstBall(unsigned int count);
        int setSecondBall(unsigned int count);
        int setThirdBall(unsigned int count);
        
        int setFirstPinsUp(std::vector<unsigned int> &pins);
        int setSecondPinsUp(std::vector<unsigned int> &pins);
        int setThirdPinsUp(std::vector<unsigned int> &pins);
        
        unsigned int getFirstBall() const;
        unsigned int getSecondBall() const;
        unsigned int getThirdBall() const;
        
        std::vector<unsigned int> getFirstPinsUp() const;
        std::vector<unsigned int> getSecondPinsUp() const;
        std::vector<unsigned int> getThirdPinsUp() const;
};

class Game {
    private:
        std::vector<Frame> frames;
        int score;
        void readIn(std::string fileName);
		void calcScore();
    public:
        Game(const Game& other);
        Game(const std::vector<Frame>& frames);
        Game(std::string fileName);
		unsigned int getScore();
        // getter of a copy of the frames
        Frame getFrame(unsigned int index) const;
};
