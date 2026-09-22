/**
 * @file
 * 
 * An ArrayList is a list which elements are consecutive stored as into a vector
 * 
 * This type of list is resizeable, but, being an array, if the size exceeds, the whole list is reallocated.
 * When a collection is attempted to being inserted, in order to prevent several reallocating, a unique call is done 
 * based on size of the collection which is going to be inserted.
 * If you know that several amount of calls are going to be done to add() - and you know how many 
 * elements are going to be - then you should call reallocate() by yourself, preventing the same several amount
 * of calls to the function 
 * 
 * If you need a list which will be manipulated frequently, you should opt to use a LinkedList in order to not perform 
 * any reallocating.
 * 
 * Null elements are allowed into this.
 * 
 * ArrayList does not implements any borderline about max amount of elements which can be contained into it
 * 
 * @date 2023-07-12
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
*/

#ifndef ARRAYLIST_JPL
#define ARRAYLIST_JPL

#include "List.hpp"

namespace jpl{

    namespace _utils{

        namespace _collections{

            namespace _list{

                template <typename T>
                class ArrayList : public List<T>{

                    protected:
                        std::shared_ptr<T>* list;

                        /**
                         * @brief Right Shift a part of the array list which begins from start 'till last element.
                         * It is used by add(s) function.
                         * 
                         * This function is out-of-warnings even on the first insert operation
                         * 
                         * @param start first element to be shift. 
                         * @param offset amount of cell to shift
                         * 
                         * @throw IndexOutOfBounds if start + offset is greater than list's max
                         */
                        virtual void rightShiftItems(unsigned int start, unsigned int offset){
                            if(start > this->max)
                                throw jpl::_exception::IndexOutOfBoundsException(this->max, this->size+offset);
                            if(this->size + offset > this->max)
                                this->reallocate(this->max + this->max - this->size + offset);
                            for(unsigned int i = this->size; i > start; i--)
                                this->list[i+offset-1] = this->list[i-1];
                        }
                        /**
                         * @brief Left Shift a part of the array list which begins from start 'till last element.
                         * It is used by remove(s) function.
                         * 
                         * @param start first element to be shift. 
                         * @param offset amount of cell to shift
                         * 
                         * @throw IndexOutOfBounds if start - offset is less 0
                         */
                        virtual void leftShiftItems(unsigned int start, unsigned int offset){
                            if(start - offset < 0)
                                throw jpl::_exception::IndexOutOfBoundsException(0, start - offset);
                            for(unsigned int i = start; i < this->size-1; i++)
                                this->list[i] = this->list[i+offset];
                        }

                        std::shared_ptr<T> firstOf(T t) const noexcept{
                            for (unsigned long i = 0; i < this->size; i++){
                                std::shared_ptr<T> &cr = this->list[i];
                                if(*cr.get() == t)
                                    return cr;
                            }
                            return nullptr;
                        } 

                    public:

                        ArrayList() : List<T>(){}
                        ArrayList(unsigned long size) : ArrayList<T>(){
                            this->size = 0;
                            this->max = size;
                            this->list = new std::shared_ptr<T>[this->max]; 
                        } 
                        ArrayList(const T* const array, unsigned long size) : ArrayList<T>(){
                            if(array == nullptr)
                                throw _exception::IllegalArgumentException("You cannot pass a null pointer");
                            else{
                                if(size == 0)
                                    throw _exception::IllegalArgumentException("You have passed a size parameter as 0");
                            }
                            this->size = size;
                            this->max = size;
                            this->list = new std::shared_ptr<T>[this->size];
                            for(unsigned long i = 0; i < size; i++)
                                this->list[i].reset(array[i]);
                        }
                        ArrayList(std::initializer_list<T> ls) : ArrayList<T>(){
                            this->reallocate(ls.size());
                            for(unsigned long i = 0; i < this->max; i++)
                                this->list[i] = std::make_shared<T>(ls.begin()[i]);
                            this->size = ls.size();
                        }
                        ArrayList(List<T>* list) : List<T>(list){this->addAll(list);}

