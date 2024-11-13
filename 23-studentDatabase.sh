#!/bin/bash

# Student database file
DATABASE_FILE="students.txt"

# Function to insert a new student
insert_student() {
    echo "Enter student details:"
    read -p "Name: " name
    read -p "ID: " id
    read -p "Grade: " grade

    # Append the new student to the database file
    echo "$name,$id,$grade" >> "$DATABASE_FILE"
    echo "Student added successfully."
}

# Function to delete a student
delete_student() {
    read -p "Enter student ID to delete: " id_to_delete

    # Read the database file and exclude the matching line
    grep -v ",$id_to_delete," "$DATABASE_FILE" > temp.txt
    mv temp.txt "$DATABASE_FILE"

    echo "Student deleted successfully."
}

# Function to update a student's grade
update_student() {
    read -p "Enter student ID to update: " id_to_update
    read -p "Enter new grade: " new_grade

    # Read the database file, replace the grade for the matching line, and write back to the file
    awk -F"," -v id="$id_to_update" -v new_grade="$new_grade" '
        $2 == id {$3 = new_grade}
        {print $1 "," $2 "," $3}
    ' "$DATABASE_FILE" > temp.txt
    mv temp.txt "$DATABASE_FILE"

    echo "Student grade updated successfully."
}

# Function to search for a student
search_student() {
    read -p "Enter student name or ID to search: " search_term

    # Search the database file and display matching records
    grep "$search_term" "$DATABASE_FILE" | while IFS="," read name id grade; do
        echo "Name: $name, ID: $id, Grade: $grade"
    done
}

# Function to display the contents of the database file
display_database() {
    echo "Student Database:"
    echo "---------------"
    cat "$DATABASE_FILE" | while IFS="," read name id grade; do
        echo "Name: $name, ID: $id, Grade: $grade"
    done
    echo "---------------"
}

# Main menu
while true; do
    echo "Student Database Management"
    echo "1. Insert"
    echo "2. Delete"
    echo "3. Update"
    echo "4. Search"
    echo "5. Display Database"
    echo "6. Exit"

    read -p "Enter your choice (1-6): " choice

    case $choice in
        1) insert_student ;;
        2) delete_student ;;
        3) update_student ;;
        4) search_student ;;
        5) display_database ;;
        6) echo "Exiting..." && break ;;
        *) echo "Invalid choice. Please try again." ;;
    esac

    echo
done