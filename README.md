# btree-in-c
This is a B-tree implementation in C. It includes clean, well-structured code and dynamic explanations of what happens in every part of the implementation.


B-Tree

A B-tree is a self-balancing search tree optimized for systems thatread and write large blocks of data, such as databases and file systems.

Unlike binary search trees, a B-tree stores multiple keys in eachnode, dramatically reducing the height of the tree. Fewer levels meanfewer disk accesses, which is why B-trees are the standard indexingstructure for storage systems.

Why B-Trees Are Used

Common applications include databases, file systems, and storage enginesbecause they minimize disk I/O.

Operation   Complexity

Search      O(log n)Insert      O(log n)Delete      O(log n)

The Minimum Degree (T)

Minimum keys per non-root node: T - 1

Maximum keys per node: 2T - 1

Maximum children: 2T

Why 2T - 1?

2T - 1 is always odd, so every full node has a unique middle key thatcan be promoted during a split.

Example (T = 3):

10 20 30 40 50
      ^

Split:

      30
     /  \
10 20    40 50

Why T - 1?

Two minimum-size siblings plus the separator key from the parent mergeinto exactly one full node:

(T-1) + 1 + (T-1) = 2T - 1

This guarantees merging never overflows a node.

Insertion

New keys are only inserted into leaf nodes.

Internal nodes never receive a newly inserted key directly. They gainkeys only when one of their children splits and promotes its middle key.

Steps:

Find the correct leaf.

Insert if there is space.

If full, split the node.

Promote the middle key.

Repeat upward if necessary.

Split the root if needed.

Deletion

Deletion maintains the rule that every non-root node contains at leastT - 1 keys.

The algorithm uses:

Direct removal from leaves when safe.

Replacement with predecessor/successor for internal nodes.

Borrowing from siblings when possible.

Merging siblings when borrowing is impossible.

Merging restores the minimum occupancy while preserving all B-treeinvariants.

Project Features

Search

Insertion

Deletion

Node splitting

Borrowing

Merging

Clean C implementation

Educational comments and explanations
