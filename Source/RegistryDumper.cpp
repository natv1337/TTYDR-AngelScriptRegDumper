#include "RegistryDumper.hpp"

#include <fstream>
#include <switch.h>

#include "MemoryReader.hpp"

void RegistryDumper::dumpRegistry() {
    m_isDumping = true;

    dumpObjectMethods();
    dumpGlobalFunctions();
    dumpGlobalProperties();

    std::ofstream outputStream(m_outputPath);
    outputStream << m_jsonObject.dump(4);
    outputStream.close();

    m_isDumping = false;
}

void RegistryDumper::dumpGlobalFunctions() {
    m_dumperState = DumperState_GlobalFunctions;

    u64 currentGlobalFunctionPtr = 0;
    m_memoryReader->readMain(0x2502490, &currentGlobalFunctionPtr, sizeof(currentGlobalFunctionPtr));

    ASGlobalFunction globalFunction;
    nlohmann::json globalFunctionArray = nlohmann::json::array();

    while (currentGlobalFunctionPtr) {
        m_memoryReader->readAbsolute(currentGlobalFunctionPtr, &globalFunction, sizeof(globalFunction));
        u64 nextPointer = globalFunction.NextGlobalFunction;

        m_memoryReader->readAbsolute(globalFunction.Namespace, &m_namespaceBuffer[0], sizeof(m_namespaceBuffer));
        m_memoryReader->readAbsolute(globalFunction.Declaration, &m_declarationBuffer[0], sizeof(m_declarationBuffer));
        u64 functionPointer = globalFunction.FuncPtr;

        globalFunctionArray.push_back(
            {
                { "namespace", m_namespaceBuffer },
                { "declaration", m_declarationBuffer },
                { "pointer", functionPointer - m_memoryReader->getMainStart() }
            }
        );

        currentGlobalFunctionPtr = nextPointer;
    }

    m_jsonObject["global_functions"] = globalFunctionArray;
}

void RegistryDumper::dumpObjectMethods() {
    m_dumperState = DumperState::DumperState_ObjectMethods;

    u64 currentObjectMethodPtr = 0;
    m_memoryReader->readMain(0x25024B8, &currentObjectMethodPtr, sizeof(currentObjectMethodPtr));
    
    ASObjectMethod objectMethod;
    nlohmann::json objectMethodArray = nlohmann::json::array();

    char objectName[64];
    while (currentObjectMethodPtr) {
        m_memoryReader->readAbsolute(currentObjectMethodPtr, &objectMethod, sizeof(objectMethod));
        u64 nextPointer = objectMethod.NextObjectMethod;

        m_memoryReader->readAbsolute(objectMethod.Namespace, &m_namespaceBuffer[0], sizeof(m_namespaceBuffer));
        m_memoryReader->readAbsolute(objectMethod.Declaration, &m_declarationBuffer[0], sizeof(m_declarationBuffer));
        m_memoryReader->readAbsolute(objectMethod.ObjectName, &objectName[0], sizeof(objectName));
        u64 functionPointer = objectMethod.FuncPtr;

        objectMethodArray.push_back(
            {
                { "namespace", m_namespaceBuffer },
                { "object", objectName },
                { "declaration", m_declarationBuffer },
                { "pointer", functionPointer }
            }
        );

        currentObjectMethodPtr = nextPointer;
    }

    m_jsonObject["object_methods"] = objectMethodArray;
}

void RegistryDumper::dumpGlobalProperties() {
    m_dumperState = DumperState_GlobalProperties;

    u64 currentGlobalPropertyPtr = 0;
    m_memoryReader->readMain(0x25024C0, &currentGlobalPropertyPtr, sizeof(currentGlobalPropertyPtr));

    ASGlobalProperty globalProperty;
    nlohmann::json globalPropertyArray = nlohmann::json::array();

    while (currentGlobalPropertyPtr) {
        m_memoryReader->readAbsolute(currentGlobalPropertyPtr, &globalProperty, sizeof(globalProperty));
        u64 nextPointer = globalProperty.NextGlobalProperty;

        m_memoryReader->readAbsolute(globalProperty.Namespace, &m_namespaceBuffer[0], sizeof(m_namespaceBuffer));
        m_memoryReader->readAbsolute(globalProperty.Declaration, &m_declarationBuffer[0], sizeof(m_declarationBuffer));
        u64 valuePointer = globalProperty.ValuePtr;

        globalPropertyArray.push_back(
            {
                {"namespace", m_namespaceBuffer},
                {"declaration", m_declarationBuffer}, 
                {"pointer", valuePointer - m_memoryReader->getMainStart()}
            }
        );

        currentGlobalPropertyPtr = nextPointer;
    }

    m_jsonObject["global_properties"] = globalPropertyArray;
}
