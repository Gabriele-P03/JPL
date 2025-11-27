/**
 * A buffer stores raw sound data.
 * Remember that, as described into OpenAL doc, a buffer can be attached to several sources.
 * 
 * A buffer is represented by a unique regName
 */
#ifndef SOUNDBUFFER_JPL
#define SOUNDBUFFER_JPL

#include "ErrorUtils.hpp"
#include <jpl/utils/structure/list/ArrayList.hpp>

namespace jpl{
    namespace _sounds{

        class Source;

        class Buffer{

            friend void attachBuffer(Source* s, Buffer* b);
            friend void deleteBuffer(Buffer* b);

            private:
                const std::string regName;
                unsigned int buffer;
                _utils::_collections::_list::ArrayList<Source*> sources;

            public:
                Buffer(const std::string &regName) : regName(regName){
                    alGenBuffers(1, &this->buffer);
                    _errors::checkForError();
                }

                unsigned int getBuffer() const noexcept{
                    return this->buffer;
                }

                std::string getRegName() const noexcept{
                    return this->regName;
                }
        };
    }
}

#endif