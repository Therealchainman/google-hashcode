#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <regex>
#include <set>
#include <chrono>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <cassert>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <random>
#include <array>
#include <bitset>
#define rep(i, n) for (i = 0; i < n; ++i)
#define REP(i, k, n) for (i = k; i <= n; ++i)
#define REPR(i, k, n) for (i = k; i >= n; --i)
#define pb push_back
#define all(a) a.begin(), a.end()
#define fastio               \
    ios::sync_with_stdio(0); \
    cin.tie(0);              \
    cout.tie(0)
#define ll long long
#define uint unsigned long long
#define inf 0x3f3f3f3f3f3f3f3f
#define mxl INT64_MAX
#define mnl INT64_MIN
#define mx INT32_MAX
#define mn INT32_MIN
#define endl '\n'
using namespace std;
using namespace std::chrono;

ll mod(ll a, ll b)
{
    return (a % b + b) % b;
}

// This stores the number of teams, teams_array= [2,3,5], at i=0 is the number of teams of size 2, i=1 is the number of teams of size 3,
// And i=2 is the number of teams of size 4.
struct pizzeria_data
{
    vector<ll> teams_array;
    // TODO:  Add the comparator for the heuristic, it should sort based on lowest score, than based on larger number of pizzas for that entry.
    // This is the priority queue for the pizzas.
    priority_queue<heuristic, custom_comparator> pq_pizzas;
    vector<vector<string>> pizzas_available;
    ll num_pizzas;

    explicit pizzeria_data()
    {
        teams_array.clear();
        pizzas_available.clear();
    }

    // This method will do it all, it will fill in the priority queue for the pizzas.
    void fill()
    {

        for (int i = 0; i < num_pizzas; i++)
        {
            for (int j = i + 1; j < num_pizzas; j++)
            {
                for (int k = j + 1; k < num_pizzas; k++)
                {
                    for (int w = k + 1; w < num_pizzas; w++)
                    {
                        unordered_set<string> seen_ingred;
                        ll overlap = 0;
                        overlap += build_heuristic(seen_ingred, pizzas_available[i]);
                        overlap += build_heuristic(seen_ingred, pizzas_available[j]);
                        overlap += build_heuristic(seen_ingred, pizzas_available[k]);
                        overlap += build_heuristic(seen_ingred, pizzas_available[w]);
                        heuristic cur_heuristic;
                        cur_heuristic.score = overlap;
                        cur_heuristic.pizzas = {i, j, k, w};
                        cur_heuristic.num_pizzas = 4;
                        pq_pizzas.push(cur_heuristic);
                    }
                }
            }
        }
        for (int i = 0; i < num_pizzas; i++)
        {
            for (int j = i + 1; j < num_pizzas; j++)
            {
                for (int k = j + 1; k < num_pizzas; k++)
                {
                    unordered_set<string> seen_ingred;
                    ll overlap = 0;
                    overlap += build_heuristic(seen_ingred, pizzas_available[i]);
                    overlap += build_heuristic(seen_ingred, pizzas_available[j]);
                    overlap += build_heuristic(seen_ingred, pizzas_available[k]);
                    heuristic cur_heuristic;
                    cur_heuristic.score = overlap;
                    cur_heuristic.pizzas = {i, j, k};
                    cur_heuristic.num_pizzas = 3;
                    pq_pizzas.push(cur_heuristic);
                }
            }
        }
        for (int i = 0; i < num_pizzas; i++)
        {
            for (int j = i + 1; j < num_pizzas; j++)
            {
                unordered_set<string> seen_ingred;
                ll overlap = 0;
                overlap += build_heuristic(seen_ingred, pizzas_available[i]);
                overlap += build_heuristic(seen_ingred, pizzas_available[j]);
                heuristic cur_heuristic;
                cur_heuristic.score = overlap;
                cur_heuristic.pizzas = {i, j};
                cur_heuristic.num_pizzas = 2;
                pq_pizzas.push(cur_heuristic);
            }
        }
    }

