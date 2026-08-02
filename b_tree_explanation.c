#include <stdio.h>
#include <stdlib.h>

#define T 3
// leaf = 1 root = 0
struct btree{
    int * keys;
    int t;
    struct btree ** c;
    int n;
    int leaf;
};
void printtree(struct btree *tree){
    int i;

    for(i = 0; i < tree->n; i++){
        if(!tree->leaf)
            printtree(tree->c[i]);

        printf("%d ", tree->keys[i]);
    }

    if(!tree->leaf)
        printtree(tree->c[i]);
}
struct btree * creatnode(int t , int leaf){
    printf("========= newnode is being created======== \n\n");
    struct btree * newnode = malloc(sizeof(struct btree));
    if(newnode == NULL){
        printf("newnode allocation failed\n\n");
        return NULL;
    }
    else{
        printf("newnode allocation was made\n\n");
    }
    newnode->c = malloc(sizeof(struct btree *) * (t * 2));
    if(newnode->c == NULL){
        printf("new node children allocation has failed\n\n");
        return NULL;
    }
    else{
        printf("new nodw children has been allocated\n\n ");
    }
    newnode->keys = malloc(sizeof(int) * (t * 2 - 1));
    if (newnode->keys == NULL){
        printf(" new node keys allocation has failed\n\n");
        return NULL;
    }
    else{
        printf("new node keys has been allocated\n\n");
    }
    newnode->n = 0;
    printf("newnode->n = 0\n\n");
    newnode->leaf = leaf ;
    if(leaf){
        printf("new node will be a leaf\n\n ");
    }
    else{
        printf(" new node will not be a leaf\n\n");
    }
    newnode->t = T;
    printf("new node t = %d\n\n",T);
    printf("========= node created hase been finished==========\n\n");

    return newnode ;
}
void splitchild(struct btree * parent, int i , struct btree * left){
    printf("=========split child has started============\n\n");
    printf(" we are calling createnode function to");
    struct btree * right = creatnode(T , left->leaf);
    printf(" a right child has been created \n\n");

    right->n = T-1;
    printf("right child-> t == %d/ which is the minimum \n\n",T - 1);

    for(int j =  0 ; j < T-1; j ++){
        right->keys[j] = left->keys[j + T];
        printf("the: %d key of left is now the: %d of the right child\n\n",j + T, j);
    }
     printf(" cheking if left child is not a leaf...\n\n");
    if(!left->leaf){
        printf("left child is not a leaf so we need to move its childern\n\n");
        for ( int j = 0; j < T; j ++){
            right->c[j] = left->c[j + T];
            printf("the %d child of the left child is now the %d child of the right child\n\n ", j + T, j);
        }

        printf("left was not a leaf");
    }
    left->n = T - 1;

    printf(" left->n = %d - 1 since we pass half the keys to right\n\nand since a full node is twice the size of a min node we use this \n\n", T);
    
    printf(" we shift the parents children so they could fit the new right child\n\n");
    for(int j =  parent->n; j >= i + 1; j --){
        parent->c[j + 1] = parent->c[j];
            printf(" parent child number: %d is shifted to number: %d\n\n", j, j + 1);
    }
    printf(" node children has been shiften\n\n");
    parent->c[i + 1] = right;

    printf(" right is being placed in parent->c[%d]\n\n", i + 1);

    printf(" we also need to shift parent keys to clear space for the last key of the left child\n\n");
    for(int j = parent->n -1; j >= i ; j --){
        parent->keys[j +1] = parent->keys[j];

        printf(" parent key number: %d has been shifted to be key numner: %d\n\n",j,j+1);
    }
    printf(" parent keys has been shifted\n\n");

    parent->keys[i] = left->keys[T - 1];

    printf("parent key number %d = left child key number: %d\n\n", i, T -1);

    parent->n++;

    printf(" we increase parent-> n from: %d to : %d\n\n", parent->n - 1, parent->n);

    printf("========= split child has been finished========\n\n");
}
void insertnotfull(struct btree * root, int key){
    printf("========= insert nut full has started ==========\n\n");
    int j = root->n -1;
    printf(" we set j to be the last key index: %d\n\n", j);

    printf("checking if root is a leaf...\n\n");
    if(root->leaf){

        printf(" root is a leaf!\n\n");

        printf("we check what is the correct index for key...\n\n");
        while (j >= 0 && root->keys[j] > key){
            root->keys[j +1] = root->keys[j];
             j --;
        }
        printf(" the correct index is %d\n\n",j);
        root->keys[ j + 1] = key;
        printf("insert the key to %d\n\n", j + 1);
        root->n++;
        printf(" we increament the root->n to %d\n\n", root->n);
    }
    else{
         printf("we check what is the correct index for key child...\n\n");
        while(j >= 0 && root->keys[j] > key){
            j--;
        }
        printf(" the key index need to be adjusted to child index so we add 1\n\n");
        j ++;
        printf("we increament %d to: %d\n\n", j - 1, j);

        printf(" we check if root->c[%d] dont have to much keys keys... \n\n", j);
        if (root->c[j]->n == T * 2 -1){
            printf("it has to much so we split it\n\n");
            splitchild(root, j , root->c[j]);

            printf("since we splited the child node to two we need to see if it gets to the right or left\n\n ");
            if (root-> keys[j] < key){
              printf(" it goes to the right so we need to increament j\n\n");
            j ++;
            }
        }
        printf(" the node dont have the maximum keys amount so we insert\n\n");
        insertnotfull(root->c[j] , key);
      printf("========= insert not full finished===========\n\n");
    }
}
struct btree * insert(struct btree * root, int key){
    printf("==========insert has started=========\n\n");

