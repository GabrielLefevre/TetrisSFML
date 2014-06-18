#include "TetrominoBoard.hpp"

TetrominoBoard::TetrominoBoard(sf::Vector2f position) :
    shapeMatrix(2*4),
    matriceTetro(),
    piece(0, 0, sf::Vector2i(INIT_POS_X, INIT_POS_Y))
{
    viderPiece();

    matriceTetro.setPosition(position);
    matriceTetro.setSize(sf::Vector2f(4 * TOP_SIZE, 2 * TOP_SIZE));
    matriceTetro.setFillColor(sf::Color(0, 0, 0, 64));
    matriceTetro.setOutlineThickness(1);
    matriceTetro.setOutlineColor(sf::Color::Black);

    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++)
        {
            shapeMatrix[i*2+j] = *(new sf::RectangleShape(sf::Vector2f(TOP_SIZE, TOP_SIZE)));
            shapeMatrix[i*2+j].setPosition(position + sf::Vector2f(TOP_SIZE*i, TOP_SIZE*j));
            shapeMatrix[i*2+j].setOutlineThickness(-1);
            shapeMatrix[i*2+j].setOutlineColor(sf::Color(255, 255, 255,100));
            shapeMatrix[i*2+j].setFillColor(sf::Color::Transparent);
        }
}

TetrominoBoard::~TetrominoBoard() {}

void TetrominoBoard::newPiece(Tetromino & t)
{
    piece = t;
    viderPiece();
    dessinePiece();
}

Tetromino TetrominoBoard::getPiece()
{
    return piece;
}

void TetrominoBoard::dessinePiece()
{
    MatrixShape shape = piece.getMatrixShape();
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
        {
            if (shape[i][j])
                setMatrixData(i, j, piece.getTypePiece() + 20);
        }
}

void TetrominoBoard::viderPiece()
{
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            setMatrixData(i, j, 0);
}

void TetrominoBoard::setMatrixData(int x, int y, int data)
{
    if (x<0 || x>=4 || y<1 || y>=3)
        return;
    matrix[x][y] = data;

    if (matrix[x][y] >= 20 && matrix[x][y] < 27)
    {
        sf::Color c = Tetromino::couleurs[matrix[x][y]-20];
        shapeMatrix[x*2+y-1].setFillColor(c);
    }
    else
        shapeMatrix[x*2+y-1].setFillColor(sf::Color::Transparent);
}

void TetrominoBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(matriceTetro, states);
    for (int i=0; i< (2*4); i++)
        target.draw(shapeMatrix[i], states);
}