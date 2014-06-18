#ifndef GAME_H
#define GAME_H

#include "headers.hpp"

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrominoBoard.hpp"
#include "string.hpp"
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Game : public sf::Drawable
{
    private:
        sf::Vector2i * _window_size;

        TetrisBoard matrix;
        TetrominoBoard tetroSuiv;
        vector<sf::Text> textTetroSuiv;
        sf::Text scoreInfos;
        sf::Text commandes;

        char _replay;
        sf::Music _music;
    public:


        int _score;
        int nbLignes;
        int nbTetro;
        sf::Time timeLastMoveDown;
        sf::Clock gameClock;
    private:
        bool nextPiece(bool cancelCountTetromino);

        // sf::Drawable
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    public:
        Game(sf::Vector2i * window_size);
        virtual ~Game();

        int getLevel();
        void setTimeLastMoveDown();

        sf::Time getGameTime();
        void restartGame();

        sf::Time getTimeAutoMoveDown();

        virtual void onEvent(sf::Event & event);
        virtual void update();
};

#endif // GAME_H
