// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const {
    int height = this->getHeight();
		if (this->root == NULL)
		{
			cout << "NULL\n";
			return;
		}
		queue<AVLNode *> q;
		q.push(root);
		AVLNode *temp;
		int count = 0;
		int maxNode = 1;
		int level = 0;
		int space = pow(2, height);
		printNSpace(space / 2);
		while (!q.empty())
		{
			temp = q.front();
			q.pop();
			if (temp == NULL)
			{
				cout << " ";
				q.push(NULL);
				q.push(NULL);
			}
			else
			{
				cout << temp->data;
				q.push(temp->pLeft);
				q.push(temp->pRight);
			}
			printNSpace(space);
			count++;
			if (count == maxNode)
			{
				cout << endl;
				count = 0;
				maxNode *= 2;
				level++;
				space /= 2;
				printNSpace(space / 2);
			}
			if (level == height)
				return;
		}
}


//TODO: Implement all AVLTree<K, T> methods here
template<class K, class T> 
typename AVLTree<K,T> ::AVLNode*  AVLTree<K,T>::rotateRight(AVLNode*& node) {

	if (node==nullptr||node->pLeft==nullptr) return node; 

	AVLNode* leftChild=node->pLeft; 
	node->pLeft=leftChild->pRight; 
	leftChild->pRight=node; 

	if (leftChild->balance == LH) {
		// Trường hợp cây con trái của leftChild cao hơn
		node->balance = EH;
		leftChild->balance = EH;
	}
	else { // leftChild->balance == EH
		// Trường hợp cây cân bằng
		node->balance = LH;
		leftChild->balance = RH;
	}

	return leftChild; 
}

template<class K,class T>
typename AVLTree<K,T>:: AVLNode* AVLTree<K,T>:: rotateLeft(AVLNode*& node) {

	if (node==nullptr||node->pRight==nullptr) return node; 

	AVLNode* rightChild=node->pRight; 
	node->pRight=rightChild->pLeft; 
	rightChild->pLeft=node; 

	if (rightChild->balance==RH) {
		node->balance=EH; 
		rightChild->balance = EH;
	}

	else {
        node->balance = RH;
        rightChild->balance = LH;
    }

	return rightChild; 
}


template<class K, class T> 
void AVLTree<K,T>:: clearHelper(AVLNode* node) {
	if (node == nullptr) return;
    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template<class K, class T> 
AVLTree<K,T>::AVLTree() {
	this->root=nullptr; 
}

template<class K, class T> 
AVLTree<K,T>::~AVLTree() {
	clearHelper(root); 
	this->root=nullptr;
}

template<class K, class T> 
int AVLTree<K,T>:: height(AVLNode* n) const {
if (n==nullptr) return 0; 
int hl = height(n->pLeft);
int hr = height(n->pRight);
return 1 + (hl > hr ? hl : hr);
}

template<class K, class T> 
int AVLTree<K,T>:: getHeight() const {
return height(root);  
}


template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::insertRec(AVLNode* node, const K& key, const T& value) {
    // 1. Chèn như BST
    if (node == nullptr)
        return new AVLNode(key, value);

    if (key < node->key)
        node->pLeft = insertRec(node->pLeft, key, value);
    else if (key > node->key)
        node->pRight = insertRec(node->pRight, key, value);
    else
        return node; // Không chèn trùng key

    // 2. Tính hệ số cân bằng
    int balance = getBalance(node);

    // 3. Kiểm tra 4 trường hợp mất cân bằng

    // Case 1: Left Left
    if (balance > 1 && key < node->pLeft->key)
        return rotateRight(node);

    // Case 2: Right Right
    if (balance < -1 && key > node->pRight->key)
        return rotateLeft(node);

    // Case 3: Left Right
    if (balance > 1 && key > node->pLeft->key) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // Case 4: Right Left
    if (balance < -1 && key < node->pRight->key) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    // 4. Nếu không mất cân bằng, trả node hiện tại
    return node;
}


template <class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value) {
    root = insertRec(root, key, value);
}

template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::findMin(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->pLeft != nullptr)
        current = current->pLeft;
    return current;
}

template<class K, class T>
int AVLTree<K, T>::getBalance (AVLNode* node) {
    if (node == nullptr) return 0;
    return height(node->pLeft) - height(node->pRight);
}
 

