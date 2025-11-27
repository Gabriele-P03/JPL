/**
 * BufferManager provides the main list of buffers and manages 'em internally
 */
#ifndef SOUNDBUFFER_MANAGER_JPL
#define SOUNDBUFFER_MANAGER_JPL

#include <unordered_map>
#include "Buffer.hpp"

namespace jpl{
    namespace _sounds{

        class BufferManager{

            private:
                std::unordered_map<std::string, Buffer*> buffers;

            public:

                BufferManager();

                /**
                 * Add new buffer
                 * @param buffer
                 * @throw IllegalArgumentException if buffer is nullptr
                 */
                void addBuffer(Buffer* buffer);

                /**
                 * Generate new buffer and insert into buffers
                 * @param regName 
                 * @throw IllegalArgumentException if regName is empty string
                 * @throw IllegalArgumentException if there's already a buffer registered with the same regName
                 * @return new buffer
                 */
                Buffer* genBuffer(const std::string &regName);

                /**
                 * @param regName
                 * @return buffer registered with regName or nullptr if not found
                 */
                Buffer* getBufferByRegName(const std::string &regName) const noexcept;
        };
    }
}

#endif