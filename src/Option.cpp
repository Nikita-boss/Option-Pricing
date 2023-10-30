// Implementation of header file Option.hpp

#include "Option.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param payoff Payoff object
	Option::Option(const std::shared_ptr<Payoff>& payoff)
	: _payoff{payoff} {}

	/// @brief Copy constructor
	/// @param source Option Object
	Option::Option(const Option& source)
	: _payoff{source._payoff} {}

	/// @brief Copy assignemnt
	/// @param source Option Object
	/// @return Option Object
	Option& Option::operator= (const Option& source)
	{
		// check for self-assignment
		if (this == &source)
		{
			return *this;
		}

		_payoff = source._payoff;

		return *this;
	}

	/// @brief Default destructor
	Option::~Option() {}
}