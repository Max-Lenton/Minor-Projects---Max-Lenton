#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_version compiled;
        SDL_VERSION(&compiled);
        printf("SDL2 is installed. Compiled version: %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
        SDL_Quit();
        return 0;
    } else {
        printf("SDL2 is not installed or could not be initialized.\n");
        return 1;
    }
}