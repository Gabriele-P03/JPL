#include "Utils.hpp"

jpl::_utils::_collections::_list::ArrayList<std::string>* jpl::_sounds::getDeviceInfo(ALCdevice* dev, int param){
    jpl::_utils::_collections::_list::ArrayList<std::string>* list = new jpl::_utils::_collections::_list::ArrayList<std::string>();
    const char* buffer = alcGetString(dev, param);
    size_t countNull = 0, i = 0;
    char cr;
    std::string bStr = "";
    while(true){
        cr = buffer[i++];
        if(cr == '\0'){
            countNull++;
            list->add(bStr);
            bStr = "";
            if(param == ALC_DEVICE_SPECIFIER || param == ALC_CAPTURE_DEVICE_SPECIFIER){
                if(countNull > 1){
                    break;
                }
            }else if( countNull > 0){
                break;
            }
            continue;
        }
        bStr += cr;
    }
    return list;
}