#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>

TEST_CASE("test_001")
{
    AVLTree<int, std::string> tree;

    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(5, "five");

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    tree.printTreeStructure();
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();

    CHECK(output == "  ten    \n five  twenty  \n");
}

TEST_CASE("test_002")
{
    AVLTree<int, std::string> tree;

    tree.insert(1, "1");
    tree.insert(2, "2");
    tree.insert(3, "3");

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    tree.printTreeStructure();
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();

    CHECK(output == "  2    \n 1  3  \n");
}

// TODO