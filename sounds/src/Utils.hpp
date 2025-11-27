/**
 * Utility functions to retrieve OpenAL's occurred errors
 */
#ifndef OPENAL_UTILS_JPL
#define OPENAL_UTILS_JPL

#include "ErrorUtils.hpp"
#include <al/alc.h>
#include <jpl/utils/structure/list/ArrayList.hpp>

namespace jpl{
    namespace _sounds{

        inline ALCdevice* openDevice(const char* deviceName){
            ALCdevice* dev = alcOpenDevice(deviceName);
            if(dev == nullptr){
                throw jpl::_exception::RuntimeException("Device could not be retrieved");
            }
            return dev;
        }

        /**
         * Capabilities of this function are the same as the alcGetString ones
         * @param dev
         * @param param
         * @return information param about given dev
         */
        extern _utils::_collections::_list::ArrayList<std::string>* getDeviceInfo(ALCdevice* dev, int param);
    }
}

#endif