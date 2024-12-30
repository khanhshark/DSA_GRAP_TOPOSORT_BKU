/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

       void connect(T from, T to, float weight = 0)
    {
        // TODO
       
         // Kiểm tra nếu đỉnh from và to có tồn tại
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        if (!fromNode) {
            throw VertexNotFoundException(this->vertex2str(from)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'from'
        }
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if (!toNode) {
            throw VertexNotFoundException(this->vertex2str(to)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'to'
        }
        

        // (c) Nếu from và to trùng nhau (self-loop)
            if (this->vertexEQ(from , to)) {
              fromNode->connect(toNode,weight);
            }
            // (d) Nếu from và to khác nhau
            else {
                toNode->connect(fromNode,weight);
                fromNode->connect(toNode,weight); // Vô hướng, nên thêm cả chiều ngược lại
            }
                
            }
    void disconnect(T from, T to)
    {
        // TODO
        // (a) Lấy các VertexNode tương ứng với các đỉnh from và to
    typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
    if (!fromNode) {
        throw VertexNotFoundException(this->vertex2str(from)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'from'
    }

    typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
    if (!toNode) {
        throw VertexNotFoundException(this->vertex2str(to)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'to'
    }

    // (b) Kiểm tra sự tồn tại của cạnh từ from đến to trong danh sách các cạnh của fromNode
    typename AbstractGraph<T>::Edge* edgeToRemove1 = fromNode->getEdge(toNode);
    if (!edgeToRemove1) {
        typename AbstractGraph<T>::Edge edge1(fromNode, toNode);
        throw EdgeNotFoundException(this->edge2Str(edge1));  
    }
    if(this->vertexEQ(from , to)){
    fromNode->removeTo(toNode);  // Xóa cạnh từ fromNode đến toNode
    delete edgeToRemove1;         // Giải phóng bộ nhớ của cạnh
    }
    else{
    typename AbstractGraph<T>::Edge* edgeToRemove2 = toNode->getEdge(fromNode);
    fromNode->removeTo(toNode); 
    toNode->removeTo(fromNode);
    delete edgeToRemove1; 
    delete edgeToRemove2; 
    }
 
   
    }
    void remove(T vertex)
    {
        // TODO
        // Tìm VertexNode tương ứng với đỉnh cần xóa
    typename AbstractGraph<T>::VertexNode* vertexNode = this->getVertexNode(vertex);

    // Kiểm tra nếu đỉnh không tồn tại, ném ngoại lệ VertexNotFoundException
    if (!vertexNode) {
        throw VertexNotFoundException(this->vertex2str(vertex)); // Ném ngoại lệ nếu không tìm thấy đỉnh
    }

    // Duyệt qua danh sách các cạnh đi ra từ đỉnh vertexNode và xóa các cạnh này
    DLinkedList<T> outwardEdges = this->getOutwardEdges(vertex);
    typename DLinkedList<T>::Iterator outIt = outwardEdges.begin();
    while (outIt != outwardEdges.end()) {
        // Lấy đỉnh đích từ các cạnh đi ra
         typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(*outIt);
        if(this->vertexEQ(vertex , toNode->vertex)){
              typename AbstractGraph<T>::Edge* edgeToRemove = vertexNode->getEdge(toNode);
             if (edgeToRemove) {
                vertexNode->removeTo(toNode); // Xóa cạnh từ vertexNode (đỉnh nguồn)
                delete edgeToRemove; // Giải phóng bộ nhớ của cạnh
            }
        }
        else {
            typename AbstractGraph<T>::Edge* edgeToRemove1 = vertexNode->getEdge(toNode);
            typename AbstractGraph<T>::Edge* edgeToRemove2 = toNode->getEdge(vertexNode);
            vertexNode->removeTo(toNode); 
            toNode->removeTo(vertexNode);
            delete edgeToRemove1; 
            delete edgeToRemove2; 
        }
        ++outIt;
    }
    // Sau khi đã xóa hết các cạnh, xóa đỉnh khỏi đồ thị
    this->nodeList.removeItem(vertexNode); // Loại bỏ vertexNode khỏi danh sách các đỉnh trong đồ thị


    // Giải phóng bộ nhớ của vertexNode
    delete vertexNode; // Giải phóng bộ nhớ của vertexNode
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

    // (b) Thêm tất cả các đỉnh vào đồ thị
    for (int i = 0; i < nvertices; ++i) {
        graph->add(vertices[i]);
    }

    // (c) Thêm tất cả các cạnh vào đồ thị
    for (int i = 0; i < nedges; ++i) {
        graph->connect(edges[i].from, edges[i].to, edges[i].weight);
    }

    // (d) Trả về con trỏ đến đồ thị được tạo
    return graph;
    }
};

#endif /* UGRAPHMODEL_H */
