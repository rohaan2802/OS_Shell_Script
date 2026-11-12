#!/bin/bash
# Specifies that this script should be executed using the Bash shell.


echo -e "\n\n\n\t\t\t SOLUTION OF PROBLEM# 01 (DISK CLEANUP)....... !\n\n"


# Function to find and delete the largest files
identify_largest_files_function() 
{
    echo -e "\n\n\t\t---->  IDENTIFYING THE TOP 10 LARGEST FILES IN THE HOME DIRECTORY.......!\n\n"
    sleep 2    
    # find "$HOME" -type f --> Finds all files in the home directory.
    # find                 --> A command to search for files and directories.
    # "$HOME"              --> The user's home directory (e.g., /home/username in UBUNTO).
    # -type f              --> Specifies that we are looking for files only (not directories).
    
    
    # -exec du -ah {} +    --> Gets file sizes in a human-readable format (-h).
    # -exec                --> Allows executing a command on the found files.
    # du                   --> Stands for "disk usage"; used to check file sizes.
    # -a                   --> Shows file sizes for all files.
    # -h                   --> Displays sizes in human-readable format (KB, MB, GB).
    # {}                   --> Represents each found file.
    # +                    --> Optimizes execution by passing multiple files at once instead of running du separately for each.
    
    
    # | sort -rh           --> Sorts results in descending order by size (-r means reverse).
    # |                    --> Pipes the output from du to sort.
    # -r 		       --> Sorts results in reverse order (largest files first).
    # -h                   --> Sorts based on human-readable file sizes (so "1G" is greater than "500M").

    # | head -10           --> Displays only the top 10 largest files.
    
    l_files=$(find "$HOME" -type f -exec du -ah {} + | sort -rh | head -10)
    
    # echo "$largest_files"--> Displays the largest files.
    echo -e "$l_files"
    
    echo -e "\n\n\n\t\t\t DO YOU WANT TO DELETE THESE FILES ? ('Y'/'N')       \c"
    read -r ch1
    echo -e "\n\n\n"
    # echo "$l_files"      --> Prints the list of top 10 largest files stored in the variable $l_files.
    
    # | awk '{print $2}'
    # awk                  --> A text-processing tool.
    # {print $2}           --> This selects the second column from each line of the list.
    
    # | xargs rm -i
    # xargs                --> Converts input from standard output (file list) into command arguments.
    # rm                   --> Removes files.
    # -i                   --> Interactive mode → Asks for confirmation before deleting each file.
    
    # Why use xargs?
    # Handles multiple files (even if separated by newlines or spaces).
    # Avoids "argument list too long" errors.
    # Works with commands that don’t accept piped input directly.
    #  Normally, rm cannot delete multiple files from a list , Using xargs, it works
    # if you want no confirmation while deleting files, replace  xargs rm -i with: xargs rm -f
    
    if [[ "$ch1" == "y" || "$ch1" == "Y" ]]; then
        echo "$l_files" | awk '{print $2}' | xargs rm -i
    fi
}




