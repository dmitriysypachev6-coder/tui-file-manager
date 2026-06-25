#include <iostream>
#include <filesystem>     // Библиотека для работы с системными файлами
#include <ncursesw/ncurses.h>      // Библиотека для навигации
#include <locale.h>
#include <vector>

namespace fs = std::filesystem;

int main() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak();             // Что бы не нажимать Enter
    noecho();             // Что бы не показывать вводимые символы
    keypad(stdscr, TRUE); // Управление стрелками
    
    fs::path currentPath = "/home/dmitriy";   // Путь к файлу
    int cursorPosition = 0;
    
     while (true){
        clear();
        // Вывод файлов из пути
         printw("\n===%s===\n", currentPath.c_str());
         std::vector<std::string> filename;
         std::vector<bool> isDir;
        
        for (const auto& entry : fs::directory_iterator(currentPath)) {
           filename.push_back(entry.path().filename().string());
           isDir.push_back(entry.is_directory());
        }

        for (int i = 0; i < filename.size(); i++) {
           if (i == cursorPosition) {
                attron(A_REVERSE);
           }

            if (isDir[i]) {
                printw("[DIR] %s\n", filename[i].c_str());
            } else {
                printw("[FILE] %s\n", filename[i].c_str());
            }

           if (i == cursorPosition) {
                attroff(A_REVERSE);
           }
        }        
    
        // Создаем переменные для команд
        printw("\nComand ('end' - quit, '<--' - back to parent path, '-->' - go to folder)");
        printw("\nEnter the function: ");

        refresh();

        int ch = getch();
        
        
        if (ch == KEY_UP) {
            if (cursorPosition > 0){
                cursorPosition--;
            }   
        }
        else if (ch == KEY_DOWN) {
            if (cursorPosition < filename.size() -1) {
                cursorPosition++;
            }  
        }
        else if (ch == KEY_LEFT) {
            currentPath = currentPath.parent_path();
        }
        else if (ch == KEY_RIGHT) {
            if (isDir[cursorPosition]) {
                currentPath = currentPath / filename[cursorPosition];
                cursorPosition = 0;
            }
        }
        else if (ch == KEY_END) {
            break;
        }

    }
    
    endwin();
    return 0;
}
