#!/bin/bash

# Factorial with iteration
factorial_iterative() {
    local n=$1
    local result=1
    for ((i=1; i<=n; i++)); do
        result=$((result * i))
    done
    echo "$result"
}

# Factorial with recursion
factorial_recursive() {
    local n=$1
    if [ $n -eq 0 ] || [ $n -eq 1 ]; then
        echo 1
    else
        echo $((n * $(factorial_recursive $((n-1)))))
    fi
}

# Main menu function
main_menu() {
    while true; do
        clear
        echo "Factorial Calculator"
        echo "1. Calculate Factorial (Iterative)"
        echo "2. Calculate Factorial (Recursive)"
        echo "3. Exit"
        read -p "Enter your choice (1-3): " choice

        case $choice in
            1)
                read -p "Enter a number to calculate the factorial: " number
                if ! [[ "$number" =~ ^[0-9]+$ ]]; then
                    echo "Error: Invalid input. Please enter a positive integer."
                    read -p "Press Enter to continue..."
                    continue
                fi
                echo "Factorial of $number (iterative): $(factorial_iterative $number)"
                read -p "Press Enter to continue..."
                ;;
            2)
                read -p "Enter a number to calculate the factorial: " number
                if ! [[ "$number" =~ ^[0-9]+$ ]]; then
                    echo "Error: Invalid input. Please enter a positive integer."
                    read -p "Press Enter to continue..."
                    continue
                fi
                echo "Factorial of $number (recursive): $(factorial_recursive $number)"
                read -p "Press Enter to continue..."
                ;;
            3)
                echo "Exiting..."
                break
                ;;
            *)
                echo "Error: Invalid choice. Please try again."
                read -p "Press Enter to continue..."
                ;;
        esac
    done
}

# Run the main menu
main_menu