/**
 * @file
 * 
 * A linked list is an ordered collection of nodes which - probably - are not sequentially stored 
 * in memory due to available space during insertion time
 * 
 * A node is a struct which contains the element T, a pointer to the previous node and a pointer 
 * to the next one; thus, this is actually a Double and Circular Linked List as described by DSA.
 * The first node is called also Head and the last one Tail.
 * Being a DLL, insertion is O(1) only when it is done either at head or tail of it, otherwise is O(n).
 * Searching is always O(n).
 * Indexed Deletion - removeAt(ulong) - is just like Insertion; Smooth one - remove(T) - is O(n).
 * 
 * Just like ArrayList, if you declare a Linked List of pointer, no new object will be allocated
 * 
 * @date 14-09-2023
 * @author Gabriele-P03
*/
#ifndef LINKED_LIST_JPL
#define LINKED_LIST_JPL

#include "List.hpp"

namespace jpl{
    namespace _utils{
        namespace _collections{
            namespace _list{
                

                template<typename T>
                class LinkedList : public List<T>{

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
                                T* t = new T();
                                *t = element;
                                this->element = std::shared_ptr<T>(t);
                                this->next = next;
                                this->previous = previous;
                            }
                            Node(T element) : Node(element, nullptr, nullptr){}
                            Node(std::shared_ptr<T> ptr) : Node(ptr, nullptr, nullptr){}
                            Node(){}
                        };

                        Node *head, *tail;

                        Node* forwardProbe(unsigned long steps) const {
                            if( steps >= this->max )
                                throw new _exception::IndexOutOfBoundsException(this->max-1, steps);
                            Node* node = head;
                            for(unsigned long i = 0; i < steps; i++){
                                node = node->next;
                            }
                            return node;
                        }
                        Node* reverseProbe(unsigned long steps) const{
                            if( steps >= this->max )
                                throw new _exception::IndexOutOfBoundsException(this->max-1, steps);
                            Node* node = tail;
                            for(unsigned long i = steps; i > 0; i--){
                                node = node->previous;
                            }
                            return node;
                        }

                        Node* smartProbe(unsigned long index) const{
                            if(index >= this->max){
                                throw new _exception::IndexOutOfBoundsException(this->max-1, index);
                            }
                            if( index <= (this->size/2) ){
                                return this->forwardProbe(index);
                            }                                
                            return this->reverseProbe(this->size-index-1);
                        }

                        /**
                         * @brief
                         * It detaches the given node by attaching the previous one with the next one.
                         * This function does not delete Node's element 
                         * @throw NullPointer if node is nullptr
                        */
                        void detach(Node* &node){
                            if(node == nullptr)
                                throw new _exception::NullPointerException("A nullptr cannot be passed");                           
                            if(node->previous != nullptr){  //not head
                                if(node->next != nullptr){  //not tail
                                    node->previous->next = node->next;
                                    node->next->previous = node->previous;
                                }else{//tail -> new tail
                                    node->previous->next = nullptr;
                                    this->tail = node->previous;
                                }
                            }else{  //head -> new head
                                if(node->next != nullptr){
                                    node->next->previous = nullptr;
                                    this->head = node->next;
                                }
                            }
                            this->size--;
                            this->max--;
                        }

                        /**
                         * It detaches all nodes among first and last (both included)
                         * You must be sure that all nodes from first to last are deleted
                        */
                        void detach_s(Node* &first, Node* &last, unsigned long size){
                            if(first == this->head){
                                if(last != this->tail){
                                    last->next->previous = nullptr;
                                    this->head = last->next;
                                    this->size -= size;
                                    this->max -= size;
                                }else{
                                    this->head = nullptr;
                                    this->tail = nullptr;
                                    this->size = 0;
                                    this->max = 0;
                                }
                            }else{
                                if(last != this->tail){
                                    first->previous->next = tail->next;
                                    tail->next->previous = first->previous;
                                }else{
                                    first->previous->next = nullptr;
                                    this->tail = first->previous;
                                }
                                this->size -= size;
                                this->max -= size;
                            }
                        }

                        void clearAllOf(T t, unsigned long index){
                            if(index >= this->size)
                                throw new _exception::IndexOutOfBoundsException(this->max-1, index);
                            Node* cr = this->head;
                            for(unsigned long i = index+1; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    cr->element.reset();
                            }    
                        }

                        Node* firstNodeOf(T t, unsigned long end) const noexcept{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < end; i++){
                                if(*cr->element.get() == t)
                                    return cr;
                                cr = cr->next;
                            }
                            return nullptr;
                        }

                        void createNewNode(Node* &cr, T &t, unsigned long end) const noexcept{
                            if(Pointerable<T>::isPointer(t)){
                                //Check if t is being managed by a former std::shared_ptr
                                Node* former = this->firstNodeOf(t, end);
                                if(former != nullptr)
                                    cr->element = std::shared_ptr<T>(former->element);
                                else{
                                    T* nw = new T;
                                    *nw = t;
                                    cr->element = std::make_shared<T>(t);
                                }
                            }else{
                                T* nw = new T;
                                *nw = t;
                                cr->element = std::make_shared<T>(t);
                            }
                        }
                    public:

