#include "TetrisBoard.hpp"

TetrisBoard::TetrisBoard(sf::Vector2i * window_size) :
    matriceTetro(),
    shapeMatrix(area_width * area_height),
    pieceCourrante(0, 0, sf::Vector2i(INIT_POS_X, INIT_POS_Y))
{
    pieceCouranteActive = true;
    clearBoard();
    _window_size = window_size;

    // positionnement du tetris board à l'écran
    sf::Vector2f top_left_pos(22 * TOP_SIZE, 4*TOP_SIZE);
    matriceTetro.setPosition(top_left_pos);
    matriceTetro.setSize(sf::Vector2f(area_width * TOP_SIZE, area_height * TOP_SIZE));
    matriceTetro.setFillColor(sf::Color(0, 0, 0, 64));
    matriceTetro.setOutlineThickness(1);
    matriceTetro.setOutlineColor(sf::Color::Black);

    for (int i=0; i<area_width; i++)
        for (int j=0; j<area_height; j++)
        {
            shapeMatrix[i*area_height+j] = sf::RectangleShape(sf::Vector2f(TOP_SIZE, TOP_SIZE));
            shapeMatrix[i*area_height+j].setPosition(top_left_pos + sf::Vector2f(TOP_SIZE*i, TOP_SIZE*j));
            shapeMatrix[i*area_height+j].setOutlineThickness(-1);
            shapeMatrix[i*area_height+j].setOutlineColor(sf::Color(255, 255, 255,100));
            shapeMatrix[i*area_height+j].setFillColor(sf::Color::Transparent);
        }
}

TetrisBoard::~TetrisBoard(){}

//Creation d'une piece
bool TetrisBoard::newPiece(Tetromino p, bool init_pos)
{
    pieceCouranteActive = true;
    pieceCourrante = p;
    if (init_pos)
        pieceCourrante.setPosition(sf::Vector2i(INIT_POS_X, INIT_POS_Y));
    if (verifierPlacementPiece(pieceCourrante.getPosition(), pieceCourrante.getOrientation()))
        return true;
    else
    {
        pieceCouranteActive = false;
        return false;
    }
}

//Getter de la piece courrante
Tetromino & TetrisBoard::getPieceCourrante()
{
    return pieceCourrante;
}

//Nettoyer le tetris board
void TetrisBoard::clearBoard()
{
    for (int i=0; i<area_width; i++)
        for (int j=-2; j<area_height; j++)
            setBoardData(i, j, 0);
    pieceCouranteActive = false;
}

//Methode pour faire tomber la piece tout en bas
int TetrisBoard::HardDrop()
{
    int nb_down = 0;
    sf::Vector2i new_pos = pieceCourrante.getPosition();
    sf::Clock c;
    while (verifierPlacementPiece(new_pos + sf::Vector2i(0, 1), pieceCourrante.getOrientation()))
    {
        nb_down++;
        new_pos.y++;
    }
    effacePieceCourrante();
    pieceCourrante.setPosition(new_pos);
    dessinePieceCourrante(false);
    fixTetro();

    return nb_down;
}

//Methode pour faire descendre la piece
bool TetrisBoard::MoveDown()
{
    sf::Vector2i new_pos = pieceCourrante.getPosition() + sf::Vector2i(0, 1);
    if (verifierPlacementPiece(new_pos, pieceCourrante.getOrientation()))
    {
        pieceCourrante.setPosition(new_pos);
        return true;
    }
    else
    {
        effacePieceCourrante();
        dessinePieceCourrante(false);
        fixTetro();
        return false;
    }
}

//Methode pour tester si une piece ou la grille est présente en dessous de la courrante
bool TetrisBoard::MoveDownCheck()
{
    return verifierPlacementPiece(pieceCourrante.getPosition() + sf::Vector2i(0, 1),
                                  pieceCourrante.getOrientation());
}

