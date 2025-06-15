#pragma once

#include "Types.hpp"

#include <unordered_map>

enum class MenuState {
    MainMenu,
    PreDump,
    Dumping,
    PostDump,
    Exit,
    Error
};

#define DUMP_OUTPUT_PATH "sdmc:/ttydr_output.json"

class Application {
public:
    void initialize();
    void uninitialize();

    void run();

    void beginDumping();

private:
    void initializeServices();
    void uninitializeServices();

    void printMenu();
    void handleStateInput();

    void enterState(MenuState state);
    void updateState();

    static void dumpingRoutine(void* arg);

private:
    class MemoryReader* m_memoryReader;
    class RegistryDumper* m_registryDumper;

    MenuState m_menuState = MenuState::MainMenu;

    static inline bool s_dumpThreadRunning = false;

};

inline Application* g_application = nullptr;