#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// structure for product information.
typedef struct product{
    int id   ;
    char name[40];
    int quantity;
    float price;
    struct product*next;
}product_t;


typedef struct list{
    product_t * head;
    int count;
}list_t;

//initial condition for list
void initList(list_t * list){
    list->head=NULL;
    list->count=0;
}

/* ----------------------------- Simple File Handling -----------------------------
   We store products in a text file "inventory.txt" as:
   id|name|quantity|price
   Example: 101|Sugar|100|45
---------------------------------------------------------------------------------*/

void saveInventory(list_t *list) {
    if (!list) return;
    FILE *fp = fopen("inventory.txt", "w");
    if (!fp) {
        printf("Could not open inventory.txt for writing.\n");
        return;
    }
    product_t *cur = list->head;
    while (cur) {
        fprintf(fp, "%d|%s|%d|%.2f\n", cur->id, cur->name, cur->quantity, cur->price);
        cur = cur->next;
    }
    fclose(fp);
    
}

void loadInventory(list_t *list) {
    if (!list) return;
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        product_t *node = (product_t*)malloc(sizeof(product_t));
        if (!node) {
            printf("Memory allocation failed while loading file.\n");
            break;
        }
        node->next = NULL;

        int id, qty;
        float price;
        char name[40];

        if (sscanf(line, "%d|%39[^|]|%d|%f", &id, name, &qty, &price) == 4) {
            
            node->id = id;
            node->quantity = qty;
            node->price = price;
            strncpy(node->name, name, sizeof(node->name));
            node->name[sizeof(node->name)-1] = '\0';

            // Append to list
            if (!list->head) {
                list->head = node;
            } else {
                product_t *t = list->head;
                while (t->next) t = t->next;
                t->next = node;
            }
            list->count++;
        } else {
            free(node);
        }
    }
    fclose(fp);
    
}

//---------- For Shopkeeper ----------

// Add product in the shop by its id , name , quantity and price

void addProduct(list_t *list){
    if (list == NULL) {
        printf("Invalid list pointer.\n");
        return;
    }

    product_t * newProduct = (product_t*)malloc(sizeof(product_t));
    if (newProduct == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter product ID: ");
    scanf("%d", &newProduct->id);
    printf("Enter product name: ");
    scanf(" %39[^\n]", newProduct->name); 
    printf("Enter price: ");
    scanf("%f", &newProduct->price);
    printf("Enter quantity: ");
    scanf("%d", &newProduct->quantity);
    newProduct->next = NULL;

    // Checking for duplicate ID
    product_t* temp = list->head;
    while (temp != NULL) {
        if (temp->id == newProduct->id) {
            printf("Product with ID %d already exists!\n", newProduct->id);
            free(newProduct);
            return;
        }
        temp = temp->next;
    }

    // Add product to end of list
    if (list->head == NULL) {
        list->head = newProduct;
    } else {
        temp = list->head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newProduct;
    }
    list->count++; 
    printf("Product added successfully!\n");
    saveInventory(list); // Saving after adding
}

void displayProduct(list_t *list)
{
    if (list == NULL || list->head == NULL) {
        printf("No products available.\n");
        return;
    }
    else {
        product_t * trav = list->head;
        printf("\n%-5s %-20s %-10s %-10s\n", "ID", "NAME", "QUANTITY", "PRICE");
        printf("--------------------------------------------------\n");
        while(trav != NULL) {
            printf("%-5d %-20s %-10d $%-10.2f\n", 
                   trav->id, trav->name, trav->quantity, trav->price);
            trav = trav->next;
        }
        printf("\n");
    }
}

void deleteProduct(list_t * list, int id) {
    if (list == NULL) {
        printf("Invalid list pointer.\n");
        return;
    }
    
    product_t *temp = list->head, *prev = NULL;

    if (temp != NULL && temp->id == id) {
        list->head = temp->next;
        free(temp);
        list->count--; 
        printf("Product deleted successfully.\n");
        saveInventory(list); // Save after delete
        return;
    }

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Product not found.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    list->count--;
    printf("Product deleted successfully.\n");
    saveInventory(list); // Saving after deletion
}

product_t* searchProduct(list_t *list, int id) {
    if (list == NULL) {
        return NULL;
    }

    product_t *temp = list->head;
    
    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }
    
    return NULL;
}

void updateProduct(list_t * list, int id) {
    if (list == NULL) {
        printf("Invalid list pointer.\n");
        return;
    }

    product_t* prod = searchProduct(list, id);
    if (prod == NULL) {
        printf("Product not found.\n");
        return;
    }
    printf("Current product: ID: %d, Name: %s, Price: %.2f, Quantity: %d\n", 
           prod->id, prod->name, prod->price, prod->quantity);
    
    printf("Enter new name: ");
    scanf(" %39[^\n]", prod->name);
    printf("Enter new price: ");
    scanf("%f", &prod->price);
    printf("Enter new quantity: ");
    scanf("%d", &prod->quantity);
    printf("Product updated successfully.\n");
    saveInventory(list); // Save after update
}

