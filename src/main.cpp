#include "app.h"

int main(int argc, char* argv[]) {
    InitialiseProgram();
    MainLoop();
    Cleanup();
    return 0;
}
