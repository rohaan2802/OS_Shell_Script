# Operating Systems – Shell Scripting (CS-2006)

**Operating Systems lab work from Spring 2025 at FAST-NUCES – Bash scripts and C/C++ programs on processes, pipes, threads and CPU scheduling.**

[![Language](https://img.shields.io/badge/Language-Bash%20%2B%20C%2FC%2B%2B-0f5c55?style=for-the-badge)](https://github.com/rohaan2802/OS-ShellScripting)
[![Course](https://img.shields.io/badge/Course-CS--2006-9a4a0c?style=for-the-badge)](https://github.com/rohaan2802/OS-ShellScripting)
[![University](https://img.shields.io/badge/FAST--NUCES-Spring%202025-0f5c55?style=for-the-badge)](https://github.com/rohaan2802/OS-ShellScripting)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

---

## Live Links

| What | Link |
|------|------|
| Repository | [https://github.com/rohaan2802/OS-ShellScripting](https://github.com/rohaan2802/OS-ShellScripting) |
| Website (GitHub Pages) | [https://rohaan2802.github.io/OS-ShellScripting/](https://rohaan2802.github.io/OS-ShellScripting/) |
| Download (ZIP) | [https://github.com/rohaan2802/OS-ShellScripting/archive/refs/heads/main.zip](https://github.com/rohaan2802/OS-ShellScripting/archive/refs/heads/main.zip) |

---

## Table of Contents

1. [Live Links](#live-links)
2. [Overview](#overview)
3. [What's Inside](#whats-inside)
4. [Assignment 1](#assignment-1)
5. [Assignment 2](#assignment-2)
6. [Folder Structure](#folder-structure)
7. [How to Run](#how-to-run)
8. [Screenshots](#screenshots)
9. [License](#license)
10. [Author](#author)

---

## Overview

This repository is my CS-2006 Operating Systems lab work from Spring 2025. There are two assignments.

Assignment 1 is mostly Bash: a disk cleanup helper, a file-utility menu, an application-usage tracker, a personal expense ledger, and a C string toolkit that runs each task in a grandchild process via `fork` and `execlp`.

Assignment 2 is C and C++: a multiplayer zombie survival game using processes and pipes, professor–student office hours with threads, a cricket lineup tool built around SJF, SRTF and Round Robin, four processes passing a message in a circle over unnamed pipes, and a three-level multilevel feedback queue scheduler.

The assignment briefs (PDF) are in each assignment folder, next to the code.

> Course: **CS-2006 – Operating Systems** · **Spring 2025** · **FAST-NUCES** · Roll: **22I-2327**

---

## What's Inside

| Section | Folder | What's in it |
|---------|--------|----------------|
| Assignment 1 brief | [assignment-01/assignment-01.pdf](assignment-01/assignment-01.pdf) | The original Assignment 1 statement |
| Q01 Disk cleanup | [assignment-01/q01-disk-cleanup](assignment-01/q01-disk-cleanup) | `q01-disk-cleanup.sh` |
| Q02 File utilities | [assignment-01/q02-file-utilities](assignment-01/q02-file-utilities) | `q02-file-utilities.sh` |
| Q03 App usage | [assignment-01/q03-app-usage](assignment-01/q03-app-usage) | `q03-app-usage.sh` |
| Q04 String toolkit | [assignment-01/q04-string-toolkit](assignment-01/q04-string-toolkit) | `main_file.c` and five worker programs, plus a Makefile |
| Q05 Expense tracker | [assignment-01/q05-expense-tracker](assignment-01/q05-expense-tracker) | `q05-expense-tracker.sh` |
| Assignment 2 brief | [assignment-02/assignment-02.pdf](assignment-02/assignment-02.pdf) | The original Assignment 2 statement |
| Q01 Zombie survival | [assignment-02/q01-zombie-survival](assignment-02/q01-zombie-survival) | `zombie-survival.cpp` |
| Q02 Office hours | [assignment-02/q02-office-hours](assignment-02/q02-office-hours) | `office-hours.cpp` |
| Q03 Cricket scheduling | [assignment-02/q03-cricket-scheduling](assignment-02/q03-cricket-scheduling) | `cricket-scheduling.cpp` |
| Q04 Circular pipes | [assignment-02/q04-circular-pipes](assignment-02/q04-circular-pipes) | `circular-pipes.c` |
| Q05 MFQ scheduler | [assignment-02/q05-mfq-scheduler](assignment-02/q05-mfq-scheduler) | `mfq-scheduler.cpp` |

---

## Assignment 1

### Q01 – Disk cleanup

A Bash script that walks `$HOME` and helps free space.

- Lists the ten largest files (`find`, `du`, `sort`) and asks before deleting them.
- Has a duplicate-file helper (MD5 checksums, with options to delete, hard-link or move the copies). In the default run that step is left commented, because a full scan of `$HOME` can take a while.
- Clears cache under `~/.cache` and common Firefox/Chrome cache folders. If you run it as root it also cleans `/var/log` and runs `apt autoremove`.
- Suggests files not accessed in the last 10 days (`-atime +10`) and asks before deleting them.

**This script can delete files.** Do not run it on a machine you care about until you have read it. The largest-file and old-file steps ask first (`Y`/`N`, and `rm -i`). The cache clean does not ask.

```bash
cd assignment-01/q01-disk-cleanup
chmod +x q01-disk-cleanup.sh
./q01-disk-cleanup.sh
```

### Q02 – File utilities

A menu-driven script. Every helper message is also appended to `text_file.txt`.

1. Show a file's permissions, then invert them (for example `-rw-r--r--` becomes `---x-wx-wx`) and show the result.
2. Search a file for a string. A dot in the pattern is treated as "any character", so `c.t` matches `cat` and `cot`.
3. Ask for `N`, then write `dummy_file.txt` with the first `N` lines of even-positioned files in the current directory and the last `N` lines of odd-positioned files (`.sh` files are skipped).
4. If a file's modification time is more than 24 hours old, `touch` it so the time becomes now.

```bash
cd assignment-01/q02-file-utilities
chmod +x q02-file-utilities.sh
./q02-file-utilities.sh
```

Sample (option 2, pattern `c.t`):

```
OUTPUT RESULT --->  The cat sat on the mat
OUTPUT RESULT --->  A cot is small
```

### Q03 – Application usage

Tracks the focused X11 window every 10 seconds, appends a line to `~/app_usage.log`, and prints a short usage report (minutes and seconds). If a listed "non-productive" app (chrome, gnome-terminal, nautilus, gedit, evince) stays in focus for a minute, the script sends a desktop notification and tries to close it with `pkill`.

Needs `xdotool`. `notify-send` is used for the warning.

```bash
sudo apt install xdotool libnotify-bin
cd assignment-01/q03-app-usage
chmod +x q03-app-usage.sh
./q03-app-usage.sh
```

Stop it with `Ctrl+C`. The limit is one minute, which is short on purpose so the block can be tested without waiting around.

### Q04 – String toolkit (C)

You type one word. The parent writes it to `input.txt`, then for each task it double-forks and the grandchild runs a worker with `execlp`. Each worker reads `input.txt` and appends a line to `output.txt` (task, PID, result). Option 6 prints that file and exits.

| Option | Worker | What it does |
|--------|--------|----------------|
| 1 | `reverse_str` | Reverse the string |
| 2 | `length_str` | Length |
| 3 | `ascii_add_2_str` | Add 2 to each character's ASCII value |
| 4 | `sort_str` | Sort the characters |
| 5 | `capitalize_str` | Uppercase |

```bash
cd assignment-01/q04-string-toolkit
make
./main_file
```

Sample, input `OperatingSystems`:

```
TASK: REVERSE | PID: 51537 | RESULT: smetsySgnitarepO
TASK: FIND LENGTH | PID: 51539 | RESULT: 16
TASK: ADD 2 ASCII | PID: 51545 | RESULT: QrgtcvkpiU{uvgou
TASK: SORTING THE STRING | PID: 51553 | RESULT: OSaeegimnprsstty
TASK: CAPITALIZE THE STRING | PID: 51555 | RESULT: OPERATINGSYSTEMS
```

The input is a single token (`scanf("%s")`), so no spaces. The workers have to be in the same directory as `main_file` — `make` builds all of them there.

### Q05 – Expense tracker

A small ledger stored in `expenses.csv` (`date,category,amount`). The menu lets you:

1. Add a row (category can be FOOD, BILLS, SHOPPING, INCOME, OTHER, or whatever you type).
2. Print a per-category total, overall expenses, monthly savings (`income − expenses`), and a warning if expenses go over the budget (default `100000`, changeable from the menu).
3. Print the CSV.
4. Plot spending over time with gnuplot (income rows are left out of the graph). Optional: `sudo apt install gnuplot`.
5. Set a new budget.
6. Exit.

```bash
cd assignment-01/q05-expense-tracker
chmod +x q05-expense-tracker.sh
./q05-expense-tracker.sh
```

Sample, after a few entries (FOOD 250 + 400, BILLS 1200, INCOME 5000) and a budget of 1000:

```
                         FOOD = 650
                        BILLS = 1200
                       INCOME = 5000

                TOTAL EXPENSES  => 1850
                MONTHLY SAVINGS => 3150

        ALERT: BUDGET LIMIT EXCEEDED ..... !
```

---

## Assignment 2

### Q01 – Zombie survival

A terminal game. The parent is the game manager. Each player and each zombie is a child process and talks to the parent through a pipe. Players randomly move, attack, or find a weapon; zombies bite a random player. After five turns the higher score wins.

The number of zombies comes from the last four digits of my roll number: `(2327 * 3) % 20 + 1`, which is 2.

```bash
cd assignment-02/q01-zombie-survival
make
./zombie-survival
```

### Q02 – Office hours

A professor thread and a student thread per person. Students take turns asking a question (`question_start` / `question_done`); the professor waits, answers, and only then is the next student allowed through (`answer_start` / `answer_done`). Shared flags are `std::atomic`, with no mutexes or semaphores, as the brief asked.

```bash
cd assignment-02/q02-office-hours
make
./office-hours
```

It will ask how many students. A run with one student looks like this:

```
PROFESSOR IS SLEEPING.....
STUDENT 1 ASKS A QUESTION......
STUDENT 1 HAS ASKED THE QUESTION.....
PROFESSOR STARTS ANSWERING STUDENT 1'S QUESTION.....
PROFESSOR HAS ANSWERED STUDENT 1'S QUESTION.....
STUDENT 1 RECEIVED THE ANSWER AND LEAVES......
```

### Q03 – Cricket scheduling

You enter batsmen (name, matches, strike rate, average) and bowlers (name, matches, economy, wickets, average). For batting, SJF orders by strike rate and SRTF by average, then a predicted team score is printed. For bowling, SJF uses economy, SRTF uses wickets, and Round Robin rotates bowlers over a number of overs you choose.

```bash
cd assignment-02/q03-cricket-scheduling
make
./cricket-scheduling
```

Sample batting order (SJF, higher strike rate first):

```
Fakhar - STRIKE RATE: 132 - AVG SCORE: 35.5
Babar - STRIKE RATE: 125.5 - AVG SCORE: 45.2
Rizwan - STRIKE RATE: 118 - AVG SCORE: 42
PREDICTED TEAM SCORE: 122.70 RUNS
```

### Q04 – Circular pipes

Four processes, A → B → C → D → A, connected by unnamed pipes. Process A starts with a message and logs it to `log_A.txt`. Each of B, C and D reads the message, lets you append a note, writes its own log file, and passes the result on. A prints the fully modified message when it comes back.

```bash
cd assignment-02/q04-circular-pipes
make
./circular-pipes
```

Sample:

```
PROCESS A, ENTER INITIAL MESSAGE:                  Hello OS
PROCESS B, ENTER THE MODIFICATION:                 - Processed by B
PROCESS C, ENTER MODIFICATION:                     - Processed by C
PROCESS D, ENTER MODIFICATION:                     - Processed by D
FINAL MODIFIED MESSAGE RECEIVED BY PROCESS A: Hello OS - Processed by B - Processed by C - Processed by D
```

### Q05 – Multilevel feedback queue

A 3-level MFQ scheduler.

- Queue 1: Round Robin, quantum 4. If a process is not done, it drops to queue 2.
- Queue 2: Round Robin, quantum 8. If still not done, it drops to queue 3.
- Queue 3: FCFS, runs to completion.

You enter `N`, then each process's PID, arrival time and burst time. The program prints turnaround time and waiting time.

```bash
cd assignment-02/q05-mfq-scheduler
make
./mfq-scheduler
```

Sample (the four-process example from the brief: bursts 20, 10, 15, 12):

```
PROCESS 1 -> TURNAROUND TIME: 54, WAITING TIME: 34
PROCESS 2 -> TURNAROUND TIME: 28, WAITING TIME: 18
PROCESS 3 -> TURNAROUND TIME: 53, WAITING TIME: 38
PROCESS 4 -> TURNAROUND TIME: 40, WAITING TIME: 28
```

---

## Folder Structure

```
OS-ShellScripting/
├── assignment-01/
│   ├── assignment-01.pdf
│   ├── q01-disk-cleanup/
│   │   └── q01-disk-cleanup.sh
│   ├── q02-file-utilities/
│   │   └── q02-file-utilities.sh
│   ├── q03-app-usage/
│   │   └── q03-app-usage.sh
│   ├── q04-string-toolkit/
│   │   ├── Makefile
│   │   ├── main_file.c
│   │   ├── reverse_str.c
│   │   ├── length_str.c
│   │   ├── ascii_add_2_str.c
│   │   ├── sort_str.c
│   │   └── capitalize_str.c
│   └── q05-expense-tracker/
│       └── q05-expense-tracker.sh
├── assignment-02/
│   ├── assignment-02.pdf
│   ├── q01-zombie-survival/
│   │   ├── Makefile
│   │   └── zombie-survival.cpp
│   ├── q02-office-hours/
│   │   ├── Makefile
│   │   └── office-hours.cpp
│   ├── q03-cricket-scheduling/
│   │   ├── Makefile
│   │   └── cricket-scheduling.cpp
│   ├── q04-circular-pipes/
│   │   ├── Makefile
│   │   └── circular-pipes.c
│   └── q05-mfq-scheduler/
│       ├── Makefile
│       └── mfq-scheduler.cpp
├── screenshots/
├── assets/
│   ├── css/style.css
│   ├── js/main.js
│   ├── favicon.svg
│   └── og-image.png
├── index.html
├── LICENSE
└── README.md
```

---

## How to Run

You need a Linux environment (or WSL) with Bash, `gcc` and `g++`. The C programs use POSIX calls (`fork`, `pipe`, `execlp`, pthreads), so they are meant to run on Linux, not plain Windows.

```bash
git clone https://github.com/rohaan2802/OS-ShellScripting.git
cd OS-ShellScripting
```

Each C/C++ folder has a Makefile:

```bash
cd assignment-01/q04-string-toolkit
make          # builds main_file and the five workers
./main_file
make clean    # removes the binaries and input.txt / output.txt
```

```bash
cd assignment-02/q01-zombie-survival
make && ./zombie-survival
```

The office-hours program is linked with `-pthread` (already in its Makefile).

Runtime files such as `expenses.csv`, `output.txt` and `log_A.txt` are produced when you run the programs. They are listed in `.gitignore` and are safe to delete.

---

## Screenshots

### 1. Repository Folder Structure

![Repository folder structure](screenshots/01-repository-structure.png)

### 2. Assignment 1 Brief

![Assignment 1 brief](screenshots/02-assignment-01-brief.png)

### 3. Disk Cleanup Script Output

![Disk cleanup script output](screenshots/03-q01-largest-files.png)

### 4. File Utilities Main Menu

![File utilities main menu](screenshots/05-q02-main-menu.png)

### 5. Invert File Permissions

![Invert file permissions](screenshots/06-q02-invert-permissions.png)

### 6. Search with a Wildcard (c.t)

![Search with a wildcard](screenshots/07-q02-search-string.png)

### 7. Copy First and Last N Lines

![Copy first and last N lines](screenshots/08-q02-copy-content.png)

### 8. Update a Stale Modified Date

![Update a stale modified date](screenshots/09-q02-modified-date.png)

### 9. Application Usage Report

![Application usage report](screenshots/10-q03-usage-report.png)

### 10. String Toolkit Menu

![String toolkit menu](screenshots/11-q04-string-menu.png)

### 11. String Toolkit Results

![String toolkit results](screenshots/12-q04-string-results.png)

### 12. Expense Tracker – Add and Summary

![Expense tracker add and summary](screenshots/13-q05-add-and-summary.png)

### 13. Budget Alert

![Budget alert](screenshots/14-q05-budget-alert.png)

### 14. Spending Trend Graph

![Spending trend graph](screenshots/15-q05-spending-trend.png)

### 15. Assignment 2 Brief

![Assignment 2 brief](screenshots/16-assignment-02-brief.png)

### 16. Zombie Survival Game

![Zombie survival game](screenshots/17-q01-zombie-survival.png)

### 17. Office Hours (Professor and Student)

![Office hours](screenshots/18-q02-office-hours.png)

### 18. Cricket Batting Order (SJF)

![Cricket batting order](screenshots/19-q03-cricket-batting.png)

### 19. Cricket Bowling Rotation (Round Robin)

![Cricket bowling rotation](screenshots/19b-q03-cricket-bowling.png)

### 20. Circular Pipe Message Passing

![Circular pipe message passing](screenshots/20-q04-circular-pipes.png)

### 21. Multilevel Feedback Queue Stats

![Multilevel feedback queue stats](screenshots/21-q05-mfq-scheduler.png)

### 22. Website Home (Dark Theme)

![Website home, dark theme](screenshots/22-website-hero-dark.png)

### 23. Website Home (Light Theme)

![Website home, light theme](screenshots/23-website-hero-light.png)

### 24. Website on a Phone

![Website on a phone](screenshots/24-website-mobile.png)

### 25. Assignment 1 Section on the Website

![Assignment 1 section on the website](screenshots/25-website-assignment-1.png)

---

## License

The code I wrote is under the [MIT License](LICENSE). The assignment PDFs belong to their instructors at FAST-NUCES and are here only for reference with this lab work. If you are an owner and want something removed, please open an [issue](https://github.com/rohaan2802/OS-ShellScripting/issues).

---

## Author

**Mohammad Rohaan**
BS Computer Science – FAST-NUCES
Roll no. 22I-2327

- GitHub: [@rohaan2802](https://github.com/rohaan2802)
- Email: [m.rohaanarshad@gmail.com](mailto:m.rohaanarshad@gmail.com)
