#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;
using namespace sf;

int main()
{
    int size = 20;               // Each snake or food block is 20x20 pixels
    float delay;                 // Movement delay, varies with difficulty
    bool isPaused = false;       // Controls whether the game is paused
    bool isGameOver = false;     // Flag to check if the game has ended

    std::cout << "Hey this is my new Snake Game Enjoy..." << endl;

    RenderWindow window(VideoMode(800, 600), "snake game");  // Create the game window

    RectangleShape snakeHead(Vector2f(size, size));          // Snake's head block
    snakeHead.setPosition(100.f, 100.f);                     // Initial position
    snakeHead.setFillColor(Color(0, 100, 0));                // Dark green color

    // User chooses difficulty level which sets the snake speed
choose:
    int n;
    cout << "Choose the difficulty of the game :-\n1. Easy.\n2. Medium\n3. Hard" << endl;
    cout << "select : " << endl;
    cin >> n;
    if (n == 1)
        delay = 0.25f;
    else if (n == 2)
        delay = 0.16f;
    else if (n == 3)
        delay = 0.1f;
    else
    {
        cout << "CHOOSE PROPERLY(1,2,3)\n\n\n" << endl;
        goto choose;
    }

    vector<RectangleShape> snakeBody; // This holds all parts of the snake's growing body

    RectangleShape food(Vector2f(20, 20)); // The red food block
    food.setFillColor(Color::Red);
    food.setPosition(300, 300);           // Initial food position

    Vector2i direction(0, 0);             // Current movement direction (x, y)
    Vector2i nextDirection = direction;   // Buffer to prevent reversing instantly
    Clock moveClock;                      // Controls timed movement

    // Load font for in-game text (pause, game over)
    Font font;
    font.loadFromFile("BAUHS93.ttf");

    Text pause("Game Paused", font, 50);      // Pause message
    pause.setFillColor(Color::White);
    pause.setPosition(250, 250);

    Text gameOver("Game Over! Press X to Restart.", font, 40); // Game over message
    gameOver.setFillColor(Color::White);
    gameOver.setPosition(175, 250);

    // Load sound effects for eating and dying
    SoundBuffer eatSoundBuffer, gameOverSoundBuffer;
    eatSoundBuffer.loadFromFile("EatEffect.wav");
    gameOverSoundBuffer.loadFromFile("GameOver.wav");
    Sound eatSound(eatSoundBuffer), gameOverSound(gameOverSoundBuffer);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // Close the game window
            if (event.type == Event::Closed)
                window.close();

            // Toggle pause when ESC is pressed
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                isPaused = !isPaused;

            // Reset the game if 'X' is pressed after game over
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::X && isGameOver)
            {
                snakeHead.setPosition(100, 100);
                food.setPosition(300, 300);
                snakeBody.clear();
                direction = {0, 0};
                nextDirection = direction;
                isPaused = 0;
                isGameOver = 0;
            }

            // Handle direction input only when game is active
            if (!isPaused && !isGameOver)
            {
                if (event.type = Event::KeyPressed) // NOTE: This should be '==' not '=' but left unchanged
                {
                    // Prevent the snake from reversing into itself
                    if (event.key.code == Keyboard::Up && direction.y != 1)
                        nextDirection = {0, -1};
                    else if (event.key.code == Keyboard::Down && direction.y != -1)
                        nextDirection = {0, 1};
                    else if (event.key.code == Keyboard::Left && direction.x != 1)
                        nextDirection = {-1, 0};
                    else if (event.key.code == Keyboard::Right && direction.x != -1)
                        nextDirection = {1, 0};
                }
            }
        }

        // Main game logic only runs if not paused or game over
        if (!isPaused && !isGameOver)
        {
            // Move snake on clock-based delay
            if (moveClock.getElapsedTime().asSeconds() >= delay)
            {
                direction = nextDirection;

                // Save current head position before moving
                Vector2f prev = snakeHead.getPosition();

                // Move the head based on direction
                snakeHead.move(direction.x * size, direction.y * size);

                // Move each body part to the position of the part in front of it
                for (int i = snakeBody.size() - 1; i > 0; i--)
                {
                    snakeBody[i].setPosition(snakeBody[i - 1].getPosition());
                }
                if (!snakeBody.empty())
                    snakeBody[0].setPosition(prev);

                // Check collision with food
                if (snakeHead.getGlobalBounds().intersects(food.getGlobalBounds()))
                {
                    // Add a new body segment
                    RectangleShape newPart(Vector2f(size, size));
                    newPart.setFillColor(Color(0, 100, 0));
                    newPart.setPosition(prev);
                    snakeBody.push_back(newPart);
                    eatSound.play();

                    // Generate new food position within bounds
                    int rows = window.getSize().y / size;
                    int cols = window.getSize().x / size;
                    food.setPosition((rand() % cols) * size, (rand() % rows) * size);
                }

                // Collision with walls → game over
                Vector2f pos = snakeHead.getPosition();
                if (pos.x >= window.getSize().x || pos.y >= window.getSize().y || pos.x < 0 || pos.y < 0)
                {
                    isPaused = true;
                    isGameOver = true;
                    gameOverSound.play();
                }

                // Collision with itself → game over
                for (auto &x : snakeBody)
                {
                    if (snakeHead.getPosition() == x.getPosition())
                    {
                        isPaused = true;
                        isGameOver = true;
                        gameOverSound.play();
                        break;
                    }
                }
                moveClock.restart(); // Reset the movement timer
            }
        }

        // Start rendering visuals
        window.clear(Color(0, 255, 0, 128)); // Light green background

        // Display pause or game over messages
        if (isPaused && !isGameOver)
            window.draw(pause);

        if (isGameOver && isPaused)
            window.draw(gameOver);

        // Draw food, snake body, and head
        window.draw(food);
        for (auto &x : snakeBody)
            window.draw(x);
        window.draw(snakeHead);

        window.display(); // Push everything to screen
    }

    std::cout << "Hope you enjoyed this game..." << endl;
    return 0;
}
