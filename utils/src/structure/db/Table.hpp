/**
 * @file
 * This implementation of Table is able to manage a dynamic run-time indexing in order to 
 * give you a certain freedom of usage such as adding/removing columns or managing tuples
 * 
 * A tuple does not know anything about data-type of its elements, it is the column implementation that
 * tell you about it; therefore an element of a tuple can be retrieved either as std::string directly from the
 * tuple or as its type from Table which contains the tuple via Column getValue() method.
 * 
 * @author Gabriele-P03
 */

#ifndef TABLE_JPL
#define TABLE_JPL

#include <jpl/logger/Logger.hpp>
#include "../list/ArrayList.hpp"
#include "columns/AbstractColumn.hpp"
#include "../../functional/predicate/BiPredicate.hpp"
#include <jpl/exception/runtime/IllegalStateException.hpp>
#include "Tuple.hpp"

namespace jpl{
    namespace _utils{
        namespace _collections{

            class TableWrapper;

            class Table{

                protected:
                    const std::string name;
                    _list::ArrayList<AbstractColumn*>* columns;
                    _list::ArrayList<Tuple*>* tuples;
                    const bool editableTuple;
                    const bool editableColumn; 

                    /**
                     * Check if tuple's values list is right-sized. If it is empty, it reallocates it and set
                     * each of N value as empty string
                     */
                    virtual void checkSizeTuple(Tuple* tuple) const;

                public:

                    friend class TableWrapper;

                    Table(std::string name, bool editableTuple, bool editableColumn);
                    Table(std::string name, bool editableTuple, bool editableColumn, _list::ArrayList<AbstractColumn*>* columns);
                    Table(std::string name, bool editableTuple, bool editableColumn, _list::ArrayList<AbstractColumn*>* columns, _list::ArrayList<Tuple*>* tuples);

                    void addColumn(AbstractColumn* column);
                    void addColumn(size_t index, AbstractColumn* column);
                    AbstractColumn* getColumn(std::string name);
                    AbstractColumn* getColumn(size_t index);
                    bool hasColumn(std::string name) const noexcept;
                    void removeColumn(AbstractColumn* column);                                                                   
                    void removeColumn(size_t index);

                    void addTuple(Tuple* tuple);
                    void addTuples(_list::ArrayList<Tuple*> tuples);
                    void addTuple(size_t index, Tuple* tuple);
                    void addTuples(size_t index, _list::ArrayList<Tuple*> tuples);
                    void removeTuple(Tuple* tuple);
                    void removeTuple(size_t index);
                    void removeTuples(_list::ArrayList<Tuple*> tuples);
                    void removeTuplesIf(_functional::Predicate<Tuple*> predicate);


                    std::string getName() const noexcept{return this->name;}
                    bool isEditableTuple() const noexcept{return this->editableTuple;}
                    bool isEditableColumn() const noexcept{return this->editableColumn;}
                    size_t getColumnsSize() const noexcept{return this->columns->getSize();}
                    size_t getTuplesSize() const noexcept{return this->tuples->getSize();}

                    ~Table(){
                        delete this->columns;
                        delete this->tuples;
                    }
            };
        }
    }
}

#endif
