#include <iostream>


#include <array>
#include <ctime>
#include <bsoncxx/json.hpp>

#include "src/ui/MainWindow.h"
#include "src/websockets/WebUiConnection.h"
#include "src/mongo/MongoClient.h"


int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    MainWindow window = MainWindow();
    window.start();




    return 0;
}


