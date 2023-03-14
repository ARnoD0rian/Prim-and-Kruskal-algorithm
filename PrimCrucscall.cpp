#include <iostream>
#include<vector>
#include<chrono>
#include<random>
#include<fstream>
#include<string>
#include<queue>
#include<list>
#include<algorithm>
using namespace std;

using ll = long long;
bool cmp(vector<ll> &a, vector<ll> &b){
    return a[0] < b[0];
}

ll dist(vector<vector<ll> > &arr) {
    ll distation = 0;
    for (ll i = 0; i < arr.size(); ++i) {
        distation += arr[i][0];
    }
    return distation;
}
void res(ll res, string name){
    ofstream file;
    file.open(name + "Result.txt", ios::app);
    file << to_string(res) + "\n";
    file.close();
}
void results(chrono::duration<double> time, string name){
    ofstream file;
    file.open(name + ".txt", ios::app);
    file << to_string(time.count()) + "\n";
    file.close();
}
void generation(vector<vector<ll> > &arr, ll n) {
    mt19937 mt(time(nullptr));
    for (ll i = 0; i < n - 1; ++i){
        for (ll j = i + 1; j < n; ++j){
            arr[i][j] = mt() % 10;
            arr[i][j] = abs(arr[i][j]);
            arr[j][i] = arr[i][j];
        }
    }
    for (ll i = 0; i < n; ++i){
        for (ll j = 0; j < n; ++j){
            cout<<arr[i][j]<<' ';
        }
        cout<<endl;
    }
}

void counter(vector<vector<ll> > &v) {
    for (ll i = 0; i < v.size(); ++i) {
        cout << v[i][1] + 1<< ' ' << v[i][2] + 1 << ' '<< v[i][0] << endl;
    }
    cout<<endl;
}
void seachMinimumDst(vector<vector<ll>> &graph, vector<ll> &q, vector<vector<ll> > &arr, vector<bool> &visited) {
    vector<ll> v(3,  999);
    for (ll i = 0; i < q.size(); ++i) {
        for(ll j = 0; j < graph.size(); ++j) {
            if ((graph[q[i]][j] != 0) && (v[0] > graph[q[i]][j]) && (!visited[j])) {
                v[0] = graph[q[i]][j];
                v[1] = q[i];
                v[2] = j;
            }
        }
    }
    visited[v[2]] = true;
    q.push_back(v[2]);
    arr.push_back(v);
    return;
}

void addq(ll visit, priority_queue<pair<ll, ll>, vector<pair<ll, ll> >, greater<pair<ll, ll> > > &queue, vector<vector<ll> > &graph, vector<bool> &visited){
    pair<ll, ll> v;
    for (ll i = 0; i < graph.size(); ++i) {
        if ((!visited[i]) && (graph[visit][i] != 0)){
            v.first = graph[visit][i];
            v.second = i;
            queue.push(v);
        }
    }
}

ll prim(vector<vector<ll> > &graph) {
    chrono::duration<double> time;
    auto start = chrono::steady_clock::now();
    ll distation = 0;
    ll n = graph.size();
    pair<ll, ll> v;
    vector<bool> visited(n, 0);
    priority_queue<pair<ll, ll>, vector<pair<ll, ll> >, greater<pair<ll, ll> > > queue;

    for (ll i = 0; i < n; ++i) {
        for(ll j = i + 1; j < n; ++j) {
            if (graph[i][j] != 0){
                v.first = graph[i][j];
                v.second = j;
                queue.push(v);
                visited[i] = true;
                for (ll k = j + 1; k < n; ++k){
                    if (graph[i][k] != 0){
                        v.first = graph[i][k];
                        v.second = k;
                        queue.push(v);
                    }
                }
                break;
            }
        }
        if (visited[i]){
            n--;
            break;
        }
    }
    while ((n > 0) && (!queue.empty())) {
        v = queue.top();
        queue.pop();
        if (visited[v.second]){
            continue;
        }
        distation += v.first;
        visited[v.second] = true;
        n--;
        addq(v.second, queue, graph, visited);
    }
    auto end = chrono::steady_clock::now();
    time = end- start;
    results(time,"Prim");
    //counter(arr);
    return distation;
}

void balance(vector<vector<ll> > &arr, ll index, ll n) {//переставляем элемент вверх, пока возможно
    ll indexChild = 2 * index + 1; // индексы детей
    if ((indexChild < n) && (arr[indexChild][0] < arr[index][0])){// если ребенок меньше родителя переставляем
        swap(arr[indexChild], arr[index]);
        balance(arr, indexChild, n);// спускаемся вниз
    }
    if ((indexChild + 1 < n) && (arr[indexChild + 1][0] < arr[index][0])){// все аналогично
        swap(arr[index], arr[indexChild + 1]);
        balance(arr, indexChild + 1, n);
    }
}
ll searchCruscal(vector<vector<ll> > &arr) {
    ll m = arr[0][0];
    ll index = 0;
    for (ll i = 1; i < arr.size(); ++i){
        if (m > arr[i][0]){
            m = arr[i][0];
            index = i;
        }
    }
    return index;
}
ll getRoot(vector<ll> &setted, ll v){
    if (setted[v] == v){
        return v;
    }else{
        return setted[v] = getRoot(setted, setted[v]);
    }
}
bool merge(vector<ll> &setted, vector<ll> rank, ll a, ll b){
    ll setA = getRoot(setted, a), setB = getRoot(setted, b);
    if (setA == setB){
        return false;
    }else{
        if (rank[setA] < rank[setB]){
            setted[setA] = setB;
        }else if (rank[setA] > rank[setB]){
            setted[setB] = setA;
        }else{
            setted[setA] = setB;
            rank[setB]++;
        }
        return true;
    }
}
ll crucscal(vector<vector<ll> > &graph) {
    chrono::duration<double> time;
    ll distation = 0;
    ll n = graph.size(), q;
    vector<ll> variety (n);
    vector<vector<ll> > queue;
    vector<ll> v(3);
    vector<ll> rk(n,1);

    for (ll i = 0; i < n - 1; ++i) {
        for (ll j = i + 1; j < n; ++j) {
            if (graph[i][j] != 0){
                v[0] = graph[i][j];
                v[1] = i;
                v[2] = j;
            queue.push_back(v); 
            }
        }
    }
    auto start = chrono::steady_clock::now();
    sort(queue.begin(), queue.end(), cmp);

    for (ll i = 0; i < n; ++i){
        variety[i] = i;
    }

    ll k = 0;
    n--;
    while ((n > 0) && (k != queue.size())){
        v = queue[k++];
        if (merge(variety, rk, v[1], v[2])){
            n--;
            distation += v[0];
        }

    }
    //counter(arr);
    auto end = chrono::steady_clock::now();
    time = end - start;
    results(time,"Crussacll");
    return distation;

}
void start(ll n){
    ifstream file;
    string path;
    path = "tests/test/sprase/" + to_string(n) + ".txt";
    file.open(path);
    vector<vector<ll> > graph(1000, vector<ll>(1000));
    if (file.is_open()) {
        string i, j, w;
        while(!file.eof()){
            file >> i >> j >> w;
            graph[stoi(i)][stoi(j)] = stoi(w);
            graph[stoi(j)][stoi(i)] = stoi(w);
        }
    }else {
        cout << "ti pidor" << endl;
    }
    file.close();
   ll Crusscall =  crucscal(graph);
   ll Prim = prim(graph);
   res(Prim, "Prim");
   res(Crusscall, "Crusscall");
}
int main() {
    for (ll i = 20; i <= 200; i+=5){
        start(i);
    }
    return 0;
}