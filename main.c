#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//<stdlib.h> contains a function system() we can run other code's exe files
// through it

// console colors
// collected from https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

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
                system("admin.exe");
            }
            else
            {
                printf(RED "Sorry authentication failed!\nRestart the app.exe or (Press enter)\n" RESET);
                getch();
            }
            break;
        case 2:
            printf("\n" CYAN "Redirecting to user page..." RESET);
            system("user.exe");
            break;
        case 3:
            t = 0;
            printf(BLUE "Thanks for using.Press any key to close :)" RESET);
            getch();
            break;
        default:
            printf(RED "Invallid choice. Try again." RESET "\n\n");
            break;
        }
    }

    return 0;
}