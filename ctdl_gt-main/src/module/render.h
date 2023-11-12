#pragma once
#include "score.h"

void renderHitBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {800, 440, 200, 100};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderSkipBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {210, 440, 200, 100};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);
}
void renderAgainBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {530, 320, 200, 100};
    SDL_RenderCopy(gRenderer, againBtnTexture, NULL, &destinationRect);
}

// history area
void renderHistory(vector<vector<Card>> history)
{
    int padding = 0;
    int len = history.size();
    int numberOfCards = 0;
    for (int i = 0; i < len; i++)
    {
        padding += 70;
        numberOfCards += history[i].size();
        for (Card card : history[i])
        {
            card.setWidth(96);
            card.setHeight(139);

            if (i != len - 1)
                card.blackColor();
            else
                card.normalColor();

            if (numberOfCards >= 13)
            {
                padding = 80;
                card.setX((SCREEN_WIDTH / 6) + padding);
            }
            else
            {
                card.setX((SCREEN_WIDTH / 6) + padding);
            }
            card.setY(250);

            SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());

            padding += 40;
        }
    }
}

void renderPassWin(SDL_Texture *winTexture, int padding = 0)
{
    SDL_Delay(1000);
    SDL_Rect destinationRect;
    destinationRect = {SCREEN_WIDTH / 2 - 280 + (210 * (padding + 1)), SCREEN_HEIGHT / 2 - 280, 200, 200};
    SDL_RenderCopy(gRenderer, winTexture, NULL, &destinationRect);
}

void renderAnimationHit(vector<vector<Card>> history, User &player, vector<Computer> &computers)
{
    int frame = 20;
    int padding = 0;
    vector<int> tempX(history.size());

    for (int i = 0; i < history.size(); i++)
    {
        padding += 50;
        tempX[i] = (SCREEN_WIDTH / 4) + padding;
    }

    for (int k = 1; k <= frame; k++)
    {
        for (int i = 0; i < history.size(); i++)
        {
            if (i >= history.size() - 1)
            {
                for (Card &card : history[i])
                {
                    card.setWidth(card.getWidth() - 2);
                    card.setHeight(card.getHeight() - 3);
                    card.setX(card.getX() + ((tempX[i] - card.getX()) / 2));
                    card.setY(card.getY() - 14);
                    for (Card &card : history[i])
                    {
                        SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());
                    }

                    SDL_RenderPresent(gRenderer);
                    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
                    printResult(player, computers);
                    for (Computer &computer : computers)
                    {
                        computer.printBackCard();
                    }
                    player.printCards();
                    card.setX(card.getX() + 40);
                }
            }
        }
        SDL_Delay(10);
    }
}

void renderAnimationHitComputer(vector<vector<Card>> history, User &player, vector<Computer> &computers, int &id)
{
    int frame = 20;
    vector<int> tempX(history.size());
    int padding = 0;
    int x = 0;
    int y = 0;

    if (id == 0)
    {
        x = 50;
        y = SCREEN_HEIGHT / 2 - 145;
    }
    else if (id == 1)
    {
        x = SCREEN_WIDTH / 2 - 100;
        y = 5;
    }
    else
    {
        x = SCREEN_WIDTH - 200;
        y = SCREEN_HEIGHT / 2 - 145;
    }

    for (int i = 0; i < history.size(); i++)
    {
        padding += 50;
        tempX[i] = (SCREEN_WIDTH / 4) + padding;
    }

    for (int k = 1; k <= frame; k++)
    {
        for (int i = 0; i < history.size(); i++)
        {
            if (i >= history.size() - 1)
            {
                for (Card &card : history[i])
                {
                    if (k == 1)
                    {
                        card.setX(x);
                        card.setY(y);
                    }
                    card.setHeight(115);
                    card.setWidth(80);

                    if (id == 0)
                    {
                        card.setX(card.getX() + ((tempX[i] - card.getX()) / 2));
                        card.setY(card.getY() + 2);
                    }
                    else if (id == 1)
                    {
                        card.setX(card.getX() - ((card.getX() - tempX[i]) / 2));
                        card.setY(card.getY() + 12);
                    }
                    else
                    {
                        card.setX(card.getX() - ((card.getX() - tempX[i]) / 2));
                        card.setY(card.getY() + 2);
                    }

                    for (Card &card : history[i])
                    {
                        SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());
                    }

                    SDL_RenderPresent(gRenderer);
                    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
                    printResult(player, computers);
                    for (Computer &computer : computers)
                    {
                        computer.printBackCard();
                    }
                    player.printCards();
                    card.setX(card.getX() + 40);
                }
            }
        }
        SDL_Delay(10);
    }
}