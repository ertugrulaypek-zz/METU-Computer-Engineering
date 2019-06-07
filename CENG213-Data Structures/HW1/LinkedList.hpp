#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T> 
class LinkedList {
    private:
		/*First node of the linked-list*/
        Node<T>* head;
        /*Last node of the linked-list*/
		Node<T>* tail;
		/*size of the linked-list*/
		size_t  size; 
    public:

        LinkedList();
        LinkedList(const LinkedList<T>& ll);
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* Return head of the linked-list*/
        Node<T>* getHead() const;
        /* Set head of the linked-list*/
        void setHead(Node<T>* n);
        /* Return tail of the linked-list*/
        Node<T>* getTail() const;
        /* Set tail of the linked-list*/
        void setTail(Node<T>* n);
        /* Get the previous node of the node that contains the data item. 
         * If the head node contains the data item, this method returns NULL.*/
        Node<T>* findPrev(const T& data) const;
        /* Get the node that stores the data item. 
         * If data is not found in the list, this function returns NULL.*/
        Node<T>* findNode(const T& data) const;
        /* Insert a new node to store the data item. 
         * The new node should be placed after the “prev” node. 
         * If prev is NULL then insert new node to the head.*/
        void insertNode(Node<T>* prev, const T& data); 
        /* This method is used to delete the node that is next to “prevNode”. 
         * PS:prevNode is not the node to be deleted. */
        void deleteNode(Node<T>* prevNode);  
        /* This method is used to clear the contents of the list.*/
        void clear();
        /* This method returns true if the list empty, otherwise returns false.*/
        bool isEmpty() const;
        /* This method returns the current size of the list. */
        size_t getSize() const;
        /*Prints the list. This method was already implemented. Do not modify.*/
        void print() const;
};

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = head;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template<class T>
LinkedList<T>::LinkedList() : head(NULL), tail(NULL), size(0) {}





template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &ll){
    Node<T>* rhstraveller=ll.getHead();
    Node<T> *newNode = new Node<T>(rhstraveller->getData());
    head = newNode;
    Node<T>* lhstraveller=head;

    while(rhstraveller->getNext()){
        rhstraveller->setNext(rhstraveller->getNext());
        lhstraveller->setNext(new Node<T> (lhstraveller->getData()));
        
    }
    Node<T>* lastNode= new Node<T>(rhstraveller->getData());
    tail=lastNode;
    size=ll.getSize();

}





template<class T>
LinkedList<T>::~LinkedList() { 
    clear(); 
}


template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &ll){
    Node<T>* rhstraveller=ll.getHead();
    Node<T>* newNode = new Node<T> (rhstraveller->getData());
    head = newNode;
    Node<T>* lhstraveller=head;

    while(rhstraveller->getNext()){
        rhstraveller->setNext(rhstraveller->getNext());
        lhstraveller->setNext(new Node<T> (lhstraveller->getData()));

        /*lhstraveller->next=new Node<T>*(rhstraveller->getData());*/
        /*lhstraveller=lhstraveller->next;*/
    }

    Node<T>* lastNode= new Node<T> (rhstraveller->getData());
    setTail(lastNode);
    size=ll.getSize();
    return *this;

}

template<class T>
Node<T>* LinkedList<T>::getHead() const{
    return head;
}

template<class T>
void LinkedList<T>::setHead(Node<T>* rhs){
    head=rhs;
}

template<class T>
Node<T>* LinkedList<T>::getTail() const{
    return tail;
}

template<class T>
void LinkedList<T>::setTail(Node<T>* rhs){
    tail=rhs;
}

template<class T>
Node<T>* LinkedList<T>::findPrev(const T& wanted) const{
    Node<T> prev=NULL;
    Node<T>* traveller=head;
    while(1){
        if(traveller.getData()==wanted){
            break;
        }
        else{
            prev=traveller;
            traveller=traveller->getNext();

        }
    }
    return prev;
}

template<class T>
Node<T>* LinkedList<T>::findNode(const T& wanted) const{
    Node<T>* traveller=head;
    while(!tail){
        if(traveller.getData()==wanted){
            return traveller;
        }
    }
    if(tail.getData()==wanted){
        return tail;
    }
    return NULL;
}

template<class T>
void LinkedList<T>::insertNode(Node<T>* wantedprev, const T& insertdata){
    
    if(wantedprev!=NULL){
        Node<T>* newNode= new Node<T>(insertdata);
        Node<T>* temp=wantedprev->getNext();
        wantedprev->setNext(newNode);
        newNode->setNext(temp);

    }
    else{
        Node<T>* newNode=new Node<T> (insertdata);
        head=newNode;
    }
    size++;
}


template<class T>
void LinkedList<T>::deleteNode(Node<T>* givenprev){
    Node<T>* temp=givenprev->getNext();
    givenprev->setNext(temp->getNext());
    delete temp;
    size--;

}

template<class T>
void LinkedList<T>::clear(){
    Node<T>* temp=head;
    Node<T>* tempnext=head->getNext();
    while(tempnext){
        delete temp;
        temp=tempnext;
        tempnext=tempnext->getNext();

    }
    delete temp;
    delete tempnext;

}

template<class T>
bool LinkedList<T>::isEmpty() const{
    return head==NULL;
}

template<class T>
size_t LinkedList<T>::getSize() const{
    return size;
}







/* end of your implementations*/

#endif
