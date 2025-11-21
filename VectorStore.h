#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#pragma once
// NOTE: Per assignment rules, only this single include is allowed here.
#include "main.h"

// ------------------------------
// AVL balance enum
// ------------------------------
enum BalanceValue
{
    LH = -1, // Left Higher
    EH = 0,  // Equal Height
    RH = 1   // Right Higher
};

// ------------------------------
// Generic AVL Tree (template)
// ------------------------------
template <class K, class T>
class AVLTree {
    friend class VectorStore; // Allow VectorStore to access protected/private members
    
    public:
        class AVLNode {
        public:

            int height; 

            K key;
            T data;
            AVLNode* pLeft;
            AVLNode* pRight;
            BalanceValue balance;

            AVLNode(const K& key, const T& value)
                : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(EH) {}
                
            friend class VectorStore; // Allow VectorStore to access AVLNode members
        };

    protected:
        AVLNode* root;

        AVLNode* rotateRight(AVLNode*& node);
        AVLNode* rotateLeft(AVLNode*& node);
        void clearHelper(AVLNode* node);

    public:
        AVLTree();
        ~AVLTree();

        //===================THEM VAO DE TU THEM CHU KHONG TU XOAY======
        AVLNode* buildBalancedTree(const std::vector<std::pair<K, T>>& sortedPairs, int left, int right); 
        //=======================================

        //===================time limit=========
        int getHeight(AVLNode* node) const; 
        void updateHeight(AVLNode* node); 
        //======================================

        AVLNode* insertRec(AVLNode* node, const K& key, const T& value);
        void insert(const K& key, const T& value);

        int getBalance(AVLNode* node); 

        AVLNode* findMin(AVLNode* node); 

        AVLNode* removeRec(AVLNode* node, const K& key); 

        void remove(const K& key);


        bool contains(const K& key) const;

        int height(AVLNode* node) const; 
        int getHeight() const;

        int countNodes(AVLNode* node) const; 

        int getSize() const;
        bool empty() const;
        void clear();

        void printTreeStructure() const;

        void inorderHelper(AVLNode* node, void (*action)(const T&)) const; 

        void inorderTraversal(void (*action)(const T&)) const;

        AVLNode* getRoot() const { return root; }
};

enum Color
{
    RED,
    BLACK
};

// RedBlackTree class
template <class K, class T>
class RedBlackTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    // RBTNode class

    class RBTNode
    {
    public:
        K key;
        T data;
        Color color;
        RBTNode *parent;
        RBTNode *left;
        RBTNode *right;

        // Constructor
        RBTNode(const K &key, const T &value);

        void recolorToRed();
        void recolorToBlack();

        friend class VectorStore; // Allow VectorStore to access RBTNode members
    };

private:
    RBTNode *root;

protected:
    void rotateLeft(RBTNode *node);
    void rotateRight(RBTNode *node);

    RBTNode *lowerBoundNode(const K &key) const;
    RBTNode *upperBoundNode(const K &key) const;

public:
    RedBlackTree();
    ~RedBlackTree();

    bool empty() const;
    int size() const;
    void clear();
    
    //---------------------------------------
    void insert(const K &key, const T &value);

    void insertFixup(RBTNode* node); 
    //---------------------------------------


    //----------REMOVE SUPPPORTERS ----------
    RBTNode* maximum(RBTNode* node) ;

    void transplant(RBTNode* u, RBTNode* v); 

    void deleteFixup(RBTNode* x, RBTNode* xParent); 
    void remove(const K &key);
    //----------------------------------------
    

    RBTNode *find(const K &key) const;
    bool contains(const K &key) const;

    RBTNode *lowerBound(const K &key, bool &found) const;
    RBTNode *upperBound(const K &key, bool &found) const;

    void printTreeStructure() const;
};

// ------------------------------
// VectorRecord
// ------------------------------
class VectorRecord
{
public:
    int id;
    std::string rawText;
    int rawLength;
    std::vector<float> *vector;
    double distanceFromReference;
    double norm;

    VectorRecord()
        : id(-1), rawLength(0), vector(nullptr), distanceFromReference(0.0), norm(0.0) {}

    VectorRecord(int _id,
                 const std::string &_rawText,
                 std::vector<float> *_vec,
                 double _dist,
                 double _norm = 0)
        : id(_id),
          rawText(_rawText),
          rawLength(static_cast<int>(_rawText.size())),
          vector(_vec),
          distanceFromReference(_dist),
          norm(_norm) {}

//---------ADD-------------
// Copy constructor
VectorRecord(const VectorRecord& other) {
        id = other.id;
        rawText = other.rawText;
        rawLength = other.rawLength;
        distanceFromReference = other.distanceFromReference;
        norm = other.norm;
        
        // Deep copy cho vector - CODE ĐÚNG
    if (other.vector != nullptr) {
        vector = new std::vector<float>(*other.vector);
    } else {
        vector = nullptr;
    }
}

//==============ADD=============
 ~VectorRecord() {
        if (vector != nullptr) {
            delete vector;
            vector = nullptr;
        }
}


    // Overload operator << to print only the id
    friend std::ostream &operator<<(std::ostream &os, const VectorRecord &record);
};

