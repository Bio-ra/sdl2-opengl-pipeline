#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>


#include "quad.h"
#include "app.h"
#include "shaderLoader.h"
#include "shaderUniformHandler.h"
#include "textureHandler.h"
#include "transformation.h"
#include "camera.h"
#include "sprite.h"  

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* window = nullptr; 
SDL_GLContext glContext = nullptr;

//#################################################
// Initialisation function
//#################################################


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

    window = SDL_CreateWindow(
        "Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );

    if (window == nullptr) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }

    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: "
                  << glewGetErrorString(glewStatus) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    CreateQuadGeometry();
}

//#################################################
// Main application loop
//#################################################

void MainLoop() {
    bool running = true;
    SDL_Event e;

    GLuint shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);
    
    Shader shader(shaderProgram);
    Texture dogtexture("dog.png");

    // create sprites using the Texture we loaded
    Sprite sprite1(&dogtexture);
    sprite1.setPosition(200.0f, 300.0f);
    sprite1.setSize(200.0f, 200.0f);

    Sprite sprite2(&dogtexture);
    sprite2.setPosition(600.0f, 150.0f);
    sprite2.setSize(150.0f, 150.0f);
    sprite2.setRotation(0.3f);

    Sprite sprite3(&dogtexture);
    sprite3.setPosition(-100.0f, -150.0f);
    sprite3.setSize(350.0f, 350.0f);
    sprite3.setRotation(0.6f);

    // get uniform locations once
    GLint locProjection = glGetUniformLocation(shaderProgram, "uProjection");
    GLint locModel = glGetUniformLocation(shaderProgram, "uModel");
    GLint locSampler = glGetUniformLocation(shaderProgram, "spriteTexture");
    GLint locTint = glGetUniformLocation(shaderProgram, "uTint");

    // Set sampler to texture unit 0 and default tint to opaque white
    if (locSampler >= 0) glUniform1i(locSampler, 0);
    if (locTint >= 0) glUniform4f(locTint, 1.0f, 1.0f, 1.0f, 1.0f);
    
    // Create camera (safe: initialized after uniforms, before loop)
    Camera2D camera((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f, 1.0f);
    
    // Mouse drag state
    bool isDragging = false;
    int lastMouseX = 0;
    int lastMouseY = 0;
    
    while (running) {

        //##### SDL POLL EVENT #####
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                isDragging = true;
                lastMouseX = e.button.x;
                lastMouseY = e.button.y;
            }
            else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                isDragging = false;
            }
            else if (e.type == SDL_MOUSEMOTION && isDragging) { //drag
                int dx = e.motion.x - lastMouseX;
                int dy = e.motion.y - lastMouseY;
                camera.move((float)-dx, (float)-dy);  
                lastMouseX = e.motion.x;
                lastMouseY = e.motion.y;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) camera.zoomBy(1.0f/1.1f);      // zoom
                else if (e.wheel.y < 0) camera.zoomBy(1.1f);  
            }
            else if (e.type == SDL_WINDOWEVENT &&
                     (e.window.event == SDL_WINDOWEVENT_RESIZED ||  //remake viewport on resize
                      e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
                int newW = e.window.data1;
                int newH = e.window.data2;
                glViewport(0, 0, newW, newH);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Upload camera view-projection (safe: done once per frame)
        Mat3 projection = camera.getViewProjection((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, true);
        if (locProjection >= 0) glUniformMatrix3fv(locProjection, 1, GL_FALSE, projection.data());

        // Draw sprites (handles texture bind, model & tint uniforms, and DrawQuad)
        sprite1.draw(locModel, locTint);
        sprite2.draw(locModel, locTint);
        sprite3.draw(locModel, locTint);
        
        SDL_GL_SwapWindow(window);

    glDeleteProgram(shaderProgram);
    }
}

//#################################################
// cleanup function
//#################################################

void Cleanup() {

    DeleteQuadGeometry();

    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}
