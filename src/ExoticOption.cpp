// Implementation of header file ExoticOption.hpp

#include "ExoticOption.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param payoff Payoff object
	ExoticOption::ExoticOption(const std::shared_ptr<Payoff>& payoff) :
					Option{payoff} {}

	/// @brief Construct with Payoff object and pricing engine
	/// @param payoff payoff object
	/// @param engine pricing engine object
	ExoticOption::ExoticOption(const std::shared_ptr<Payoff>& payoff,
								 const std::shared_ptr<PricingEngine>& engine) :
								Option{payoff}, _engine{engine} {}

	/// @brief Copy constructor
	/// @param source ExoticOption object
	ExoticOption::ExoticOption(const ExoticOption& source) : 
								 Option{source._payoff} 
	{
		if (source._engine != nullptr)
		{
			_engine = source._engine;
		}
	}

	/// @brief Copy assignemnt
	/// @param source ExoticOption object
	/// @return ExoticOption object
	ExoticOption& ExoticOption::operator= (const ExoticOption& source)
	{
		// self-assignment check
		if (this == &source)
		{
			return *this;
		}
		
		_payoff = source._payoff;
		_engine = source._engine;

		return *this;
	}

	/// @brief Default desctructor
	ExoticOption::~ExoticOption() {}

	/// @brief set pricing engine
	/// @param engine Pricing engine object
	void ExoticOption::setEngine(const std::shared_ptr<PricingEngine>& engine)
	{
		_engine = engine;
	}

	/// @brief option price
	/// @return price
	double ExoticOption::getPrice() const
	{
		return _engine->getEnginePrice(_payoff);
	}
}