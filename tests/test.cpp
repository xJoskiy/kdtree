#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kdtree.h"

template <class T>
class point : public std::vector<T> {
    size_t dim;

public:
    size_t get_dim() const {
        return dim;
    };

    point(std::initializer_list<T> list) : std::vector<T>(list), dim(list.size()) {}

    point(const std::vector<T>& vec) : std::vector<T>(vec), dim(vec.size()) {}

    bool operator==(const point& p) const {
        return static_cast<const std::vector<T>&>(p) == static_cast<const std::vector<T>&>(*this);
    }
};

TEST(test_kdtree, test_insertion) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}};  //, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
}

TEST(test_kdtree, test_size) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    point<int> p({2, 2});
    tree.insert(p);
    tree.insert(p);

    ASSERT_EQ(8, tree.size());
}

TEST(test_kdtree, test_root_query) {
    std::vector<point<int>> points;
    kdtree<point<int>> tree;
    point<int> p({2, 2});
    tree.insert(p);

    auto res = tree.query_search({1, 1}, {3, 3});
    std::vector<point<int>> expected{{2, 2}};
    EXPECT_EQ(expected, res);
}

TEST(test_kdtree, test_query) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    point<int> p({-24, 10});
    tree.insert(p);
    tree.insert(p);

    auto res = tree.query_search({1, 2}, {9, 6});
    std::vector<point<int>> expected{{2, 3}, {5, 4}, {7, 2}, {9, 6}};
    EXPECT_EQ(expected, res);
}

TEST(test_kdtree, test_as_vector) {
    std::vector<point<int>> points{{2, 3}, {4, 7}, {5, 4}, {7, 2}, {8, 1}, {9, 6}};
    kdtree<point<int>> tree(points);
    point<int> p({2, 2});
    tree.insert(p);
    tree.insert(p);

    auto res = tree.as_vector();
    std::vector<point<int>> expected{{2, 3}, {4, 7}, {5, 4}, {7, 2},
                                     {8, 1}, {9, 6}, {2, 2}, {2, 2}};
    EXPECT_EQ(expected, res);
}