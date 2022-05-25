
#include "ask3_maths.cpp"
#include <time.h>
void initQuads(vector<Quad> &quads, Quad quad)
{
    Quad NW, NE;
    Quad SW, SE;
    NW = {static_cast<int>(quads.size(), quad.qID, quad.level, "NW", quad.x - quad.len / 4.0, quad.y + quad.len / 4.0, quad.len / 2.0)};
    quads.push_back(NW);
    quad.divs.push_back(NW);

    NE = {static_cast<int>(quads.size(), quad.qID, quad.level, "NE", quad.x + quad.len / 4.0, quad.y + quad.len / 4.0, quad.len / 2.0)};
    quads.push_back(NE);

    quad.divs.push_back(NE);

    SW = {static_cast<int>(quads.size(), quad.qID, quad.level, "SW", quad.x - quad.len / 4.0, quad.y - quad.len / 4.0, quad.len / 2.0)};
    quads.push_back(SW);

    quad.divs.push_back(SW);

    SE = {static_cast<int>(quads.size(), quad.qID, quad.level, "SE", quad.x + quad.len / 4.0, quad.y - quad.len / 4.0, quad.len / 2.0)};
    quads.push_back(SE);
    quad.divs.push_back(SE);
}
int exists_inQuad(Quad quad, double x, double y)
{
    if (quad.x + quad.len / 2.0 >= x && quad.x - quad.len / 2.0 <= x && quad.y + quad.len / 2.0 >= y && quad.y - quad.len / 2.0 <= y)
        return 1;
    else
        return 0;
}
void insertPlanet(vector<Planet> &planets, vector<Quad> &quads, Quad quad, int i)
{
    Planet planet = planets.at(i);
    for (int i = 0; i < quads.size(); i++)
    {
        if (exists_inQuad(quads.at(i), planets.at(i).x, planets.at(i).y) == 1)
        {
            quads.at(i).planets.push_back(planet);
            break;
        }
    }
}
void insertQuadTree(vector<Planet> &planets, vector<Quad> &quads, Quad quad, int times)
{
    for (int i = 0; i < times; i++)
    {

        insertPlanet(planets, quads, quad, i);
        initQuads(quads, quad);
        quads.insert(quads.begin(), quad);
        for (int j = 0; j < quads.size(); j++)
        {
            Node newNode = {quads.at(j).qID, quads.at(j).parent, quads.at(j).planets, quads.at(j), 0, 0, NULL, NULL, NULL, NULL};
            nodes.push_back(newNode);
        }
    }
}

void creaetQuad(vector<Node> &nodes)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i + 1; j < nodes.size(); j++)
        {
            if (nodes.at(i).nID == nodes.at(j).parent)
            {
                if (nodes.at(j).quad.pos == "NW")
                {
                    if (nodes.at(i).NW == NULL)
                    {
                        nodes.at(i).NW = &nodes.at(j);
                    }
                }
                else if (nodes.at(j).quad.pos == "NE")
                {
                    if (nodes.at(i).NE == NULL)
                    {
                        nodes.at(i).NE = &nodes.at(j);
                    }
                }
                else if (nodes.at(j).quad.pos == "SW")
                {
                    if (nodes.at(i).SW == NULL)
                    {
                        nodes.at(i).SW = &nodes.at(j);
                    }
                }
                else if (nodes.at(j).quad.pos == "SE")
                {
                    if (nodes.at(i).SE == NULL)
                    {
                        nodes.at(i).SE = &nodes.at(j);
                    }
                }
            }
        }
    }
}

void tbb_parrallelCom()
{

    parallel_for(
        blocked_range<size_t>(0, nodes.size()),
        [=](const blocked_range<size_t> &range)
        {
            for (int i = range.begin(); i != range.end(); ++i)
            {
                double x = 0.0, y = 0.0, mass = 0.0, upX = 0.0, upY = 0.0;

                if (nodes.at(i).planets.size() > 0)
                {

                    for (int j = 0; j < nodes.at(i).planets.size(); j++)
                    {
                        mass += nodes.at(i).planets[j].mass;
                        upX += (nodes.at(i).planets.at(j).x * nodes.at(i).planets.at(j).mass);
                        upY += (nodes.at(i).planets.at(j).y * nodes.at(i).planets.at(j).mass);
                    }
                    nodes.at(i).x = upX / mass;
                    nodes.at(i).y = upY / mass;
                }
            }
        },
        auto_partitioner());
}
void tbb_parallelCalc()
{
    parallel_for(
        blocked_range<size_t>(0, planets.size()),
        [=](const blocked_range<size_t> &range)
        {
            for (int i = range.begin(); i != range.end(); ++i)
            {
                planets.at(i).Fx = 0.0;
                planets.at(i).Fy = 0.0;
                force(nodes.at(0), planets.at(i), nodes);
                // calculateForce(nodes.at(0),planets.at(index),nodes);
                planets.at(i).Vx = planets.at(i).Vx + (planets.at(i).Fx / planets.at(i).mass);
                planets.at(i).Vy = planets.at(i).Vy + (planets.at(i).Fy / planets.at(i).mass);
                planets.at(i).x = planets.at(i).x + planets.at(i).Vx;
                planets.at(i).y = planets.at(i).y + planets.at(i).Vy;
            }
        },
        auto_partitioner());
}
int main(int argc, char *argv[])
{
    // struct timespec  end,start;
    double elapsed;
    struct timespec end, start;
    Quad quad;
    ifstream ss;
    string file_in;
    char *file_out;
    int times, N;
    double R;
    if (argc == 5)
    {
        file_in = argv[1];
        times = atoi(argv[2]);
        file_out = argv[3];
        threads = atoi(argv[4]);
    }
    else
        cerr << "Wrong number of arguments" << endl;
    ifstream fin(file_in);
    fin >> N;
    fin >> R;
    /*inserting  elements to planets vector*/
    for (int i = 0; i < N; i++)
    {
        Planet planet;
        fin >> planet.x;
        fin >> planet.y;
        fin >> planet.Vx;
        fin >> planet.Vy;
        fin >> planet.mass;
        fin >> planet.name;
        planet.Fx = 0;
        planet.Fy = 0;

        planets.push_back(planet);
    }
    quad = {0, 0, 0, "init", 0, 0, 2 * R};

    clock_gettime(CLOCK_MONOTONIC, &start);
    initQuads(quads, quad);
    insertQuadTree(planets, quads, quad, times);

    creaetQuad(nodes);
    tbb_parrallelCom();
    tbb_parallelCalc();
    // for (int i = 0; i < planets.size(); i++)
    //{
    //     cout << planets.at(i).pID << endl;
    //}
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("TIME ELAPSED %fs \n", elapsed);
    FILE *fout = fopen(file_out, "w+");
    for (int i = 0; i < planets.size(); i++)
    {
        char arr[planets.at(i).name.length() + 1];
        strcpy(arr, planets.at(i).name.c_str());
        fprintf(fout, "%13Le %13Le %13Le %13Le %13Le %13s\n", planets.at(i).x, planets.at(i).y, planets.at(i).Vx, planets.at(i).Vy, planets.at(i).mass, arr);
    }
    return 0;
}