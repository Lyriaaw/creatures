#include <iostream>


#include <array>
#include <ctime>

#include "portaudio/include/portaudio.h"


#include "src/ui/MainWindow.h"
#define SAMPLE_RATE (44100)


using namespace std;
using namespace std::chrono;
using namespace sf;


typedef int PaStreamCallback( const void *input,
                              void *output,
                              unsigned long frameCount,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData ) ;
typedef struct
{
    float left_phase;
    float right_phase;
}
        paTestData;

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;
}

void trySound() {

    auto err = Pa_Initialize();
    if( err != paNoError ){
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

    }


    err = Pa_Terminate();
    if( err != paNoError )
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );


}




int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    trySound();

    std::cout << "Hello World!" << std::endl;


//    MainWindow window = MainWindow();
//    window.start();

    return 0;
}

