#include "../unit_test.hpp"

bool UNIT_TEST_Sort_Togo::sort_topo05() {
  string name = "sort_topo05";
  stringstream output;
  //! data ------------------------------------
    DLinkedListSE<int> data;
    data.add(1);
    data.add(2);
    data.add(0);
    data.add(3);
    data.add(3);
    data.add(8);
    data.add(9);
    // data.sort();
  //! expect ----------------------------------
  string expect = "";

  //! output ----------------------------------
 

  //! remove data -----------------------------

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
