# OS-ShellScripting

Operating-systems lab: five Bash/C problems covering disk cleanup, a file-utility menu, X11 application-usage tracking, a **fork/execlp** string toolkit, and a personal-finance CSV tracker. Assignment #01 is fully sourced here; Assignment #02 is a PDF plus zip in the original tree.

**Author:** Mohammad Rohaan · **Roll:** 22I-2327 · **GitHub:** [rohaan2802](https://github.com/rohaan2802)

---

## Table of contents

1. [Problem and context](#problem-and-context)
2. [Features](#features)
3. [Architecture](#architecture)
4. [Algorithms and line-level behavior](#algorithms-and-line-level-behavior)
5. [File-by-file reference](#file-by-file-reference)
6. [I/O formats](#io-formats)
7. [Tech stack](#tech-stack)
8. [Repository structure](#repository-structure)
9. [Build and run](#build-and-run)
10. [Usage](#usage)
11. [Constants](#constants)
12. [Results](#results)
13. [Limitations](#limitations)
14. [How to extend](#how-to-extend)
15. [Author](#author)

---

## Problem and context

Assignment #01 (folder `A_22I-2327_Mohammad_Rohaan` on GitHub) asks for practical OS skills: walking `$HOME` with `find`, hashing for duplicates, `chmod`/`grep`/`stat`, process creation (`fork` + `execlp`), and a small persistent data file.

| Question | Deliverable | What it actually does |
|----------|-------------|------------------------|
| **Q01** | `Q_01.sh` | Disk cleanup: top-10 largest files, optional cache wipe, “old” files by atime |
| **Q02** | `Q_02.sh` | Interactive menu: invert Unix permissions, grep a string, stitch even/odd files, bump mtime if stale |
| **Q03** | `Q_03.sh` | 10-second loop: track focused X11 window, warn/`pkill` “non-productive” apps |
| **Q04** | `main_file.c` + workers | Parent writes `input.txt`, double-forks, grandchild `execlp`s a string tool |
| **Q05** | `Q_05.sh` | Expense ledger `expenses.csv`, awk summaries, optional gnuplot trend |

This working copy includes `Q_01.sh`, `Q_02.sh`, `Q_03.sh`, `Q_05.sh`, `main_file.c`, `reverse_str.c`, `length_str.c`. The original tree also has **Q04** `ascii_add_2_str.c`, `sort_str.c`, `capitalize_str.c` (and their binaries). There is **no** `Q_04.sh`; Q04 is the C suite.

---

## Features

- Teaching-style comments (Q01/Q02 include long hashing / hard-link notes in-file).
- Q01: `du` + `sort -rh` + `head -10`; MD5 duplicate map (function present but **not called** in the default run).
- Q02: logs every helper message to `text_file.txt` via `tee -a`.
- Q03: `xdotool` + `notify-send` + `pkill`; live report every 10 seconds.
- Q04: POSIX `open`/`read`/`write`, `fork`, `wait`, `execlp("./reverse_str", ...)`.
- Q05: CSV `date,category,amount`; budget alert; gnuplot here-doc.

---

## Architecture

```
Q_01.sh ──► identify_largest_files_function
        ──► clean_unnecessary_system_files_function
        ──► suggest_old_files_function
        (detect_duplicates_files_function is defined but commented out)

Q_02.sh ──► while true menu
              1 display_permissions_function  → chmod u=,g=,o=
              2 search_string_function        → grep -E | tee
              3 copy_content_function         → dummy_file.txt
              4 check_modified_date_function  → touch if age > 24h

Q_03.sh ──► while true: track_usage_function; generate_report_function; sleep 10

Q_04    ──► main_file  writes input.txt
              fork → child fork → grandchild execlp worker
              workers read input.txt, append output.txt
              parent wait; at exit cat output.txt

Q_05.sh ──► while true menu → expenses.csv / expenses.dat / gnuplot
```

**Q04 process tree (per menu choice 1–5):**

```
main_file (parent)
   └── child (pid > 0 in first fork? wait — see algorithm)
         └── grandchild: execlp("./reverse_str", ...)
```

`main_file.c` does:

1. `pid = fork()`
2. If `pid == 0` (first child): `child_pid = fork()` again.
3. If `child_pid == 0` (grandchild): `switch(ch1)` → `execlp`.
4. Else (middle process): `wait(NULL); exit(0)`.
5. Parent of first fork: `wait(NULL)`.

So the **worker is the grandchild**; the intermediate process reaps it and exits. The original parent only waits for the first child.

---

## Algorithms and line-level behavior

### Q01 — `Q_01.sh`

**`identify_largest_files_function`**

1. `find "$HOME" -type f -exec du -ah {} + | sort -rh | head -10` stored in `l_files`.
2. Print the list; prompt `DO YOU WANT TO DELETE THESE FILES ? ('Y'/'N')`.
3. If `ch1` is `y` **or** `Y`: `echo "$l_files" | awk '{print $2}' | xargs rm -i`.
   - **Bug:** the test is `[[ "$ch1" == "y" || "$ch1"=="Y" ]]` — the second comparison **lacks spaces** around `==`, so `'Y'` may not match as intended on all Bash versions. Prefer `[[ "$ch1" == "y" || "$ch1" == "Y" ]]`.
4. `awk '{print $2}'` is the path column of `du -ah` output (`SIZE PATH`). Paths with spaces break `xargs` without `-0`.

**`detect_duplicates_files_function` (defined, not invoked)**

- `declare -A file_map`; `duplicates=()`.
- `find "$HOME" -type f -print0` piped into `while IFS= read -r -d '' file`.
- `checksum=$(md5sum "$file" | awk '{print $1}')`.
- If `file_map[$checksum]` already set → append to `duplicates`; else store first path.
- Menu: (1) `rm -i` all duplicates; (2) `ln -f "${file_map[$checksum]}" "$dup"` — **uses `$checksum` from the last loop iteration**, so hard-link target is wrong for mixed hashes; (3) `mkdir -p "$HOME/duplicates" && mv -i …`.

**`clean_unnecessary_system_files_function`**

- Unconditional `rm -rf "$HOME/.cache/"*`, Firefox `cache2`, Chrome `Default/Cache`.
- If `$EUID -eq 0`: `rm -rf /var/log/*.log /var/log/*.gz` and `apt autoremove -y`; else skip logs.

**`suggest_old_files_function`**

- Banner says “NOT ACCESSED IN THE LAST 10 DAYS” and comments mention `-atime +90`, but the command is:

  `old_files=$(find "$HOME" -type f -atime -10)`

  `-atime -10` means **accessed within the last 10 days** (recent files), the opposite of the prompt. Confirm before deleting.
- On `Y`/`y`: `echo "$old_files" | xargs rm -i`.

**Main:** calls largest, **skips** duplicates (`#detect_duplicates_files_function`), then cache clean, then old-files suggest.

### Q02 — `Q_02.sh`

**`helper_function`:** `echo -e "\t\t\t$1" | tee -a "$text_file_name"` with `text_file_name="text_file.txt"`.

**`display_permissions_function`**

1. Read filename; `[ -e "$file_name_var" ]`.
2. `permissions_var=$(ls -l … | awk '{print $1}')` e.g. `-rw-r--r--`.
3. Slice: `user_var=${permissions_var:1:3}`, group `:4:3`, other `:7:3`.
4. Nested `permissions_function` maps **current rwx string to the bits that are currently OFF** (toggle-style):

| Current | chmod symbolic returned |
|---------|-------------------------|
| `rwx` | `=` (empty = strip all? `echo "="`) |
| `rw-` | `x` |
| `r-x` | `w` |
| `r--` | `wx` |
| `-wx` | `r` |
| `-w-` | `rx` |
| `--x` | `rw` |
| `---` | `rwx` |

5. `chmod u="$new_user_var",g="$new_group_var",o="$new_other_var" "$file_name_var"`.
6. Logs `User=…, Group=…, Others=…`.

Example: `-rw-r--r--` → user `rw-`→`x`, group `r--`→`wx`, other `r--`→`wx` → `--x-wx-wx`.

**`search_string_function`:** read file + pattern; log timestamp; `grep -E "${search_str//./.}"` (the substitution does **not** escape dots — `${var//./.}` replaces `.` with `.`). Prefixes hits with `OUTPUT RESULT --->` via `sed`, `tee -a` log.

**`copy_content_function`:**

1. Read `N`.
2. `files_var=($(ls -p | grep -v / | grep -v '\.sh$'))` — regular files in **cwd**, excluding directories and `*.sh`.
3. 1-based index even → `even_files_var`, odd → `odd_files_var`.
4. Truncate `dummy_file.txt`; `head -n "$N"` from even files; `tail -n "$N"` from odd files; `cat` the result.

**`check_modified_date_function`:** `var1=$(stat -c %Y file)` epoch mtime; `var2=$(date +%s)`; `var3=$(( (var2-var1)/3600 ))` hours. If `var3 > 24`, `touch` the file (updates mtime to now).

**Menu loop:** options 1–4, 5 exits with `SCRIPT TERMINATED AT $(date)`.

### Q03 — `Q_03.sh`

- `log_file_path="$HOME/app_usage.log"`
- `declare -A app_usage`
- `arr1=("chrome" "gnome-terminal" "nautilus" "gedit" "evince")` treated as **non-productive** (includes the terminal and file manager).
- `LIMIT=1` **minute**.

**`get_active_window_function`:** `xdotool getwindowfocus getwindowname 2>/dev/null`.

**`track_usage_function`:** every call adds **10 seconds** to `app_usage["$active_app_var"]`; appends `HH:MM:SS - window title` to the log. If the title **contains** a string from `arr1` (substring `[[ "$active_app_var" == *"$i"* ]]`) **and** accumulated seconds `>= LIMIT*60` (60s): `notify-send` and `pkill "$i"`.

**`generate_report_function`:** `clear`; print each app as `MINUTES` / `SECONDS`.

**Main:** infinite `track; report; sleep 10`. Needs an X11 session (`xdotool`).

### Q04 — C string workers

**`main_file.c`**

- `scanf("%s", str)` — **no spaces** in the input string (`str[50]`).
- `open("input.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)`; `write` string + newline.
- Loop menu `1..6`; `scanf(" %c", &ch1)`; `'6'` breaks.
- Double `fork` + `execlp` as above. Invalid choice prints and `sleep(2)` in the grandchild.
- After leaving the loop: `system("cat output.txt")`.

**`reverse_str.c`**

- `open("input.txt", O_RDONLY)`; `read` into `str[50]`; strip trailing `\n`.
- In-place reverse `for i in 0 .. len/2`.
- Append to `output.txt` (`O_APPEND`, mode `0666`):  
  `TASK: REVERSE | PID: %d | RESULT: %s`

**`length_str.c`**

- Same read/strip; `len` after newline removal.
- Append: `TASK: FIND LENGTH | PID: %d | RESULT: %d`

**GitHub-only workers (from `TREE.txt` + `main_file` cases):**

| Binary | `execlp` | Intended effect |
|--------|----------|-----------------|
| `ascii_add_2_str` | case `'3'` | Add 2 to each character’s ASCII |
| `sort_str` | case `'4'` | Sort characters |
| `capitalize_str` | case `'5'` | Uppercase |

Workers must live in the **current directory** (`./reverse_str`). They do not take argv strings; they only read `input.txt`.

### Q05 — `Q_05.sh`

- `data_file="expenses.csv"`; `budget=100000`; `touch $data_file`.
- **Add:** prompts amount, category (`FOOD`, `BILLS`, …), date `YYYY-MM-DD`; appends `date,category,amount`.
- **Summary:** `awk -F ',' '{arr[$2]+=$3} END {…}'` per category; `total_expenses=$(awk -F',' '{sum+=$3} END {print sum}')`; `savings=$((total_income - total_expenses))` — **`total_income` is never set** (empty → arithmetic error / 0).
- Alert if `total_expenses > budget`.
- **Export:** `cat $data_file` (does not write a new file despite the “Expenes.csv” message).
- **Visualize:** require `gnuplot`; awk `NF==3 && $2!="Income" && $3 ~ /^[0-9]+$/` sum by date → `expenses.dat`; plot `using 1:2 with linespoints`, `pause 5`.
- **Set budget:** regex `^[0-9]+$` then assign `budget`.

---

## File-by-file reference

| Path (workspace) | Original path | Notes |
|------------------|---------------|-------|
| `Q_01.sh` | `…/Q_01_Sol/Q_01.sh` | ~820 lines including appended hashing essays |
| `Q_02.sh` | `…/Q_02_Sol/Q_02.sh` | Menu + test-case comments |
| `Q_03.sh` | `…/Q_03_Sol/Q_03.sh` | Needs `xdotool`, `notify-send` |
| `Q_05.sh` | `…/Q_05_Sol/Q_05.sh` | Ends with a `halt` vs `poweroff` note (not executed) |
| `main_file.c` | `…/Q_04_Sol/main_file.c` | Orchestrator |
| `reverse_str.c` | `…/Q_04_Sol/reverse_str.c` | Reverse worker |
| `length_str.c` | `…/Q_04_Sol/length_str.c` | Length worker |
| *(GitHub)* | `ascii_add_2_str.c`, `sort_str.c`, `capitalize_str.c` | Missing from this extract |
| *(GitHub)* | `Assignment 1.pdf`, `Assignment #02/` | Briefs |

---

## I/O formats

### Q02 log `text_file.txt`

Plain lines from `tee -a`, including `OPTION 02 SELECTED AT …`, grep hits, permission summaries.

### Q03 `~/app_usage.log`

```
10:00:10 - Google Chrome
10:00:20 - Google Chrome
```

### Q04 `input.txt`

```
hello
```

### Q04 `output.txt` (appended per operation)

```
			TASK: REVERSE | PID: 4321 | RESULT: olleh
			TASK: FIND LENGTH | PID: 4322 | RESULT: 5
```

### Q05 `expenses.csv`

```
            2025-03-01,FOOD,250
            2025-03-02,BILLS,1200
```

(Leading spaces are written by the `echo "            $date_var,…"` line.)

### Q05 `expenses.dat` (gnuplot)

```
2025-03-01 250
2025-03-02 1200
```

---

## Tech stack

Bash (`#!/bin/bash`), GNU coreutils (`find`, `du`, `sort`, `awk`, `xargs`, `tee`, `stat`), `md5sum`, `chmod`, `grep -E`, C89/C99 POSIX (`unistd.h`, `fcntl.h`, `sys/wait.h`), `xdotool`, `libnotify` (`notify-send`), optional **gnuplot**.

---

## Repository structure

```
OS-ShellScripting/
├── Q_01.sh
├── Q_02.sh
├── Q_03.sh
├── Q_05.sh
├── main_file.c
├── reverse_str.c
├── length_str.c
└── Assignment #01/A_22I-2327_Mohammad_Rohaan/   # GitHub
    ├── Q_01_Sol/Q_01.sh
    ├── Q_02_Sol/Q_02.sh
    ├── Q_03_Sol/Q_03.sh
    ├── Q_04_Sol/{main_file,reverse_str,length_str,ascii_add_2_str,sort_str,capitalize_str}.c
    └── Q_05_Sol/Q_05.sh
```

---

## Build and run

Linux/WSL (Q01 `find "$HOME"`, Q03 X11, Q04 `fork`).

```bash
chmod +x Q_01.sh Q_02.sh Q_03.sh Q_05.sh
./Q_01.sh          # DESTRUCTIVE if you answer Y
./Q_02.sh          # cwd should contain test files for option 3
./Q_03.sh          # sudo apt install xdotool libnotify-bin
./Q_05.sh          # sudo apt install gnuplot   # for option 4
```

Q04:

```bash
gcc reverse_str.c -o reverse_str
gcc length_str.c -o length_str
gcc ascii_add_2_str.c -o ascii_add_2_str   # if present
gcc sort_str.c -o sort_str
gcc capitalize_str.c -o capitalize_str
gcc main_file.c -o main_file
./main_file
```

Keep binaries in the same directory as `main_file` so `execlp("./reverse_str", …)` resolves (`execlp` searches `PATH` unless the name contains `/`; here the first argument is `"./reverse_str"` so it is path-based).

---

## Usage

**Q01:** Run in a VM or throwaway home. Do not run as root unless you intend to wipe `/var/log` and `apt autoremove`. Duplicate detection is off unless you uncomment the function call.

**Q02 option 3:** Create `file1.txt` … `file5.txt` as in the script comments, then choose N (e.g. 2).

**Q03:** Focus Chrome for 60+ seconds of *tracked* time (6 loops × 10s increment) to hit `LIMIT=1` minute and trigger `pkill chrome`.

**Q04:** Enter a single token (e.g. `OperatingSystems`), pick 1 then 2, then 6 to dump `output.txt`.

**Q05:** Option 1 several times, option 2 for awk totals, option 5 to set budget, option 4 for a 5-second gnuplot window.

---

## Constants

| Name | Value | File |
|------|-------|------|
| Top-N files | 10 | Q01 |
| Duplicate hash | MD5 | Q01 |
| Cache paths | `~/.cache`, Firefox `cache2`, Chrome `Default/Cache` | Q01 |
| atime window | `-10` days (see bug vs comments) | Q01 |
| Q02 log | `text_file.txt` | Q02 |
| Q03 log | `$HOME/app_usage.log` | Q03 |
| Q03 poll | 10 seconds | Q03 |
| Q03 increment | +10 s per poll | Q03 |
| `LIMIT` | 1 minute | Q03 |
| `arr1` | chrome, gnome-terminal, nautilus, gedit, evince | Q03 |
| `str[50]` | max input | Q04 |
| `input.txt` mode | `0644` | Q04 |
| `output.txt` mode | `0666` append | Q04 |
| Default budget | `100000` | Q05 |
| Date format | `%Y-%m-%d` | Q05 gnuplot |

---

## Results

No captured runtime logs in this extract. Expected artifacts after a lab session: `text_file.txt`, `dummy_file.txt`, `~/app_usage.log`, `input.txt` / `output.txt`, `expenses.csv` / `expenses.dat`.

---

## Limitations

- Q01 can **delete user files** and browser caches; duplicate finder is commented out; atime predicate contradicts the English prompt.
- Q02 permission “toggle” is a fixed lookup table, not a user-chosen `chmod` octal.
- Q03 treats the terminal as non-productive and will kill it after 1 minute of focus.
- Q04 `scanf("%s")` cannot accept phrases; no check that `open`/`execlp` succeeded (`execlp` failure falls through to `exit(0)`).
- Q05 `total_income` undefined; CSV has leading spaces that can break `NF==3` gnuplot filter; amounts must be integers for the plot regex `^[0-9]+$`.
- Q04 extra `.c` files may be absent in a partial clone.

---

## How to extend

- Uncomment and fix Q01 hard-link (`file_map` lookup per duplicate).
- Change Q01 to `-atime +10` if the spec is unused files.
- Q04: `getline` for spaces; `perror` on `execlp`; compile all five workers from a tiny Makefile.
- Q05: set `total_income` or drop savings; `trim` CSV fields.

---

## Author

**Mohammad Rohaan**  
Student ID: **22I-2327**  
GitHub: **rohaan2802**  
Course: Operating Systems — shell scripting and process control lab.

Do not publish these scripts as “safe cleanup tools” without the warnings above.
