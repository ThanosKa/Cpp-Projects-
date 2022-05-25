#include "ask3.h"

void algebra(Planet &pl, Planet &cur_planet)
{
    double dx = cur_planet.x - pl.x;
    double dy = cur_planet.y - pl.y;
    double distance = sqrt(dx * dx + dy * dy);
    double F = Gravity * ((pl.mass + cur_planet.mass) / (distance * distance));
    pl.Fx = pl.Fx + F * (dx / distance);
    pl.Fy = pl.Fy + F * (dy / distance);
}

void force(Node &nNode, Planet &pl, vector<Node> &nodes)
{

    Planet cur_planet;
    Node cur_node;
    if (nNode.planets.size() > 0)
    {
        double dx = pl.x - cur_planet.x;
        double dy = pl.y - cur_planet.y;
        double distance = sqrt(dx * dx + dy * dy);

        if (nNode.planets.size() == 1)
        {
            cur_planet = nNode.planets.at(0);
        }
        if (nNode.planets.size() > 1)
        {
            cur_planet.x = nNode.x;
            cur_planet.y = nNode.y;
            cur_planet.name = "center";
        }
        if (nNode.NW == NULL && nNode.NE == NULL && nNode.SW == NULL && nNode.SE == NULL)
        {
            if (!(cur_planet.pID == pl.pID && cur_planet.name == pl.name))
            {
                algebra(pl, cur_planet);
            }
        }
        else if (nNode.quad.len / distance < 0.5)
        {
            algebra(pl, cur_planet);
        }
        else
        {
            if (nNode.NW != NULL)
            {
                cur_node = nodes.at(nNode.NW->nID + 1);
                force(cur_node, pl, nodes);
            }
            if (nNode.NE != NULL)
            {
                cur_node = nodes.at(nNode.NE->nID + 1);
                force(cur_node, pl, nodes);
            }
            if (nNode.SW != NULL)
            {
                cur_node = nodes.at(nNode.SW->nID + 1);
                force(cur_node, pl, nodes);
            }
            if (nNode.SE != NULL)
            {
                cur_node = nodes.at(nNode.SE->nID + 1);
                force(cur_node, pl, nodes);
            }
        }
    }
}