
#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>
#include <cmath>
#include <iomanip>
#include "helper.h"
#include <algorithm>

TEST_CASE("test_201")
{
    std::vector<float> refVec = {0.0f, 0.0f};
    VectorStore store(2, simpleEmbedding, refVec);

    std::vector<float> v1 = {1.0f, 0.0f};
    std::vector<float> v2 = {0.0f, 1.0f};

    double dist = store.l2Distance(v1, v2);
    CHECK(std::abs(dist - std::sqrt(2.0)) < 1e-6);
}

TEST_CASE("test_202")
{
    std::vector<float> refVec = {0.0f, 0.0f};
    VectorStore store(2, simpleEmbedding, refVec);

    std::vector<float> v1 = {1.0f, 1.0f};
    std::vector<float> v2 = {4.0f, 5.0f};

    double dist = store.l1Distance(v1, v2);
    CHECK(dist == 7.0);
}

TEST_CASE("test_203")
{
    std::vector<float> refVec = {0.0f, 0.0f};
    VectorStore store(2, simpleEmbedding, refVec);

    std::vector<float> v1 = {-1.0f, 1.0f};
    std::vector<float> v2 = {1.0f, -1.0f};

    double sim = store.cosineSimilarity(v1, v2);
    CHECK(std::abs(sim + 1.0) < 1e-6);
}

TEST_CASE("test_204")
{
    std::vector<float> refVec = {0.0f, 0.0f};
    VectorStore store(2, simpleEmbedding, refVec);

    std::vector<float> query = {3.0f, 4.0f};
    std::vector<float> reference = {1.0f, 1.0f};

    // averageDistance = 5.0
    double averageDistance = 5.0;
    int k = 2;
    double c0_bias = 1e-9;
    double c1_slope = 0.05;

    double estimatedD = store.estimateD_Linear(query, k, averageDistance, reference, c0_bias, c1_slope);
    CHECK(doctest::Approx(1.89445).epsilon(1e-6) == estimatedD);
}

TEST_CASE("test_204")
{
    std::vector<float> refVec = {0.0f, 0.0f};
    VectorStore store(2, simpleEmbedding, refVec);

    std::vector<float> query = {3.0f, 4.0f};
    std::vector<float> reference = {1.0f, 1.0f};

    // averageDistance = 5.0
    double averageDistance = 5.0;
    int k = 2;
    double c0_bias = 1e-9;
    double c1_slope = 0.05;

    double estimatedD = store.estimateD_Linear(query, k, averageDistance, reference, c0_bias, c1_slope);
    CHECK(doctest::Approx(1.89445).epsilon(1e-6) == estimatedD);
}

