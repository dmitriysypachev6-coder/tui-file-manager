#include <iostream>
#include <filesystem>     // Библиотека для работы с системными файлами
#include <ncursesw/ncurses.h>      // Библиотека для навигации
#include <locale.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

// Функция для красивого размера
std::string formatSize(unsigned long long bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes) + " B";
    } else if (bytes < 1024 * 1024) {
        double kb = bytes / 1024.0;
        return std::to_string(kb).substr(0, 4) + " KB";
    } else if (bytes < 1024 * 1024 * 1024) {
        double mb = bytes / (1024.0 * 1024.0);
        return std::to_string(mb).substr(0, 4) + " MB";
    } else {
        double gb = bytes / (1024.0 * 1024.0 * 1024.0);
        return std::to_string(gb).substr(0, 4) + " GB";
    }   
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    initscr();
    start_color();     // Включаем цвета
    set_escdelay(0);  // Убираем задержку ESC
    cbreak();             // Что бы не нажимать Enter
    noecho();             // Что бы не показывать вводимые символы
    keypad(stdscr, TRUE); // Управление стрелками
    
    // Указываем пары цветов
    init_pair(1, COLOR_BLUE, COLOR_BLACK);   
    init_pair(2, COLOR_WHITE, COLOR_BLACK);  
    
    fs::path currentPath;   // Путь к файлу
    int cursorPosition = 0;
    int offset = 0;  // Смещение для скролла
    const int visibleLines = 34;  // Сколько файлов видно на экране
    
    // Определяем начальную папку
    if (argc >= 2) {
        currentPath = argv[1];  // Если передали путь как аргумент
    } else {
        currentPath = fs::current_path();  // Иначе текущая папка
    }

    bool showHiden = false;

    while (true){
        clear();
        // Вывод файлов из пути
        printw("\n===%s===\n", currentPath.c_str());
        std::vector<std::string> filename;
        std::vector<bool> isDir;

        for (const auto& entry : fs::directory_iterator(currentPath)) {
            std::string name = entry.path().filename().string();
            if (name[0] == '.' && !showHiden) {
                continue;
            }
            filename.push_back(name);
            isDir.push_back(entry.is_directory());
        }

        // Защита от пустой папки
        if (filename.empty()) {
            printw("\n[Папка пуста]\n");
            printw("\nComand ('end || esc' - quit, '<--' || 'a' - back)");
            refresh();
            int ch = getch();
            if (ch == 'a' || ch == 'A' || ch == KEY_LEFT || ch == KEY_END || ch == 27) {
                if (ch == KEY_END || ch == 27) break;
                currentPath = currentPath.parent_path();
                cursorPosition = 0;
                offset = 0;
            }
            continue;
        }

        // Выводим только видимые файлы
        for (int i = offset; i < offset + visibleLines && i < filename.size(); i++) {
            if (i == cursorPosition) {
                attron(A_REVERSE);  // Инверсия для выделения
            }

            if (isDir[i]) {
                attron(COLOR_PAIR(1));  // Синий цвет для папок
                printw("[DIR] %s\n", filename[i].c_str());
                attroff(COLOR_PAIR(1));
            } else {
                // Получаем размер файла 
                std::error_code ec;
                auto size = fs::file_size(currentPath / filename[i], ec);
                if (!ec) {
                    std::string sizeStr = formatSize(size);
                    attron(COLOR_PAIR(2));  
                    printw("[FILE] %s (%s)\n", filename[i].c_str(), sizeStr.c_str());
                    attroff(COLOR_PAIR(2));
                } else {
                    attron(COLOR_PAIR(2));
                    printw("[FILE] %s (no access)\n", filename[i].c_str());
                    attroff(COLOR_PAIR(2));
                }
            }

            if (i == cursorPosition) {
                attroff(A_REVERSE);
            }
        }        
    
        // Создаем переменные для команд
        printw("\nComand ('end || esc' - quit, '<--' || 'a' - back to parent path, '--> || d' - go to folder, 'q' - new file, 'z' - new folder, delete -- 'e', edit -- 'x')");
        printw("\nEnter the function: ");

        refresh();

        int ch = getch();
        
        // Навигация вверх
        if (ch == 'w' || ch == 'W' || ch == KEY_UP) {
            if (cursorPosition > 0){
                cursorPosition--;
                if (cursorPosition < offset) {
                    offset--;
                }
            }   
        }
        // Навигация вниз
        else if (ch == 's' || ch == 'S' || ch == KEY_DOWN) {
            if (!filename.empty() && cursorPosition < filename.size() - 1) {
                cursorPosition++;
                if (cursorPosition >= offset + visibleLines) {
                    offset++;
                }
            }  
        }
        // Выход в родительскую папку
        else if (ch == 'a' || ch == 'A' || ch == KEY_LEFT) {
            currentPath = currentPath.parent_path();
            cursorPosition = 0;
            offset = 0;  // Сбрасываем скролл при выходе
        }
        // Вход в папку
        else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT) {
            if (!filename.empty() && cursorPosition < filename.size() && isDir[cursorPosition]) {
                currentPath = currentPath / filename[cursorPosition];
                cursorPosition = 0;
                offset = 0;  // Сбрасываем скролл при входе
            }
        }
        // Выход по End
        else if (ch == KEY_END) {
            break;
        }
        // Выход по ESC
        else if (ch == 27) {
            break;
        }
        // Создание файла
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
            cursorPosition = 0;
            offset = 0;
        }
        // Удаление файла/папки
        else if (ch == 'e' || ch == 'E') { 
            if (!filename.empty() && cursorPosition < filename.size()) {
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
                    if (cursorPosition >= filename.size() - 1) {
                        cursorPosition--;
                    }
                }
            }
        }
        // Создание папки
        else if (ch == 'z' || ch == 'Z') {
            echo();
            char foldName[256];
            printw("Enter name folder: ");
            refresh();
            getstr(foldName);
            noecho();
            bool created = std::filesystem::create_directory(currentPath / foldName);
            cursorPosition = 0;
            offset = 0;
        } 
        // Открытие в nano
        else if (ch == 'x' || ch == 'X') {
            if (!filename.empty() && cursorPosition < filename.size() && !isDir[cursorPosition]) {
                def_prog_mode();
                endwin();

                std::string fullPath = currentPath / filename[cursorPosition];
                system(("nano " + fullPath).c_str());

                reset_prog_mode();
                refresh();
                clear();
            }
        }
        else if (ch == '.' || ch == 'r' || ch == 'R') {
            showHiden = !showHiden;
            cursorPosition = 0;
            offset = 0;
        }
    }
    endwin();
    return 0;
}
