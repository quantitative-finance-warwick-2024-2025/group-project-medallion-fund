#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "general_agent.hpp"
#include "specialised_agents.hpp"


// 1. Test the Base Agent Class 

TEST_CASE("Agent base: setup & next_step with a bond-only (RiskHater) strategy, plus variance/sharpe") 
{
    // Use riskhater which invests 100% in bonds
    // M = 1 => 1 risky asset, N = 3 => total 3 periods => means 2 "steps".
    RiskHater agent(1, 3);

    // PERIOD 0 (setup)
    // Bond return = 1%, asset price = 100

    agent.setup(0.01, {100.0});
    REQUIRE(agent.current_wealth() == Approx(1.0)); // starts with wealth=1

    // PERIOD 1 (next_step)
  
    // Bond return = 2%, asset price = 110
    // RiskHater invests 100% in bond => new wealth = old_wealth*(1 + 0.01) = 1.01
    agent.next_step(0.02, {110.0});
    REQUIRE(agent.current_wealth() == Approx(1.01));

    // PERIOD 2 (next_step)
   
    // Bond return = 3%, asset price = 90
    // Again invests 100% in bond => new wealth = 1.01*(1 + 0.02) = 1.0302
    agent.next_step(0.03, {90.0});
    REQUIRE(agent.current_wealth() == Approx(1.0302).epsilon(1e-8));

 
    // Check total_return()

    // finalWealth=1.0506, initialWealth=1.0 => total_return(false, 0, 2) = 1.0302 - 1 = 0.0302
    REQUIRE(agent.total_return(false, 0, 2) == Approx(0.0302).epsilon(1e-6));

    // The period returns are:
    //   R0 = 1.01/1.0 - 1 = 0.01
    //   R1 = 1.0302/1.01 - 1 = 0.02
    // mean = (0.01 + 0.02)/2 = 0.015
    // variance = sum((Ri - mean)^2)/(n-1)
    //          = ((0.01-0.015)^2 + (0.02-0.015)^2)/1
    //          = (0.005^2 + 0.005^2) = 2*(2.5e-5) = 5e-5
    REQUIRE(agent.variance_return() == Approx(5e-5).epsilon(1e-8));

    // 3) Check sharpe_ratio()
    // Because this agent invests 100% in the bond, => Sharpe ~ 0.
    double sr = agent.sharpe_ratio();
    std::cout << "Sharpe ratio: " << sr << std::endl;
    REQUIRE(sr == Approx(0.0).epsilon(1e-8));
}

// 2. Test Each Specialized Agent's Position Logic and that positions sum to 1
// RiskHater => invests 100% in bond

TEST_CASE("RiskHater: initial & update positions") {
    RiskHater rh(2, 3);  // M=2 => positions of size 3

    auto pos_init = rh.initial_position();
    // Check size
    REQUIRE(pos_init.size() == 3);

    // Check each entry
    REQUIRE(pos_init[0] == Approx(1.0));
    REQUIRE(pos_init[1] == Approx(0.0));
    REQUIRE(pos_init[2] == Approx(0.0));

    // Check sum to 1
    double sum_init = 0.0;
    for (double p : pos_init) {
        sum_init += p;
    }
    REQUIRE(sum_init == Approx(1.0));

    // Now check update_position()
    auto pos_upd = rh.update_position();
    REQUIRE(pos_upd.size() == 3);
    REQUIRE(pos_upd[0] == Approx(1.0));
    REQUIRE(pos_upd[1] == Approx(0.0));
    REQUIRE(pos_upd[2] == Approx(0.0));

    // Check sum to 1
    double sum_upd = 0.0;
    for (double p : pos_upd) {
        sum_upd += p;
    }
    REQUIRE(sum_upd == Approx(1.0));
}


// NaiveInvestor => invests equally among bond + M assets
TEST_CASE("NaiveInvestor: initial & update positions") {
    
    NaiveInvestor naive(2, 3);

    auto pos_init = naive.initial_position();
    REQUIRE(pos_init.size() == 3);

    // Should be [1/3, 1/3, 1/3]
    REQUIRE(pos_init[0] == Approx(1.0/3.0));
    REQUIRE(pos_init[1] == Approx(1.0/3.0));
    REQUIRE(pos_init[2] == Approx(1.0/3.0));

    // Check sum to 1
    double sum_init = 0.0;
    for (double p : pos_init) {
        sum_init += p;
    }
    REQUIRE(sum_init == Approx(1.0));

    // update_position() => same
    auto pos_upd = naive.update_position();
    REQUIRE(pos_upd[0] == Approx(1.0/3.0));
    REQUIRE(pos_upd[1] == Approx(1.0/3.0));
    REQUIRE(pos_upd[2] == Approx(1.0/3.0));

    // Check sum to 1
    double sum_upd = 0.0;
    for (double p : pos_upd) {
        sum_upd += p;
    }
    REQUIRE(sum_upd == Approx(1.0));
}

