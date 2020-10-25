#include <iostream>


#include <array>
#include <ctime>

#include "src/ui/MainWindow.h"
#include "src/websockets/WebUiConnection.h"



//------------------------------------------------------------------------------





//------------------------------------------------------------------------------


int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    MainWindow window = MainWindow();
    window.start();


    return 0;
}


