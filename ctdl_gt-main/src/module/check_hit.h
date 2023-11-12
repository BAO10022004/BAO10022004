#include <iostream>
#include <vector>
using namespace std;

void sortCard(vector<Card> selectedCards)
{
	int i, key, j;
	for (i = 1; i < selectedCards.size(); i++)
	{
		key = selectedCards[i].getValue();
		j = i - 1;
		while (j >= 0 && selectedCards[j].getValue() > key)
		{
			selectedCards[j + 1].setValue(selectedCards[j].getValue());
			j = j - 1;
		}
		selectedCards[j + 1].setValue(key);
	}
}
bool checkStraight(vector<Card> selectedCards);
bool checkConsecutivePairs(vector<Card> selectedCards)
{
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (selectedCards[i].getValue() == 15)
			return 0;
	}
	if (selectedCards.size() % 2 == 1)
		return 0;
	vector<Card> straight_1;
	vector<Card> straight_2;
	int number_1 = 0;
	int number_2 = 0;
	for (int i = 0; i < selectedCards.size(); i += 2)
	{
		straight_1.push_back(selectedCards[i]);
	}
	for (int i = 1; i < selectedCards.size(); i += 2)
	{
		straight_2.push_back(selectedCards[i]);
	}
	if (!checkStraight(straight_1))
		return 0;
	if (!checkStraight(straight_2))
		return 0;
	for (int i = 0; i < number_1; i++)
	{
		if (straight_1[i].getValue() != straight_2[i].getValue())
			return 0;
	}
	return 1;
}
bool checkStraight(vector<Card> selectedCards)
{
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (selectedCards[i].getValue() == 15)
			return 0;
	}
	sortCard(selectedCards);
	for (int i = 0; i < selectedCards.size() - 1; i++)
	{ // 3 4 5
		if (selectedCards[selectedCards.size() - 1].getValue() != selectedCards[i].getValue() + selectedCards.size() - i - 1)
		{
			return 0;
		}
	}
	return 1;
}
bool checkPairs(vector<Card> selectedCards)
{
	for (int i = 1; i < selectedCards.size(); i++)
		if (selectedCards[0].getValue() != selectedCards[i].getValue())
			return 0;
	return 1;
}
bool check(vector<Card> selectedCards)
{
	if (selectedCards.size() == 1)
		return 1;
	if (selectedCards.size() >= 6)
		if (checkConsecutivePairs(selectedCards) == 1)
			return 1;
	if (selectedCards.size() > 2)
		if (checkStraight(selectedCards))
			return 1;
	if (selectedCards.size() >= 2)
		if (checkPairs(selectedCards))
			return 1;
	return 0;
}
int findValueMax(vector<Card> selectedCards)
{
	int maxValue = selectedCards[0].getValue();
	for (int i = 1; i < selectedCards.size(); i++)
	{
		if (maxValue < selectedCards[i].getValue())
			maxValue = selectedCards[i].getValue();
	}
	return maxValue;
}

int findSuitsMax(vector<Card> selectedCards)
{
	int index;
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (findValueMax(selectedCards) == selectedCards[i].getValue())
		{
			index = i;
			break;
		}
	}
	int maxSuits = selectedCards[index].getSuits();
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (findValueMax(selectedCards) == selectedCards[i].getValue() && selectedCards[i].getSuits() < maxSuits)
			maxSuits = selectedCards[i].getSuits();
	}
	return maxSuits;
}
Card findMax(vector<Card> selectedCards)
{
	Card Cardreturn;
	Cardreturn.setSuits(findSuitsMax(selectedCards));
	Cardreturn.setValue(findValueMax(selectedCards));

	return Cardreturn;
}
bool check_hit(vector<Card> history, vector<Card> users)
{
	// check tu quy
	if (history.size() != users.size())
	{
		if (history.size() == 1 && history[0].getValue() == 15)
		{
			if (users.size() == 4 && checkPairs(users) == 1)
				return 1;
			if (users.size() >= 6 && checkConsecutivePairs(users) == 1)
				return 1;
		}
	}
	if ((history.size() >= 6 && users.size() == 4) && checkConsecutivePairs(history) == 1 && checkPairs(history) == 1)
		return 1;
	if ((history.size() < users.size()) && checkConsecutivePairs(history) == 1 && checkConsecutivePairs(users) == 1)
		return 1;
	if (history.size() == users.size())
	{
		// doi & tam & tu
		if (history.size() >= 2 || history.size() <= 4)
		{
			if (users.size() == history.size() && checkPairs(users) == 1 && checkPairs(history) == 1)
			{
				if (findValueMax(history) < findValueMax(users))
				{
					return 1;
				}
				else if (findValueMax(history) == findValueMax(users))

				{
					if (findSuitsMax(history) > findSuitsMax(users))
					{
						return 1;
					}
				}
			}
		}
		// sanh
		if (history.size() >= 3)
		{
			if (checkStraight(history) == 1 && checkStraight(users) == 1)
			{
				if (findValueMax(users) > findValueMax(history))
					return 1;
				else if (findValueMax(users) == findValueMax(history))
				{
					if (findSuitsMax(history) > findSuitsMax(users))
						return 1;
				}
			}
		}
		if (history.size() == 1)
		{
			if (history[0].getValue() < users[0].getValue())
				return 1;
			else if (history[0].getValue() == users[0].getValue())
			{
				if (history[0].getSuits() > users[0].getSuits())
					return 1;
			}
		}
	}
	return 0;
}