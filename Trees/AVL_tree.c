#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#pragma warning(disable : 4996)

struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int height;
};

// Funkcja tworz�ca nowy w�ze� drzewa AVL
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 1; // nowy w�ze� jest na najni�szym poziomie
    return(node);
}

// Funkcja zwracaj�ca wysoko�� w�z�a
int height(struct Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Funkcja zwracaj�ca wi�ksz� z dw�ch liczb
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Wstawianie nowego w�z�a do drzewa AVL
struct Node* insert(struct Node* node, int key) {
    // Klasyczne wstawianie jak w BST
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
    {
        node->left = insert(node->left, key);
        node->left->parent = node;
    }
    else if (key > node->key)
    {
        node->right = insert(node->right, key);
        node->right->parent = node;
    }
    else // Duplikaty nie s� dozwolone
        return node;

    return node;
}

struct Node* findMin(struct Node* node) {
    struct Node* current = node;

    while (current && current->left != NULL) {
        printf("%d -> ", current->key);
        current = current->left;
    }
    if (current != NULL)
        printf("%d\n", current->key);
    return current;
}

struct Node* findMax(struct Node* node) {
    struct Node* current = node;

    while (current && current->right != NULL) {
        printf("%d -> ", current->key);
        current = current->right;
    }

    if (current != NULL)
        printf("%d\n", current->key);

    return current;
}

struct Node* deleteNode(struct Node* root, int key) {
    if (root == NULL)
        return root;

    // Je�li klucz jest mniejszy od klucza w�z�a, to nale�y i�� w lewe poddrzewo
    if (key < root->key)
        root->left = deleteNode(root->left, key);

        // Je�li klucz jest wi�kszy od klucza w�z�a, to nale�y i�� w prawe poddrzewo
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

        // Je�li klucz jest r�wny kluczowi w�z�a, to w�ze� jest do usuni�cia
    else {
        // W�ze� z jednym lub brakiem dzieci
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node* temp = root->left ? root->left : root->right;

            // Brak dzieci
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else // Jeden dziecko
                *root = *temp; // Kopiujemy zawarto��
            free(temp);
        }
        else {
            // W�ze� z dw�jk� dzieci
            struct Node* temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    return root;
}



int bstToVine(struct Node* grand) {
    int count = 0;
    struct Node* tmp = grand->right;

    // Traverse until tmp becomes NULL
    while (tmp) {
        // If left exists for node
        // pointed by tmp then
        // right rotate it.
        if (tmp->left) {
            struct Node* oldTmp = tmp;
            tmp = tmp->left;
            oldTmp->left = tmp->right;
            tmp->right = oldTmp;
            grand->right = tmp;
        }
            // If left doesn't exist
            // add 1 to count and
            // traverse further right to
            // flatten remaining BST.
        else {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}

// Function to compress given tree
// with its root as grand->right.
void compress(struct Node* grand, int m) {
    struct Node* tmp = grand->right;

    // Traverse and left-rotate root m times
    // to compress given vine form of BST.
    for (int i = 0; i < m; i++) {
        struct Node* oldTmp = tmp;
        tmp = tmp->right;
        grand->right = tmp;
        oldTmp->right = tmp->left;
        tmp->left = oldTmp;
        grand = tmp;
        tmp = tmp->right;
    }
}

// Function to implement the algorithm
struct Node* balanceAVL(struct Node* root) {
    // create dummy node with value 0
    struct Node* grand = newNode(0);

    // assign the right of dummy node as our input BST
    grand->right = root;

    // get the number of nodes in input BST and
    // simultaneously convert it into right linked list.
    int count = bstToVine(grand);

    // gets the height of tree in which all levels
    // are completely filled.
    int h = log2(count + 1);

    // get number of nodes until second last level
    int m = pow(2, h) - 1;

    // Left rotate for excess nodes at last level
    compress(grand, count - m);

    // Left rotation till m becomes 0
    // Step is done as mentioned in algo to
    // make BST balanced.
    for (m = m / 2; m > 0; m /= 2) {
        compress(grand, m);
    }

    // return the balanced tree
    return grand->right;
}

int getNodeLevel(struct Node* root, int key, int level) {
    if (root == NULL)
        return 0;
    if (root->key == key)
        return level;

    int downlevel = getNodeLevel(root->left, key, level + 1);
    if (downlevel != 0)
        return downlevel;

    downlevel = getNodeLevel(root->right, key, level + 1);
    return downlevel;
}

void printNodesAtLevel(struct Node* root, int level, int currentLevel) {
    if (root == NULL)
        return;

    if (currentLevel == level) {
        printf("%d ", root->key);
        //ot = deleteNode(root, root->key);
    }
    else {
        printNodesAtLevel(root->left, level, currentLevel + 1);
        printNodesAtLevel(root->right, level, currentLevel + 1);
    }
}

void reverseInOrder(struct Node* root) {
    if (root == NULL)
        return;
    reverseInOrder(root->right);
    printf("%d ", root->key);
    reverseInOrder(root->left);
}

void printPreorder(struct Node* root) {
    if (root == NULL)
        return;
    printf("%d ", root->key);
    printPreorder(root->left);
    printPreorder(root->right);
}

int getHeightOfSubtree(struct Node* root) {
    if (root == NULL)
        return 0;
    int leftHeight = getHeightOfSubtree(root->left);
    int rightHeight = getHeightOfSubtree(root->right);
    return 1 + max(leftHeight, rightHeight);
}

void deleteSubtree(struct Node* root) {
    if (root == NULL)
        return;

    deleteSubtree(root->left);
    deleteSubtree(root->right);

    root->left = NULL;
    root->right = NULL;
    if (root->parent != NULL)
    {
        if (root->parent->left != NULL)
        {
            if (root->parent->left->key == root->key) root->parent->left = NULL;
        }
        if (root->parent->right != NULL)
        {
            if (root->parent->right->key == root->key) root->parent->right = NULL;
        }
    }
    free(root);
}


void bubbleSort(int arr[], int size) {
    int i, j, temp;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

struct Node* constructAVL(int numbers[], int begin, int end, struct Node* root, int height)
{
    if (begin > end)
    {
        return root;
    }
    int mid = (begin + end) / 2;

    root = insert(root, numbers[mid]);

    root = constructAVL(numbers, 0, mid - 1, root, height + 1);
    root = constructAVL(numbers, mid + 1, end, root, height + 1);

}

struct Node* getNode(struct Node* root, int key)
{
    if (root == NULL)
        return NULL;

    if (root->key == key)
        return root;

    // Search in the left subtree if the key is less than the current node's key
    if (key < root->key)
        return getNode(root->left, key);

    // Search in the right subtree if the key is greater than the current node's key
    return getNode(root->right, key);
}

void printAvl(struct Node* root, int level)
{
    if (root != NULL)
    {
        printf("\nLevel: %d\n", level + 1);
        printf("Key: %d\n", root->key);
        if (root->right != NULL) printf("Right: %d\n", root->right->key);
        if (root->left != NULL) printf("Left: %d\n", root->left->key);
        printAvl(root->right, level + 1);
        printAvl(root->left, level + 1);
    }
}

int main() {
    srand(time(NULL));
    struct Node* root = NULL;
    int n, key;
    int choice;
    srand(time(NULL));
    char ch;
    int* numbers;

    printf("Want to generate numbers or input by yourself? (g/i):");
    scanf(" %c", &ch);
    if (ch == 'g') {
        printf("Enter the number of items to generate (n <= 15): ");
        scanf("%d", &n);
        numbers = (int*)malloc(n * sizeof(int));
        printf("Generated numbers: ");
        for (int i = 0; i < n; ++i) {
            key = rand() % 100 + 1;
            numbers[i] = key;
            printf("%d ", key);
        }
        printf("\n");
    }
    else if (ch == 'i') {
        printf("Enter the number of items to input (n <= 15): ");
        scanf("%d", &n);
        numbers = (int*)malloc(n * sizeof(int));
        printf("Enter %d numbers: ", n);
        for (int i = 0; i < n; ++i) {
            scanf("%d", &key);
            numbers[i] = key;
            printf("%d ", key);
        }
    }
    else {
        printf("Invalid choice.\n");
        return 1;
    }

    bubbleSort(numbers, n);
    printf("SORTED: ");
    for (int i = 0; i < n; i++)
        printf("%d ", numbers[i]);
    root = constructAVL(numbers, 0, n - 1, root, 0);
    printAvl(root, 0);

    do {
        printf("\nMenu:\n");
        printf("1. Find the element with the smallest value in the tree\n");
        printf("2. Find the element with the highest value in the tree\n");
        printf("3. Find the level of the node with the specified key, print the elements at this level and delete the node\n");
        printf("4. Print all tree elements in descending order\n");
        printf("5. Write the subtree in pre-order order, specify the height and delete it\n");
        printf("6. Balance the tree and list items before and after balancing\n");
        printf("7. Exit\n");
        printf("Choose command: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Path to the element with the smallest value: ");
                findMin(root);
                break;
            case 2:
                printf("Path to the element with the highest value: ");
                findMax(root);
                break;
            case 3:
                printf("Enter the value of the node key to find: ");
                scanf("%d", &key);
                int level = getNodeLevel(root, key, 1);
                printf("The node with the value %d is at %d.\n", key, level);
                printf("Elements at the level %d: ", level);
                printNodesAtLevel(root, level, 1);
                deleteNode(root, key);
                printf("The node was deleted...");
                break;
            case 4:
                printf("Elements in descending order: ");
                reverseInOrder(root);
                printf("\n");
                break;
            case 5:
                printf("Enter the value of the root: ");
                scanf("%d", &key);
                struct Node* subtreeRoot = getNode(root, key);
                if (subtreeRoot != NULL)
                {
                    printf("Subtree in Preorder: ");
                    printPreorder(subtreeRoot);
                    printf("\n");
                    int subtreeHeight = getHeightOfSubtree(subtreeRoot);
                    printf("Subtree height: %d\n", subtreeHeight);
                    deleteSubtree(subtreeRoot);
                    printf("Removed subtree.\n");
                }
                break;
            case 6:
                printf("Before balancing: ");
                printPreorder(root);
                printf("\n");
                root = balanceAVL(root);
                printf("After balancing: ");
                printPreorder(root);
                printf("\n");
                break;
            case 7:
                printf("Exit.\n");
                break;
            case 8:
                printAvl(root, 0);
                break;
            default:
                printf("Wrong choice. Select again.\n");
        }
    } while (choice != 7);

    deleteSubtree(root);

    return 0;
}