#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#pragma warning(disable : 4996)

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int height;
    int balance;
}Node;

struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    //node->height = 1;
    node->balance = 0;
    return(node);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

struct Node* insert(struct Node* node, int key) {
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
    else
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

struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;

    return current;
}

struct Node* deleteNode(struct Node* root, int key) {
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node* temp = root->left ? root->left : root->right;


            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else {
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

    while (tmp) {
        if (tmp->left) {
            struct Node* oldTmp = tmp;
            tmp = tmp->left;
            oldTmp->left = tmp->right;
            tmp->right = oldTmp;
            grand->right = tmp;
        }
        else {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}


void compress(struct Node* grand, int m) {
    struct Node* tmp = grand->right;

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

struct Node* balanceDSW(struct Node* root) {
    struct Node* grand = newNode(0);

    grand->right = root;

    int count = bstToVine(grand);

    int h = log2(count + 1);

    int m = pow(2, h) - 1;

    compress(grand, count - m);

    for (m = m / 2; m > 0; m /= 2) {
        compress(grand, m);
    }
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

void getBalances(struct Node* root) {
    if (root == NULL)
        return;
    root->balance = getHeightOfSubtree(root->left) - getHeightOfSubtree(root->right);
    printPreorder(root->left);
    printPreorder(root->right);
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
    root = NULL;
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

    if (key < root->key)
        return getNode(root->left, key);

    return getNode(root->right, key);
}

void printAvl(struct Node* root, int level)
{
    if (root != NULL)
    {
        printf("\nLevel: %d\n", level + 1);
        printf("Key: %d\n", root->key);
        printf("Balance: %d\n", root->balance);
        if (root->right != NULL) printf("Right: %d\n", root->right->key);
        if (root->left != NULL) printf("Left: %d\n", root->left->key);
        printAvl(root->right, level + 1);
        printAvl(root->left, level + 1);
    }
}

struct Node* replaceWithHigherChild(Node* root) {
    if (getHeightOfSubtree(root->right) > getHeightOfSubtree(root->left))
    {
        struct Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    else
    {
        struct Node* temp = minValueNode(root->left);
        root->key = temp->key;
        root->left = deleteNode(root->left, temp->key);
    }
    return root;
}

int checkCurrentLevel(Node* root, int level, Node* treeRoot)
{
    if (root == NULL)
    {
        return 0;
    }
    if (level == 1)
    {
//        printf("\n%d", root->key);
        if (abs(root->balance) > 1)
        {
            int key = root->key;
            root = replaceWithHigherChild(root);
            insert(treeRoot, key);
            return 1;
        }
    }
    else if (level > 1)
    {
        checkCurrentLevel(root->left, level - 1, treeRoot);
        checkCurrentLevel(root->right, level - 1, treeRoot);
    }
    return 0;
}

void balance(Node* root, int h)
{
    int i;
    int wasBalanced = 0;
    while (1)
    {
        wasBalanced = 0;

        for (i = 1; i <= h; i++)
        {
            wasBalanced = checkCurrentLevel(root, i, root);
            if (wasBalanced) break;
            //printf("\n%d", i);
        }
        getBalances(root);
        if (wasBalanced == 0) break;
    }
}

#ifdef _WIN32
#include <Windows.h>
double what_time_is_it()
{
    FILETIME ft;
    ULONGLONG nTime100;
    GetSystemTimeAsFileTime(&ft);
    nTime100 = ((ULONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
    return (double)nTime100 * 1e-9;
}
#else

#include <time.h>

double what_time_is_it() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec * 1e-9;
}

#endif

int main() {
    srand(time(NULL));
    struct Node* root = NULL;
    int n, key;
    int choice;
    srand(time(NULL));
    char ch;
    int* numbers;
    double min_time = 0, max_time = 0, balance_time = 0, print_time = 0;
    double start, end;

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
//    printAvl(root, 0);

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
                start = what_time_is_it();
                findMin(root);
                end = what_time_is_it();
                min_time += (end - start);
                printf("Time: %lf\n", min_time);
                break;
            case 2:
                printf("Path to the element with the highest value: ");
                start = what_time_is_it();
                findMax(root);
                end = what_time_is_it();
                max_time += (end - start);
                printf("Time: %lf\n", max_time);
                break;
            case 3:
                printf("Enter the value of the node key to find: ");
                scanf("%d", &key);
                int level = getNodeLevel(root, key, 1);
                printf("The node with the value %d is at %d.\n", key, level);
                printf("Elements at the level %d: ", level);
                printNodesAtLevel(root, level, 1);
                deleteNode(root, key);
//                getBalances(root);
                break;
            case 4:
                printf("Elements in descending order: ");
                start = what_time_is_it();
                reverseInOrder(root);
                end = what_time_is_it();
                print_time += (end - start);
                printf("\nTime: %lf\n", print_time);
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
//                getBalances(root);
                break;
            case 6:
                printf("Tree before balancing:");
                printPreorder(root);
                printf("\n");
                int c;
                int h = getHeightOfSubtree(root);
                printf("Balancing with DSW or removing the roots of subtrees? (1/2):");
                scanf("%d", &c);
                if (c == 1) {
                    start = what_time_is_it();
                    root = balanceDSW(root);
                    end = what_time_is_it();
                    balance_time += (end - start);
                    printf("Time: %lf", balance_time);
//                    printf("\nBalance:");
//                    getBalances(root);
                }
                else if (c == 2) {
                    start = what_time_is_it();
                    balance(root, h);
                    end = what_time_is_it();
                    balance_time += (end - start);
                    printf("\nTime: %lf", balance_time);
//                    printf("Balance:");
//                    getBalances(root);
                }
                printf("\n");
                printf("Tree after balancing:");
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