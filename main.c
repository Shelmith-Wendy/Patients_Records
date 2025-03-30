#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATIENTS 100
#define MAX_INPUT_LENGTH 100
#define MAX_HISTORY_LENGTH 500

typedef struct {
    char name[MAX_INPUT_LENGTH];
    int id;
    int age;
    char* medical_history;
} Patient;

Patient patients[MAX_PATIENTS];
int patient_count = 0;

void clear_input_buffer() {
    while (getchar() != '\n');
}

Patient* find_patient(int id) {
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == id) {
            return &patients[i];
        }
    }
    return NULL;
}

void display_patient(Patient* p) {
    if (p == NULL) {
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
        display_patient(&patients[i]);
    }
}

void add_patient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("Maximum patient capacity reached!\n");
        return;
    }

    Patient new_patient;
    printf("\nEnter patient ID: ");
    if (scanf("%d", &new_patient.id) != 1) {
        printf("Invalid ID input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    Patient* existing = find_patient(new_patient.id);
    if (existing != NULL) {
        printf("\nPatient with ID %d already exists:\n", new_patient.id);
        display_patient(existing);
        
        printf("\nDo you want to add to this patient's medical history? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        clear_input_buffer();
        
        if (choice == 'y' || choice == 'Y') {
            printf("\nCurrent medical history: %s\n", existing->medical_history);
            printf("Enter additional medical history: ");
            char new_history[MAX_HISTORY_LENGTH];
            fgets(new_history, MAX_HISTORY_LENGTH, stdin);
            new_history[strcspn(new_history, "\n")] = '\0';

            if (strlen(new_history) > 0) {
                char* updated_history = malloc(strlen(existing->medical_history) + strlen(new_history) + 3);
                if (!updated_history) {
                    printf("Memory allocation failed.\n");
                    return;
                }
                strcpy(updated_history, existing->medical_history);
                strcat(updated_history, ", ");
                strcat(updated_history, new_history);
                free(existing->medical_history);
                existing->medical_history = updated_history;
                printf("\nMedical history updated successfully!\n");
            }
        }
        return;
    }
    
    printf("Enter patient name: ");
    fgets(new_patient.name, MAX_INPUT_LENGTH, stdin);
    new_patient.name[strcspn(new_patient.name, "\n")] = '\0';
    
    printf("Enter patient age: ");
    if (scanf("%d", &new_patient.age) != 1) {
        printf("Invalid age input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    printf("Enter medical history (press Enter for none): ");
    char history[MAX_HISTORY_LENGTH];
    fgets(history, MAX_HISTORY_LENGTH, stdin);
    history[strcspn(history, "\n")] = '\0';
    
    new_patient.medical_history = strdup(strlen(history) ? history : "No medical history recorded");
    if (!new_patient.medical_history) {
        printf("Failed to allocate memory for medical history.\n");
        return;
    }
    
    patients[patient_count++] = new_patient;
    printf("\nPatient added successfully!\n");
    display_patient(&patients[patient_count-1]);
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
        if (patients[i].id == id) {
            printf("\nRemoving patient %s (ID: %d)...\n", patients[i].name, id);
            free(patients[i].medical_history);
            
            for (int j = i; j < patient_count - 1; j++) {
                patients[j] = patients[j + 1];
            }
            
            patient_count--;
            printf("Patient removed successfully.\n");
            return;
        }
    }
    
    printf("Patient with ID %d not found.\n", id);
}

void free_all_memory() {
    for (int i = 0; i < patient_count; i++) {
        free(patients[i].medical_history);
    }
}

bool show_menu_again_prompt() {
    printf("\nWould you like to see the menu again? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    clear_input_buffer();
    return (choice == 'y' || choice == 'Y');
}

void show_menu() {
    printf("\nMain Menu:\n");
    printf("1. Add new patient\n");
    printf("2. Update patient medical history\n");
    printf("3. Remove discharged patient\n");
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