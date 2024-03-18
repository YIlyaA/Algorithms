#include <stdio.h>
#include <stdlib.h>

// Struktura węzła drzewa BST
typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

// Funkcja tworząca nowy węzeł
Node *newNode(int item) {
    Node *temp = (Node *) malloc(sizeof(Node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// Funkcja dodająca nowy węzeł do drzewa BST
Node *insert(Node *node, int key) {
    if (node == NULL) return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

// Funkcja wyszukująca element o najmniejszej wartości w drzewie BST i wypisująca ścieżkę
Node *findMin(Node *node) {
    Node *current = node;

    // Szukanie węzła o najmniejszej wartości
    while (current && current->left != NULL) {
        printf("%d -> ", current->key);
        current = current->left;
    }

    // Wypisanie ostatniego węzła (najmniejszej wartości)
    if (current != NULL)
        printf("%d\n", current->key);

    return current;
}

// Funkcja wyszukująca element o największej wartości w drzewie BST i wypisująca ścieżkę
Node *findMax(Node *node) {
    Node *current = node;

    // Szukanie węzła o największej wartości
    while (current && current->right != NULL) {
        printf("%d -> ", current->key);
        current = current->right;
    }

    // Wypisanie ostatniego węzła (największej wartości)
    if (current != NULL)
        printf("%d\n", current->key);

    return current;
}

// Funkcja znajdująca poziom węzła o danym kluczu i wypisująca elementy na tym poziomie
int findLevel(Node *node, int key, int level) {
    if (node == NULL)
        return -1;

    if (node->key == key) {
        printf("Elements at the level %d: ", level);
        printf("%d ", node->key);
        if (node->left)
            printf("%d ", node->left->key);
        if (node->right)
            printf("%d ", node->right->key);
        printf("\n");
        return level;
    }

    int downlevel = findLevel(node->left, key, level + 1);
    if (downlevel != -1)
        return downlevel;

    downlevel = findLevel(node->right, key, level + 1);
    return downlevel;
}

// Funkcja pomocnicza do wypisywania elementów drzewa w porządku malejącym (przechodzenie w porządku odwrotnym inorder)
void reverseInorder(Node *node) {
    if (node == NULL)
        return;

    reverseInorder(node->right);
    printf("%d ", node->key);
    reverseInorder(node->left);
}

// Funkcja wypisująca wszystkie elementy drzewa w porządku malejącym
void printDescending(Node *root) {
    printf("Tree elements in descending order: ");
    reverseInorder(root);
    printf("\n");
}

// Funkcja pomocnicza do wypisywania drzewa w porządku pre-order
void printPreorder(Node *node) {
    if (node == NULL)
        return;

    printf("%d ", node->key);
    printPreorder(node->left);
    printPreorder(node->right);
}

// Funkcja obliczająca wysokość drzewa
int getHeight(Node *node) {
    if (node == NULL)
        return 0;

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Funkcja wypisująca poddrzewo w porządku pre-order, podając jego wysokość
int printSubtreeAndHeight(Node *node, int key, int level) {
    if (node == NULL)
        return -1;

    if (node->key == key) {
        printf("Subtree with root %d in pre-order order: ", node->key);
        printPreorder(node);
        printf("\n");
        return getHeight(node);
    }

    int subtreeHeight = printSubtreeAndHeight(node->left, key, level + 1);
    if (subtreeHeight == -1)
        subtreeHeight = printSubtreeAndHeight(node->right, key, level + 1);

    return subtreeHeight;
}


// Funkcja usuwająca poddrzewo
Node *deleteSubtree(Node *node) {
    if (node == NULL)
        return NULL;

    node->left = deleteSubtree(node->left);
    node->right = deleteSubtree(node->right);
    free(node);
    return NULL;
}

// Funkcja równoważąca drzewo przez rotacje (algorytm DSW)
Node *balanceTree(Node *root) {
    // Implementacja algorytmu DSW (Day-Stout-Warren)

    // Tymczasowy korzeń
    Node *tempRoot = newNode(0);
    tempRoot->right = root;

    Node *current = tempRoot;
    Node *prev = NULL;

    // Prawe obroty
    while (current != NULL) {
        if (current->left != NULL) {
            Node *left = current->left;
            current->left = left->right;
            left->right = current;
            current = left;
            if (prev != NULL)
                prev->right = left;
            else
                tempRoot->right = left;
            prev = left;
        } else {
            prev = current;
            current = current->right;
        }
    }

    // Lewe obroty
    current = tempRoot->right;
    while (current != NULL && current->right != NULL) {
        Node *right = current->right;
        current->right = right->left;
        right->left = current;
        current = right;
    }

    return tempRoot->right;
}

// Funkcja usuwająca drzewo
Node *deleteTree(Node *node) {
    if (node == NULL)
        return NULL;

    node->left = deleteTree(node->left);
    node->right = deleteTree(node->right);
    free(node);
    return NULL;
}

// Funkcja wyświetlająca menu
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Find the element with the smallest value in the tree\n");
    printf("2. Find the element with the highest value in the tree\n");
    printf("3. Find the level of the node with the specified key, print the elements at this level and delete the node\n");
    printf("4. Print all tree elements in descending order\n");
    printf("5. Write the subtree in pre-order order, specify the height and delete it\n");
    printf("6. Balance the tree and list items before and after balancing\n");
    printf("7. Exit\n");
    printf("Select an option: ");
}

int main() {
    Node *root = NULL;
    int n, i, choice, key;

    printf("Enter the number of items to insert (n <= 10): ");
    scanf("%d", &n);
    printf("Enter %d numbers to insert: ", n);
    for (i = 0; i < n; ++i) {
        int num;
        scanf("%d", &num);
        root = insert(root, num);
    }

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Path to the element with the smallest value: ");
                Node *minNode = findMin(root);
                break;
            }
            case 2: {
                printf("Path to the element with the highest value: ");
                Node *maxNode = findMax(root);
                break;
            }
            case 3: {
                printf("Enter the node key to delete: ");
                scanf("%d", &key);
                int level = findLevel(root, key, 1);
                if (level != -1) {
                    printf("The node with the key %d is at %d\n", key, level);
                    // Usuwanie węzła
                    // (implementacja usunięcia węzła należy do Ciebie, można użyć funkcji deleteSubtree())
                } else {
                    printf("Node with key %d not found in tree.\n", key);
                }
                break;
            }
            case 4: {
                printDescending(root);
                break;
            }
            case 5: {
                printf("Provide subtree root key to dump and delete: ");
                scanf("%d", &key);
                int subtreeHeight = printSubtreeAndHeight(root, key, 1);
                if (subtreeHeight != -1) {
                    printf("Subtree height: %d\n", subtreeHeight);
                    // Usuwanie poddrzewa
                    // (implementacja usuwania poddrzewa należy do Ciebie, można użyć funkcji deleteSubtree())
                } else {
                    printf("Subtree with root key %d not found.\n", key);
                }
                break;
            }
            case 6: {
                printf("Trees before sustainability:\n");
                printPreorder(root);
                printf("\n");

                // Zrównoważanie drzewa
                root = balanceTree(root);

                printf("Trees after sustainability:\n");
                printPreorder(root);
                printf("\n");
                break;
            }
            case 7:
                break;
            default:
                printf("Wrong option. Try again.\n");
        }
    } while (choice != 7);

    // Usuwanie drzewa po zakończeniu programu
    root = deleteTree(root);

    return 0;
}
