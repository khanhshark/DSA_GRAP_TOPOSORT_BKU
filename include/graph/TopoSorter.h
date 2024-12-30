/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "hash/xMap.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
template <class T, class V>
using XHashMap = xMap<T, V>;

template <class T>
class TopoSorter
{
public:
  static int DFS;
  static int BFS;

protected:
  DGraphModel<T> *graph;
  int (*hash_code)(T &, int);

public:
  TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
  {
    this->graph = graph;
    this->hash_code = hash_code;
  }
  DLinkedList<T> sort(int mode = 0, bool sorted = true)
  {
    return (mode == BFS) ? bfsSort(sorted) : dfsSort(sorted);
  }
  DLinkedList<T> bfsSort(bool sorted = true)
  {
    // TODO
    // Kết quả của thuật toán
    DLinkedList<T> topoOrder;
    // HashMap để lưu trữ độ vào (in-degree) của mỗi đỉnh
    XHashMap<T, int> inDegree(hash_code);
    // Bước 1: Sử dụng hàm có sẵn để lấy độ vào của từng đỉnh
    inDegree = vertex2inDegree(hash_code);
    // Bước 2: Lấy danh sách các đỉnh có độ vào bằng 0
    DLinkedListSE<T> zeroInDegree = listOfZeroInDegrees();
    if (sorted)
    {
      zeroInDegree.sort();
    }
    // Khởi tạo hàng đợi (Queue)
    Queue<T> queue;

    // Đưa các đỉnh có độ vào bằng 0 vào hàng đợi
    for (T vertex : zeroInDegree)
    {
      queue.push(vertex);
    }

    // Bước 3: BFS - Duyệt qua các đỉnh
    while (!queue.empty())
    {
      T current = queue.pop(); // Lấy đỉnh đầu tiên từ hàng đợi
      topoOrder.add(current);  // Thêm đỉnh vào kết quả

      // Duyệt qua các đỉnh láng giềng
      DLinkedListSE<T> neighbors = graph->getOutwardEdges(current);

      // Giảm độ vào của các đỉnh láng giềng và thêm vào hàng đợi nếu độ vào = 0
      for (T neighbor : neighbors)
      {
        inDegree.put(neighbor, inDegree.get(neighbor) - 1);
        if (inDegree.get(neighbor) == 0)
        {
          queue.push(neighbor); // Thêm vào hàng đợi nếu độ vào bằng 0
        }
      }
    }

    return topoOrder;
  }

  DLinkedList<T> dfsSort(bool sorted = true)
  {
    // Kết quả của thuật toán
    DLinkedList<T> topoOrder;
    // HashMap để đánh dấu các đỉnh đã thăm
    XHashMap<T, bool> visited(hash_code);
    // Lấy danh sách các đỉnh từ đồ thị
    DLinkedListSE<T> vertices = this->listOfZeroInDegrees();
    // Nếu cần, sắp xếp danh sách đỉnh
    if (sorted)
    {
      vertices.sort();
    }
    DLinkedListSE<T> hash = graph->vertices();
    // Đánh dấu tất cả các đỉnh là chưa thăm
    for (T vertex : hash)
    {
      visited.put(vertex, false);
    }
    // Stack để hỗ trợ duyệt DFS
    Stack<T> stack;
    // Lặp qua tất cả các đỉnh
    for (T vertex : vertices)
    {
      if (!visited.get(vertex))
      {
        stack.push(vertex);
        visited.put(vertex, true);
      

      while (!stack.empty())
      {
        T current = stack.peek();
        //! sắp xếp các đỉnh
        DLinkedListSE<T> out = graph->getOutwardEdges(current);

        int checkout = 0;
        for (T vertexOut : out)
        {
          if (!visited.get(vertexOut))
          {
           
            stack.push(vertexOut);
            visited.put(vertexOut, true);
            checkout++;
            break;
          }
        }
        if (checkout == 0)
        {
          stack.pop();
          topoOrder.add(0, current);
        }
      }
      }
    }
    return topoOrder;
  }

protected:
  // Helper functions
  XHashMap<T, int> vertex2inDegree(int (*hash)(T &, int))
  {
    // Lấy danh sách các đỉnh trong đồ thị
    DLinkedList<T> vertices = graph->vertices();
    // Khởi tạo bảng băm với hàm băm
    XHashMap<T, int> result(hash);

    // Duyệt qua tất cả các đỉnh trong đồ thị
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      // Lưu bậc ra của từng đỉnh vào bảng băm
      result.put(*it, graph->inDegree(*it));
    }

    // Trả về bảng băm chứa bậc ra của tất cả các đỉnh
    return result;
  }

  XHashMap<T, int> vertex2outDegree(int (*hash)(T &, int))
  {
    // Lấy danh sách các đỉnh trong đồ thị
    DLinkedList<T> vertices = graph->vertices();
    // Khởi tạo bảng băm với hàm băm
    XHashMap<T, int> result(hash);

    // Duyệt qua tất cả các đỉnh trong đồ thị
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      // Lưu bậc ra của từng đỉnh vào bảng băm
      result.put(*it, graph->outDegree(*it));
    }

    // Trả về bảng băm chứa bậc ra của tất cả các đỉnh
    return result;
  }

  DLinkedList<T> listOfZeroInDegrees()
  {
    DLinkedList<T> result;                       // Khởi tạo danh sách liên kết đôi để chứa các đỉnh có bậc vào bằng 0
    DLinkedList<T> vertices = graph->vertices(); // Lấy danh sách các đỉnh trong đồ thị

    // Duyệt qua tất cả các đỉnh trong đồ thị
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      if (graph->inDegree(*it) == 0)
      {                  // Kiểm tra nếu bậc vào của đỉnh bằng 0
        result.add(*it); // Thêm đỉnh vào danh sách kết quả
      }
    }

    return result; // Trả về danh sách các đỉnh có bậc vào bằng 0
  }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
