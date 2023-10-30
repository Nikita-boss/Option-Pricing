// Payoff class encapsulates data such maturiy, strike price
// type (Call or Put) and exercise (European or American)

#ifndef PAYOFF_HPP
#define PAYOFF_HPP

namespace PricingLibrary {

	class Payoff
	{
	public:
		enum Type {Call=1, Put=-1};
		enum Exercise {European=1, American=2};

		Payoff(double maturity, double strike, Type type, Exercise exercise); // default constructor
		Payoff(const Payoff& source); // copy constructor
		Payoff& operator=(const Payoff& source); // copy assignemn
		~Payoff(); // destructor

		double getMaturity() const;	// get maturity
		double getStrike() const; // get strike price
		Type getType() const; // get type
		Exercise getExercise() const; // get exercise 

	private:
		double _maturity;	// expiration date in years
		double _strike;	// strike price
		Type _type;	// option type
		Exercise _exercise; // exercise type: european, american
	};
}

#endif