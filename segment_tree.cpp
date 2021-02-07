template<typename T, typename AssociativeOperation>
struct segment_tree {
    int SZ;
    T identity;
    AssociativeOperation TT;
    std::vector<T> data;

    segment_tree() {}

    segment_tree(int _SZ, T _identity, AssociativeOperation _TT)
            : SZ(_SZ), identity(_identity), TT(_TT) {
        data.resize(2 * SZ, identity);
    }

    // Returns the value at index `i`
    T operator[](int i) {
        assert(0 <= i && i < SZ);
        return data[SZ + i];
    }

    // Assigns value `v` at index `i`
    void assign(int i, T v) {
        assert(0 <= i && i < SZ);
        data[i += SZ] = v;
        for (i /= 2; i; i /= 2)
            data[i] = TT(data[2 * i], data[2 * i + 1]);
    }

    // Returns the result of a left fold of the elements at indices in `[first, last]` over TT
    T accumulate(int first, int last) {
        assert(0 <= first && last < SZ);
        last += 1;
        T left = identity, right = identity;
        for (first += SZ, last += SZ; first < last; first /= 2, last /= 2) {
            if (first & 1) left  = TT(left, data[first++]);
            if (last  & 1) right = TT(data[--last], right);
        }
        return TT(left, right);
    }
};

// ways to use

// 1
// normal additive seg tree
segment_tree<int, plus<int>> tree(SIZE, 0LL, plus<int>());

// 2
// use a lambda function inside main function
auto min_oper = [](int a,int b) { return min(a,b); };
segment_tree<int, decltype(min_oper)> tree(SIZE, MAX_INT, min_oper); 

// 3
// use a function defined globally
struct Node {
    int value;
};

Node combine(Node l, Node r) {
    return {l.value + r.value};
}

using comb_type = Node(*)(Node, Node);

segment_tree<Node, comb_type> tree(SIZE, Identity, combine);

// 4
// passing by reference
Node combine_ref(Node &l, Node &r) {
    return {l.value + r.value};
}

using comb_ref_type = Node(*)(Node &,Node &);
segment_tree<Node, comb_ref_type> tree(SIZE, Identity, combine_ref);
