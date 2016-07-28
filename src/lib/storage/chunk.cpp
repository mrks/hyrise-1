#include "common.hpp"
#include "chunk.hpp"
#include "raw_attribute_vector.hpp"

#include <iomanip>

namespace opossum {

chunk::chunk() {

}

void chunk::add_column(column_type type) {
	// FIXME replace with boost::hana

	switch(type) {
		case int_type:
			_columns.emplace_back(std::make_shared<raw_attribute_vector<int>>());
			break;
		case float_type:
			_columns.emplace_back(std::make_shared<raw_attribute_vector<float>>());
			break;
		case string_type:
			_columns.emplace_back(std::make_shared<raw_attribute_vector<std::string>>());
			break;
		default:
			std::cout << "keine Lust auf andere Typen" << std::endl;
	}
}

void chunk::append(std::initializer_list<all_type_variant> values) {
	if(_columns.size() != values.size()) {
		throw std::runtime_error("append: number of columns (" + to_string(_columns.size()) + ") does not match value list (" + to_string(values.size()) + ")");
	}

	auto column_it = _columns.begin();
	auto value_it = values.begin();
	for(; column_it != _columns.end(), value_it != values.end(); column_it++, value_it++) {
		(*column_it)->append(*value_it);
	}
}

std::vector<int> chunk::column_string_widths(int max) const {
	std::vector<int> widths(_columns.size());
	for(size_t col = 0; col < _columns.size(); ++col) {
		for(size_t row = 0; row < size(); ++row) {
			int width = to_string((*_columns[col])[row]).size();
			if(width > widths[col]) {
				if(width >= max) {
					widths[col] = max;
					break;
				}
				widths[col] = width;
			}
		}
	}
	for(auto w : widths) std::cout << "!!! " << w << std::endl;
	return widths;
}

void chunk::print(std::ostream &out, const std::vector<int> &widths_in) const {
	auto widths = widths_in.size() > 0 ? widths_in : column_string_widths(20);
	for(size_t row = 0; row < size(); ++row) {
		for(size_t col = 0; col < _columns.size(); ++col) {
			out << std::setw(widths[col]) << (*_columns[col])[row] << "|" << std::setw(0);			
		}
		out << std::endl;
	}
}

size_t chunk::size() const {
	if(DEBUG && _columns.size() == 0) {
		throw std::runtime_error("Can't calculate size on table without columns");
	}
	return _columns.front()->size();
}

}