/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0){
        //TODO
        // Kiểm tra nếu đỉnh from và to có tồn tại
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        if (!fromNode) {
            throw VertexNotFoundException(this->vertex2str(from)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'from'
        }
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if (!toNode) {
            throw VertexNotFoundException(this->vertex2str(to)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'to'
        }
        
        // Thêm cạnh vào đồ thị
       fromNode->connect(toNode,weight);
    }
    void disconnect(T from, T to){
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
    typename AbstractGraph<T>::Edge* edgeToRemove = fromNode->getEdge(toNode);
    if (!edgeToRemove) {
        typename AbstractGraph<T>::Edge edge1(fromNode, toNode);
        throw EdgeNotFoundException(this->edge2Str(edge1));  
    }

    // (c) Xóa cạnh này khỏi đồ thị
    fromNode->removeTo(toNode);  // Xóa cạnh từ fromNode đến toNode
    delete edgeToRemove;         // Giải phóng bộ nhớ của cạnh
    }
   void remove(T vertex) {
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
        if (toNode) {
            // Xóa cạnh này từ đỉnh nguồn
            typename AbstractGraph<T>::Edge* edgeToRemove = vertexNode->getEdge(toNode);
            if (edgeToRemove) {
                vertexNode->removeTo(toNode); // Xóa cạnh từ vertexNode (đỉnh nguồn)
                delete edgeToRemove; // Giải phóng bộ nhớ của cạnh
            }
        }
        ++outIt;
    }

    // Duyệt qua danh sách các đỉnh mà có cạnh đi vào vertexNode và xóa các cạnh này
    DLinkedList<T> inwardEdges = this->getInwardEdges(vertex);
    typename DLinkedList<T>::Iterator inIt = inwardEdges.begin();
    while (inIt != inwardEdges.end()) {
        // Lấy đỉnh nguồn mà có cạnh đi vào vertexNode
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(*inIt);
        if (fromNode) {
            // Lấy cạnh cần xóa giữa fromNode và vertexNode
            typename AbstractGraph<T>::Edge* edgeToRemove = fromNode->getEdge(vertexNode);
            if (edgeToRemove) {
                fromNode->removeTo(vertexNode); // Xóa cạnh từ fromNode (đỉnh nguồn) đến vertexNode
                delete edgeToRemove; // Giải phóng bộ nhớ của cạnh
            }
        }
        ++inIt;
    }

    // Sau khi đã xóa hết các cạnh, xóa đỉnh khỏi đồ thị
    this->nodeList.removeItem(vertexNode); // Loại bỏ vertexNode khỏi danh sách các đỉnh trong đồ thị

    // Giải phóng bộ nhớ của vertexNode
    delete vertexNode; // Giải phóng bộ nhớ của vertexNode
}
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
         // (a) Khởi tạo một đối tượng DGraphModel mới.
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);
        // (b) Thêm tất cả các đỉnh trong vertices vào đồ thị.
        for (int i = 0; i < nvertices; ++i) {
            graph->add(vertices[i]);  // Giả sử có hàm addVertex() để thêm đỉnh vào đồ thị
        }
         // (d) Thêm tất cả các cạnh vào đồ thị.
        for (int i = 0; i < nedges; ++i) {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
            // Giả sử connect() thêm một cạnh với trọng số vào đồ thị.
        }
        // (e) Trả về con trỏ đến đồ thị đã khởi tạo.
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */

