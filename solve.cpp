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
    // The vector inside is the vector holding the array of the ingredients for that pizza
    vector<vector<string>> pizzas;
    ll num_pizzas;
    // The number of teams that pizzas have been delivered to
    ll numPizzasDelivered;

    explicit pizzeria_data()
    {
        teams_array.clear();
        pizzas.clear();
        numPizzasDelivered = 0;
    }

    vector<vector<ll>> pizzasDelivered()
    {
        vector<vector<ll>> res;
        ll i = 0;
        while (teams_array[0] > 0 || teams_array[1] > 0 || teams_array[2] > 0)
        {
            if (teams_array[0] > 0 && num_pizzas >= 2)
            {
                res.push_back({2, i++, i++});
                teams_array[0]--;
                num_pizzas -= 2;
                numPizzasDelivered++;
                continue;
            }
            if (teams_array[1] > 0 && num_pizzas >= 3)
            {
                res.push_back({3, i++, i++, i++});
                teams_array[1]--;
                num_pizzas -= 3;
                numPizzasDelivered++;
                continue;
            }
            if (teams_array[2] > 0 && num_pizzas >= 4)
            {
                res.push_back({4, i++, i++, i++, i++});
                teams_array[2]--;
                num_pizzas -= 4;
                numPizzasDelivered++;
                continue;
            }
            if (true)
            {
                break;
            }
        }
        return res;
    }
};

int main()
{
    vector<const char *> inputs = {"inputs/a.in", "inputs/b.in", "inputs/c.in", "inputs/d.in", "inputs/e.in"};
    vector<const char *> outputs = {"outputs/a.out", "outputs/b.out", "outputs/c.out", "outputs/d.out", "outputs/e.out"};
    for (ll i = 0; i < inputs.size(); i++)
    {
        freopen(inputs[i], "r", stdin);
        ll num_pizzas, T2, T3, T4, num_ingredients;
        string ingredient;
        cin >> num_pizzas >> T2 >> T3 >> T4;
        //TODO, check if you need to clear the priority queue here.
        pizzeria_data pizzeria;
        pizzeria.teams_array = {T2, T3, T4};
        pizzeria.num_pizzas = num_pizzas;
        pizzeria.pizzas = vector<vector<string>>(num_pizzas);
        for (ll i = 0; i < num_pizzas; i++)
        {
            vector<ll> vec;
            cin >> num_ingredients;
            while (num_ingredients--)
            {
                cin >> ingredient;
                pizzeria.pizzas[i].push_back(ingredient);
            }
        }

        vector<vector<ll>> delivered = pizzeria.pizzasDelivered();
        ofstream file;
        file.open(outputs[i]);
        file << pizzeria.numPizzasDelivered << endl;

        for (vector<ll> del : delivered)
        {
            stringstream oss;
            copy(del.begin(), del.end(), ostream_iterator<ll>(oss, " "));
            file << oss.str() << endl;
        }
        file.close();
    }

    return 0;
}