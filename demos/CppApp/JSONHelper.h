#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "..\..\reallinkcpp\include\reallinkcpplib.h"
#include <nlohmann/json.hpp>
#pragma once

reallinkcpp::value JSONToRealLinkValue(const nlohmann::json& j);
nlohmann::json RealLinkValueToJSON(const reallinkcpp::value& v);

#endif 