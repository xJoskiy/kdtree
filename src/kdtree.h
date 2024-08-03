// This is general implementation of k-dimensional tree with support of a custom stored points

#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <vector>

// @param T is a type of a stored point
// T supports operator[]
// T[i] should support <, <=, >, >=

template <class point_type>
class kdtree {
private:
    class node {
        friend class kdtree;

        node *left_;
        node *right_;
        point_type point_;
        size_t axis_;

        node(const point_type &point, node *left, node *right, size_t axis)
            : point_(point), left_(left), right_(right), axis_(axis) {};

        ~node() {
            if (left_ != nullptr) delete left_;
            if (right_ != nullptr) delete right_;
        };
    };

    node *root_;
    size_t size_;

    template <typename T>
    void insert_recursive(T &&point, node *&dest, size_t cur_dim);

    void search_recursive(node *start, std::vector<point_type> &res, const point_type &lower_bound,
                          const point_type &upper_bound) const;

public:
    kdtree() : root_(nullptr), size_(0) {};
    kdtree(const std::vector<point_type> &points);
    ~kdtree();

    size_t size();
    void erase(node *dest);

    std::vector<point_type> as_vector() const;
    void add_subtree(node *start, std::vector<point_type> &res) const;

    // insertion may lead a tree to unbalanced state
    template <typename T>
    void insert(T &&point);

    // return points in a k-dimensional rectangle
    std::vector<point_type> query_search(const point_type &lower_bound,
                                         const point_type &upper_bound) const;
};

template <class point_type>
std::vector<point_type> kdtree<point_type>::as_vector() const {
    std::vector<point_type> res;
    add_subtree(root_, res);
    return res;
}

template <class point_type>
void kdtree<point_type>::add_subtree(node *start, std::vector<point_type> &res) const {
    if (start == nullptr) return;
    add_subtree(start->left_, res);
    add_subtree(start->right_, res);
    res.push_back(start->point_);
}

template <class point_type>
std::vector<point_type> kdtree<point_type>::query_search(const point_type &lower_bound,
                                                         const point_type &upper_bound) const {
    std::vector<point_type> res;
    search_recursive(root_, res, lower_bound, upper_bound);
    return res;
}

template <class point_type>
bool fits(const point_type &point, const point_type &l, const point_type &r) {
    for (size_t i = 0; i < point.get_dim(); i++) {
        if (point[i] < l[i] or point[i] > r[i]) return false;
    }

    return true;
}

template <class point_type>
void kdtree<point_type>::search_recursive(node *start, std::vector<point_type> &res,
                                          const point_type &lower_bound,
                                          const point_type &upper_bound) const {
    if (start == nullptr) return;

    size_t cur_axis = start->axis_;
    auto cur_val = start->point_[cur_axis];
    if (fits(start->point_, lower_bound, upper_bound)) res.push_back(start->point_);

    if (lower_bound[cur_axis] <= cur_val)
        search_recursive(start->left_, res, lower_bound, upper_bound);

    if (cur_val <= upper_bound[cur_axis])
        search_recursive(start->right_, res, lower_bound, upper_bound);
}

template <class point_type>
size_t kdtree<point_type>::size() {
    return size_;
}

template <class point_type>
kdtree<point_type>::kdtree(const std::vector<point_type> &points) : kdtree<point_type>() {
    for (const auto &p : points) {
        insert(p);
    }
}

template <class point_type>
template <class T>
void kdtree<point_type>::insert(T &&point) {
    size_t cur_dim = 0;
    insert_recursive(std::forward<T>(point), root_, cur_dim);
    size_++;
}

template <class point_type>
template <class T>
void kdtree<point_type>::insert_recursive(T &&point, kdtree<point_type>::node *&dest,
                                          size_t cur_dim) {
    if (dest == nullptr)
        dest = new node(std::forward<T>(point), nullptr, nullptr, cur_dim % point.get_dim());

    else if (point[cur_dim] <= dest->point_[cur_dim]) {
        if (dest->left_ == nullptr)
            dest->left_ = new node(std::forward<T>(point), nullptr, nullptr, (cur_dim + 1) % point.get_dim());
        else
            insert_recursive(std::forward<T>(point), dest->left_, cur_dim + 1);
    } else {
        if (dest->right_ == nullptr)
            dest->right_ = new node(std::forward<T>(point), nullptr, nullptr, (cur_dim + 1) % point.get_dim());
        else
            insert_recursive(std::forward<T>(point), dest->right_, cur_dim + 1);
    }
}

template <class point_type>
void kdtree<point_type>::erase(kdtree<point_type>::node *dest) {
    delete dest;
}

template <class point_type>
kdtree<point_type>::~kdtree() {
    erase(root_);
}

#endif  // KDTREE_H