    printf(" we check if root == NULL...\n\n");
    if(root == NULL){
        printf("root is null so we create root to be a node\n\n");
        root = creatnode(T,1);
        root->n = 1;
        printf("since root is the only node and was just created we know it only have 1 child so root->n = 1\n\n");
        root->keys[0] = key;
        printf(" we insert the key %d into the first key slot since the root is empty\n\n",key);
        printf("=======insert is over========\n\n");
        return root;
    }
    else{
        printf(" root is != NULL\n\n");

        printf("we check if root have maximum amount of children...\n\n");
        if(root->n == T * 2 - 1){
            printf("it does!");
            struct btree * parent = creatnode(T , 0);
            printf(" we created a parent node to root so we could move the middle key to it\n\n");
            parent->c[0] = root;
            printf(" we set root to be the left most child of the parent\n\n");

            splitchild(parent , 0 , root);
            printf(" we split root to give space to insert the key\n\n");

            printf("check if we insert to left or right child of parent...\n\n");
            int j = 0;
            if(parent->keys[j] < key){
                printf("we insert to the right idx 1\n\n");
                j ++;

            }
            if(j == 0){
                printf("we insert to the left child idx 0\n\n");
            }
            printf(" we insert the key %d to child %d", key,j);
            insertnotfull(parent->c[j], key);

            printf("============insert is finished==========\n\n");
            return parent;
        }
        printf(" root have less then maximum keys so we insert\n\n");
        insertnotfull( root , key);
        printf("============insert is finished==========\n\n");
        return root;
    }
}
void deletefromnode(struct btree * node , int key);
int findkey(struct btree *node, int key) {
    printf("======= find key has started========\n\n");
    int idx = 0;
    printf(" checking what idx the key is...");
    while (idx < node->n && node->keys[idx] < key) {// finds key inside a node

        idx++;
        printf("key index : %d\n\n",idx);

    }
    printf("finel key index is: %d\n\n",idx);
    return idx;
    printf("======= find key has finished========\n\n");
}

