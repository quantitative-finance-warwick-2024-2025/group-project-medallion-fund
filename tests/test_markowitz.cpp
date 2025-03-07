# define CATCH_CONFIG_MAIN
# include "catch.hpp"
# include <iostream>
# include "specialised_agents.hpp"


MarkowitzSavvy marko_man_single_asset(1, 5, 3, 0.03);

std::vector<double> bonds = {0.01, 0.02, 0.005, 0.02, 0.00, 0.03};
std::vector<double> stock1 = {100, 101, 102, 99, 105, 106};
std::vector<double> stock2 = {50, 49, 49, 55, 56, 57};

TEST_CASE("MarkowitzSavvy initial position", "[tests]")
{
    marko_man_single_asset.setup(bonds[0], {stock1[0]});
    
    marko_man_single_asset.next_step(bonds[1], {stock1[1]});
    double w1 = marko_man_single_asset.current_wealth();

    marko_man_single_asset.next_step(bonds[2], {stock1[2]});
    double w2 = marko_man_single_asset.current_wealth();
    
    std::vector<std::vector<double>> positions = marko_man_single_asset.get_positions();
    Matrix matpos = Matrix(positions);
    std::cout << matpos << std::endl;
}