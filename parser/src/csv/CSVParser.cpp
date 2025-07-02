#include "CSVParser.hpp"

jpl::_parser::_csv::CSVParser::CSVParser() : jpl::_parser::_csv::CSVParser(";") {}
jpl::_parser::_csv::CSVParser::CSVParser(const char* separator) : separator(separator){
    if(separator == nullptr)
        throw new jpl::_exception::IllegalArgumentException("Char separator is nullptr");
    this->multiTable = false;
}


void jpl::_parser::_csv::CSVParser::addTable(){//jpl::_utils::_collections::Table* table){
    /*if(table == nullptr)
        throw new jpl::_exception::IllegalArgumentException("You cannot add a nullptr");*/
    //if(!this->multiTable && !this->tables.isEmpty())
        throw new jpl::_exception::IllegalStateException("This CSV Parser already contains a table instance. If you wanna insert another one, you have to enable multi-table mode before");
    /*for(size_t i = 0; i < this->tables.getSize(); i++){
        jpl::_utils::_collections::Table* cr = this->tables.get(i);
        if(cr == table)
            throw new jpl::_exception::RuntimeException("This table has been already inserted");
        if(cr->getName().compare(table->getName()) == 0)
            throw new jpl::_exception::RuntimeException("There's already a table named as " + table->getName());
    }
    this->tables.add(table);
    */
}


void jpl::_parser::_csv::CSVParser::parse(std::istream* is){
    std::string buffer;
    std::string line;
    while(std::getline(*is, line)){
        if(line.empty()){
            break;  //In multi table mode, the empty line marks separation between header and data
        }else{
            buffer += line + "\n";
        }
        if(!this->multiTable){
            break;  //In standard mode, there could not be any separation-empty-line
        }
    }
    this->parseHeader(buffer);
    this->parseData(is);
}


void jpl::_parser::_csv::CSVParser::parseHeader(std::string header){
    size_t i = 0, j;    //i -> table; j -> column
    bool check = !this->tables.isEmpty();   // (1)
    std::istringstream iss = std::istringstream(header);
    std::string tableDecl;
    size_t endSingleCol;
    while(std::getline(iss, tableDecl)){
        if(tableDecl.empty())
            continue;
        j = 0;
        jpl::_utils::_collections::Table* table;
        if(check){
            if(this->tables.getSize() <= i){    //Check if there are at least i-1 tables
                throw new jpl::_exception::RuntimeException("There are no enough tables into list");
            }
            table = this->tables.get(i++);
        }else{
            table = new jpl::_utils::_collections::Table(std::to_string(i++), true, true);    //Instance i-th table 
        }
        while ( (endSingleCol = tableDecl.find(this->separator) ) != std::string::npos){   //Each column
            std::string colName = tableDecl.substr(0, endSingleCol);
            if(colName.empty()){
                throw new jpl::_parser::_csv::_exception::CSVParsingException("There's an empty column name for the " + std::to_string(i) + " table");
            }
            tableDecl.erase(0, endSingleCol+1);
            if(check){
                if(table->getColumnsSize() <= j){
                    throw new jpl::_exception::RuntimeException("Table " + table->getName() + " has only " + std::to_string(table->getColumnsSize()) + " columns");
                }
                if(table->getColumn(j)->getName().compare(colName) != 0){
                    throw new jpl::_parser::_csv::_exception::CSVParsingException("Table " + table->getName() + " does not contain " + colName + " column");
                }
            }else{
                table->addColumn(new jpl::_utils::_collections::Column<std::string>(colName, false));
            }
            j++;
        }
        if(!check){
            this->tables.add(table);
        }
        if(i == 0 && !this->multiTable){
            break;
        }
    }
}

void jpl::_parser::_csv::CSVParser::parseData(std::istream* is){
    if(is == nullptr){
        throw new jpl::_exception::IllegalArgumentException("input stream passed is nullptr");
    }
    std::string line;
    size_t i = 0;
    jpl::_utils::_collections::Table* table = this->tables.get(i);
    while(std::getline(*is, line)){
        if(this->multiTable){
            if(line.empty())
                i = 0;
            else{
                if(i+1 > this->tables.getSize())
                    throw new jpl::_parser::_csv::_exception::CSVParsingException("There's a set of data of " + std::to_string(i+1) + " rows. Max is " + std::to_string(this->tables.getSize()));
                table = this->tables.get(i++);
            }
        }
        size_t endValue;
        jpl::_utils::_collections::_list::ArrayList<std::string>* values = new jpl::_utils::_collections::_list::ArrayList<std::string>(table->getColumnsSize());
        jpl::_utils::_collections::Tuple* tuple = new jpl::_utils::_collections::Tuple(table->getTuplesSize(), values);
        table->addTuple(tuple);
        size_t j = 0;
        bool flag = true;
        do{
            endValue = line.find(this->separator);
            if(endValue == std::string::npos){
                flag = false;
                endValue = line.size();
            }else{
                line.erase(0, endValue+1);
            }
            std::string value = line.substr(0, endValue);
            if(value.empty()){//Empty value, it checks if column is mandatory
                jpl::_utils::_collections::AbstractColumn* col = table->getColumn(j);
                if(col->isMandatory()){
                    throw new jpl::_parser::_csv::_exception::CSVParsingException(col->getName() + " column is mandatory");
                }
            }
            jpl::_utils::_collections::TableWrapper::setSmartValue(table, table->getTuplesSize()-1, j++, value);
        }while(flag);
        if(j != table->getColumnsSize()){   //Once line is completely parsed, it checks if there are any missing mandatory columns
            for(size_t iMissCol = j; iMissCol < table->getColumnsSize(); iMissCol++ ){
                jpl::_utils::_collections::AbstractColumn* col = table->getColumn(iMissCol);
                if(col->isMandatory()){
                    throw new jpl::_parser::_csv::_exception::CSVParsingException(col->getName() + " column is mandatory");
                }
            }
        }
    }
}