#pragma once

#include <json.hpp>

enum DumperState : int {
    DumperState_ObjectMethods,
    DumperState_GlobalFunctions,
    DumperState_GlobalProperties
};

class MemoryReader;

class RegistryDumper {
public:
    RegistryDumper(const char* outputPath, MemoryReader* memoryReader)
        : m_outputPath(outputPath), m_memoryReader(memoryReader) {}

    void dumpRegistry();

    const char* getNamespace() const { return &m_namespaceBuffer[0]; }
    const char* getDeclaration() const { return &m_declarationBuffer[0]; }

    const bool isDumping() const { return m_isDumping; }
    const DumperState getDumperState() const { return m_dumperState; }

private:
    void dumpGlobalFunctions();
    void dumpObjectMethods();
    void dumpGlobalProperties();

private:
    const char* m_outputPath = nullptr;
    MemoryReader* m_memoryReader = nullptr;
    nlohmann::json m_jsonObject;
    char m_namespaceBuffer[64];
    char m_declarationBuffer[256];
    DumperState m_dumperState = DumperState_ObjectMethods;
    bool m_isDumping = false;

};
