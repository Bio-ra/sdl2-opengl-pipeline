#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>

// globals
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* window = nullptr; 
SDL_GLContext glContext = nullptr;

//initialise program function
void InitialiseProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    window = SDL_CreateWindow(
        "Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if (window == nullptr) {
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

}

void Main(){
    // Main loop
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int newW = e.window.data1;
                    int newH = e.window.data2;
                    glViewport(0, 0, newW, newH); //update viewport on resize
                }
            }
        }

        // clear and present
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }
}

void Cleanup(){

    if (glContext) {
        SDL_GL_DeleteContext(glContext); //  delete context
        glContext = nullptr;
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char* argv[]) {
    // Initialize SDL
    InitialiseProgram();

    // Run main loop
    Main();

    // Cleanup and exit
    Cleanup();

    return 0;
}
