//
// Created by Amalric Lombard de Buffières on 7/3/20.
//


#include <SDL_types.h>
#include <SDL_pixels.h>

Uint32 ColourToUint(int R, int G, int B)
{
    return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

Uint32 SDLColourToUint(SDL_Color color)
{
    return (Uint32)((color.r << 16) + (color.g << 8) + (color.b << 0));
}



SDL_Colour UintToColour(Uint32 colour)
{
    SDL_Colour tempcol;
    tempcol.a = 255;
    tempcol.r = (colour >> 16) & 0xFF;
    tempcol.g = (colour >> 8) & 0xFF;
    tempcol.b = colour & 0xFF;
    return tempcol;
}