#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define DATA_FILE "library.dat"

struct Book {
    int id;
    char title[100];
    char author[100];
    int year;
    int isAvailable;
};

struct Book library[MAX_BOOKS];
int bookCount = 0;

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Save library to file
void saveLibrary() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (!file) {
        printf("Error saving file.\n");
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(library, sizeof(struct Book), bookCount, file);
    fclose(file);
    printf("Library saved successfully.\n");
}

// Load library from file
void loadLibrary() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (!file) {
        printf("No previous library found. Starting fresh.\n");
        return;
    }
    fread(&bookCount, sizeof(int), 1, file);
    fread(library, sizeof(struct Book), bookCount, file);
    fclose(file);
    printf("Library loaded successfully.\n");
}

// Add a new book
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full.\n");
        return;
    }

    struct Book newBook;
    newBook.id = (bookCount == 0) ? 1 : library[bookCount - 1].id + 1;

    printf("Enter title: ");
    clearInputBuffer();
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';

    printf("Enter author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    printf("Enter year: ");
    scanf("%d", &newBook.year);

    newBook.isAvailable = 1;

    library[bookCount++] = newBook;

    printf("Book added successfully.\n");
}

// Search books
void searchBook() {
    char title[100];
    int found = 0;

    printf("Enter title to search: ");
    clearInputBuffer();
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    for (int i = 0; i < bookCount; i++) {
        if (strstr(library[i].title, title)) {
            printf("ID %d | %s by %s (%d) | %s\n",
                library[i].id,
                library[i].title,
                library[i].author,
                library[i].year,
                library[i].isAvailable ? "Available" : "Issued"
            );
            found = 1;
        }
    }

    if (!found) printf("No matching books found.\n");
}

// Display all books
void displayBooks() {
    if (bookCount == 0) {
        printf("Library is empty.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        printf("[%d] %s by %s (%d) - %s\n",
            library[i].id,
            library[i].title,
            library[i].author,
            library[i].year,
            library[i].isAvailable ? "Available" : "Issued"
        );
    }
}

// Issue a book
void issueBook() {
    int id;
    printf("Enter book ID to issue: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            if (!library[i].isAvailable) {
                printf("Book already issued.\n");
                return;
            }
            library[i].isAvailable = 0;
            printf("Book issued successfully.\n");
            return;
        }
    }

    printf("Book not found.\n");
}

// Return book
void returnBook() {
    int id;
    printf("Enter book ID to return: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            if (library[i].isAvailable) {
                printf("Book was not issued.\n");
                return;
            }
            library[i].isAvailable = 1;
            printf("Book returned successfully.\n");
            return;
        }
    }

    printf("Book not found.\n");
}

// Delete a book
void deleteBook() {
    int id;
    printf("Enter ID of book to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Book not found.\n");
        return;
    }

    for (int i = index; i < bookCount - 1; i++) {
        library[i] = library[i + 1];
    }

    bookCount--;
    printf("Book deleted.\n");
}

// Sorting functions
void sortByTitle() {
    for (int i = 0; i < bookCount - 1; i++)
        for (int j = i + 1; j < bookCount; j++)
            if (strcmp(library[i].title, library[j].title) > 0)
                { struct Book temp = library[i]; library[i] = library[j]; library[j] = temp; }
    printf("Sorted by title.\n");
}

void sortByAuthor() {
    for (int i = 0; i < bookCount - 1; i++)
        for (int j = i + 1; j < bookCount; j++)
            if (strcmp(library[i].author, library[j].author) > 0)
                { struct Book temp = library[i]; library[i] = library[j]; library[j] = temp; }
    printf("Sorted by author.\n");
}

void sortByYear() {
    for (int i = 0; i < bookCount - 1; i++)
        for (int j = i + 1; j < bookCount; j++)
            if (library[i].year > library[j].year)
                { struct Book temp = library[i]; library[i] = library[j]; library[j] = temp; }
    printf("Sorted by year.\n");
}

void sortByID() {
    for (int i = 0; i < bookCount - 1; i++)
        for (int j = i + 1; j < bookCount; j++)
            if (library[i].id > library[j].id)
                { struct Book temp = library[i]; library[i] = library[j]; library[j] = temp; }
    printf("Sorted by ID.\n");
}

// Sorting menu
void sortMenu() {
    int choice;
    printf("\n--- Sort Books ---\n");
    printf("1. By Title\n");
    printf("2. By Author\n");
    printf("3. By Year\n");
    printf("4. By ID\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: sortByTitle(); break;
        case 2: sortByAuthor(); break;
        case 3: sortByYear(); break;
        case 4: sortByID(); break;
        default: printf("Invalid choice.\n");
    }
}

void menu() {
    int choice;
    loadLibrary();

    while (1) {
        printf("\n--- Library System ---\n");
        printf("1. Add Book\n");
        printf("2. Search Book\n");
        printf("3. Display All Books\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Sort Books\n");
        printf("8. Save Library\n");
        printf("9. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: addBook(); break;
            case 2: searchBook(); break;
            case 3: displayBooks(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: sortMenu(); break;
            case 8: saveLibrary(); break;
            case 9:
                saveLibrary();
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}