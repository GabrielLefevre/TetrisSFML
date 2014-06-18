#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include "headers.hpp"

#include "Tetromino.hpp"
#include "random.hpp"
using namespace std;
using namespace sf;

class TetrisBoard : public sf::Drawable
{
    public:

        TetrisBoard(sf::Vector2i * window_size);
        virtual ~TetrisBoard();

        void update();

        bool newPiece(Tetromino p, bool init_pos = true); // place la nouvelle pièce en haut
        Tetromino & getPieceCourrante();

        void clearBoard();

        int HardDrop();

        bool MoveDown();
        bool MoveDownCheck();
        bool moveLeft();
        bool moveRight();
        bool rotateLeft();
        bool rotateRight();

        // les valeurs x et y sont les nouvelles coordonnées du tetromino
        // la valeur z correspond à la nouvelle orientation (0-3)
        // si z = ancienne orientation, la rotation n'a pas lieu.
        sf::Vector3i Rotate(int actualOrient, int nextOrientTest);

        void fixTetro();
        void dessinePieceCourrante(bool drawGhost);
        void effacePieceCourrante();
        int viderLigne();
        bool pieceActive();
        int getBoardData(int x, int y) const;
        void setBoardData(int x, int y, int data);

    private:
        // element graphique
        sf::RectangleShape matriceTetro; // le rectangle principal
        vector<sf::RectangleShape> shapeMatrix; // les rectangles de chaque cases

        sf::Vector2i * _window_size;

        bool verifierPlacementPiece(sf::Vector2i pos, int o);   // par rapport à la position absolue
        bool verifierDeplacementPiece(sf::Vector2i var_pos, int o);// par rapport à la position relative

        Tetromino pieceCourrante;
        bool pieceCouranteActive;
        uint8_t area[area_width][area_height];
        uint8_t areasup[area_width][2];

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // TETRISBOARD_H
