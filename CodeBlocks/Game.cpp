#include "Game.hpp"

Game::Game(sf::Vector2i * window_size) :
    matrix(window_size),
    tetroSuiv(sf::Vector2f(550, 30)),
    textTetroSuiv(),

    _replay(0),
    gameClock()
{
    _window_size = window_size;
    _score = 0;
    _window_size = window_size;

    sf::Font * f = new sf::Font;
    f->loadFromFile("police.ttf");

    tetroSuiv.newPiece(*(new Tetromino(0, 0, sf::Vector2i(0, 0))));
    sf::Text t;
    t.setCharacterSize(15);
    t.setColor(sf::Color::White);
    t.setFont(*f);
    t.setString("Suivant :");
    t.setPosition(550, 5);
    textTetroSuiv.push_back(t);

    _music.openFromFile("tetris.wav");
    _music.play();
    _music.setLoop(true);

    restartGame();

    scoreInfos.setCharacterSize(15);
    scoreInfos.setColor(sf::Color::White);
    scoreInfos.setFont(*f);
    scoreInfos.setPosition(350, 50);


    commandes.setCharacterSize(13);
    commandes.setColor(sf::Color::White);
    commandes.setFont(*f);
    commandes.setPosition(50, 450);
    commandes.setString((wstring)L"Commandes :\n"+
                        L"Gauche et droite : Déplacements\n"+
                        L"Espace : Rotation\n"
                        L"Haut : Descendre rapidement\n"+
                        L"Bas : Descendre d'une case\n"+
                        L"Entrée : Recommencer\n"+
                        L"P/S : Jouer/Stopper la musique");
}

Game::~Game() {}

//évenement des touches de clavier
void Game::onEvent(sf::Event & event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
                case sf::Keyboard::Left:
                    matrix.moveLeft();
                break;
                case sf::Keyboard::Right:
                    matrix.moveRight();
                break;
                case sf::Keyboard::Down:
                    matrix.MoveDown();
                    setTimeLastMoveDown();
                break;
                case sf::Keyboard::Up:
                    matrix.HardDrop();
                break;
                case sf::Keyboard::Space:
                    matrix.rotateRight();
                break;
                case sf::Keyboard::Return:
                    _replay = 1;
                break;
                case sf::Keyboard::S:
                    _music.pause();
                break;
                case sf::Keyboard::P:
                    _music.play();
                break;
            }
        break;
        default: break;
    }
}

bool Game::nextPiece(bool cancelCountTetromino = false)
{
    Tetromino p = tetroSuiv.getPiece();
    if (!matrix.newPiece(p))
        return false;
    tetroSuiv.newPiece(*(new Tetromino(rand_int(0, 6), 0, sf::Vector2i(0, 0))));
    if (!cancelCountTetromino) nbTetro++;
    return true;
}

sf::Time Game::getGameTime()
{
    return gameClock.getElapsedTime();
}

//méthode pour remettre le jeu à zéro
void Game::restartGame()
{
    matrix.clearBoard();
    matrix.newPiece(*(new Tetromino(rand_int(0, 6), 0)));
    tetroSuiv.newPiece(*(new Tetromino(rand_int(0, 6), 0, sf::Vector2i(0, 0))));
    nbLignes = _score = 0;
    nbTetro = 1;
    gameClock.restart();
    setTimeLastMoveDown();
}

void Game::update()
{
    if (_replay == 1)
    {
        restartGame();
        _replay = 0;
    }

    matrix.effacePieceCourrante();

    if (!matrix.pieceActive())
    {   // passage à la pièce suivante
        int LigneToDelete = matrix.viderLigne();
        nbLignes += LigneToDelete;
        switch (LigneToDelete)
        {
            case 1: _score += 10;break;
            case 2: _score += 25; break;
            case 3: _score += 50 ; break;
            case 4: _score += 100; break;
            default: break;
        }
        if (_score > 1999999999)
            _score = 1999999999;

        // on tente de placer la piece suivante
        if (!nextPiece())
            restartGame();
        setTimeLastMoveDown();
    }

    sf::Time time_diff = getGameTime()-timeLastMoveDown;
    if (time_diff > getTimeAutoMoveDown())
    {
        matrix.MoveDown();
       setTimeLastMoveDown();
    }
    scoreInfos.setString("Score : "+to_string(_score, " "));
    matrix.dessinePieceCourrante(false);
    matrix.update();
}

void Game::setTimeLastMoveDown()
{
    sf::Time t = timeLastMoveDown + getTimeAutoMoveDown();
    if (t > getGameTime())
        t = getGameTime();
    timeLastMoveDown = t;
}

sf::Time Game::getTimeAutoMoveDown()
{
	//Variable pour la vitesse
    return sf::seconds(0.6);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(matrix, states);
        target.draw(tetroSuiv, states);
    for (unsigned int i = 0; i<textTetroSuiv.size(); i++)
        target.draw(textTetroSuiv[i], states);
    target.draw(scoreInfos, states);
    target.draw(commandes, states);
}
