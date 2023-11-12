#pragma once
#include <bits/stdc++.h>
#include <algorithm>
#include "card.h"
#include "user.h"
#include "algorithms.h"
using namespace std;

vector<int> cards_will_Hit_1(vector<Card> computer, vector<Card> history)
{
    vector<int> temp1;
    int temp = computer.size();
    mergeSort(computer, 0, temp - 1);
    map<int, int> mp, mp1;
    for (auto it : computer)
    {
        mp[it.getValue()]++;
    }
    int index = 0; // Biến này dùng để lấy giá trị của 3 lá cùng giá trị hoặc 4 lá cùng giá trị
    for (auto it : history)
    {
        index = it.getValue();
        mp1[it.getValue()]++;
    }
    if (mp1[15] == 1 || mp1[15] == 2)
    {
        for (int i = 0; i < computer.size(); i++)
        {
            if (mp[computer[i].getValue()] == 4)
            {
                temp1.push_back(i);
                temp1.push_back(i + 1);
                temp1.push_back(i + 2);
                temp1.push_back(i + 3);
                return temp1;
            }
        }
    }
    if ((int)history.size() == 1)
    {
        for (int i = 0; i < temp; ++i)
        {
            if (computer[i].getValue() > history[0].getValue())
            {
                temp1.push_back(i);
                return temp1;
            }
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                temp1.push_back(i);
                return temp1;
            }
        }
    }
    else if ((int)history.size() == 2)
    {
        bool ok = true;
        int suit_his1 = history[0].getSuits();
        int suit_his2 = history[1].getSuits();
        for (int i = 0; i < temp; i++)
        {
            if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() == history[0].getValue()))
            {
                if (suit_his1 != 3 && suit_his2 != 3)
                {
                    temp1.push_back(i);
                    temp1.push_back(i + 1);
                    return temp1;
                }
            }
            else if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() > history[0].getValue()))
            {
                temp1.push_back(i);
                temp1.push_back(i + 1);
                return temp1;
            }
        }
    }
    else
    {
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            for (int i = index + 1; i <= 15; i++)
            {
                // Có thể là dấu lớn nma tao tiếc vụ máy có tứ quý mà đánh sáp(3 lá giống nhau)
                if (mp[i] == mp1[index])
                {
                    for (int i = 0; i < (int)computer.size(); i++)
                    {
                        if (computer[i].getValue() == index)
                        {
                            temp1.push_back(i);
                            temp1.push_back(i + 1);
                            temp1.push_back(i + 2);
                            return temp1;
                        }
                    }
                }
            }
        }

        vector<pair<int, int>> v;
        // Xu ly sanh
        v.push_back({computer[0].getValue(), 0});
        for (int i = 1; i < (int)computer.size(); i++)
        {
            if (computer[i].getValue() != computer[i - 1].getValue())
            {
                v.push_back({computer[i].getValue(), i});
            }
        }
        // Dùng sliding window
        int a[16];
        int k = (int)history.size();
        auto it = v.begin();
        for (int i = 0; i < k; i++)
        {
            a[i] = (*it).first;
            temp1.push_back((*it).second);
            it++;
        }
        if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        for (int i = k; i < (int)v.size(); i++)
        {
            temp1.erase(temp1.begin());
            a[i] = (*it).first;
            temp1.push_back((*it).second);
            if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
            {
                return temp1;
            }
            else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
            {
                return temp1;
            }
            it++;
        }
        return temp1;
    }
}

