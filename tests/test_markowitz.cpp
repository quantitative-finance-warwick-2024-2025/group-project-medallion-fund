# define CATCH_CONFIG_MAIN
# include "catch.hpp"
# include <iostream>
# include "specialised_agents.hpp"

# define EPSILON 1e-4

std::vector<double> bonds = {0.01, 0.02, 0.005, 0.04, 0.01, 0.02, 0.01, 0.015, 0.02};
std::vector<double> stock1 = {100, 101, 102, 99, 105, 75, 70, 90, 63};
std::vector<double> stock2 = {50, 49, 49, 55, 56, 68, 71, 82, 59};

TEST_CASE("MarkowitzSavvy single asset", "[tests]")
{   
    MarkowitzSavvy marko_man_single_asset(1, 6, 3, 0.03);

    // Testing that the initial position is correct
    marko_man_single_asset.setup(bonds[0], {stock1[0]});
    double w0 = marko_man_single_asset.current_wealth();
    REQUIRE(w0 == 1.0);
    
    // Should invest in bonds
    marko_man_single_asset.next_step(bonds[1], {stock1[1]});
    double w1 = marko_man_single_asset.current_wealth();
    REQUIRE(w1 == 1*(1 + 0.01));

    // Still should invest in bonds
    marko_man_single_asset.next_step(bonds[2], {stock1[2]});
    double w2 = marko_man_single_asset.current_wealth();
    REQUIRE(w2 == w1*(1 + 0.02));
    
    // Markowitz time
    marko_man_single_asset.next_step(bonds[3], {stock1[3]});
    double w3 = marko_man_single_asset.current_wealth();
    REQUIRE(w3 == Approx(17126469.0/20000000.0).epsilon(EPSILON));

    // Testing that when risk free rate is higher than target return, the agent will invest in bonds
    marko_man_single_asset.next_step(bonds[4], {stock1[4]});
    double w4 = marko_man_single_asset.current_wealth();
    REQUIRE(w4 == Approx(w3 * 1.04).epsilon(EPSILON));

    // Testing bankruptcy
    marko_man_single_asset.next_step(bonds[5], {stock1[5]});
    double w5 = marko_man_single_asset.current_wealth();
    REQUIRE(w5 == 0.0);

    // Testing that the agent stays bankrupt
    marko_man_single_asset.next_step(bonds[6], {stock1[6]});
    double w6 = marko_man_single_asset.current_wealth();
    REQUIRE(w6 == 0.0);
}

TEST_CASE("MarkowitzSavvy two assets", "[tests]")
{   
    MarkowitzSavvy marko_man_two_assets(2, 8, 6, 0.03);
    
    // Testing that the initial position is correct
    marko_man_two_assets.setup(bonds[0], {stock1[0], stock2[0]});
    double w0 = marko_man_two_assets.current_wealth();
    REQUIRE(w0 == 1.0);

    // Should invest in bonds
    marko_man_two_assets.next_step(bonds[1], {stock1[1], stock2[1]});
    double w1 = marko_man_two_assets.current_wealth();
    REQUIRE(w1 == 1*(1 + 0.01));

    // Still should invest in bonds
    marko_man_two_assets.next_step(bonds[2], {stock1[2], stock2[2]});
    double w2 = marko_man_two_assets.current_wealth();
    REQUIRE(w2 == w1*(1 + 0.02));

    // Still invest in bonds
    marko_man_two_assets.next_step(bonds[3], {stock1[3], stock2[3]});
    double w3 = marko_man_two_assets.current_wealth();
    REQUIRE(w3 == w2*(1 + 0.005));

    // Still invest in bonds
    marko_man_two_assets.next_step(bonds[4], {stock1[4], stock2[4]});
    double w4 = marko_man_two_assets.current_wealth();
    REQUIRE(w4 == w3*(1 + 0.04));

    // Still invest in bonds
    marko_man_two_assets.next_step(bonds[5], {stock1[5], stock2[5]});
    double w5 = marko_man_two_assets.current_wealth();
    REQUIRE(w5 == w4*(1 + 0.01));

    // Markowitz time
    marko_man_two_assets.next_step(bonds[6], {stock1[6], stock2[6]});
    double w6 = marko_man_two_assets.current_wealth();
    REQUIRE(w6 == Approx(1.1201306814226928).epsilon(EPSILON));

    // Markowitz again
    marko_man_two_assets.next_step(bonds[7], {stock1[7], stock2[7]});
    double w7 = marko_man_two_assets.current_wealth();
    REQUIRE(w7 == Approx(1.2477316261560714).epsilon(EPSILON));

    // Markowitz again
    marko_man_two_assets.next_step(bonds[8], {stock1[8], stock2[8]});
    double w8 = marko_man_two_assets.current_wealth();
    REQUIRE(w8 == Approx(1.203032074867376).epsilon(EPSILON));
}