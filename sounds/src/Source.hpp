/**
 * A source is the container of a set of buffer.
 * Remember that, as described into OpenAL doc, a buffer can be attached to several sources.
 * 
 * A source is represented by a unique regName
 */
#ifndef SOUNDSOURCE_JPL
#define SOUNDSOURCE_JPL

#include "ErrorUtils.hpp"
#include <jpl/utils/structure/list/ArrayList.hpp>

namespace jpl{
    namespace _sounds{

        class Buffer;

        class Source{

            friend void attachBuffer(Source* s, Buffer* b);

            private:
                const std::string regName;
                unsigned int source;
                _utils::_collections::_list::ArrayList<Buffer*> buffers;

            public:
                Source(const std::string &regName) : regName(regName){
                    alGenSources(1, &this->source);
                    _errors::checkForError();
                }

                unsigned int getSource() const noexcept{
                    return this->source;
                }

                std::string getRegName() const noexcept{
                    return this->regName;
                }
        };
    }
}

#endif