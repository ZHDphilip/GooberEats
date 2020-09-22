/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: PointToPointRouter.cpp
*/

#include "provided.h"
#include <list>
#include <map>
#include <cmath>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_strMap;
    struct Node {
        Node(GeoCoord coord, GeoCoord destination, Node* par = nullptr)
            :g(0), parent(par), gc(coord)
        {
            h = distanceEarthMiles(gc, destination);
            f = h + g;
        }
        //~Node(){ delete parent; }
        Node* parent;
        double g, h, f;
        GeoCoord gc;
    };
    /////////////////////
    /*Node* findNodeWithLowF(list<Node*> list) const {
        typename std::list<Node*> ::iterator p = list.begin();
        double min = 100000000;
        Node* Min = nullptr;
        while (p != list.end()) {
            if ((*p)->f <= min) {
                min = (*p)->f;
                Min = *p;
            }
            p++;
        }
        return Min;
    }*/
    //////////////////////
    double calG(Node* start, Node* current) const {
        double G_add = distanceEarthMiles(start->gc, current->gc);
        double g_par;
        if (start->parent != nullptr)
            g_par = start->parent->g;
        else
            g_par = 0;
        return G_add + g_par;
    }
    double calH(Node* current, const GeoCoord& end) const {
        return distanceEarthMiles(current->gc, end); //still needs improvement, Euc Dist is not optimized
    }
    double calF(Node* current) const {
        return current->g + current->h;
    }
    /////////////////////////////////////////////////////////////////////////
    /*Node* isInList(Node* node, list<Node*> list) const {
        typename std::list<Node*>::iterator it = list.begin();
        while (it != list.end()) {
            if ((*it)->gc == node->gc)
                return (*it);
            it++;
        }
        return nullptr;
    }*/
    ////////////////////////////////////////////////////////////////////////////////////
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_strMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
    //delete m_strMap;
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const {

    route.clear();

    if (start == end) {     //if start and end are exactly the same, then clear the list, set dist=0, and return we found the route
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    vector<StreetSegment> test1, test2;
    if (!m_strMap->getSegmentsThatStartWith(start, test1) || !m_strMap->getSegmentsThatStartWith(end, test2))
        return BAD_COORD;       //test if the two GeoCoords passed in are valid.

    map<GeoCoord, Node*> openList;
    map<GeoCoord, Node*> closeList;
    Node* startPoint = new Node(start, end, nullptr);
    //Node* endPoint = new Node(end, end, nullptr);

    openList[start] = startPoint;

    bool indicator = false; //indicating we have not yet found the path yet.

    Node* current = startPoint;
    //Node* result;
    while (!openList.empty() && !indicator) {
        current = openList.begin()->second;     //let current point to the Node with smallest F value
        for (auto it = openList.begin(); it != openList.end(); it++) {
            if (it->second->f < current->f) {
                current = it->second;
            }
        }
        if (current->gc == end) {   //if current is equal to end, meaning we have found the route, and it is supposed to be the optimized route
            indicator = true;
            //result = current;
            break;
        }
        openList.erase(openList.find(current->gc));

        vector<StreetSegment> adjacent;     //get all the segments that start with our current GeoCoord
        m_strMap->getSegmentsThatStartWith(current->gc, adjacent);
        vector<StreetSegment>::iterator it = adjacent.begin();
        while (it != adjacent.end()) {      //For Every adjacent GeoCoord
            GeoCoord dest = (*it).end;
            double current_cost = current->g + distanceEarthMiles(current->gc, dest);
            if (openList.find(dest) != openList.end()) {
                if (openList[dest]->g > current_cost) {
                    openList[dest]->g = current_cost;
                    openList[dest]->f = current_cost + openList[dest]->h;
                    openList[dest]->parent = current;
                }
            }
            else if (closeList.find(dest) != closeList.end())
            {
                if (closeList[dest]->g > current_cost) {
                    openList[dest] = closeList[dest];
                    //delete (*(closeList.find(dest)))->second;
                    auto iter = closeList.find(dest);
                    closeList.erase(iter);
                }
            }
            else
            {   //push onto the openlist
                Node* nextNode = new Node(dest, end, current);
                openList[dest] = nextNode;
                openList[dest]->g = current_cost;
                openList[dest]->f = current_cost + openList[dest]->h;
                openList[dest]->parent = current;
            }
            it++;
        }
        closeList[current->gc] = current;
    }
    if (indicator) {    //meaning we have found the route
        double distance = 0;
        list<StreetSegment> res;
        while (current->parent != nullptr) {
            vector<StreetSegment> temp;
            m_strMap->getSegmentsThatStartWith(current->parent->gc, temp);
            vector<StreetSegment>::iterator p = temp.begin();
            while (p != temp.end()) {
                if ((*p).end == current->gc) {
                    res.push_front(*p);
                    distance += distanceEarthMiles(current->gc, current->parent->gc);
                    current = current->parent;
                    break;
                }
                else
                    p++;
            }
        }
        //clean up dynamically allocated memory

        route = res;
        totalDistanceTravelled = distance;
        //openList.clear();
        //closeList.clear();
        /*for (typename map<GeoCoord,Node*>::iterator p1 = openList.begin();p1 != openList.end();) {
            //if(p1->second!=nullptr)
            delete (*p1).second;
            p1 = openList.erase(p1);
        }
        for (typename map<GeoCoord, Node*>::iterator p2 = closeList.begin();p2 != openList.end();) {
            //if(p2->second!=nullptr)
            delete (*p2).second;
            p2 = openList.erase(p2);
        }*/
        return DELIVERY_SUCCESS;
    }
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
    const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
