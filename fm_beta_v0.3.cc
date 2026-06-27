#include <iostream>
#include <filesystem>     // Библиотека для работы с системными файлами
#include <ncursesw/ncurses.h>      // Библиотека для навигации
#include <locale.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

std::string formatSize(unsigned long long bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes) + " B";
    } else if (bytes < 1024 * 1024) {
        double kb = bytes / 1024.0;
        return std::to_string(kb).substr(0, 4) + " KB";
    } else if (bytes < 1024 * 1024 * 1024) {
        double mb = bytes / (1024.0 * 1024.0);
        return std::to_string(mb).substr(0, 4) + "MB";
    } else {
        double gb = bytes / (1024.0 * 1024.0 * 1024.0);
        return std::to_string(gb).substr(0, 4) + " GB";
    }   
}

int main() {
    setlocale(LC_ALL, "");

    initscr();
    set_escdelay(0);  // Убираем задержку ESC
    cbreak();             // Что бы не нажимать Enter
    noecho();             // Что бы не показывать вводимые символы
    keypad(stdscr, TRUE); // Управление стрелками
    
    fs::path currentPath;   // Путь к файлу
    int cursorPosition = 0;
    int offset = 0;  // Смещение для скролла
    const int visibleLines = 34;  // Сколько файлов видно на экране
    
     const char* homeDir = getenv("HOME");

    if (homeDir != nullptr) {
        currentPath = homeDir;
    } else {
        currentPath = "/";
    }


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

        // Выводим только видимые файлы
        for (int i = offset; i < offset + visibleLines && i < filename.size(); i++) {
           if (i == cursorPosition) {
                attron(A_REVERSE);
           }

            if (isDir[i]) {
                printw("[DIR] %s\n", filename[i].c_str());
            } else {
                // Получаем размер файла 
                std::error_code ec;
                auto size = fs::file_size(currentPath / filename[i], ec);
                if (!ec) {
                    std::string sizeStr = formatSize(size);
                    printw("[FILE] %s (%s)\n", filename[i].c_str(), sizeStr.c_str());
                } 
            }

           if (i == cursorPosition) {
                attroff(A_REVERSE);
           }
        }        
    
        // Создаем переменные для команд
        printw("\nComand ('end || esc' - quit, '<--' || 'a' - back to parent path, '--> || d' - go to folder, 'q' - new file, 'z' - new folder, delete -- 'e')");
        printw("\nEnter the function: ");

        refresh();

        int ch = getch();
        
        
        if (ch == 'w' || ch == 'W' || ch == KEY_UP) {
            if (cursorPosition > 0){
                cursorPosition--;
                if (cursorPosition < offset) {
                    offset--;
                }
            }   
        }
        else if (ch == 's' || ch == 'S' || ch == KEY_DOWN) {
            if (cursorPosition < filename.size() -1) {
                cursorPosition++;
                if (cursorPosition >= offset + visibleLines) {
                    offset++;
                }
            }  
        }
        else if (ch == 'a' || ch == 'A' || ch == KEY_LEFT) {
            currentPath = currentPath.parent_path();
            cursorPosition = 0;
            offset = 0;  // Сбрасываем скролл при выходе
        }
        else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT) {
            if (isDir[cursorPosition]) {
                currentPath = currentPath / filename[cursorPosition];
                cursorPosition = 0;
                offset = 0;  // Сбрасываем скролл при входе
            }
        }
        else if (ch == KEY_END) {
            break;
        }
        else if (ch == 27) {
            break;
        }
        else if (ch == 'q' || ch == 'Q') {
            echo();        // Отоброжение вводимых символов
            char newfilename[256];     // Буфер для имени файла
            printw("\nEnter name file: ");
            refresh();
            getstr(newfilename);       // Получаем имя файла
            noecho();
            std::ofstream newFile(currentPath / newfilename);    // Создаем файл
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
        else if (ch == 'e' || ch == 'E') { 
            if (!filename.empty() && cursorPosition < filename.size() ) {
                fs::path targetPath = currentPath / filename[cursorPosition];
                
                printw("Are you sure? (y/n): ");
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
        else if (ch == 'z' || ch == 'Z') {
            echo();
            char foldName[256];
            printw("Enter name folder: ");
            refresh();
            getstr(foldName);
            noecho();
            bool created = std::filesystem::create_directory(currentPath / foldName);
        } 
        else if (ch == 'x' || ch == 'X') {
            if (!isDir[cursorPosition]) {
                def_prog_mode();
                endwin();

                std::string fullPath = currentPath / filename[cursorPosition];
                system (("nano " + fullPath).c_str());

                reset_prog_mode();
                refresh();
                clear();
            }
        }
    }
    endwin();
    return 0;
}
