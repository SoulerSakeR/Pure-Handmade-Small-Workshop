#include "WaveFunctionCollapse.h"
#include "lib/wfc/image.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"


using namespace std;

int WaveFunctionCollapse::get_random_seed() {
	return rand();
}

Wave::Heuristic WaveFunctionCollapse::to_heuristic(const string& heuristic_string) {
#define CASE(enum) \
    if (heuristic_string == #enum) return Wave::Heuristic::enum

    CASE(Scanline);
    CASE(Entropy);
    CASE(MRV);

    throw runtime_error("Invalid Heuristic: " + heuristic_string);
#undef CASE
}

void WaveFunctionCollapse::run_overlapping_xml(rapidxml::xml_node<>* node) {
    string name = get_attribute(node, "name");

    auto size = get_attribute(node, "size", "48");
    uint32_t width = stoi(get_attribute(node, "width", size));
    uint32_t height = stoi(get_attribute(node, "height", size));

    uint32_t N = stoi(get_attribute(node, "N", "3"));

    bool periodic_output = get_attribute(node, "periodic", "False") == "True";
    bool periodic_input =
        get_attribute(node, "periodicInput", "True") == "True";

    bool ground = get_attribute(node, "ground", "False") == "True";
    uint32_t symmetry = stoi(get_attribute(node, "symmetry", "8"));
    uint32_t screenshots = stoi(get_attribute(node, "screenshots", "2"));
    string heuristic = get_attribute(node, "heuristic", "Entropy");

    cerr << "< " << name << endl;

    const string image_path = "samples/" + name + ".png";
    auto m = read_image(image_path);

    if (!m.has_value()) {
        throw runtime_error("Error while loading " + image_path);
    }

    auto& img = m.value();

    OverlappingWFC::Options options = {};
    options.periodic_input = periodic_input;
    options.periodic_output = periodic_output;
    options.i_W = img.MX;
    options.i_H = img.MY;
    options.o_W = width;
    options.o_H = height;
    options.symmetry = uint32_t((1 << symmetry) - 1);
    options.pattern_size = N;
    options.heuristic = to_heuristic(heuristic);
    options.ground = ground;

    OverlappingWFC wfc(options, img);

    for (uint32_t i = 0; i < screenshots; i++) {
        for (uint32_t test = 0; test < 10; test++) {
            int seed = get_random_seed();
            bool success = wfc.run(seed);

            cout << "> ";
            if (success) {
                cout << "DONE" << endl;
                write_image_png("results/" + name + to_string(seed) + ".png",
                    wfc.get_output());
                break;
            }
            else {
                cout << "CONTRADICTION" << endl;
            }
        }
    }
}

void WaveFunctionCollapse::run_overlapping(string name, string image_path, string output_path, uint32_t width, uint32_t height, uint32_t N, bool periodic_output, bool periodic_input, bool ground, uint32_t symmetry, uint32_t screenshots, string heuristic) {
    
    cerr << "< " << name << endl;

    auto m = read_image(image_path);

    if (!m.has_value()) {
        throw runtime_error("Error while loading " + image_path);
    }

    auto& img = m.value();

    OverlappingWFC::Options options = {};
    options.periodic_input = periodic_input;
    options.periodic_output = periodic_output;
    options.i_W = img.MX;
    options.i_H = img.MY;
    options.o_W = width;
    options.o_H = height;
    options.symmetry = uint32_t((1 << symmetry) - 1);
    options.pattern_size = N;
    options.heuristic = to_heuristic(heuristic);
    options.ground = ground;

    OverlappingWFC wfc(options, img);

    for (uint32_t i = 0; i < screenshots; i++) {
        for (uint32_t test = 0; test < 10; test++) {
            int seed = get_random_seed();
            bool success = wfc.run(seed);

            cout << "> ";
            if (success) {
                cout << "DONE" << endl;
                write_image_png(output_path + name + to_string(seed) + ".png",
                    wfc.get_output());
                break;
            }
            else {
                cout << "CONTRADICTION" << endl;
            }
        }
    }
}

void WaveFunctionCollapse::read_config_file(const string& config_path) noexcept {
    ifstream config_file(config_path);
    vector<char> buffer((istreambuf_iterator<char>(config_file)),
        istreambuf_iterator<char>());
    buffer.push_back('\0');
    auto document = new rapidxml::xml_document<>;
    document->parse<0>(&buffer[0]);

    rapidxml::xml_node<>* root_node = document->first_node("samples");
    PHPath path = PHPath(config_path);
    string dir_path = path.getFileDir() + "/" + "samples";
    for (rapidxml::xml_node<>* node = root_node->first_node("overlapping"); node;
        node = node->next_sibling("overlapping")) {
        run_overlapping_xml(node);
    }

    delete document;
}
