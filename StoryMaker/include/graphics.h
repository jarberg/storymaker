#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
std::pair<int, int> getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return { width, height };
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
std::pair<int, int> getConsoleSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return { w.ws_col, w.ws_row };
}
#endif



// Shared atomic to signal thread shutdown
std::atomic<bool> keepRunning(true);

void watchConsoleResize(std::function<void(std::pair<int, int>)> onResize) {  
   std::pair<int, int> lastSize = getConsoleSize();  

   while (keepRunning) {  
       std::pair<int, int> newSize = getConsoleSize();  

       if (newSize != lastSize) {  
           lastSize = newSize;  
           onResize(newSize);  // Callback with new size  
       }  

       std::this_thread::sleep_for(std::chrono::milliseconds(200));  
   }  
}

std::string repeat(char ch, int count) {
    return std::string((count > 0) ? count : 0, ch);
}

void renderDoorInFrame(int canvasWidth, int canvasHeight, int doorWidth, int doorHeight, int paddingTop, int paddingSide) {
    // Bounds check
    if (doorWidth + paddingSide * 2 > canvasWidth || doorHeight + paddingTop * 2 > canvasHeight) {
        std::cerr << "Error: Door and padding exceed canvas size.\n";
        return;
    }

    int doorX = (canvasWidth - doorWidth) / 2;   // starting column of door
    int doorY = (canvasHeight - doorHeight) / 2; // starting row of door

    for (int row = 0; row < canvasHeight; ++row) {
        for (int col = 0; col < canvasWidth; ++col) {
            bool isTop = (row == doorY);
            bool isBottom = (row == doorY + doorHeight - 1);
            bool isSide = (col == doorX || col == doorX + doorWidth - 1);
            bool isInside = (row > doorY && row < doorY + doorHeight - 1 &&
                col > doorX && col < doorX + doorWidth - 1);

            // Draw corners and edges
            if (row >= doorY && row < doorY + doorHeight &&
                col >= doorX && col < doorX + doorWidth) {
                if ((isTop || isBottom) && (col == doorX || col == doorX + doorWidth - 1)) {
                    std::cout << "+"; // corners
                }
                else if (isTop || isBottom) {
                    std::cout << "-";
                }
                else if (isSide) {
                    std::cout << "|";
                }
                else if (row == doorY + doorHeight / 2 && col == doorX + doorWidth - 3) {
                    std::cout << "o"; // doorknob
                }
                else {
                    std::cout << " ";
                }
            }
            else {
                std::cout << " "; // padding area
            }
        }
        std::cout << "\n";
    }
}