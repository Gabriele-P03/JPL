/**
 * @file
 * 
 * All list implementation - unlike Set, or other such implementations - allow you to insert duplicate elements
 * and null both; otherwise a RuntimeException or a NullPointerException is thrown
 * 
 * In all duplicated or nullable implementations, elements are managed via std::shared_ptr, therefore, in case of
 * pointer type, you have never to destory them by yourself. Those one provide special function to catch all std::shared_ptr
 * which point to the same object and call std::shared_ptr::restore() for each of them. 
 * As you can get by cppreference.com, even after have restored the last std::shared_ptr, you do not need to call delete operator
 * on the object since it is already called by the last restored one. 
 * 
 * All JPL's list implementation let you insert null and duplicate elements both. 
 * 
 * @date 2023-07-12
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
*/

#ifndef LISTI_JPL
#define LISTI_JPL

#include <memory>
#include "../Collection.hpp"
#include <jpl/exception/runtime/IndexOutOfBoundsException.hpp>
#include <jpl/exception/runtime/NotFoundException.hpp>
#include <jpl/exception/runtime/NullPointerException.hpp>

namespace jpl{

    namespace _utils{

        namespace _collections{
            
            template <typename T>
            class List : public Collection<T>{
                    
                protected:

                    /**
                     * Create a new empty list
                     * 
                     * @param nullableElements if this list allow null elements
                     * @param duplicateElements if this list allow duplicate elements
                     * @param resizeable if this list can be resized
                    */
                    List(bool nullableElements, bool duplicateElements, bool resizeable) : Collection<T>(nullableElements, duplicateElements, resizeable){}
                    List() : List<T> (true, true, true){}
                    /**
                     * Create a new List from the given collection
                     * 
                     * @param collection 
                     * @throw NullPointer if collection is nullptr
                    */
                    List(List<T>* list) : List<T>(){
                        if(list == nullptr){
                            throw _exception::NullPointerException("You cannot pass a nullptr as list");
                        }
                    }
                    
                public:

                    /**
                     * Insert the given t at index
                     * 
                     * @param index index
                     * @param t new element
                     * 
                     * @throw NullPointerException if list does not permit null elements and t it is
                     * @throw RuntimeException if an element is already present into index position
                    */
                    virtual void add(unsigned long index, T t) = 0;
                    /**
                     * Insert the whole given list at index
                     * 
                     * @param index
                     * @param list
                     * 
                     * @throw NullPointerException if list does not permit null elements
                     * @throw NullPointerException if collection is nullptr
                    */
                    virtual void addAll(unsigned long index, List<T>* list) = 0;
                    /**
                    * @brief Insert into the structure all items contained into list at the end
                    * if the array list
                    * 
                    * @param list 
                    *
                    * @throw NullPointerException if list does not permit null elements
                    * @throw NullPointerException if collection is nullptr
                    */
                    virtual void addAll(List<T>* list) = 0;          

                    /**
                     * @return the element at the given index
                     * 
                     * @throw IndexOutOfBounds if index is graeter than size
                    */
                    virtual T get(unsigned long index) const = 0;


                    /**
                     * @return the index of the first occurrence of t. 
                     * If t is not found into the list, it returns max 
                    */
                    virtual unsigned long firstOccurrence(T t) const noexcept = 0;
                    /**
                     * @return the index of the last occurrence of t.
                     * If t is not found into the list, it returns max
                    */
                    virtual unsigned long lastOccurrence(T t) const noexcept = 0;

                    
                    /**
                     * @brief remove the element at the given index
                     * 
                     * @throw IndexOutOfBounds if index is graeter than length()-1 or less than 0
                    */
                    virtual void removeAt(unsigned long index) = 0;
                    /**
                     * @brief remove t from this collection
                     * It does not throw NotFoundException if t has not been found
                     * 
                     * @param t item to remove
                     * @return if it has removed a t element
                     */
                    virtual bool remove(T t) noexcept = 0 ;   
                    /**
                     * @brief remove all t instance - if T is a pointer type - from this collection 
                     * It does not throw NotFoundException if t has not been found
                     * 
                     * @param t item to remove
                     * @return the amount of t element removed
                     */
                    virtual size_t removeAllOf(T t) noexcept = 0;                  
                    /**
                     * @brief remove all list's items from this collection
                     * It does not throw NotFoundException if t has not been found
                     * 
                     * @param list list of items to remove
                     */
                    virtual void removeAll(List<T>* list) noexcept = 0;
                    /**
                     * @brief Remove all elements which respect the given predicate. If T is a pointer to object, unless you 
                     * have checked if element is nullptr, a segmentation-fault will occur
                     * @param predicate
                     * @return the amount of element removed 
                    */
                    virtual size_t removeAllIf(_functional::Predicate<T> predicate) = 0;

                    /**
                     * Set t at the given index (which was already occuped)
                     * 
                     * @return the previous element which was at index
                     * 
                     * @throw IndexOutOfBounds if index is graeter or equals than getSize()-1
                     * @throw NullPointerException if list does not permit null elements and t it is
                    */
                    virtual T set(unsigned long index, T t) = 0;

                    /**
                     * @param start index of the first element to copy
                     * @param end index of the last element to copy (included) 
                     * 
                     * @return a sub list of the current one. If start is equals to end, am empty list is returned
                     * 
                     * @throw IndexOutOfBounds if start is graeter or equals than max
                     * @throw IndexOutOfBounds if end is graeter or equals than max
                    */
                    virtual List<T>* subList(unsigned long start, unsigned long end) const = 0;
                    /**
                     * @brief The list is computed 'till the end of the target oneremove
                     * 
                     * @param start index of the first element to copy
                     * 
                     * @return a sub list of the current one. If start is equals to end, am empty list is returned
                     * 
                     * @throw IndexOutOfBounds if start is graeter or equals than max
                    */
                    virtual List<T>* subList(unsigned long start) const = 0;

                    virtual bool resetHelper(std::shared_ptr<T> &ptr){
                        #ifdef DEL_EFF_DS_JPL
                            bool del = false;
                            T t; 
                            if(this->pointer)
                                if(ptr.use_count() == 1){
                                    del = true;
                                    t = *ptr.get();
                                }
                        #endif
                        ptr.reset();
                        #ifdef DEL_EFF_DS_JPL
                            if(del)
                                Ereaseable<T>::erease(t);
                            return del;
                        #else
                            return false;
                        #endif
                    }
            };
        }
    }
}

#endif