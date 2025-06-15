#include "MemoryReader.hpp"

#include <dmntcht.h>

MemoryReader::MemoryReader() {
    pmdmntGetApplicationProcessId(&m_processId);
    pminfoGetProgramId(&m_titleId, m_processId);

    dmntchtForceOpenCheatProcess();
    DmntCheatProcessMetadata metadata;
    dmntchtGetCheatProcessMetadata(&metadata);

    m_main.StartAddress = metadata.main_nso_extents.base;
    m_main.Size         = metadata.main_nso_extents.size;

    m_heap.StartAddress = metadata.heap_extents.base;
    m_heap.Size         = metadata.heap_extents.size;
}

bool MemoryReader::readAbsolute(u64 address, void* buffer, size_t bufferSize) const {
    if (R_SUCCEEDED(dmntchtReadCheatProcessMemory(address, buffer, bufferSize))) {
        return true;
    }

    return false;
}

bool MemoryReader::readMain(u64 offset, void* buffer, size_t bufferSize) const {
    if (R_SUCCEEDED(dmntchtReadCheatProcessMemory(m_main.StartAddress + offset, buffer, bufferSize))) {
        return true;
    }

    return false;
}

bool MemoryReader::readHeap(u64 offset, void* buffer, size_t bufferSize) const {
    if (R_SUCCEEDED(dmntchtReadCheatProcessMemory(m_heap.StartAddress + offset, buffer, bufferSize))) {
        return true;
    }

    return false;
}
