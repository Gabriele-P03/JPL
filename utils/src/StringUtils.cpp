#include "StringUtils.hpp"

jpl::_utils::_collections::_list::LinkedList<std::string>* jpl::_utils::_string::split(std::string src, std::regex regex){
    jpl::_utils::_collections::_list::LinkedList<std::string>* vec = new jpl::_utils::_collections::_list::LinkedList<std::string>();
    auto word_begin = std::sregex_token_iterator(src.begin(), src.end(), regex, -1);
    auto word_end = std::sregex_token_iterator();
    for(; word_begin != word_end; ++word_begin){
        std::string match_string = *word_begin;
        vec->add(match_string);
    }
    return vec;
}


size_t jpl::_utils::_string::getIndexGroupOver(std::string src, std::regex o, std::regex c){
    size_t i = 0, count = 0;
    do{
        jpl::_utils::_string::RegexMatch ms = jpl::_utils::_string::findFirstOf(src, o);
        jpl::_utils::_string::RegexMatch me = jpl::_utils::_string::findFirstOf(src, c);
        if(me.pos == src.size())
            break;
        if(ms.pos == src.size() || ms.pos > me.pos){
            count += me.pos + me.length;
            src = src.substr(me.pos + me.length);
            i--;
            if(i == 0) break;
        }else if(ms.pos < me.pos){
            count += ms.pos + ms.length;
            src = src.substr(ms.pos + ms.length);
            i++;
        }else
            return ms.pos;
    }while(i > 0);
    if(i > 0)
        throw new jpl::_exception::RuntimeException("Amount of opening chars do not correspond to closing one");
    return count-1;
}

jpl::_utils::_collections::_list::LinkedList<std::string>* jpl::_utils::_string::splitBalanced(const std::string &src, const std::regex &o, const std::regex &c, const std::regex &s){
    jpl::_utils::_collections::_list::LinkedList<std::string>* list = new jpl::_utils::_collections::_list::LinkedList<std::string>();
    std::string buffer = src;
    size_t opened = 0;  //It stores how many times a sub-group has been opened without has been closed
    while(!buffer.empty()){
        jpl::_utils::_string::RegexMatch oirm = jpl::_utils::_string::findFirstOf(buffer, o);
        jpl::_utils::_string::RegexMatch sirm = jpl::_utils::_string::findFirstOf(buffer, s);
        jpl::_utils::_string::RegexMatch cirm = jpl::_utils::_string::findFirstOf(buffer, c);

        size_t oi = oirm.pos;
        size_t si = sirm.pos;
        size_t ci = cirm.pos;

        bool si_valid = (si != buffer.size());
        bool oi_valid = (oi != buffer.size());
        bool ci_valid = (ci != buffer.size());

        if(si_valid && si < oi){    //First occurrence before new sub-group
            std::string occ = jpl::_utils::_string::trim(buffer.substr(0, si));
            list->add(occ);
            buffer.erase(0, si+1);
        }else{
            if(oi_valid ^ ci_valid){
                throw jpl::_exception::RuntimeException("Invalid balanced string: " + buffer);
            }
            if(oi_valid){
                opened++;
            }

            //Let's check whereas there's another opening bracket before closing one

            if(ci_valid){
                opened--;
            }
        }
    }
    return list;
}