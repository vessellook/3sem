#include <iostream>
#include <sstream>
#include "sequence/linked_list_sequence.h"
#include "sequence/array_sequence.h"
#include "sorting/heapsort.h"
#include "sorting/quicksort.h"
#include "sorting/mergesort.h"

using sem2::LinkedList;
using sem2::DynamicArray;
using sem2::LinkedListSequence;

template <typename T>
sem3::Sorting<T> * getSorting(const std::string &str) {
    if (str.empty() || str == "quick") {
        return new sem3::QuickSort<T>();
    } else if (str == "heap") {
        return new sem3::HeapSort<T>();
    } else if (str == "merge") {
        return new sem3::MergeSort<T>();
    } else {
        throw std::invalid_argument("strange sorting type");
    }
}

void print(const double *array, unsigned count) {
    for (unsigned i = 0; i < count; i++) {
	    std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}


std::string getHelpInfo(const std::string &filename) {
	std::stringstream ss;
	ss << "NAME\n\t"
	   << filename << " - program for sorting numbers\n"
	   << "SYNOPSIS\n\t"
	   << filename <<" -h\n\t"
	   << filename << " -a algorithm <array>\n\t"
	   << filename << " <array> -a algorithm\n"
	   << "EXAMPLE\n\t"
	   << filename << " -a quick 1 7 9 5\n"
	   << "OPTIONS\n\t"
	   << "-h, --help\n\t\t"
	   << "Show this info and exit\n\t"
	   << "-a, --algorithm\n\t\t"
	   << "Set sorting algorithm. Available values are quicksort, mergesort and heapsort\n"
	   << std::endl;
	return ss.str();
}

int main(int argc, char **argv) {
	std::string algorithm();
	LinkedListSequence<double> *list = new LinkedListSequence<double>();
	for(unsigned i = 1; i < argc; i++) {
		if (argv[i] == "-h" || argv[i] == "--help") {
			std::cout << getHelpInfo(argv[0]);
			return 0;
		} else if ((argv[i] == "-a"
			    || argv[i] == "--algorithm")
			  && i < agvc - 1) {
		       algorithm = argv[i++];
		} else {
			try {
				list->prepend(stod(argv[i]));
			} catch(std::exception &unused) {
				std::cout << argv[0] << ": invalid input\nFor details look " << argv[0] << " -h\n";
				return 0;
			}
		}
	}
	sem3::Sorting<double> *sorting = getSorting<double>(algorithm);
	double *array = list->getItems(); 
	std::cout << "your array: ";
	print(array, list->getLength());
	sorting->sort(array, list->getLength());
	print(array, list->getLength());
}
