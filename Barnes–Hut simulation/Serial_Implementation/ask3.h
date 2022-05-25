#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
//#include "tbb/tbb.h"
#include <time.h>

using namespace std;
//using namespace tbb;

long double Gravity = 6.673e-11;
int threads;
struct Planet
{
    int pID;
    long double x, y;
    long double Vx, Vy;
    long double mass;
    long double Fx, Fy;
    string name;
};
typedef struct Planet Planet;

struct Quad
{
    int qID;
    int parent;
    int level;
    string pos; //(nw,ne,sw,se)
    double x;
    double y;
    double len;
    vector<Planet> planets;
    vector<Quad> divs;
};
typedef struct Quad Quad;

struct Node
{
    int nID;
    int parent;
    vector<Planet> planets;
    Quad quad;
    double x;
    double y;
    Node *NW;
    Node *NE;
    Node *SW;
    Node *SE;
};
typedef struct Node Node;

vector<Planet> planets;
vector<Quad> quads;
vector<Node> nodes;

extern void algebra(Planet &pl, Planet &cur_planet);
extern void force(Node &nNode, Planet &pl, vector<Node> &nodes);
extern void tbb_parrallelCom();
extern void  tbb_parallelCalc();