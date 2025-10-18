#include "HTMLCommon.hpp"

bool jpl::_parser::_common::removeHTMLComment(std::string &line, bool flagComment) noexcept{
    size_t s = line.find("<!--");
    bool another_comment = false;
    if (s != std::string::npos){
        size_t s1 = line.find("<!--", (s+sizeof("<!--")));
        bool s1_npos = s1 != std::string::npos;
        if(flagComment || s1_npos){
            if(s1_npos){
                another_comment = true;
            }
            if(line.find("-->") > s1){
                throw jpl::_parser::_dtd::_exception::DTDException("It seems like there's an innested comment");
            }
        }
        flagComment = true;
    }else{
        s = 0;
    }
    size_t e = line.find("-->");
    if(e != std::string::npos){
        if(!flagComment){
            throw jpl::_parser::_dtd::_exception::DTDException("It seems like an end-tag does not close any comment");
        }
        if(e<s){
            s = 0;  //It means that a new comment starts on the same line where end tag of the previous comment is located at
            another_comment = true;
        }
        flagComment = false;
        e += sizeof("-->")-1;
    }else{
        if(flagComment){
            e = line.size();    //Previous call of this function did not found end-tag of comment
        }else{
            e = 0;
        }
    }
    line = jpl::_utils::_string::trim(line.erase(s,e-s));
    if(another_comment){
        flagComment = jpl::_parser::_common::removeHTMLComment(line, false);
    }
    return flagComment;
}