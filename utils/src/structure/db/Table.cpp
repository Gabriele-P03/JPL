#include "Table.hpp"

jpl::_utils::_collections::Table::Table(std::string name, bool editableTuple, bool editableColumn) : jpl::_utils::_collections::Table(name, editableTuple, editableColumn, new jpl::_utils::_collections::_list::ArrayList<jpl::_utils::_collections::AbstractColumn*>()){}
jpl::_utils::_collections::Table::Table(std::string name, bool editableTuple, bool editableColumn, jpl::_utils::_collections::_list::ArrayList<jpl::_utils::_collections::AbstractColumn*>* columns) : jpl::_utils::_collections::Table(name, editableTuple, editableColumn, columns, new jpl::_utils::_collections::_list::ArrayList<jpl::_utils::_collections::Tuple*>()){}
jpl::_utils::_collections::Table::Table(std::string name, bool editableTuple, bool editableColumn, jpl::_utils::_collections::_list::ArrayList<jpl::_utils::_collections::AbstractColumn*>* columns, jpl::_utils::_collections::_list::ArrayList<jpl::_utils::_collections::Tuple*>* tuples)
    : name(name), editableTuple(editableTuple), editableColumn(editableColumn){
    this->columns = columns;
    this->tuples = tuples;
    for(size_t i = 0; i < this->columns->getSize(); i++)
        columns->get(i)->setIndex(i+1);
}

/**
 * COLUMNS
 */

