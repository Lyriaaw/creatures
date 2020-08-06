#include <iostream>


#include <array>
#include <ctime>

#include "src/ui/MainWindow.h"


using namespace std;
using namespace std::chrono;
using namespace sf;







int main(int argc, char **argv)
{
    srand((unsigned) time(0));

//    Farm * farm = new Farm();
//    farm->InitFromRandom();
//
//    while (true) {
//        farm->Tick(false);
//    }


    MainWindow window = MainWindow();
    window.start();

    return 0;
}