// ------------------------------
// VectorStore
// ------------------------------
class VectorStore
{
private:
    AVLTree<double, VectorRecord> *vectorStore;
    RedBlackTree<double, VectorRecord> *normIndex;

    std::vector<float> *referenceVector;
    VectorRecord *rootVector;

    int dimension;
    int count;
    double averageDistance;

    //============ADD==============
    std::vector<VectorRecord> allRecords;
    int nextId;
    //================================

     //============ADD-in advance method==============
    double minDistanceFromRef;
    double maxDistanceFromRef;
    //================================

    std::vector<float> *(*embeddingFunction)(const std::string &);

    double distanceByMetric(const std::vector<float> &a,
                            const std::vector<float> &b,
                            const std::string &metric) const;

    //================ADD FUNCTION==============
    void buildAVLFromSortedRecords(const std::vector<VectorRecord*>& sortedRecords, VectorRecord* newRoot, int left, int right);

    void buildBalancedAVL(const std::vector<VectorRecord*>& sortedRecords, int left, int right);
//=============================================
    void rebuildRootIfNeeded();
    void rebuildTreeWithNewRoot(VectorRecord *newRoot);

    VectorRecord *findVectorNearestToDistance(double targetDistance) const;


    //=============ADD FUNCTION INLINE=============
    static bool compareByDistance(const VectorRecord* a, const VectorRecord* b);
    //=================================

public:
    VectorStore(int dimension,
                std::vector<float> *(*embeddingFunction)(const std::string &),
                const std::vector<float> &referenceVector);
    ~VectorStore();

    void test_vectorStore() { vectorStore->printTreeStructure(); }
    void test_normIndex() { normIndex->printTreeStructure(); }

    int size();
    bool empty(); 
    void clear();

    std::vector<float> *preprocessing(std::string rawText);

    //=============SUPPLEMENT===============
    double calculateNorm(const vector<float>& vec) const; 
    void addText(std::string rawText);

    //========SUPPORT FUNCTION=========== 
    VectorRecord* findInorderIndex(AVLTree<double, VectorRecord>::AVLNode* node, int& currentIndex, int targetIndex); 
    // Helper để tìm node tại vị trí index theo thứ tự inorder (0-based)
    AVLTree<double, VectorRecord>::AVLNode* findInorderIndex(AVLTree<double, VectorRecord>::AVLNode* node, int& currentIndex, int targetIndex) const;
    //===================================
    VectorRecord *getVector(int index);
    
    std::string getRawText(int index);
    int getId(int index);

    //=============SUPPORT=======================
    const VectorRecord* findNearestToTargetConst(AVLTree<double, VectorRecord>::AVLNode* node, double target, double& minDiff, const VectorRecord* candidate) const; 
    //======================================
    bool removeAt(int index);

    void setReferenceVector(const std::vector<float> &newReference);
    std::vector<float> *getReferenceVector() const;
    VectorRecord *getRootVector() const;
    double getAverageDistance() const;
    void setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &));

    void forEach(void (*action)(std::vector<float> &, int, std::string &));
    std::vector<int> getAllIdsSortedByDistance() const;

    //======================SUPPORT FUNC=============
    void getAllVectorsHelper(AVLTree<double, VectorRecord>::AVLNode* node, std::vector<VectorRecord*>& vec) const; 

    //===============================================
    std::vector<VectorRecord *> getAllVectorsSortedByDistance() const;

    double cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2) const;
    double l1Distance(const std::vector<float> &v1, const std::vector<float> &v2) const;
    double l2Distance(const std::vector<float> &v1, const std::vector<float> &v2) const;

    double estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias = 1e-9, double c1_slope = 0.05);

    //================ADD FUNCTION=================
    const double EPSILON = 1e-9;
    //=============================================
    int findNearest(const std::vector<float> &query, std::string metric = "cosine");

//===============FUNCTION SORT-MERGE SORT===============
// Hàm merge hai mảng con đã sắp xếp
void merge(std::vector<std::pair<double, int>>& arr, int left, int mid, int right);
// Hàm Merge Sort chính
void mergeSort(std::vector<std::pair<double, int>>& arr, int left, int right);

// Thêm mới cho VectorRecord*
    void mergeVectorRecords(std::vector<VectorRecord*>& arr, int left, int mid, int right) const;
    void mergeSortVectorRecords(std::vector<VectorRecord*>& arr, int left, int right) const;

//===========================================================
    int *topKNearest(const std::vector<float> &query, int k, std::string metric = "cosine");


    //=============RANGEQUERY HELPER FUNCTION=================
    void rangeQueryHelper(AVLTree<double, VectorRecord>::AVLNode* node,double minDist, double maxDist, std::vector<int>& result) const; 
    //========================================================
    int *rangeQueryFromRoot(double minDist, double maxDist) const;
    int *rangeQuery(const std::vector<float> &query, double radius, std::string metric = "cosine") const;
    int *boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const;

    //========================ADD ADVANCED==================
    void recalculateMinMaxDistance();
    //======================================================
    double getMaxDistance() const;
    double getMinDistance() const;
    VectorRecord computeCentroid(const std::vector<VectorRecord *> &records) const;


    void updatenextId(); 
};

#endif // VECTORSTORE_H
