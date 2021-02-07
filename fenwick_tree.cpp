// Fenwick tree
// Initializes a fenwick tree with array of indices [0, SZ-1]
// need to define a inverse operation as well when working range sum.

template<typename T, typename AssociativeOperation, typename InverseOperation = std::nullptr_t>
struct binary_indexed_tree {
    int SZ;
    T identity;
    AssociativeOperation TT;
    InverseOperation S;
    std::vector<T> data;

    binary_indexed_tree() {}

    binary_indexed_tree(int _SZ, T _identity, AssociativeOperation _TT, InverseOperation _S = nullptr)
            : SZ(_SZ), identity(_identity), TT(_TT), S(_S) {
        data.assign(2 * SZ, identity);
    }

    // Replaces the current value at index i with TT(current value, v)
    void add(int i, T v) {
        for (i++; i <= SZ; i += i & -i)
            data[i] = TT(data[i], v);
    }

    // Replaces the element at index i with v. Requires InverseOperation to be defined.
    void replace(int i, T v) {
        static_assert(!std::is_same<decltype(S), std::nullptr_t>::value, "InverseOperation not defined.");
        add(i, S(v, get(i)));
    }

    // Returns the value at index i. Requires InverseOperation to be defined.
    T get(int idx) const {
        static_assert(!std::is_same<decltype(S), std::nullptr_t>::value, "InverseOperation not defined.");
        assert(0 <= idx && idx < SZ);
        return accumulate(idx, idx);
    }

    // Returns the result of accumulating the elements at indices in [0, len]
    T accumulate_prefix(int len) const {
        assert(0 <= len && len < SZ);
        len += 1; // custom change
        T res = identity;
        for (; len; len -= len & -len)
            res = TT(res, data[len]);
        return res;
    }

    // Returns the result of accumulating the elements at indices in [l, r]. Requires InverseOperation be defined.
    T accumulate(int l, int r) const {
        static_assert(!std::is_same<decltype(S), std::nullptr_t>::value, "InverseOperation not defined.");
        assert(0 <= l && l < SZ and 0 <= r && r < SZ);
        if(l > r) return identity;
        else if(l == 0) return accumulate_prefix(r);
        else return S(accumulate_prefix(r), accumulate_prefix(l - 1));
    }
};

// Usage

// 1 
// additive BIT tree
using fenwick_tree = binary_indexed_tree<int, plus<int>, minus<int> >;
fenwick_tree tree(SIZE, 0, plus<int>(), minus<int>());

// 2
// minimum tree 
// NOTE : there is no inverse operation defined here.
int min_oper(int a, int b) {
    return min(a, b);
}
using min_oper_type = int(*)(int, int);
binary_indexed_tree<int, min_oper_type> tree(SIZE, INT_MAX, min_oper);