#include <iostream>


#include <array>
#include <ctime>

#include "src/farm/Entity.h"
#include "src/ui/MainWindow.h"


using namespace std;
using namespace std::chrono;
using namespace sf;







int main(int argc, char **argv)
{

    srand((unsigned) time(0));

    cout << "Hello ?" << endl;

    MainWindow window = MainWindow();
    window.start();

    return 0;
}
