#include <stdio.h>
#include <stdlib.h>

#define T 3

// struct btree represents a node directly
struct btree {
    int *keys;
    int t;
    struct btree **c;
    int n;
    int leaf; // 1 if leaf, 0 if internal node
};

// Function Prototypes
void printtree(struct btree *tree);
struct btree *creatnode(int t, int leaf);
void splitchild(struct btree *parent, int i, struct btree *left);
void insertnotfull(struct btree *root, int key);
struct btree *insert(struct btree *root, int key);

// Deletion Prototypes
int findkey(struct btree *node, int key);
int getpred(struct btree *node, int idx);
int getsucc(struct btree *node, int idx);
void borrowprev(struct btree *node, int idx);
void borrownext(struct btree *node, int idx);
void merge(struct btree *node, int idx);
void fill(struct btree *node, int idx);
void removefromleaf(struct btree *node, int idx);
void removefromnonleaf(struct btree *node, int idx);
void removefromnode(struct btree *node, int key);
struct btree *delete(struct btree *root, int key);

// Print tree in order
void printtree(struct btree *tree) {
    if (tree == NULL) return;
    int i;
    for (i = 0; i < tree->n; i++) {
        if (!tree->leaf)
            printtree(tree->c[i]);

        printf("%d ", tree->keys[i]);
    }
    if (!tree->leaf)
        printtree(tree->c[i]);
}

// Create node
struct btree *creatnode(int t, int leaf) {
    struct btree *newnode = malloc(sizeof(struct btree));
    newnode->c = malloc(sizeof(struct btree *) * (T * 2));
    newnode->keys = malloc(sizeof(int) * (T * 2 - 1));
    newnode->n = 0;
    newnode->leaf = leaf;
    newnode->t = T;
    return newnode;
}

// Split child
void splitchild(struct btree *parent, int i, struct btree *left) {
    struct btree *right = creatnode(T, left->leaf);
    right->n = T - 1;

    for (int j = 0; j < T - 1; j++) {
        right->keys[j] = left->keys[j + T];
    }

    if (!left->leaf) {
        for (int j = 0; j < T; j++) {
            right->c[j] = left->c[j + T];
        }
    }
    left->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--) {
        parent->c[j + 1] = parent->c[j];
    }
    parent->c[i + 1] = right;

    // Fixed bug here: was j++ in your draft, needs to decrement j-- when shifting right
    for (int j = parent->n - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = left->keys[T - 1];
    parent->n++;
}

// Insert into non-full node
void insertnotfull(struct btree *root, int key) {
    int j = root->n - 1;
    if (root->leaf) {
        while (j >= 0 && root->keys[j] > key) {
            root->keys[j + 1] = root->keys[j];
            j--;
        }
        root->keys[j + 1] = key;
        root->n++;
    } else {
        while (j >= 0 && root->keys[j] > key) {
            j--;
        }
        j++;
        if (root->c[j]->n == T * 2 - 1) {
            splitchild(root, j, root->c[j]);

            if (root->keys[j] < key) {
                j++;
            }
        }
        insertnotfull(root->c[j], key);
    }
}

// Main insert call
struct btree *insert(struct btree *root, int key) {
    if (root == NULL) {
        root = creatnode(T, 1);
        root->n = 1;
        root->keys[0] = key;
        return root;
    } else {
        if (root->n == T * 2 - 1) {
            struct btree *parent = creatnode(T, 0);
            parent->c[0] = root;

            splitchild(parent, 0, root);

            int j = 0;
            if (parent->keys[j] < key) {
                j++;
            }
            insertnotfull(parent->c[j], key);
            return parent;
        }
        insertnotfull(root, key);
        return root;
    }
}

// --- DELETION HELPER FUNCTIONS ---

// Find index of first key greater than or equal to key
int findkey(struct btree *node, int key) {
    int idx = 0;
    while (idx < node->n && node->keys[idx] < key) {// finds key inside a node
        idx++;
    }
    return idx;
}

