// This is general implementation of k-dimensional tree with support of a custom stored points

#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <vector>

// @param T is a type of a stored point
// T supports operator[]
// T[i] should support <, <=, >, >=

template <class T>
class kdtree {
private:
    class node {
        friend class kdtree;

        node *left_;
        node *right_;
        const T &point_;
        size_t axis_;

        node(const T &point, node *left, node *right, size_t axis)
            : point_(point), left_(left), right_(right), axis_(axis) {};

        ~node() {
            if (left_ != nullptr) delete left_;
            if (right_ != nullptr) delete right_;
        };
    };

    node *root_;
    size_t size_;

    void insert_recursive(const T &point, node *&dest, size_t cur_dim);
    void search_recursive(node *start, std::vector<T> &res, const T &lower_bound,
                          const T &upper_bound);

public:
    kdtree() : root_(nullptr), size_(0) {};
    kdtree(const std::vector<T> &points);
    ~kdtree();

    size_t size();
    void erase(node *dest);
    void insert(const T &point);

    // return points in a k-dimensional rectangle
    std::vector<T> query_search(const T &lower_bound, const T &upper_bound);
};

template <class T>
std::vector<T> kdtree<T>::query_search(const T &lower_bound, const T &upper_bound) {
    std::vector<T> res;
    search_recursive(root_, res, lower_bound, upper_bound);
    return res;
}

template <class T>
void kdtree<T>::search_recursive(node *start, std::vector<T> &res, const T &lower_bound,
                                 const T &upper_bound) {
    if (start == nullptr) return;

    size_t cur_axis = start->axis_;
    auto cur_val = start->point_[cur_axis];
    if (lower_bound[cur_axis] <= cur_val and cur_val <= upper_bound[cur_axis])
        res.push_back(start->point_);

    if (lower_bound[cur_axis] <= cur_val)
        search_recursive(start->left_, res, lower_bound, upper_bound);
    
    if (cur_val <= upper_bound[cur_axis])
        search_recursive(start->right_, res, lower_bound, upper_bound);
}

template <class T>
size_t kdtree<T>::size() {
    return size_;
}

template <class T>
kdtree<T>::kdtree(const std::vector<T> &points) : kdtree<T>() {
    for (const auto &p : points) {
        insert(p);
    }
}

template <class T>
void kdtree<T>::insert(const T &point) {
    size_t cur_dim = 0;
    insert_recursive(point, root_, cur_dim);
    size_++;
}

template <class T>
void kdtree<T>::insert_recursive(const T &point, kdtree<T>::node *&dest, size_t cur_dim) {
    if (dest == nullptr)
        dest = new node(point, nullptr, nullptr, cur_dim + 1);

    else if (point[cur_dim] < dest->point_[cur_dim]) {
        if (dest->left_ == nullptr) {
            dest->left_ = new node(point, nullptr, nullptr, (cur_dim + 1) % point.dim);
        } else
            insert_recursive(point, dest->left_, (cur_dim + 1) % point.dim);
    } else {
        if (dest->right_ == nullptr) {
            dest->right_ = new node(point, nullptr, nullptr, cur_dim + 1);
        } else
            insert_recursive(point, dest->right_, cur_dim + 1);
    }
}

template <class T>
void kdtree<T>::erase(typename kdtree<T>::node *dest) {
    delete dest;
}

template <class T>
kdtree<T>::~kdtree() {
    erase(root_);
}

#endif  // KDTREE_H
