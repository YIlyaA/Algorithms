#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};

// Funkcja tworząca nowy węzeł drzewa AVL
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // nowy węzeł jest na najniższym poziomie
    return(node);
}

// Funkcja zwracająca wysokość węzła
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Funkcja zwracająca większą z dwóch liczb
int max(int a, int b) {
    return (a > b)? a : b;
}

// Funkcja wykonująca prawy obrót wokół węzła y
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Wykonanie rotacji
    x->right = y;
    y->left = T2;

    // Aktualizacja wysokości węzłów
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Funkcja wykonująca lewy obrót wokół węzła x
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Wykonanie rotacji
    y->left = x;
    x->right = T2;

    // Aktualizacja wysokości węzłów
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Znajdowanie różnicy wysokości lewego i prawego poddrzewa węzła
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Wstawianie nowego węzła do drzewa AVL
struct Node* insert(struct Node* node, int key) {
    // Klasyczne wstawianie jak w BST
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Duplikaty nie są dozwolone
        return node;

    // Aktualizacja wysokości węzła nadrzędnego
    node->height = 1 + max(height(node->left), height(node->right));

    // Sprawdzenie zrównoważenia węzła
    int balance = getBalance(node);

    // Niezrównoważone drzewo, należy rozważyć różne przypadki rotacji

    // Przypadek lewo-lewy
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Przypadek prawo-prawo
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Przypadek lewo-prawo
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Przypadek prawo-lewo
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Drzewo zrównoważone
    return node;
}

// Funkcja wyszukująca węzeł o najmniejszej wartości
struct Node * minValueNode(struct Node* node) {
    struct Node* current = node;

    // Idziemy do najbardziej lewego węzła
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Funkcja usuwająca węzeł o podanej wartości klucza
struct Node* deleteNode(struct Node* root, int key) {
    // Podstawowy przypadek
    if (root == NULL)
        return root;

    // Jeśli klucz jest mniejszy od klucza węzła, to należy iść w lewe poddrzewo
    if (key < root->key)
        root->left = deleteNode(root->left, key);

        // Jeśli klucz jest większy od klucza węzła, to należy iść w prawe poddrzewo
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