// Get predecessor key (rightmost key in left subtree)
int getsucc(struct btree *node, int idx) {
    struct btree *curr = node->c[idx]; 
    while (!curr->leaf) { // finds a fitting child to switch with
        curr = curr->c[curr->n];
    }
    return curr->keys[curr->n - 1];
}

// Get successor key (leftmost key in right subtree)
int getprev(struct btree *node, int idx) {
    struct btree *curr = node->c[idx + 1]; // same
    while (!curr->leaf) {
        curr = curr->c[0];
    }
    return curr->keys[0];
}

// Borrow a key from left sibling c[idx-1]
void borrowprev(struct btree *node, int idx) {
    struct btree *child = node->c[idx];
    struct btree *sibling = node->c[idx - 1];

    for (int i = child->n - 1; i >= 0; i--) { // giving space
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--) { // moving child if not leaf
            child->c[i + 1] = child->c[i];
        }
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->leaf) {
        child->c[0] = sibling->c[sibling->n]; // moving grandchild if leaf
    }

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];// moving the replacemnt -p

    child->n += 1;//updates-|
    sibling->n -= 1; //     |
}

// Borrow a key from right sibling c[idx+1]
void borrownext(struct btree *node, int idx) {
    struct btree *child = node->c[idx];
    struct btree *sibling = node->c[idx + 1]; // same as borrow from pref but idx +1 

    child->keys[child->n] = node->keys[idx];

    if (!child->leaf) {
        child->c[child->n + 1] = sibling->c[0];
    }

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i <= sibling->n; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; i++) {
            sibling->c[i - 1] = sibling->c[i];
        }
    }

    child->n += 1;
    sibling->n -= 1;
}

// Merge node's child c[idx] with c[idx+1]
void merge(struct btree *node, int idx) {
    struct btree *child = node->c[idx]; // left
    struct btree *sibling = node->c[idx + 1]; // right

    child->keys[T - 1] = node->keys[idx]; // send the parent node down a s a middle node

    for (int i = 0; i < sibling->n; i++) { // merge
        child->keys[i + T] = sibling->keys[i];// moving right child keys into left after parent pass the target node
    }

    if (!child->leaf) {// if not leaf
        for (int i = 0; i <= sibling->n; i++) {
            child->c[i + T] = sibling->c[i];// also passing the childrens
        }
    }

    for (int i = idx + 1; i < node->n; i++) { // shifting keys to give space for the target replacment
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = idx + 2; i <= node->n; i++) { // we remove a child so we would have less children so we push to the left
        node->c[i - 1] = node->c[i];
    }

    child->n += sibling->n + 1; // combining the lengh of both children
    node->n--; // update node

    free(sibling->keys);
    free(sibling->c);
    free(sibling);
}

// Fill child c[idx] if it has fewer than T keys
void fill(struct btree *node, int idx) {
    if (idx != 0 && node->c[idx - 1]->n >= T) { // if left have enugh keys we borrow
        borrowprev(node, idx);
    } else if (idx != node->n && node->c[idx + 1]->n >= T) { // same with right
        borrownext(node, idx);
    } else {
        if (idx != node->n) {
            merge(node, idx);// if index not the last we merge with the sibling bigger then the child
        } else {
            merge(node, idx - 1); // else we must merge with a smaller sibling
        }
    }
}

// Remove key from leaf node
void removefromleaf(struct btree *node, int idx) {
    for (int i = idx + 1; i < node->n; i++) {// shifting the keys
        node->keys[i - 1] = node->keys[i];
    }
    node->n--; // update after removal
}

// Remove key from internal (non-leaf) node
void removefromnonleaf(struct btree *node, int idx) {
    int key = node->keys[idx]; // key

    if (node->c[idx]->n >= T) {// checks if w ecan borrow from smaller child node
        int pred = getpred(node, idx); // getting the closest key to the target if the biggest key on the left
        node->keys[idx] = pred; // switch if with the deleted key
        removefromnode(node->c[idx], pred); // delet the key from the child node since they swap places
    } else if (node->c[idx + 1]->n >= T) { // if left child dont have enough keys we check on right
        int succ = getsucc(node, idx); // getting the closest key to the target the smallest key on the right
        node->keys[idx] = succ; // we swap with  the child
        removefromnode(node->c[idx + 1], succ); // delete
    } else {
        merge(node, idx); // if both left and right nodes dont have enough keys we merge them together
        removefromnode(node->c[idx], key); //the remove from the new merged child that swap the target with the closest key
    }
}

