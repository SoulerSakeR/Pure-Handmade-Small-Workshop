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
#include "lib/wfc/overlapping_wfc.hpp"
#include "lib/wfc/array_3d.hpp"
#include "lib/wfc/wfc.hpp"
#include "core/Utils/PHPath.h"




class WaveFunctionCollapse 
{
public:
	static int get_random_seed();
	static Wave::Heuristic to_heuristic(const std::string& heuristic_string);
	static void run_overlapping_xml(rapidxml::xml_node<>* node);
	static void run_overlapping(std::string name,
		std::string image_path,
		std::string output_path = "wfc_results/",
		uint32_t width = 48,
		uint32_t height = 48,
		uint32_t N = 3,
		bool periodic_output = false,
		bool periodic_input = true,
		bool ground = false,
		uint32_t symmetry = 8,
		uint32_t screenshots = 2,
		std::string heuristic = "Entropy");
	static void read_config_file(const std::string& config_path) noexcept;

};