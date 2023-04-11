#pragma once
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <time.h>

#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include "lib/wfc/image.hpp"
#include "lib/wfc/overlapping_wfc.hpp"
#include "lib/wfc/array_3d.hpp"
#include "lib/wfc/wfc.hpp"
#include "core/Utils/PHPath.h"

using namespace rapidxml;
using namespace std;


class WaveFunctionCollapse 
{
public:
	static int get_random_seed();
	static Wave::Heuristic to_heuristic(const string& heuristic_string);
	static void run_overlapping(xml_node<>* node);
	void read_config_file(const string& config_path) noexcept;

};