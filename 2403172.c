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

// global variables
int unsuccessful_recharge;

int Admin();
int User();
int is_exist(char number[], FILE *reference)
{
    char line[32];
    int found = 0;
    while (fgets(line, sizeof(line), reference))
    {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, number) == 0)
            found++;
    }
    fclose(reference);
    return found;
}

int main()
{
    int t = 1;
    while (t)
    {
        int choice;
        printf("\n1. Admin page\n2. User page\n3. Exit \n");
        printf("[" MAGENTA "MAIN" RESET "] Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // admin page
            printf("Enter Admin password: ");
            int password;
            scanf("%d", &password);
            if (password == 2403172)
            {
                // logged in
                printf(GREEN "Login successful!" RESET "\n");
                printf(CYAN "Redirecting to admin page..." RESET "\n");
                Admin();
            }
            else
            {
                printf(RED "Sorry authentication failed!\nRestart the app.exe or (Press enter)\n" RESET);
                getchar();
            }
            break;
        case 2:
            printf(CYAN "Redirecting to user page...\n" RESET);
            User();
            break;
        case 3:
            printf(BLUE "Thanks for using. Press Enter to close :)" RESET);
            getchar();
            break;
        default:
            printf(RED "Invallid choice. Try again." RESET "\n\n");
            break;
        }
    }

    return 0;
}

// admin section starts here
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
    if (!history)
    {
        // Try to create the file if it doesn't exist
        history = fopen("admin/history.txt", "w");
        if (!history)
        {
            printf(RED "Error: Could not create history file.\n" RESET);
            return;
        }
        fclose(history);
        printf(YELLOW "No history found.\n" RESET);
        return;
    }
    char line[80];
    while (fgets(line, sizeof(line), history))
    {
        printf("%s", line);
    }
    fclose(history);
}

void search()
{
    char number[12];
    int num_size = 0;
    printf("Please enter your number: ");
    scanf("%11s", number);
    for (int i = 1; number[i] != '\0'; i++)
        num_size = i + 1;
    if (num_size != 11 || number[0] != '0' || number[1] != '1')
    {
        printf(RED "Please enter a valid number\n" RESET);
        return;
    }
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

int Admin()
{
    char number[12];
    int num_size = 0;
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
            printf("Enter how many cards and of what minutes(40, 60, 100): ");
            scanf("%d %d", &count, &minute);
            generate_crads(count, minute);
            break;
        case 2:
            delete_card();
            break;
        case 3:
            printf("Please enter your number: ");
            scanf("%11s", number);
            for (int i = 1; number[i] != '\0'; i++)
                num_size = i + 1;

            if (num_size == 11 && number[0] == '0' && number[1] == '1')
                unblock_number(number);
            else
                printf(RED "Please enter a valid number\n" RESET);

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
            t = 0;
            break;
        default:
            printf(RED "Invalid choice. Please try again!" RESET);
            break;
        }
    }
    return 0;
}
// admin section ends here

// user section starts here
void check_balance()
{
    FILE *balance = fopen("user/balance.txt", "r+");
    int b = 0;
    fscanf(balance, "%d", &b);
    fclose(balance);
    printf("You have %d minutes in your account.\n", b);
}

