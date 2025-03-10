#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/matrix.hpp"  // Go up one folder (..) from tests into src to include matrix.hpp.


TEST_CASE("Matrix: Default constructor") {
    Matrix m;
    REQUIRE(m.NRows() == 0);
    REQUIRE(m.NCols() == 0);

    // Since matrix is empty (0x0) accessing (0,0) should throw
    REQUIRE_THROWS(m(0,0));
}

TEST_CASE("Matrix: (rows, cols) constructor") {
    Matrix m(3,4);
    REQUIRE(m.NRows() == 3);
    REQUIRE(m.NCols() == 4);

    // Check all initialized to 0
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 4; j++){
            REQUIRE(m(i,j) == 0.0);
        }
    }

    // Out-of-range 
    REQUIRE_THROWS(m(3,0));   // invalid row
    REQUIRE_THROWS(m(0,4));   // invalid col

    // Check read and write of matrix
    m(1,2) = 42.0;
    REQUIRE(m(1,2) == 42.0);
}

TEST_CASE("Matrix: vector-based constructor") {
    std::vector<std::vector<double>> data = {
        {1.0,  2.0,  3.0},
        {4.5,  5.5,  6.5}
    };
    Matrix m(data);
    REQUIRE(m.NRows() == 2);
    REQUIRE(m.NCols() == 3);
    REQUIRE(m(0,0) == 1.0);
    REQUIRE(m(0,1) == 2.0);
    REQUIRE(m(0,2) == 3.0);
    REQUIRE(m(1,0) == 4.5);
    REQUIRE(m(1,1) == 5.5);
    REQUIRE(m(1,2) == 6.5);

    // Check out of range throws
    REQUIRE_THROWS(m(2,0));
    REQUIRE_THROWS(m(0,3));
}

TEST_CASE("Matrix: operator* (multiplication) minimal test") {

    Matrix A({
        {1, 2},
        {3, 4}
    });
    Matrix B({
        {5, 6},
        {7, 8}
    });

    Matrix C = A * B;

    // Check dimensions
    REQUIRE(C.NRows() == 2);
    REQUIRE(C.NCols() == 2);

    // Checked by hand
    REQUIRE(C(0,0) == 19.0);
    REQUIRE(C(0,1) == 22.0);
    REQUIRE(C(1,0) == 43.0);
    REQUIRE(C(1,1) == 50.0);

    // Test dimension mismatch
    Matrix D({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });
    REQUIRE_THROWS(A * D);
}


TEST_CASE("Matrix: transpose") {
    Matrix M({
        {1, 2, 3},
        {4, 5, 6}
    });
    Matrix Mt = M.T();
    // Transpose should be 3x2
    REQUIRE(Mt.NRows() == 3);
    REQUIRE(Mt.NCols() == 2);

    REQUIRE(Mt(0,0) == 1.0);
    REQUIRE(Mt(1,0) == 2.0);
    REQUIRE(Mt(2,0) == 3.0);
    REQUIRE(Mt(0,1) == 4.0);
    REQUIRE(Mt(1,1) == 5.0);
    REQUIRE(Mt(2,1) == 6.0);

    // Even when shape doesnt change check for swap
    Matrix S({
        {1, 2},
        {3, 4}
    });
    Matrix S_t = S.T();
    REQUIRE(S_t.NRows() == 2);
    REQUIRE(S_t.NCols() == 2);

    REQUIRE(S_t(0,0) == 1.0);
    REQUIRE(S_t(0,1) == 3.0);
    REQUIRE(S_t(1,0) == 2.0);
    REQUIRE(S_t(1,1) == 4.0);
}

TEST_CASE("Matrix: inverse (2x2)") {

    Matrix M({
        {1, 2},
        {3, 4}
    });

    Matrix invM = M.inverse();
    REQUIRE(invM.NRows() == 2);
    REQUIRE(invM.NCols() == 2);

    // Inverse should be [[-2,1],[1.5, -0.5]]
    // verify by multiplying M*invM => Identity
    Matrix I = M * invM;
    REQUIRE(I.NRows() == 2);
    REQUIRE(I.NCols() == 2);

    REQUIRE(I(0,0) == Approx(1.0));
    REQUIRE(I(0,1) == Approx(0.0).margin(1e-12));
    REQUIRE(I(1,0) == Approx(0.0).margin(1e-12));
    REQUIRE(I(1,1) == Approx(1.0));
}

TEST_CASE("Matrix: inverse (non-square)") {
    Matrix rect(2,3);
    REQUIRE_THROWS(rect.inverse());
}

TEST_CASE("Matrix: inverse (singular)") {
    // e.g. second row is multiple of first => singular
    Matrix M({
        {1, 2},
        {2, 4}
    });
    REQUIRE_THROWS_WITH(M.inverse(), "Matrix is singular and cannot be inverted. Perhaps increase lookback?");
}

TEST_CASE("Matrix: returns() - normal") {
    // E.g 3 time steps (rows) for 2 assets (cols)

    Matrix prices({
        {100, 200},
        {110, 210},
        {121, 189}
    });
    Matrix ret = prices.returns();

    // returns will be 1 row less
    REQUIRE(ret.NRows() == 2);
    REQUIRE(ret.NCols() == 2);

    //ret computed by hand
    // e.g ret(0,0) => (110 - 100)/100 = 0.10

    REQUIRE(ret(0,0) == Approx(0.10));
    REQUIRE(ret(0,1) == Approx(0.05));
    REQUIRE(ret(1,0) == Approx(0.10));
    REQUIRE(ret(1,1) == Approx(-0.1));
}

TEST_CASE("Matrix: returns() - not enough data") {
    // Cannot compute returns with only 1 row
    Matrix singleRow({{100, 200}});
    REQUIRE_THROWS_WITH(singleRow.returns(),"Not enough data to calculate returns.");
}

TEST_CASE("Matrix: returns() - zero previous price") {
    Matrix hasZero({
        {100, 0},
        {110, 210}
    });
    // Division by zero
    REQUIRE_THROWS_WITH(hasZero.returns(), "Price zero : returns");
}

TEST_CASE("Matrix: slicerows") {
    // 5x3
    Matrix big({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10,11,12},
        {13,14,15}
    });
    // slice from row=1 to row=4 => that should include rows [1,2,3], total 3 rows
    Matrix sub = big.slicerows(1, 4);
    REQUIRE(sub.NRows() == 3);
    REQUIRE(sub.NCols() == 3);

    REQUIRE(sub(0,0) == 4);
    REQUIRE(sub(0,1) == 5);
    REQUIRE(sub(0,2) == 6);
    REQUIRE(sub(2,0) == 10);
    REQUIRE(sub(2,1) == 11);
    REQUIRE(sub(2,2) == 12);
}