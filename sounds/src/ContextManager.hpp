/**
 * As described into OpenAL documentation, sounds' data are stored into buffers.
 * A buffer is always attached at least to one source (buffers may be shared among sources)
 * 
 * A set of sources form a context; buffers are not part of a context!!!
 */
#ifndef SOURCEMANAGER_JPL
#define SOURCEMANAGER_JPL

#include "Source.hpp"
#include <al/alc.h>
#include <unordered_map>

namespace jpl{
    namespace _sounds{

        class ContextManager{

            private:
                ALCdevice* device;
                ALCcontext* context;
                std::unordered_map<std::string, Source*> sources;

            public:
                ContextManager(ALCdevice* device, ALCint* attrlist);

                void addSource(Source* source);

                /**
                 * Generate new source
                 * @param regName
                 * @throw IllegalArgumentException if regName is empty string
                 * @throw IllegalArgumentException if a source with the same regName has been already registered 
                 */
                Source* genSource(const std::string &regName);

                /**
                 * @param regName
                 * @return Source with the given regName or nullptr if not found
                 */
                Source* getSourceByRegName(const std::string &regName) const noexcept;

                ALCdevice* getDevice() const noexcept{
                    return this->device;
                }
                ALCcontext* getContext() const noexcept{
                    return this->context;
                }

                ~ContextManager();
        };
    }
}

#endif