void jpl::_utils::_collections::Table::addColumn(jpl::_utils::_collections::AbstractColumn* column){
    this->addColumn(this->columns->getSize(), column);
}
void jpl::_utils::_collections::Table::addColumn(std::size_t index, jpl::_utils::_collections::AbstractColumn* column){
    if(!this->editableColumn)
      throw jpl::_exception::IllegalStateException("This table does not allow columns-editing");
    if(column == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as column");
    if(index > this->columns->getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->columns->getSize(), index);
    for(size_t i = 0; i < this->columns->getSize(); i++){
        jpl::_utils::_collections::AbstractColumn* col = this->columns->get(i);
        if(col->getName().compare(column->getName()) == 0){
            throw jpl::_exception::RuntimeException("There's already a column names as " + column->getName() + " in " + this->name + " table");
        }
    }    
    this->columns->add(index, column);
    for(size_t i = index; i < this->columns->getSize(); i++)
        this->columns->get(i)->setIndex(i+1);
}

jpl::_utils::_collections::AbstractColumn* jpl::_utils::_collections::Table::getColumn(std::string name){
    for(size_t i = 0; i < this->columns->getSize(); i++){
        jpl::_utils::_collections::AbstractColumn* cr = this->columns->get(i);
        if(cr->getName().compare(name) == 0)
            return cr;
    }
    throw jpl::_exception::NotFoundException("Column " + name + " not found in the table " + this->name);
}
jpl::_utils::_collections::AbstractColumn* jpl::_utils::_collections::Table::getColumn(size_t index){
    if(index >= this->columns->getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->columns->getSize(), index);
    return this->columns->get(index);
}

bool jpl::_utils::_collections::Table::hasColumn(std::string name) const noexcept{
    for(size_t i = 0; i < this->columns->getSize(); i++){
        if(this->columns->get(i)->getName().compare(name) == 0)
            return true;
    }
    return false;
}

void jpl::_utils::_collections::Table::removeColumn(jpl::_utils::_collections::AbstractColumn* column){
    if(!this->editableColumn)
      throw jpl::_exception::IllegalStateException("This table does not allow columns-editing");
    if(column == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as column");
    if(column->isMandatory())
        throw jpl::_exception::IllegalStateException("Column " + column->getName() + " is mandatory and it is cannot be removed");
    if(!this->columns->remove(column))
        throw jpl::_exception::NotFoundException("Column " + column->getName() + " has not been found in table " + this->name);    
    for(size_t i = 0; i < this->columns->getSize(); i++)
        this->columns->get(i)->setIndex(i+1);
}
void jpl::_utils::_collections::Table::removeColumn(size_t index){
    if(!this->editableColumn)
      throw jpl::_exception::IllegalStateException("This table does not allow columns-editing");
    if(index >= this->columns->getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->columns->getSize(), index);
    jpl::_utils::_collections::AbstractColumn* cr = this->columns->get(index);
    if(cr->isMandatory())
        throw jpl::_exception::IllegalStateException("Column " + cr->getName() + " is mandatory and it is cannot be removed");
    this->columns->removeAt(index); 
    for(size_t i = index; i < this->columns->getSize(); i++)
        this->columns->get(i)->setIndex(i+1);
}


/**
 * TUPLES
 */
void jpl::_utils::_collections::Table::addTuple(jpl::_utils::_collections::Tuple* tuple){
    this->addTuple(this->tuples->getSize(), tuple);
}
void jpl::_utils::_collections::Table::addTuple(std::size_t index, jpl::_utils::_collections::Tuple* tuple){
    if(!this->editableTuple)
      throw jpl::_exception::IllegalStateException("This table does not allow tuple-editing");
    if(tuple == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as tuple");
    if(index > this->tuples->getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->tuples->getSize(), index);
    this->checkSizeTuple(tuple);    
    this->tuples->add(index, tuple);  
}
void jpl::_utils::_collections::Table::addTuples(jpl::_utils::_collections::_list::ArrayList<Tuple*> tuples){
    this->addTuples(this->tuples->getSize(), tuples);
}
void jpl::_utils::_collections::Table::addTuples(size_t index, jpl::_utils::_collections::_list::ArrayList<Tuple*> tuples){
    if(index > this->tuples->getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->tuples->getSize(), index);
    for(size_t i = 0; i < tuples.getSize(); i++){
        Tuple* tuple = tuples.get(i);
        if(tuple == nullptr)
            throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as tuple. Index: " + i);
        this->checkSizeTuple(tuple);
    }
    this->tuples->addAll(index, &tuples);
}
void jpl::_utils::_collections::Table::removeTuple(jpl::_utils::_collections::Tuple* tuple){
    if(!this->editableTuple)
        throw jpl::_exception::IllegalStateException("The table " + this->name + " does not allow to edit its tuples");
    if(tuple == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add a nullptr as tuple");
    if(!this->tuples->remove(tuple))
        throw jpl::_exception::NotFoundException("This table does not contains the given tuple"); 
}
void jpl::_utils::_collections::Table::removeTuple(size_t index){
    if(!this->editableTuple)
        throw jpl::_exception::IllegalStateException("The table " + this->name + " does not allow to edit its tuples");
    this->tuples->removeAt(index);
}
void jpl::_utils::_collections::Table::removeTuples(jpl::_utils::_collections::_list::ArrayList<Tuple*> tuples){
    if(!this->editableTuple)
        throw jpl::_exception::IllegalStateException("The table " + this->name + " does not allow to edit its tuples");
    for(size_t i = 0; i < tuples.getSize(); i++){
        if(tuples.get(i) == nullptr)
            throw jpl::_exception::IllegalArgumentException("You cannot remove nullptr as tuple. Index: " + i);
    }
    this->tuples->removeAll(&tuples);
}
void jpl::_utils::_collections::Table::removeTuplesIf(jpl::_utils::_functional::Predicate<jpl::_utils::_collections::Tuple*> predicate){
    if(!this->editableTuple)
        throw jpl::_exception::IllegalStateException("The table " + this->name + " does not allow to edit its tuples");
    this->tuples->removeAllIf(predicate);
}

void jpl::_utils::_collections::Table::checkSizeTuple(Tuple* tuple) const {
    size_t max = this->columns->getSize();
    if(tuple->values->getMax() == 0){
        tuple->values->reallocate(max);
    }else{
        if(tuple->values->getMax() != max)
            throw jpl::_exception::RuntimeException("A tuple must have " + std::to_string(max) + " cell in order to be inserted into " + this->getName() + " table");
    }
}