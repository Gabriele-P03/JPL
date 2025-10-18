#include "src/StringUtils.hpp"
#include <iostream>
int main(){

    jpl::_utils::_collections::_list::LinkedList<std::string> * list = jpl::_utils::_string::splitBalanced(
        "x*,p,(a+,j?,g*),(q|w|e)",
        std::regex("\\("),
        std::regex("\\)"),
        std::regex("[,|\\|]")
    );

    for(size_t i = 0; i < list->getSize(); i++){
        std::cout<<list->get(i)<<std::endl;
    }
}