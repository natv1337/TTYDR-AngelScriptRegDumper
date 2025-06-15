#pragma once

#include "Types.hpp"
#include <stdio.h>

struct AddressSpace {
    u64 StartAddress = 0;
    u64 Size         = 0;
};

class MemoryReader {
public:
    MemoryReader();

    bool readAbsolute(u64 address, void* buffer, size_t bufferSize) const;
    bool readMain(u64 offset, void* buffer, size_t bufferSize) const;
    bool readHeap(u64 offset, void* buffer, size_t bufferSize) const;

    const u64 getProcessId() const { return m_processId; }
    const u64 getTitleId() const { return m_titleId; }

    const u64 getMainStart() const { return m_main.StartAddress; }
    const u64 getMainSize() const { return m_main.Size; }

    const u64 getHeapStart() const { return m_heap.StartAddress; }
    const u64 getHeapSize() const { return m_heap.Size; }

private:
    u64 m_processId     = 0;
    u64 m_titleId       = 0;
    AddressSpace m_main = {};
    AddressSpace m_heap = {};
};
