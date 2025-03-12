# Comparative Analysis of Trading Strategies

This project implements an agent-based simulation framework in C++ to evaluate and compare various trading strategies using an object-oriented design. It integrates custom matrix operations and file I/O routines to simulate different investment approaches in a dynamic market environment (2018-2024).

## Overview

- **Investment Agents:**  
  The framework is built around a base class `general agent` that provides common functionality to all strategies. `specialised agents` then inherit from this base class to implement distinct investment strategies:   
  - `RiskHater:` Invests entirely in bonds for a low-risk, fixed-income approach.  
  - `NaiveInvestor:` Equally allocates funds across bonds and risky assets, providing a simple investment strategy.  
  - `MarkowitzSavvy:` Employs a Markowitz-inspired portfolio optimization technique using historical asset returns and covariance analysis to maximize risk-adjusted performance, with two variants targeting daily returns of `0.001 and 0.005`.

- **Matrix Operations:**  
  A custom `Matrix` class implements fundamental linear algebra operations—such as multiplication, inversion, transposition, and slicing—that are essential for efficient Markowitz portfolio optimization and seamless data flow management.

- **Data Handling:**  
  File I/O routines handle reading asset data from CSV files and writing simulation outputs—such as wealth evolution and cumulative returns—for each strategy. Asset data is read from the `Asset.CSV` file located in the `/src/datafiles` folder, which is generated using the DataGeneration Python file included in the source.


- **Testing and Build:**  
  Comprehensive tests—covering matrix operations, general agent functionality, and Markowitz optimization—ensure that each module behaves as expected. The project is built using CMake with provided guidelines, streamlining both compilation and testing.



## Usage

- To run the main program, execute **my_program** after building it with CMake.
- The program is implemented in **main.cpp** and performs the following tasks:
  - Reads market data from CSV files.
  - Instantiates multiple specialized agents ( RiskHater, NaiveInvestor, MarkowitzSavvy1, MarkowitzSavvy2).
  - Runs a simulation for each agent over multiple time periods.
  - Outputs performance metrics to the console and generates a CSV file (**Strategy_Output.csv**) with detailed results,enabling clear analysis of each investment strategy under controlled conditions.

- After running the main program, the provided Python Plotting file in the source folder was used for analysis and visualization.


## Submission Context

This repository forms the basis of our detailed report, "Comparative Analysis of Trading Strategies Using an Object-Oriented C++ Framework." It offers a high-level introduction to the simulation framework and its core components, with detailed discussions in the report.
