#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1) Définir les structures de données qui seront utilisées dans le cadre de ce programme.

typedef struct Admin {
    char email[50];
    char password[50];
    int recoveryCode;
} Admin;

typedef struct Date{
    int jour;
    int mois;
    int annee;
}Date;

typedef struct Poste{
    char titre[50];
}Poste;

typedef struct Employe{
    int code;
    char nom[20];
    char prenom[20];
    Date date_naissance;
    Poste titre_poste;
    float salaire;

}Employe;



void clearScreen() {
    system("cls");
}


void changePassword(const char* email) {
    FILE* adminFile = fopen("abdallah ismaili - admin.txt", "r+");
    if (adminFile == NULL) {
        printf("Error opening admin file.\n\n");
        return;
    }

    Admin admin;
    FILE* tempFile = tmpfile();

    while (fscanf(adminFile, "%s %s %d", admin.email, admin.password, &admin.recoveryCode) == 3) {
        if (strcmp(email, admin.email) == 0) {
            printf("Enter a new password: ");
            scanf("%s", admin.password);

            fprintf(tempFile, "%s %s %d\n", admin.email, admin.password, admin.recoveryCode);

            clearScreen();
            printf("Password changed successfully.\n\n");
        } else {
            fprintf(tempFile, "%s %s %d\n", admin.email, admin.password, admin.recoveryCode);
        }
    }

    fclose(adminFile);

    adminFile = fopen("abdallah ismaili - admin.txt", "w");
    rewind(tempFile);

    while (fscanf(tempFile, "%s %s %d", admin.email, admin.password, &admin.recoveryCode) == 3) {
        fprintf(adminFile, "%s %s %d\n", admin.email, admin.password, admin.recoveryCode);
    }

    fclose(tempFile);
    fclose(adminFile);
}


int passwordRecovery() {
    char inputEmail[50];
    int inputRecoveryCode;

    printf("Password Recovery\n\n");

    printf("Enter your email: ");
    scanf("%s", inputEmail);

    FILE* adminFile = fopen("abdallah ismaili - admin.txt", "r");

    if (adminFile == NULL) {
        printf("Error opening admin file.\n\n");
        return 0;
    }

    Admin admin;

    while (fscanf(adminFile, "%s %s %d", admin.email, admin.password, &admin.recoveryCode) == 3) {
        if (strcmp(inputEmail, admin.email) == 0) {
            fclose(adminFile);

            printf("Enter the recovery code: ");
            scanf("%d", &inputRecoveryCode);

            if (inputRecoveryCode == admin.recoveryCode) {
                clearScreen();
                printf("Recovery code is valid. You can now change your password.\n");
                changePassword(inputEmail);
                return 1;
            } else {
                clearScreen();
                printf("Invalid recovery code. Password recovery failed.\n");
                return 0;
            }
        }
    }

    fclose(adminFile);

    clearScreen();
    printf("Admin with email %s not found.\n", inputEmail);
    return 0;
}


int login() {
    int maxTrys = 3;
    int trys = 0;

    do {
        char inputEmail[50];
        char inputPassword[50];

        printf("\nWelcome, Please login with your admin account.\n\n");

        printf("1. Login\n");
        printf("2. Password Recovery\n");
        printf("3. Cancel login\n");
        printf("\nEnter your choice (1 or 2 or 3): ");

        int choice;
        int choiceResult = scanf("%d", &choice);

        if (choiceResult != 1) {

            while (getchar() != '\n');
            trys++;
            clearScreen();
            printf("Invalid input. Please enter a valid number.\n");

            continue;

        }

        clearScreen();

        switch (choice) {
            case 1:
                printf("Enter your email: ");
                scanf("%s", inputEmail);

                printf("Enter your password: ");
                scanf("%s", inputPassword);

                FILE* adminFile = fopen("abdallah ismaili - admin.txt", "r");

                if (adminFile == NULL) {
                    printf("Error opening admin file.\n\n");
                    return 0;
                }

                Admin admin;

                while (fscanf(adminFile, "%s %s %d", admin.email, admin.password, &admin.recoveryCode) == 3) {
                    if (strcmp(inputEmail, admin.email) == 0 && strcmp(inputPassword, admin.password) == 0) {
                        fclose(adminFile);
                        return 1;
                    }
                }

                fclose(adminFile);
                printf("Invalid email or password. Login failed.\n\n");
                break;

            case 2:
                if (passwordRecovery()) {

                    printf("Please login with your recovered password.\n\n");

                    printf("Enter your email: ");
                    scanf("%s", inputEmail);

                    printf("Enter your password: ");
                    scanf("%s", inputPassword);

                    FILE* adminFileLogin = fopen("abdallah ismaili - admin.txt", "r");

                    if (adminFileLogin == NULL) {
                        printf("Error opening admin file.\n\n");
                        return 0;
                    }

                    Admin adminLogin;

                    while (fscanf(adminFileLogin, "%s %s %d", adminLogin.email, adminLogin.password, &adminLogin.recoveryCode) == 3) {
                        if (strcmp(inputEmail, adminLogin.email) == 0 && strcmp(inputPassword, adminLogin.password) == 0) {
                            fclose(adminFileLogin);
                            return 1;
                        }
                    }

                    fclose(adminFileLogin);
                    printf("Invalid email or password. Login failed.\n\n");
                }
                break;

            case 3:
                return 0;

            default:
                printf("Invalid choice. Choose between (1-3)\n");
        }

        trys++;
    } while (trys < maxTrys);

    clearScreen();
    printf("Exceeded maximum login attempts. Exiting the program.\n");
    return 0;
}



