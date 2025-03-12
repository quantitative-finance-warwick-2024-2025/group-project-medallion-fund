#include "specialised_agents.hpp"
#include "matrix.hpp"

RiskHater::RiskHater(unsigned int m, unsigned int n) : Agent(m, n) {}

std::vector<double> RiskHater::initial_position()
{
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0; // invests 100% in bonds
    return position;
}

std::vector<double> RiskHater::update_position()
{
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0; // doesnt change position
    return position;
}

////////////////

NaiveInvestor::NaiveInvestor(unsigned int m, unsigned int n) : Agent(m, n) {}

std::vector<double> NaiveInvestor::initial_position()
{
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

std::vector<double> NaiveInvestor::update_position()
{
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

////////////////

MarkowitzSavvy::MarkowitzSavvy(unsigned int m, unsigned int n, unsigned int l, double r) : Agent(m, n), lookback(l),
                                                                                           target_return(r) {}

std::vector<double> MarkowitzSavvy::initial_position()
{
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0;
    return position;
}

std::vector<double> MarkowitzSavvy::update_position()
{
    // If not enough data to calculate Markowitz portfolio, invest in bonds
    if (current_period < lookback - 1 || lookback < 3)
    {
        std::vector<double> position(M + 1, 0.0);
        position[0] = 1.0;
        return position;
    }

    std::vector<std::vector<double>> returns = returns_matrix(past_asset_prices, current_period, lookback, M);
    std::vector<std::vector<double>> mean(1, mean_returns(returns)); // save as a vector of vectors for compatibility with matrix class
    std::vector<std::vector<double>> covariance = covariance_returns(returns);

    Matrix mean_matrix(mean);
    Matrix covariance_matrix(covariance);

    Matrix inverse_covariance = covariance_matrix.inverse();

    std::vector<std::vector<double>> ones_vector(1, std::vector<double>(M, 1.0));
    Matrix ones(ones_vector);

    Matrix A = ones * inverse_covariance * ones.T();
    Matrix B = ones * inverse_covariance * mean_matrix.T();
    Matrix C = mean_matrix * inverse_covariance * mean_matrix.T();

    double r = past_bond_returns[current_period];

    double temp_target_return;

    if (target_return <= r)
    {
        temp_target_return = r;
    }
    else
    {
        temp_target_return = target_return;
    }

    double factor = (temp_target_return - r) / (A(0, 0) * pow(r, 2) - 2 * B(0, 0) * r + C(0, 0));
    Matrix pos_matrix = factor * (inverse_covariance * (mean_matrix + (-r) * ones).T());

    std::vector<double> asset_position(M, 0.0);
    for (unsigned int i = 0; i < M; i++)
    {
        asset_position[i] = pos_matrix(i, 0);
    }

    double bond_position = 1.0; // Bond position is the remaining wealth
    for (unsigned int i = 0; i < M; i++)
    {
        bond_position -= asset_position[i];
    }

    std::vector<double> position(M + 1, 0.0); // Create actual position vector
    position[0] = bond_position;
    for (unsigned int i = 0; i < M; i++)
    {
        position[i + 1] = asset_position[i];
    }

    return position;
}

std::vector<std::vector<double>> returns_matrix(std::vector<std::vector<double>> &prices, unsigned int current_period,
                                                unsigned int lookback, unsigned int num_assets)
{
    std::vector<std::vector<double>> returns(lookback - 1, std::vector<double>(num_assets, 0.0));

    for (unsigned int i = 1; i < lookback; i++)
    {
        for (unsigned int j = 0; j < num_assets; j++)
        {
            returns[i - 1][j] = prices[current_period - lookback + 1 + i][j] / prices[current_period - lookback + i][j] - 1.0;
        }
    }
    return returns;
}

std::vector<double> mean_returns(std::vector<std::vector<double>> &returns)
{
    unsigned int lookbackM1 = returns.size(); // Lookback - 1
    unsigned int num_assets = returns[0].size();

    std::vector<double> mean(num_assets, 0.0);
    for (unsigned int i = 0; i < num_assets; i++)
    {
        for (unsigned int j = 0; j < lookbackM1; j++)
        {
            mean[i] += returns[j][i];
        }
        mean[i] /= lookbackM1;
    }
    return mean;
}

std::vector<std::vector<double>> covariance_returns(std::vector<std::vector<double>> &returns)
{
    unsigned int lookback = returns.size();
    unsigned int num_assets = returns[0].size();

    std::vector<double> mean = mean_returns(returns);

    std::vector<std::vector<double>> covariance(num_assets, std::vector<double>(num_assets, 0.0));
    for (unsigned int i = 0; i < num_assets; i++)
    {
        for (unsigned int j = 0; j < num_assets; j++)
        {
            for (unsigned int k = 0; k < lookback; k++)
            {
                covariance[i][j] += (returns[k][i] - mean[i]) * (returns[k][j] - mean[j]);
            }
            covariance[i][j] /= lookback - 1;
        }
    }
    return covariance;
}