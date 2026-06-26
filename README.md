# tui-file-manager

# beta v0.1

I started this project on June 22, 2026, without knowing any libraries.

Over three days of learning and coding.

In the future, I plan to further develop the project in all areas. I also plan to create a logo for it.

For now, the program has basic functionality—specifically, viewing folders but not modifying them. There is also a basic command, END, to exit the program. The right arrow is used to enter a folder, while the left arrow is used to exit.
One of the project's goals is to implement intuitive, one-handed controls.

Here are brief instructions on how to launch it:
Installing dependencies:
```bash
sudo apt install libncurses-dev     --Debian/Ubuntu
```
```bash
sudo dnf install ncurses-devel      --Fedora
```
Then, open any text editor and paste my code. Save it to any folder , name 'fs'. Afterward, go to the terminal and enter:
```bash
cd ~/your/path
```
Enterance to super user:
```bash
sudo su
```

Next comes compilation and execution:

```bash
g++ fs.cc -o filemanager -lncursesw
./filemanager
```
Or you could just download the binary.
Link --> https://github.com/dmitriysypachev6-coder/tui-file-manager/archive/refs/tags/v0.1-beta.zip
Link2 --> https://github.com/dmitriysypachev6-coder/tui-file-manager/archive/refs/tags/v0.1-beta.tar.gz

# beta v0.2

This version added the ability to create and delete files and folders. In the future, I plan to add the ability to open .txt files in the nano text editor or any other terminal-based text editor.