bool checkComputerCanHit(vector<Card> computer, vector<Card> history, Computer com)
{
    // Đánh dấu lá bài của máy với lá bài trên bàn
    map<int, int> mp, mp1;
    // Số lá bài trên tay ít hơn số lá bài trên bàn thì không đánh
    if ((int)computer.size() < (int)history.size())
    {
        cout << "La bai tren tay it hon la bai tren ban " << endl;
        return true;
    }
    // Kiem tra tam cuoi cua la truoc
    Card the_last_history = history[(int)history.size() - 1];
    Card the_first_history = history[0];
    // Nếu lá cuối sảnh là lá át cơ thì không đánh
    if (the_last_history.getValue() == 14 && the_last_history.getSuits() == 3)
    {
        cout << "At co khong danh" << endl;
        return true;
    }
    // Sắp xếp lại lá bài trước khi check
    mergeSort(computer, 0, (int)computer.size() - 1);
    // Xếp bài của máy thành sảnh, couple, single,special
    vector<pair<int, int>> couple, single, quaCards, threePairsOfPine, special; // .first is value of card, .second is index of computer's cards
    set<pair<int, int>> lobby;
    // Đánh dấu lá bài của máy và lá bài lịch sử
    int indexSpecial = 0;
    for (auto it : computer)
    {
        mp[it.getValue()]++;
        indexSpecial++;
        if (mp[it.getValue()] == 4) // Đánh dấu lá bài đặt biệt
        {
            cout << "Co tu quy " << it.getValue() << endl;
            quaCards.push_back({it.getValue(), indexSpecial - 4});
            quaCards.push_back({it.getValue(), indexSpecial - 3});
            quaCards.push_back({it.getValue(), indexSpecial - 2});
            quaCards.push_back({it.getValue(), indexSpecial - 1});
            cout << "Vi tri tu quy:  " << endl;
            for (int i = 0; i < quaCards.size(); i++)
            {
                cout << "Lap lan thu i: " << i << ' ' << quaCards[i].second << endl;
            }
        }
        else if (indexSpecial >= 6 && it.getValue() - 3 >= 3 && mp[it.getValue() - 1] == 2 && mp[it.getValue() - 2] == 2 && mp[it.getValue()] == 2)
        {
            threePairsOfPine.clear();
            threePairsOfPine.push_back({it.getValue() - 2, indexSpecial - 6});
            threePairsOfPine.push_back({it.getValue() - 2, indexSpecial - 5});
            threePairsOfPine.push_back({it.getValue() - 1, indexSpecial - 4});
            threePairsOfPine.push_back({it.getValue() - 1, indexSpecial - 3});
            threePairsOfPine.push_back({it.getValue(), indexSpecial - 2});
            threePairsOfPine.push_back({it.getValue(), indexSpecial - 1});
            cout << "3 doi thong: gia tri va vi tri " << endl;
            for (int i = 0; i < threePairsOfPine.size(); i++)
            {
                cout << threePairsOfPine[i].first << ' ' << threePairsOfPine[i].second << endl;
            }
        }
    }
    int index = 0;
    for (auto it : history)
    {
        index = it.getValue();
        mp1[it.getValue()]++;
    }
    // Sang sanh ra
    vector<pair<int, int>> v;
    // Xu ly sanh tim kiem phan tu doc nhat
    v.push_back({computer[0].getValue(), 0});
    for (int i = 1; i < (int)computer.size(); i++)
    {
        if (computer[i].getValue() != computer[i - 1].getValue())
        {
            v.push_back({computer[i].getValue(), i});
        }
    }
    // Neu co 1 tu quy thi xet
    if (quaCards.size() == 4)
    {
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i].first == quaCards.front().first && v[i].second == quaCards.front().second)
            {
                v.erase(v.begin() + i);
            }
        }
    }
    else if (quaCards.size() > 4)
    {
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i].first == quaCards.front().first && v[i].second == quaCards.front().second || v[i].second == quaCards[4].second && v[i].second == quaCards[4].second)
            {
                cout << "Co 2 tu quy tro len" << endl;
                v.erase(v.begin() + i);
            }
        }
    }
    // Xoa phan tu 3 doi thong
    if (threePairsOfPine.size() == 6)
    {
        cout << "Xoa phan tu 3 doi thong cua vector v" << endl;
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i].second == threePairsOfPine[0].second || v[i].second == threePairsOfPine[2].second || v[i].second == threePairsOfPine[4].second)
            {
                v.erase(v.begin() + i);
            }
        }
    }
    cout << "Vecto v: ";
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i].first << " ";
    }
    cout << endl;
    int count = 0;
    // Dùng sliding window
    int a[16];
    int k = 3;
    auto it = v.begin();
    for (int i = 0; i < k; i++)
    {
        a[i] = (*it).first;
        it++;
    }
    // Sliding window first can hit
    if (a[k - 1] - a[0] + 1 == k)
    {
        cout << "sliding window lan dau" << endl;
        for (int i = 0; i < k; i++)
        {
            lobby.insert({(*(v.begin() + i)).first, (*(v.begin() + i)).second});
        }
        for (auto po : lobby)
        {
            cout << po.first << ' ' << po.second << endl;
        }
    }
    for (int i = k; i < (int)v.size(); i++)
    {
        a[i] = (*it).first;
        if (a[i] == 15)
        {
            break;
        }
        if (a[i] - a[i - k + 1] + 1 == k)
        {
            cout << "Hello " << i << ' ' << i - k + 1 << endl;
            for (int p = i - k + 1; p <= i; p++)
            {
                lobby.insert({(*(v.begin() + p)).first, (*(v.begin() + p)).second});
            }
        }
        it++;
    }
    cout << "Sanh Sau khi sang la " << endl;
    if (lobby.size() == 0)
    {
        cout << "Khong co sanh" << endl;
    }
    else
    {
        cout << "Gia tri va vi tri cua lobby " << endl;
        for (pair<int, int> x : lobby)
        {
            mp[x.first]--; // Danh dau lai bai
            cout << x.first << " " << x.second << endl;
        }
        cout << endl;
    }
    // Single + Couple + special
    for (int i = 0; i < (int)computer.size(); i++)
    {
        if (lobby.find({computer[i].getValue(), i}) == lobby.end()) // Khong tim thay
        {
            if (mp[computer[i].getValue()] == 1)
            {
                single.push_back({computer[i].getValue(), i});
            }
            else if (mp[computer[i].getValue()] == 2)
            {
                if (threePairsOfPine.size() == 0)
                {
                    couple.push_back({computer[i].getValue(), i});
                }
                else
                {
                    bool checkPair = true;
                    for (int p = 0; p < threePairsOfPine.size(); p++)
                    {
                        if (computer[i].getValue() == threePairsOfPine[p].first && i == threePairsOfPine[p].second)
                        {
                            checkPair = false;
                            break;
                        }
                    }
                    if (checkPair)
                    {
                        couple.push_back({computer[i].getValue(), i});
                    }
                }
            }
            else if (mp[computer[i].getValue()] == 3)
            {
                // Kiem tra thử xem trong 3 đôi thông có không
                if (threePairsOfPine.size() == 0)
                {
                    special.push_back({computer[i].getValue(), i});
                }
                else
                {
                    bool checkPair2 = true;
                    for (int p = 0; p < threePairsOfPine.size(); p++)
                    {
                        if (computer[i].getValue() == threePairsOfPine[p].first && i == threePairsOfPine[p].second)
                        {
                            checkPair2 = false;
                            break;
                        }
                    }
                    if (checkPair2)
                    {
                        special.push_back({computer[i].getValue(), i});
                    }
                    else
                    {
                        single.push_back({computer[i].getValue(), i});
                    }
                }
            }
        }
    }
    cout << "Tong so la bai sau khi sang: " << special.size() + couple.size() + single.size() + lobby.size() + threePairsOfPine.size() + quaCards.size() << endl;
    if (special.size() != 0)
    {
        cout << "Gia tri va vi tri cua spcecial" << endl;
        for (int i = 0; i < special.size(); i++)
        {
            cout << special[i].first << ' ' << special[i].second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai dat biet " << endl;
    }

    if (couple.size() != 0)
    {
        cout << "Gia tri va vi tri cua couple" << endl;
        for (int i = 0; i < couple.size(); i++)
        {
            cout << couple[i].first << ' ' << couple[i].second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai couple " << endl;
    }

    if (single.size() != 0)
    {
        cout << "Gia tri va vi tri cua single" << endl;
        for (int i = 0; i < single.size(); i++)
        {
            cout << single[i].first << ' ' << single[i].second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai single " << endl;
    }
    if (lobby.size() != 0)
    {
        cout << "Gia tri va vi tri cua lobby" << endl;
        for (auto it : lobby)
        {
            cout << it.first << ' ' << it.second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai lobby " << endl;
    }

    if (threePairsOfPine.size() != 0)
    {
        cout << "Gia tri va vi tri cua threePairsOfPine" << endl;
        for (int i = 0; i < threePairsOfPine.size(); i++)
        {
            cout << threePairsOfPine[i].first << ' ' << threePairsOfPine[i].second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai threePairsOfPine " << endl;
    }

    if (quaCards.size() != 0)
    {
        cout << "Gia tri va vi tri cua quaCards" << endl;
        for (int i = 0; i < quaCards.size(); i++)
        {
            cout << quaCards[i].first << ' ' << quaCards[i].second << endl;
        }
    }
    else
    {
        cout << "Khong co la bai quaCards " << endl;
    }
    cout << endl;
    // Case: couple of pairs 2
    if (mp1[15] == 2 && quaCards.size() != 0)
    {
        return false;
    }
    // Case: Card history is 2
    if (mp1[15] == 1 && threePairsOfPine.size() != 0)
    {
        return false;
    }
    // Check computer can hit
    if (history.size() == 1)
    {
        // So sánh giá trị của bài trên bàn với bài trên tay
        for (int i = 0; i < (int)single.size(); ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (single[i].first > history[0].getValue())
            {
                if (single[i].first == 15 && com.getSkipCount() < 3)
                    return true;
                return false;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3)
            else if ((single[i].first == history[0].getValue()) && (computer[single[i].second].getSuits() < history[0].getSuits()))
            {
                if (single[i].first == 15 && com.getSkipCount() < 3)
                    return true;
                return false;
            }
        }
        return true;
    }
    else if (history.size() == 2)
    {
        for (int i = 0; i < (int)couple.size(); ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (couple[i].first > history[0].getValue())
            {
                if (couple[i].first == 15 && com.getSkipCount() < 3)
                    return true;
                return false;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3) computer[couple[i].second].getSuits() <
            else if ((couple[i].first == history[0].getValue()) && (history[1].getSuits() != 3))
            {
                if (couple[i].first == 15 && com.getSkipCount() < 3)
                    return true;
                return false;
            }
        }
        return true;
    }
    else
    {
        // Kiem tra sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            if (special.size() != 0 && mp[special.size() - 1] == mp1[index])
            {
                return false;
            }
            return true;
        }
        int cnt = 1;
        bool check = false;
        int i = 0;
        int valueFirst = 0;
        for (pair<int, int> it : lobby)
        {
            if (i == 0)
            {
                i++;
                valueFirst = it.first;
                continue;
            }
            if (it.first - 1 == valueFirst)
            {
                valueFirst = it.first;
                cnt++;
            }
            else
            {
                valueFirst = it.first;
                if (cnt == (int)history.size())
                {
                    if (computer[it.second - cnt + 1].getValue() > history[0].getValue())
                    {
                        return false;
                    }
                    else if (computer[it.second - cnt + 1].getValue() == history[0].getValue() && computer[it.second].getSuits() < history[history.size() - 1].getSuits())
                    {
                        return false;
                    }
                    cnt = 1;
                }
                else if (cnt < (int)history.size())
                {
                    cnt = 1;
                }
                else if (cnt > (int)history.size())
                {
                    check = true;
                    cnt = 1;
                }
            }
        }
        cout << "Kiem tra xong " << endl;
        if (check)
        {
            return false;
        }
        return true;
    }
}

vector<int> cardsWillHit2(vector<Card> computer, vector<Card> history)
{
    cout << "Day la khuc danh" << endl;
    // Đánh dấu lá bài của máy với lá bài trên bàn
    map<int, int> mp, mp1;
    // Sắp xếp lại lá bài trước khi danh
    // mergeSort(computer, 0, (int)computer.size() - 1);
    // Xếp bài của máy thành sảnh, couple, single,special
    vector<pair<int, int>> couple, single, quaCards, threePairsOfPine, special; // .first is value of card, .second is index of computer's cards
    set<pair<int, int>> lobby;
    // Đánh dấu lá bài của máy và lá bài lịch sử
    int indexSpecial = 0;
    for (auto it : computer)
    {
        mp[it.getValue()]++;
        indexSpecial++;
        if (mp[it.getValue()] == 4) // Đánh dấu lá bài đặt biệt
        {
            quaCards.push_back({it.getValue(), indexSpecial - 3});
            quaCards.push_back({it.getValue(), indexSpecial - 2});
            quaCards.push_back({it.getValue(), indexSpecial - 1});
            quaCards.push_back({it.getValue(), indexSpecial});
        }
        else if (indexSpecial >= 6 && it.getValue() - 3 >= 3 && mp[it.getValue() - 1] == 2 && mp[it.getValue() - 2] == 2 && mp[it.getValue()] == 2)
        {
            threePairsOfPine.clear();
            threePairsOfPine.push_back({it.getValue() - 2, indexSpecial - 6});
            threePairsOfPine.push_back({it.getValue() - 2, indexSpecial - 5});
            threePairsOfPine.push_back({it.getValue() - 1, indexSpecial - 4});
            threePairsOfPine.push_back({it.getValue() - 1, indexSpecial - 3});
            threePairsOfPine.push_back({it.getValue(), indexSpecial - 2});
            threePairsOfPine.push_back({it.getValue(), indexSpecial - 1});
        }
    }
    int index = 0;
    for (auto it : history)
    {
        index = it.getValue();
        mp1[it.getValue()]++;
    }
    // Sang sanh ra
    vector<pair<int, int>> v;
    // Xu ly sanh tim kiem phan tu doc nhat
    v.push_back({computer[0].getValue(), 0});
    for (int i = 1; i < (int)computer.size(); i++)
    {
        if (computer[i].getValue() != computer[i - 1].getValue())
        {
            v.push_back({computer[i].getValue(), i});
        }
    }
    // Neu co 1 tu quy thi xet
    if (quaCards.size() == 4)
    {
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i] == quaCards.front())
            {
                v.erase(v.begin() + i);
            }
        }
    }
    else if (quaCards.size() > 4)
    {
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i].first == quaCards.front().first && v[i].second == quaCards.front().second || v[i].second == quaCards[4].second && v[i].second == quaCards[4].second)
            {
                v.erase(v.begin() + i);
            }
        }
    }
    // Xoa phan tu 3 doi thong
    if (threePairsOfPine.size() == 6)
    {
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (v[i].second == threePairsOfPine[0].second || v[i].second == threePairsOfPine[2].second || v[i].second == threePairsOfPine[4].second)
            {
                v.erase(v.begin() + i);
            }
        }
    }
    cout << "Day la ham danh " << endl;
    // for (int i = 0; i < v.size(); i++)
    // {
    //     cout << v[i].first << " ";
    // }
    int count = 0;
    // Dùng sliding window
    int a[16];
    int k = 3;
    auto it = v.begin();
    for (int i = 0; i < k; i++)
    {
        a[i] = (*it).first;
        it++;
    }
    // Sliding window first can hit
    if (a[k - 1] - a[0] + 1 == k)
    {
        for (int i = 0; i < k; i++)
        {
            lobby.insert({(*(v.begin() + i)).first, (*(v.begin() + i)).second});
        }
    }
    for (int i = k; i < (int)v.size(); i++)
    {
        a[i] = (*it).first;
        if (a[i] == 15)
        {
            break;
        }
        if (a[i] - a[i - k + 1] + 1 == k)
        {
            for (int p = i - k + 1; p <= i; p++)
            {
                lobby.insert({(*(v.begin() + p)).first, (*(v.begin() + p)).second});
            }
        }
        it++;
    }
    if (lobby.size() != 0)
    {
        for (pair<int, int> x : lobby)
        {
            mp[x.first]--; // Danh dau lai bai
        }
    }

    // Single + Couple + special
    for (int i = 0; i < (int)computer.size(); i++)
    {
        if (lobby.find({computer[i].getValue(), i}) == lobby.end()) // Khong tim thay
        {
            if (mp[computer[i].getValue()] == 1)
            {
                single.push_back({computer[i].getValue(), i});
            }
            else if (mp[computer[i].getValue()] == 2)
            {
                // Check 3 doi thong
                bool flag = true;
                for (int l = 0; l < threePairsOfPine.size(); l++)
                {
                    if (threePairsOfPine[l].first == computer[i].getValue() && threePairsOfPine[l].second == i)
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    couple.push_back({computer[i].getValue(), i});
                }
            }
            else if (mp[computer[i].getValue()] == 3)
            {
                bool flag = true;
                for (int l = 0; l < threePairsOfPine.size(); l++)
                {
                    if (computer[threePairsOfPine[l].second].getValue() == computer[i].getValue() && threePairsOfPine[l].second == i)
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    special.push_back({computer[i].getValue(), i});
                }
                else
                {
                    single.push_back({computer[i].getValue(), i});
                }
            }
        }
    }
    // Check computer can hit
    vector<int> ans;
    // Case: couple of pairs 2
    if (mp1[15] == 2 && quaCards.size() != 0)
    {
        for (int i = 0; i < quaCards.size(); i++)
        {
            ans.push_back(quaCards[i].second);
        }
        return ans;
    }
    // Case: Card history is 2
    if (mp1[15] == 1 && threePairsOfPine.size() != 0)
    {
        for (int i = 0; i < threePairsOfPine.size(); i++)
        {
            ans.push_back(threePairsOfPine[i].second);
        }
        return ans;
    }

    if (history.size() == 1)
    {
        // So sánh giá trị của bài trên bàn với bài trên tay
        for (int i = 0; i < (int)single.size(); ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (single[i].first > history[0].getValue())
            {
                ans.push_back(single[i].second);
                return ans;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3)
            else if ((single[i].first == history[0].getValue()) && (computer[single[i].second].getSuits() < history[0].getSuits()))
            {
                ans.push_back(single[i].second);
                return ans;
            }
        }
        cout << "Ham danh single bi loi " << endl;
    }
    else if (history.size() == 2)
    {
        for (int i = 0; i < (int)couple.size() - 1; ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (couple[i].first > history[0].getValue())
            {
                ans.push_back(couple[i].second);
                ans.push_back(couple[i + 1].second);
                return ans;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3)
            else if ((couple[i].first == history[0].getValue()) && (history[0].getSuits() != 3 && history[1].getSuits() != 3))
            {
                ans.push_back(couple[i].second);
                ans.push_back(couple[i + 1].second);
                return ans;
            }
        }
        cout << "Ham danh couple bi loi " << endl;
    }
    else
    {
        // Kiem tra sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            if (special.size() != 0 && mp1[index] == 3 && mp[special[special.size() - 1].first] == mp1[index])
            {
                for (int i = 0; i < special.size(); i++)
                {
                    if (special[i].first > history[0].getValue())
                    {
                        ans.push_back(special[i].second);
                    }
                }
                return ans;
            }
            else if (quaCards.size() != 0 && mp1[index] == 4 && mp[quaCards[quaCards.size() - 1].first] == mp1[index])
            {
                for (int i = 0; i < quaCards.size(); i++)
                {
                    if (quaCards[i].first > history[0].getValue())
                    {
                        ans.push_back(quaCards[i].second);
                    }
                }
                return ans;
            }
            cout << "Ham chat tu quy bi bug " << endl;
        }
        int cnt = 1;
        bool check = false;
        int i = 0;
        int valFirst = 0; // Save the last it.first
        for (pair<int, int> it : lobby)
        {
            if (i == 0)
            {
                i++;
                valFirst = it.first;
                ans.push_back(it.second);
                continue;
            }
            if ((it).first - 1 == valFirst)
            {
                valFirst = it.first;
                cnt++;
                ans.push_back(it.second);
            }
            else
            {
                valFirst = it.first;
                if (cnt == (int)history.size())
                {
                    if (computer[it.second - cnt + 1].getValue() > history[0].getValue())
                    {
                        return ans;
                    }
                    // computer[it.second - cnt + 1].getSuits() < history[0].getSuits()
                    else if (computer[it.second - cnt + 1].getValue() == history[0].getValue() && history[2].getSuits() != 3)
                    {
                        return ans;
                    }
                    ans.clear();
                    ans.push_back(it.second);
                    cnt = 1;
                }
                else if (cnt < (int)history.size())
                {
                    ans.clear();
                    ans.push_back(it.second);
                    cnt = 1;
                }
                else if (cnt > (int)history.size())
                {
                    check = true;
                    ans.clear();
                    ans.push_back(it.second);
                    cnt = 1;
                }
            }
        }
        if (check)
        {
            ans.clear();
            int b[16];
            int k1 = (int)history.size();
            auto it = lobby.begin();
            for (int i = 0; i < k1; i++)
            {
                b[i] = (*it).first;
                ans.push_back((*it).second);
                it++;
            }
            if (b[0] > history[0].getValue() && b[k1 - 1] - a[0] + 1 == k1)
            {
                return ans;
            }
            // computer[(*(it--)).second].getSuits()
            else if (b[0] == history[0].getValue() && history[k1 - 1].getSuits() != 3 && b[k1 - 1] - b[0] + 1 == k1)
            {
                return ans;
            }
            for (int i = k1; i < (int)lobby.size(); i++)
            {
                ans.erase(ans.begin());
                b[i] = (*it).first;
                ans.push_back((*it).second);
                if (b[i - k + 1] > history[0].getValue() && b[i] - b[i - k1 + 1] + 1 == k1)
                {
                    return ans;
                }
                //(computer[(*it).second].getSuits()
                else if ((b[i - k1 + 1] == history[0].getValue()) && history[0].getSuits() != 3 && (b[i] - b[i - k1 + 1] + 1 == k1))
                {
                    return ans;
                }
                it++;
            }
        }
        cout << "Sanh Loi " << endl;
    }
}
// Trả về giá trị true là không đánh, trả về giá trị false là đánh
bool check_computer(vector<Card> computer, vector<Card> history)
{
    // Số lá bài trên tay ít hơn số lá bài trên bàn thì không đánh
    if ((int)computer.size() < (int)history.size())
    {
        return true;
    }
    // Sắp xếp lại lá bài trước khi check
    // mergeSort(computer, 0, (int)computer.size() - 1);

    // Xử lý cóc
    if ((int)history.size() == 1)
    {
        // So sánh giá trị của bài trên bàn với bài trên tay
        for (int i = 0; i < (int)computer.size(); ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3)
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                return false;
            }
        }
        return true;
    }
    // Xu ly doi
    else if ((int)history.size() == 2)
    {
        // Đánh dấu lá bài trên tay
        map<int, int> mp;
        // Biến check có tứ quý không
        bool check_Qua = false;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
            if (mp[it.getValue()] == 4)
            {
                check_Qua = true;
            }
        }
        // Xử lý chặt heo
        if (check_Qua)
        {
            return false;
        }

        // Xử lý đôi có cùng giá trị
        if (mp[history[0].getValue()] == 2)
        {
            if (history[0].getSuits() != 3 && history[1].getSuits() != 3)
            {
                return false;
            }
        }
        for (int i = 0; i < (int)computer.size(); i++)
        {
            if (mp[computer[i].getValue()] == 2 && computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        // Đánh dấu lá bài của máy với lá bài trên bàn
        map<int, int> mp, mp1;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
        }
        int index = 0;
        for (auto it : history)
        {
            index = it.getValue();
            mp1[it.getValue()]++;
        }
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            if (mp[index] == mp1[index])
            {
                return false;
            }
            return true;
        }
        int temp1 = (int)history.size(), temp2 = (int)computer.size();
        // Kiem tra tam cuoi cua la truoc
        Card the_last_history = history[temp1 - 1];
        Card the_first_history = history[0];
        // Nếu lá cuối sảnh là lá át cơ thì không đánh
        if (the_last_history.getValue() == 14 && the_last_history.getSuits() == 3)
        {
            return true;
        }
        else
        {
            vector<pair<int, int>> v;

            // Xu ly sanh
            v.push_back({computer[0].getValue(), 0});
            for (int i = 1; i < temp2; i++)
            {
                if (computer[i].getValue() != computer[i - 1].getValue())
                {
                    v.push_back({computer[i].getValue(), i});
                }
            }
            // Dùng sliding window
            int a[16];
            int k = (int)history.size();
            auto it = v.begin();
            for (int i = 0; i < k; i++)
            {

                a[i] = (*it).first;
                it++;
            }
            if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
            {
                return false;
            }
            else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
            {
                return false;
            }
            for (int i = k; i < (int)v.size(); i++)
            {

                a[i] = (*it).first;
                if (a[i] == 15)
                {
                    return true;
                }
                if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
                {
                    return false;
                }
                else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
                {
                    return false;
                }
                it++;
            }
            return true;
        }
    }
}
void doneTurn(User &player, vector<Computer> &computers)
{
    int count = 0;

    if (player.getSkip())
    {
        count++;
    }
    for (Computer computer : computers)
    {
        if (computer.getSkip())
        {
            count++;
        }
    }

    if (count == 3 - gameResult.size())
    {
        history.clear();
        if (!player.getSkip())
        {
            player.setUserTurn(true);
            player.setIsFirst(true);
        }
        else
        {
            player.setUserTurn(false);
        }

        player.setSkip(false);

        for (Computer &computer : computers)
        {
            if (!computer.getSkip())
            {
                computer.setUserTurn(true);
                computer.setIsFirst(true);
            }
            else
            {
                computer.setUserTurn(false);
            }

            computer.setSkip(false);
        }

        if (player.isUserTurn())
        {
            for (int i = 0; i < COMPUTER_NUM; i++)
            {
                if (!computers[i].getIsFinish())
                {
                    computers[i].setUserTurn(true);
                    break;
                }
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

        renderHistory(history);

        if (!player.getIsFinish())
        {
            renderHitBtn();
            if (!player.getIsFirst())
            {
                renderSkipBtn();
            }

            player.printCards();

            if (player.getSkip())
            {
                player.printSkipText();
            }
        }
        else
        {
            player.printWinner();
        }

        if (player.getSkip())
        {
            player.printSkipText();
        }

        // print skip text and back cards
        for (Computer computer : computers)
        {
            if (computer.getIsFinish())
            {
                computer.printWinner(computer.getId());
            }
            computer.printBackCard();

            if (computer.getSkip())
            {
                computer.printSkipText(computer.getId());
            }
        }

        SDL_Delay(500);
        SDL_RenderPresent(gRenderer);
    }
}