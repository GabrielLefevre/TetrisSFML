#include "Tetromino.hpp"
#include "random.hpp"

using namespace std;
using namespace sf;

Tetromino::Tetromino(int t, int o, sf::Vector2i pos) :
    piece(4)
{
    vector<bool> v(4, false);
    for (int i=0; i<4; i++)
        piece[i] = v;

    setTypePiece(t);
    setOrientation(o);
    setPosition(pos);
}
Tetromino::Tetromino(int t, int o) : Tetromino(t, o, sf::Vector2i(0, 0))
{
}
Tetromino::Tetromino() : Tetromino(rand_int(0, 6), 0)
{
}

Tetromino::~Tetromino()
{
}

int Tetromino::getTypePiece()
{
    return type_piece;
}

void Tetromino::setTypePiece(int type)
{
    while (type < 0) type += 7;
    type = type % 7;
    type_piece = type;

    couleur = Tetromino::couleurs[type_piece];
    setMatrixShape();
}

int Tetromino::getOrientation()
{
    return orientation;
}

void Tetromino::setOrientation(int o)
{
    while (o < 0) o += 4;
    o = o % 4;
    orientation = o;
    setMatrixShape();
}

sf::Vector2i Tetromino::getPosition()
{
    return position;
}

void Tetromino::setPosition(sf::Vector2i pos)
{
    position = pos;
}

float Tetromino::getVerticalCenter()
{
    MatrixShape shape = getMatrixShape();
    int min = -1, max = 0;
    for (int j=0; j<4; j++)
        for (int i=0; i<4; i++)
            if (shape[i][j])
            {
                max = j;
                if (min == -1)
                    min = j;
            }
    return area_height - (position.y + (max - min)/2.f) - 1;

}

sf::Vector2f Tetromino::getCenter()
{
    MatrixShape shape = getMatrixShape();
    int mini = -1, maxi = 0;
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            if (shape[i][j])
            {
                maxi = i;
                if (mini == -1)
                    mini = i;
            }
    int minj = -1, maxj = 0;
    for (int j=0; j<4; j++)
        for (int i=0; i<4; i++)
            if (shape[i][j])
            {
                maxj = j;
                if (minj == -1)
                    minj = j;
            }
    return sf::Vector2f(position.x + (maxi - mini)/2.f, position.y + (maxj - minj)/2.f);

}

sf::Color Tetromino::getColor()
{
    return couleur;
}

MatrixShape Tetromino::getMatrixShape()
{
    return piece;

}

void Tetromino::setMatrixShape()
{
    if (type_piece < 0 || type_piece > 6 || orientation < 0 || orientation > 3)
        return;
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            piece[i][j] = pieces[type_piece][orientation][j][i];
        }
    }
}

const sf::Color Tetromino::couleurs[7] =
{
    sf::Color(255,201,14),
    sf::Color(0,162,232),
    sf::Color(34,177,76),
    sf::Color(237,28,36),
    sf::Color(255,127,39),
    sf::Color(63,72,204),
    sf::Color(163,73,164)
};
//Matrice contenant toutes les pièces dans chacunes de ses positions
const bool Tetromino::pieces[7][4][4][4] =
{
    { // O
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },

    { // I
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        }


    },

    { // S
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0}
        }
    },

    { // Z
        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0}
        }
    },

    { // L
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {1, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0}
        }
    },

    { // J
        {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 1, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        }
    },

    { // T
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },

        {

            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 1, 0, 0}
        },

        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0}
        },
    }
};
