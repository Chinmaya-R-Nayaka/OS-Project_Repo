#  Unix Utilities Implementation (G4_Project2)

## Overview

This project implements a set of commonly used Unix commands from scratch using C, focusing on **system calls and low-level OS concepts**. It also includes a **custom shell** to execute these commands.

---

## Objectives

* Understand **process creation and execution**
* Work with **file I/O system calls**
* Implement real-world **Unix utilities**
* Learn how **shell interacts with OS**

---

## Commands Implemented

### 1 custom_wc

Counts number of **lines, words, and characters** in a file.

#### Logic:

* Reads file character-by-character using `fgetc()`
* Tracks:

  * Lines → counts `\n`
  * Words → uses state flag (`in_word`)
  * Characters → increments per character

#### Usage:

```bash
custom_wc file.txt
```

---

### 2 custom_cat

Displays contents of a file on terminal.

#### Logic:

* Opens file using `open()`
* Reads in chunks using `read()`
* Writes to stdout using `write()`
* Stops at EOF

#### Usage:

```bash
custom_cat file.txt
```

---

### 3 custom_grep

Searches for a pattern in a file and prints matching lines.

#### Features:

* Case-insensitive search (`-i`)
* Line number display (`-n`)

#### Logic:

* Reads file line-by-line using `fgets()`
* Uses `strstr()` for pattern matching
* Applies transformations for case-insensitive matching

#### Usage:

```bash
custom_grep pattern file.txt
custom_grep -i pattern file.txt
custom_grep -n pattern file.txt
```

---

### 4 customfind

Searches for files/directories by name.

#### Features:

* Recursive search (`-r`)
* Case-insensitive search (`-iname`)

#### Logic:

* Traverses directories recursively
* Matches filenames with user input
* Prints full path if match found

#### Usage:

```bash
customfind filename
customfind -r filename
customfind -iname filename
```

---

### 5 custom_mv

Moves or renames files.

#### Logic:

* Uses `rename()` system call
* Checks destination:

  * If directory → moves file inside
  * Else → renames file

#### Usage:

```bash
custom_mv source.txt destination.txt
custom_mv source.txt target_folder
custom_mv source_folder target_folder
```

---

### 6 custom_ls

Lists files in a directory.

#### Features:

* Skips hidden files (`.` files)

#### Logic:

* Uses `opendir()` and `readdir()`
* Iterates through directory entries
* Prints file names

#### Usage:

```bash
custom_ls
custom_ls <directory>
```

---

## Custom Shell (myshell)

A simple shell that:

* Takes user input
* Parses commands
* Executes using `fork()` and `exec()`

#### Features:

* Executes custom commands
* Basic command parsing

#### Usage:

```bash
./myshell
```

---

## Compilation

Compile individual commands:

```bash
gcc commands/custom_wc.c -o custom_wc
gcc commands/custom_cat.c -o custom_cat
gcc commands/custom_grep.c -o custom_grep
gcc commands/custom_ls.c -o custom_ls
gcc commands/custom_mv.c -o custom_mv
gcc commands/customfind.c -o customfind
```

---

## Running Commands

```bash
./custom_cat commands/test.txt
./custom_wc commands/test.txt
./custom_grep -i light commands/test.txt
./custom_mv old_folder new_folder
./custom_ls 
./customfind . -iname file_name
```

---

## Project Structure

```text
G4_Project2_UnixUtilities/
│
├── commands/
│   ├── custom_wc.c
│   ├── custom_cat.c
│   ├── custom_grep.c
│   ├── custom_find.c
│   ├── custom_mv.c
│   ├── custom_ls.c
│
├── shell/
│   └── shell.c
│
├── docs/
├── README.md
├── Makefile
```

---

## Concepts Learned

* Process creation (`fork()`)
* Program execution (`exec()`)
* File handling (`open, read, write`)
* Directory handling (`opendir, readdir`)
* System-level programming

---

## Conclusion

This project demonstrates how fundamental Unix commands work internally and provides hands-on experience with **Operating System concepts**, making it highly useful for **GATE, interviews, and system programming**.

---

## Authors

1. **Buri Gunavardhan** (24JE0601)
2. **C Bharadwaja** (24JE0602)
3. **Chamalla Bhanu Prakash** (24JE0603)
4. **Chandrima Biswas** (24JE0604)
5. **Chapineni Sujitha** (24JE0605)
6. **Chinmaya R Nayaka** (24JE0606)
