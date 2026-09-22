/**
 * @file 
 * 
 * This is the usual implementation of stack using a LIFO method.
 * JPL stack is a doubled LL similar implementation, but it does not inherit it since most of all its functions should have been 
 * overriden returning nullptr; altough it is located into the same List's package.
 * Elements into a stack are managed via std::shared_ptr since this DS let you push null and duplicate elements both.
 * Just like for AL and LL, defining DEL_EFF_DS_JPL, you let the DS destroys the element once the last smart-pointer had released it
 * 
 * @author Gabriele-P03
 * @date 2024-04-03
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STACK_JPL
#define STACK_JPL

#include "../Collection.hpp"

namespace jpl{
    namespace _utils{
        namespace _collections{
            namespace _list{
                
                template <typename T>
                class Stack : public Collection<T>{

                    private:
                        /**
                         * @brief this function is override just for avoid compile-error
                         * @throw RuntimeException anytime it is called
                        */
                        virtual void add(T t) {
                            throw _exception::RuntimeException("You must call push(T) in order to add an element into a stack");
                        }

                    protected:
                        struct Node{

                            std::shared_ptr<T> element;
                            Node* previous;
                            Node* next;

                            Node(std::shared_ptr<T> ptr, Node* next, Node* previous){
                                this->element = std::shared_ptr<T>(ptr);
                                this->next = next;
                                this->previous = previous;
                            }

                            Node(T element, Node* next, Node* previous){
                                this->element = std::make_shared<T>(element);
                                this->next = next;
                                this->previous = previous;
                            }
                            Node(T element) : Node(element, nullptr, nullptr){}
                            Node(std::shared_ptr<T> ptr) : Node(ptr, nullptr, nullptr){}
                            Node(){}
                        };

                        Node *head, *tail;


                        Node* firstNodeOf(T t) const noexcept{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    return cr;
                                cr = cr->next;
                            }
                            return nullptr;
                        }

                        void createNewNode(Node* &cr, T t, unsigned long end) const noexcept{
                            if(this->pointer){
                                //Check if t is being managed by a former std::shared_ptr
                                Node* former = this->firstNodeOf(t);
                                if(former != nullptr)
                                    cr->element = std::shared_ptr<T>(former->element);
                                else{
                                    cr->element = std::make_shared<T>(t);
                                }
                            }else{
                                cr->element = std::make_shared<T>(t);
                            }
                        }

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

                    public:
                        Stack() : Collection<T>(true, true, true){
                            this->head = nullptr;
                            this->tail = nullptr;
                        }
                       
                        /**
                         * @return The last element into the stack without removing it
                         * @throw IndexOutOfBoundsException if it is empty
                        */
                        virtual T peek() const{
                            if(this->size == 0)
                                throw _exception::IndexOutOfBoundsException(0, 0);
                            return *this->tail->element.get();    
                        }
                        /**
                         * @return The last element into the stack after have removed it
                         * @throw IndexOutOfBoundsException if it is empty
                        */
                        virtual T pop(){
                            if(this->size == 0)
                                throw _exception::IndexOutOfBoundsException(0, 0);
                            T t;
                            Nullable<T>::nullify(t);
                            T cr = *this->tail->element.get();
                            Copyable<T>::copy(t, cr);
                            this->resetHelper(this->tail->element);
                            this->tail = this->tail->previous;
                            delete this->tail->next;
                            this->tail->next = nullptr;
                            this->size--;
                            return t;
                        }
                        /**
                         * @return Push at the end of the stack the given element
                         * @param t the new element
                        */
                        virtual void push(T t) noexcept {
                            Node* nw = new Node();
                            this->createNewNode(nw, t, this->size);
                            if(this->size == 0){
                                this->head = nw;
                                this->tail = nw;
                            }else{
                                this->tail->next = nw;
                                nw->previous = this->tail;
                                this->tail = nw;
                            }
                            this->size++;
                        }   
                        /**
                         * @brief Its returned value reflects how many times you have to call pop(), before in 
                         * order to get t
                         * @param t element to look for
                         * @return the index of t in the stack
                        */
                        virtual unsigned long search(T t) const noexcept{
                            Node* cr = this->tail;    
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    return i;  
                                cr = cr->previous;
                            }
                            return this->size;
                        }

                        /**
                         * @brief It clears the stack calling pop() for each of elements
                        */
                        virtual void clear() noexcept override{
                            for(unsigned long i = 0; i < this->size; i++)
                                this->pop();
                            this->max=0:
                            this->size=0;
                        }
                        /**
                         * @brief it does not remove elements from the stack (peek)
                         * @return an array of size-sized which contains all elements
                        */
                        virtual T* toArray() const noexcept override{
                            if(this->size == 0)
                                return nullptr;
                            T* buffer = new T[this->size];
                            Node* cr = this->tail;
                            for(unsigned long i = 0; i < this->size; i++){
                                buffer[i] = *cr->element.get();
                                cr = cr->previous;
                            }
                            return buffer;
                        }
                        /**
                         * @brief Execute a forEach loop on each element
                         * @param consumer consumer
                        */
                        virtual void forEach(_functional::Consumer<T> consumer) {
                            Node* cr = this->tail;
                            for(unsigned long i = 0; i < this->size; i++){
                                consumer.test(*cr->element.get());
                                cr = cr->previous;
                            }
                        }
                        /**
                         * @param t to look for
                         * @return if the stack contains t
                        */
                        virtual bool contains(T t) const noexcept {
                            Node* cr = this->tail;    
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    return true;  
                                cr = cr->previous;
                            }
                            return false;
                        }

                        ~Stack(){
                            this->clear();
                        }
                };
            }
        }
    }
}

#endif