# Function to find and process duplicate files
detect_duplicates_files_function()
{
    echo -e "\n\n\n\n\t\t\t---->  DETECTING DUPLICATE FILES ......!\n\n"
  
    # declare -A file_map: Declares an associative array (key-value pairs) to store file checksums.
    # declare is used to define variables with special properties.
    # -A tells Bash that file_map is an associative array (like a dictionary in Python or a hashmap in C++).
    # file_map is the name of the associative array.
  
    # What is an Associative Array?
    # In Bash, normal arrays use numeric indices (array[0], array[1]).
    # Associative arrays use keys instead of numbers.
    
    declare -A file_map
    
    # duplicates is a normal (indexed) array, not an associative array.
    # =() initializes it as an empty array.
    
    # declare -A file_map	Creates an associative array to store key-value pairs (e.g., filename → hash).
    # duplicates=()	Initializes an empty normal array for storing duplicate file names.
    
    duplicates=()
    
    # find "$HOME" -type f -print0 --> Finds all files and prints them null-terminated (safer for spaces in filenames).
    # while IFS= read -r -d '' file; do ... done --> Loops over each file.
    # checksum=$(md5sum "$file" | awk '{print $1}') --> Computes an MD5 checksum for each file.
    # if [[ -n "${file_map[$checksum]}" ]]; then duplicates+=("$file"); else file_map[$checksum]="$file"; fi:
    # If the checksum already exists in file_map, this file is a duplicate, so it's added to the duplicates array.
    # Otherwise, it's stored in file_map as a unique file.
    
    # Step 1: while IFS= read -r -d '' file; do

    # while → Starts a loop that reads filenames.
    # IFS= → Sets the Internal Field Separator to an empty value to prevent trimming whitespace.
    # read -r -d '' file:
    # read -r → Reads input without interpreting backslashes (-r prevents escape character issues).
    # -d '' → Reads input until a null character (\0) is found. This ensures filenames with spaces or special characters are read 
    # correctly.
    # file → Stores each filename read from the find command.
    # What is this doing?
    # It reads filenames one by one from the find command without issues caused by spaces or special characters.

    # Step 2: Generate MD5 Hash of Each File


    # md5sum "$file" → Generates an MD5 checksum (unique hash) for the file.
    # awk '{print $1}' → Extracts only the hash value (ignoring the filename).
    # Each file's content is converted into a unique hash to detect duplicates.

    # Step 3: Check If Hash Exists in file_map
    
    # ${file_map[$checksum]} → Checks if this hash already exists in the file_map associative array.
    # -n → Checks if the value is not empty (i.e., a duplicate exists).
    # duplicates+=("$file") → Adds the duplicate file to the duplicates array.
    # If the hash already exists, the file is a duplicate and is added to the duplicates array.
    
    # Step 4: If Not Duplicate, Store in file_map 
    
    # if the file is not a duplicate, we store the hash → filename pair in the file_map

    # Step 5: Feed Filenames to the Loop
    # done < <(...) → Runs process substitution, treating output of find as input to while read.
    # find "$HOME" -type f -print0:
    # find "$HOME" → Searches for all files in the home directory.
    # -type f → Includes only files (no directories).
    # -print0 → Uses null-terminated filenames to handle spaces and special characters.

    # Step 6: Check If Any Duplicates Exist
    # ${#duplicates[@]} → Gets the count of duplicate files.
    # -eq 0 → Checks if the count is zero (no duplicates).
    # return → Exits the function/script.
    # If no duplicate files are found, it prints a message and exits.

    # Step 7: Print Duplicate Files
    # printf '%s\n' "${duplicates[@]}":
    # Prints each duplicate file on a new line.
    # More efficient than echo for handling large lists.
    # If duplicate files are found, it displays the list.


    while IFS= read -r -d '' file; do
        checksum=$(md5sum "$file" | awk '{print $1}')
        if [[ -n "${file_map[$checksum]}" ]]; then
            duplicates+=("$file")
        else
            file_map[$checksum]="$file"
        fi
    done < <(find "$HOME" -type f -print0)
    
    if [[ ${#duplicates[@]} -eq 0 ]]; then
        echo -e "\n\n\t\t\tNO DUPLICATE FILES FOUND ......!\n\n"
        return
    fi
    
        echo -e "\n\n\t\t\tDUPLICATE FILES FOUND ......!\n\n"
    printf '%s\n' "${duplicates[@]}"
    
    
   # A hard link is a file system feature that allows multiple filenames to point to the same physical data on disk. Unlike  
   # symbolic (soft) links, hard links share the same inode, meaning they are direct references to the actual file data.

   # Why Use Hard Links in This Script?
   # If two or more duplicate files exist, instead of keeping multiple copies, we can replace the duplicates with hard links. This 
   # saves space while keeping all filenames intact.

   # Example:
   # Suppose you have two identical files:

   # /home/user/file1.txt
   #/home/user/Documents/file2.txt

   # If they are duplicates, instead of storing two copies, we can hard-link one to the other:

   # ln /home/user/file1.txt /home/user/Documents/file2.txt

   # Now, both file1.txt and file2.txt point to the same data on disk. Deleting one of them does not delete the actual data unless 
   # all hard links to that inode are removed.

   # A hard link is NOT an alias, but rather a second (or more) reference to the same physical data on disk.

   # Key Properties of Hard Links:
   # Same Data, Multiple Names

   # A hard link is not a separate file but another name for the same file.
   # Both the original file and the hard link share the same inode (a unique identifier for file data).

   # Changes Reflect in Both

   # If you modify the contents of one file, changes appear in all hard-linked versions as they point to the same physical data.

   # Deleting a Hard Link Does NOT Delete Data

   # The actual data is only removed when all hard links to it are deleted.
   # Example:

   # echo "Hello" > file1.txt
   # ln file1.txt file2.txt   # Creates a hard link to file1.txt
   # Now, file1.txt and file2.txt are the same file on disk. If you do:

   # echo "World" >> file2.txt
   # Both file1.txt and file2.txt will now contain:

   # Hello
   # World
   # because they are just different names pointing to the same data.

   # However, if you delete file1.txt, file2.txt still exists and retains the data.

   # Difference from an Alias (or Soft Link):
   # Aliases or symbolic (soft) links just point to a file path. If the original file is deleted, the link breaks.
   # Hard links are real references to the data itself and continue to function even if the original filename is removed.
  
  
   # Asks the user how they want to handle the duplicates.
   
    echo -e "\n\n\t\t\tCHOOSE THE OPTION GIVEN BELOW :"
    echo -e "\n\n\t\t\t1. DELETE DUPLICATES"
    echo -e "\n\n\t\t\t2. HARD-LINK DUPLICATES"
    echo -e "\n\n\t\t\t3. MOVE DUPLICATE FILES TO ~/duplicates/"
    echo -e "\n\n\t\t\t   ENTER YOUR CHOICE HERE:    \c"
    read -r var # read the input in var variable
    echo -e "\n\n\n"
    # Case Statement:
    # 1) rm -i "${duplicates[@]}": Deletes duplicates interactively.
    # 2) for dup in "${duplicates[@]}"; do ln -f "${file_map[$checksum]}" "$dup"; done:
    # Replaces duplicates with hard links to the first detected version.
    # 3) mkdir -p "$HOME/duplicates" && mv -i "${duplicates[@]}" "$HOME/duplicates/":
    # Moves duplicates to a ~/duplicates/ directory.



    # Step 1: Case Statement Syntax
    # case → Starts a case statement (similar to switch in C/C++).
    # $var → The variable being checked.
    # in → Begins case matching.
    # It checks the value of $var and executes the corresponding block.


    # Step 2: Case 1 → Delete Duplicate Files
    # 1) → If $var is 1, execute this block.
    # rm -i "${duplicates[@]}":
    # rm → Deletes files.
    # -i → Interactive mode (asks for confirmation before deleting).
    # "${duplicates[@]}" → Expands to all duplicate files.
    # If the user selects option 1, it prompts before deleting each duplicate file.

    
    # Step 3: Case 2 → Replace Duplicates with Hard Links
    # 2) → If $var is 2, execute this block.
    # for dup in "${duplicates[@]}" → Loops through each duplicate file.
    # ln -f "${file_map[$checksum]}" "$dup":
    # ln → Creates a hard link.
    # -f → Forces overwriting if the file exists.
    # "${file_map[$checksum]}" → The original (first) file stored in the associative array.
    # "$dup" → The duplicate file.
    # If the user selects option 2, it replaces all duplicate files with hard links to the original file.
    # This saves space because hard links share the same data.
    
    
    # Step 4: Case 3 → Move Duplicates to a Folder
    # 3) → If $var is 3, execute this block.
    # mkdir -p "$HOME/duplicates" --> Creates a folder named duplicates in the home directory.
    #                             --> -p ensures it does not fail if the folder already exists.
    # && → Ensures the next command runs only if the folder was created successfully.
    # mv -i "${duplicates[@]}" "$HOME/duplicates/" --> Moves duplicate files to ~/duplicates/.
    # 					 --> -i asks for confirmation before overwriting.
    # If the user selects option 3, it moves all duplicate files to a "duplicates" folder instead of deleting them.
    
    
    # Step 5: Ending the Case Statement
    # esac → Ends the case statement.
    
    
    case $var in
        1) rm -i "${duplicates[@]}" ;;
        2) for dup in "${duplicates[@]}"; do
               cs=$(md5sum "$dup" | awk '{print $1}')
               ln -f "${file_map[$cs]}" "$dup"
           done ;;
        3) mkdir -p "$HOME/duplicates" && mv -i "${duplicates[@]}" "$HOME/duplicates/" ;;
    esac
}



