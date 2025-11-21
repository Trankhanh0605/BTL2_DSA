#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>
#include <cmath>
#include <iomanip>
#include "helper.h"
#include <algorithm>

TEST_CASE("test_301")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    for (int i = 0; i <= 10; ++i)
    {
        store.addText(std::to_string(i));
    }

    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output ==
              "---\n"
              "        4                \n"
              "    2        8        \n"
              "  1    3    6    10    \n"
              "             5  7  9  11  \n"
              "---\n"
              "                (4,B)                                \n"
              "        (2,B)                (6,B)                \n"
              "    (1,B)        (3,B)        (5,B)        (8,R)        \n"
              "  NULL    NULL    NULL    NULL    NULL    NULL    (7,B)    (10,B)    \n"
              " NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  NULL  (9,R)  (11,R)  \n"
              "---");
    }

    {
        int *rawResult = store.rangeQueryFromRoot(2, 8);
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 7);
            delete[] rawResult;
        }
        std::sort(result.begin(), result.end());
        const std::vector<int> expected = {3, 4, 5, 6, 7, 8, 9};
        for (int i = 0; i < 7; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        int *rawResult = store.rangeQuery({5}, 2, "l2");
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 5);
            delete[] rawResult;
        }
        std::sort(result.begin(), result.end());
        const std::vector<int> expected = {4, 5, 6, 7, 8};
        for (int i = 0; i < 5; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        int *rawResult = store.boundingBoxQuery({5}, {11});
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 6);
            delete[] rawResult;
        }
        std::sort(result.begin(), result.end());
        const std::vector<int> expected = {6, 7, 8, 9, 10, 11};
        for (int i = 0; i < 6; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        CHECK(store.getMaxDistance() == 10);
        CHECK(store.getMinDistance() == 0);
    }

    {
        vector<VectorRecord *> list_VectorRecord = store.getAllVectorsSortedByDistance();
        VectorRecord center = store.computeCentroid(list_VectorRecord);

        CHECK((*center.vector)[0] == 5);
        delete center.vector;
    }
}

TEST_CASE("test_302")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    for (int i = 0; i <= 10; ++i)
    {
        store.addText(std::to_string(i));
    }

    int id = store.findNearest({6.2}, "l1");
    CHECK(id == 7);

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    int *ids = store.topKNearest({5}, 5, "l1");
    for (int i = 0; i < 4; i++)
        cout << ids[i];
    delete[] ids;
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();
    CHECK(output == "Value m: 4\n6578");
}
