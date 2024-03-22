#ifndef MATVIEW_HPP
#define MATVIEW_HPP

template<typename T>
struct BaseMatrixView {
    BaseMatrixView(T* data, int n_rows, int n_cols)
        : data(data), n_rows(n_rows), n_cols(n_cols)
    {}

    T& operator()(int row, int col) {
        return data[row * n_cols + col]; 
    }

    const T& operator()(int row, int col) const { 
        return data[row * n_cols + col]; 
    }

    T* data;
    const int n_rows;
    const int n_cols;
};

using MatrixView = BaseMatrixView<float>;

#endif
