#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For rand(), srand()
#include <time.h>

// console colors
// collected from https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

void generate_crads(int count, int minute)
{
    FILE *fptr;
    switch (minute)
    {
    case 100:
        fptr = fopen("unused_cards/100.txt", "a+");
        break;
    case 60:
        fptr = fopen("unused_cards/60.txt", "a+");
        break;
    case 40:
        fptr = fopen("unused_cards/40.txt", "a+");
        break;
    default:
        printf(RED "Invallid choice. Try again." RESET);
        break;
    }

    srand(time(NULL));
    for (int i = 1; i <= count; i++)
    {
        // this generate a 20 digit number in string format
        char card[21] = "";
        for (int j = 0; j < 20; j++)
        {
            int num = (rand() % 9) + 1; // int 0-9
            char random[2] = {num + '0', '\0'};
            strcat(card, random);
        }
        fputs(card, fptr);
        fputs("\n", fptr);
    }
    fclose(fptr);
    printf(GREEN "Successfully created %d cards of %d minutes." RESET "\n", count, minute);
}

void delete_card()
{
    char card_to_delete[21];
    int minute;
    printf("Enter minute type (40, 60, 100): ");
    scanf("%d", &minute);
    printf("Enter card number to delete: ");
    scanf("%20s", card_to_delete);

    char filename[64];
    if (minute == 100)
        strcpy(filename, "unused_cards/100.txt");
    else if (minute == 60)
        strcpy(filename, "unused_cards/60.txt");
    else if (minute == 40)
        strcpy(filename, "unused_cards/40.txt");
    else
    {
        printf(RED "Invalid minute type.\n");
        return;
    }

    FILE *src = fopen(filename, "r+");
    FILE *tmp = fopen("unused_cards/temp.txt", "a+");
    if (!src || !tmp)
    {
        printf("File error.\n");
        if (src)
            fclose(src);
        if (tmp)
            fclose(tmp);
        return;
    }

    char line[32];
    int found = 0;
    while (fgets(line, sizeof(line), src))
    {
        // Remove newline for comparison
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, card_to_delete) != 0)
            fprintf(tmp, "%s\n", line);
        else
            found = 1;
    }
    fclose(src);
    fclose(tmp);

    if (found)
    {
        remove(filename);
        rename("unused_cards/temp.txt", filename);
        printf(GREEN "Card deleted successfully.\n" RESET);
    }
    else
    {
        remove("unused_cards/temp.txt");
        printf(RED "Card not found.\n" RESET);
    }
}

void unblock_number(char number[])
{
    FILE *blocked_nums = fopen("blocked_numbers/numbers.txt", "r");
    FILE *blocked_nums_tmp = fopen("blocked_numbers/temp.txt", "a+");

    char line[12];
    int found = 0;
    while (fgets(line, sizeof(line), blocked_nums))
    {
        // Remove newline for comparison
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, number) != 0)
            fprintf(blocked_nums_tmp, "%s\n", line);
        else
            found = 1;
    }
    fclose(blocked_nums);
    fclose(blocked_nums_tmp);
    if (found)

        printf(GREEN "Success! \n%s has been removed from blocked list.\n" RESET, number);
    else
        printf(RED "%s dosen't exists in blocked list.\n" RESET, number);

    remove("blocked_numbers/numbers.txt");
    rename("blocked_numbers/temp.txt", "blocked_numbers/numbers.txt");
}

void statistics()
{
    int c_40 = 0, c_60 = 0, c_100 = 0;
    int uc_40 = 0, uc_60 = 0, uc_100 = 0;
    FILE *tc_40 = fopen("used_cards/40.txt", "r");
    FILE *tc_60 = fopen("used_cards/60.txt", "r");
    FILE *tc_100 = fopen("used_cards/100.txt", "r");
    FILE *utc_40 = fopen("unused_cards/40.txt", "r");
    FILE *utc_60 = fopen("unused_cards/60.txt", "r");
    FILE *utc_100 = fopen("unused_cards/100.txt", "r");

    char line[32];
    while (fgets(line, sizeof(line), tc_40))
    {
        line[strcspn(line, "\n")] = 0;
        c_40++;
    }
    while (fgets(line, sizeof(line), tc_60))
    {
        line[strcspn(line, "\n")] = 0;
        c_60++;
    }
    while (fgets(line, sizeof(line), tc_100))
    {
        line[strcspn(line, "\n")] = 0;
        c_100++;
    }
    while (fgets(line, sizeof(line), utc_40))
    {
        line[strcspn(line, "\n")] = 0;
        uc_40++;
    }
    while (fgets(line, sizeof(line), utc_60))
    {
        line[strcspn(line, "\n")] = 0;
        uc_60++;
    }
    while (fgets(line, sizeof(line), utc_100))
    {
        line[strcspn(line, "\n")] = 0;
        uc_100++;
    }
    printf("\nTotal cards sold:\t40min: %d\t60min: %d\t100min: %d\n", c_40, c_60, c_100);
    printf("Total remaining cards:\t40min: %d\t60min: %d\t100min: %d\n", uc_40, uc_60, uc_100);
    printf("Total amount sold in TK:40min: %d\t60min: %d\t100min: %d\n", c_40 * 50, c_60 * 70, c_100 * 120);
}

void history()
{
    printf("\nCard NO\t\t     Date\tTime  Min Mobile NO\n");
    FILE *history = fopen("admin/history.txt", "r");
    char line[65];
    while (fgets(line, sizeof(line), history))
    {
        printf("%s", line);
    }
    fclose(history);
}
void search()
{
    char number[12];
    printf("Enter mobile number: ");
    scanf("%11s", number);
    printf("\nCard NO\t\t     Date\tTime  \n");

    FILE *history = fopen("admin/history.txt", "r");
    char line[65];
    while (fgets(line, sizeof(line), history))
    {
        char card[21];
        char date[15];
        char time[15];
        int min;
        char h_number[12];
        // Parse the string
        sscanf(line, "%20s %14s %14s %d %11s", card, date, time, &min, h_number);

        if (strcmp(h_number, number) == 0)
        {
            printf("%s %s %s %d", card, date, time, min);
        }
    }
    printf("\n");
    fclose(history);
}

int main()
{
    int t = 1;
    while (t)
    {
        int option;
        printf("\n1. New card \n2. Delete card \n3. Unlock number \n4. History \n5. Statistics \n6. Search \n7. Exit\n"
               "[" MAGENTA "ADMIN" RESET "] Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            int count, minute;
            printf("Enter how many cards and of what minutes: ");
            scanf("%d %d", &count, &minute);
            generate_crads(count, minute);
            break;
        case 2:
            delete_card();
            break;
        case 3:
            char number[12];
            printf("Enter the number you want to unblock: ");
            scanf("%11s", number);
            unblock_number(number);
            break;
        case 4:
            history();
            break;
        case 5:
            statistics();
            break;
        case 6:
            search();
            break;
        case 7:
            printf(BLUE "Thank you for using :)\n" RESET);
            printf(CYAN "Redirecting to homepage...\n" RESET);
            system("main.exe");
            t = 0;
            break;
        default:
            printf(RED "Invalid choice. Please try again!" RESET);
            break;
        }
    }
    return 0;
}