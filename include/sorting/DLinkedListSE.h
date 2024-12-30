/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T>{
public:
    
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ){
        
    };
    
    DLinkedListSE(const DLinkedList<T>& list){
        this->copyFrom(list);
    }
    
    void sort(int (*comparator)(T&,T&)=0){
        //TODO: implement this function
        //     - You should implement the merge sort algorithm
        if (this->head->next == this->tail || this->head->next->next == this->tail) {
            return;  // Nếu danh sách trống hoặc chỉ có một phần tử
        }
        this->head->next = mergeSort(this->head->next, comparator);  // Bắt đầu sắp xếp từ phần tử sau head
        this->head->next->prev = this->head;
        // Cập nhật lại con trỏ prev của tail
        typename DLinkedList<T>::Node* current = this->head->next;
        while (current->next != this->tail) {  // Duyệt đến phần tử cuối cùng
            current = current->next;
        }

        // Cập nhật tail->prev để trỏ đến phần tử cuối cùng
        this->tail->prev = current;
    };
    
protected:
    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
private:
// Hàm chia danh sách thành hai nửa
   typename DLinkedList<T>:: Node *split( typename DLinkedList<T>:: Node *head)
    {
        typename DLinkedList<T>::Node *fast = head;
        typename DLinkedList<T>::Node *slow = head;
        while (fast->next != this->tail && fast->next->next !=  this->tail)
        {
            fast = fast->next->next;
            slow = slow->next;
        }
        typename DLinkedList<T>::Node *second = slow->next; // Điểm bắt đầu của nửa sau
        slow->next =  this->tail;            // Chia danh sách, nửa đầu kết thúc tại `slow`
        return second;                // Trả về con trỏ tới nửa sau
    }

// Hàm hợp nhất hai danh sách đã được sắp xếp
    typename DLinkedList<T>::Node* merge(typename DLinkedList<T>::Node* left, typename DLinkedList<T>::Node* right, int (*comparator)(T&, T&) = 0) {
        if (left ==  this->tail) return right;
        if (right ==  this->tail) return left;

        if (compare(left->data, right->data, comparator) <= 0) {
            left->next = merge(left->next, right, comparator);
            if (left->next !=  this->tail) {  // Nếu next của left không phải tail, cập nhật prev
                left->next->prev = left;
            }       
            return left;
        } else {
                right->next = merge(left, right->next, comparator);
                if (right->next !=  this->tail) {  // Nếu next của right không phải tail, cập nhật prev
                    right->next->prev = right;
                }
                return right;
        }
    }
// Hàm sắp xếp danh sách liên kết đôi
 // Hàm sắp xếp danh sách liên kết đôi
    typename DLinkedList<T>::Node* mergeSort(typename DLinkedList<T>::Node* head, int (*comparator)(T&, T&) = 0) {
        if (head ==  this->tail || head->next ==  this->tail) {  // Kiểm tra head là tail hoặc chỉ có một phần tử
            return head;
        }

        // Chia danh sách thành hai nửa
        typename DLinkedList<T>::Node* second = split(head);

        // Đệ quy sắp xếp hai nửa
        head = mergeSort(head, comparator);
        second = mergeSort(second, comparator);

        // Hợp nhất hai nửa đã sắp xếp
        return merge(head, second, comparator);
    }
};


#endif /* DLINKEDLISTSE_H */

