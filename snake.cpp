#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;
using namespace sf;
int main()
{
    int size=20;
    float delay;
    bool isPaused=false;
    bool isGameOver=false;

    std::cout<<"Hey this is my new Snake Game Enjoy..."<<endl;
    RenderWindow window(VideoMode(800,600),"snake game");
    RectangleShape snakeHead(Vector2f(size,size));
    snakeHead.setPosition(100.f,100.f);
    snakeHead.setFillColor(Color(0,100,0));

    choose:
    int n;
    cout<<"Choose the difficulty of the game :-\n1. Easy.\n2. Medium\n3. Hard"<<endl;
    cout<<"select : "<<endl;
    cin>>n;
    if(n==1)
        delay=0.25f;
    else if(n==2)
        delay=0.16f;
    else if(n==3)
        delay=0.1f;
    else
    {
        cout<<"CHOOSE PROPERLY(1,2,3)\n\n\n"<<endl;
        goto choose;
    }
    vector<RectangleShape> snakeBody; //For Dynamic Length Snake Body
    
    RectangleShape food(Vector2f(20,20));
    food.setFillColor(Color::Red);
    food.setPosition(300,300);

    Vector2i direction(0,0);
    Vector2i nextDirection=direction;
    Clock moveClock;

    Font font;
    font.loadFromFile("BAUHS93.ttf");
    Text pause("Game Paused",font,50);
    pause.setFillColor(Color::White);
    pause.setPosition(250,250);

    Text gameOver("Game Over! Press X to Restart.",font,40);
    gameOver.setFillColor(Color::White);
    gameOver.setPosition(175,250);

    //Adding Sound Effect
    SoundBuffer eatSoundBuffer,gameOverSoundBuffer;
    eatSoundBuffer.loadFromFile("EatEffect.wav");
    gameOverSoundBuffer.loadFromFile("GameOver.wav");
    Sound eatSound(eatSoundBuffer),gameOverSound(gameOverSoundBuffer);

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {

            if(event.type==Event::Closed)
                window.close();

            if(event.type==Event::KeyPressed&&event.key.code==Keyboard::Escape)
                isPaused=!isPaused;
            
            //Reastart the game
            if(event.type==Event::KeyPressed&&event.key.code==Keyboard::X&&isGameOver)
            {
                snakeHead.setPosition(100,100);
                food.setPosition(300,300);
                snakeBody.clear();
                direction={0,0};
                nextDirection=direction;
                isPaused=0;
                isGameOver=0;
            }
            
            //navigating the snake
            if(!isPaused&&!isGameOver)
            {   
                if(event.type=Event::KeyPressed)
                {
                    if(event.key.code==Keyboard::Up&&direction.y!=1)
                        nextDirection={0,-1};
                    else if(event.key.code==Keyboard::Down&&direction.y!=-1)
                        nextDirection={0,1};
                    else if(event.key.code==Keyboard::Left&&direction.x!=1)
                        nextDirection={-1,0};
                    else if(event.key.code==Keyboard::Right&&direction.x!=-1)
                        nextDirection={1,0};
                }
            }
        }
    
        if(!isPaused&&!isGameOver)
        {
            if (moveClock.getElapsedTime().asSeconds() >= delay)
            {
                direction = nextDirection;
                
                Vector2f prev=snakeHead.getPosition();//To move the body along with Head

                snakeHead.move(direction.x * size, direction.y * size);
                
                //moving the body of the snake;
                for(int i=snakeBody.size()-1;i>0;i--)
                {
                    snakeBody[i].setPosition(snakeBody[i-1].getPosition());
                }
                if(!snakeBody.empty())
                    snakeBody[0].setPosition(prev);
                
                //when snake eats the apple increase the size
                if(snakeHead.getGlobalBounds().intersects(food.getGlobalBounds()))
                {
                    RectangleShape newPart(Vector2f(size,size));
                    newPart.setFillColor(Color(0,100,0));
                    newPart.setPosition(prev);
                    snakeBody.push_back(newPart);
                    eatSound.play();

                    //generating the new food
                    int rows=window.getSize().y/size;
                    int cols=window.getSize().x/size;
                    food.setPosition((rand()%cols)*size,(rand()%rows)*size);
                }

                Vector2f pos=snakeHead.getPosition();
                if(pos.x>=window.getSize().x||pos.y>=window.getSize().y||pos.x<0||pos.y<0)
                {
                    isPaused=true;
                    isGameOver=true;
                    gameOverSound.play();
                }

                for(auto& x:snakeBody)
                {
                    if(snakeHead.getPosition()==x.getPosition())
                    {
                        isPaused=true;
                        isGameOver=true;
                        gameOverSound.play();
                        break;
                    }
                }
                moveClock.restart();
            }
        }

        //displaying the objects on the window
        window.clear(Color(0,255,0,128));

        if (isPaused&&!isGameOver)
            window.draw(pause);

        if(isGameOver&&isPaused)
            window.draw(gameOver);

        window.draw(food);
        for(auto& x:snakeBody)
            window.draw(x);
        window.draw(snakeHead);
        window.display();
    }
    std::cout<<"Hope you enjoyed this game..."<<endl;
    return 0;
}