#include "TableWrapper.hpp"

/*********************************************************
 *  STRING
 ********************************************************/


std::string jpl::_utils::_collections::TableWrapper::getStringValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<std::string>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a string column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<std::string>*)col)->getValue(tuple);
}
std::string jpl::_utils::_collections::TableWrapper::getStringValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<std::string>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a string column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<std::string>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setStringValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, std::string value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<std::string>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a string column");
    if(col->isMandatory() && value.empty())
        throw jpl::_exception::IllegalArgumentException("Column " + colName + " does not allow empty string");    
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<std::string>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setStringValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, std::string value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<std::string>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a string column");
    if(col->isMandatory() && value.empty())
        throw jpl::_exception::IllegalArgumentException("Column " + col->getName() + " does not allow empty string");    
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<std::string>*)col)->setValue(tuple, value);
}



/*********************************************************
 *  BOOLEAN
 ********************************************************/

bool jpl::_utils::_collections::TableWrapper::getBoolValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<bool>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a boolean column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<bool>*)col)->getValue(tuple);
}
bool jpl::_utils::_collections::TableWrapper::getBoolValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<bool>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a boolean column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<bool>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setBoolValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, bool value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<bool>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a boolean column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<bool>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setBoolValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, bool value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<bool>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a boolean column"); 
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<bool>*)col)->setValue(tuple, value);
}



/*********************************************************
 *  DOUBLE
 ********************************************************/

double jpl::_utils::_collections::TableWrapper::getDoubleValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<double>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a boolean column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<double>*)col)->getValue(tuple);
}
double jpl::_utils::_collections::TableWrapper::getDoubleValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<double>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a boolean column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<double>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setDoubleValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, double value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<double>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a double column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<double>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setDoubleValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, double value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<double>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a double column"); 
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<double>*)col)->setValue(tuple, value);
}


/*********************************************************
 *  LONG
 ********************************************************/

long jpl::_utils::_collections::TableWrapper::getLongValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<long>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a long column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<long>*)col)->getValue(tuple);
}
long jpl::_utils::_collections::TableWrapper::getLongValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<long>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a long column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<long>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setLongValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, long value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<long>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a long column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<long>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setLongValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, long value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<long>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a long column"); 
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<long>*)col)->setValue(tuple, value);
}



/*********************************************************
 *  INT
 ********************************************************/

int jpl::_utils::_collections::TableWrapper::getIntValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<int>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not an integer column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<int>*)col)->getValue(tuple);
}
int jpl::_utils::_collections::TableWrapper::getIntValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<int>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not an integer column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<int>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setIntValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, int value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<int>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not an integer column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<int>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setIntValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, int value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<int>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not an integer column"); 
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<int>*)col)->setValue(tuple, value);
}


/*********************************************************
 *  CHAR*
 ********************************************************/

const char* jpl::_utils::_collections::TableWrapper::getCharsValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<const char*>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a const pointer char column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<const char*>*)col)->getValue(tuple);
}
const char* jpl::_utils::_collections::TableWrapper::getCharsValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<const char*>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a const pointer char column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    return ((jpl::_utils::_collections::Column<const char*>*)col)->getValue(tuple);
}

void jpl::_utils::_collections::TableWrapper::setCharsValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, const char* value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    if(!dynamic_cast<Column<const char*>*>(col))
        throw jpl::_exception::RuntimeException("Column " + colName + " is not a const pointer char column");
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<const char*>*)col)->setValue(tuple, value);
}
void jpl::_utils::_collections::TableWrapper::setCharsValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, const char* value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    if(!dynamic_cast<Column<const char*>*>(col))
        throw jpl::_exception::RuntimeException("Column " + col->getName() + " is not a const pointer char column"); 
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    ((jpl::_utils::_collections::Column<const char*>*)col)->setValue(tuple, value);
}


/**
 * SMART
 */

void jpl::_utils::_collections::TableWrapper::setSmartValue(jpl::_utils::_collections::Table* table, size_t iTuple, size_t iCol, std::string value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(iCol);
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    if(dynamic_cast<Column<std::string>*>(col))
        ((jpl::_utils::_collections::Column<std::string>*)col)->setValue(tuple, value);
    else if(dynamic_cast<Column<bool>*>(col))
        ((jpl::_utils::_collections::Column<bool>*)col)->setValue(tuple, jpl::_utils::_conversions::str_bool(value));
    else if(dynamic_cast<Column<double>*>(col))
        ((jpl::_utils::_collections::Column<double>*)col)->setValue(tuple, std::atof(value.c_str()));
    else if(dynamic_cast<Column<long>*>(col))
        ((jpl::_utils::_collections::Column<long>*)col)->setValue(tuple, std::atol(value.c_str()));
    else if(dynamic_cast<Column<int>*>(col))
        ((jpl::_utils::_collections::Column<int>*)col)->setValue(tuple, std::atoi(value.c_str()));
    else if(dynamic_cast<Column<const char*>*>(col)){
        const char *buf = new char[value.size()];
        char* tmp = (char*)&buf[0];
        memcpy(tmp, value.c_str(), value.size()); 
        ((jpl::_utils::_collections::Column<const char*>*)col)->setValue(tuple, buf);
    }
    else
        throw jpl::_exception::RuntimeException("No dynamic cast for column was valid");    
}
void jpl::_utils::_collections::TableWrapper::setSmartValue(jpl::_utils::_collections::Table* table, size_t iTuple, std::string colName, std::string value){
    if(table == nullptr)
        throw jpl::_exception::IllegalArgumentException("Table instance is nullptr");
    if(!table->isEditableTuple())
        throw jpl::_exception::IllegalStateException("Table " + table->getName() + " does not allow tuple-editing");
    AbstractColumn* col = table->getColumn(colName);
    jpl::_utils::_collections::Tuple* tuple = table->tuples->get(iTuple);
    if(dynamic_cast<Column<std::string>*>(col))
        ((jpl::_utils::_collections::Column<std::string>*)col)->setValue(tuple, value);
    else if(dynamic_cast<Column<bool>*>(col))
        ((jpl::_utils::_collections::Column<bool>*)col)->setValue(tuple, jpl::_utils::_conversions::str_bool(value));
    else if(dynamic_cast<Column<double>*>(col))
        ((jpl::_utils::_collections::Column<double>*)col)->setValue(tuple, std::atof(value.c_str()));
    else if(dynamic_cast<Column<long>*>(col))
        ((jpl::_utils::_collections::Column<long>*)col)->setValue(tuple, std::atol(value.c_str()));
    else if(dynamic_cast<Column<int>*>(col))
        ((jpl::_utils::_collections::Column<int>*)col)->setValue(tuple, std::atoi(value.c_str()));
    else if(dynamic_cast<Column<const char*>*>(col)){
        const char *buf = new char[value.size()];
        char* tmp = (char*)&buf[0];
        memcpy(tmp, value.c_str(), value.size()); 
        ((jpl::_utils::_collections::Column<const char*>*)col)->setValue(tuple, buf);
    }else
        throw jpl::_exception::RuntimeException("No dynamic cast for column was valid");    
}