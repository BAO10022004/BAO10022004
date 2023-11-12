#include "score.h"

SDL_Rect skipBtnArea = {210, 440, 200, 100};
SDL_Rect hitBtnArea = {800, 440, 200, 100};
SDL_Rect againBtnArea = {470, 300, 200, 100};

void skipBtnEvent(User &player, vector<Computer> &computers)
{
    if (!player.getIsFirst())
    {
        player.setUserTurn(false);
        player.printSkipText();
        player.setSkip(true);
    }
}

void hitBtnEvent(User &player, vector<Computer> &computers)
{
    if (player.selectedCardsLen())
    {

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
        printResult(player, computers);
        player.hit();
        renderAnimationHit(history, player, computers);
        if (player.checkWin())
            player.setPlace();

        if (!player.getIsFinish())
        {
            renderHitBtn();
            if (!player.getIsFirst())
            {
                renderSkipBtn();
            }
            player.printCards();
        }
        else
        {
            player.printWinner();
        }

        renderHistory(history);

        if (player.getSkip())
        {
            player.printSkipText();
        }
        // print skip text and back cards
        for (Computer computer : computers)
        {
            if (!computer.getIsFinish())
            {
                computer.printBackCard();
            }
            else
            {
                computer.printWinner(computer.getId());
            }

            if (computer.getSkip())
            {
                computer.printSkipText(computer.getId());
            }
        }
    }
}

// print others stuff
void renderSelectEvent(User player, vector<Computer> computers)
{

    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
    printResult(player, computers);
    if (!player.getIsFirst())
    {
        renderSkipBtn();
    }
    renderHitBtn();
    renderHistory(history);

    player.printCards();

    if (player.getSkip())
    {
        player.printSkipText();
    }

    for (Computer computer : computers)
    {
        if (!computer.getIsFinish())
        {
            computer.printBackCard();
        }
        else
        {
            computer.printWinner(computer.getId());
        }

        if (computer.getSkip())
        {
            computer.printSkipText(computer.getId());
        }
    }
}

void cardSelectEvent(User &player, vector<Computer> &computers, int mouseX, int mouseY)
{
    // user cards area
    int cardCout = player.getCardCount();
    SDL_Rect cardArea = {50, 560, 80 * cardCout + 40, 174};
    if (mouseX >= cardArea.x && mouseX <= cardArea.x + cardArea.w &&
        mouseY >= cardArea.y && mouseY <= cardArea.y + cardArea.h)
    {
        // index of the card user clicked
        int index;

        // calculate the index user clicked
        int minValue = 50, maxValue;
        for (int i = 0; i < cardCout; i++)
        {
            maxValue = minValue + 80;

            if (minValue <= mouseX && maxValue >= mouseX)
            {
                index = i;
                break;
            }

            if (i == cardCout - 1)
            {
                index = cardCout - 1;
                break;
            }

            minValue = maxValue;
        }

        player.changeSelected(index);
        renderSelectEvent(player, computers);
    }
}