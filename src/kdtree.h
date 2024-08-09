// This is general implementation of k-dimensional tree with support of a custom stored points

#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <vector>

namespace kdt {

template <typename point_type>
class kdtree;

// @brief k-dimensional rectangle used for query search
template <typename point_type>
struct rect {
    enum bound_type { kOpen = 0, kClosed };

    point_type lower_bound_, upper_bound_;
    std::vector<bound_type> lower_bound_type_, upper_bound_type_;

    rect() = default;
    rect(const rect<point_type>&) = default;
    rect(rect<point_type>&&) = default;
    rect<point_type>& operator=(const rect<point_type>&) = default;

    rect(const point_type& lower_bound, const point_type& upper_bound)
        : lower_bound_(lower_bound), upper_bound_(upper_bound) {
        lower_bound_type_ = std::vector<bound_type>(lower_bound.get_dim(), kOpen);
        upper_bound_type_ = std::vector<bound_type>(upper_bound.get_dim(), kOpen);
        assert(lower_bound_.get_dim() == upper_bound_.get_dim());
    }

    void set_bound_type(std::vector<bound_type> lower_bound_type,
                        std::vector<bound_type> upper_bound_type) {
        assert(lower_bound_.get_dim() == upper_bound_type_.size());
        lower_bound_type_ = lower_bound_type;
        upper_bound_type_ = upper_bound_type;
    }

    bool fits(const point_type& point) const {
        assert(point.get_dim() == lower_bound_.get_dim());
        for (size_t i = 0; i < point.get_dim(); i++) {
            if (lower_bound_type_[i] == kOpen) {
                if (point[i] <= lower_bound_[i]) return false;
            } else if (point[i] < lower_bound_[i])
                return false;

            if (upper_bound_type_[i] == kOpen) {
                if (point[i] >= upper_bound_[i]) return false;
            } else if (point[i] > upper_bound_[i])
                return false;
        }

        return true;
    }
};

/* @tparam T is a type of a stored point,
** T should support operator[],
** T[i] should support <, <=, >, >=, ==
*/
template <typename point_type>
class kdtree {
private:
    class node {
        friend class kdtree;

        node* left_;
        node* right_;
        point_type point_;
        size_t axis_;

        node(const point_type& point, node* left, node* right, size_t axis)
            : point_(point), left_(left), right_(right), axis_(axis) {}

        ~node() {
            if (left_ != nullptr) delete left_;
            if (right_ != nullptr) delete right_;
        }

        node(node&&) = delete;
        node(const node&) = delete;
        node& operator=(const node&) = delete;
        node& operator=(node&&) = delete;
    };

    node* root_;
    size_t size_;

    template <typename T>
    void insert_recursive(T&& point, node*& dest, size_t cur_dim);

    void search_recursive(node* start, std::vector<point_type>& res,
                          const rect<point_type>& box) const;

public:
    kdtree() : root_(nullptr), size_(0){};

    // TODO: Create a balanced tree from a set points
    kdtree(const std::vector<point_type>& points);
    kdtree(const std::initializer_list<point_type>& points);
    kdtree& operator=(kdtree&&);
    kdtree(kdtree&& tree);
    ~kdtree();

    kdtree(const kdtree&) = delete;

    size_t size() const;
    void erase(node* dest);

    std::vector<point_type> as_vector() const;
    void add_subtree(node* start, std::vector<point_type>& res) const;

    // insertion may lead a tree to unbalanced state
    template <typename T>
    void insert(T&& point);

    // return points in a k-dimensional rectangle
    std::vector<point_type> query_search(const rect<point_type>& box) const;
};

template <typename point_type>
kdtree<point_type>& kdtree<point_type>::operator=(kdtree<point_type>&& tree) {
    root_ = tree.root_;
    tree.root_ = nullptr;
    tree.size_ = 0;
    return *this;
}

template <typename point_type>
kdtree<point_type>::kdtree(kdtree<point_type>&& tree) {
    root_ = tree.root_;
    tree.root_ = nullptr;
    tree.size_ = 0;
}

template <typename point_type>
std::vector<point_type> kdtree<point_type>::as_vector() const {
    std::vector<point_type> res;
    add_subtree(root_, res);
    return res;
}

template <typename point_type>
void kdtree<point_type>::add_subtree(node* start, std::vector<point_type>& res) const {
    if (start == nullptr) return;
    add_subtree(start->left_, res);
    add_subtree(start->right_, res);
    res.push_back(start->point_);
}

template <typename point_type>
std::vector<point_type> kdtree<point_type>::query_search(const rect<point_type>& box) const {
    std::vector<point_type> res;
    search_recursive(root_, res, box);
    return res;
}

template <typename point_type>
void kdtree<point_type>::search_recursive(node* start, std::vector<point_type>& res,
                                          const rect<point_type>& box) const {
    if (start == nullptr) return;

    size_t cur_axis = start->axis_;
    auto cur_val = start->point_[cur_axis];
    if (box.fits(start->point_)) res.push_back(start->point_);

    if (box.lower_bound_[cur_axis] <= cur_val) search_recursive(start->left_, res, box);

    if (cur_val <= box.upper_bound_[cur_axis]) search_recursive(start->right_, res, box);
}

template <typename point_type>
size_t kdtree<point_type>::size() const {
    return size_;
}

template <typename point_type>
kdtree<point_type>::kdtree(const std::vector<point_type>& points) : kdtree<point_type>() {
    for (const auto& p : points) {
        insert(p);
    }
}

template <typename point_type>
kdtree<point_type>::kdtree(const std::initializer_list<point_type>& points) : kdtree<point_type>() {
    for (const auto& p : points) {
        insert(p);
    }
}

template <typename point_type>
template <typename T>
void kdtree<point_type>::insert(T&& point) {
    size_t cur_dim = 0;
    insert_recursive(std::forward<T>(point), root_, cur_dim);
    size_++;
}

template <typename point_type>
template <typename T>
void kdtree<point_type>::insert_recursive(T&& point, kdtree<point_type>::node*& dest,
                                          size_t cur_dim) {
    cur_dim %= point.get_dim();
                                         
    if (dest == nullptr)
        dest = new node(std::forward<T>(point), nullptr, nullptr, cur_dim);

    else if (point[cur_dim] <= dest->point_[cur_dim]) {
        if (dest->left_ == nullptr)
            dest->left_ = new node(std::forward<T>(point), nullptr, nullptr,
                                   (cur_dim + 1) % point.get_dim());
        else
            insert_recursive(std::forward<T>(point), dest->left_, cur_dim + 1);
    } else {
        if (dest->right_ == nullptr)
            dest->right_ = new node(std::forward<T>(point), nullptr, nullptr,
                                    (cur_dim + 1) % point.get_dim());
        else
            insert_recursive(std::forward<T>(point), dest->right_, cur_dim + 1);
    }
}

template <typename point_type>
void kdtree<point_type>::erase(kdtree<point_type>::node* dest) {
    delete dest;
}

template <typename point_type>
kdtree<point_type>::~kdtree() {
    erase(root_);
}

}  // namespace kdt

#endif  // KDTREE_H
