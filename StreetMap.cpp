/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: StreetMap.cpp
*/

#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const string& s) {
    unsigned int h = std::hash<string>()(s);
    return h;
    //return std::hash<string>()(s);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap <GeoCoord, vector<StreetSegment>>* m_map;
    int countLines(istream& inf)   //count the num of lines in a particular file
    {
        int lineCount = 0;
        string line;
        while (getline(inf, line))
            lineCount++;
        return lineCount;
    }
};

StreetMapImpl::StreetMapImpl()
{
    m_map = new ExpandableHashMap <GeoCoord, vector<StreetSegment>>;
}

StreetMapImpl::~StreetMapImpl()
{
    //delete m_map;
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if (!infile) {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    else {
        string line;
        while (getline(infile, line)) {
            int linesTobeRead = 0;
            string currentStrName = line;     //and we can start on mapping the next street
            string num;
            getline(infile, num);
            int multiplier = 1;
            for (int i = num.size() - 1;i >= 0;i--) {
                linesTobeRead += (num[i] - '0') * multiplier;
                multiplier *= 10;
            }
            for (int j = 0;j < linesTobeRead;j++) {
                string pos[4];
                string thisLine;
                getline(infile, thisLine);
                int index = 0;
                for (int k = 0;k < thisLine.size();k++) {
                    if (thisLine[k] == ' ' || k == thisLine.size() - 1) {
                        pos[index] = thisLine.substr(0, k);
                        thisLine = thisLine.substr(k + 1);
                        k = -1;
                        index++;
                    }
                    else if (index == 3) {
                        pos[index] = thisLine;
                        thisLine = "";
                    }
                    else {
                        continue;
                    }
                }
                GeoCoord start(pos[0], pos[1]);
                GeoCoord end(pos[2], pos[3]);
                //StreetSegment* n = new StreetSegment(start, end, currentStrName);   //create such a streetsegment
                //StreetSegment* r = new StreetSegment(end, start, currentStrName);   //generate the reverse segment
                StreetSegment n(start, end, currentStrName);   //create such a streetsegment
                StreetSegment r(end, start, currentStrName);   //generate the reverse segment
                /*
                what to do next:
                check whether mapping with start/end as KeyType already exist
                    if so:
                        update the corresponding vector
                    else
                        add a new mapping with start/end as KeyType
                        and create a new vector as Valuetype
                */
                if (m_map->find(start) == nullptr) {    //mapping the streetsegment
                    vector<StreetSegment> newS;
                    newS.push_back(n);
                    m_map->associate(start, newS);
                    //cerr << '(' << n->start.latitudeText << "," << n->start.longitudeText << ") (" << n->end.latitudeText << "," << n->end.longitudeText << ")" << n->name << endl;
                }
                else {
                    vector<StreetSegment>* startV = m_map->find(start);
                    startV->push_back(n);
                    //cerr << '(' << n->start.latitudeText << "," << n->start.longitudeText << ") (" << n->end.latitudeText << "," << n->end.longitudeText << ")" << n->name << endl;
                }
                if (m_map->find(end) == nullptr) {    //mapping the reverse streetsegment
                    vector<StreetSegment> newE;
                    newE.push_back(r);
                    m_map->associate(end, newE);
                    //cerr << '(' << r->start.latitudeText << "," << r->start.longitudeText << ") (" << r->end.latitudeText << "," << r->end.longitudeText << ")" << r->name << endl;
                }
                else {
                    vector<StreetSegment>* endV = m_map->find(end);
                    endV->push_back(r);
                    //cerr << '(' << r->start.latitudeText << "," << r->start.longitudeText << ") (" << r->end.latitudeText << "," << r->end.longitudeText << ")" << r->name << endl;
                }
            }
        }
        return true;
    }
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    vector<StreetSegment>* target = m_map->find(gc);
    if (target == nullptr) {
        return false;
    }
    segs = *target;
    return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    return m_impl->getSegmentsThatStartWith(gc, segs);
}