# Function to clean system files
clean_unnecessary_system_files_function() 
{
    # Deletes browser cache and system cache files.
    # rm -rf
    # rm → Remove (delete) files or directories.
    # -r → Recursive, meaning it deletes all files/folders inside directories.
    # -f → Force delete, ignores errors (does not ask for confirmation).

    # $HOME/.cache/"*
    # $HOME/.cache/ → The general cache directory in your home folder.
    # * → Deletes all files and folders inside .cache/.

    # $HOME/.mozilla/firefox/*.default-release/cache2/"*
    # Firefox stores cache files inside cache2/ in the profile folder.
    # *.default-release/ → Matches the default Firefox profile.
    # cache2/* → Deletes everything inside the cache folder.


    # $HOME/.config/google-chrome/Default/Cache/"*
    # Google Chrome stores cache in:
    # $HOME/.config/google-chrome/
    # Inside the Default/Cache/ folder.
    # * → Deletes all cached files.


    # What Does This Script Do?
    # Prints a message saying "Cleaning Cache Files".
    # Deletes all cache files from:
    # System cache folder ($HOME/.cache/).
    # Firefox cache (cache2/ inside *.default-release/).
    # Google Chrome cache (Default/Cache/).


    # Warning
    # This script will permanently delete all cache files.
    # Some apps may take longer to load the first time after clearing the cache.
    # Do not run as sudo, or it may delete system files.
    
    echo -e "\n\n\n\t\t\t---->  CLEANING CACHE FILES .....!\n\n"
    rm -rf "$HOME/.cache/"* "$HOME/.mozilla/firefox/*.default-release/cache2/"* "$HOME/.config/google-chrome/Default/Cache/"*
    
    
    # If running as root, cleans system logs (/var/log/) and removes unused packages (apt autoremove).
    # If not root, skips log cleanup.
    
    if [[ "$EUID" -eq 0 ]]; then
        echo -e "\n\n\t\t\tCLEANING OLD LOGS ........!"
        rm -rf /var/log/*.log /var/log/*.gz
        echo -e "\n\n\t\t\tRUNNING APT AUTOREMOVE......!"
        apt autoremove -y
    else
        echo -e "\n\n\t\t\tSKIPPING LOG CLEANUP (RECQUIRES ROOT) !"
    fi
}



# Function to suggest files for deletion based on access time
suggest_old_files_function()
{
    # Step 1: Print a Message
 
    echo -e "\n\n\n\n\t\t\t---->  FINDING FILES NOT ACCESSED IN THE LAST 10 DAYS.......!"
    sleep 2
    # Step 2: Find Old Files   
    # Finds files not accessed in the last 3 months (-atime +90).
    # Difference Between +90 and -90
    # -atime +90	Finds files not accessed in the last 90+ days (older than 3 months).
    # -atime -90	Finds files accessed within the last 90 days (recent files).
    # Use -atime +90 → To find & delete old unused files.
    # Use -atime -90 → To list files still in use.

    old_files=$(find "$HOME" -type f -atime +10)
    
     if [ -z "$old_files" ]; then
        echo -e "\n\n\n\t\t\t NO FILES FOUND THAT HAVE NOT BEEN ACCESSED IN THE LAST 10 DAYS."
        return
     fi
    # find → Search for files based on conditions.
    # "$HOME" → Searches inside the user's home directory.
    # -type f → Looks for files only (ignores directories).
    # -atime +90 → Finds files that have not been accessed in the last 90 days (3 months).
    # $(...) → Stores the result inside the old_files variable.

    # Step 3: Display Found Files
    # Prints the list of old files found by find.

    echo "$old_files"
    
    # Step 4: Ask the User for Deletion Confirmation
    
    
    echo -e "\n\n\n\t\t\t DO YOU WANT TO DELETE THESE OLD FILES ? ('Y'/'N')       \c"
    read -r ch2
    echo -e "\n\n\n"
    # Step 5: Delete Old Files If User Confirms
    
    if [[ "$ch2" == "y" || "$ch2" == "Y" ]]; then
        echo "$old_files" | xargs rm -i
    fi
}


# Run the functions

identify_largest_files_function
#detect_duplicates_files_function
clean_unnecessary_system_files_function
suggest_old_files_function

echo -e "\n\n\n\n\t\t\t*******************  DISK CLEANUP COMPLETED  *******************\n\n\n"
