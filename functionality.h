#pragma once

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */


void fallingPiece(float& timer, float& delay, int &color) {
    if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1] += 1;                   //How much units downward
        }
        if (!anamoly()) {
            int n = rand() % 7;
            color = 1 + (rand() % 7);

                for (int i=0;i<4;i++){
                    point_1[i][0] = BLOCKS[n][i] % 2;
                    point_1[i][1] = BLOCKS[n][i] / 2;
                }

        }
        timer = 0;
    }
    
}

/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///
bool x_motion_bound(const int &x) {
    if (x == 1) {
        for (int i = 0; i < 4; i++) {
            if (point_1[i][0] + 1 >= N || gameGrid[point_1[i][1]][point_1[i][0]+1] || point_1[i][1]==M-1) {
                return false;
            }
        }
    }

    else if (x == -1) {
        for (int i = 0; i < 4; i++) {
            if (point_1[i][0] - 1 < 0 || gameGrid[point_1[i][1]][point_1[i][0] - 1] || point_1[i][1] == M - 1) {
                return false;
            }
        }
    }

    return true;
}

void x_motion(int&x) {
    if (x == 1 && x_motion_bound(x)) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0]++;
        }
    }

    else if (x == -1 && x_motion_bound(x)) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0]--;
        }
    }

    x = 0;
}

bool is_at_bottom() {
    for (int i = 0; i < 4; i++) {
        if (point_1[i][1] >= M - 1 || gameGrid[point_1[i][1]+1][point_1[i][0]]) {
            return true;
        }
    }

    return false;
}

void stop_at_bottom(int color) {
    if (is_at_bottom()) {
        for (int i = 0; i < 4; i++) {
            gameGrid[point_1[i][1]][point_1[i][0]] = color;
        }
    }
}

bool can_rotate() {
    float center_x = point_1[1][0];
    float center_y = point_1[1][1];

    for (int i = 0; i < 4; i++) {
        float relative_x = point_1[i][0] - center_x;
        float relative_y = point_1[i][1] - center_y;

        // Perform rotation around the center point (center_x, center_y)
        // using a 90-degree clockwise rotation matrix
        float new_relative_x = relative_y;
        float new_relative_y = -relative_x;

        // Translate back to the original coordinate system
        int new_x = center_x + new_relative_x;
        int new_y = center_y + new_relative_y;

        // Check if the new position is within the boundaries and unoccupied
        if (new_x < 0 || new_x >= N || new_y < 0 || new_y >= M || gameGrid[new_y][new_x]) {
            return false;
        }
    }

    return true;
}

void rotation(bool &x) {
    if (x && can_rotate()) {
        float center_x = point_1[1][0];
        float center_y = point_1[1][1];

        for (int i = 0; i < 4; i++) {
            float relative_x = point_1[i][0] - center_x;
            float relative_y = point_1[i][1] - center_y;

            // Perform rotation around the center point (center_x, center_y)
            // using a 90-degree clockwise rotation matrix
            float new_relative_x = relative_y;
            float new_relative_y = -relative_x;

            // Translate back to the original coordinate system
            point_1[i][0] = center_x + new_relative_x;
            point_1[i][1] = center_y + new_relative_y;
        }
    }

    x = false;
}

void clearLines() {
    linesCleared = 0;


    for (int row = M - 1; row >= 0; row--) {
        bool lineIsFull = true;

        for (int col = 0; col < N; col++) {
            if (gameGrid[row][col] == 0) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            // Shift all lines above the cleared line downwards
            score += 10;
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < N; c++) {
                    gameGrid[r][c] = gameGrid[r - 1][c];
                }
            }

            // Clear the topmost row
            for (int c = 0; c < N; c++) {
                gameGrid[0][c] = 0;
            }

            linesCleared++;

            // Adjust the current piece's position if it was affected by the line clear
            for (int i = 0; i < 4; i++) {
                if (point_1[i][1] < row) {
                    point_1[i][1]++;
                }
            }


        }
    }

    // Update score or perform other actions based on the number of cleared lines
    // ...

    
}


// Function to display the main menu and handle input
void mainMenu(sf::RenderWindow& window) {
    // Load font for menu text
    sf::Font font;
    font.loadFromFile("img/font.ttf");

    // Create menu text
    sf::Text titleText("Tetris Game", font, 50);
    titleText.setPosition(50, 100);
    titleText.setFillColor(sf::Color::White);

    sf::Text startText("Press SPACE to Start", font, 30);
    startText.setPosition(70, 200);
    startText.setFillColor(sf::Color::White);

    sf::Text exitText("Press ESC to Exit", font, 30);
    exitText.setPosition(90, 250);
    exitText.setFillColor(sf::Color::White);

    // Play background music
    sf::Music music;
    music.openFromFile("sound/Tetris.ogg");
    music.setVolume(50);
    music.setLoop(true); // Loop the music
    music.play();

    // Main menu loop
    bool inMenu = true;
    while (inMenu) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space)
                    inMenu = false; // Exit the menu
                else if (event.key.code == sf::Keyboard::Escape)
                    window.close(); // Exit the game
            }
        }

        // Clear the window and draw the menu text
        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.draw(startText);
        window.draw(exitText);
        window.display();
    }

    // Stop the background music before proceeding to the game
    music.stop();
}

///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////