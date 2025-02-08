#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <nlohmann/json.hpp>
#pragma once


typedef struct tagUnit
{
    int Col;
    int Row;
    int Bin;
    
    struct tagUnit()
    {
        Col = Row = Bin = 0;
    }
    struct tagUnit(int col, int row, int bin)
    {
        Col = col;
        Row = row;
        Bin = bin;
    };

}UNIT;

typedef struct tagSubstrate 
{
    std::string Name; 
    int Rows;
    int Cols;
    std::vector<UNIT> Units;

public:
    struct tagSubstrate()
    {
    }
    struct tagSubstrate(std::string uniqueName)
    {
        Name = uniqueName;
    }

    void Serialize(nlohmann::json& jsonDestination)
    {
        //ref link : https://snowdeer.github.io/c++/2022/01/11/cpp-nlohmann-json-example/
        jsonDestination["name"] = Name;
        jsonDestination["cols"] = Cols;
        jsonDestination["rows"] = Rows;
        nlohmann::json units;
        for (int i = 0; i< Units.size(); i++)
        {
            nlohmann::json unit = { 
                {"col", Units[i].Col},
                {"row", Units[i].Row},
                {"bin", Units[i].Bin}
            };
            units.push_back(unit);
        }
        jsonDestination["units"] = units;
    }

    bool Deserialize(const nlohmann::json& jsonSource)
    {
        Units.clear();
        Name = jsonSource["name"];
        Cols = (int)jsonSource["cols"];
        Rows = (int)jsonSource["rows"];

        for (int i = 0; i < jsonSource["units"].size(); i++)
        {
            UNIT unit;
            unit.Col = jsonSource["units"][i]["col"];
            unit.Row = jsonSource["units"][i]["row"];
            unit.Bin = jsonSource["units"][i]["bin"];
            Units.push_back(unit);
        }
        return true;
       
    }
    
}SUBSTRATE;

#endif