void recharge(char u_number[])
{
    unsuccessful_recharge = 3;
    char card_to_recharge[21];
    int minute, c, taka;
    printf("\n1. 40 minutes at 50 taka \n2. 60 minutes at 70 taka \n3. 100 minutes at 120 taka \n\n"
           "[" MAGENTA "USER" RESET "] Please choose a package: ");
    scanf("%d", &c);
    switch (c)
    {
    case 1:
        minute = 40;
        taka = 50;
        break;
    case 2:
        minute = 60;
        taka = 70;
        break;
    case 3:
        minute = 100;
        taka = 120;
        break;
    default:
        printf(RED "Invalid choice. Please try again.\n" RESET);
        return;
        break;
    }

    while (unsuccessful_recharge)
    {
        printf("Enter card number to use.\n" YELLOW "You have %d chances left: \n" RESET, unsuccessful_recharge);
        scanf("%20s", card_to_recharge);

        char unused_filename[64], used_filename[64], balance_filename[64];
        if (minute == 100)
        {
            strcpy(used_filename, "used_cards/100.txt");
            strcpy(unused_filename, "unused_cards/100.txt");
            strcpy(balance_filename, "admin/balance/100.txt");
        }
        else if (minute == 60)
        {
            strcpy(used_filename, "used_cards/60.txt");
            strcpy(unused_filename, "unused_cards/60.txt");
            strcpy(balance_filename, "admin/balance/60.txt");
        }
        else if (minute == 40)
        {
            strcpy(used_filename, "used_cards/40.txt");
            strcpy(unused_filename, "unused_cards/40.txt");
            strcpy(balance_filename, "admin/balance/40.txt");
        }
        else
        {
            printf(RED "Invalid minute type.\n" RESET);
            return;
        }

        FILE *unused_src = fopen(unused_filename, "r+");
        int found = is_exist(card_to_recharge, unused_src);

        // successful recharge
        if (found)
        {
            // adding the card number in the used_cards file
            char line[32];
            FILE *unused_src = fopen(unused_filename, "r+");
            FILE *unused_tmp = fopen("unused_cards/temp.txt", "a+");
            while (fgets(line, 32, unused_src))
            {
                line[strcspn(line, "\n")] = 0;
                if (strcmp(line, card_to_recharge) != 0)
                {
                    fputs(line, unused_tmp);
                    fputs("\n", unused_tmp);
                }
            }
            fclose(unused_src);
            fclose(unused_tmp);
            remove(unused_filename);
            rename("unused_cards/temp.txt", unused_filename);

            FILE *used_src = fopen(used_filename, "a+");
            fputs(card_to_recharge, used_src);
            fputs("\n", used_src);
            fclose(used_src);

            // adding the minutes in the user's balance
            FILE *balance = fopen("user/balance.txt", "r+");
            FILE *balance_tmp = fopen("user/balance_temp.txt", "w+");
            int prev_balance = 0;
            fscanf(balance, "%d", &prev_balance);
            fprintf(balance_tmp, "%d", minute + prev_balance);
            fclose(balance_tmp);
            fclose(balance);
            remove("user/balance.txt");
            rename("user/balance_temp.txt", "user/balance.txt");

            // Get current date and time
            char date[15], time_str[15];
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            strftime(date, sizeof(date), "%d-%m-%Y", t);
            strftime(time_str, sizeof(time_str), "%H:%M", t);

            // adding transaction to admin history
            FILE *history = fopen("admin/history.txt", "a+");
            char transaction[80];
            snprintf(transaction, sizeof(transaction), "%s %s %s %d %s", card_to_recharge, date, time_str, minute, u_number);
            fprintf(history, "%s\n", transaction);
            fclose(history);

            switch (minute)
            {
            case 40:
                printf(GREEN "Successfully added 40 minutes to your account.\n" RESET);
                break;
            case 60:
                printf(GREEN "Successfully added 60 minutes to your account.\n" RESET);
                break;
            case 100:
                printf(GREEN "Successfully added 100 minutes to your account.\n" RESET);
                break;
            }
            return;
        }
        else
        {
            // remove("unused_cards/temp.txt");
            printf(RED "Card not found.\n" RESET);
            unsuccessful_recharge--;
            if (unsuccessful_recharge == 0)
            {
                FILE *blocked_num = fopen("blocked_numbers/numbers.txt", "a+");
                fputs(u_number, blocked_num);
                fputs("\n", blocked_num);
                fclose(blocked_num);
                printf(RED "You have been blocked!\n" RESET);
                return;
            }
        }
    }
}

int User()
{
    char u_number[12];
    int num_size = 0;
    printf("\nPlease enter your number: ");
    scanf("%11s", u_number);
    for (int i = 1; u_number[i] != '\0'; i++)
        num_size = i + 1;

    if (num_size < 11 || u_number[0] != '0' || u_number[1] != '1')
    {
        printf(RED "Please enter a valid number\n" RESET);
        return 0;
    }

    int t = 1;
    while (t)
    {
        // checks if this user's number exist in the block list. if it exist then he can not access any functionality
        FILE *blocked_num = fopen("blocked_numbers/numbers.txt", "r+");
        int found = is_exist(u_number, blocked_num);
        if (found)
        {
            printf(RED "This number is blocked!\n" RESET);
            break;
            ;
        }

        int choice;
        printf("\n1. Check balance\n2. Recharge\n3. Exit \n"
               "[" MAGENTA "USER" RESET "] Please enter your option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            check_balance();
            break;
        case 2:
            recharge(u_number);
            break;
        case 3:
            t = 0;
            break;
        default:
            printf(RED "Invallid choice. Try again." RESET);
            break;
        }
    }
    return 0;
}