//Methode pour déplacer la piece vers la gauche
bool TetrisBoard::moveLeft()
{
    sf::Vector2i new_pos = pieceCourrante.getPosition() + sf::Vector2i(-1, 0);
    if (verifierPlacementPiece(new_pos, pieceCourrante.getOrientation()))
    {
        pieceCourrante.setPosition(new_pos);
        return true;
    }
    return false;
}

//Methode pour déplacer la piece vers la droite
bool TetrisBoard::moveRight()
{

    sf::Vector2i new_pos = pieceCourrante.getPosition() + sf::Vector2i(1, 0);
    if (verifierPlacementPiece(new_pos, pieceCourrante.getOrientation()))
    {
        pieceCourrante.setPosition(new_pos);
        return true;
    }
    return false;
}

//Methode pour faire pivoter la piece (vers la droite)
bool TetrisBoard::rotateRight()
{
    int new_rotation = (pieceCourrante.getOrientation()-1)%4;
    sf::Vector3i new_pos = Rotate(pieceCourrante.getOrientation(), new_rotation);
    if (new_pos.z == pieceCourrante.getOrientation())
        return false;
    pieceCourrante.setOrientation(new_rotation);
    pieceCourrante.setPosition(pieceCourrante.getPosition()+
                               sf::Vector2i(new_pos.x, new_pos.y));
    return true;
}

//Methode pour fixer une piece
void TetrisBoard::fixTetro()
{
    pieceCouranteActive = false;

    for (int i=0; i<area_width; i++)
        for (int j=-2; j<area_height; j++)
            if (getBoardData(i, j) >= 20 && getBoardData(i, j) <= 26)
                setBoardData(i, j, getBoardData(i, j) - 10);
}

//Methode pour afficher la piece courrante sur la grille
void TetrisBoard::dessinePieceCourrante(bool drawPiece)
{
    if (!pieceCouranteActive)
        return;
    if (drawPiece)
    {
        int y = pieceCourrante.getPosition().y;
        while (verifierPlacementPiece(sf::Vector2i(pieceCourrante.getPosition().x, y+1), pieceCourrante.getOrientation()))
            y++;
        sf::Vector2i fpos = sf::Vector2i(pieceCourrante.getPosition().x, y);
        MatrixShape fshape = pieceCourrante.getMatrixShape();
        for (int i=0; i<4; i++)
            for (int j=0; j<4; j++)
                if (fshape[i][j])
                    setBoardData(fpos.x + i,
                                 fpos.y + j,
                                 pieceCourrante.getTypePiece() + 30);

    }
    sf::Vector2i pos = pieceCourrante.getPosition();
    MatrixShape shape = pieceCourrante.getMatrixShape();
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            if (shape[i][j])
                setBoardData(pos.x + i, pos.y + j, pieceCourrante.getTypePiece() + 20);
}

bool TetrisBoard::pieceActive()
{
    return pieceCouranteActive;
}

//Methode qui efface la piece pour la faire descendre
void TetrisBoard::effacePieceCourrante()
{
    for (int i=0; i<area_width; i++)
        for (int j=-2; j<area_height; j++)
            if ((getBoardData(i, j) >= 20 && getBoardData(i, j) <= 26) || (getBoardData(i, j) >= 30 && getBoardData(i, j) <= 36))
                setBoardData(i, j, 0);
}

int TetrisBoard::getBoardData(int x, int y) const
{
    if (x<0 || x>=area_width || y<-2 || y>=area_height)
        return 0;
    if (y<0) return areasup[x][y+2];
    else return area[x][y];
}

void TetrisBoard::setBoardData(int x, int y, int data)
{
    if (x<0 || x>=area_width || y<-2 || y>=area_height)
        return;
    if (y<0)
    {
        areasup[x][y+2] = data;
        return;
    }
    area[x][y] = data;
}

