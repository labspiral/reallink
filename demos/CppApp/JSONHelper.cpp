/*
 *
 *                                                            ,--,      ,--,
 *             ,-.----.                                     ,---.'|   ,---.'|
 *   .--.--.   \    /  \     ,---,,-.----.      ,---,       |   | :   |   | :      ,---,           ,---,.
 *  /  /    '. |   :    \ ,`--.' |\    /  \    '  .' \      :   : |   :   : |     '  .' \        ,'  .'  \
 * |  :  /`. / |   |  .\ :|   :  :;   :    \  /  ;    '.    |   ' :   |   ' :    /  ;    '.    ,---.' .' |
 * ;  |  |--`  .   :  |: |:   |  '|   | .\ : :  :       \   ;   ; '   ;   ; '   :  :       \   |   |  |: |
 * |  :  ;_    |   |   \ :|   :  |.   : |: | :  |   /\   \  '   | |__ '   | |__ :  |   /\   \  :   :  :  /
 *  \  \    `. |   : .   /'   '  ;|   |  \ : |  :  ' ;.   : |   | :.'||   | :.'||  :  ' ;.   : :   |    ;
 *   `----.   \;   | |`-' |   |  ||   : .  / |  |  ;/  \   \'   :    ;'   :    ;|  |  ;/  \   \|   :     \
 *   __ \  \  ||   | ;    '   :  ;;   | |  \ '  :  | \  \ ,'|   |  ./ |   |  ./ '  :  | \  \ ,'|   |   . |
 *  /  /`--'  /:   ' |    |   |  '|   | ;\  \|  |  '  '--'  ;   : ;   ;   : ;   |  |  '  '--'  '   :  '; |
 * '--'.     / :   : :    '   :  |:   ' | \.'|  :  :        |   ,/    |   ,/    |  :  :        |   |  | ;
 *   `--'---'  |   | :    ;   |.' :   : :-'  |  | ,'        '---'     '---'     |  | ,'        |   :   /
 *             `---'.|    '---'   |   |.'    `--''                              `--''          |   | ,'
 *               `---`            `---'                                                        `----'
 *
 * 2025 Copyright to (c)SpiralLAB. All rights reserved.
 * Description : JSON helper by nlohmann
 * Author : hong chan, choi / hcchoi@spirallab.co.kr (http://spirallab.co.kr)
 *
 */


#include "JSONHelper.h"
#include <chrono>
#include <regex>


reallinkcpp::value JSONToRealLinkValue(const nlohmann::json& j)
{
    std::map<std::string, reallinkcpp::value> result;

    for (auto it = j.begin(); it != j.end(); ++it)
    {
        const auto& key = it.key();
        const auto& v = it.value();

        if (v.is_string())
        {
            result[key] = value(v.get<std::string>());
        }
        else if (v.is_number_integer())
        {
            result[key] = value(static_cast<double>(v.get<int>())); // as doubles
        }
        else if (v.is_number_unsigned())
        {
            result[key] = value(static_cast<double>(v.get<unsigned int>())); // as double
        }
        else if (v.is_number_float())
        {
            result[key] = value(v.get<double>());
        }
        else if (v.is_boolean())
        {
            result[key] = value(v.get<bool>());
        }
        else if (v.is_object())
        {
            // Recursively convert nested objects
            result[key] = value(JSONToRealLinkValue(v));
        }
        else if (v.is_array())
        {
            // Convert arrays to a vector of kvalue
            std::vector<value> arrayValues;
            for (const auto& arrayElement : v)
            {
                if (arrayElement.is_string())
                    arrayValues.push_back(value(arrayElement.get<std::string>()));
                else if (arrayElement.is_number_integer())
                    arrayValues.push_back(value(static_cast<double>(arrayElement.get<int>())));
                else if (arrayElement.is_number_unsigned())
                    arrayValues.push_back(value(static_cast<double>(arrayElement.get<unsigned int>())));
                else if (arrayElement.is_number_float())
                    arrayValues.push_back(value(arrayElement.get<double>()));
                else if (arrayElement.is_boolean())
                    arrayValues.push_back(value(arrayElement.get<bool>()));
                else if (arrayElement.is_object())
                    arrayValues.push_back(value(JSONToRealLinkValue(arrayElement)));
                else
                    throw std::runtime_error("Unsupported JSON array element type");
            }
            result[key] = value(arrayValues);
        }
        else if (v.is_null())
        {
            // warning ! value is not assigned
        }
        else
        {
            throw std::runtime_error("Unsupported JSON type for key: " + key);
        }
    }
    return value(result);
}

nlohmann::json RealLinkValueToJSON(const reallinkcpp::value& v)
{
    switch (v.type()) {
    case value_type::map: {
        nlohmann::json result;
        for (const auto& pair : v.as_map()) {
            result[pair.first] = RealLinkValueToJSON(pair.second);
        }
        return result;
    }
    case value_type::array: {
        nlohmann::json result = nlohmann::json::array();
        for (const auto& item : v.as_array()) {
            result.push_back(RealLinkValueToJSON(item));
        }
        return result;
    }
    case value_type::string:
        return v.as_string();
    case value_type::float64:
        return v.as_double();
    case value_type::boolean:
        return v.as_bool();
    case value_type::null:
        return nullptr;
    default:
        throw std::runtime_error("Unsupported kvalue type");
    }
}


