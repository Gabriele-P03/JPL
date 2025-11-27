#include "src/BufferManager.hpp"
#include "src/ContextManager.hpp"
#include "src/SourceBufferWrapper.hpp"
#include "src/Utils.hpp"

#include <jpl/parser/wav/WAV.hpp>

using namespace jpl;
using namespace _sounds;
using namespace _parser;
using namespace _parser;

int main(){
    alGetError();
    ContextManager cm = ContextManager(openDevice(NULL), NULL);
    alcMakeContextCurrent(cm.getContext());
    BufferManager bm = BufferManager();

    getDeviceInfo(NULL, ALC_DEVICE_SPECIFIER);
    std::fstream* f = new std::fstream;
    f->open("C:\\Users\\Utente\\JPL\\sounds\\out\\windows\\wav_test.wav", std::ios_base::in | std::ios_base::binary);
    _wav::WAV* wav = _wav::parse(f);

    Source* s = cm.genSource("test");
    Buffer* b = bm.genBuffer("test");


    alBufferData(b->getBuffer(), AL_FORMAT_STEREO16, wav->data.data, wav->data.subchunk2Size, wav->fmt.sampleRate);
    _errors::checkForError();
    attachBuffer(s,b);
    _errors::checkForError();
    alSourcePlay(s->getSource());
    _errors::checkForError();

    while(true){
    }

    //alBufferData(b->getBuffer(), );
}