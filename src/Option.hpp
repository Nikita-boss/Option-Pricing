// General option class from which specific option class are derived:
// VanillaOption or ExoticOption

#ifndef OPTION_HPP
#define OPTION_HPP

#include "Payoff.hpp"
#include <memory>

namespace PricingLibrary {

	class Option
	{
	protected:
		std::shared_ptr<Payoff> _payoff; // Payoff object

	public:
		explicit Option(const std::shared_ptr<Payoff>& payoff); // default constructor
		Option(const Option& source); // copy constructor
		Option& operator= (const Option& source); // copy assignemnt
		virtual ~Option(); // default destructor

		virtual double getPrice() const =0; // option price
	};
}

#endif