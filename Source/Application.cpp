#include "Application.hpp"

#include "MemoryReader.hpp"
#include "RegistryDumper.hpp"

#include <dmntcht.h>
#include <switch.h>

static ::PadState s_padState;
static u64 s_padDown;
static ::Thread s_dumpingThread;
static bool g_dumpThreadRunning = false;

void Application::initialize() {
    g_application = this;

    ::consoleInit(nullptr);
    ::padConfigureInput(1, ::HidNpadStyleSet_NpadStandard);
    ::padInitializeDefault(&s_padState);

    initializeServices();
    m_memoryReader   = new MemoryReader();
    m_registryDumper = new RegistryDumper(DUMP_OUTPUT_PATH, m_memoryReader);

    threadCreate(&s_dumpingThread, Application::dumpingRoutine, nullptr, nullptr, 0x10000, 0x2C, -2);
}

void Application::uninitialize() {
    threadClose(&s_dumpingThread);
    uninitializeServices();
    delete m_registryDumper;
    delete m_memoryReader;

    ::consoleExit(nullptr);
}

void Application::initializeServices() {
    ::pmdmntInitialize();
    ::pminfoInitialize();
    ::dmntchtInitialize();
}

void Application::uninitializeServices() {
    ::dmntchtExit();
    ::pminfoExit();
    ::pmdmntExit();
}

inline const char* g_dumperStates[] = {"Object Methods", "Global Functions", "Global Properties"};

void Application::printMenu() {
    switch (m_menuState) {
    case MenuState::MainMenu:
        printf("\n PMTTYDR AngelScript Registry Dumper\n\n (A) Dump\n (+) Exit\n");
        break;
    case MenuState::PreDump:
        printf("\n Begin dumping? This may take a while.\n\n (A) Dump\n (B) Cancel\n");
        break;
    case MenuState::Dumping:
        printf("\n Dumping %s...\n %s\n %s\n", 
            g_dumperStates[m_registryDumper->getDumperState()], 
            m_registryDumper->getNamespace(),
            m_registryDumper->getDeclaration()
        );
        break;
    case MenuState::PostDump:
        printf("\n Finished dumping.\n\n (A) Continue");
        break;
    case MenuState::Error:
        printf("\n An error has occured. Please try again.\n");
        break;
    default:
        break;
    }
}

void Application::handleStateInput() {
    switch (m_menuState) {
    case MenuState::MainMenu:
        if (s_padDown & ::HidNpadButton_A) {
            enterState(MenuState::PreDump);
        } else if (s_padDown & ::HidNpadButton_Plus) {
            enterState(MenuState::Exit);
        }
        break;
    case MenuState::PreDump:
        if (s_padDown & ::HidNpadButton_A) {
            enterState(MenuState::Dumping);
        } else if (s_padDown & ::HidNpadButton_B) {
            enterState(MenuState::MainMenu);
        }
        break;
    case MenuState::PostDump:
        if (s_padDown & ::HidNpadButton_A) {
            enterState(MenuState::MainMenu);
        }
        break;
    default:
        break;
    }
}

void Application::enterState(MenuState state) {
    m_menuState = state;
    switch (state) {
    case MenuState::Dumping:
        g_dumpThreadRunning = true;
        threadStart(&s_dumpingThread);
        break;
    case MenuState::PostDump:
        break;
    default:
        break;
    }
}

void Application::updateState() {
    switch (m_menuState) {
    case MenuState::Dumping:
        if (!g_dumpThreadRunning) {
            printf("Here\n");
            threadWaitForExit(&s_dumpingThread);
            enterState(MenuState::PostDump);
        }
        break;
    default:
        break;
    }
}

void Application::run() {
    while (::appletMainLoop()) {
        if (m_menuState == MenuState::Exit) {
            break;
        }

        ::consoleClear();

        ::padUpdate(&s_padState);
        s_padDown = ::padGetButtonsDown(&s_padState);

        handleStateInput();
        printMenu();
        updateState();

        ::consoleUpdate(nullptr);
    }
}

void Application::beginDumping() { m_registryDumper->dumpRegistry(); }

void Application::dumpingRoutine(void* arg) {
    g_application->beginDumping();
    g_dumpThreadRunning = false;
}