                        LinkedList() : List<T>(){
                            this->head = nullptr;
                            this->tail = nullptr;
                        }
                        LinkedList(unsigned long size) : List<T>(){
                            if(size > 0){
                                Node* pre = nullptr;
                                for(unsigned long i = 0; i < size; i++){
                                    Node* cr = new Node();
                                    if(i == 0){
                                        this->head = cr;
                                    }else{
                                        pre->next = cr;
                                        cr->previous = pre;
                                    }
                                    pre = cr;
                                }
                                this->tail = pre;   //Pre is always set as the last one
                                this->max = size;
                                this->size = size;
                            }
                        }
                        LinkedList(List<T>* list) : LinkedList<T>(list->getSize()){
                            this->addAll(list);
                        }
                        LinkedList(std::initializer_list<T> list) : List<T>(){
                            unsigned long size = list.size();
                            Node* pre = nullptr;
                            for(unsigned long i = 0; i < size; i++){
                                T t = list.begin()[i];
                                Node* cr = new Node();
                                this->createNewNode(cr, t, i);
                                if(i == 0){
                                    this->head = cr;
                                }else{
                                    pre->next = cr;
                                    cr->previous = pre;
                                }
                                pre = cr;
                            }
                            this->tail = pre;
                            this->max = size;
                            this->size = size;
                        }

                        virtual T get(unsigned long index) const override{
                            if(index == 0){
                                return *this->head->element.get();
                            }else if(index == this->size-1){
                                return *this->tail->element.get();
                            }else{
                                return *this->smartProbe(index)->element.get();
                            }
                        }

                        virtual void add(unsigned long index, T t) override{
                            Node* newNode = new Node;
                            this->createNewNode(newNode, t, this->size);
                            if(index == 0){ //head
                                if(this->size != 0){//LL not empty
                                    this->head->previous = newNode;
                                    newNode->next = this->head;
                                }else{//If LL is empty, head is equal to tail
                                    this->tail = newNode;
                                }
                                this->head = newNode;
                            }else if(index == this->size){//Tail
                                this->tail->next = newNode;
                                newNode->previous = this->tail;
                                this->tail = newNode;
                            }else{
                                Node* tmp = this->smartProbe(index);
                                tmp->previous->next = newNode;
                                newNode->previous = tmp->previous;
                                tmp->previous = newNode;
                                newNode->next = tmp;    
                            }
                            if(this->size++ >= this->max){
                                this->max = this->size;
                            }
                        }

                        virtual void add(T t) noexcept override{
                            this->add(this->size, t);
                        }