int getprev(struct btree * node , int idx){
    printf(" ========== get prev has started==========\n\n");

    struct btree * curr = node->c[idx - 1];
    printf(" we need to have the current node and start with the left child: %d\n\n",idx - 1);
    while(!curr->leaf){
        curr = curr->c[curr->n ];
        ("we go down the curr node last child idx: %d\n\n",curr->n);
    }
    printf("======= get prev has finished return key:%d==========\n\n",curr->keys[curr->n-1]);
    return curr->keys[curr->n - 1];
}
int getsucc(struct btree* node , int idx){
    printf("======= get succ has started =========\n\n");
    
    printf(" we need to fund the succ we start with the right child : node->c[%d]\n\n",idx + 1);
    struct btree * curr = node->c[idx + 1];
    while(!curr->leaf){
        printf("we go down to the left most child every time...\n\n");
        curr = curr->c[0];
    }
    printf("get succ returned the closest keu to the target key\n\n");
    printf("=======gett succ has finished=====\n\n");

    return curr->keys[0];
}
void borrowfromprev(struct btree * node , int idx){
    printf("======borrow from prev has started=======\n\n");
    printf(" we need to create the child sibling and its left sibling\n\n ");
    struct btree * child = node->c[idx];
    struct btree * sibling = node->c[idx - 1];
     printf(" we need to shift the child keys to the right to clear room for the parent key\n\n");
    for ( int i = child->n + 1 ; i > 0; i --){
        child->keys[i] = child->keys[i - 1];
        printf("child->keys[%d] now shifted to eb child->keys[%d]\n\n", i - 1, i);
    }

    child->keys[0] = node->keys[idx - 1] ;
    printf("now he have space so child->keys[0] == node->keys->%d\n\n", idx - 1);
    printf(" now we borrow the left  biggest key sibling->key[%d] to be node->keys[%d]\n\n",sibling->keys[sibling->n - 1], idx -1);

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];
    printf("check if child is a leaf...\n\n");

    if (!child->leaf){
        printf(" child is not a leaf so we move left biggest child to it \n\n");
        printf(" we makes space for the left child\n\n");

        for(int i = child->n; i > 0 ; i --){
            child->c[i] = child->c[i - 1];

            printf("child->c[%d] sifted to child->c[%d]\n\n", i - 1, i);
           }
           printf(" after shift sibling biggest child is now child->c[0]\n\n");
           child->c[0] = sibling->c[sibling->n];
} 

