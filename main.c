#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 4
#define MAX_INPUT_LENGTH 100
#define MAX_HISTORY_LENGTH 500

typedef struct {
    char* name;
    int id;  // Auto-assigned
    int age;
    char* medical_history;
} Patient;

Patient** patients = NULL;
int patient_count = 0;
int patient_capacity = 0;
int next_id = 1;  // Auto-incrementing ID counter

void clear_input_buffer() {
    while (getchar() != '\n');
}

Patient* find_patient(int id) {
    for (int i = 0; i < patient_count; i++) {
        if (patients[i]->id == id) {
            return patients[i];
        }
    }
    return NULL;
}

void display_patient(Patient* p) {
    if (!p) {
        printf("Patient not found.\n");
        return;
    }
    printf("\nPatient Details:\n");
    printf("ID: %d\n", p->id);
    printf("Name: %s\n", p->name);
    printf("Age: %d\n", p->age);
    printf("Medical History: %s\n", p->medical_history);
    printf("----------------------------\n");
}

void display_all_patients() {
    if (patient_count == 0) {
        printf("\nNo patients in the system.\n");
        return;
    }
    
    printf("\nAll Patients (%d):\n", patient_count);
    for (int i = 0; i < patient_count; i++) {
        display_patient(patients[i]);
    }
}

void expand_patient_array() {
    if (patient_count >= patient_capacity) {
        patient_capacity = (patient_capacity == 0) ? INITIAL_CAPACITY : patient_capacity * 2;
        patients = realloc(patients, patient_capacity * sizeof(Patient*));
        if (!patients) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void add_patient() {
    expand_patient_array();
    
    Patient* new_patient = malloc(sizeof(Patient));
    if (!new_patient) {
        fprintf(stderr, "Failed to allocate patient\n");
        exit(EXIT_FAILURE);
    }

    // Auto-assign ID
    new_patient->id = next_id++;
    clear_input_buffer();
    
    printf("\nEnter patient name: ");
    char input_name[MAX_INPUT_LENGTH];
    fgets(input_name, MAX_INPUT_LENGTH, stdin);
    input_name[strcspn(input_name, "\n")] = '\0';

    new_patient->name = malloc(strlen(input_name) + 1);
    if (!new_patient->name) {
        fprintf(stderr, "Failed to allocate memory for name \n");
        free(new_patient);
        exit(EXIT_FAILURE);
    }
    strcpy(new_patient->name, input_name);
    
    printf("Enter patient age: ");
    char age_input[10];
    fgets(age_input, sizeof(age_input), stdin);
    new_patient->age = atoi(age_input);
    
    
    printf("Enter medical history (press Enter for none): ");
    char history[MAX_HISTORY_LENGTH];
    fgets(history, MAX_HISTORY_LENGTH, stdin);
    history[strcspn(history, "\n")] = '\0';
    
    new_patient->medical_history = strdup(strlen(history) ? history : "No medical history recorded");
    if (!new_patient->medical_history) {
        fprintf(stderr, "Failed to allocate medical history\n");
        free(new_patient);
        exit(EXIT_FAILURE);
    }
    
    patients[patient_count++] = new_patient;
    printf("\nPatient added successfully!");
    printf("\n----------------------------");
    display_patient(new_patient);
}

void update_patient_history() {
    int id;
    printf("\nEnter patient ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    Patient* patient = find_patient(id);
    if (!patient) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }
    
    printf("\nCurrent medical history for %s (ID: %d):\n%s\n", 
           patient->name, patient->id, patient->medical_history);
    
    printf("\nEnter additional medical history (press Enter to cancel): ");
    char new_history[MAX_HISTORY_LENGTH];
    fgets(new_history, MAX_HISTORY_LENGTH, stdin);
    new_history[strcspn(new_history, "\n")] = '\0';
    
    if (strlen(new_history) == 0) {
        printf("Update canceled.\n");
        return;
    }

    if (strcmp(patient->medical_history, "No medical history recorded") == 0) {
        free(patient->medical_history);
        patient->medical_history = strdup(new_history);
    } else {
        char* updated_history = malloc(strlen(patient->medical_history) + strlen(new_history) + 3);
        if (!updated_history) {
            printf("Failed to allocate memory for updated history.\n");
            return;
        }
        strcpy(updated_history, patient->medical_history);
        strcat(updated_history, ", ");
        strcat(updated_history, new_history);
        free(patient->medical_history);
        patient->medical_history = updated_history;
    }
    
    printf("\nMedical history updated successfully!\n");
    display_patient(patient);
}

void remove_patient() {
    int id;
    printf("\nEnter patient ID to remove: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    for (int i = 0; i < patient_count; i++) {
        if (patients[i]->id == id) {
            printf("\nRemoving patient %s (ID: %d)...\n", patients[i]->name, id);
            free(patients[i]->name);
            free(patients[i]->medical_history);
            free(patients[i]);
            
            for (int j = i; j < patient_count - 1; j++) {
                patients[j] = patients[j + 1];
            }
            
            patient_count--;
            
            // Optional: shrink array if mostly empty
            if (patient_capacity > INITIAL_CAPACITY && patient_count < patient_capacity / 4) {
                patient_capacity /= 2;
                patients = realloc(patients, patient_capacity * sizeof(Patient*));
            }
            
            printf("Patient removed successfully.\n");
            return;
        }
    }
    
    printf("Patient with ID %d not found.\n", id);
}

void free_all_memory() {
    for (int i = 0; i < patient_count; i++) {

        free(patients[i]->name);
        free(patients[i]->medical_history);
        free(patients[i]);
    }
    free(patients);
    patients = NULL;
    patient_count = patient_capacity = 0;
    next_id = 1;  // Reset ID counter
}

bool show_menu_again_prompt() {
    printf("\nWould you like to see the menu again? (y)(else exit system): ");
    char choice;
    scanf(" %c", &choice);
    clear_input_buffer();
    if(choice == 'y' || choice == 'Y') {
        return true;
    }
    printf("Exiting the system");
    free_all_memory();
    exit(0);
}

void show_menu() {
    printf("\nMain Menu:\n");
    printf("1. Add new patient\n");
    printf("2. Update patient medical history\n");
    printf("3. Discharge patient\n");
    printf("4. View patient details\n");
    printf("5. View all patients\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    printf("=== Hospital Patient Management System ===\n");
    
    bool show_menu_flag = true;
    
    while (1) {
        if (show_menu_flag) {
            show_menu();
        }
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1: add_patient(); break;
            case 2: update_patient_history(); break;
            case 3: remove_patient(); break;
            case 4: {
                int id;
                printf("\nEnter patient ID to view: ");
                scanf("%d", &id);
                display_patient(find_patient(id));
                break;
            }
            case 5: display_all_patients(); break;
            case 6: 
                free_all_memory();
                printf("\nExiting system. All memory freed.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        show_menu_flag = show_menu_again_prompt();
    }
}