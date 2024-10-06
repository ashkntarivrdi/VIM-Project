# VIM Text Editor

This project is a simplified text editor inspired by Vim. The editor is developed in C and offers basic text manipulation functionalities, mimicking some of Vim's command-line interface (CLI) features. This project is part of a course assignment to implement a file editing tool without a graphical user interface (GUI).

## Features

The following commands and functionalities are supported by the editor:

### General Text Editing Features:
- **Create File**: 
  ```bash
  createfile --file <file path>
  ```
  Creates a new file at the specified path. Automatically creates any non-existent directories.

- **Insert Text**:
  ```bash
  insertstr --file <file path> --str <text> --pos <line number>:<start position>
  ```
  Inserts a string at a specified line and position in the file.

- **Display File Content**:
  ```bash
  cat --file <file path>
  ```
  Displays the content of the specified file.

- **Remove Text**:
  ```bash
  removestr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Removes a specified number of characters from a file either forward or backward from the specified position.

- **Copy Text**:
  ```bash
  copystr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Copies a specified number of characters from a file to the clipboard.

- **Cut Text**:
  ```bash
  cutstr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Cuts a specified number of characters from a file and saves them to the clipboard.

- **Paste Text**:
  ```bash
  pastestr --file <file path> --pos <line number>:<start position>
  ```
  Pastes the clipboard content into the file at the specified position.

- **Find Text**:
  ```bash
  find --str <text> --file <file path> [--count] [--at <num>] [--all] [--byword]
  ```
  Finds a specific string in the file and supports options to count occurrences, find the nth occurrence, or locate by word.

- **Replace Text**:
  ```bash
  replace --str1 <old text> --str2 <new text> --file <file path> [--at <num>] [--all]
  ```
  Replaces occurrences of a string in the file with another string.

- **Undo**:
  ```bash
  undo --file <file path>
  ```
  Reverts the last modification made to the file.

- **Grep Functionality**:
  ```bash
  grep --str <pattern> --files [<file1> <file2> ...]
  ```
  Searches for a pattern in multiple files and displays the lines containing the pattern.

- **Text Comparison**:
  ```bash
  compare <file1> <file2>
  ```
  Compares two files line by line, showing differences.

### Advanced Features:
- **Directory Tree Display**:
  ```bash
  tree <depth>
  ```
  Displays the directory tree up to a certain depth.

- **Auto Indentation**:
  ```bash
  auto-indent <file>
  ```
  Automatically indents the content of the file according to coding standards.

### Error Handling:
The program outputs appropriate error messages for invalid commands or arguments, such as `invalid command` for unrecognized input.

## Installation

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```
   
2. Compile the project using the following command:
   ```bash
   gcc -o vim_editor vim_editor.c
   ```
   
3. Run the program:
   ```bash
   ./vim_editor
   ```
   
## Usage

Use the commands as described above to create, modify, and interact with text files. The program runs in a command-line environment and mimics Vim-like behaviors.

## Contributing

Feel free to fork this repository and submit pull requests if you would like to contribute to improving this project.

