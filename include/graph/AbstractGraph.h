/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;


template<class T>
class AbstractGraph: public IGraph<T>{
public:
    class Edge; //forward declaration
    class VertexNode; //forward declaration
    class Iterator; //forward declaration
    
private:
protected:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    //!Lưu trữ danh sách các đỉnh (vertex) trong đồ thị.
    DLinkedList<VertexNode*> nodeList; 
    
    //Function pointers:
    //! Con trỏ hàm so sánh hai đỉnh. Dùng để xác định hai đỉnh có "bằng nhau" không.
    bool (*vertexEQ)(T&, T&); //to compare two vertices
    
    string (*vertex2str)(T&); //to obtain string representation of vertices
    
    
    VertexNode* getVertexNode(T& vertex){
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while(it != nodeList.end()){
            VertexNode* node = *it;
            if(vertexEQ(node->vertex, vertex) ) return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode& node){
        return vertex2str(node.vertex);
    }
    string edge2Str(Edge& edge){
        stringstream os;
        os << "E("
                << vertex2str(edge.from->vertex)
                << ","
                << vertex2str(edge.to->vertex)
                << ")";
        return os.str();
    }
    
public:
    AbstractGraph(
            bool (*vertexEQ)(T&, T&)=0, 
            string (*vertex2str)(T&)=0){
        
        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph(){
        //TODO
    }
    
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);
    vertexEQFunc getVertexEQ(){
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str(){
        return this->vertex2str;
    }

    
//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight=0) =0;
    virtual void disconnect(T from, T to)=0;
    virtual void remove(T vertex)=0;
    
    
    
    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) {
        //TODO
        if(getVertexNode(vertex) != nullptr) return;
        VertexNode* newNode = new VertexNode(vertex,vertexEQ,vertex2str);
        nodeList.add(newNode);
    }
    virtual bool contains(T vertex){
        //TODO
    // Kiểm tra nếu đồ thị có chứa đỉnh với giá trị vertex
    typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
    while (it != nodeList.end()) {
        VertexNode* node = *it;
        if (vertexEQ(node->vertex, vertex)) {
            return true;
        }
        it++;
    }
    return false;
    }
    virtual float weight(T from, T to) {
      
    // Tìm đỉnh từ 'from' và 'to'
    VertexNode* fromNode = getVertexNode(from);
    if (!fromNode) {
        throw VertexNotFoundException(vertex2str(from)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'from'
    }
    VertexNode* toNode = getVertexNode(to);
    if (!toNode) {
        throw VertexNotFoundException(vertex2str(to)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'to'
    }
    // Tìm cạnh giữa đỉnh 'from' và 'to'
    Edge* edge = fromNode->getEdge(toNode);
     
    if (!edge) {
        // Nếu không tìm thấy cạnh, tạo đối tượng Edge và ném ngoại lệ
        Edge edge1(fromNode, toNode);
        throw EdgeNotFoundException(edge2Str(edge1));
        
    }
    // Nếu tìm thấy cạnh, trả về trọng số của cạnh
    return edge->weight;
}
    virtual DLinkedList<T> getOutwardEdges(T from){
        //TODO     
         // Tìm VertexNode tương ứng với đỉnh 'from'
    VertexNode* fromNode = getVertexNode(from);
    
    // Kiểm tra nếu không tìm thấy đỉnh 'from', ném ngoại lệ
    if (!fromNode) {
        
      
        throw VertexNotFoundException(vertex2str(from)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'from'
    } 
    // Nếu đỉnh 'from' tồn tại, lấy danh sách các cạnh đi ra từ đỉnh này
    return fromNode->getOutwardEdges();
    }
    
    virtual DLinkedList<T>  getInwardEdges(T to){
        //TODO
        // Tìm VertexNode tương ứng với đỉnh 'to'
    VertexNode* toNode = getVertexNode(to);
    // Kiểm tra nếu không tìm thấy đỉnh 'to', ném ngoại lệ
    if (!toNode) {
        throw VertexNotFoundException(vertex2str(to)); // Ném ngoại lệ nếu không tìm thấy đỉnh 'to'
    }
    // Khởi tạo danh sách trống để lưu các đỉnh có cạnh đi vào đỉnh 'to'
    DLinkedList<T> list;
    
    // Duyệt qua tất cả các đỉnh trong nodeList
    typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
    while (it != nodeList.end()) {
        VertexNode* node = *it;
        
        // Duyệt qua các cạnh kề của đỉnh 'node'
        typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
        while (edgeIt != node->adList.end()) {
            Edge* edge = *edgeIt;
            // Nếu cạnh có đỉnh đích là 'to', thêm đỉnh nguồn vào danh sách
            if ( vertexEQ(edge->to->vertex, to) ) {
                list.add(node->vertex); // Thêm đỉnh nguồn vào danh sách
                //break;
            }
            
            edgeIt++;
        }
        it++;
    }
    // Trả về danh sách các đỉnh có cạnh đi vào đỉnh 'to'
    return list;
    }
    
    virtual int size() {
        //TODO
        return nodeList.size(); // nodeList là danh sách lưu trữ các đỉnh trong đồ thị
    }
    virtual bool empty(){
        //TODO
          return size() == 0; 
    };
    virtual void clear(){
        //TODO
     typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
    while (it != nodeList.end()) {
        VertexNode* node = *it;
        it++;  // Tiến tới phần tử tiếp theo trước khi xóa phần tử hiện tại
        delete node; // Giải phóng bộ nhớ cho đỉnh
    }
    nodeList.clear(); // Xóa tất cả các đỉnh trong danh sách
    }
    virtual int inDegree(T vertex){
        //TODO
    // Duyệt qua tất cả các đỉnh và kiểm tra bậc vào
        VertexNode* toNode = getVertexNode(vertex);
        if(toNode != NULL) return toNode->inDegree();
        throw VertexNotFoundException(vertex2str(vertex)); 
    }
    virtual int outDegree(T vertex){
        //TODO
         VertexNode* toNode = getVertexNode(vertex);
        if(toNode != NULL) return toNode->outDegree();
     throw VertexNotFoundException(vertex2str(vertex)); 
    }
    
    virtual DLinkedList<T> vertices(){
        //TODO
         // Tạo một danh sách để chứa các đỉnh
        DLinkedList<T> allVertices;

    // Duyệt qua tất cả các đỉnh trong nodeList và thêm vào danh sách allVertices
    for (auto it = nodeList.begin(); it != nodeList.end(); ++it) {
        allVertices.add((*it)->getVertex());
    }

    // Trả về danh sách tất cả các đỉnh
    return allVertices;
    }
    virtual bool connected(T from, T to){
        //TODO
        // Tìm đỉnh từ và đỉnh đến trong đồ thị
    VertexNode* fromNode = getVertexNode(from);
    VertexNode* toNode = getVertexNode(to);

    // Nếu một trong hai đỉnh không tồn tại, ném ngoại lệ VertexNotFoundException
    if (fromNode == nullptr) {
        throw VertexNotFoundException(vertex2str(from));
    }
    if (toNode == nullptr) {
        throw VertexNotFoundException(vertex2str(to));
    }

    // Kiểm tra xem có cạnh từ đỉnh from đến đỉnh to hay không
    return fromNode->getEdge(toNode) != nullptr;
    }
    void println(){
        cout << this->toString() << endl;
    }
    virtual string toString(){
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
//BEGIN of VertexNode    
    class VertexNode{   
    private:
        template<class U>
        friend class UGraphModel; //UPDATED: added
       
        T vertex;
        int inDegree_, outDegree_;
        // Danh sách liên kết đôi lưu các cạnh kết nối từ đỉnh hiện tại.
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ)(T&, T&);// Con trỏ hàm so sánh hai đỉnh.
        string (*vertex2str)(T&);
        
    public:
        VertexNode():adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){}
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&))
            :adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }
        T& getVertex(){
            return vertex;
        }
        void connect(VertexNode* to, float weight=0){
            //TODO
            Edge* edge = this->getEdge(to);
            if(edge){
                edge->weight = weight;
                return;
            }
            adList.add(new Edge(this, to, weight));
            this->outDegree_++;
            to->inDegree_++;
        }
        DLinkedList<T> getOutwardEdges(){
            //TODO
            DLinkedList<T> edges;
            for (auto it = adList.begin(); it != adList.end(); ++it) {
                edges.add((*it)->to->vertex);
            }
            return edges;
        }

        Edge* getEdge(VertexNode* to){
            //TODO
            for (auto it = adList.begin(); it != adList.end(); ++it) {
            if (vertexEQ((*it)->to->vertex,to->vertex)) {
                return *it;
            }
        }
        return nullptr; // Không tìm thấy cạnh
        }
        bool equals(VertexNode* node){
            //TODO
            return vertexEQ(this->vertex, node->vertex);
        }
        
        void removeTo(VertexNode* to){
            //TODO
            for (auto it = adList.begin(); it != adList.end(); ++it) {
            if (vertexEQ((*it)->to->vertex,to->vertex)) {
                adList.removeItem(*it);
                this->outDegree_--;
                to->inDegree_--;
                return;
            }
        }
        }
        int inDegree(){
            //TODO
            return inDegree_;
        }
        int outDegree(){
            //TODO
            return outDegree_;
        }
        string toString(){
            stringstream os;
            os << "V("
                    << this->vertex << ", "
                    << "in: " << this->inDegree_ << ", "
                    << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge{
    private:
        //Trỏ đến đỉnh bắt đầu của cạnh.
        VertexNode* from;
        //Trỏ đến đỉnh kết thúc của cạnh.
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight=0){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        
        bool equals(Edge* edge){
            //TODO
             // So sánh đỉnh bắt đầu, đỉnh kết thúc, và trọng số
        return this->from->equals(edge->from)  && 
               edge->to->equals(this->to);    
        }

        static bool edgeEQ(Edge*& edge1, Edge*& edge2){
            return edge1->equals(edge2);
        }
        string toString(){
            stringstream os;
            os << "E("
                    << this->from->vertex
                    << ","
                    << this->to->vertex
                    << ","
                    << this->weight
                    << ")";
            return os.str();
        }
    };
 //END of Edge
    

//BEGIN of Iterator
public:
    class Iterator{
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true){
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            }
            else{
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator& operator=(const Iterator& iterator){
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*(){
            return (*nodeIt)->vertex;
        }
        
        bool operator!=(const Iterator& iterator){
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            nodeIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
//END of Iterator
};

#endif /* ABSTRACTGRAPH_H */