//Methode pour effacer une ligne complete
int TetrisBoard::viderLigne()
{
    int nbLignes = 0;
    for (int i=0; i<area_height; i++)
    {   // on parcours les lignes de haut en bas
        bool full = true;
        // on vérifie si la ligne est complète
        for (int j=0; j<area_width && full; j++)
            if (getBoardData(j, i) < 10)
                full = false;
        if (full)
        {
            nbLignes++;
            // on efface la ligne
            for (int j=0; j<area_width; j++)
                setBoardData(j, i, 0);

            // on décale tout ce qui se trouve au dessus
            for (int j=i; j>-2; j--) // on remonte les lignes
                for (int k=0; k<area_width; k++)
                    setBoardData(k, j, getBoardData(k, j-1));

            // on efface la ligne tout en haut
            for (int j=0; j<area_width; j++)
                setBoardData(j, -2, 0);
        }
    }
    return nbLignes;
}

//Methode qui verifie le placement de la piece
bool TetrisBoard::verifierPlacementPiece(sf::Vector2i pos, int o)
{
    // test si la forme sort totalement de l'écran
    if (pos.x > area_width || pos.y > area_height || pos.y+4 < -2 || pos.x+4 < 0)
        return false;
    // crée un tetromino de test qui correspond à la position/orientation/forme testée
    Tetromino pieceTest(pieceCourrante.getTypePiece(), o, pos);
    MatrixShape shape = pieceTest.getMatrixShape();
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            sf::Vector2i posB(pos.x + i, pos.y + j);
            if (shape[i][j] &&
                (posB.x < 0 || posB.y < -2 ||       // ici, on prend en compte que la matrice a 2 lignes invisibles au dessus
                 posB.x >= area_width || posB.y >= area_height ||
                 (getBoardData(posB.x,posB.y) >= 10 && getBoardData(posB.x,posB.y) < 20)))
            {
                return false;
            }
        }
    }
    return true;
}

//Methode qui verifie le deplacement de la piece
bool TetrisBoard::verifierDeplacementPiece(sf::Vector2i var_pos, int o)
{
    return verifierPlacementPiece(pieceCourrante.getPosition() + var_pos, o);
}

// Vérifie si le déplacement de la pièce est possible
sf::Vector3i TetrisBoard::Rotate(int actualOrient, int nextOrientTest)
{
    if(pieceCourrante.getTypePiece() == 0 ||
                verifierDeplacementPiece(sf::Vector2i(0, 0), nextOrientTest))
        return sf::Vector3i(0, 0, nextOrientTest);
    if (pieceCourrante.getTypePiece() == 1)
    {
        if ((actualOrient == 2 && nextOrientTest == 3) ||
            (actualOrient == 1 && nextOrientTest == 0))
        {
            if (verifierDeplacementPiece(sf::Vector2i(1, 0), nextOrientTest))
                return sf::Vector3i(1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-2, 0), nextOrientTest))
                return sf::Vector3i(-2, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, 2), nextOrientTest))
                return sf::Vector3i(1, 2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-2, -1), nextOrientTest))
                return sf::Vector3i(-2, -1, nextOrientTest);
        }
        else if ((actualOrient == 3 && nextOrientTest == 0) ||
            (actualOrient == 2 && nextOrientTest == 1))
        {
            if (verifierDeplacementPiece(sf::Vector2i(2, 0), nextOrientTest))
                return sf::Vector3i(2, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, 0), nextOrientTest))
                return sf::Vector3i(-1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(2, -1), nextOrientTest))
                return sf::Vector3i(2, -1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, 2), nextOrientTest))
                return sf::Vector3i(-1, 2, nextOrientTest);
        }
        else if ((actualOrient == 3 && nextOrientTest == 2) ||
            (actualOrient == 0 && nextOrientTest == 1))
        {
            if (verifierDeplacementPiece(sf::Vector2i(-1, 0), nextOrientTest))
                return sf::Vector3i(-1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(2, 0), nextOrientTest))
                return sf::Vector3i(2, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, -2), nextOrientTest))
                return sf::Vector3i(-1, -2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(2, 1), nextOrientTest))
                return sf::Vector3i(2, 1, nextOrientTest);
        }
        else if ((actualOrient == 0 && nextOrientTest == 3) ||
            (actualOrient == 1 && nextOrientTest == 2))
        {
            if (verifierDeplacementPiece(sf::Vector2i(-2, 0), nextOrientTest))
                return sf::Vector3i(-2, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, 0), nextOrientTest))
                return sf::Vector3i(1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-2, 1), nextOrientTest))
                return sf::Vector3i(-2, 1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, -2), nextOrientTest))
                return sf::Vector3i(1, -2, nextOrientTest);
        }
    }
    else
    {
        if ((actualOrient == 2 && nextOrientTest == 1) ||
            (actualOrient == 0 && nextOrientTest == 1))
        {
            if (verifierDeplacementPiece(sf::Vector2i(1, 0), nextOrientTest))
                return sf::Vector3i(1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, -1), nextOrientTest))
                return sf::Vector3i(1, -1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(0, 2), nextOrientTest))
                return sf::Vector3i(0, 2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, 2), nextOrientTest))
                return sf::Vector3i(1, 2, nextOrientTest);
        }
        else if ((actualOrient == 3 && nextOrientTest == 0) ||
            (actualOrient == 3 && nextOrientTest == 2))
        {
            if (verifierDeplacementPiece(sf::Vector2i(1, 0), nextOrientTest))
                return sf::Vector3i(1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, 1), nextOrientTest))
                return sf::Vector3i(1, 1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(0, -2), nextOrientTest))
                return sf::Vector3i(0, -2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, -2), nextOrientTest))
                return sf::Vector3i(1, -2, nextOrientTest);
        }
        else if ((actualOrient == 0 && nextOrientTest == 3) ||
            (actualOrient == 2 && nextOrientTest == 3))
        {
            if (verifierDeplacementPiece(sf::Vector2i(-1, 0), nextOrientTest))
                return sf::Vector3i(-1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, -1), nextOrientTest))
                return sf::Vector3i(-1, -1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(0, 2), nextOrientTest))
                return sf::Vector3i(0, 2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, 2), nextOrientTest))
                return sf::Vector3i(-1, 2, nextOrientTest);
        }
        else if ((actualOrient == 1 && nextOrientTest == 2) ||
            (actualOrient == 1 && nextOrientTest == 0))
        {
            if (verifierDeplacementPiece(sf::Vector2i(-1, 0), nextOrientTest))
                return sf::Vector3i(-1, 0, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(-1, 1), nextOrientTest))
                return sf::Vector3i(-1, 1, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(0, -2), nextOrientTest))
                return sf::Vector3i(0, -2, nextOrientTest);
            else if (verifierDeplacementPiece(sf::Vector2i(1, -2), nextOrientTest))
                return sf::Vector3i(1, -2, nextOrientTest);
        }
    }
    return sf::Vector3i(0, 0, actualOrient);
}

void TetrisBoard::update()
{
    for (int x=0; x<area_width; x++)
        for (int y=0; y<area_height; y++)
        {
          if (area[x][y] >= 10 && area[x][y] < 17)
                {
                    sf::Color c = Tetromino::couleurs[area[x][y]-10];
                    c.a = 192;
                    shapeMatrix[x*area_height+y].setFillColor(c);
                }
                else if (area[x][y] >= 20 && area[x][y] < 27)
                {
                    sf::Color c = Tetromino::couleurs[area[x][y]-20];
                    shapeMatrix[x*area_height+y].setFillColor(c);
                }
                else if (area[x][y] >= 30 && area[x][y] < 37)
                {
                    sf::Color c = Tetromino::couleurs[area[x][y]-30];
                    c.a = 64;
                    shapeMatrix[x*area_height+y].setFillColor(c);
                }
                else
                    shapeMatrix[x*area_height+y].setFillColor(sf::Color::Transparent);
        }
}

//Methode pour déssiner le tetris board (grille)
void TetrisBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(matriceTetro, states);
    for (int i=0; i< (area_width*area_height); i++)
        target.draw(shapeMatrix[i], states);
}
