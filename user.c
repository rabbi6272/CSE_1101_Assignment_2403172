#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h> // For time()

// console colors
// collected from https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

void check_balance()
{
    FILE *balance = fopen("user/balance.txt", "r+");
    int b = 0;
    fscanf(balance, "%d", &b);
    fclose(balance);
    printf("You have %d minutes in your account.\n", b);
}

void recharge()
{
    static int not_found = 0;
    char number[12];
    int num_size = 0;

    if (not_found != 3)
    {
        printf("Please enter your number: ");
        scanf("%11s", number);
        for (int i = 0; number[i] != '\0'; i++)
            num_size++;
        if (num_size < 11 || number[0] != '0' || number[1] != '1')
        {
            printf(RED "Please enter a valid number\n" RESET);
            return;
        }

        FILE *blocked_num = fopen("blocked_numbers/numbers.txt", "a+");
        char num[32];
        int found_num = 0;
        while (fgets(num, sizeof(num), blocked_num))
        {
            // Remove newline for comparison
            num[strcspn(num, "\n")] = 0;
            if (strcmp(num, number) == 0)
                found_num = 1;
        }
        if (found_num)
        {
            printf(RED "This number is blocked!\n" RESET);
            return;
        }

        char card_to_recharge[21];
        int minute, c, taka;
        printf("1. 40 minutes at 50 taka \n2. 60 minutes at 70 taka \n3. 100 minutes at 120 taka \n"
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
        printf("Enter card number to use: ");
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
        FILE *unused_tmp = fopen("unused_cards/temp.txt", "a+");
        FILE *used_src = fopen(used_filename, "a+");

        char line[32];
        int found = 0;
        while (fgets(line, sizeof(line), unused_src))
        {
            line[strcspn(line, "\n")] = 0;
            if (strcmp(line, card_to_recharge) != 0)
                fprintf(unused_tmp, "%s\n", line);
            else
                found = 1;
        }
        fclose(unused_src);
        fclose(unused_tmp);

        // successful recharge
        if (found)
        {
            // adding the card number in the used_cards file
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
            char transaction[65];
            sprintf(transaction, "%s %s %s %d %s", card_to_recharge, date, time_str, minute, number);
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
        }
        else
        {
            // remove("unused_cards/temp.txt");
            printf(RED "Card not found.\n" RESET);
            not_found++;

            if (not_found == 3)
            {
                fputs(number, blocked_num);
                fputs("\n", blocked_num);
                fclose(blocked_num);
            }
        }
        remove(unused_filename);
        rename("unused_cards/temp.txt", unused_filename);
    }
    else
    {
        printf(RED "You have been blocked!\n" RESET);
    }
}

int main()
{

    int t = 1;
    while (t)
    {
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
            recharge();
            break;
        case 3:
            t = 0;
            printf(BLUE "Thank you for using :)" RESET "\n" CYAN "Redirecting to homepage...\n" RESET);
            system("main.exe");
            break;
        default:
            printf(RED "Invallid choice. Try again." RESET);
            break;
        }
    }
    return 0;
}