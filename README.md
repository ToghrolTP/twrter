# Twrter

A command-line typing test program to measure and improve your typing speed and accuracy.

## Description

**Twrter** is a lightweight, text-based application written in C that allows users to practice typing by selecting from predefined passages. The program tracks the time taken to type the chosen passage, counts mistakes (incorrect keystrokes), and calculates words per minute (WPM) and accuracy percentage. It’s an ideal tool for anyone looking to enhance their typing skills in a minimalistic, terminal-based environment.

## Features

- **Multiple Passages**: Choose from three text passages:
  - Two longer passages for extended practice.
  - A short test passage ("mohammadreza") for quick testing.
- **Real-Time Feedback**: Displays your typing progress character by character, with screen clearing for a clean view.
- **Performance Metrics**:
  - Time taken to complete the passage (in seconds).
  - Number of mistakes made.
  - Words per minute (WPM), calculated using an average of 5 characters per word.
  - Typing accuracy as a percentage.
- **Backspace Support**: Correct mistakes by using the backspace key during typing.
- **Unix-Compatible**: Utilizes `termios` for character-by-character input without requiring Enter.

## Requirements

- A Unix-like operating system (e.g., Linux, macOS).
- A C compiler (e.g., `gcc`).

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ToghrolTP/twrter.git
   cd twrter
   make && ./bin/twrter
    ```