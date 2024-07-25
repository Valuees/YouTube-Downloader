#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL_LENGTH 99
#define MAX_PATH_LENGTH 256
#define COMMAND_SIZE 512
#define PATH_FILE "download_path.txt"

int checker = 0, n;
char url[MAX_URL_LENGTH];
char default_path[MAX_PATH_LENGTH] = "\\playlist";
char command[COMMAND_SIZE];
char path[MAX_PATH_LENGTH];

// Function prototypes
void clearScreen();
void printErrorAndExit(const char *message);
void checkF();
void title();
void mp3DL_check();
void mp4DL_check();
void setDefaultPath();
void choosePath();
int executeCommand(const char *cmd);
void loadPathFromFile();
void savePathToFile();

int yt_dl_mp3()
{
    snprintf(command, COMMAND_SIZE, "yt-dlp -f 140 -P %s %s", path, url);
    if (executeCommand(command) == 0)
    {
        snprintf(command, COMMAND_SIZE, "cd %s && rename *.m4a *.mp3", path);
        executeCommand(command);
        printf("Download completed successfully.\n");
        return 0;
    }
    else
    {
        printf("Download failed.\n");
        return -1;
    }
}

int yt_dl_mp4()
{
    snprintf(command, COMMAND_SIZE, "yt-dlp -f 22 -P %s %s", path, url);
    if (executeCommand(command) == 0)
    {
        printf("Download completed successfully.\n");
        return 0;
    }
    else
    {
        printf("Download failed.\n");
        return -1;
    }
}

int executeCommand(const char *cmd)
{
    int result = system(cmd);
    if (result != 0)
    {
        printf("Command '%s' failed with exit code %d.\n", cmd, result);
    }
    return result;
}

void checkF()
{
    int result;

    printf("Checking for Python...\n");
    result = system("python -V");
    if (result == 0)
    {
        printf("Python is installed.\n");

        printf("Checking for pip...\n");
        result = system("pip -V");
        if (result == 0)
        {
            printf("pip is installed.\n");

            printf("Checking for yt-dlp...\n");
            result = system("yt-dlp --version");
            if (result != 0)
            {
                printf("yt-dlp is not installed. Installing yt-dlp...\n");
                result = system("pip install yt-dlp");
                if (result == 0)
                {
                    printf("yt-dlp installed successfully.\n");
                    result = system("yt-dlp --version");
                    if (result == 0)
                    {
                        printf("yt-dlp is now available.\n");
                        clearScreen();
                    }
                    else
                    {
                        printErrorAndExit("Error: yt-dlp not available after installation.");
                    }
                }
                else
                {
                    printErrorAndExit("Error: yt-dlp installation failed.");
                }
            }
            else
            {
                printf("yt-dlp is already installed.\n");
                clearScreen();
            }
        }
        else
        {
            printErrorAndExit("Error: pip not available.");
        }
    }
    else
    {
        printErrorAndExit("Error: Python not available. Please install Python: https://www.python.org/");
    }
}

void clearScreen()
{
    system("cls");
}

void printErrorAndExit(const char *message)
{
    printf("%s\n", message);
    checker++;
    clearScreen();
}

void title()
{
    clearScreen();
    printf("*-----------------------------------*\n\tYoutube MP%d Downloader\n*-----------------------------------*\nCopy url and right click to paste\n", n);
    printf("LINK(url) >> ");
    scanf("%s", url);
}

void mp3DL_check()
{
    if (yt_dl_mp3() == 0)
    {
        printf("*-----------------------------------*\n\tDownload successful.\n*-----------------------------------*\n\n");
    }
    else
    {
        printf("*-------------------------------*\n\tDownload failed.\n*-------------------------------*\n\n");
    }
}

void mp4DL_check()
{
    if (yt_dl_mp4() == 0)
    {
        printf("*-----------------------------------*\n\tDownload successful.\n*-----------------------------------*\n\n");
    }
    else
    {
        printf("*-------------------------------*\n\tDownload failed.\n*-------------------------------*\n\n");
    }
}

void setDefaultPath()
{
    printf("Current Default Download Path: %s\n", default_path);
    printf("Set New Default Download Path >> ");
    scanf("%s", default_path);
    savePathToFile();
}

void choosePath()
{
    int choice;
    printf("Choose Download Path:\n");
    printf("[1] Use Default Path (%s)\n", default_path);
    printf("[2] Specify New Path\n");
    printf("~$ ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2))
    {
        printf("Invalid choice. Please enter 1 or 2.\n~$ ");
        while (getchar() != '\n');  // Clear the input buffer
    }

    if (choice == 1)
    {
        strcpy(path, default_path);
    }
    else if (choice == 2)
    {
        printf("Specify New Path >> ");
        scanf("%s", path);
    }
}

void loadPathFromFile()
{
    FILE *file = fopen(PATH_FILE, "r");
    if (file)
    {
        if (fgets(default_path, sizeof(default_path), file))
        {
            // Remove trailing newline if present
            size_t len = strlen(default_path);
            if (len > 0 && default_path[len - 1] == '\n')
            {
                default_path[len - 1] = '\0';
            }
        }
        fclose(file);
    }
    else
    {
        // Set default path if file does not exist
        strcpy(default_path, "\\playlist");
    }
}

void savePathToFile()
{
    FILE *file = fopen(PATH_FILE, "w");
    if (file)
    {
        fprintf(file, "%s\n", default_path);
        fclose(file);
    }
    else
    {
        printf("Error: Could not save path to file.\n");
    }
}

int main()
{
    int choose;
    loadPathFromFile();
    checkF();

    do
    {
        if (checker == 0)
        {
            printf("All checks passed. Ready to proceed.\n");
            clearScreen();
            printf("*-----------------------------------*\n\tYoutube Downloader\n*-----------------------------------*\n");
            printf("[1] Audio Download (Mp3)\n[2] Video Download (Mp4)\n[3] Set Default Download Path\n[0] Exit\n~$ ");

            while (scanf("%d", &choose) != 1 || (choose < 0 || choose > 3))
            {
                printf("Invalid option. Please enter a number between 0 and 3.\n~$ ");
                while (getchar() != '\n');  // Clear the input buffer
            }

            switch (choose)
            {
                case 0:
                    clearScreen();
                    printf("Exiting...\n");
                    break;
                case 1:
                    n = 3;
                    title();
                    choosePath();
                    mp3DL_check();
                    break;
                case 2:
                    n = 4;
                    title();
                    choosePath();
                    mp4DL_check();
                    break;
                case 3:
                    setDefaultPath();
                    break;
                default:
                    printf("Invalid option. Please try again.\n");
                    break;
            }
        }
        else
        {
            printf("Some checks failed. Please fix the issues and try again.\n");
            break;
        }

    } while (choose != 0);

    return 0;
}
