/**
 * Since there's a mutual inclusion between Source.hpp and Buffer.hpp, a wrapper is needed in order
 * to perform attachment between them 
 */
#ifndef SOURCE_BUFFER_WRAPPER_JPL
#define SOURCE_BUFFER_WRAPPER_JPL

#include "Source.hpp"
#include "Buffer.hpp"

namespace jpl{
    namespace _sounds{

        /**
         * Attach buffer to source
         * @param s
         * @param b
         * @throw IllegalArgumentException if either s or b are nullptr
         * @throw RuntimeException if s already contains b or any buffer with the same b's regName
         * @throw RuntimeException if b has been already attached to s or to any source with the same s' regName
         */
        extern void attachBuffer(Source* s, Buffer* b);

        /**
         * Delete buffer after have detched from all sources. 
         * According to OpenAL's doc, calling alSourcei(source, AL_BUFFER, 0), all buffers attached to source will be detached
         * @param b
         * @throw IllegalArgumentException if b is nullptr
         */
        extern void deleteBuffer(Buffer* b);
        /**
         * Delete source after have detched all buffers from it
         * @param b
         * @throw IllegalArgumentException if s is nullptr
         */
        extern void deleteSource(Source* s);
    }
}

#endif