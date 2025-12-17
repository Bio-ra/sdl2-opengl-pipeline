#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "quad.h"
#include "app.h"
#include "shaderLoader.h"
#include "shaderUniformHandler.h"
#include "textureHandler.h"
#include "transformation.h"
#include "camera.h"
#include "sprite.h"
#include "spriteBatch.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* window = nullptr; 
SDL_GLContext glContext = nullptr;

// #################################################
// Initialisation function
// #################################################


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

    if (!window) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }

    glContext = SDL_GL_CreateContext(window);

    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

    // GLEW initialization
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_BLEND);

    // prefer separate to handle color and alpha correctly
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    CreateQuadGeometry();
}

// #################################################
// Main application loop
// #################################################

void MainLoop() {


    bool running = true;
    SDL_Event e;

    GLuint shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);
    
    Shader shader(shaderProgram);
    Texture dogtexture("dog.png");
    Texture bombTexture("spriteAtlas_bomb_walk.png");

    Sprite sprite1(&dogtexture);
    sprite1.setPosition(100.0f, 200.0f);
    sprite1.setSize(200.0f, 200.0f);

    // Sprite sprite2(&bombTexture);
    // sprite2.setPosition(600.0f, 150.0f);
    // sprite2.setSize(19.0f * 8.0f, 19.0f * 8.0f);
    // sprite2.setFrame(0, 0, 4, 1);

    SpriteBatch batch(1024);

    // Create multiple sprite instances
    std::vector<Sprite> bombs;
    const int bombCount = 20;
    const int cols = 5;
    const float startX = 400.0f;
    const float startY = 200.0f;
    const float spacingX = 20.0f;
    const float spacingY = 20.0f;
    for (int i = 0; i < bombCount; ++i) {
        Sprite b(&bombTexture);
        int col = i % cols;
        int row = i / cols;
        b.setPosition(startX + col * spacingX, startY + row * spacingY);
        b.setSize(19.0f, 19.0f);
        b.setFrame(0, 0, 4, 1);
        bombs.push_back(b);
    }

    // get uniform locations once
    GLint locProjection = glGetUniformLocation(shaderProgram, "uProjection");
    GLint locModel = glGetUniformLocation(shaderProgram, "uModel");
    GLint locSampler = glGetUniformLocation(shaderProgram, "spriteTexture");
    GLint locTint = glGetUniformLocation(shaderProgram, "uTint");

    // Set sampler to texture unit 0 and default tint to opaque white
    if (locSampler >= 0) glUniform1i(locSampler, 0);
    if (locTint >= 0) glUniform4f(locTint, 1.0f, 1.0f, 1.0f, 1.0f);
    
    // Create camera
    Camera2D camera(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f, 1.0f);
    
    // Mouse drag state
    bool isDragging = false;
    int lastMouseX = 0, lastMouseY = 0;
    
    // timing / animation state
    Uint64 perfFreq = SDL_GetPerformanceFrequency();
    Uint64 lastTicks = SDL_GetPerformanceCounter();
    float animTime = 0.0f;
    // set FPS
    const float animFPS = 8.0f;

    while (running) {

        // ##### SDL POLL EVENT #####
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
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
            else if (e.type == SDL_MOUSEMOTION && isDragging) {
                camera.move(-(e.motion.x - lastMouseX), -(e.motion.y - lastMouseY));
                lastMouseX = e.motion.x;
                lastMouseY = e.motion.y;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                camera.zoomBy(e.wheel.y > 0 ? 1.0f / 1.1f : 1.1f);
            }
            else if (e.type == SDL_WINDOWEVENT && 
                     (e.window.event == SDL_WINDOWEVENT_RESIZED || 
                      e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Get current window size (handle resizing) and upload camera view-projection
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);

        // Request view-projection WITHOUT camera zoom so sprites keep constant size
        Mat3 projection = camera.getViewProjection((float)winW, (float)winH, true, false);
        if (locProjection >= 0) glUniformMatrix3fv(locProjection, 1, GL_FALSE, projection.data());

        // Use SpriteBatch to submit all sprites
        batch.begin();
        sprite1.submitToBatch(batch);
        // sprite2.submitToBatch(batch);
        for (auto &b : bombs) b.submitToBatch(batch);
        batch.end(locModel, locTint);

        // Update sprite animation frame based on time
        Uint64 currentTicks = SDL_GetPerformanceCounter();
        animTime += (float)(currentTicks - lastTicks) / perfFreq;
        lastTicks = currentTicks;
        if (animTime >= 1.0f / animFPS) {
            // sprite2.nextFrame();
            for (auto &b : bombs) b.nextFrame();
            animTime = 0.0f;
        }

        SDL_GL_SwapWindow(window);

    }
    glDeleteProgram(shaderProgram);
}

// #################################################
// cleanup function
// #################################################

void Cleanup() {
    
    DeleteQuadGeometry();

    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}