TEST_CASE("test_205")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    store.addText("0"); // root

    CHECK(printVectorRecord(store.getRootVector()) == "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00");

    store.addText("1"); // root
    CHECK(printVectorRecord(store.getRootVector()) == "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00");
    CHECK(store.getAverageDistance() == 0.5);

    {
        std::vector<string> expected = {"ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00",
                                        "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00"};
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 2; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }
    }

    store.addText("2"); // root
    CHECK(printVectorRecord(store.getRootVector()) == "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00");
    CHECK(store.getAverageDistance() == 1);

    {
        std::vector<string> expected = {"ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00",
                                        "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00",
                                        "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=2.00, Norm=2.00"};
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 3; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }
    }

    store.addText("10"); // root
    CHECK(printVectorRecord(store.getRootVector()) == "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00");
    CHECK(store.getAverageDistance() == 3.25);

    {
        std::vector<string> expected = {
            "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00",
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=2.00, Norm=2.00",
            "ID=4, RawText=10, RawLength=2, Vector=[10.00], Distance=10.00, Norm=10.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }
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

        CHECK(output == "---\n    2        \n  1    3    \n          4  \n---\n    (2,B)        \n  (1,B)    (3,B)    \n NULL  NULL  NULL  (4,R)  \n---");
    }

    store.addText("3");
    CHECK(printVectorRecord(store.getRootVector()) == "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00");
    CHECK(store.getAverageDistance() == 3.2);
    {
        std::vector<string> expected = {
            "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00",
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=2.00, Norm=2.00",
            "ID=5, RawText=3, RawLength=1, Vector=[3.00], Distance=3.00, Norm=3.00",
            "ID=4, RawText=10, RawLength=2, Vector=[10.00], Distance=10.00, Norm=10.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 5; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }
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

        CHECK(output == "---\n"
                        "    2        \n"
                        "  1    5    \n"
                        "       3  4  \n"
                        "---\n"
                        "    (2,B)        \n"
                        "  (1,B)    (5,B)    \n"
                        " NULL  NULL  (3,R)  (4,R)  \n---");
    }
}

TEST_CASE("test_206")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    store.addText("0");  // 0
    store.addText("1");  // 1
    store.addText("2");  // 2
    store.addText("10"); // 3

    CHECK(store.getAverageDistance() == 3.25);
    CHECK(printVectorRecord(store.getRootVector()) == "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00");

    {
        std::vector<string> expected = {
            "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00",
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=1.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=2.00, Norm=2.00",
            "ID=4, RawText=10, RawLength=2, Vector=[10.00], Distance=10.00, Norm=10.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }
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

        CHECK(output == "---\n    2        \n  1    3    \n          4  \n---\n    (2,B)        \n  (1,B)    (3,B)    \n NULL  NULL  NULL  (4,R)  \n---");
    }

    store.setReferenceVector({11.f});
    CHECK(store.getAverageDistance() == 7.75);
    CHECK(printVectorRecord(store.getRootVector()) == "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=9.00, Norm=2.00");
    {
        std::vector<string> expected = {
            "ID=4, RawText=10, RawLength=2, Vector=[10.00], Distance=1.00, Norm=10.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=9.00, Norm=2.00",
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=10.00, Norm=1.00",
            "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=11.00, Norm=0.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "---\n"
                        "    3        \n"
                        "  4    2    \n"
                        "          1  \n"
                        "---\n"
                        "    (2,B)        \n"
                        "  (1,B)    (3,B)    \n"
                        " NULL  NULL  NULL  (4,R)  \n"
                        "---");
    }
}

TEST_CASE("test_207")
{
    std::vector<float> refVec = {-1.0f};
    VectorStore store(1, simpleEmbedding, refVec);
    store.addText("0");
    store.addText("1");
    store.addText("2");
    store.addText("3");
    store.addText("4");
    CHECK(printVectorRecord(store.getRootVector()) == "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=2.00, Norm=1.00");
    CHECK(store.getAverageDistance() == 3);
    {
        std::vector<string> expected = {
            "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=1.00, Norm=0.00",
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=2.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00",
            "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00",
            "ID=5, RawText=4, RawLength=1, Vector=[4.00], Distance=5.00, Norm=4.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 5; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "---\n"
                        "    2        \n"
                        "  1    4    \n"
                        "       3  5  \n"
                        "---\n"
                        "    (2,B)        \n"
                        "  (1,B)    (4,B)    \n"
                        " NULL  NULL  (3,R)  (5,R)  \n---");
    }

    store.removeAt(0);
    CHECK(printVectorRecord(store.getRootVector()) == "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00");
    CHECK(store.getAverageDistance() == 3.5);
    {
        std::vector<string> expected = {
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=2.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00",
            "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00",
            "ID=5, RawText=4, RawLength=1, Vector=[4.00], Distance=5.00, Norm=4.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "---\n"
                        "    4        \n"
                        "  2    5    \n"
                        "    3        \n"
                        "---\n"
                        "    (4,B)        \n"
                        "  (2,B)    (5,B)    \n"
                        " NULL  (3,R)  NULL  NULL  \n"
                        "---");
    }

    store.removeAt(3);
    CHECK(printVectorRecord(store.getRootVector()) == "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00");
    CHECK(store.getAverageDistance() == 3.0);
    {
        std::vector<string> expected = {
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=2.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00",
            "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 3; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "---\n"
                        "  3    \n"
                        " 2  4  \n"
                        "---\n"
                        "  (3,B)    \n"
                        " (2,B)  (4,B)  \n"
                        "---");
    }

    store.addText("-10");
    CHECK(printVectorRecord(store.getRootVector()) == "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00");
    CHECK(store.getAverageDistance() == 4.5);
    {
        std::vector<string> expected = {
            "ID=2, RawText=1, RawLength=1, Vector=[1.00], Distance=2.00, Norm=1.00",
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00",
            "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00",
            "ID=5, RawText=-10, RawLength=3, Vector=[-10.00], Distance=9.00, Norm=10.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();
        CHECK(output == "---\n"
                        "    3        \n"
                        "  2    4    \n"
                        "          5  \n"
                        "---\n"
                        "    (3,B)        \n"
                        "  (2,B)    (4,B)    \n"
                        " NULL  NULL  NULL  (5,R)  \n"
                        "---");
    }

    store.removeAt(0);
    CHECK(printVectorRecord(store.getRootVector()) == "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00");
    {
        std::vector<string> expected = {
            "ID=3, RawText=2, RawLength=1, Vector=[2.00], Distance=3.00, Norm=2.00",
            "ID=4, RawText=3, RawLength=1, Vector=[3.00], Distance=4.00, Norm=3.00",
            "ID=5, RawText=-10, RawLength=3, Vector=[-10.00], Distance=9.00, Norm=10.00",
        };
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 3; ++i)
        {
            CHECK(printVectorRecord(sorted[i]) == expected[i]);
        }

        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        cout << "---" << endl;
        store.test_vectorStore();
        cout << "---" << endl;
        store.test_normIndex();
        cout << "---";
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();
        CHECK(output == "---\n"
                        "  4    \n"
                        " 3  5  \n"
                        "---\n"
                        "  (4,B)    \n"
                        " (3,B)  (5,B)  \n"
                        "---");
    }

    store.removeAt(0);
    store.removeAt(0);
    store.removeAt(0);
    CHECK(store.getRootVector() == nullptr);
    CHECK(store.getAverageDistance() == 0.0);

    store.addText("-1");
    CHECK(printVectorRecord(store.getRootVector()) == "ID=1, RawText=-1, RawLength=2, Vector=[-1.00], Distance=0.00, Norm=1.00");

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    cout << "---" << endl;
    store.test_vectorStore();
    cout << "---" << endl;
    store.test_normIndex();
    cout << "---";
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();
    CHECK(output == "---\n"
                    " 1  \n"
                    "---\n"
                    " (1,B)  \n"
                    "---");
}

TEST_CASE("test_208")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    store.addText("0"); // root
    CHECK(printVectorRecord(store.getRootVector()) == "ID=1, RawText=0, RawLength=1, Vector=[0.00], Distance=0.00, Norm=0.00");

    store.setEmbeddingFunction(simpleEmbedding2);
    store.addText("1"); // root
    CHECK(printVectorRecord(store.getVector(1)) == "ID=2, RawText=1, RawLength=1, Vector=[2.00], Distance=2.00, Norm=2.00");
}