                        virtual bool contains(T t) const noexcept override{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    return true;
                                cr = cr->next;
                            }
                            return false;
                        }
                        virtual void clear() noexcept override{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                cr->element.reset();
                                delete cr;
                                cr = cr->next;
                            }   
                            this->size = 0;
                            this->max = 0;
                        }
                        virtual void addAll(unsigned long index, List<T>* list) override{
                            if(list == nullptr){
                                throw new _exception::NullPointerException("list passed to addAll cannot be nullptr");
                            }
                            unsigned long size = list->getSize();
                            Node* first = nullptr, *pre = nullptr;
                            for(unsigned long i = 0; i < size; i++){
                                T t = list->get(i);
                                Node* n = new Node;
                                this->createNewNode(n, t, this->size+i);
                                if(i == 0){
                                    first = n;
                                }else{
                                    pre->next = n;
                                    n->previous = pre;
                                }
                                pre = n;
                            }
                            if(this->size == 0){
                                this->head = first;
                                this->tail = pre;   //It is safe even when collection has one item
                            }else{
                                if(index == 0){
                                    this->head->previous = pre;
                                    pre->next = this->head;
                                    this->head = first;
                                }else if(index == this->size){
                                    this->tail->next = first;
                                    first->previous = this->tail;
                                    this->tail = pre;
                                }else{
                                    Node* cr1 = this->smartProbe(index);
                                    cr1->next->previous = pre;
                                    pre->next = cr1->next;
                                    cr1->next = first;
                                    first->previous = cr1;
                                }
                            }
                            this->size += size;
                            if(this->size > this->max)
                                this->max = this->size;
                        }
                        virtual void addAll(List<T>* list) override{
                            this->addAll(this->size, list);
                        } 
                        virtual unsigned long firstOccurrence(T t) const noexcept override {
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t)
                                    return i;
                                cr = cr->next;
                            }
                            return this->size;
                        }
                        virtual unsigned long lastOccurrence(T t) const noexcept override {
                            Node* cr = this->tail;
                            for(unsigned long i = this->size; i > 0; i--){
                                if(*cr->element.get() == t)
                                    return i-1;
                                cr = cr->previous;
                            }
                            return this->size;
                        }

                        virtual void removeAt(unsigned long index) override{
                            Node* cr = this->smartProbe(index);
                            cr->element.reset();
                            this->detach(cr);
                            delete cr;
                        }
                        virtual void remove(T t) noexcept override{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t){
                                    cr->element.reset();
                                    this->detach(cr); 
                                    delete cr;
                                    return;
                                }
                                cr = cr->next;
                            }
                        } 
                        virtual void removeAllOf(T t) noexcept override{
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(*cr->element.get() == t){
                                    cr->element.reset();
                                    this->detach(cr); 
                                    delete cr;
                                    //Now let's figure out all other t elements into this list, but before let's check if it a pointer
                                    if(Pointerable<T>::isPointer(t))
                                        this->clearAllOf(t, i);
                                    return;
                                }
                                cr = cr->next;
                            }
                        }                    
                        virtual void removeAll(List<T>* list) noexcept override{
                            Node* cr = this->head;
                            Node* firstDeleted = nullptr;
                            unsigned long len = 0;
                            for(unsigned long i = 0; i < list->getSize(); i++){
                                T ls = list->get(i);
                                for(unsigned long j = 0; j < this->size; j++){
                                    if(*cr->element.get() == ls){
                                        cr->element.reset();
                                        delete cr;
                                        if(firstDeleted == nullptr){
                                            firstDeleted = cr;
                                        }
                                        len++;
                                    }else{
                                        if(firstDeleted != nullptr){
                                            this->detach_s(firstDeleted, cr->previous, len);
                                            firstDeleted = nullptr;
                                            len = 0;
                                        }
                                    }
                                    cr = cr->next;
                                }      
                                if(firstDeleted != nullptr){
                                    this->detach_s(firstDeleted, cr, len);
                                }                        
                            }

                        }
                        virtual void removeAllIf(_functional::Predicate<T> predicate) noexcept override{
                            Node* cr = this->head;
                            Node* firstDeleted = nullptr;
                            unsigned long len = 0;
                            for(unsigned long i = 0; i < this->size; i++){
                                if(predicate.test(*cr->element.get())){
                                    cr->element.reset();
                                    if(firstDeleted == nullptr){
                                        firstDeleted = cr;
                                    }
                                    len++;
                                }else{
                                    if(firstDeleted != nullptr){
                                        this->detach_s(firstDeleted, cr->previous, len);
                                        firstDeleted = nullptr;
                                        len = 0;
                                    }
                                }
                                cr = cr->next;
                            }
                            if(firstDeleted != nullptr){
                                this->detach_s(firstDeleted, cr, len);
                            }  
                        }
                        virtual T set(unsigned long index, T t) override {
                            Node* cr = this->smartProbe(index);
                            T cp;
                            Nullable<T>::nullify(cp);
                            Copyable<T>::copy(cp, *cr->element.get());
                            Node* former = this->firstNodeOf(t, this->size);
                            if(former == nullptr){
                                cr->element.reset();
                                cr->element = std::make_shared<T>(t);
                            }
                            else{
                                cr->element.reset();
                                cr->element = std::shared_ptr<T>(cr->element);
                            }
                            return cp;
                        }
                        virtual List<T>* subList(unsigned long start, unsigned long end) const override {
                            if(start >= end)
                                throw new jpl::_exception::IndexOutOfBoundsException(end, start);
                            if(end > this->max)
                                throw new jpl::_exception::IndexOutOfBoundsException(this->max, end);
                            unsigned long offset = end - start;
                            jpl::_utils::_collections::_list::LinkedList<T>* list = new jpl::_utils::_collections::_list::LinkedList<T>(offset);
                            Node* cr = this->head, *cr1 = list->head;
                            for(unsigned long i = 0; i < offset; i++){
                                cr1->element = cr->element;
                                cr = cr->next;
                                cr1 = cr1->next;
                            }
                            return list;
                        }
                        virtual List<T>* subList(unsigned long start) const override {return this->subList(start, this->max);}
                        virtual void forEach(_functional::Consumer<T> consumer) override {
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                consumer.test(*cr->element.get());
                                cr = cr->next;
                            }
                        }
                        virtual T* toArray() const noexcept override{
                            if(this->size == 0)
                                return nullptr;
                            T* buffer = new T[this->size];
                            Node* cr = this->head;
                            for(unsigned long i = 0; i < this->size; i++){
                                buffer[i] = *cr->element.get();
                                cr = cr->next;
                            }
                            return buffer;
                        }

                        friend std::ostream& operator<<(std::ostream& os, const LinkedList<T> &list){
                            Node* cr = list.head;
                            for(unsigned long i = 0; i < list.size; i++){
                                Printable<T>::print(os, *cr->element.get());
                                cr = cr->next;
                            }
                            return os;
                        }                    
                };
            }
        }
    }
}

#endif