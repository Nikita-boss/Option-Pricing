# Option-Pricing
Project to analytically price European Vanilla options and calculate Greeks.

# Code design
The codebase was designed to present a unified, well-structured, robust, and flexible solution to the option pricing project. The diagram below illustrates the UML class diagram.
![UML](https://github.com/Nikita-boss/Option-Pricing/assets/68224348/a56713cf-9343-4fe0-9b29-dd6011487c44)

We have a Payoff class that defines two Enumerate structures: Type either call or put and Exercise either European or American. The purpose of this class is to encapsulate all required option data that is not necessary to price options.
We have an abstract class Option from which we inherit two option classes: VanillaOption and ExoticOption. The VanillaOption class is used to determine European call and put option prices and Greeks, while the ExoticOption class to determine the price of the American perpetual options. This design choice is very robust as in the future it allows other types of options to be inherited from the Options class, such as Barrier options.
We have an abstract class PricingEngine from which we inherit three classes: AnalyticEuropeanEngine to analytically price European vanilla options and find corresponding Greeks, NumericalEuropeanEngine to numerically price European vanilla options and find corresponding delta and gamma Greeks, and AnalyticAmericanPerpetualEngine to analytically price American perpetual options.
Specific option objects would accept the Payoff object and appropriate Pricing Engine. Then the price of the option and Greeks can be found by calling appropriate functions which internally call the corresponding Pricing Engine function. This design choice is very flexible as it allows to pass other types of engines to price the same option, for example, Monte-Carlo or Finite Difference Method to price vanilla options. Furthermore, it allows to collect all types of options through a pointer to the base class and determine the price by calling the getPrice function, which internally will call the appropriate Pricing Engine function.
We created a general EngineException class and inherited IncorrectEngineException from it. We used it to throw an exception when AnalyticEuropeanEngine was passed to American vanilla options or when AnalyticAmericanPerpetualEngine was passed to European options. This robust design allows in the future to create new classes that would inherit from the EngineException class to define exceptions when dealing with other types of Pricing Engines.
Example of pricing European vanilla call option: Define type of the option: Call or Put: Payoff::Type type{Payoff::Call};
Define exercise type of the option: European:
Payoff::Exercise exercise(Payoff::European);
Create an AnalyticEuropeanEngine instance and pass underlying price, volatility, risk-free rate and cost of carry parameter:
auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(S, sig, r, b)};
Create a Payoff instance and pass expiration time in years, strike price, type of option (call or put) and exercise type of option (European or American):
auto payoff_call{std::make_shared<Payoff>(T_vector[i], K_vector[i], type, exercise)};
Define VanillaOption instance and pass Payoff object and Pricing Engine object:
VanillaOption myEuropeanCall{payoff_call, analytic_engine};
Get option price:
double price = myEuropeanCall.getPrice();

In general, we could extend our codebase by creating an abstract class Instrument and inheriting the Option’s class from it. This would allow other financial derivatives, such as futures, to be priced by passing a Pricing Engine to it.
For this project, we defined member functions for Greeks calculations in VanillaOption as it was not required for the American perpetual options. In the future, we could put them as pure virtual functions inside the Option base class and override in each inherited class.
Moreover, we encapsulated certain function inside Helper_functions namespace to help solve problems related to pricing options given vector or matrix of parameters. For example, we have print function to print option parameters along with its price or greeks, mesh function to return a vector of doubles with certain number of intervals, compute function that returns a matrix of option prices given a matrix of parameters.
Overall, we believe this design choice for the project provides a robust, flexible and well- structured solution to the option pricing task.
