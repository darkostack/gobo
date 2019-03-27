#ifndef GO_HEAP_HPP_
#define GO_HEAP_HPP_

#include "gobo-core-config.h"

#include <stddef.h>

#include "utils/wrap_stdint.h"

namespace go {
namespace Utils {

/**
 * This class represents a memory block.
 *
 * A block is of the structure as below.
 *
 *     +------------------------------+
 *     | mSize   |  mMemory |  mNext  |
 *     |---------|----------| --------|
 *     | 2 bytes |  n bytes | 2 bytes |
 *     +------------------------------+
 *
 * Since block metadata is of 4-byte size, mSize and mNext are separated at the beginning
 * and end of the block to make sure the mMemory is aligned with long.
 *
 */
class Block
{
    friend class Heap;

public:
    uint16_t GetSize(void) const { return mSize; }

    void SetSize(uint16_t aSize) { mSize = aSize; }

    uint16_t GetNext(void) const
    {
        return *reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(this) + sizeof(mSize) + mSize);
    }

    void SetNext(uint16_t aNext)
    {
        *reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(this) + sizeof(mSize) + mSize) = aNext;
    }

    void *GetPointer(void) { return &mMemory; }

    uint16_t GetLeftNext(void) const { return *(&mSize - 1); }

    bool IsLeftFree(void) const { return GetLeftNext() != 0; }

    bool IsFree(void) const { return mSize != kGuardBlockSize && GetNext() != 0; }

private:
    enum
    {
        kGuardBlockSize = 0xffff,
    };

    uint16_t mSize;

    uint8_t mMemory[sizeof(uint16_t)];
};

/**
 * This class defines functionality to manipulate heap.
 *
 * This implementation is currently for mbedTLS.
 *
 * The memory is divided into blocks. The whole picture is as follows:
 *
 *     +--------------------------------------------------------------------------+
 *     |    unused      |    super   | block 1 | block 2 | ... | block n | guard  |
 *     +----------------+------------+---------+---------+-----+---------+--------+
 *     | kAlignSize - 2 | kAlignSize | 4 + s1  | 4 + s2  | ... | 4 + s4  |   2    |
 *     +--------------------------------------------------------------------------+
 *
 */
class Heap
{
public:
    Heap(void);

    void *CAlloc(size_t aCount, size_t aSize);

    void Free(void *aPointer);

    bool IsClean(void) const
    {
        Heap &       self  = *const_cast<Heap *>(this);
        const Block &super = self.BlockSuper();
        const Block &first = self.BlockRight(super);
        return super.GetNext() == self.BlockOffset(first) && first.GetSize() == kFirstBlockSize;
    }

    size_t GetCapacity(void) const { return kFirstBlockSize; }

    size_t GetFreeSize(void) const { return mMemory.mFreeSize; }

private:
    enum
    {
        kMemorySize         = GOBO_CONFIG_HEAP_SIZE,
        kAlignSize          = sizeof(long),
        kBlockRemainderSize = kAlignSize - sizeof(uint16_t) * 2,
        kSuperBlockSize     = kAlignSize - sizeof(Block),
        kFirstBlockSize     = kMemorySize - kAlignSize * 3 + kBlockRemainderSize,
        kSuperBlockOffset   = kAlignSize - sizeof(uint16_t),
        kFirstBlockOffset   = kAlignSize * 2 - sizeof(uint16_t),
        kGuardBlockOffset   = kMemorySize - sizeof(uint16_t),
    };

    Block &BlockAt(uint16_t aOffset) { return *reinterpret_cast<Block *>(&mMemory.m16[aOffset / 2]); }

    Block &BlockOf(void *aPointer)
    {
        uint16_t offset = static_cast<uint16_t>(reinterpret_cast<uint8_t *>(aPointer) - mMemory.m8);
        offset -= sizeof(uint16_t);
        return BlockAt(offset);
    }

    Block &BlockSuper(void) { return BlockAt(kSuperBlockOffset); }

    Block &BlockNext(const Block &aBlock) { return BlockAt(aBlock.GetNext()); }

    Block &BlockRight(const Block &aBlock) { return BlockAt(BlockOffset(aBlock) + sizeof(Block) + aBlock.GetSize()); }

    Block &BlockPrev(const Block &aBlock);

    bool IsLeftFree(const Block &aBlock) { return (BlockOffset(aBlock) != kFirstBlockOffset && aBlock.IsLeftFree()); }

    uint16_t BlockOffset(const Block &aBlock)
    {
        return static_cast<uint16_t>(reinterpret_cast<const uint8_t *>(&aBlock) - mMemory.m8);
    }

    void BlockInsert(Block &aPrev, Block &aBlock);

    union
    {
        uint16_t mFreeSize;
        // Make sure memory is long aligned
        long     mLong[kMemorySize / sizeof(long)];
        uint8_t  m8[kMemorySize];
        uint16_t m16[kMemorySize / sizeof(uint16_t)];
    } mMemory;
};

} // namespace Utils
} // namespace go

#endif // GO_HEAP_HPP_
