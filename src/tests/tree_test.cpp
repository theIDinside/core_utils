// System headers
#include <string>
#include <vector>
#include <tuple>

// TODO(fix): This is a really bad definition name. 
#ifdef DEBUG
	#include <fmt/core.h>
#endif

// Library headers, defined in the include path to compiler (-I ...)
#include <containers/tree.hpp>

namespace skog = cx::containers::skog;
using skog::u64;


int main(int argc, const char** argv) {

	#ifdef DEBUG
	
	#endif

	std::vector<std::pair<std::string, u64>> strnums{};

	auto video_game_characters = skog::make_root<std::string, skog::u64>("Super Mario", 30);
	strnums.push_back({"Joel", 1});
	strnums.push_back({"Ellie", 3});
	strnums.push_back({"Samus", 13});
	strnums.push_back({"Link", 4});
	strnums.push_back({"Kratos", 27});
	strnums.push_back({"Donkey Kong", 23});
	strnums.push_back({"Aloy", 333});

	#ifdef DEBUG
	fmt::print("Order of insertion into tree: \n");
	#endif

	for(const auto& [s, n] : strnums) {
		#ifdef DEBUG
			fmt::print("Data: {} \tKey: {}\n", s, n);
		#endif
		video_game_characters->insert(s, n);
	}
	
	#ifdef DEBUG
	fmt::print("In order traversal of tree: \n");
	#endif

	// Map TreeNode tn => X
	auto map_fn = [](auto tree_node_pointer) {
		#ifdef DEBUG
			fmt::print("Data: {} \t\tKey: {}\n", tree_node_pointer->data, tree_node_pointer->k);
		#endif
	};

	skog::in_order_map(video_game_characters.get(), map_fn);

}
