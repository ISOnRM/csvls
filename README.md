# csvls

**csvls** is a command-line utility that lists files and directories in **CSV format**.

It provides flexible options to display file metadata such as type, owner, group, size, permissions, timestamps, and more.

---

## Installation

### Build from source

Clone the repository and build with **make**:

```bash
git clone https://github.com/yourname/csvls.git
cd csvls
make
sudo make install
```

---

## TL;DR

```bash
csvls -gOst -S
```

Output:

```csv
type,owner,group,size,path
D,mark,users,4.0K,.
F,mark,users,0B,./ThisIsAFile
L,mark,users,12B,./ThisIsALink
D,mark,users,4.0K,./ThisIsADir
```

---

## Usage

```bash
csvls -h
```

```text
Usage:
  csvls [options] <targets>...

Description:
  Targets are given without a leading "-" and must be existing directories or files.
  Multiple targets can be passed.

Options:
  -h, --help               Show this help message and exit.
                           If combined with other options, they will be ignored.
  -d, --show-dev           Display device ID.
  -I, --show-inode         Display inode number.
  -t, --show-type          Display file type.
  -p, --show-perms         Display file permissions.
  -n, --show-nlinks        Display number of hard links.
  -O, --show-owner         Display owner’s user ID.
  -g, --show-group         Display group ID.
  -s, --show-size          Display size in bytes.
  -b, --show-blocks        Display number of blocks.
  -a, --show-access-time   Display last access time.
  -m, --show-mod-time      Display last modification time.
  -M, --show-meta-mod-time Display metadata change time.
  -N, --name               Display entry name.
  -c, --canonical          Display canonical path.
  -S, --sort               Sort output by selected columns.
  -r, --recursive          Enable recursive directory traversal.
```

---

## Examples

### Pass multiple directories

```bash
csvls -t . ThisIsADir   # --show-type
```

```csv
type,path
D,.
F,./ThisIsAFile
L,./ThisIsALink
D,./ThisIsADir
D,ThisIsADir
F,ThisIsADir/FileOne
F,ThisIsADir/FileThree
F,ThisIsADir/FileTwo
```

### Pass a file (or multiple files)

```bash
csvls -sOg ThisIsAFile   # --show-size --show-owner --show-group
```

```csv
size,owner,group,path
0B,mark,users,ThisIsAFile
```

### Show device

```bash
csvls -d   # --show-dev
```

```csv
device,path
66311,.
66311,./ThisIsAFile
66311,./ThisIsALink
66311,./ThisIsADir
```

### Show inode

```bash
csvls -I   # --show-inode
```

```csv
inode,path
10394808,.
10394993,./ThisIsAFile
10395323,./ThisIsALink
10395317,./ThisIsADir
```

### Show entries' permissions

```bash
csvls -p   # --show-perms
```

```csv
permissions,path
rwxr-xr-x,.
rw-r--r--,./ThisIsAFile
rwxrwxrwx,./ThisIsALink
rwxr-xr-x,./ThisIsADir
```

### Show number of hard links

```bash
csvls -n   # --show-nlinks
```

```csv
links,path
3,.
1,./ThisIsAFile
1,./ThisIsALink
2,./ThisIsADir
```

### Show entries' owner

```bash
csvls -O   # --show-owner
```

```csv
owner,path
mark,.
mark,./ThisIsAFile
mark,./ThisIsALink
mark,./ThisIsADir
```

### Show entries' group

```bash
csvls -g   # --show-group
```

```csv
group,path
users,.
users,./ThisIsAFile
users,./ThisIsALink
users,./ThisIsADir
```

### Show entries' human-readable size

```bash
csvls -s   # --show-size
```

```csv
size,path
4.0K,.
0B,./ThisIsAFile
12B,./ThisIsALink
4.0K,./ThisIsADir
```

### Show amount of blocks

```bash
csvls -b   # --show-blocks
```

```csv
blocks,path
8,.
0,./ThisIsAFile
0,./ThisIsALink
8,./ThisIsADir
```

### Show entries' access time

```bash
csvls -a   # --show-access-time
```

```csv
access_time,path
02-08-2025 12:50:22,.
02-08-2025 12:49:29,./ThisIsAFile
02-08-2025 12:50:22,./ThisIsALink
02-08-2025 12:49:34,./ThisIsADir
```

### Show entries' modification time

```bash
csvls -m   # --show-mod-time
```

```csv
mod_time,path
02-08-2025 12:50:21,.
02-08-2025 12:49:29,./ThisIsAFile
02-08-2025 12:50:21,./ThisIsALink
02-08-2025 12:49:34,./ThisIsADir
```

### Show entries' metadata modification time

```bash
csvls -M   # --show-meta-mod-time
```

```csv
meta_mod_time,path
02-08-2025 12:50:21,.
02-08-2025 12:49:29,./ThisIsAFile
02-08-2025 12:50:21,./ThisIsALink
02-08-2025 12:49:34,./ThisIsADir
```

### Show entries' name (Option::Name is inserted by default)

```bash
csvls -N   # --name
```

```csv
path
.
./ThisIsAFile
./ThisIsALink
./ThisIsADir
```

### Show entries' canonical path instead of their names

```bash
csvls -c   # --canonical
```

```csv
path
/home/mark/csvls_showcase/test
/home/mark/csvls_showcase/test/ThisIsAFile
/home/mark/csvls_showcase/script.py
/home/mark/csvls_showcase/test/ThisIsADir
```

### Recursively traverse directories

```bash
csvls -r   # --recursive
```

```csv
path
.
./ThisIsAFile
./ThisIsALink
./ThisIsADir
./ThisIsADir/FileOne
./ThisIsADir/FileThree
./ThisIsADir/FileTwo
```

### Without --sort

```bash
csvls -sgOt .   # --show-size --show-group --show-owner --show-type
```

```csv
size,group,owner,type,path
4.0K,users,mark,D,.
0B,users,mark,F,./ThisIsAFile
12B,users,mark,L,./ThisIsALink
4.0K,users,mark,D,./ThisIsADir
```

### With --sort

```bash
csvls -sgOt -S .   # --show-size --show-group --show-owner --show-type --sort
```

```csv
type,owner,group,size,path
D,mark,users,4.0K,.
F,mark,users,0B,./ThisIsAFile
L,mark,users,12B,./ThisIsALink
D,mark,users,4.0K,./ThisIsADir
```

---

## Implementation Details

* **No heavy use of `std::filesystem`:**
  Most functions in `std::filesystem` call `stat` multiple times for different attributes.
  Instead, `csvls` calls `stat` once per entry and extracts all attributes directly.

* **Source structure (`src/`):**

  * `aliases_and_concepts/` → Aliases and concepts
  * `cli/` → Argument parser class
  * `checker/` → Functions that validate parsed options and arguments
  * `assembler/` → Class that collects entries and calls `stat`
  * `csv_writer/` → Class that writes CSV output to stdout


## Harsh lesson

  I learned the hard way that if you don't sit down, think, and outline a project's structure on paper beforehand, actual coding becomes a Sisyphean task.
  This would have been ten times easier if I had done so, but unfortunately I did not.
  This is my first ever C++ project of a somewhat decent size. In the process I learned that I love C a lot more.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