// ----------------------------- Billing with GST and Offers -----------------------------

void generateBill(list_t *list) {
    if (list == NULL) {
        printf("Invalid list pointer.\n");
        return;
    }

    int pid, qty;
    float subtotal = 0, discount = 0, gst = 0, finalAmount = 0;
    char choice;

    do {
        printf("Enter product ID: ");
        scanf("%d", &pid);
        product_t* prod = searchProduct(list, pid);

        if (prod == NULL) {
            printf("Product not found. Try again.\n");
            continue;
        }

        printf("Product: %s | Price: $%.2f | Available Quantity: %d\n", 
               prod->name, prod->price, prod->quantity);

        while (1) {
            printf("Enter quantity to buy: ");
            scanf("%d", &qty);
            if (qty <= 0) {
                printf("Quantity must be positive.\n");
            } else if (qty > prod->quantity) {
                printf("Insufficient stock. Available: %d\n", prod->quantity);
            } else {
                break;
            }
        }

        float cost = prod->price * qty;
        subtotal += cost;
        prod->quantity -= qty;

        printf("Added to bill: %s x %d = $%.2f\n", prod->name, qty, cost);
        printf("Add more products? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    // Discount system
    if (subtotal >= 2000)
        discount = 0.15 * subtotal;
    else if (subtotal >= 1000)
        discount = 0.10 * subtotal;
    else if (subtotal >= 500)
        discount = 0.05 * subtotal;
    else
        discount = 0;

    float discountedTotal = subtotal - discount;
    gst = 0.05 * discountedTotal;
    finalAmount = discountedTotal + gst;
    float actualAmt = subtotal + gst;
    float savedAmt = discount; 

    printf("\nGenerating bill with GST and Offers...\n");
    // Bill summary
    printf("\n=========== BILL SUMMARY ===========\n");
    printf("Subtotal             : $%.2f\n", subtotal);
    printf("Discount Applied     : $%.2f\n", discount);
    printf("After Discount       : $%.2f\n", discountedTotal);
    printf("GST (5%%)             : $%.2f\n", gst);
    printf("Total Amount to Pay  : $%.2f\n", finalAmount);
    printf("Amount you have saved : $%.2f\n", savedAmt);
    printf("====================================\n");
    printf("Thank You. Visit again.\n");
    saveInventory(list); // Save updated quantities after purchase
}

void freeList(list_t *list) {
    if (list == NULL) return;
    
    product_t *current = list->head;
    product_t *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->count = 0;
}

int shopkeeperLogin() {
    char password[20];
    printf("Enter Shopkeeper password: ");
    scanf("%19s", password); 
    return strcmp(password, "admin123") == 0;
}

// ----------------------------- Main Program -----------------------------

int main() {

    list_t inventory;
    initList(&inventory);

    // Load inventory from file if it exists
    loadInventory(&inventory);
    
    int roleChoice, choice, id;

    while (1) {
        printf("\nLogin as:\n1. Shopkeeper\n2. Customer\n3. Exit\nEnter choice: ");
        scanf("%d", &roleChoice);

        if (roleChoice == 3) {
            // Save once more before exiting
            saveInventory(&inventory);
            printf("Exiting program.\n");
            break;
        }

        if (roleChoice == 1) {
            if (!shopkeeperLogin()) {
                printf("Incorrect password. Access denied.\n");
                continue;
            }

            do {
                printf("\n--- Shopkeeper Menu ---\n");
                printf("1. Add Product\n");
                printf("2. Display Products\n");
                printf("3. Search Product\n");
                printf("4. Update Product\n");
                printf("5. Delete Product\n");
                printf("6. Logout\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: addProduct(&inventory); break;
                    case 2: displayProduct(&inventory); break;
                    case 3:
                        printf("Enter product ID to search: ");
                        scanf("%d", &id);
                        product_t* prod = searchProduct(&inventory, id);
                        if (prod)
                            printf("Found: %s | $%.2f | Quantity: %d\n", 
                                   prod->name, prod->price, prod->quantity);
                        else
                            printf("Product not found.\n");
                        break;
                    case 4:
                        printf("Enter product ID to update: ");
                        scanf("%d", &id);
                        updateProduct(&inventory, id);
                        break;
                    case 5:
                        printf("Enter product ID to delete: ");
                        scanf("%d", &id);
                        deleteProduct(&inventory, id);
                        break;
                    case 6:
                        printf("Logging out...\n");
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
            } while (choice != 6);

        } else if (roleChoice == 2) {
            if (inventory.head == NULL) {
                printf("No products available currently.\n");
                continue;
            }

            printf("\n--- Available Products ---\n");
            displayProduct(&inventory);
            printf("AVAILABEL OFFERS : \n");
            printf("Get 15%% off on purchases above $2000.\n");
            printf("Get 10%% off on purchases above $1000.\n");
            printf("Get 5%% off on purchases above $500.\n\n");
            printf("ADD TO CART PRODUCTS : \n");
            generateBill(&inventory);

        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    freeList(&inventory);
    
    return 0;
}
