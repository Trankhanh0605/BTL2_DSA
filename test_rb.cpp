#include "doctest/doctest.h"
#include "src/VectorStore.h"

TEST_CASE("test_101")
{
    RedBlackTree<int, string> rbt;
    // http://www.rnlkwc.ac.in/pdf/study-material/comsc/Design.pdf

    // Insert 8
    rbt.insert(8, "8");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == " (8,B)  \n");
    }

    // Insert 18
    rbt.insert(18, "18");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (8,B)    \n NULL  (18,R)  \n");
    }

    // Insert 5
    rbt.insert(5, "5");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (8,B)    \n (5,R)  (18,R)  \n");
    }

    // Insert 15
    rbt.insert(15, "15");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (8,B)        \n  (5,B)    (18,B)    \n NULL  NULL  (15,R)  NULL  \n");
    }

    // Insert 17
    rbt.insert(17, "17");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (8,B)        \n  (5,B)    (17,B)    \n NULL  NULL  (15,R)  (18,R)  \n");
    }

    // Insert 25
    rbt.insert(25, "25");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (8,B)                \n    (5,B)        (17,R)        \n  NULL    NULL    (15,B)    (18,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (25,R)  \n");
    }

    // Insert 40
    rbt.insert(40, "40");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (8,B)                \n    (5,B)        (17,R)        \n  NULL    NULL    (15,B)    (25,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  (18,R)  (40,R)  \n");
    }

    //  Insert 80
    rbt.insert(80, "80");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (17,B)                \n    (8,R)        (25,R)        \n  (5,B)    (15,B)    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }
}

TEST_CASE("test_102")
{
    RedBlackTree<int, string> rbt;
    rbt.insert(8, "8");
    rbt.insert(18, "18");
    rbt.insert(5, "5");
    rbt.insert(15, "15");
    rbt.insert(17, "17");
    rbt.insert(25, "25");
    rbt.insert(40, "40");
    rbt.insert(80, "80");

    rbt.remove(5);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (17,B)                \n    (8,B)        (25,R)        \n  NULL    (15,R)    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(17);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (15,B)                \n    (8,B)        (25,R)        \n  NULL    NULL    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(18);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (15,B)        \n  (8,B)    (40,R)    \n NULL  NULL  (25,B)  (80,B)  \n");
    }

    rbt.remove(40);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (15,B)        \n  (8,B)    (25,B)    \n NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(15);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (25,B)    \n (8,B)  (80,B)  \n");
    }
}