                        /**
                         * @brief It reallocates current list as size amount
                         * 
                         * @param size 
                        */
                        virtual void reallocate(unsigned long size){
                            if(this->size >= size)
                                throw jpl::_exception::IllegalArgumentException("An arraylist cannot be reallocated with a size less than its current one. If you wanna reduce its size, you should delete");
                            this->max = size;
                            if(this->size == 0)
                                this->list = new std::shared_ptr<T>[size];
                            else{
                                std::shared_ptr<T>* tmp = new std::shared_ptr<T>[size];
                                for(unsigned long i = 0; i < this->size; i++){
                                    tmp[i] = std::shared_ptr<T>(this->list[i]);
                                }
                                delete [] this->list;   //Do not reset elements
                                this->list = tmp;
                            }
                        }


                        virtual T get(unsigned long index) const override{
                            if(index >= this->size)
                                throw jpl::_exception::IndexOutOfBoundsException(this->size, index);
                            return *this->list[index].get();
                        }


                        virtual void add(unsigned long index, T t) override{
                            if(index >= this->max)
                                this->reallocate(index+1);
                            std::shared_ptr<T> &cr = this->list[index];
                            if(this->pointer){
                                std::shared_ptr<T> former = this->firstOf(t);
                                if(former != nullptr){
                                    cr = std::shared_ptr<T>(former);
                                    this->size++;
                                    return;
                                }
                            }
                            cr = std::make_shared<T>(t);
                            this->size++;
                        }
                        virtual void add(T t) noexcept override{this->add(this->size, t);}


                        virtual void addAll(unsigned long index, List<T>* list) override{
                            if(list == nullptr)
                                throw _exception::NullPointerException("Collection passed to addAll cannot be nullptr");
                            if(index+list->getSize() > this->max)
                                this->reallocate(index+list->getSize()-this->max);
                            if(index < this->size)
                                this->rightShiftItems(list->getSize(), 1);
                            for(unsigned long i = 0; i < list->getSize(); i++){
                                std::shared_ptr<T> &cr = this->list[index+i];
                                T t = list->get(i);
                                if(this->pointer){
                                    std::shared_ptr<T> former = this->firstOf(t);
                                    if(former != nullptr){
                                        cr = std::shared_ptr<T>(former);
                                        continue;
                                    }
                                }
                                cr = std::make_shared<T>(t);
                            }
                            this->size += index + list->getSize() - this->size;
                        }
                        virtual void addAll(List<T>* list) noexcept override{this->addAll(this->size, list);}


                        virtual bool contains(const T t) const noexcept override{
                            for(unsigned long i = 0; i < this->size; i++)
                                if( *this->list[i].get() == t )
                                    return true;
                            return false;
                        } 

                        virtual void removeAt(unsigned long index) override{
                            if(index >= this->size)
                                throw jpl::_exception::IndexOutOfBoundsException(this->size, index);
                            std::shared_ptr<T> &cr = this->list[index];
                            this->resetHelper(cr);
                            this->leftShiftItems(index, 1);
                            this->reallocate(--this->size);
                        }
                        virtual bool remove(T t) noexcept override{
                            for(unsigned long i = 0; i < this->size; i++){
                                std::shared_ptr<T> &cr = this->list[i];
                                if( *cr.get() == t ){
                                    this->resetHelper(cr);
                                    this->leftShiftItems(i, 1);
                                    this->reallocate(--this->size);
                                    return true;
                                }
                            }
                            return false;    
                        }  
   
