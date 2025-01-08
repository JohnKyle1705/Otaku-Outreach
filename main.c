#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Member {
    char name[50];
    int commission;
    struct Member* left;
    struct Member* right;
    int level;
} Member;

Member* createMember(const char* name, int level) {
    Member* newMember = (Member*)malloc(sizeof(Member));
    if (newMember == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newMember->name, name);
    newMember->commission = 0;
    newMember->left = NULL;
    newMember->right = NULL;
    newMember->level = level;
    return newMember;
}

int memberExists(Member* root, const char* name) {
    if (root == NULL) return 0;
    if (strcmp(root->name, name) == 0) return 1;
    return memberExists(root->left, name) || memberExists(root->right, name);
}

void addDownline(Member* root, const char* parentName, const char* newName) {
    if (root == NULL) return;
    if (strcmp(root->name, parentName) == 0) {
        if (root->left == NULL) {
            root->left = createMember(newName, root->level + 1);
            root->commission += calculateCommission(root->level + 1);
        } else if (root->right == NULL) {
            root->right = createMember(newName, root->level + 1);
            root->commission += calculateCommission(root->level + 1);
        } else {
            printf("Member %s already has two downlines.\n", parentName);
        }
    } else {
        addDownline(root->left, parentName, newName);
        addDownline(root->right, parentName, newName);
    }
}

int calculateCommission(int level) {
    int baseCommission = 500;
    int commissionPercentage;
    if (level == 0) commissionPercentage = 50;
    else if (level == 1) commissionPercentage = 30;
    else if (level == 2) commissionPercentage = 20;
    else commissionPercentage = 10;
    return (baseCommission * commissionPercentage) / 100;
}

void displayDownlines(Member* root, const char* memberName) {
    if (root == NULL) return;
    if (strcmp(root->name, memberName) == 0) {
        printf("\nDownlines of Member %s:\n", memberName);
        if (root->left)
            printf("  \nName: %s, Commission: P%d, Level: %d\n", root->left->name, root->left->commission, root->left->level);
        if (root->right)
            printf("  \nName: %s, Commission: P%d, Level: %d\n", root->right->name, root->right->commission, root->right->level);
    } else {
        displayDownlines(root->left, memberName);
        displayDownlines(root->right, memberName);
    }
}

void displayTree(Member* root, int level) {
    if (root == NULL) return;
    displayTree(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("  ");
    printf("\nName: %s, Commission: P%d, Level: %d\n", root->name, root->commission, root->level);
    displayTree(root->left, level + 1);
}

void displayMenu() {
    //Preserve welcome message
    printf("\n                 O            +===========================================================+            O\n");
    printf("               oo oo ---->    |           Welcome to the Otaku Outreach Network           |    <---- oo oo\n");
    printf("                 O            +===========================================================+            O\n\n");

    // Menu remains on screen
    printf("+-----------------------------------------------+\n");
    printf("|         OtaKu Outreach Network Menu         |\n");
    printf("+-----------------------------------------------+\n");
    system("cls");
    printf("1. Add Member\n");
    printf("2. Display Downlines of a Member\n");
    printf("3. Display Entire Network Tree\n");
    printf("4. Leave\n");
    printf("Enter your choice: ");
}

int authenticateBoss(const char* password) {
    char inputPassword[50];
    printf("Enter password: ");
    fgets(inputPassword, sizeof(inputPassword), stdin);
    inputPassword[strcspn(inputPassword, "\n")] = 0;
    return strcmp(inputPassword, password) == 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freeTree(Member* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    printf("\n                 O            +===========================================================+            O\n");
    printf("               oo oo ---->    |           Welcome to the Otaku Outreach Network           |    <---- oo oo\n");
    printf("                 O            +===========================================================+            O\n\n");
    char rootName[50];
    printf("Enter the name of the Boss: ");
    fgets(rootName, sizeof(rootName), stdin);
    rootName[strcspn(rootName, "\n")] = 0;

    Member* root = createMember(rootName, 0);

    const char* password = "otaku123";
    int authenticated = 0;
    int attempts = 0;

    while (attempts < 3 && !authenticated) {
        system("cls"); // Clear screen
        printf("\n+-----------------------------------------------+\n");
        printf("|         Authenticate as the Boss              |\n");
        printf("+-----------------------------------------------+\n");
        if (authenticateBoss(password)) authenticated = 1;
        else {
            attempts++;
            printf("Invalid password. Please try again.\n");
        }
    }

    if (!authenticated) {
        printf("Maximum attempts exceeded. Exiting...\n");
        return 0;
    }

    system("cls"); // Clear screen
    printf("\n+-----------------------------------------------+\n");
    printf("|         Authentication Successful             |\n");
    printf("+-----------------------------------------------+\n");


    int choice;
    char parentName[50], newName[50];

    while (1) {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                system("cls"); // Clear screen after each operation

                printf("\n                 O            +===========================================================+            O\n");
                printf("               oo oo ---->    |           Welcome to the Otaku Outreach Network           |    <---- oo oo\n");
                printf("                 O            +===========================================================+            O\n\n");
                printf("+-----------------------------------------------+\n");
                printf("|         OtaKu Outreach Network Menu           |\n");
                printf("+-----------------------------------------------+\n");
                printf("Adding Member...\n");
                printf("Enter the recruiter's name: ");
                fgets(parentName, sizeof(parentName), stdin);
                parentName[strcspn(parentName, "\n")] = 0;
                if (!memberExists(root, parentName)) {
                    printf("Member %s does not exist in the network.\n", parentName);
                    break;
                }
                printf("Enter the new member's name: ");
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = 0;
                addDownline(root, parentName, newName);
                printf("Member %s added successfully.\n", newName);
                break;
            case 2:
                system("cls"); // Clear screen
                printf("\n                 O            +===========================================================+            O\n");
                printf("               oo oo ---->    |           Welcome to the Otaku Outreach Network           |    <---- oo oo\n");
                printf("                 O            +===========================================================+            O\n\n");
                printf("+-----------------------------------------------+\n");
                printf("|         OtaKu Outreach Network Menu           |\n");
                printf("+-----------------------------------------------+\n");
                printf("Displaying Downlines...\n");
                printf("Enter the member's name to display downlines: ");
                fgets(parentName, sizeof(parentName), stdin);
                parentName[strcspn(parentName, "\n")] = 0;
                displayDownlines(root, parentName);
                break;
            case 3:
                system("cls"); // Clear screen
                 printf("\n                 O            +===========================================================+            O\n");
                printf("               oo oo ---->    |           Welcome to the Otaku Outreach Network           |    <---- oo oo\n");
                printf("                 O            +===========================================================+            O\n\n");
                printf("+-----------------------------------------------+\n");
                printf("|           OtaKu Outreach Network Menu         |\n");
                printf("+-----------------------------------------------+\n");
                printf("Displaying Entire Tree...\n");
                displayTree(root, 0);
                break;
            case 4:
                 printf("\n+-----------------------------------------------+\n");
                 printf("|              Leaving the System               |\n");
                 printf("+-----------------------------------------------+\n");
                 printf("Leaving...\n");
    return 0;
                freeTree(root);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        system("pause"); //Pause to view the output before clearing the screen.  Remove if not needed.
    }
    freeTree(root);
    return 0;
}
