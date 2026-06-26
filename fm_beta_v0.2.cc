#include <iostream>
#include <filesystem>     // Библиотека для работы с системными файлами
#include <ncursesw/ncurses.h>      // Библиотека для навигации
#include <locale.h>
#include <vector>
#include <fstream>

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
        printw("\nComand ('end' - quit, '<--' - back to parent path, '-->' - go to folder, 'ins' - new file, '/' - new folder)");
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
        else if (ch == KEY_IC) {
            echo();        // Отоброжение вводимых символов
            char filename[256];     // Буфер для имени файла
            printw("\nEnter name file: ");
            refresh();
            getstr(filename);       // Получаем имя файла
            noecho();
            std::ofstream newFile(currentPath / filename);    // Создаем файл
            // Проверяем создание файла
            if (newFile.is_open()) {
                printw("\nGreat");
                newFile.close();
            } else {
                printw("\nFail creat file");
            }
        refresh();
        getch();
        }
        else if (ch == KEY_DC) { 
            if (!filename.empty() && cursorPosition < filename.size() ) {
                fs::path targetPath = currentPath / filename[cursorPosition];
                
                printw("\nAre you sure? (y/n)");
                refresh();

                int confirm = getch();
                if (confirm == 'y' || confirm == 'Y') {
                    std::error_code ec;
                    fs::remove_all(targetPath, ec);
                    if (!ec) {
                        printw("\nGreat");
                    } else {
                        printw("\nFail delete file");
                    }
                    if (cursorPosition >= filename.size() -1) {
                        cursorPosition--;
                    }
                }
            }
        }
        else if (ch == '/') {
            echo();
            char foldName[256];
            printw("\nEnter name folder: ");
            refresh();
            getstr(foldName);
            noecho();
            bool created = std::filesystem::create_directory(currentPath / foldName);
        }    
    }
    endwin();
    return 0;
}
