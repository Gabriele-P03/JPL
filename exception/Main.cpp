#include "Main.hpp"

void f();

int main(){

    try{
        f();
    }catch(jpl::_exception::RuntimeException &ex){
        f();
    }

}

void f(){
    throw jpl::_exception::RuntimeException("Errore ciao");
}