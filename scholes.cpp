#include <iostream>
#include <cmath>
#include <iomanip>

double normalCDF(double x) {
    return erfc(-x / sqrt(2.0))/2.0;
}

// Used the Black-Scholes Formula to calculate the European Call Option
double calculate_options_price(double volatility, double price, double strike, double time, double rate) {
    double d1 = (log(price / strike) + (rate + ((volatility * volatility) / 2.0) * time)) / (volatility * sqrt(time));
    double d2 = d1 - (volatility * sqrt(time));

    double option_price =  normalCDF(d1) * price - (normalCDF(d2) * strike * exp(-rate * time));

    return ceil(option_price * 100.0) / 100.0;
}

// Difference between the call option price and calculated price
double minimized_function(double volatility, double price, double strike, double time, double rate, double value) {
    return value - calculate_options_price(volatility, price, strike, time, rate);
}

// Used secant method to calculate the implied volatility
double calculate_implied_volatility(double price, double strike, double time, double rate, double value) {
    double h = 0.01;
    double guess = 0.10;
    double m = (minimized_function(guess + h, price, strike, time, rate, value) - minimized_function(guess - h, price, strike, time, rate, value)) / (2 * h);
    double impl_vol = guess - minimized_function(guess, price, strike, time, rate, value)/m;

    while (abs(impl_vol-guess) > 0.01) {
        double guess = impl_vol;
        double m = (minimized_function(guess + h, price, strike, time, rate, value) - minimized_function(guess - h, price, strike, time, rate, value)) / (2 * h);
        double impl_vol = guess - minimized_function(guess, price, strike, time, rate, value)/m;
    }

    return impl_vol;
}





int main() {
    int choice;
    double volatility;
    double price_of_asset;
    double strike_price;
    double time_exp;
    double interest_rate;
    double option_price;

    std::cout << "What would you like to calculate?\n";
    std::cout << "1. Implied Volatility\n2. Option Price\n";
    // TODO: Add option to calculate Put option price

    std::cin >> choice;

    // volatility = 0.15;
    // price_of_asset = 300.0;
    // strike_price = 250.0;
    // time_exp = 1.0;
    // interest_rate = 0.03;
    // option_price = 58.82

    if (choice == 1) {
        std::cout << "Please enter the volatility as a percent: ";
        std::cin >> volatility;
        volatility /= 100.0;

        std::cout << "Please enter the price of the asset: ";
        std::cin >> price_of_asset;

        std::cout << "Please enter the strike price: ";
        std::cin >> strike_price;

        std::cout << "Please enter the time until expiration in years: ";
        std::cin >> time_exp;

        std::cout << "Please enter the risk-free interest rate as a percent: ";
        std::cin >> interest_rate;
        interest_rate /= 100.0;

        std::cout << "The implied volatility is " << calculate_implied_volatility(price_of_asset, strike_price, time_exp, interest_rate, option_price) << std::endl;
    }
    else if (choice == 2) {
        std::cout << "Please enter the volatility as a percent: ";
        std::cin >> volatility;
        volatility /= 100.0;

        std::cout << "Please enter the price of the asset: ";
        std::cin >> price_of_asset;

        std::cout << "Please enter the strike price: ";
        std::cin >> strike_price;

        std::cout << "Please enter the time until expiration in years: ";
        std::cin >> time_exp;

        std::cout << "Please enter the risk-free interest rate as a percent: ";
        std::cin >> interest_rate;
        interest_rate /= 100.0;

        std::cout << "The derived price of the European call option is $" << calculate_options_price(volatility, price_of_asset, strike_price, time_exp, interest_rate) << std::endl;
    }

    return 0;
}
