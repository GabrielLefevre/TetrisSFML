#include "Application.hpp"
int main()
{
	//Lance le jeu et return 0 lorsque qu'il y a aucune erreur
    srand(time(NULL));
    Application app;
    app.run();
    return 0;
}
