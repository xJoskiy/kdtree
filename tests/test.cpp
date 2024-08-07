#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kdtree.h"

using namespace kdt;

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
    std::vector<point<int>> points{{1, 1}, {1, 2}, {2, 1}, {3, 3}};
    kdtree<point<int>> tree(points);
}

TEST(test_kdtree, test_size) {
    std::vector<point<int>> points{{1, 1}};
    kdtree<point<int>> tree(points);
    for (int i = 0; i < 10; i++) tree.insert(point<int>({i, -i}));

    ASSERT_EQ(11, tree.size());
}

TEST(test_kdtree, test_rect_query) {
    using enum rect<point<int>>::bound_type;

    std::vector<point<int>> points({{1, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 3}, {3, 2}, {3, 3}});
    kdtree<point<int>> tree(points);

    std::vector<rect<point<int>>::bound_type> lower_bound_type{kClosed, kOpen};
    std::vector<rect<point<int>>::bound_type> upper_bound_type{kOpen, kClosed};
    rect<point<int>> box({1, 1}, {3, 3});
    box.set_bound_type(lower_bound_type, upper_bound_type);

    std::vector<point<int>> expected{{1, 2}, {2, 2}, {2, 3}};
    auto res = tree.query_search(box);
    EXPECT_EQ(expected, res);
}

TEST(test_kdtree, test_as_vector) {
    kdtree<point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    std::vector<point<int>> expected{{4, 4}, {4, 6}, {4, 5}, {6, 4}, {6, 6}, {6, 5}, {5, 5}};
    auto res = tree.as_vector();
    EXPECT_EQ(expected, res);
}

TEST(test_kdtree, test_move_init) {
    kdtree<point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    kdtree<point<int>> another_tree = std::move(tree);
    std::vector<point<int>> expected_vec{};
    std::vector<point<int>> res_vec = tree.as_vector();
    EXPECT_EQ(expected_vec, res_vec);
    EXPECT_EQ(0, tree.size());
}

TEST(test_kdtree, test_move_assign) {
    kdtree<point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    kdtree<point<int>> another_tree;
    another_tree = std::move(tree);
    std::vector<point<int>> expected_vec{};
    std::vector<point<int>> res_vec = tree.as_vector();
    EXPECT_EQ(expected_vec, res_vec);
    EXPECT_EQ(0, tree.size());
}