template<class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeRec(AVLNode* node, const K& key) {
    if (node == nullptr)
        return node;

    // --- 1. Xóa như BST ---
    if (key < node->key)
        node->pLeft = removeRec(node->pLeft, key);
    else if (key > node->key)
        node->pRight = removeRec(node->pRight, key);
    else {
        // Node có 1 hoặc 0 con
        if (node->pLeft == nullptr || node->pRight == nullptr) {
            AVLNode* temp = node->pLeft ? node->pLeft : node->pRight;

            if (temp == nullptr) { // không có con
                temp = node;
                node = nullptr;
            } else { // có 1 con
                *node = *temp;
            }
            delete temp;
        } else {
            // Node có 2 con
            AVLNode* temp = findMin(node->pRight);
            node->key = temp->key;
            node->data = temp->data;
            node->pRight = removeRec(node->pRight, temp->key);
        }
    }

    if (node == nullptr)
        return node;

    // --- 2. Tính lại balance factor ---
    int balance = getBalance(node);

    // --- 3. 4 Trường hợp mất cân bằng sau khi xóa ---

    // Left Left
    if (balance > 1 && getBalance(node->pLeft) >= 0)
        return rotateRight(node);

    // Left Right
    if (balance > 1 && getBalance(node->pLeft) < 0) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // Right Right
    if (balance < -1 && getBalance(node->pRight) <= 0)
        return rotateLeft(node);

    // Right Left
    if (balance < -1 && getBalance(node->pRight) > 0) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    return node;
}

template<class K, class T> 
 void AVLTree<K,T>:: remove(const K& key) {
	 root=removeRec(root, key); 
 }

template<class K, class T> 
bool AVLTree<K,T>:: contains(const K& key) const {
	AVLNode* current=root; 
	while (current) {
		if (key==current->key) {
			return true; 
		}
		if (key<current->key) current=current->pLeft; 
		else current=current->pRight; 
	}
	return false; 
}

template<class K, class T>
int AVLTree<K,T>:: countNodes(AVLNode* node) const {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->pLeft) + countNodes(node->pRight);
    }

template<class K, class T> 
int AVLTree<K,T>:: getSize() const {
return countNodes(root); 
}

template<class K, class T>
bool AVLTree<K,T>:: empty() const {
return (root==nullptr|| getSize()==0); 
}

template<class K, class T>
void AVLTree<K,T>:: clear() {
clearHelper(root);
    root = nullptr;
}

template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLNode* node, void (*action)(const T&)) const {
    if (node == nullptr) return;
    inorderHelper(node->pLeft, action);   // 1. Duyệt cây con trái
    action(node->data);                   // 2. Xử lý node hiện tại
    inorderHelper(node->pRight, action);  // 3. Duyệt cây con phải
}

template<class K, class T> 
void AVLTree<K,T>:: inorderTraversal(void (*action)(const T&)) const {
	inorderHelper(root,action); 
}



// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const {
    if (this->root == nullptr) {
        cout << "NULL\n";
        return;
    }
    
    queue<RBTNode*> q;
    q.push(root);
    RBTNode* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    
    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode*> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()) {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++) {
            RBTNode* node = heightQ.front();
            heightQ.pop();
            if (node->left) heightQ.push(node->left);
            if (node->right) heightQ.push(node->right);
        }
    }
    
    int space = pow(2, height);
    printNSpace(space / 2);
    
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        
        if (temp == nullptr) {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        } else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            } else {
                cout << "B)";
            }
            
            q.push(temp->left);
            q.push(temp->right);
        }
        
        printNSpace(space);
        count++;
        
        if (count == maxNode) {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        
        if (level == height) {
            return;
        }
    }
}

//TODO: Implement all other RedBlackTree<K, T> methods here
template <class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K &key, const T &value)
    : key(key), data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr)
{
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed()
{
    this->color = RED;
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack()
{
    this->color = BLACK;
}

// =====================================
// Rotation methods implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode* node)
{
    if (node == nullptr || node->right == nullptr) return;
    
    RBTNode* rightChild = node->right;
    
    // Step 1: rightChild's left becomes node's right
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }
    
    // Step 2: Update parent pointers
    rightChild->parent = node->parent;
    
    if (node->parent == nullptr) {
        // node was root, now rightChild becomes root
        root = rightChild;
    } else if (node == node->parent->left) {
        // node was left child
        node->parent->left = rightChild;
    } else {
        // node was right child
        node->parent->right = rightChild;
    }
    
    // Step 3: node becomes left child of rightChild
    rightChild->left = node;
    node->parent = rightChild;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode* node)
{
    if (node == nullptr || node->left == nullptr) return;
    
    RBTNode* leftChild = node->left;
    
    // Step 1: leftChild's right becomes node's left
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }
    
    // Step 2: Update parent pointers
    leftChild->parent = node->parent;
    
    if (node->parent == nullptr) {
        // node was root, now leftChild becomes root
        root = leftChild;
    } else if (node == node->parent->left) {
        // node was left child
        node->parent->left = leftChild;
    } else {
        // node was right child
        node->parent->right = leftChild;
    }
    
    // Step 3: node becomes right child of leftChild
    leftChild->right = node;
    node->parent = leftChild;
}

