<h1><p align="center">tvar</p></h1>

`tvar` is a simple command-line tool written in C for storing and retrieving temporary variable data, such as the output of scans or scripts. All data is stored in `/tmp/tvar.db` and will be destroyed when the system is rebooted (as `/tmp` is cleared on restart on most Unix/Linux systems).

## Features
- Store arbitrary data from stdin and get a unique numeric variable name (e.g., 101, 102, ...)
- Retrieve data by variable name
- List all stored variable names
- Data is temporary and will be lost after a system reboot

## Usage

### Store Data
Store the output of a command or any text:

```sh
echo "my secret data" | tvar
# Output: 101
```

The output (e.g., `101`) is the variable name you can use to retrieve the data later.

#### Store Data with a Custom Variable Name
You can specify your own variable name (up to 7 characters) using `-n`:

```sh
echo "my secret data" | tvar -n myvar
# Output: myvar
```

If the name already exists, the command will exit with code 2 and print nothing.

### Retrieve Data
Retrieve the data for a given variable name:

```sh
tvar -r 101
# Output: my secret data
```

If the variable does not exist, the command will exit with code 1 and print nothing.

### List All Variables
List all currently stored variable names:

```sh
tvar -l
# Output:
# 101
# 102
# ...
```

### Example Workflow

```sh
# Store the output of a scan and print the variable name
nmap -sV target.com | tvar
# Output: 101

# Store with a custom variable name
nmap -sV target.com | tvar -n scan1
# Output: scan1

# Print the scan result directly through the pipeline
tvar -r 101
# OR
tvar -r scan1

# List all stored variables
tvar -l
```

## Help Menu (for README only)

```
Usage: tvar [OPTION]
  -l           List all stored variable names
  -r VAR       Read and print the data for variable VAR
  -n NAME      Store stdin as a variable with custom name NAME (up to 7 chars)

If no option is given, tvar reads from stdin, stores the data, and prints the new variable name.
```

## Notes
- Data is stored in `/tmp/tvar.db` and will be lost after a system reboot.
- Variable names are sequential numbers starting from 101.
- Maximum 100 variables can be stored at a time.
- No help menu is printed by the program itself; see this README for usage.

## Use Cases
- Store temporary scan results or command outputs for later retrieval in scripts.
- Share data between different shell sessions without writing custom temp files.

---

Feel free to modify or extend `tvar` for your own needs!