// Internal recursive deletion
void removefromnode(struct btree *node, int key) {
    int idx = findkey(node, key); // find the index of the key or the child

    if (idx < node->n && node->keys[idx] == key) {// checks if the key is in the current node
        if (node->leaf) {
            removefromleaf(node, idx);// remove 
        } else {
            removefromnonleaf(node, idx);//remove 
        }
    } else {
        if (node->leaf) { // if the node is a leaf and we didnt find it it means it is not on the tree
            printf("\nKey %d not found in tree!", key);
            return;
        }

        int flag = (idx == node->n);// if child is last

        if (node->c[idx]->n < T) {// not enough children 
            fill(node, idx);
        }

        if (flag && idx > node->n) {// fill delete a key from the node so now if idx is bigger the condision fills
            removefromnode(node->c[idx - 1], key); // we remove from the node we pass the target to
        } else {
            removefromnode(node->c[idx], key); // if a leaf wasnt merge 
        }
    }
}

// Main delete call: returns the new root pointer back to main
struct btree *delete(struct btree *root, int key) {
    if (root == NULL) { // checks if tree empty
        printf("\nTree is empty!");
        return NULL; //reset the tree
    }

    removefromnode(root, key); //sent to remove 

    // If root key count drops to 0, update root to its first child or NULL
    if (root->n == 0) {
        struct btree *tmp = root;
        if (root->leaf) {
            root = NULL;
        } else {
            root = root->c[0];
        }
        free(tmp->keys);
        free(tmp->c);
        free(tmp);
    }

    return root;
}

int main() {
    struct btree *tree = NULL;

    // Insertion
    tree = insert(tree, 7);
    tree = insert(tree, 21);
    tree = insert(tree, 9);
    tree = insert(tree, 48);
    tree = insert(tree, 1);
    tree = insert(tree, 17);
    tree = insert(tree, 84);
    tree = insert(tree, 36);
    tree = insert(tree, 27);
    tree = insert(tree, 52);

    printf("Tree after insertions:\n");
    printtree(tree);
    printf("\n\n");

    // Deletion testing
    printf("Deleting 17...\n");
    tree = delete(tree, 17);
    printtree(tree);
    printf("\n\n");

    printf("Deleting 48...\n");
    tree = delete(tree, 48);
    printtree(tree);
    printf("\n\n");

    printf("Deleting 7...\n");
    tree = delete(tree, 7);
    printtree(tree);
    printf("\n");

    return 0;
}

// ============================================================================
// B-TREE DELETION CALL HIERARCHY
// ============================================================================
//
// removeKey(tree, key)  [Entry Point]
// │
// ├── traverse(node)  (Optional: debug/print tree)
// │
// └── removeFromNode(node, key)  [Recursive Core Deletion]
//     │
//     ├── findKey(node, key)  [Finds key index 'idx' in current node]
//     │
//     ├── IF (key is found in this node):
//     │   │
//     │   ├── IF (node is Leaf):
//     │   │   └── removeFromLeaf(node, idx)
//     │   │
//     │   └── IF (node is Non-Leaf):
//     │       └── removeFromNonLeaf(node, idx)
//     │           ├── getPredecessor(node, idx)
//     │           ├── getSuccessor(node, idx)
//     │           ├── removeFromNode(child, pred/succ)  (Recursive call)
//     │           └── merge(node, idx)
//     │
//     └── IF (key is NOT in this node):
//         │
//         ├── fill(node, idx)  [Ensures target child has >= t keys before entry]
//         │   ├── borrowFromPrev(node, idx)
//         │   ├── borrowFromNext(node, idx)
//         │   └── merge(node, idx)
//         │
//         └── removeFromNode(child, key)  (Recursive call down to child)
//
// ============================================================================