void operationsList();


int compareByCode(const void* a, const void* b) {
    return ((Employe*)a)->code - ((Employe*)b)->code;
}

int compareByName(const void* a, const void* b) {
    return strcmp(((Employe*)a)->nom, ((Employe*)b)->nom);
}

int compareBySalary(const void* a, const void* b) {
    return ((Employe*)a)->salaire - ((Employe*)b)->salaire;
}

void sortEmployees(Employe* employes, int nombreEmployes, int (*compare)(const void*, const void*)) {
    qsort(employes, nombreEmployes, sizeof(Employe), compare);
}



void loadFromFile(Employe** employes, int* nombreEmployes) {
    FILE* file = fopen("abdallah ismaili - employees.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %d/%d/%d %s %f",
                  &(*employes)[*nombreEmployes].code,
                  (*employes)[*nombreEmployes].nom,
                  (*employes)[*nombreEmployes].prenom,
                  &(*employes)[*nombreEmployes].date_naissance.jour,
                  &(*employes)[*nombreEmployes].date_naissance.mois,
                  &(*employes)[*nombreEmployes].date_naissance.annee,
                  (*employes)[*nombreEmployes].titre_poste.titre,
                  &(*employes)[*nombreEmployes].salaire) == 8) {

        (*nombreEmployes)++;

        if (*nombreEmployes % 100 == 0) {
            printf("Number of employees exceeds the initial array size. Reallocating more memory.\n");

            int newSize = *nombreEmployes + 100;
            *employes = realloc(*employes, newSize * sizeof(Employe));

            if (*employes == NULL) {
                printf("Memory reallocation failed. Exiting.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}



void saveToFile(const Employe* employes, int nombreEmployes) {
    FILE* file = fopen("abdallah ismaili - employees.txt", "w");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < nombreEmployes; i++) {
        fprintf(file, "%d %s %s %02d/%02d/%04d %s %.2f\n",
                employes[i].code,
                employes[i].nom,
                employes[i].prenom,
                employes[i].date_naissance.jour,
                employes[i].date_naissance.mois,
                employes[i].date_naissance.annee,
                employes[i].titre_poste.titre,
                employes[i].salaire);
    }

    fclose(file);
    printf("Saved.\n\n");
}



void displayEmployees(const Employe* employes, int nombreEmployes) {
    printf("Employee List:\n\n");
    printf("Code | Nom | Prenom | Date de Naissance | Titre de Poste | Salaire\n");

    for (int i = 0; i < nombreEmployes; i++) {
        printf("%d | %s | %s | %02d/%02d/%04d | %s | %.2f\n __________________________________________________________________ \n",
               employes[i].code,
               employes[i].nom,
               employes[i].prenom,
               employes[i].date_naissance.jour,
               employes[i].date_naissance.mois,
               employes[i].date_naissance.annee,
               employes[i].titre_poste.titre,
               employes[i].salaire);
    }
}



void displayEmployeeInfo(const Employe* employee) {
    // printf("Code | Nom | Prenom | Date de Naissance | Titre de Poste | Salaire\n");
    printf("\n%d | %s | %s | %02d/%02d/%04d | %s | %.2f\n",
           employee->code,
           employee->nom,
           employee->prenom,
           employee->date_naissance.jour,
           employee->date_naissance.mois,
           employee->date_naissance.annee,
           employee->titre_poste.titre,
           employee->salaire);

    printf("\n");
}



void addEmployee(Employe** employes, int* nombreEmployes) {
    if (*nombreEmployes % 100 == 0) {
        int newSize = *nombreEmployes + 100;
        *employes = realloc(*employes, newSize * sizeof(Employe));

        if (*employes == NULL) {
            printf("Memory reallocation failed. Exiting.\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter the employee's name: ");
    scanf("%s", (*employes)[*nombreEmployes].nom);

    printf("Enter the employee's surname: ");
    scanf("%s", (*employes)[*nombreEmployes].prenom);

    printf("Enter the employee's date of birth (DD/MM/YYYY): ");
    scanf("%d/%d/%d",
          &(*employes)[*nombreEmployes].date_naissance.jour,
          &(*employes)[*nombreEmployes].date_naissance.mois,
          &(*employes)[*nombreEmployes].date_naissance.annee);

    getchar();

    printf("Enter the employee's job title: ");
    fgets((*employes)[*nombreEmployes].titre_poste.titre, sizeof((*employes)[*nombreEmployes].titre_poste.titre), stdin);
    strtok((*employes)[*nombreEmployes].titre_poste.titre, "\n");

    printf("Enter the employee's salary: ");
    scanf("%f", &(*employes)[*nombreEmployes].salaire);

    (*employes)[*nombreEmployes].code = (*employes)[*nombreEmployes - 1].code + 1;

    (*nombreEmployes)++;
    displayEmployeeInfo(&(*employes)[*nombreEmployes - 1]);
    saveToFile(*employes, *nombreEmployes);
}



void deleteEmployee(Employe** employes, int* nombreEmployes) {

    displayEmployees(*employes, *nombreEmployes);

    int employeeCodeToDelete;

    printf("\nEnter the code of the employee you want to delete: ");
    if (scanf("%d", &employeeCodeToDelete) != 1) {
        printf("Invalid input. Please enter a valid employee code.\n");
        while (getchar() != '\n');
        return;
    }

    int indexOfEmployeeToDelete = -1;
    for (int i = 0; i < *nombreEmployes; i++) {
        if ((*employes)[i].code == employeeCodeToDelete) {
            indexOfEmployeeToDelete = i;
            break;
        }
    }

    if (indexOfEmployeeToDelete != -1) {
        printf("\nEmployee information to delete:\n\n");
        displayEmployeeInfo(&(*employes)[indexOfEmployeeToDelete]);

        char confirmDeletion;
        printf("\n >>>Are you sure you want to delete this employee? (y/n): ");
        scanf(" %c", &confirmDeletion);

        if (confirmDeletion == 'y' || confirmDeletion == 'Y') {
            for (int i = indexOfEmployeeToDelete; i < *nombreEmployes - 1; i++) {
                (*employes)[i] = (*employes)[i + 1];
            }

            (*nombreEmployes)--;

            printf("Employee deleted successfully.\n\n");
            saveToFile(*employes, *nombreEmployes);

        } else {
            printf("Deletion canceled.\n\n");

        }
    } else {
        printf("Employee with code %d not found.\n\n", employeeCodeToDelete);

    }
}



void searchEmployee(const Employe* employes, int nombreEmployes) {
    int searchCode;
    char searchName[20];
    char searchSurname[20];
    int found = 0;

    printf("\n1. Search by Code\n");
    printf("2. Search by Name\n");
    printf("3. Search by Surname\n");
    printf("\n>>>Enter your choice (1-3): ");

    int searchOption;
    scanf("%d", &searchOption);

    switch (searchOption) {
        case 1:
            printf(">>>Enter the employee code: ");
            scanf("%d", &searchCode);

            for (int i = 0; i < nombreEmployes; i++) {
                if (employes[i].code == searchCode) {
                    displayEmployeeInfo(&employes[i]);
                    found = 1;
                    return;
                }
            }

            if (!found) {
                printf("No employee found with code %d.\n", searchCode);
            }
            break;

        case 2:
            printf(">>>Enter the employee name: ");
            scanf("%s", searchName);

            for (int i = 0; i < nombreEmployes; i++) {
                if (strcmp(employes[i].nom, searchName) == 0) {
                    displayEmployeeInfo(&employes[i]);
                    found = 1;
                }
            }

            if (!found) {
                printf("No employee found with name %s.\n", searchName);
            }
            break;

        case 3:
            printf(">>>Enter the employee surname: ");
            scanf("%s", searchSurname);

            for (int i = 0; i < nombreEmployes; i++) {
                if (strcmp(employes[i].prenom, searchSurname) == 0) {
                    displayEmployeeInfo(&employes[i]);
                    found = 1;
                }
            }

            if (!found) {
                printf("No employee found with surname %s.\n", searchSurname);
            }
            break;

        default:
            printf("Invalid choice for search. Please enter a number between 1 and 3.\n");
            return;
    }

    printf("Search complete.\n\n");
}



int validateJobTitle(const char* title) {

    const char* validTitles[] = {"President", "VP-Sales", "VP-Marketing", "Sales-Manager", "Sales-Rep"};

    for (int i = 0; i < sizeof(validTitles) / sizeof(validTitles[0]); i++) {
        if (strcmp(title, validTitles[i]) == 0) {
            return 1;
        }
    }

    return 0;
}



void updateEmployee(Employe* employes, int nombreEmployes) {

    displayEmployees(employes, nombreEmployes);

    int updateCode;

    printf("Enter the employee code to update: ");
    scanf("%d", &updateCode);

    int found = 0;

    for (int i = 0; i < nombreEmployes; i++) {
        if (employes[i].code == updateCode) {
            printf("\nCurrent Employee Information:\n");
            displayEmployeeInfo(&employes[i]);

            char OldTitre[50];
            strncpy(OldTitre, employes[i].titre_poste.titre, sizeof(OldTitre) - 1);
            OldTitre[sizeof(OldTitre) - 1] = '\0';

            printf("\nEnter the new job title: ");
            scanf("%s", employes[i].titre_poste.titre);

            if (validateJobTitle(employes[i].titre_poste.titre)) {
                printf("Enter the new salary: ");
                scanf("%f", &employes[i].salaire);

                printf("\nEmployee information updated successfully.\n");
                saveToFile(employes, nombreEmployes);
                displayEmployeeInfo(&employes[i]);

            } else {
                strncpy(employes[i].titre_poste.titre, OldTitre, sizeof(employes[i].titre_poste.titre) - 1);
                employes[i].titre_poste.titre[sizeof(employes[i].titre_poste.titre) - 1] = '\0';  // Ensure null-termination
                printf("Invalid job title. Update canceled.\n");
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No employee found with code %d.\n", updateCode);
    }
}




void convertToCSV(const Employe* employes, int nombreEmployes) {
    FILE* csvFile = fopen("abdallah_ismaili_employees.csv", "w");

    if (csvFile == NULL) {
        printf("Error opening CSV file for writing.\n");
        return;
    }

    fprintf(csvFile, "Code,Nom,Prenom,Date de Naissance,Titre de Poste,Salaire\n");

    for (int i = 0; i < nombreEmployes; i++) {
        fprintf(csvFile, "%d,%s,%s,%02d/%02d/%04d,%s,%.2f\n",
                employes[i].code,
                employes[i].nom,
                employes[i].prenom,
                employes[i].date_naissance.jour,
                employes[i].date_naissance.mois,
                employes[i].date_naissance.annee,
                employes[i].titre_poste.titre,
                employes[i].salaire);
    }

    fclose(csvFile);
    printf("Conversion to CSV successful. CSV file saved as 'abdallah_ismaili_employees.csv'.\n");
}




int main() {

    int loggedIn = login();

    if (loggedIn) {

        int order;
        char response;

        Employe* employes = malloc(100 * sizeof(Employe));;
        int nombreEmployes = 0;

        loadFromFile(&employes, &nombreEmployes);

        clearScreen();
        printf("\nWELCOME TO THE PROGRAM\n\n");

        do {
            operationsList();
            printf(">>> Please choose an operation (1-8): ");

            if (scanf("%d", &order) != 1) {
                printf("Invalid input. Please enter a number.\n");
                while (getchar() != '\n');
                continue;
            }

            switch (order) {
                case 1:
                    printf("\nYou chose: Ajouter de nouveaux employes\n");

                    addEmployee(&employes, &nombreEmployes);

                    printf(">>>Do you want to add another employee? (y/n): ");
                    scanf(" %c", &response);

                    while (response == 'y' || response == 'Y') {
                        printf("\n");

                        addEmployee(&employes, &nombreEmployes);
                        printf(">>>Do you want to add another employee? (y/n): ");
                        scanf(" %c", &response);

                        clearScreen();
                    }

                    clearScreen();

                    break;



                case 2:
                    printf("\nYou chose: Supprimer des employes existants\n");

                    deleteEmployee(&employes, &nombreEmployes);

                    printf(">>>Do you want to delete another employee? (y/n): ");
                    scanf(" %c", &response);

                    while (response == 'y' || response == 'Y') {
                        printf("\n");

                        deleteEmployee(&employes, &nombreEmployes);
                        printf(">>>Do you want to delete another employee? (y/n): ");
                        scanf(" %c", &response);
                        clearScreen();
                    }

                    clearScreen();
                    printf("\n");
                    break;

                case 3:
                    printf("\nYou chose: Rechercher un employe\n");

                    searchEmployee(employes, nombreEmployes);


                    printf(">>>Do you want to search another employee? (y/n): ");
                    scanf(" %c", &response);

                    while (response == 'y' || response == 'Y') {
                            clearScreen();
                        printf("\n");

                        searchEmployee(employes, nombreEmployes);
                        printf(">>>Do you want to search another employee? (y/n): ");
                        scanf(" %c", &response);

                    }

                    clearScreen();
                    printf("\n");
                    break;

                case 4:
                    printf("\nYou chose: Mettre à jour les informations d'un certain employe\n");

                    updateEmployee(employes, nombreEmployes);


                    printf(">>>Do you want to update another employee? (y/n): ");
                    scanf(" %c", &response);

                    while (response == 'y' || response == 'Y') {
                        printf("\n");

                        updateEmployee(employes, nombreEmployes);
                        printf(">>>Do you want to update another employee? (y/n): ");
                        scanf(" %c", &response);

                        clearScreen();
                    }

                    clearScreen();
                    printf("\n");
                    break;

                case 5:
                    clearScreen();
                    printf("\nYou chose: Enregistrer les informations dans un fichier\n");

                    saveToFile(employes, nombreEmployes);

                    printf("\n");
                    break;

                case 6:
                    printf("\nYou chose: Afficher toute les employees\n");

                    // displayEmployees(employes, nombreEmployes);

                    printf("\nSort by:\n");
                    printf("1. Code\n");
                    printf("2. Name\n");
                    printf("3. Salary\n");
                    printf("4. Return to main menu\n");

                    int sortOption;
                    printf(">>>Enter your choice (1-4): ");
                    scanf("%d", &sortOption);

                    switch (sortOption) {
                        case 1:
                            sortEmployees(employes, nombreEmployes, compareByCode);
                            break;
                        case 2:
                            sortEmployees(employes, nombreEmployes, compareByName);
                            break;
                        case 3:
                            sortEmployees(employes, nombreEmployes, compareBySalary);
                            break;
                        case 4:
                            break;
                        default:
                            printf("Invalid choice. Returning to the main menu.\n");
                            break;
                    }

                    displayEmployees(employes, nombreEmployes);

                    printf("\n");
                    break;

                case 7:
                    clearScreen();
                    printf("\nYou chose: Convertir les informations en fichier CSV\n");
                    convertToCSV(employes, nombreEmployes);
                    printf("\n");
                    break;

                case 8:
                    clearScreen();
                    printf("\nYou chose: Quitter le programme\n");
                    break;

                default:
                    clearScreen();
                    printf("Invalid choice. Please enter a number between 1 and 6.\n");
            }
        } while (order != 8);

        free(employes);
        clearScreen();
        printf("Exiting the program. Goodbye!\n");
        return 0;

    } else {
        clearScreen();
        printf("Login failed. Exiting the program.\n\n");
        return 0;
    }

    return 0;
}



void operationsList() {
    printf("\n");
    printf("OPERATIONS MENU:\n\n");
    printf("1. Ajouter de nouveaux employes\n");
    printf("2. Supprimer des employes existants\n");
    printf("3. Rechercher un employe\n");
    printf("4. Mettre à jour les informations d'un certain employe\n");
    printf("5. Enregistrer les informations dans un fichier\n");
    printf("6. Afficher toute les employees\n");
    printf("7. Convertir les informations en fichier CSV\n\n");
    printf("8. Quitter le programme\n\n");
}


