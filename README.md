# OS-ShellScripting

Operating systems lab work: **Bash shell scripts** for system administration tasks and a **C process/string utilities** suite driven by `fork`/`execlp`. Includes Assignment #01 solutions and Assignment #02 materials.

**Author:** Mohammad Rohaan · **Roll:** 22I-2327

---

## Overview

Assignment #01 covers practical OS scripting and process management:

| Question | Focus |
|----------|--------|
| **Q01** | Disk cleanup - top largest files, duplicate detection, interactive deletion |
| **Q02** | Logging / text-file oriented shell automation |
| **Q03** | Additional Bash administration workflow |
| **Q04** | C string toolkit with parent menu + child `execlp` workers |
| **Q05** | Further Bash assignment script |

Assignment #02 provides the PDF brief and a submission zip (`i222327_A2_A.zip`).

---

## Features

- Heavily commented Bash solutions (teaching-oriented).
- Q04 C utilities: reverse, length, ASCII+2, sort, capitalize - each as its own executable.
- Parent `main_file.c` writes input to `input.txt`, forks, and `execlp`s the selected tool.
- PDF assignment briefs for reference.

---

## Repository structure

```text
OS-ShellScripting/
├── Assignment #01/
│   ├── Assignment 1.pdf
│   └── A_22I-2327_Mohammad_Rohaan/
│       ├── Q_01_Sol/Q_01.sh
│       ├── Q_02_Sol/Q_02.sh
│       ├── Q_03_Sol/Q_03.sh
│       ├── Q_04_Sol/
│       │   ├── main_file.c
│       │   ├── reverse_str.c | length_str.c | ascii_add_2_str.c
│       │   ├── sort_str.c | capitalize_str.c
│       │   └── (prebuilt binaries may be present)
│       └── Q_05_Sol/Q_05.sh
└── Assignment #02/
    ├── Assignment2.pdf
    └── i222327_A2_A.zip
```

---

## Build / run

### Bash scripts (Linux/WSL/macOS or Git Bash)

```bash
cd "Assignment #01/A_22I-2327_Mohammad_Rohaan/Q_01_Sol"
chmod +x Q_01.sh
./Q_01.sh
```

Repeat for `Q_02.sh`, `Q_03.sh`, and `Q_05.sh` in their respective folders.

> **Caution:** Q01 can delete files interactively after listing the largest items under `$HOME`. Run in a safe environment and answer prompts carefully.

### C string suite (Q04)

```bash
cd "Assignment #01/A_22I-2327_Mohammad_Rohaan/Q_04_Sol"

gcc reverse_str.c -o reverse_str
gcc length_str.c -o length_str
gcc ascii_add_2_str.c -o ascii_add_2_str
gcc sort_str.c -o sort_str
gcc capitalize_str.c -o capitalize_str
gcc main_file.c -o main_file

./main_file
# enter a string, then choose menu options 1-5 (6 to exit)
```

Workers read from `input.txt` created by the parent.

---

## Usage

1. Prefer WSL/Ubuntu for path and `find`/`du` behavior matching the scripts.
2. Q02 writes log-style output to `text_file.txt` in the working directory.
3. Keep Q04 binaries next to `main_file` so `execlp("./reverse_str", ...)` resolves.

---

## Extending

- Factor Q01 duplicate detection into a reusable library script.
- Replace `scanf("%s")` in `main_file.c` with line-oriented input for spaces.
- Add unit tests that feed fixed `input.txt` fixtures to each worker.
- Unzip Assignment #02 and document its scripts alongside Assignment #01.

---

## License

Academic coursework - follow institutional policies for redistribution.