    ll build_heuristic(unordered_set<string> &seen_ingredients, vector<string> ingredients)
    {
        ll overlap = 0;
        for (string ingred : ingredients)
        {
            if (seen_ingredients.count(ingred) > 0)
            {
                overlap++;
            }
            else
            {
                seen_ingredients.insert(ingred);
            }
        }
        return overlap;
    }

    void optimize()
    {
        heuristic cur_best;
        unordered_set<ll> delivered_pizzas;
        ll total_score = 0, num_different_pizzas;
        while (!pq_pizzas.empty())
        {
            cur_best = pq_pizzas.top();
            pq_pizzas.pop();

            if (check(delivered_pizzas, cur_best.pizzas))
            {
                num_different_pizzas = cur_best.pizzas.size();
                total_score += (num_different_pizzas * num_different_pizzas);
                unordered_set<ll>::iterator it;
                for (it = begin(cur_best.pizzas); it != end(cur_best.pizzas); it++)
                {
                    delivered_pizzas.insert(*it);
                }
                num_pizzas -= cur_best.num_pizzas;
                // This is just to update the teams array.
                if (cur_best.num_pizzas == 2)
                {
                    teams_array[0]--;
                }
                else if (cur_best.num_pizzas == 3)
                {
                    teams_array[1]--;
                }
                else
                {
                    teams_array[2]--;
                }
            }
            if (!check_is_enough_teams_and_pizzas())
            {
                break;
            }
        }
    }

    bool check_is_enough_teams_and_pizzas()
    {
        if (num_pizzas < 2)
        {
            return false;
        }
        else if (num_pizzas == 2 && teams_array[0] == 0)
        {
            return false;
        }
        else if (num_pizzas == 3 && teams_array[0] == 0 && teams_array[1] == 0)
        {
            return false;
        }
        else if (num_pizzas > 3 && teams_array[0] == 0 && teams_array[1] == 0 && teams_array[2] == 0)
        {
            return false;
        }
        return true;
    }

    // Check that you have not delivered this pizza already.
    bool check(unordered_set<ll> delivered_pizzas, unordered_set<ll> cur_pizzas)
    {
        unordered_set<ll>::iterator it;
        for (it = begin(cur_pizzas); it != end(cur_pizzas); it++)
        {
            ll cur_pizza = *it;
            if (delivered_pizzas.count(cur_pizza) > 0)
            {
                return false;
            }
        }
        return true;
    }
};

// The score is the overlap,
struct heuristic
{
    ll score, num_pizzas;
    unordered_set<ll> pizzas;
};

int main()
{
    auto start = high_resolution_clock::now();
    freopen("inputs/a_example", "r", stdin);
    ll num_pizzas, T2, T3, T4, num_ingredients;
    string ingredient;
    cin >> num_pizzas >> T2 >> T3 >> T4;
    //TODO, check if you need to clear the priority queue here.
    pizzeria_data pizzeria;
    pizzeria.teams_array = {T2, T3, T4};
    cout << "Check on teams_array" << endl;
    for (int i = 0; i < pizzeria.teams_array.size(); i++)
    {
        cout << "team-size:" << pizzeria.teams_array[i] << endl;
    }
    pizzeria.pizzas_available = vector<vector<string>>(num_pizzas);
    cout << "size of the pizzas available" << pizzeria.pizzas_available.size() << endl;
    for (int i = 0; i < num_pizzas; i++)
    {
        vector<ll> vec;
        cin >> num_ingredients;
        while (num_ingredients > 0)
        {
            cin >> ingredient;
            pizzeria.pizzas_available[i].push_back(ingredient);
            num_ingredients--;
        }
        cout << "current pizza index:" << i << endl;
        for (string ingred : pizzeria.pizzas_available[i])
        {
            cout << "ingreds:" << ingred << endl;
        }
    }
    pizzeria.fill();
    // TODO: Write the code that will pop from the priority queue and give the heuristically potential best result.
    pizzeria.optimize();
    return 0;
}