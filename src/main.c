#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <threads.h>
#include <unistd.h>

char *chose_passage();
int start_typing(char *passage);
char getch();
int get_real_word_count(const char *text);

int main() {
  start_typing(chose_passage());

  return EXIT_SUCCESS;
}

char *chose_passage() {
  char *passage1 =
      "If you're visiting this page, you're likely here because you're "
      "searching for a random sentence. Sometimes a random word just isn't "
      "enough, and that is where the random sentence generator comes into "
      "play. By inputting the desired number, you can make a list of as many "
      "random sentences as you want or need. Producing random sentences can be "
      "helpful in a number of different ways. For writers, a random sentence "
      "can help them get their creative juices flowing. Since the topic of the "
      "sentence is completely unknown, it forces the writer to be creative "
      "when the sentence appears. There are a number of different ways a "
      "writer can use the random sentence for creativity. The most common way "
      "to use the sentence is to begin a story. Another option is to include "
      "it somewhere in the story. A much more difficult challenge is to use it "
      "to end a story. In any of these cases, it forces the writer to think "
      "creatively since they have no idea what sentence will appear from the "
      "tool.";
  char *passage2 =
      "For those writers who have writers' block, this can be an excellent way "
      "to take a step to crumbling those walls. By taking the writer away from "
      "the subject matter that is causing the block, a random sentence may "
      "allow them to see the project they're working on in a different light "
      "and perspective. Sometimes all it takes is to get that first sentence "
      "down to help break the block. It can also be successfully used as a "
      "daily exercise to get writers to begin writing. Being shown a random "
      "sentence and using it to complete a paragraph each day can be an "
      "excellent way to begin any writing session. Random sentences can also "
      "spur creativity in other types of projects being done. If you are "
      "trying to come up with a new concept, a new idea or a new product, a "
      "random sentence may help you find unique qualities you may not have "
      "considered. Trying to incorporate the sentence into your project can "
      "help you look at it in different and unexpected ways than you would "
      "normally on your own.";
  char *passage_test = "The quick brown fox jumps over the lazy dog";

  printf("Chose the passage you want to type\n");
  printf("1 - Passage 1\n");
  printf("2 - Passage 2\n");
  printf("3 - Passage test\n");
  printf("> ");

  int choice;

  if (scanf("%d", &choice) != 1) {
      printf("Invalid input\n");
      return NULL;
  }

  switch (choice) {
  case 1:
    return passage1;
  case 2:
    return passage2;
  case 3:
    return passage_test;
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
  getch(); // Wait for user to press a key
  printf("\033[2J\033[H");

  int mistakes = 0;
  int letter = 0;
  int len = strlen(passage);

  char *typed_text = malloc(len + 1);
  memset(typed_text, 0, len + 1);

  struct timespec start, end;
  timespec_get(&start, TIME_UTC);

  while (letter < len) {
    // Move cursor to top without full clear (less flicker)
    printf("\033[H");
    printf("Progress: %d/%d characters\n", letter, len);

    // Show progress with typed text
    printf("Your typing:\n");
    for (int i = 0; i < len; i++) {
      if (i < letter) {
        // Show typed characters with color
        if (typed_text[i] == passage[i]) {
          printf("\033[32m%c\033[0m", typed_text[i]); // Green for correct
        } else {
          printf("\033[31m%c\033[0m", typed_text[i]); // Red for incorrect
        }
      } else if (i == letter) {
        // Highlight current position with underscore or bold
        printf("\033[1m\033[4m%c\033[0m", passage[i]); // Bold and underlined
      } else {
        printf("%c", passage[i]); // Upcoming characters
      }
    }
    printf("\n"); // Newline for clarity

    // Show current input prompt below
    fflush(stdout); // Ensure output is displayed immediately

    char input = getch();

    // Handle backspace
    if ((input == 127 || input == 8) && letter > 0) {
      letter--;
      typed_text[letter] = 0;
      continue;
    }

    // Store and check input
    typed_text[letter] = input;
    if (input == passage[letter]) {
      letter++;
    } else {
      mistakes++;
      letter++;
    }
  }

  timespec_get(&end, TIME_UTC);
  double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  double words = get_real_word_count(passage);
  double wpm = (words * 60) / time_taken;

  printf("\n\nTyping completed!\n");
  printf("Time taken: %.0f seconds\n", time_taken);
  printf("Mistakes made: %d\n", mistakes);
  printf("Words per minute: %.2f\n", wpm);
  printf("Accuracy: %.2f%%\n", 100.0 * (len - mistakes) / len);

  free(typed_text);
  return EXIT_SUCCESS;
}

// Reads a single character from standard input without echo or buffering.
// Note: This function is Unix-specific and relies on termios functionality.
char getch() {
  struct termios original;
  if (tcgetattr(0, &original) < 0) {
    perror("tcgetattr()");
    return EXIT_FAILURE; // Exit if we can't retrieve terminal settings
  }

  struct termios modified = original; // Copy for temporary modifications
  modified.c_lflag &= ~ICANON; // Disable canonical mode (no Enter required)
  modified.c_lflag &= ~ECHO;   // Disable echoing of input characters
  modified.c_cc[VMIN] = 1;     // Read at least 1 character
  modified.c_cc[VTIME] = 0;    // No timeout, wait indefinitely
  if (tcsetattr(0, TCSANOW, &modified) < 0) {
    perror("tcsetattr() failed to set non-canonical mode");
    return EXIT_FAILURE; // Exit if we can't apply the new settings
  }

  char buf; // Buffer for the input character
  if (read(0, &buf, 1) < 0) {
    perror("read()");
    tcsetattr(0, TCSADRAIN, &original); // Attempt to restore settings
    return EXIT_FAILURE;                 // Exit if reading fails
  }

  if (tcsetattr(0, TCSADRAIN, &original) < 0) {
    perror("tcsetattr() failed to restore original settings");
    // Continue, as input was successfully read
  }

  return buf;
}

int get_real_word_count(const char *text) {
  int word_count = 0;
  int in_word = 0; // Flag to track if we're currently in a word

  // Iterate through the text
  for (int i = 0; text[i] != '\0'; i++) {
    if (isspace(text[i]) || ispunct(text[i])) {
      in_word = 0; // We've hit a space or punctuation
    } else if (!in_word) {
      word_count++; // We've found the start of a new word;
      in_word = 1;
    }
  }

  return word_count;
}