// =====================================
// Bound methods implementation
// =====================================

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBoundNode(const K& key) const
{
    RBTNode* current = root;
    RBTNode* result = nullptr;
    
    while (current != nullptr) {
        if (key == current->key) {
            // Found exact match, this is the lower bound
            return current;
        } else if (key < current->key) {
            // Current node is greater than key, potential candidate
            result = current;
            current = current->left;
        } else {
            // Current node is less than key, move to right subtree
            current = current->right;
        }
    }
    
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBoundNode(const K& key) const
{
    RBTNode* current = root;
    RBTNode* result = nullptr;
    
    while (current != nullptr) {
        if (key < current->key) {
            // Current node is greater than key, potential candidate
            result = current;
            current = current->left;
        } else {
            // Current node is less than or equal to key, move to right subtree
            current = current->right;
        }
    }
    
    return result;
}


//====================================
//Constructor, destructor
//====================================
template <class K, class T>
RedBlackTree<K, T>::RedBlackTree() : root(nullptr)
{
}

template <class K, class T>
RedBlackTree<K, T>::~RedBlackTree()
{
    clear();
}



template <class K, class T> 
bool RedBlackTree<K,T>:: empty() const {
return root==nullptr; 
}

// =====================================
// Size and Clear methods implementation
// =====================================

template <class K, class T>
int RedBlackTree<K, T>::size() const
{
    if (root == nullptr) {
        return 0;
    }
    
    std::queue<RBTNode*> q;
    q.push(root);
    int count = 0;
    
    while (!q.empty()) {
        RBTNode* current = q.front();
        q.pop();
        count++;
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    
    return count;
}

template <class K, class T>
void RedBlackTree<K, T>::clear()
{
    if (root == nullptr) {
        return;
    }
    
    std::queue<RBTNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        RBTNode* current = q.front();
        q.pop();
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
        
        delete current;
    }
    
    root = nullptr;
}

// =====================================
// Insert and balancing implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::insert(const K& key, const T& value)
{
    // Create new node with RED color
    RBTNode* newNode = new RBTNode(key, value);
    
    // Standard BST insertion
    if (root == nullptr) {
        // Case 1: Tree is empty, new node becomes root
        root = newNode;
        newNode->recolorToBlack(); // Root must be black
        return;
    }
    
    // Find insertion position
    RBTNode* current = root;
    RBTNode* parent = nullptr;
    
    while (current != nullptr) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key already exists, do nothing (as per specification)
            delete newNode;
            return;
        }
    }
    
    // Insert new node
    newNode->parent = parent;
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    // Fix Red-Black Tree violations
    insertFixup(newNode);
}

template <class K, class T>
void RedBlackTree<K, T>::insertFixup(RBTNode* node)
{
    while (node != root && node->parent->color == RED) {
        RBTNode* parent = node->parent;
        RBTNode* grandparent = parent->parent;
        
        // Case A: Parent is left child of grandparent
        if (parent == grandparent->left) {
            RBTNode* uncle = grandparent->right;
            
            // Case 1: Uncle is RED (recoloring)
            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent; // Move up to grandparent
            } else {
                // Case 2: Node is right child (triangle case)
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                
                // Case 3: Node is left child (line case)
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateRight(grandparent);
            }
        } 
        // Case B: Parent is right child of grandparent (symmetric)
        else {
            RBTNode* uncle = grandparent->left;
            
            // Case 1: Uncle is RED (recoloring)
            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent; // Move up to grandparent
            } else {
                // Case 2: Node is left child (triangle case)
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                
                // Case 3: Node is right child (line case)
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateLeft(grandparent);
            }
        }
    }
    
    // Ensure root is black (Case 1 from specification)
    root->recolorToBlack();
}



