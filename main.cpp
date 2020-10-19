#include <iostream>


#include <array>
#include <ctime>

#include "src/ui/MainWindow.h"
#include <SFML/Audio.hpp>
#include <unistd.h>

#include "stk/Noise.h"


#include "stk/SineWave.h"
#include "stk/RtAudio.h"
using namespace stk;
using namespace stk;

using namespace stk;
using namespace stk;

using namespace std;
using namespace std::chrono;
using namespace sf;


int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
          double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    SineWave *sine = (SineWave *) dataPointer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    for ( unsigned int i=0; i<nBufferFrames; i++ ) {
        *samples++ = sine->tick();
//        std::cout << "Tick ?" << std::endl;
    }
    return 0;
}

int tick2( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
          double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    SineWave *sine = (SineWave *) dataPointer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    for ( unsigned int i=0; i<nBufferFrames; i++ ) {
        *samples++ = sine->tick();
//        std::cout << "Tick ?" << std::endl;
    }
    return 0;
}

void testSound(double frequency) {
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 44100.0 );
    SineWave sine;
    RtAudio dac;

    std::cout << dac.getDeviceInfo(1).outputChannels << std::endl;


    // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 1;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    try {
        dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, frequency == 440 ? &tick : &tick2, (void *) &sine );
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
        return;
    }
    sine.setFrequency(frequency);
    sine.addPhase(0.1);
    try {
        dac.startStream();
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
        return;
    }

    usleep(10000000);



    // Block waiting here.
    // Shut down the output stream.
    try {
        dac.closeStream();
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
    }
}




int main(int argc, char **argv)
{

    auto f = [](double frequency){
        testSound(frequency);
    };




    std::thread soundThread1(f, 440);
    std::thread soundThread2(f, 300);

//    soundThread1.join();
    soundThread2.join();
    std::cout << "Finished" << std::endl;

//    srand((unsigned) time(0));
//
//
//
//
//    MainWindow window = MainWindow();
//    window.start();

    return 0;
}
