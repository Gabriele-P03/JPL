/**
 * @file CSVParser.hpp
 * 
 * CSV Parser
 * 
 * You are free to declare char separator which you desire; otherwise, the default constructor will use
 * semi-colon by default (and you will not be able to change it).
 * 
 * Values and columns are stored into a Table instance. By default, the it is set with a full list of not mandatory Column<std::string>; if you are 
 * interested into using a table with others specialized columns, you can add your own.
 * The default table(s) is instanced only when any parse() method is called and tables is empty (1).
 * 
 * This CSV Parser provides multi-table parsing mode (and that's why tables are stored into a LL), it means that is can parse several tables from a single csv file. It can be enabled via
 * setMultiTable(bool). 
 * Such a file has to be follow a certain syntax: 
 *  1) the header is composed by all tables' column declaration (one-per-line)
 *  2) an empty-line must separate data and declaration
 *  3) each set of data must be separated by an empty line
 * 
 * In standard mode, there's no need to leave an empty line 
 * 
 * In multi-table mode, the separator is the same for each of them.
 * In multi-table mode, if file contains more tables' columns declaration than tables list, this parser will not be able to instance the missing ones,
 * throwing exception 
 * 
 * (1): If no table(s) had been added before calling parse(), CSV Parser will instance it(them) as it needs, otherwise it will only check if tables' name
 * are the same as declared into csv header.
 *   
 * 
 * 
 * @author Gabriele-P03
 * @date 27-09-2024
 */

#ifndef CSV_PARSER_JPL
#define CSV_PARSER_JPL

#include <jpl/utils/structure/list/LinkedList.hpp>
#include <tuple>
#include "exception/CSVParsingException.hpp"
#include "exception/CSVColumnNotFoundException.hpp"
#include <jpl/utils/structure/db/TableWrapper.hpp>


namespace jpl{

    namespace _parser{

        namespace _csv{

            
            class CSVParser{

                protected:
                    const char* const separator;
                    _utils::_collections::_list::LinkedList<_utils::_collections::Table*> tables;
                    bool multiTable;

                    void parseHeader(std::string header);

                    /**
                     * @param is 
                     * 
                     * @throw IllegalArgumentException if is nullptr
                     * @throw CSVParsingException if any error during parsing occurs
                     */
                    void parseData(std::istream* is);
                public:

                    /**
                     * Instance a CSVParser with semi-colon as default separator and Table full-of-Column<std::string>
                     * 
                     * @param name Name of the table which will be instanced 
                     */
                    CSVParser();

                    /**
                     * Instance a CSVParser
                     * 
                     * @param separator - char separator
                     * @param table
                     * 
                     * @throw IllegalArgumentException if either separator or table is nullptr
                     */
                    CSVParser(const char* separator);

                    /**
                     * @param table
                     * 
                     * @throw IllegalArgumentException if table is nullptr
                     * @throw RuntimeException if either there's already a table called as the given one or the same pointed
                     */
                    void addTable();//_utils::_collections::Table* table);

                    void parse(std::istream* ifs);

                    bool isMultiTable() const noexcept{return this->multiTable;}
                    void setMultiTable(bool multiTable) noexcept{this->multiTable = multiTable;}

                    const char* const getSeparator() const noexcept {return this->separator;}
            };
        }
    }
}

#endif