/**
 * A simple POJO test class for ModelWrapper
 */
#ifndef POJO_TEST_FK_CLASS_DB_JPL
#define POJO_TEST_FK_CLASS_DB_JPL

#include "ModelWrapper.hpp"

namespace jpl{
    namespace _db{

        class POJOTestFK : public ModelWrapper{

            public:
                int a;
                unsigned int idPOJOTest;

                POJOTest(){
                    ModelWrapper::registerField(DBType("x", "X", false, &x));
                    ModelWrapper::registerField(DBType("y", "Y", false, &y));
                    ModelWrapper::registerField(DBType("z", "Z", false, &z));
                }
        };
    }
}

#endif