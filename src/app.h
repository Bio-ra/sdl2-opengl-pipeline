#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <GL/gl.h>

// screen dimensions
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Global window and GL context (accessible for now)
extern SDL_Window* window;
extern SDL_GLContext glContext;

// lifecycle functions
void InitialiseProgram();
void MainLoop();
void Cleanup();

#endif // APP_H
