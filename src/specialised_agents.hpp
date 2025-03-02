# pragma once
# include "general_agent.hpp"

// Invests only in bonds
class RiskHater : public Agent {
    public:
        RiskHater(unsigned int m);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};

// Invests equally in all assets
class NaiveInvestor : public Agent {
    public:
        NaiveInvestor(unsigned int m);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};

// Invests in assets based on Markowitz portfolio theory
// Invest in bonds for first lookback periods and then use Markowitz portfolio theory
// Will need to add a lot of extra variables to store the covariance matrix and expected returns
class MarkowitzSavvy : public Agent {
    private:
        unsigned int lookback;
    public:
        MarkowitzSavvy(unsigned int m, unsigned int l);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};