// MarkowitzSavvy => invests 100% in bond for the first "lookback" periods, etc.
TEST_CASE("MarkowitzSavvy: initial & early update positions") {
    // For example, M=2, N=3, lookback=3, target_return=0.05
    MarkowitzSavvy ms(2, 3, 3, 0.05);

    // Period 0 => calls initial_position() =>  [1.0, 0.0, 0.0]
    auto pos_init = ms.initial_position();
    REQUIRE(pos_init.size() == 3);
    REQUIRE(pos_init[0] == Approx(1.0));
    REQUIRE(pos_init[1] == Approx(0.0));
    REQUIRE(pos_init[2] == Approx(0.0));

    // Check sum to 1
    double sum_init = 0.0;
    for (double p : pos_init) {
        sum_init += p;
    }
    REQUIRE(sum_init == Approx(1.0));

    // Suppose current_period < lookback => invests in bond
    auto pos_upd = ms.update_position();
    REQUIRE(pos_upd.size() == 3);
    REQUIRE(pos_upd[0] == Approx(1.0));
    REQUIRE(pos_upd[1] == Approx(0.0));
    REQUIRE(pos_upd[2] == Approx(0.0));

    // Check sum to 1
    double sum_upd = 0.0;
    for (double p : pos_upd) {
        sum_upd += p;
    }
    REQUIRE(sum_upd == Approx(1.0));
}

// Testing bankruptcy scenarios for each agent
/* 
  1) RiskHater Bankruptcy:
     Since RiskHater invests 100% in bond, 
     we can bankrupt it by giving a large negative bond return (e.g. -100%). 
*/
TEST_CASE("Bankruptcy: RiskHater with large negative bond return (code as-is)") {
    RiskHater agent(1, 3);

    // Put the -1.0 in setup => that means period0's bond return is -100%.
    agent.setup(-1.0, {100.0});
    REQUIRE(agent.current_wealth() == Approx(1.0));

    // Now, when we call next_step(...) the code uses bond_return from past_bond_returns[0]
    // => wealth => 1.0 * (1 + (-1.0)) => 0 => bankrupt
    agent.next_step(0.0, {50.0});  // This 0.0 won't matter this step
    REQUIRE(agent.current_wealth() == Approx(0.0));

    // Once bankrupt, remains bankrupt
    agent.next_step(0.5, {200.0});
    REQUIRE(agent.current_wealth() == Approx(0.0));
}

/* 
  2) NaiveInvestor Bankruptcy:
     NaiveInvestor invests equally among bond + M assets => 
     we can bankrupt it by giving a negative or zero bond return 
     plus a drastically negative asset price. 
*/
TEST_CASE("Bankruptcy: NaiveInvestor with large negative bond return (code as-is)") {
    // M=1 => invests 50% in bond, 50% in a single asset
    // N=3 => total 3 periods => 2 steps
    NaiveInvestor agent(1, 3);

    // 1) Period 0 (setup):
    //    - We set bond_return = -2.0 => past_bond_returns[0] = -2.0
    //    - wealth[0] = 1.0
    agent.setup(-2.0, {1.0});
    REQUIRE(agent.current_wealth() == Approx(1.0));

    // 2) Period 1 (next_step):
    //    - The code increments current_period to 1,
    //      then uses past_bond_returns[0] == -2.0 in the wealth formula.
    //
    //    - Because NaiveInvestor invests 50% in bond, 50% in asset:
    //         bond portion = 0.5 * wealth[0] * (1 + -2.0) = 0.5 * 1 * (-1) = -0.5
    //         asset portion = 0.5 * wealth[0] * (price=0.0) = 0.5 * 1 * 0 = 0
    //      => total new wealth = -0.5 => bankrupt => wealth[1] = 0
    agent.next_step(0.0, {1.0}); 
    REQUIRE(agent.current_wealth() == Approx(0.0));

    // 3) Period 2 (next_step):
    //    - Once bankrupt, the agent stays at 0 wealth
    agent.next_step(0.5, {2.0});
    REQUIRE(agent.current_wealth() == Approx(0.0));
}



/* 
  3) MarkowitzSavvy Bankruptcy:
     Markowitz invests 100% in bond if current_period < lookback. 
     We force negative bond returns to bankrupt it just like riskhater.
     OR skip ahead to current_period >= lookback if we want. 
     For simplicity, let's do negative bond return from start. 
*/
TEST_CASE("Bankruptcy: MarkowitzSavvy with large negative bond returns") {
    MarkowitzSavvy agent(2, 3, 3, 0.05);

    // Put -0.99 in setup => period0's bond return
    agent.setup(-0.99, {10.0, 10.0});
    REQUIRE(agent.current_wealth() == Approx(1.0));

    // 1st next_step => period=1 => uses past_bond_returns[0] = -0.99 => new wealth = 1*(1-0.99)=0.01
    agent.next_step(-2.0, {1.0, 1.0});
    REQUIRE(agent.current_wealth() == Approx(0.01).epsilon(1e-8));

    // 2nd next_step => period=2 => uses past_bond_returns[1] = -2.0 => new wealth = 0.01*(1-2)= -0.01 => bankrupt => 0
    agent.next_step(0.0, {0.5, 0.5});
    REQUIRE(agent.current_wealth() == Approx(0.0));
}


