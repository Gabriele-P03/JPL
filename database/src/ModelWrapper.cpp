#include "ModelWrapper.hpp"

void jpl::_db::ModelWrapper::registerField(DBType field){
    this->fields.insert({field.getPojoName(), field});
}