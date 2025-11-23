# FILE DIFF

It is a tool that shows difference in two files

### Install GCC

For Ubuntu:

```bash
sudo apt install gcc -y
```

## Compile using gcc

```bash
gcc -g -Wall -Wextra -o file_diff.out file_diff.c
```

## Execute the object file

1. Simple execute

```bash
./file_diff.out
```

## For Windows:

**NOTE** Clang will throw error for fopen is deprecated

### Install clang

1. Install Microsoft Visual Studio
2. Add to your windows path:
3. Win+ R > sysdm.cpl
4. Advanced > Environment Variables
5. System variables > Path > Edit
6. Add (C:\Program Files\Microsoft Visual Studio\18\<Version>\VC\Tools\Llvm\bin
   )

### Compile using clang

```bash
clang -c file_diff.c -o file_diff.o
clang file_diff.o -o file_diff_x64.exe
```

### Execute the object file

1. Simple execute

```bash
file_diff_x64.exe
```

## Creating tag

```bash
# 1. Check existing tags
git tag
# 2. Create a valid tag
git tag -a v1.0.0 -m "Release version 1.0.0"
# or lightweight tag
git tag v1.0.0
# push tag to remote
git push origin v1.0.0
```

# Feature could be added later

1. Side-by-side diff mode
2. Word-level diff
3. Output to patch file (.patch)
4. Colored HTML diff
5. Interactive TUI UI
6. Binary diff mode
7. LCS-based perfect diff