        // Jeśli klucz jest równy kluczowi węzła, to węzeł jest do usunięcia
    else {
        // Węzeł z jednym lub brakiem dzieci
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node *temp = root->left ? root->left : root->right;

            // Brak dzieci
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else // Jeden dziecko
                *root = *temp; // Kopiujemy zawartość
            free(temp);
        }
        else {
            // Węzeł z dwójką dzieci
            struct Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // Jeśli drzewo miało tylko jeden węzeł, to zwracamy pustą strukturę
    if (root == NULL)
        return root;

    // Aktualizacja wysokości węzła
    root->height = 1 + max(height(root->left), height(root->right));

    // Sprawdzanie zrównoważenia drzewa
    int balance = getBalance(root);

    // Niezrównoważone drzewo, należy rozważyć różne przypadki rotacji

    // Przypadek lewo-lewy
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Przypadek lewo-prawo
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Przypadek prawo-prawo
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Przypadek prawo-lewo
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Funkcja wyszukująca węzeł o największej wartości
struct Node * maxValueNode(struct Node* node) {
    struct Node* current = node;

    // Idziemy do najbardziej prawego węzła
    while (current->right != NULL)
        current = current->right;

    return current;
}

// Funkcja wypisująca ścieżkę od korzenia do węzła o podanej wartości
void printPathToNode(struct Node* root, int key) {
    if (root == NULL)
        return;

    printf("%d ", root->key);

    if (key < root->key)
        printPathToNode(root->left, key);
    else if (key > root->key)
        printPathToNode(root->right, key);
}

// Funkcja wyszukująca węzeł o podanej wartości i wypisująca ścieżkę do niego
void searchAndPrintPath(struct Node* root, int key) {
    struct Node* current = root;
    printf("Path to node %d: ", key);
    printPathToNode(root, key);
    printf("\n");
}

// Funkcja zwracająca poziom węzła o podanej wartości w drzewie
int getNodeLevel(struct Node* root, int key, int level) {
    if (root == NULL)
        return 0;
    if (root->key == key)
        return level;

    // Przechodzenie rekurencyjnie do lewego i prawego poddrzewa
    int downlevel = getNodeLevel(root->left, key, level + 1);
    if (downlevel != 0)
        return downlevel;

    downlevel = getNodeLevel(root->right, key, level + 1);
    return downlevel;
}

// Funkcja wypisująca elementy na podanym poziomie i usuwająca węzeł o podanej wartości
void printAndDeleteNodesAtLevel(struct Node* root, int level, int currentLevel) {
    if (root == NULL)
        return;

    if (currentLevel == level) {
        printf("%d ", root->key);
        root = deleteNode(root, root->key);
    }
    else {
        printAndDeleteNodesAtLevel(root->left, level, currentLevel + 1);
        printAndDeleteNodesAtLevel(root->right, level, currentLevel + 1);
    }
}

// Funkcja wypisująca elementy w porządku malejącym (wykorzystująca odwrócone Inorder Traversal)
void reverseInOrder(struct Node* root) {
    if (root == NULL)
        return;
    reverseInOrder(root->right);
    printf("%d ", root->key);
    reverseInOrder(root->left);
}

// Funkcja wypisująca elementy w porządku Preorder dla poddrzewa o zadanym korzeniu
void printPreorder(struct Node* root) {
    if (root == NULL)
        return;
    printf("%d ", root->key);
    printPreorder(root->left);
    printPreorder(root->right);
}

// Funkcja zwracająca wysokość poddrzewa o zadanym korzeniu
int getHeightOfSubtree(struct Node* root) {
    if (root == NULL)
        return 0;
    int leftHeight = getHeightOfSubtree(root->left);
    int rightHeight = getHeightOfSubtree(root->right);
    return 1 + max(leftHeight, rightHeight);
}

// Funkcja usuwająca poddrzewo o zadanym korzeniu
void deleteSubtree(struct Node* root) {
    if (root == NULL)
        return;
    deleteSubtree(root->left);
    deleteSubtree(root->right);
    free(root);
}

// Funkcja równoważąca drzewo przez rotacje (metoda DSW)
struct Node* balanceTree(struct Node* root) {
    // Wykonanie rotacji w prawo
    while (root->left != NULL) {
        root = rightRotate(root);
    }
    return root;
}

// Funkcja wypisująca elementy drzewa w porządku Preorder
void printPreorderTree(struct Node* root) {
    printf("Preorder before balancing: ");
    printPreorder(root);
    printf("\n");
}

// Funkcja wypisująca elementy drzewa w porządku Preorder po zrównoważeniu
void printPreorderBalancedTree(struct Node* root) {
    printf("Preorder after balancing: ");
    printPreorder(root);
    printf("\n");
}

// Funkcja główna
int main() {
    struct Node *root = NULL;
    int n, key;
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. Wczytaj %d elementowy ciąg liczb naturalnych i dodaj do drzewa AVL\n", n);
        printf("2. Wyszukaj element o najmniejszej wartości w drzewie i wypisz ścieżkę poszukiwania\n");
        printf("3. Wyszukaj element o największej wartości w drzewie i wypisz ścieżkę poszukiwania\n");
        printf("4. Podaj poziom węzła o podanej wartości w drzewie, wypisz elementy na tym poziomie i usuń węzeł\n");
        printf("5. Wypisz wszystkie elementy drzewa w porządku malejącym\n");
        printf("6. Wypisz w porządku Preorder podrzewa, którego korzeń podaje użytkownik, podaj wysokość i usuń podrzewo\n");
        printf("7. Zrównoważ drzewo przez rotacje (metoda DSW)\n");
        printf("8. Wyjście z programu\n");
        printf("Wybierz opcję: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Podaj liczbę elementów (n<=10): ");
                scanf("%d", &n);
                printf("Podaj %d liczb naturalnych: ", n);
                for (int i = 0; i < n; i++) {
                    scanf("%d", &key);
                    root = insert(root, key);
                }
                break;
            case 2:
                searchAndPrintPath(root, minValueNode(root)->key);
                break;
            case 3:
                searchAndPrintPath(root, maxValueNode(root)->key);
                break;
            case 4:
                printf("Podaj wartość klucza węzła do znalezienia: ");
                scanf("%d", &key);
                int level = getNodeLevel(root, key, 1);
                printf("Węzeł o wartości %d znajduje się na poziomie %d.\n", key, level);
                printf("Elementy na poziomie %d: ", level);
                printAndDeleteNodesAtLevel(root, level, 1);
                break;
            case 5:
                printf("Elementy w porządku malejącym: ");
                reverseInOrder(root);
                printf("\n");
                break;
            case 6:
                printf("Podaj wartość korzenia podrzewa do wypisania: ");
                scanf("%d", &key);
                struct Node* subtree = NULL;
                subtree = insert(subtree, key);
                printf("Poddrzewo w porządku Preorder: ");
                printPreorder(subtree);
                printf("\n");
                int subtreeHeight = getHeightOfSubtree(subtree);
                printf("Wysokość poddrzewa: %d\n", subtreeHeight);
                printf("Usunięto poddrzewo.\n");
                deleteSubtree(subtree);
                break;
            case 7:
                root = balanceTree(root);
                printPreorderTree(root);
                printPreorderBalancedTree(root);
                break;
            case 8:
                printf("Wyjście z programu.\n");
                break;
            default:
                printf("Niepoprawny wybór. Wybierz ponownie.\n");
        }
    } while (choice != 8);

    // Usuwanie drzewa przed zakończeniem programu
    deleteSubtree(root);

    return 0;
}
