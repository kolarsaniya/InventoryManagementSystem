# Inventory Management System in C

This is a simple, console-based inventory management system written in C. It provides a menu-driven interface for managing a shop's inventory and processing customer sales. The system uses a singly linked list to manage products in memory and saves all data to a text file for persistence.

## Features

  * **Two User Roles:**
      * **Shopkeeper (Admin):** Manages the inventory.
      * **Customer:** Can view products and make purchases.
  * **Shopkeeper Functionality:**
      * **Password Protected:** Access is secured with a password (default: `admin123`).
      * **Add Product:** Add new products with ID, name, price, and quantity.
      * **Display Products:** View a formatted table of all products in the inventory.
      * **Search Product:** Find a specific product by its ID.
      * **Update Product:** Modify the name, price, or quantity of an existing product.
      * **Delete Product:** Remove a product from the inventory using its ID.
  * **Customer Functionality:**
      * **View Products:** See all available products and their details.
      * **View Offers:** Displays current discount offers.
      * **Billing System:**
          * Add multiple products to a cart by ID and quantity.
          * Stock is validated (prevents buying more than is available).
          * Inventory is automatically updated after a purchase.
  * **Billing & Offers:**
      * Calculates a subtotal for all items.
      * Applies a tiered discount system:
          * **5% off** on purchases above ₹500
          * **10% off** on purchases above ₹1000
          * **15% off** on purchases above ₹2000
      * Applies a **5% GST** on the discounted price.
      * Generates a final bill summary showing the total saved and the final amount.
  * **Data Persistence:**
      * All inventory data is automatically saved to `inventory.txt` after any change (add, update, delete, purchase).
      * The inventory is automatically loaded from `inventory.txt` when the program starts.

## Getting Started

### Prerequisites

You need a C compiler, such as **GCC**, installed on your system.

### Compilation

1.  Save the code as `InventoryManagementSystem.c`.

2.  Open your terminal or command prompt.

3.  Navigate to the directory where you saved the file.

4.  Compile the program using GCC:

    ```bash
    gcc InventoryManagementSystem.c -o inventory
    ```

    (If you are on Windows, you might name the output `inventory.exe`).

### Running the Program

Run the compiled executable from your terminal:

```bash
./inventory
```

Or on Windows:

```bash
inventory.exe
```

## How to Use

### Main Menu

When you start the program, you will be prompted to choose a role:

```
Login as:
1. Shopkeeper
2. Customer
3. Exit
Enter choice:
```

### 1\. Shopkeeper Mode

  * You will be asked for a password.

  * **Default Password:** `admin123`

  * Once logged in, you will see the Shopkeeper Menu:

    ```
    --- Shopkeeper Menu ---
    1. Add Product
    2. Display Products
    3. Search Product
    4. Update Product
    5. Delete Product
    6. Logout
    Enter your choice:
    ```

  * All changes are saved to `inventory.txt` immediately.

### 2\. Customer Mode

  * The customer is first shown a list of all available products and the current offers.
  * The program then prompts the customer to add products to their bill.
  * Enter the **Product ID** and **Quantity** for each item you wish to buy.
  * You can add multiple items. When finished, enter 'n' when asked "Add more products? (y/n):".
  * A final bill summary will be displayed.

### 3\. Exit

  * This option safely exits the program.

## Data File

The program creates and uses a file named `inventory.txt` in the same directory to store the product data.

  * **Format:** `id|name|quantity|price`
  * **Example:**
    ```
    101|Sugar|40|45.00
    102|Tea|50|55.00
    ```