                        virtual void removeAll(List<T>* list) noexcept override{
                            for(unsigned long i = 0; i < list->getSize(); i++){
                                T ls = list->get(i);
                                bool flag = false;
                                for(unsigned long j = 0; j < this->size; j++){
                                    std::shared_ptr<T> &cr = this->list[j];
                                    if(ls == *cr.get()){
                                        this->resetHelper(cr);
                                        this->leftShiftItems(j, 1);
                                        this->size--;
                                        flag = true;
                                        j--;
                                        if(this->pointer)
                                            i--;
                                        break;
                                    }
                                }    
                            }
                            this->reallocate(this->size);
                        }
                        virtual size_t removeAllIf(_functional::Predicate<T> predicate) override{
                            bool flag = false;  //It is used to check if at least an element has been removed
                            size_t amount = 0;
                            for(unsigned long i = 0; i < this->size; ){
                                std::shared_ptr<T> &cr = this->list[i];
                                if(predicate.test(*cr.get())){
                                    this->resetHelper(cr);
                                    this->leftShiftItems(i, 1);
                                    this->size--;
                                    amount++;
                                }else
                                    i++;
                            } 
                            if(amount > 0)
                                this->reallocate(this->size);
                            return amount;
                        }
                        virtual size_t removeAllOf(T t) noexcept{
                            size_t amount = 0;
                            for(unsigned long i = 0; i < this->size; i++){
                                std::shared_ptr<T> &cr = this->list[i];
                                if(*cr.get() == t){
                                    this->resetHelper(cr);
                                    this->leftShiftItems(i, 1);
                                    amount++;
                                }
                            }
                            if(amount > 0)
                                this->reallocate(this->size-amount);
                            return amount;
                        }

                        virtual T set(unsigned long index, T t) override {
                            if(index >= this->size)
                                throw _exception::IndexOutOfBoundsException(this->size, index);
                            std::shared_ptr<T> &cr = this->list[index];
                            T cp;
                            Nullable<T>::nullify(cp);
                            Copyable<T>::copy(cp, *cr.get());
                            cr.reset(&t);
                            return cp;
                        } 

                        virtual ArrayList<T>* subList(unsigned long start, unsigned long end) const override{
                            if(start >= this->size)
                                throw _exception::IndexOutOfBoundsException(this->size, start, "Start is larger/equals than list's size");
                            if(end >= this->size)
                                throw _exception::IndexOutOfBoundsException(this->size, end, "End is larger/equals than list's size");               
                            if(start > end)
                                throw _exception::IndexOutOfBoundsException(end, start, "Start is larger than End"); 
                            ArrayList<T>* buffer = new ArrayList<T>(end-start);
                            for(unsigned long i = start; i < end; i++){
                                std::shared_ptr<T> cr = this->list[i];
                                buffer->add(*cr.get());
                            }
                            return buffer;
                        }
                        virtual ArrayList<T>* subList(unsigned long start) const override{return this->subList(start, this->size);}

                        virtual unsigned long firstOccurrence(T t) const noexcept override{ 
                            for(unsigned long i = 0; i < this->size; i++)
                                if(*this->list[i].get() == t)
                                    return i;
                            return this->max;
                        }
                        virtual unsigned long lastOccurrence(T t) const noexcept override{ 
                            for(unsigned long i = this->size-1; i >= 0; i--)
                                if(*this->list[i].get() == t)
                                    return i;
                            return this->max;
                        }

                        virtual void clear() noexcept override{
                            for (unsigned long i = 0; i < this->max; i++)
                                this->resetHelper(this->list[i]);
                            delete [] this->list;
                            this->size = 0;
                            this->max = 0;
                        }


                        virtual T* toArray() const noexcept override {
                            T* buffer = new T[this->size];
                            for (unsigned long i = 0; i < this->size; i++)
                                buffer[i] = *this->list[i].get();
                            return buffer;
                        }

                        virtual void forEach(_functional::Consumer<T> consumer){
                            for(unsigned long i = 0; i < this->size; i++){
                                consumer.test(*this->list[i].get());
                            }
                        }

                        friend std::ostream& operator<<(std::ostream& os, const ArrayList<T> &list){
                            for(unsigned long i = 0; i < list.size; i++)
                                Printable<T>::print(os, list.list[i]);
                            return os;
                        }

                        ~ArrayList(){
                            this->clear();
                        }
                };
            }
        }
    }
}

#endif