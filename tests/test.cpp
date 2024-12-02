#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kdtree.h"

using namespace kdt;

template <class T>
class Point : public std::vector<T> {
    size_t dim;

public:
    size_t GetDim() const {
        return dim;
    };

    Point(std::initializer_list<T> list) : std::vector<T>(list), dim(list.size()) {}

    Point(const std::vector<T>& vec) : std::vector<T>(vec), dim(vec.size()) {}

    bool operator==(const Point& p) const {
        return static_cast<const std::vector<T>&>(p) == static_cast<const std::vector<T>&>(*this);
    }
};

TEST(TestKDTree, TestInsertion) {
    std::vector<Point<int>> points{{1, 1}, {1, 2}, {2, 1}, {3, 3}};
    KDTree<Point<int>> tree(points);
}

TEST(TestKDTree, TestSize) {
    std::vector<Point<int>> points{{1, 1}};
    KDTree<Point<int>> tree(points);
    for (int i = 0; i < 10; i++) tree.Insert(Point<int>({i, -i}));

    ASSERT_EQ(11, tree.Size());
}

TEST(TestKDTree, TestRectQuery) {
    using enum Rect<Point<int>>::bound_type;

    std::vector<Point<int>> points({{1, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 3}, {3, 2}, {3, 3}});
    KDTree<Point<int>> tree(points);

    std::vector<Rect<Point<int>>::bound_type> lower_bound_type{kClosed, kOpen};
    std::vector<Rect<Point<int>>::bound_type> upper_bound_type{kOpen, kClosed};
    Rect<Point<int>> box({1, 1}, {3, 3});
    box.SetBoundType(lower_bound_type, upper_bound_type);

    std::vector<Point<int>> expected{{1, 2}, {2, 2}, {2, 3}};
    auto res = tree.QuerySearch(box);
    EXPECT_EQ(expected, res);
}

TEST(TestKDTree, TestAsVector) {
    KDTree<Point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    std::vector<Point<int>> expected{{4, 4}, {4, 6}, {4, 5}, {6, 4}, {6, 6}, {6, 5}, {5, 5}};
    auto res = tree.AsVector();
    EXPECT_EQ(expected, res);
}

TEST(TestKDTree, TestMoveInit) {
    KDTree<Point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    KDTree<Point<int>> another_tree = std::move(tree);
    std::vector<Point<int>> expected_vec{};
    std::vector<Point<int>> res_vec = tree.AsVector();
    EXPECT_EQ(expected_vec, res_vec);
    EXPECT_EQ(0, tree.Size());
}

TEST(TestKDTree, TestMoveAssign) {
    KDTree<Point<int>> tree{{5, 5}, {4, 5}, {6, 5}, {4, 4}, {4, 6}, {6, 4}, {6, 6}};
    KDTree<Point<int>> another_tree;
    another_tree = std::move(tree);
    std::vector<Point<int>> expected_vec{};
    std::vector<Point<int>> res_vec = tree.AsVector();
    EXPECT_EQ(expected_vec, res_vec);
    EXPECT_EQ(0, tree.Size());
}