//=================================
//DELETE AND FIXUP
//=================================

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::maximum(RBTNode* node) 
{
    while (node != nullptr && node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template <class K, class T>
void RedBlackTree<K, T>::transplant(RBTNode* u, RBTNode* v)
{
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::deleteFixup(RBTNode* x, RBTNode* xParent) {
    while (x != root && (x == nullptr || x->color == BLACK)) {
        if (x == xParent->left) {
            RBTNode* w = xParent->right;
            
            // Case 1: sibling is RED
            if (w != nullptr && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                rotateLeft(xParent);
                w = xParent->right;
            }
            
            // Case 2: both sibling's children are BLACK
            if ((w == nullptr || w->left == nullptr || w->left->color == BLACK) &&
                (w == nullptr || w->right == nullptr || w->right->color == BLACK)) {
                if (w != nullptr) {
                    w->color = RED;
                }
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3: sibling's left child is RED, right child is BLACK
                if (w != nullptr && (w->right == nullptr || w->right->color == BLACK)) {
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                    if (w != nullptr) {
                        w->color = RED;
                    }
                    rotateRight(w);
                    w = xParent->right;
                }
                
                // Case 4: sibling's right child is RED
                if (w != nullptr) {
                    w->color = xParent->color;
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                }
                xParent->color = BLACK;
                rotateLeft(xParent);
                x = root;
            }
        } else {
            // Symmetric case (x is right child)
            RBTNode* w = xParent->left;
            
            // Case 1: sibling is RED
            if (w != nullptr && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                rotateRight(xParent);
                w = xParent->left;
            }
            
            // Case 2: both sibling's children are BLACK
            if ((w == nullptr || w->left == nullptr || w->left->color == BLACK) &&
                (w == nullptr || w->right == nullptr || w->right->color == BLACK)) {
                if (w != nullptr) {
                    w->color = RED;
                }
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3: sibling's right child is RED, left child is BLACK
                if (w != nullptr && (w->left == nullptr || w->left->color == BLACK)) {
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                    if (w != nullptr) {
                        w->color = RED;
                    }
                    rotateLeft(w);
                    w = xParent->left;
                }
                
                // Case 4: sibling's left child is RED
                if (w != nullptr) {
                    w->color = xParent->color;
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                }
                xParent->color = BLACK;
                rotateRight(xParent);
                x = root;
            }
        }
    }
    
    if (x != nullptr) {
        x->color = BLACK;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::remove(const K& key)
{
    RBTNode* z = find(key);
    if (z == nullptr) return;
    
    RBTNode* y = z;
    RBTNode* x = nullptr;
    RBTNode* xParent = nullptr;  // ← Thêm biến này
    Color y_original_color = y->color;
    
    if (z->left == nullptr) {
        x = z->right;
        xParent = z->parent;  // ← Lưu parent
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        xParent = z->parent;  // ← Lưu parent
        transplant(z, z->left);
    } else {
        y = maximum(z->left);
        y_original_color = y->color;
        x = y->left;
        xParent = y;  // ← Lưu parent
        
        if (y->parent == z) {
            if (x != nullptr) x->parent = y;
            xParent = y;
        } else {
            xParent = y->parent;
            transplant(y, y->left);
            y->left = z->left;
            y->left->parent = y;
        }
        
        transplant(z, y);
        y->right = z->right;
        if (y->right != nullptr) {
            y->right->parent = y;
        }
        y->color = z->color;
    }
    
    delete z;
    
    if (y_original_color == BLACK) {
        deleteFixup(x, xParent);  // ← Truyền thêm xParent
    }
}


//=================================
//OTHERS 
//=================================
template <class K, class T> 
typename RedBlackTree<K,T>::RBTNode * RedBlackTree<K,T>:: find(const K &key) const {
RBTNode* current = root;
    
    while (current != nullptr) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K,T>:: contains(const K &key) const {
return (find(key)!=nullptr); 
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBound(const K& key, bool& found) const
{
    RBTNode* result = lowerBoundNode(key);
    found = (result != nullptr);
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBound(const K& key, bool& found) const
{
    RBTNode* result = upperBoundNode(key);
    found = (result != nullptr);
    return result;
}





// =====================================
// VectorRecord implementation
// =====================================


// Overload operator << for VectorRecord - prints only the id
std::ostream& operator<<(std::ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================


//TODO: Implement all VectorStore methods here
VectorStore::VectorStore(int dimension, vector<float>* (*embeddingFunction)(const string&), const vector<float>& referenceVector) 
: dimension(dimension), embeddingFunction(embeddingFunction), count(0), averageDistance(0.0), rootVector(nullptr)
{
    vectorStore= new AVLTree<double, VectorRecord>(); 
    normIndex= new RedBlackTree<double, VectorRecord>(); 
    this->referenceVector = new vector<float>(referenceVector);
}

VectorStore::~VectorStore(){
    clear(); 
    delete vectorStore; 
    delete normIndex; 
    delete referenceVector; 
}

int VectorStore::size() {
    return count; 
}

bool VectorStore:: empty() {
return (size()==0); 
}

void VectorStore:: clear() {

}

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;