child->n ++;
printf(" we increament child->n++ to %d\n\n", child->n);
sibling->n--;
("we decrease sibling->n-- to %d\n\n",sibling->n);
printf("====== borrow from prev has finished =======\n\n");
}
void borrowfromsucc(struct btree * node , int idx){
    printf("=====borrow from next has started=======\n\n");
    printf("we create the child node and its bigger sibling\n\n");
    struct btree * child = node->c[idx];
    struct btree * sibling = node->c[idx + 1];


    child->keys[child->n] = node->keys[idx];

    printf("we set child last key with index : %d to node->keys[%d]\n\n", child->n, idx);
    node->keys[idx] = sibling->keys[0];
    printf("we set node->key[%d] to sibling key:%d\n\n", idx, sibling->keys[0]);
    printf(" we need do shift siblings over the borrow key sibling->keys[0]\n\n");
    for(int i = 1; i <= sibling->n; i ++){
        sibling->keys[i - 1] = sibling->keys[i];
        printf("we shift sibling key : %d to index: %d\n\n", i , i-1);
    }
    printf("we check if child is a leaf...");
    if(!child->leaf){
        printf(" child is not a leaf so we move the child biggest c to sibling smallest");
        child->c[child->n + 1] = sibling->c[0];

        printf(" after moving the first child of sibling we shift the other childern left to delete\n\n");

        for(int i = 1; i <= sibling->n; i ++){

            sibling->c[i - 1] = sibling->c[i]; // delete space
            printf("sibling->c[%d] shidted to sibling->c[%d]\n\n", i , i - 1);
        }

    }

    child->n ++;
    printf(" we increase child->n++ to %d\n\n",child->n);

    sibling->n --;
    printf(" we decrease sibling->n to %d\n\n",sibling->n);

    printf("========borrow from succ is finished=======\n\n");
}
void merge( struct btree * node , int idx){
    printf("======= merge has started======\n\n");
    printf("we need to creaete child and its right bigger sibling\n\n ");
    struct btree * child = node->c[idx];
    struct btree * sibling = node->c[idx + 1];

    child->keys[child->n] = node->keys[idx];
    printf(" we borrow node->keys[%d] and place it in child->keys[%d]\n\n",node->keys[idx],child->keys[child->n]);

    printf(" we need to insetr right sibling keys into child...\n\n");
    for( int i = 0; i < sibling->n; i ++){
        child->keys[i + child->n + 1] = sibling->keys[i];
        printf("child->keys[%d] = sibling->key[%d]\n\n ",i+child->n + 1,i);
    }

    printf(" we check if child is a leaf...\n\n");
    if(!child->leaf){
        printf("child is not a leaf so we move right childern into left\n\n ");
        for(int i = 0; i <= sibling->n; i++){
            child->c[child->n + i + 1] =  sibling->c[i];
            printf("child->c[%d] = sibling->c[%d]\n\n", child->c[child->n + i + 1],sibling->c[i]);
        }
    }
    printf(" we need to shift every key after %d left to delete the parent key\n\n", idx);
    for(int i = idx + 1; i <= node->n ; i ++){
        node->keys[i - 1] = node->keys[i];
        printf("node->keys[%d] = node->keys[%d]\n\n",node->keys[i - 1],node->keys[i]);
    }
     printf(" we also need to shift every child after child->[%d] left since a key is now missing\n\n", idx + 2);
    for(int i = idx + 2; i <= node->n; i++){
        node->c[i - 1] = node->c[i];
        printf("node->c[%d] = node->c[%d]\n\n",node->c[i - 1], node->c[i]);
    }
    child->n = child->n + sibling->n + 1;
    printf(" we set child->n = %d = child->n + sibling->n + 1\n\n ",child->n + sibling->n + 1 );

    node->n --;
    printf(" we decrease node->n by one to %d\n\n",node->n);
    printf(" since every child is dynamicly allocated we must free it...\n\n");

    free(sibling->c);
    printf(" we free sibling->c");

    free(sibling->keys);
    printf(" we free sibling->keys");

    free(sibling);
    printf("we free sibling last becuse the children pointers and keys are also allocated dynamicly so we would lose them if we delete the node first\n\n");
    printf("====== merge has finished======\n\n");


}
void fill(struct btree * node , int idx){
    printf("====== fill has started ======\n\n");
    printf(" if idx is not zero and the node->child->left is bigger then T - 1...\n\n");
    printf(" if left cant borrow we also check if the right child can be borrowed from...\n\n");
if (idx != 0 && node->c[idx - 1]->n > T - 1){
    printf(" the left child can be borrow from\n\n");
    borrowfromprev(node, idx);
}

else if (idx != node->n && node->c[idx + 1]->n > T - 1){
    printf(" left cant be borrowed from but right can\n\n");
    borrowfromsucc(node, idx);
}

    else{
        printf(" both right and left dont have enough keys to borrow from\n\n");

        printf(" we check if idx points to the right most child...\n\n");

        if(idx == node->n){
            printf("it does so me cant merge the last child with the next one so we merge with the prev sibling\n\n");
            merge(node, idx - 1);
        }
        else{
            printf(" it is not so we merge with its right sibling\n\n");
            merge(node,idx);
        }
    }
    printf("====== fill is finished======\n\n");
}
void deletefromleaf(struct btree * node, int idx){
    printf("======= delete from leaf has started======\n\n");

    printf("we check the shift every key after the target key to delete it\n\n");
    while (idx < node->n - 1){
        node->keys[idx] = node->keys[idx + 1];
        printf(" node->key[%d] = node->key[%d]\n\n",idx,idx+1);
        idx++;
    }
    node->n--;
    printf("we decrease node->n-- to: %d\n\n",node->n);

    printf("======= deleye from leaf has finished======\n\n");
    return;
}
 void deletefromnonleaf(struct btree * node, int idx){

    printf("=====delete from non leaf has started======\n\n");
    printf("we need to set a varible for node->keys[idx]\n\n");
    int key = node->keys[idx];

    printf("we check if we can swap the target key with the leftt child\n\n");
    printf(" if we cant swap with prev we will also check wiht the right child\n\n");
    if(node->c[idx]->n > T - 1){
        printf(" we can swap with left child\n\n");
        int prev = getprev(node, idx);
        printf("now we use the prev key to replace in the node at the target place\n\n");
        node->keys[idx] = prev;
        printf(" we call delete on then we delete the prev key from the left child so there wont be duplicates\n\n ");
        deletefromnode(node->c[idx], prev);
    }
    else if ( node->c[idx + 1]->n > T - 1){
        printf(" we couldnt take from the left child so we takes from the right\n\n");
        int succ = getsucc(node, idx);
        node->keys[idx] = succ;
         printf("now we use the succ key to replace in the node at the target place\n\n");

         printf(" we call delete on then we delete the succ key from the right child so there wont be duplicates\n\n ");
        deletefromnode(node->c[idx + 1], succ);
    }
    else{
        printf(" we couldnt take from right and left so we merge them...\n\n");
        merge(node, idx);
        printf("delete from the merged child\n\n");
        deletefromnode(node->c[idx],key);
    }
    printf("======delete from non leaf has finished ======\n\n");
}
void deletefromnode(struct btree * node , int key){
    printf("====== delete from node has started======");
    printf(" we get the index of the child we want to delete by calling find key\n\n");
    int idx = findkey(node,key);

    printf("we check if the index is  not over flowing and if the target is in the node\n\n");
    if (idx < node->n && node->keys[idx] == key){
        printf("it is!\n\n");
        printf("check if node is leaf...\n\n");
        if(node->leaf){
            printf("node is a leaf! so we call delete from leaf\n\n");
            deletefromleaf(node, idx);
        }
        else{
            printf("node is not a leaf! so we call delete from non leaf\n\n");
            deletefromnonleaf(node, idx);
        }
}
else{
    printf(" if the key is not found on the node\n\n");
    printf(" we check if node a leaf...\n");
    if(node->leaf){
    printf("node is leaf! so we can search anymore meaning\n\n");
        printf("key not found\n\n");
        printf("======delete from nod has finishe======");
        return;
    }
    printf("node is not a leaf so we nead to chekc if we can delet from its children\n\n");
    int flag = (idx == node->n); 
    printf(" if(node->c[idx] dont have enough keys we call fill\n\n");
    if(node->c[idx]->n == T-1){
        printf(" it luck keys call fill...\n\n");
        fill( node, idx);
    }
    if(flag && idx > node->n){
        printf("if after fill a merge happend there is one less child so if idx points to the last child it is no longer exist so we delete from its left sibling\n\n");
        deletefromnode(node->c[idx - 1] , key);
    }
    else{
        printf(" if a merge didnt hapend we delete normal from the child\n\n");
        deletefromnode(node->c[idx], key);
    }

}
 printf("====== delete from node has finished ======\n\n");
}
struct btree * delete(struct btree * root , int key){
    printf(" we check if the tree is empty...\n\n");
    if(root == NULL){
        printf("tree is empty");
        printf("====== delete is finished======\n\n");
        return root;
    }
    else{
        printf(" root is not empty but it may have zero keys...\n\n");
        deletefromnode(root , key);
    if (root->n == 0) {
        printf("root hase zero keys!\n\n");
        printf(" we create a temporary root\n\n");
        struct btree *tmp = root;
        printf(" we check if root is a leaf...\n\n");
        if (root->leaf) {
            printf(" root is a leaf! so the tree is empty return NULL\n\n");
            root = NULL;
        } else {
            printf("root is not a leaf! so its only child would now become the root\n\n");
            root = root->c[0];
        }
        free(tmp->keys);
        printf(" we free the old root keys\n\n");

        free(tmp->c);
        printf(" we free the old root->c\n\n");

        free(tmp);
        printf("we free the old root\n\n");
    }
     printf("======delete has finished======");
    return root;
}
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
