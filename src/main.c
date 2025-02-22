#include <termios.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <threads.h>

char * chose_the_passage();
int start_typing(char *passage);
char getch();

int main(int argc, char ** argv) {
    start_typing(chose_the_passage());

    return EXIT_SUCCESS;
}

char *chose_the_passage() {
    char *passage1 = "If you're visiting this page, you're likely here because you're searching for a random sentence. Sometimes a random word just isn't enough, and that is where the random sentence generator comes into play. By inputting the desired number, you can make a list of as many random sentences as you want or need. Producing random sentences can be helpful in a number of different ways. For writers, a random sentence can help them get their creative juices flowing. Since the topic of the sentence is completely unknown, it forces the writer to be creative when the sentence appears. There are a number of different ways a writer can use the random sentence for creativity. The most common way to use the sentence is to begin a story. Another option is to include it somewhere in the story. A much more difficult challenge is to use it to end a story. In any of these cases, it forces the writer to think creatively since they have no idea what sentence will appear from the tool.";
    char *passage2 = "For those writers who have writers' block, this can be an excellent way to take a step to crumbling those walls. By taking the writer away from the subject matter that is causing the block, a random sentence may allow them to see the project they're working on in a different light and perspective. Sometimes all it takes is to get that first sentence down to help break the block. It can also be successfully used as a daily exercise to get writers to begin writing. Being shown a random sentence and using it to complete a paragraph each day can be an excellent way to begin any writing session. Random sentences can also spur creativity in other types of projects being done. If you are trying to come up with a new concept, a new idea or a new product, a random sentence may help you find unique qualities you may not have considered. Trying to incorporate the sentence into your project can help you look at it in different and unexpected ways than you would normally on your own.";
    char *passage_test = "mohammadreza";

    char list_of_passage[2] = {*passage1, *passage2};

    printf("Chose the passage you want to type\n");
    printf("1 - Passage 1\n");
    printf("2 - Passage 2\n");
    printf("3 - Passage test\n");
    printf("> ");

    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            return passage1;
        case 2:
            return passage2;
            break;
        case 3:
            return passage_test;
            break;
        default:
            printf("Invalid choice\n");
            return NULL;
    }
}

int start_typing(char *passage) {
    if (!passage) {
        printf("Exitting...\n");
        return EXIT_FAILURE;
    }

    printf("\nType the following passage:\n\n");
    printf("%s\n\n", passage);
    printf("Press any key to start typing...\n");
    getch();  // Wait for user to press a key

    int mistakes = 0;
    int letter = 0;
    int len = strlen(passage);
    time_t start_time = time(NULL);

    while (letter < len) {
        printf("\033[2J\033[H");  // Clear screen
        printf("Progress: %d/%d characters\n", letter, len);
        printf("Passage:\n%s\n\n", passage);

        // Show progress
        printf("Your typing:\n");
        for (int i = 0; i < letter; i++) {
            printf("%c", passage[i]);
        }

        char input = getch();

        // Handle backspace (ASCII 127 or 8)
        if ((input == 127 || input == 8) && letter > 0) {
            letter--;
            continue;
        }

        // Check if input matches the current letter
        if (input == passage[letter]) {
            letter++;
        } else {
            mistakes++;
        }
    }

    time_t end_time = time(NULL);
    double time_taken = difftime(end_time, start_time);
    double words = len / 5.0;  // Approximate number of words (5 characters per word)
    double wpm = (words * 60) / time_taken;  // Words per minute

    printf("\n\nTyping completed!\n");
    printf("Time taken: %.0f seconds\n", time_taken);
    printf("Mistakes made: %d\n", mistakes);
    printf("Words per minute: %.2f\n", wpm);
    printf("Accuracy: %.2f%%\n", 100.0 * (len - mistakes) / len);
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
