#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kdtree.h"

template <class T>
struct point : std::vector<T> {
    size_t dim;

    point(std::initializer_list<T> list) : std::vector<T>(list), dim(list.size()) {}
};

TEST(test_kdtree, test_insertion) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    tree.insert(point<int>({1, 2}));
    tree.insert(point<int>({2, 3}));
    tree.insert(point<int>({4, 7}));
}

TEST(test_kdtree, test_size) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    tree.insert(point<int>({1, 2}));
    tree.insert(point<int>({2, 3}));
    tree.insert(point<int>({4, 7}));

    ASSERT_EQ(9, tree.size());
}

// не все точки в диапазоне + ошибки valgrind + точки задваиваются
// чини :)

TEST(test_kdtree, test_query) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    tree.insert(point<int>({1, 2}));
    tree.insert(point<int>({2, 3}));
    tree.insert(point<int>({4, 7}));

    auto res = tree.query_search({1, 2}, {9, 6});
    for (auto x : res) {
        std::cout << x[0] << ' ' << x[1] << std::endl;
    }
}