#include <iostream>
#include <cmath>
#include <iomanip>

/*

TODO: Use array from either Numpy or Boost for better computation

*/


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

// Return as a pointer
double* calculate_array_options_price(double volatility[], double price, double strike, double time, double rate) {
    double* option_price = new double[sizeof(volatility)];

    for (int i = 0; i < sizeof(option_price); i++) {
        double d1 = (log(price / strike) + (rate + ((volatility[i] * volatility[i]) / 2.0) * time)) / (volatility[i] * sqrt(time));
        double d2 = d1 - (volatility[i] * sqrt(time));
        option_price[i] = ceil((normalCDF(d1) * price - (normalCDF(d2) * strike * exp(-rate * time))) * 100.0) / 100.0;
    }

    return option_price;
}

// Difference between the call option price and calculated price
double* minimized_function(double volatility[], double price, double strike, double time, double rate, double value) {
    double* ptr = calculate_array_options_price(volatility, price, strike, time, rate);
    double* difference = new double[sizeof(volatility)];

    for (int i = 0; i < sizeof(difference); i++) {
        difference[i] = value - ptr[i];
    }

    delete[] ptr;

    return difference;
}

// Used secant method to calculate the implied volatility
double calculate_implied_volatility_secant(double price, double strike, double time, double rate, double value) {
    double h = 0.01;
    double guess[100];
    for (int i = 0; i < sizeof(guess); i++) {
        guess[i] = i / 100.0;
    }

    double guess_plus_h[100] = {0};
    for (int i = 0; i < sizeof(guess); i++) {
        guess_plus_h[i] = guess[i] + h;
    }

    double guess_minus_h[100] = {0};
    for (int i = 0; i < sizeof(guess); i++) {
        guess_minus_h[i] = guess[i] - h;
    }

    double* add = minimized_function(guess_plus_h, price, strike, time, rate, value);
    double* minus = minimized_function(guess_minus_h, price, strike, time, rate, value);
    double m[sizeof(guess)] = {0};
    for (int i = 0; i < sizeof(guess); i++) {
        m[i] = (add[i] - minus[i]) / (2 * h);
    }
    delete[] add;
    delete[] minus;

    // Convert to arrays
    double* min = minimized_function(guess, price, strike, time, rate, value);
    double impl_vol[sizeof(guess)] = {0};
    for (int i = 0; i < sizeof(guess); i++) {
        impl_vol[i] = guess[i] - min[i]/m[i];
    }
    delete[] min;

    double error = 1;

    while (true) {
        for (int i = 0; i < sizeof(impl_vol); i++) {
            guess[i] = impl_vol[i];

            double guess_plus_h[100] = {0};
            for (int i = 0; i < sizeof(guess); i++) {
                guess_plus_h[i] = guess[i] + h;
            }

            double guess_minus_h[100] = {0};
            for (int i = 0; i < sizeof(guess); i++) {
                guess_minus_h[i] = guess[i] - h;
            }

            double* add = minimized_function(guess_plus_h, price, strike, time, rate, value);
            double* minus = minimized_function(guess_minus_h, price, strike, time, rate, value);
            for (int i = 0; i < sizeof(guess); i++) {
                m[i] = (add[i] - minus[i]) / (2 * h);
            }
            delete[] add;
            delete[] minus;

            double* min = minimized_function(guess, price, strike, time, rate, value);
            double impl_vol[sizeof(guess)] = {0};
            for (int i = 0; i < sizeof(guess); i++) {
                impl_vol[i] = guess[i] - min[i]/m[i];
            }
            delete[] min;
        }

        for (int i = 0; i < sizeof(impl_vol); i++) {
            if (abs(impl_vol[i] - guess[i]) < h) {
                return impl_vol[i];
            }
        }
    }

    return 0;
}

// Used Newton's method to calculate the implied volatility
// double calculate_implied_volatility_newtons(double price, double strike, double time, double rate, double value) {
//     double h = 0.01;
//     // TODO: Use a grid/vector/matrix of initial guesses
//     double guess = 0.2;
//     double m = (minimized_function(guess + h, price, strike, time, rate, value) - minimized_function(guess, price, strike, time, rate, value)) / h;
//     double impl_vol = guess - minimized_function(guess, price, strike, time, rate, value)/m;

//     while (abs((impl_vol-guess)/impl_vol) > h) {
//         double guess = impl_vol;
//         double m = (minimized_function(guess + h, price, strike, time, rate, value) - minimized_function(guess, price, strike, time, rate, value)) / h;
//         double impl_vol = guess - minimized_function(guess, price, strike, time, rate, value)/m;
//     }

//     return impl_vol;
// }





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

    volatility = 0.15;
    price_of_asset = 300.0;
    strike_price = 250.0;
    time_exp = 1.0;
    interest_rate = 0.03;
    option_price = 58.82;

    if (choice == 1) {
        // std::cout << "Please enter the volatility as a percent: ";
        // std::cin >> volatility;
        // volatility /= 100.0;

        // std::cout << "Please enter the price of the asset: ";
        // std::cin >> price_of_asset;

        // std::cout << "Please enter the strike price: ";
        // std::cin >> strike_price;

        // std::cout << "Please enter the time until expiration in years: ";
        // std::cin >> time_exp;

        // std::cout << "Please enter the risk-free interest rate as a percent: ";
        // std::cin >> interest_rate;
        // interest_rate /= 100.0;

        std::cout << "The implied volatility is " << calculate_implied_volatility_secant(price_of_asset, strike_price, time_exp, interest_rate, option_price) << std::endl;
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
